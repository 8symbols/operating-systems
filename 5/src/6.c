#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include "stack.h"

#define MAX_PATH_LEN (2048)

char buffer[MAX_PATH_LEN];

int is_directory(const char *file) {
    struct stat sb;
    if (lstat(file, &sb) == -1) {
        perror("lstat");
        return EXIT_FAILURE;
    }
    return (sb.st_mode & S_IFMT) == S_IFDIR;
}

int is_directory_exist(const char *directory) {
    DIR *dir = opendir(directory);
    if (dir == NULL) {
        return 0;
    }
    if (closedir(dir) == -1) {
        perror("closedir");
    }
    return 1;
}

int main(int argc, char *argv[]) {
    if (argc > 4 || (argc > 2 && strcmp(argv[1], "-maxdepth") != 0)) {
        fprintf(stderr, "Usage: %s [-maxdepth <number>] [<pathname>]\n",
                argv[0]);
        return EXIT_FAILURE;
    }

    int max_depth = -1;
    if (argc > 2) {
        // atoi returns zero in the case of error.
        max_depth = atoi(argv[2]);
        if (max_depth < 0) {
            fprintf(stderr, "Depth must be a non-negative integer.\n");
            return EXIT_FAILURE;
        }
    }

    char *directory;
    if (argc == 4 || argc == 2) {
        directory = argv[argc - 1];
    } else {
        if ((directory = getcwd(buffer, MAX_PATH_LEN)) == NULL) {
            perror("getcwd");
            return EXIT_FAILURE;
        }
    }

    if (!is_directory_exist(directory)) {
        fprintf(stderr, "Directory doesn't exist.\n");
        return EXIT_FAILURE;
    }

    printf("%s\n", directory);
    t_stack stack = { .head = NULL };
    if (max_depth != 0) {
        push(&stack, directory, 1);
    }
    int prev_depth = 0;

    while (peek(&stack) != NULL) {
        directory = peek(&stack)->name;
        int curr_depth = peek(&stack)->depth;
        printf("%*s%s\n", curr_depth, " ", directory);

        for (int i = curr_depth; i <= prev_depth; ++i) {
            if (chdir("..") == -1) {
                perror("chdir");
                clear(&stack);
                return EXIT_FAILURE;
            }
        }

        DIR *dir = opendir(directory);
        if (chdir(directory) == -1) {
            perror("chdir");
            prev_depth = curr_depth;
            pop(&stack);
            if (dir != NULL && closedir(dir) == -1) {
                perror("closedir");
            }
            continue;
        }
        pop(&stack);

        if (dir == NULL) {
            perror("opendir");
            prev_depth = curr_depth;
            continue;
        }

        struct dirent *dirent;
        while ((dirent = readdir(dir)) != NULL) {
            if (strcmp(dirent->d_name, ".") == 0
                    || strcmp(dirent->d_name, "..") == 0) {
                continue;
            }
            printf("%*s%s\n", curr_depth, "-", dirent->d_name);
            if (dirent->d_type == DT_UNKNOWN && is_directory(dirent->d_name)) {
                dirent->d_type = DT_DIR;
            }
            if (dirent->d_type == DT_DIR && max_depth != curr_depth) {
                push(&stack, dirent->d_name, curr_depth + 1);
            }
        }

        if (closedir(dir) == -1) {
            perror("closedir");
        }

        prev_depth = curr_depth;
    }

    return 0;
}
