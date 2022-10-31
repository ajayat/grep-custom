/**
 * Implementation of a hashtable with:
 *    - Dynamic array (automatic resizing)
 *    - Separate chaining to resolve collision
 *    - Different type for values allowed
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "hashtable.h"
#include "multitype.h"

const double LOAD_FACTOR = 0.75;
const int GROWTH_FACTOR = 2;

// Private functions

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

static int find_bucket(int capacity, MultiType key) 
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
        puts("Key cannot be hashed due to invalid key type.");
        exit(EXIT_FAILURE);
}

static LinkedEntry* find_entry(LinkedEntry *entry, MultiType key)
{
    while (entry != NULL && !multi_is_equal(entry->key, key))
        entry = entry->next;

    return entry;
}

static LinkedEntry* create_entry(MultiType key, MultiType value, LinkedEntry* next)
{
    LinkedEntry* entry = (LinkedEntry*)malloc(sizeof(LinkedEntry));
    entry->key = key;
    entry->value = value;
    entry->next = next;
    return entry;
}

static void free_entries(LinkedEntry* entry) 
{
    while (entry != NULL) 
    {
        LinkedEntry* next = entry->next;
        free(entry);
        entry = next;
    }
}

// Public functions

HashTable* hashtable_create(int capacity)
{
    if (capacity <= 0)
        goto InvalidCapacity;

    HashTable* h = (HashTable*)malloc(sizeof(HashTable));
    h->capacity = capacity;
    h->size = 0;
    h->array = (LinkedEntry**)calloc(h->capacity, sizeof(LinkedEntry*));
    return h;

    InvalidCapacity:
        puts("Capacity must be greater than 0.");
        exit(EXIT_FAILURE);
}

void hashtable_resize(HashTable* h, int new_capacity) 
{
    if (new_capacity < h->capacity) {
        puts("Size must be greater than the number of entries.");
        exit(EXIT_FAILURE);
    }
    LinkedEntry** old_array = h->array;
    h->array = (LinkedEntry**)calloc(new_capacity, sizeof(LinkedEntry*));

    for (int i = 0; i < h->capacity; i++) 
    {
        for (LinkedEntry* e = old_array[i]; e != NULL; e = e->next)
        {
            int b = find_bucket(new_capacity, e->key);
            h->array[b] = create_entry(e->key, e->value, h->array[b]);
        }
        free_entries(old_array[i]);
    }
    h->capacity = new_capacity;
    free(old_array);
}

MultiType hashtable_get(HashTable* h, MultiType key)
{
    int b = find_bucket(h->capacity, key);
    LinkedEntry* entry = find_entry(h->array[b], key);

    return (entry == NULL) ? MULTI_EMPTY : entry->value;
}

void hashtable_set(HashTable* h, MultiType key, MultiType value)
{
    int b = find_bucket(h->capacity, key);
    LinkedEntry* entry = find_entry(h->array[b], key);

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
    int b = find_bucket(h->capacity, key);
    LinkedEntry** p = &h->array[b];

    while (*p != NULL && !multi_is_equal((*p)->key, key))
        p = &(*p)->next;

    if (*p != NULL) {
        LinkedEntry* entry = *p;
        *p = entry->next;
        free(entry);
        h->size--;
    }
    if (h->size < (1 - LOAD_FACTOR) * h->capacity)
        hashtable_resize(h, h->capacity / GROWTH_FACTOR);
}

bool hashtable_contains(HashTable* h, MultiType key)
{
    int b = find_bucket(h->capacity, key);
    return find_entry(h->array[b], key) != NULL;
}

MultiType* hashtable_keys(HashTable* h)
{
    MultiType* keys = (MultiType*)malloc(h->size * sizeof(MultiType));
    int i = 0;
    for (int b = 0; b < h->capacity; b++) 
    {
        for (LinkedEntry* e = h->array[b]; e != NULL; e = e->next)
            keys[i++] = e->key;
    }
    return keys;
}

void hashtable_print(HashTable* h)
{
    puts("{");
    for (int i = 0; i < h->capacity; i++) 
    {
        for (LinkedEntry* e = h->array[i]; e != NULL; e = e->next)
            printf("  %s: %s;\n", 
                   multi_to_string(e->key), multi_to_string(e->value));
    }
    puts("}");
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
