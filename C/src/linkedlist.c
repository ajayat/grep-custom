/**
 * Implements a one-way linked list.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "multitype.h"
#include "hashtable.h"
#include "linkedlist.h"

List* list_create(MultiType value, List* next) 
{
    List* l = (List*)malloc(sizeof(List));
    l->current = value;
    l->next = next;
    return l;
}

List* list_append(List* l1, List* l2) 
{
    if (l1 == NULL) {
        return l2;
    }
    List* l = l1;
    while (l->next != NULL) {
        l = l->next;
    }
    l->next = l2;
    return l1;
}

List* list_copy(List* l) 
{
    if (l == NULL)
        return NULL;

    List* l_copy = list_create(l->current, NULL);
    List* current = l_copy;
    for (l = l->next; l != NULL; l = l->next) 
    {
        current->next = list_create(l->current, NULL);
        current = current->next;
    }
    return l_copy;
}


int list_length(List* l) 
{
    int len = 0;
    while (l != NULL) 
    {
        len++;
        l = l->next;
    }
    return len;
}

void list_free(List* l)
{
    while (l != NULL) 
    {
        List* p = l;
        l = l->next;
        multi_free(p->current);
        free(p);
    }
}

void list_print(List* l) 
{
    printf("[");
    while (l != NULL) 
    {
        printf("%s, ", multi_to_string(l->current));
        l = l->next;
    }
    printf("]\n");
}

Stack* stack_create(void) 
{
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->size = 0;
    s->head = NULL;
    return s;
}

Stack* stack_from_set(Set* set)
{
    Stack* s = stack_create();
    for (int i = 0; i < set->capacity; i++) 
    {
        for (Entry* e = set->array[i]; e != NULL; e = e->next) 
        {
            stack_push(s, e->key);
            s->size++;
        }
    }
    return s;
}

bool stack_is_empty(Stack* s) 
{
    return s->head == NULL;
}

void stack_push(Stack* s, MultiType value) 
{
    s->head = list_create(value, s->head);
    s->size++;
}

MultiType stack_top(Stack* s) 
{
    if (stack_is_empty(s)) {
        fprintf(stderr, "Error: stack is empty\n");
        exit(EXIT_FAILURE);
    }
    return s->head->current;
}

MultiType stack_pop(Stack* s) 
{
    MultiType value = stack_top(s);
    List* l = s->head;
    s->head = l->next;
    free(l);
    s->size--;
    return value;
}

void stack_free(Stack* s) 
{
    list_free(s->head);
    free(s);
}