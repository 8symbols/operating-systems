#include <stdio.h>
#include <unistd.h>
#include <memory.h>

#define MAX_SIZE (4096)
char buff[MAX_SIZE];

int main() {
    FILE *f = fopen("file.txt", "r");
    if (f == NULL) {
        perror("fopen");
        return 1;
    }
    int cycle_num = 0;

    while (1) {
        fseek(f, 0, SEEK_SET);
        int size = 0;
        while ((buff[size++] = fgetc(f)) != EOF) {
            if (size == MAX_SIZE) {
                fprintf(stderr, "size > MAX_SIZE\n");
                return 1;
            }
        }
        printf("%d) %.*s\n", cycle_num, size - 1, buff);
        ++cycle_num;
        memset(buff, 0, MAX_SIZE);
        sleep(1);
    }

    fclose(f);
    return 0;
}
