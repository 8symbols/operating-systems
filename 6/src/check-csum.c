#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "crc-8.h"

#define BUFF_SIZE (4096)

uint8_t buff[BUFF_SIZE];

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        return EXIT_FAILURE;
    }

    FILE* fp_src = fopen(argv[1], "r");
    if (fp_src == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    FILE* fp_checksums = fopen("csum.out", "r");
    if (fp_src == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    unsigned long size;
    unsigned int num = 0;

    while ((size = fread(buff, 1, BUFF_SIZE, fp_src)) != 0) {
        uint8_t checksum = crc_8(buff, size);
        uint8_t expected;
        if (fscanf(fp_checksums, "%c", &expected) != 1) {
            perror("fscanf");
            return EXIT_FAILURE;
        }
        if (checksum != expected) {
            printf("Error: checksum %u: %c != %c\n", num, checksum, expected);
        }
        ++num;
    }

    if (fclose(fp_src) != 0) {
        perror("fclose");
        return EXIT_FAILURE;
    }

    if (fclose(fp_checksums) != 0) {
        perror("fclose");
        return EXIT_FAILURE;
    }

    return 0;
}