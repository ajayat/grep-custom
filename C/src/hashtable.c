/**
 * Implementation of a hashtable with:
 *    - Dynamic array (automatic resizing)
 *    - Separate chaining to resolve collision
 *    - Different type for values allowed (MultiType)
 */

#include <stdlib.h>
#include <stdbool.h>  // bool
#include <string.h>  // strcmp, strlen
#include <stdio.h>  // printf
#include <math.h>  // ceil

#include "hashtable.h"
#include "multitype.h"
#include "linkedlist.h"

const double LOAD_FACTOR = 0.75;
const int GROWTH_FACTOR = 2;

static inline int hash_int(int capacity, int key) 
{
    return (int)(capacity * key * 1.61803398875) % capacity;
}

static int hash_string(int capacity, char* key) 
{
    int hash = 0;
    for (int i = 0; i < (int)strlen(key); i++)
        hash = (hash*31 + (int)key[i]) % capacity;

    return hash;
}

static int bucket(int capacity, MultiType key) 
{
    switch (key.type) {
        case IntType:
            return hash_int(capacity, key.value.i);
        case CharType:
            return hash_int(capacity, (int)key.value.c);
        case StringType:
            return hash_string(capacity, key.value.s);
        default:
            goto InvalidKeyType;
    }
    InvalidKeyType:
        fprintf(stderr, "Key cannot be hashed due to invalid key type.\n");
        exit(EXIT_FAILURE);
}

static Entry* find_entry(Entry* entry, MultiType key)
{
    while (entry != NULL && !multi_is_equal(entry->key, key))
        entry = entry->next;

    return entry;
}

static Entry* create_entry(MultiType key, MultiType value, Entry* next)
{
    Entry* entry = (Entry*)malloc(sizeof(Entry));
    entry->key = key;
    entry->value = value;
    entry->next = next;
    return entry;
}

static void free_entries(Entry* entry) 
{
    while (entry != NULL) 
    {
        Entry* next = entry->next;
        multi_free(entry->key);
        multi_free(entry->value);
        free(entry);
        entry = next;
    }
}

static void hashtable_resize(HashTable* h, int new_capacity) 
{
    if (new_capacity < h->size) {
        fprintf(stderr, "Capacity must be greater than the hashtable size.\n");
        exit(EXIT_FAILURE);
    }
    Entry** old_array = h->array;
    h->array = (Entry**)calloc(new_capacity, sizeof(Entry*));

    for (int i = 0; i < h->capacity; i++) 
    {
        for (Entry* e = old_array[i]; e != NULL; e = e->next)
        {
            int b = bucket(new_capacity, e->key);
            h->array[b] = create_entry(e->key, e->value, h->array[b]);
        }
        free_entries(old_array[i]);
    }
    h->capacity = new_capacity;
    free(old_array);
}

HashTable* hashtable_create(int capacity)
{
    if (capacity < 0)
        goto InvalidCapacity;

    HashTable* h = (HashTable*)malloc(sizeof(HashTable));
    h->capacity = capacity;
    h->size = 0;
    h->array = (Entry**)calloc(h->capacity, sizeof(Entry*));
    return h;

    InvalidCapacity:
        fprintf(stderr, "Capacity must be a positive integer. \n");
        exit(EXIT_FAILURE);
}

MultiType hashtable_get(HashTable* h, MultiType key)
{
    int b = bucket(h->capacity, key);
    Entry* entry = find_entry(h->array[b], key);

    if (entry == NULL) {
        fprintf(stderr, "Key %s not found.\n", multi_to_string(key));
        exit(EXIT_FAILURE);
    }
    return entry->value;
}

HashTable* double_hashtable_get(DoubleHashTable* h, MultiType key)
{
    return (HashTable*)hashtable_get(h, key).value.p;
}

void hashtable_set(HashTable* h, MultiType key, MultiType value)
{
    int b = bucket(h->capacity, key);
    Entry* entry = find_entry(h->array[b], key);

    if (entry == NULL) {
        h->array[b] = create_entry(key, value, h->array[b]);
        h->size++;
    } 
    else entry->value = value;

    if (h->size > LOAD_FACTOR * h->capacity)
        hashtable_resize(h, h->capacity * GROWTH_FACTOR);
}

void hashtable_remove(HashTable* h, MultiType key)
{
    int b = bucket(h->capacity, key);
    Entry** ptr = &h->array[b];

    while (*ptr != NULL && !multi_is_equal((*ptr)->key, key))
        ptr = &(*ptr)->next;

    if (*ptr != NULL) {
        Entry* entry = *ptr;
        *ptr = entry->next;
        free(entry);
        h->size--;
    }
    if (h->size < (1 - LOAD_FACTOR) * h->capacity)
        hashtable_resize(h, ceil(h->capacity / GROWTH_FACTOR));
}

bool hashtable_contains(HashTable* h, MultiType key)
{
    int b = bucket(h->capacity, key);
    return find_entry(h->array[b], key) != NULL;
}

MultiType* hashtable_keys(HashTable* h)
{
    MultiType* keys = (MultiType*)malloc(h->size * sizeof(MultiType));
    int i = 0;
    for (int b = 0; b < h->capacity; b++) 
    {
        for (Entry* e = h->array[b]; e != NULL; e = e->next)
            keys[i++] = e->key;
    }
    return keys;
}

void hashtable_update(HashTable* h, HashTable* other)
{
    for (int b = 0; b < other->capacity; b++) 
    {
        for (Entry* e = other->array[b]; e != NULL; e = e->next)
            hashtable_set(h, e->key, e->value);
    }
}

void hashtable_print(HashTable* h)
{
    puts("{");
    for (int i = 0; i < h->capacity; i++) 
    {
        for (Entry* e = h->array[i]; e != NULL; e = e->next)
            printf("  %s: %s;\n", 
                   multi_to_string(e->key), multi_to_string(e->value));
    }
    puts("}");
}

HashTable* hashtable_copy(HashTable* h)
{
    HashTable* h_copy = hashtable_create(h->capacity);
    for (int i = 0; i < h->capacity; i++) 
    {
        for (Entry* e = h->array[i]; e != NULL; e = e->next)
            hashtable_set(h_copy, e->key, e->value);
    }
    return h_copy;
}

void hashtable_free(HashTable* h) 
{
    for (int i = 0; i < h->capacity; i++) 
    {
        free_entries(h->array[i]);
        h->array[i] = NULL;
    }
    free(h->array);
    free(h);
}

inline void set_add(Set* s, MultiType elem)
{
    hashtable_set(s, elem, elem);
}

void set_print(Set* s)
{
    printf("{ ");
    for (int i = 0; i < s->capacity; i++) 
    {
        for (Entry* e = s->array[i]; e != NULL; e = e->next)
            printf("%s; ", multi_to_string(e->key));
    }
    printf("}\n");
}

bool set_is_subset(Set* s1, Set* s2)
{
    for (int i = 0; i < s1->capacity; i++) 
    {
        for (Entry* e = s1->array[i]; e != NULL; e = e->next)
            if (!set_contains(s2, e->key))
                return false;
    }
    return true;
}
