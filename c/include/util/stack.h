#ifndef STACK_H
#define STACK_H

#include "multitype.h"

/**
 * Stack data structure implemented using linked list.
 */
typedef struct Node {
    MultiType value;
    struct Node *next;
} Node;

typedef struct Stack {
    Node *top;
} Stack;

Stack *stack_create(void);

void stack_push(Stack *stack, MultiType item);

MultiType stack_peek(Stack *stack);

MultiType stack_pop(Stack *stack);

bool stack_is_empty(Stack *stack);

void stack_print(Stack *stack);

void stack_free(Stack *stack);

#endif  // STACK_H