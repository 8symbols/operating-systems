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
    uint16_t sum1 = 0;
    uint16_t sum2 = 0;

    for (int i = 1; i < argc; ++i) {
        FILE* fp = fopen(argv[i], "r");
        if (fp == NULL) {
            perror("fopen");
            return EXIT_FAILURE;
        }

        unsigned long size;
        while ((size = fread(buff, 1, BUFF_SIZE, fp)) != 0) {
            for (int j = 0; j < size; ++j) {
                sum1 = (sum1 + buff[j]) % 255;
                sum2 = (sum2 + sum1) % 255;
            }
        }

        if (fclose(fp) != 0) {
            perror("fclose");
            return EXIT_FAILURE;
        }
    }

    uint16_t checksum = (sum2 << 8) | sum1;
    printf("checksum: %u\n", checksum);
    printf("time: %lu microseconds\n", get_time() - timestamp);

    return 0;
}