#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <zconf.h>

#define BUFFER_SIZE (1024)

char buffer[BUFFER_SIZE];

int main(int argc, char *argv[]) {
    /* Arguments checking. */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s -<number> <pathname>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // atoi returns zero in the case of error.
    int lines = atoi(argv[1] + 1);  // "+ 1" is for '-' skipping.
    if (lines < 0) {
        fprintf(stderr, "Number of lines must be non-negative.\n");
        return EXIT_FAILURE;
    }

    int fd = open(argv[2], S_IRUSR);
    if (fd == -1) {
        perror("open");
        return EXIT_FAILURE;
    }

    /* Searching for the position of first character in the nth line
     * from the end or in the first line if there are less than n lines. */
    const int file_size = lseek(fd, 0, SEEK_END);
    int pos = file_size;
    int buffer_size = -1;
    int index = -1;

    while (lines != 0 && pos != 0) {
        buffer_size = (pos < BUFFER_SIZE) ? (pos) : (BUFFER_SIZE);
        pos -= buffer_size;
        lseek(fd, pos, SEEK_SET);

        if (read(fd, buffer, buffer_size) != buffer_size) {
            perror("read");
            return EXIT_FAILURE;
        }

        for (index = buffer_size - 1; index != -1; --index) {
            if (buffer[index] == '\n') {
                --lines;
                if (lines == 0) {
                    break;
                }
            }
        }

        ++index;
    }

    /* Printing everything starting from the found position. */
    if (buffer_size != -1) {
        printf("%.*s", buffer_size - index, buffer + index);
        pos = lseek(fd, 0, SEEK_CUR);

        while (pos != file_size) {
            if (read(fd, buffer, BUFFER_SIZE) != BUFFER_SIZE) {
                perror("read");
                return EXIT_FAILURE;
            }

            printf("%.*s", BUFFER_SIZE, buffer);
            pos += BUFFER_SIZE;
        }
    }

    /* Closing file. */
    if (close(fd) == -1) {
        perror("close");
        return EXIT_FAILURE;
    }

    return 0;
}
