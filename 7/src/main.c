#include <stdio.h>
#include <stdint.h>

void change_case(uint8_t *string, size_t length) {
    uint64_t mask = 0;
    for (uint8_t byte = 0; byte < 8; ++byte) {
        mask |= (uint64_t) 1 << (byte * 8u + 5u);
    }

    string += length;
    size_t rest = length;

    while (rest >= 8) {
        *((uint64_t *) (string - rest)) ^= mask;
        rest -= 8;
    }
    while (rest >= 4) {
        *((uint32_t *) (string - rest)) ^= mask;
        rest -= 4;
    }
    while (rest != 0) {
        *(string - rest) ^= mask;
        --rest;
    }
}

int main() {
    uint8_t string[] = "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz";
    printf("input:  %s\n", string);
    change_case(string, sizeof(string) - 1);
    printf("output: %s\n", string);
    return 0;
}
