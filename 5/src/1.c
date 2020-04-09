#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <zconf.h>

#define FILENAME ("tmp.txt")
#define BUFFER_SIZE (300 * 1024)

char buffer[BUFFER_SIZE];

/**
 * Get current time from gettimeofday().
 * @return time in microseconds.
 */
unsigned long get_time() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000000 + tv.tv_usec;
}

int main() {
    /* File creation. */
    unsigned long timestamp = get_time();
    int fd = creat(FILENAME, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        return EXIT_FAILURE;
    }
    printf("creation time: %lu microseconds\n", get_time() - timestamp);

    /* Writing to file. */
    timestamp = get_time();
    if (write(fd, buffer, BUFFER_SIZE) != BUFFER_SIZE) {
        return EXIT_FAILURE;
    }
    printf("writing time: %lu microseconds\n", get_time() - timestamp);

    /* Erasing file contents. */
    timestamp = get_time();
    if (ftruncate(fd, 0) == -1) {
        return EXIT_FAILURE;
    }
    printf("erasing time: %lu microseconds\n", get_time() - timestamp);

    /* Closing file. */
    timestamp = get_time();
    if (close(fd) == -1) {
        return EXIT_FAILURE;
    }
    printf("closing time: %lu microseconds\n", get_time() - timestamp);

    /* Removing file. */
    timestamp = get_time();
    if (remove(FILENAME) == -1) {
        return EXIT_FAILURE;
    }
    printf("removing time: %lu microseconds\n", get_time() - timestamp);

    return 0;
}
