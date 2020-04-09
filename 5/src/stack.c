#include "stack.h"

char *copy_string(const char *src) {
    int size = 32;
    char *copy = (char *) malloc(sizeof(char) * size);
    if (copy == NULL) {
        perror("malloc");
        return NULL;
    }

    copy[0] = src[0];
    for (int i = 1; copy[i - 1] != '\0'; ++i) {
        if (i == size) {
            size = (size / 2) * 3;
            char *temp = (char *) realloc(copy, sizeof(char) * size);
            if (temp == NULL) {
                perror("realloc");
                free(copy);
                return NULL;
            }
            copy = temp;
        }
        copy[i] = src[i];
    }

    return copy;
}

t_node *create_node(const char *name, int depth) {
    t_node *node = (t_node *) malloc(sizeof(t_node));
    if (node == NULL) {
        perror("malloc");
        return NULL;
    }

    node->next = NULL;
    node->depth = depth;
    if ((node->name = copy_string(name)) == NULL) {
        free(node);
        return NULL;
    }

    return node;
}

void delete_node(t_node *node) {
    free(node->name);
    free(node);
}

t_node *push(t_stack *stack, const char *name, int depth) {
    t_node *node = create_node(name, depth);
    if (node == NULL) {
        return NULL;
    }

    node->next = stack->head;
    stack->head = node;

    return node;
}

void pop(t_stack *stack) {
    t_node *node = stack->head;
    stack->head = stack->head->next;
    delete_node(node);
}

t_node *peek(const t_stack *stack) {
    return stack->head;
}

void clear(t_stack *stack) {
    while (peek(stack) != NULL) {
        pop(stack);
    }
}