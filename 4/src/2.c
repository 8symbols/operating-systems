#include <stdio.h>

int main() {
    char name[20];  // Array of char in a stack memory.
    puts("Enter name: ");
    scanf("%19s", name);  // Last one is for '\0'.
    printf("Hello,  %s.\n\n\tNice to see you.\n", name);

    puts("Enter name: ");
    scanf("%19s", name);  // Rewrite name.
    printf("Hello,  %s.\n\n\tNice to see you.\n", name);

    char *name1 = "Anna";  // Pointer to a string literal in a constant segment.
    char a_letter = name1[0];
    name1[0] = name1[3];  // Error (SIGSEGV): attempt to change read-only data.
    name1[3] = a_letter;  // Error (SIGSEGV): attempt to change read-only data.
    puts(name1);
    return 0;
}