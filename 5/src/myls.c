#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include "file_status_printer.h"

#define MAX_PATH_LEN (2048)

char buffer[MAX_PATH_LEN];

int main(int argc, char *argv[]) {
    if (argc > 3 || (argc == 3 && strcmp(argv[1], "-l") != 0)) {
        fprintf(stderr, "Usage: %s [-l] [<pathname>]\n", argv[0]);
        return EXIT_FAILURE;
    }

    int is_printing_statuses = argc == 3
        || (argc == 2 && strcmp(argv[1], "-l") == 0);

    char *directory;
    if (argc == 3 || (argc == 2 && !is_printing_statuses)) {
        directory = argv[argc - 1];
    } else {
        if ((directory = getcwd(buffer, MAX_PATH_LEN)) == NULL) {
            perror("getcwd");
            return EXIT_FAILURE;
        }
    }

    DIR *dir = opendir(directory);
    if (dir == NULL) {
        perror("opendir");
        return EXIT_FAILURE;
    }

    // It's necessary for lstat() work.
    if (chdir(directory) == -1) {
        perror("chdir");
        return EXIT_FAILURE;
    }

    struct dirent *dirent;
    while ((dirent = readdir(dir)) != NULL) {
        printf("%s\n", dirent->d_name);
        if (is_printing_statuses) {
            if (print_file_status(dirent->d_name) == EXIT_FAILURE) {
                return EXIT_FAILURE;
            }
            printf("\n");
        }
    }

    if (closedir(dir) == -1) {
        perror("closedir");
        return EXIT_FAILURE;
    }

    return 0;
}
