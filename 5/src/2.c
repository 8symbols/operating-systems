#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <zconf.h>

#define FILENAME ("tmp.txt")
#define WRITES_COUNT (300 * 1000)

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
    char *buffer = "a";

    /* Directly. */
    unsigned long timestamp = get_time();
    int fd = creat(FILENAME, S_IWUSR);
    if (fd == -1) {
        return EXIT_FAILURE;
    }
    for (int i = 0; i < WRITES_COUNT; ++i) {
        if (write(fd, buffer, 1) != 1) {
            return EXIT_FAILURE;
        }
    }
    if (close(fd) == -1) {
        return EXIT_FAILURE;
    }
    printf("directly time: %lu microseconds\n", get_time() - timestamp);

    if (remove(FILENAME) == -1) {
        return EXIT_FAILURE;
    }

    /* Using stdio. */
    timestamp = get_time();
    FILE *f = fopen(FILENAME, "w");
    if (f == NULL) {
        return EXIT_FAILURE;
    }
    for (int i = 0; i < WRITES_COUNT; ++i) {
        if (fwrite(buffer, 1, 1, f) != 1) {
            return EXIT_FAILURE;
        }
    }
    if (fclose(f) == EOF) {
        return EXIT_FAILURE;
    }
    printf("stdio time: %lu microseconds\n", get_time() - timestamp);

    if (remove(FILENAME) == -1) {
        return EXIT_FAILURE;
    }

    return 0;
}
