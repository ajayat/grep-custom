#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stdbool.h>

#include "multitype.h"

/**
 * Linked list data structure.
 */
typedef struct List {
    MultiType current; 
    struct List* next; 
} List;

List* list_create(MultiType value, List* next);

int list_length(List* l);

List* list_copy(List* l);

List* list_append(List* l1, List* l2);

void list_print(List* l);

void list_free(List* l);

/**
 * Stack data structure backed by a linked list.
 */
typedef struct Stack {
    int size;
    List* head;
} Stack;

Stack* stack_create(void);

Stack* stack_from_array(MultiType* array, int size);

bool stack_is_empty(Stack* s);

void stack_push(Stack* s, MultiType value);

MultiType stack_top(Stack* s);

MultiType stack_pop(Stack* s);

void stack_free(Stack* s);

#endif // LINKEDLIST_H
