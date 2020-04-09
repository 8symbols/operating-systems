#ifndef SRC__STACK_H_
#define SRC__STACK_H_

#include <stdlib.h>
#include <stdio.h>

typedef struct t_node {
    struct t_node *next;
    char *name;
    int depth;
} t_node;

typedef struct t_stack {
  t_node *head;
} t_stack;

t_node *push(t_stack *stack, const char *name, int depth);
void pop(t_stack *stack);
t_node *peek(const t_stack *stack);
void clear(t_stack *stack);

#endif  //SRC__STACK_H_
