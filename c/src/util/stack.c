/**
 * Implements a stack using linked list to avoid invalid references when
 * working with vectors (data may be reallocated).
 */
#include "stack.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "multitype.h"

/* Return an empty stack */
Stack *stack_create(void)
{
    Stack *stack = (Stack *)malloc(sizeof(Stack));
    stack->top = NULL;
    return stack;
}

void stack_push(Stack *stack, MultiType item)
{
    Node *new_node = (Node *)malloc(sizeof(Node));
    new_node->value = item;
    new_node->next = stack->top;
    stack->top = new_node;
}

MultiType stack_peek(Stack *stack)
{
    if (stack_is_empty(stack)) {
        fprintf(stderr, "Error: stack is empty\n");
        exit(EXIT_FAILURE);
    }
    return stack->top->value;
}

MultiType stack_pop(Stack *stack)
{
    MultiType item = stack_peek(stack);
    Node *tmp = stack->top;
    stack->top = stack->top->next;
    free(tmp);
    return item;
}

inline bool stack_is_empty(Stack *stack)
{
    return stack->top == NULL;
}

void stack_print(Stack *stack)
{
    printf("[");
    for (Node *e = stack->top; e != NULL; e = e->next)
        printf("%s, ", multi_to_string(e->value));
    printf("]\n");
}

void stack_free(Stack *stack)
{
    while (!stack_is_empty(stack))
        stack_pop(stack);
    free(stack);
}
