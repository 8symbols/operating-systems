#include <stdio.h>
#include <stdlib.h>
#include "file_status_printer.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <pathname>\n", argv[0]);
        return EXIT_FAILURE;
    }

    return print_file_status(argv[1]);
}
