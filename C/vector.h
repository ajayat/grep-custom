#ifndef VECTOR
#define VECTOR

typedef struct Vector {
    int capacity;
    int size;     // invariant 0 <= size <= capacity
    int* data;    // tableau de taille capacity
} Vector;

#endif // VECTOR