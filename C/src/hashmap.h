#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
#include "multitype.h"

typedef struct LinkedEntry {
    MultiType key;
    MultiType value;
    struct LinkedEntry* next;
} LinkedEntry;

typedef struct HashMap {
    int capacity;
    int size;
    LinkedEntry** array;
} HashMap;

extern HashMap* hashmap_create();

extern void hashmap_set(HashMap* hashmap, MultiType key, MultiType value);

extern bool hashmap_contains(HashMap* hashmap, MultiType key);

extern MultiType hashmap_get(HashMap* hashmap, MultiType key);

extern void hashmap_remove(HashMap* hashmap, MultiType key);

extern void hashmap_print(HashMap* hashmap);

extern void hashmap_free(HashMap* hashmap);

#endif // HASHMAP_H