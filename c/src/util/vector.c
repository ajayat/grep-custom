/**
 * Implements a one-way linked list, stack and vector data structures.
 */

#include "vector.h"

#include <math.h>  // ceil
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // memcpy

#include "hashtable.h"
#include "multitype.h"

const float VECTOR_GROWTH_FACTOR = 2;

static void vector_resize(Vector *v, int new_capacity)
{
    if (new_capacity < v->size) {
        fprintf(stderr, "Capacity must be greater than vector size.\n");
        exit(EXIT_FAILURE);
    }
    v->array = realloc(v->array, new_capacity * sizeof(MultiType));
    v->capacity = new_capacity;
}

Vector *vector_create(int capacity)
{
    if (capacity < 0) {
        fprintf(stderr, "Capacity must be a positive integer. \n");
        exit(EXIT_FAILURE);
    }
    Vector *v = (Vector *)malloc(sizeof(Vector));
    v->capacity = capacity;
    v->size = 0;
    v->array = calloc(capacity, sizeof(int));
    return v;
}

Vector *vector_from_array(MultiType *array, int size)
{
    Vector *v = vector_create(size);
    v->size = size;
    memcpy(v->array, array, size * sizeof(MultiType));
    return v;
}

void vector_push(Vector *v, MultiType elem)
{
    if (v->size == v->capacity)
        vector_resize(v, v->capacity * VECTOR_GROWTH_FACTOR);

    v->array[v->size++] = elem;
}

void vector_extend(Vector *v, Vector *other)
{
    for (int i = 0; i < other->size; i++)
        vector_push(v, other->array[i]);

    vector_free(other);
}

MultiType vector_pop(Vector *v)
{
    if (v->size == 0) {
        fprintf(stderr, "Vector is empty\n");
        exit(EXIT_FAILURE);
    }
    MultiType elem = v->array[--v->size];

    if (v->capacity > VECTOR_GROWTH_FACTOR * v->size)
        vector_resize(v, ceil(v->capacity / VECTOR_GROWTH_FACTOR));

    return elem;
}

void vector_print(Vector *v)
{
    printf("[");
    for (int i = 0; i < v->size; i++)
        printf("%s, ", multi_to_string(v->array[i]));
    printf("]\n");
}

inline bool vector_is_empty(Vector *v)
{
    return v->size == 0;
}

void vector_free(Vector *v)
{
    free(v->array);
    free(v);
}
