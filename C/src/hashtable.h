#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>
#include "multitype.h"

typedef struct Entry {
    MultiType key;
    MultiType value;
    struct Entry* next;
} Entry;

typedef struct HashTable {
    int capacity;
    int size;
    Entry** array;
} HashTable;

extern HashTable* hashtable_create(int capacity);

extern void hashtable_resize(HashTable* h, int new_capacity);

extern void hashtable_set(HashTable* h, MultiType key, MultiType value);

extern bool hashtable_contains(HashTable* h, MultiType key);

extern MultiType hashtable_get(HashTable* h, MultiType key);

extern void hashtable_remove(HashTable* h, MultiType key);

extern MultiType* hashtable_keys(HashTable* h);

extern void hashtable_print(HashTable* h);

extern void hashtable_free(HashTable* h);

#endif // HASHTABLE_H