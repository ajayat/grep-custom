/**
 * Implements a dynamic array with growth factor of 2.
 */
#include "vector.h"

const int VECTOR_GROWTH_FACTOR = 2;

Vector* vector_create(int capacity) 
{
    Vector* v = malloc(sizeof(struct Vector));
    v->capacity = capacity;
    v->data = calloc(capacity, sizeof(int));
    v->size = 0;
    return v;
}

void vector_resize(Vector *v, int s) 
{
    assert(0 <= s);
    if (s > v->capacity) 
    {
        // il faut agrandir => on double la capacité
        v->capacity = 2 * v->capacity;
        if (v->capacity < s) { 
            v->capacity = s; 
        }
        int *old = v->data;
        v->data = calloc(v->capacity, sizeof(int));
        for (int i = 0; i < v->size; i++) {
            v->data[i] = old[i];
        }
        free(old);
    } 
    else if (s < v->capacity / 4) 
    {
        // on peut rétrécir => on divise par deux la capacité
        v->capacity = v->capacity / 2;
        int *old = v->data;
        v->data = calloc(v->capacity, sizeof(int));
        for (int i = 0; i < s; i++) {
            v->data[i] = old[i];
        }
        free(old);
    }
    v->size = s;
}

int vector_get(Vector *v, int i) 
{
    assert(0 <= i && i < v->size);
    return v->data[i];
}

void vector_set(Vector *v, int i, int x) 
{
    assert(0 <= i && i < v->size);
    v->data[i] = x;
}

void vector_push(Vector *v, int x) 
{
    int n = vector_size(v);
    vector_resize(v, n+1);
    vector_set(v, n, x);
}

int vector_top(Vector *v) 
{
    int n = vector_size(v) - 1;
    assert(0 <= n);
    return vector_get(v, n);
}

int vector_pop(Vector *v) 
{
    int n = vector_size(v) - 1;
    assert(0 <= n);
    int r = vector_get(v, n);
    vector_resize(v, n);
    return r;
}

void vector_delete(Vector *v) 
{
    free(v->data);
    free(v);
}