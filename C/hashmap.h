#ifndef HASHMAP
#define HASHMAP

#include <stdbool.h>
#include "multitype.h"

typedef struct Entry {
    MultiType key;
    MultiType value;
    struct Entry* next;
} Entry;

typedef struct HashMap {
    int size;
    int len;
    int (*hash_function)(MultiType);
    Entry** array;
} HashMap;

extern HashMap* hashmap(int capacity, int hash_function(MultiType));

extern void hashmap_put(HashMap* hashmap, MultiType key, MultiType value);

extern bool hashmap_contains(HashMap* hashmap, MultiType key);

extern MultiType hashmap_get(HashMap* hashmap, MultiType key);

extern void hashmap_remove(HashMap* hashmap, MultiType key);

extern void hashmap_print(HashMap* hashmap);

extern void hashmap_free(HashMap* hashmap);

#endif // HASHMAP