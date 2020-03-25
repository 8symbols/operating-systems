#include <stdio.h>
#include <stdlib.h>

int main() {
    int *data = (int *) malloc(100 * sizeof(int));
    data[10] = 10;
    free(data);
    printf("%d\n", data[10]);
    return 0;
}