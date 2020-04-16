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

    FILE* fp_dst = fopen("csum.out", "w");
    if (fp_src == NULL) {
        perror("fopen");
        return EXIT_FAILURE;
    }

    unsigned long size;
    while ((size = fread(buff, 1, BUFF_SIZE, fp_src)) != 0) {
        uint8_t checksum = crc_8(buff, size);
        fprintf(fp_dst, "%c", checksum);
    }

    if (fclose(fp_src) != 0) {
        perror("fclose");
        return EXIT_FAILURE;
    }

    if (fclose(fp_dst) != 0) {
        perror("fclose");
        return EXIT_FAILURE;
    }

    return 0;
}