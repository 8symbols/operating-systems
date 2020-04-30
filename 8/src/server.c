#include <aio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <fcntl.h>
#include <asm/errno.h>
#include "connection.h"

#define MAX_CBS_COUNT (64)

struct aiocb cbs[MAX_CBS_COUNT];
size_t cbs_count = 0;

int connection_socket;

void free_files() {
    for (int i = 0; i < cbs_count; ++i) {
        while (aio_error(cbs + i) == EINPROGRESS) {
            // Wait.
        }
        free(cbs[i].aio_buf);
    }
    cbs_count = 0;
}

void perform_exit(int exit_code) {
    close(connection_socket);
    unlink(SOCKET_NAME);
    free_files();
    exit(exit_code);
}

void handle_signal_action(int sig_number)
{
    if (sig_number == SIGINT) {
        perform_exit(EXIT_SUCCESS);
    }
    if (sig_number == SIGPIPE) {
        free_files();
    }
}

int setup_signals() {
    struct sigaction sa;
    sa.sa_handler = handle_signal_action;
    if (sigaction(SIGINT, &sa, 0) != 0) {
        perror("sigaction()");
        return -1;
    }
    if (sigaction(SIGPIPE, &sa, 0) != 0) {
        perror("sigaction()");
        return -1;
    }
    return 0;
}

int main() {
    if (setup_signals() != 0) {
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un name;
    int ret;
    char buffer[BUFFER_SIZE];

    /* Create local socket. */
    connection_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (connection_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&name, 0, sizeof(struct sockaddr_un));
    name.sun_family = AF_UNIX;
    strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

    /* Bind socket to socket name. */
    ret = bind(connection_socket, (const struct sockaddr*) &name, sizeof(struct sockaddr_un));
    if (ret == -1) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    /* Prepare for accepting connections. */
    ret = listen(connection_socket, 20);
    if (ret == -1) {
        perror("listen");
        perform_exit(EXIT_FAILURE);
    }

    fd_set active_fd_set, read_fd_set;
    FD_ZERO(&active_fd_set);
    FD_SET(connection_socket, &active_fd_set);

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
    while (1) {
        /* Block until input arrives on one or more active sockets. */
        read_fd_set = active_fd_set;
        if (select(FD_SETSIZE, &read_fd_set, NULL, NULL, NULL) < 0) {
            perror("select");
            perform_exit(EXIT_FAILURE);
        }

        /* Service all the sockets with input pending. */
        for (int i = 0; i < FD_SETSIZE; ++i) {
            if (FD_ISSET(i, &read_fd_set)) {
                if (i == connection_socket) {
                    /* Connection request on original socket. */
                    int new;
                    size_t size = sizeof(name);
                    new = accept(connection_socket, (struct sockaddr*) &name, &size);
                    if (new < 0) {
                        perror("accept");
                        perform_exit(EXIT_FAILURE);
                    }
                    fprintf(stderr, "Server: connection accepted.\n");
                    FD_SET(new, &active_fd_set);
                } else {
                    /* Data arriving on an already-connected socket. */
                    ret = read(i, buffer, BUFFER_SIZE);
                    if (ret < 0) {
                        perror("read");
                        perform_exit(EXIT_FAILURE);
                    } else if (ret == 0) {
                        close(i);
                        FD_CLR(i, &active_fd_set);
                    } else {
                        if (cbs_count == MAX_CBS_COUNT) {
                            free_files();
                        }

                        int fd = open(buffer, O_RDONLY);
                        if (fd == -1) {
                            perror("open");
                            perform_exit(EXIT_FAILURE);
                        }
                        struct stat st;
                        if (stat(buffer, &st) == -1) {
                            perror("stat");
                            perform_exit(EXIT_FAILURE);
                        }
                        char *file = (char *) malloc(st.st_size * sizeof(char));
                        if (file == NULL) {
                            perror("malloc");
                            perform_exit(EXIT_FAILURE);
                        }
                        read(fd, file, st.st_size);

                        struct aiocb *aiocb = cbs + cbs_count++;
                        memset(aiocb, 0, sizeof(struct aiocb));
                        aiocb->aio_fildes = i;
                        aiocb->aio_buf = file;
                        aiocb->aio_nbytes = st.st_size;
                        if (aio_write(aiocb) == -1) {
                            perror("aio_write");
                            perform_exit(EXIT_FAILURE);
                        }
                    }
                }
            }
        }
    }
#pragma clang diagnostic pop
}