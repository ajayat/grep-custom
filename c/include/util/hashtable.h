#ifndef HASHTABLE_H
#define HASHTABLE_H

#include <stdbool.h>

#include "multitype.h"
#include "vector.h"

extern const float HASHTABLE_LOAD_FACTOR;
extern const float HASHTABLE_GROWTH_FACTOR;

/**
 * Key-value pair backed by a linked list.
 */
typedef struct Entry {
    MultiType key;
    MultiType value;
    struct Entry *next;
} Entry;

typedef struct HashTable {
    int capacity;
    int size;
    Entry **array;
} HashTable;

extern HashTable *hashtable_create(int capacity);

extern void hashtable_set(HashTable *h, MultiType key, MultiType value);

extern bool hashtable_contains(HashTable *h, MultiType key);

extern MultiType hashtable_get(HashTable *h, MultiType key);

extern HashTable *hashtable_get_or_create(HashTable *h, MultiType key);

extern void hashtable_remove(HashTable *h, MultiType key);

extern Vector *hashtable_to_vector(HashTable *h);

extern HashTable *hashtable_copy(HashTable *h);

extern void hashtable_update(HashTable *h, HashTable *other);

extern void hashtable_print(HashTable *h);

extern void hashtable_free(HashTable *h, bool deep);

/**
 * Set data type is an hashmap with keys equals to values.
 */
typedef HashTable Set;

#endif  // HASHTABLE_H
