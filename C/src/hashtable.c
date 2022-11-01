/**
 * Implementation of a hashtable with:
 *    - Dynamic array (automatic resizing)
 *    - Separate chaining to resolve collision
 *    - Different type for values allowed
 */

#include <stdlib.h>
#include <stdbool.h>  // bool
#include <string.h>  // strcmp, strlen
#include <stdio.h>  // printf
#include <math.h>  // ceil

#include "hashtable.h"
#include "multitype.h"

const double LOAD_FACTOR = 0.75;
const int GROWTH_FACTOR = 2;

// Private functions

static int hash_int(int capacity, int key) 
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
        puts("Key cannot be hashed due to invalid key type.");
        exit(EXIT_FAILURE);
}

static Entry* find_item(Entry* entries, MultiType key)
{
    while (entries != NULL && !multi_is_equal(entries->key, key))
        entries = entries->next;

    return entries;
}

static Entry* create_item(MultiType key, MultiType value, Entry* next)
{
    Entry* item = (Entry*)malloc(sizeof(Entry));
    item->key = key;
    item->value = value;
    item->next = next;
    return item;
}

static void free_entries(Entry* item) 
{
    while (item != NULL) 
    {
        Entry* next = item->next;
        free(item);
        item = next;
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
    h->array = (Entry**)calloc(h->capacity, sizeof(Entry*));
    return h;

    InvalidCapacity:
        puts("Capacity must be greater than 0.");
        exit(EXIT_FAILURE);
}

void hashtable_resize(HashTable* h, int new_capacity) 
{
    if (new_capacity < h->size) {
        puts("Capacity must be greater than the number of entries.");
        exit(EXIT_FAILURE);
    }
    Entry** old_array = h->array;
    h->array = (Entry**)calloc(new_capacity, sizeof(Entry*));

    for (int i = 0; i < h->capacity; i++) 
    {
        for (Entry* e = old_array[i]; e != NULL; e = e->next)
        {
            int b = bucket(new_capacity, e->key);
            h->array[b] = create_item(e->key, e->value, h->array[b]);
        }
        free_entries(old_array[i]);
    }
    h->capacity = new_capacity;
    free(old_array);
}

MultiType hashtable_get(HashTable* h, MultiType key)
{
    int b = bucket(h->capacity, key);
    Entry* item = find_item(h->array[b], key);

    if (item == NULL) {
        printf("Key %s not found.", multi_to_string(key));
        exit(EXIT_FAILURE);
    }
    return item->value;
}

void hashtable_set(HashTable* h, MultiType key, MultiType value)
{
    int b = bucket(h->capacity, key);
    Entry* item = find_item(h->array[b], key);

    if (item == NULL) {
        h->array[b] = create_item(key, value, h->array[b]);
        h->size++;
    } 
    else item->value = value;

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
        Entry* item = *ptr;
        *ptr = item->next;
        free(item);
        h->size--;
    }
    if (h->size < (1 - LOAD_FACTOR) * h->capacity)
        hashtable_resize(h, ceil(h->capacity / GROWTH_FACTOR));
}

bool hashtable_contains(HashTable* h, MultiType key)
{
    int b = bucket(h->capacity, key);
    return find_item(h->array[b], key) != NULL;
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
