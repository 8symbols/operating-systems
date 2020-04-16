#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "timer.h"

#define BUFF_SIZE (4096)

uint8_t buff[BUFF_SIZE];

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <pathname>...\n", argv[0]);
        return EXIT_FAILURE;
    }

    unsigned long timestamp = get_time();
    uint8_t checksum = 0;

    for (int i = 1; i < argc; ++i) {
        FILE* fp = fopen(argv[i], "r");
        if (fp == NULL) {
            perror("fopen");
            return EXIT_FAILURE;
        }

        unsigned long size;
        while ((size = fread(buff, 1, BUFF_SIZE, fp)) != 0) {
            for (int j = 0; j < size; ++j) {
                checksum ^= buff[j];
            }
        }

        if (fclose(fp) != 0) {
            perror("fclose");
            return EXIT_FAILURE;
        }
    }

    printf("checksum: %u\n", checksum);
    printf("time: %lu microseconds\n", get_time() - timestamp);

    return 0;
}