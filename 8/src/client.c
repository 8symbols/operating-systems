#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <unistd.h>
#include "connection.h"

int main() {
    struct sockaddr_un addr;
    int ret;
    int data_socket;
    char buffer[BUFFER_SIZE];

    /* Create local socket. */
    data_socket = socket(AF_UNIX, SOCK_SEQPACKET, 0);
    if (data_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(struct sockaddr_un));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);

    /* Connect socket to socket address */
    ret = connect(data_socket, (const struct sockaddr*) &addr, sizeof(struct sockaddr_un));
    if (ret == -1) {
        fprintf(stderr, "The server is down.\n");
        exit(EXIT_FAILURE);
    }

    /* Request result. */
    strcpy(buffer, "file.txt");
    ret = write(data_socket, buffer, strlen(buffer) + 1);
    if (ret == -1) {
        perror("write");
        exit(EXIT_FAILURE);
    }

    struct stat st;
    if (stat(buffer, &st) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }

    char *file = (char *) malloc(st.st_size * sizeof(char));
    /* Receive result. */
    if (read(data_socket, file, st.st_size) == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }
    printf("%.*s\n", (int) st.st_size, file);
    free(file);

    /* Close socket. */
    close(data_socket);
    exit(EXIT_SUCCESS);
}