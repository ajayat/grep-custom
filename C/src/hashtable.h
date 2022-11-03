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

extern void hashtable_set(HashTable* h, MultiType key, MultiType value);

extern bool hashtable_contains(HashTable* h, MultiType key);

extern MultiType hashtable_get(HashTable* h, MultiType key);

extern MultiType hashtable_get_default(HashTable* h, MultiType key, 
                                       MultiType default_value);

extern void hashtable_remove(HashTable* h, MultiType key);

extern MultiType* hashtable_keys(HashTable* h);

extern HashTable* hashtable_copy(HashTable* h);

extern void hashtable_update(HashTable* h, HashTable* other);

extern void hashtable_print(HashTable* h);

extern void hashtable_free(HashTable* h);

/**
 * Double hashtable is a hashtable of hashtables.
 */
typedef HashTable DoubleHashTable;

extern HashTable* double_hashtable_get(DoubleHashTable* h, MultiType key);

/**
 * Set data type is an hashmap with keys equals to values.
 */
typedef HashTable Set;

extern void set_add(Set* s, MultiType elem);

extern void set_print(Set* s);

extern bool set_is_subset(Set* s1, Set* s2);

#endif // HASHTABLE_HS