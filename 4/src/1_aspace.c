/* Example code for Think OS.

Copyright 2015 Allen Downey
License: Creative Commons Attribution-ShareAlike 3.0

*/

#include <stdio.h>
#include <stdlib.h>

int global;

int main() {
    int local = 5;  // Stack variable.
    void *p = malloc(128);  // Pointer to a data in a heap.

    printf("Address of main is %p\n", main);  // Address of main is 0x56083a054169
    printf("Address of global is %p\n", &global);  // Address of global is 0x56083a05704c
    printf("Address of local is %p\n", &local);  // Address of local is 0x7fff1e7bd1c4
    printf("Address of p is %p\n", p);  // Address of p is 0x56083a85a2a0

    void *new_p = malloc(128);
    printf("Address of new_p is %p\n", new_p);  // Address of new_p is 0x56083a85a740
    printf("Address of new local is %p\n", &new_p);  // Address of new local is 0x7fff1e7bd1c8
    free(p);
    free(new_p);

    return 0;
}