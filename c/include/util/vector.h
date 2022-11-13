#ifndef VECTOR_H
#define VECTOR_H

#include "multitype.h"

extern const float VECTOR_GROWTH_FACTOR;

/**
 * Self-resizing array data structure.
 */
typedef struct Vector {
    int capacity;
    int size;
    MultiType* array;
} Vector;

Vector* vector_create(int capacity);

Vector* vector_from_array(MultiType* array, int size);

void vector_push(Vector* v, MultiType elem);

void vector_extend(Vector* v, Vector* other);

MultiType vector_pop(Vector* v);

void vector_remove(Vector* v, MultiType elem);

void vector_print(Vector* v);

void vector_free(Vector* v);

#endif  // VECTOR_H
