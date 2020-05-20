#include <unistd.h>
#include <stdio.h>

int main() {
    if (unlink("file.txt") != 0) {
        perror("unlink");
        return 1;
    }
    return 0;
}
