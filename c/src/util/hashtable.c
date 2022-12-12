/**
 * Implementation of a hashtable with:
 *    - Dynamic array (automatic resizing)
 *    - Separate chaining to resolve collision
 *    - Different type for values allowed (MultiType)
 */

#include "hashtable.h"

#include <math.h>     // ceil
#include <stdbool.h>  // bool
#include <stdio.h>    // printf
#include <stdlib.h>
#include <string.h>  // strcmp, strlen

#include "multitype.h"
#include "stack.h"
#include "vector.h"

const float HASHTABLE_LOAD_FACTOR = 0.75;
const float HASHTABLE_GROWTH_FACTOR = 2;

static inline int hash_int(int capacity, int key)
{
    return (int)(capacity * key * 1.61803398875) % capacity;
}

static int hash_string(int capacity, char* key)
{
    int hash = 0;
    for (int i = 0; i < (int)strlen(key); i++)
        hash = (hash * 31 + (int)key[i]) % capacity;

    return hash;
}

static int hash_htbl(int capacity, Set* set)
{
    unsigned long int n = 0;
    Vector* keys = hashtable_to_vector(set);
    for (int i = 0; i < keys->size; i++)
        switch (keys->array[i].type) {
            case IntType:
                n += keys->array[i].value.i;
                break;
            case CharType:
                n += (int)keys->array[i].value.c;
                break;
            case StringType:
                n += hash_string(capacity, keys->array[i].value.s);
                break;
            case HtblType:
            case PointerType:
                n += (unsigned long int)keys->array[i].value.p;
                break;
            default:
                goto InvalidKeyType;
        }
    vector_free(keys);
    return n % capacity;

InvalidKeyType:
    fprintf(stderr, "Key cannot be hashed due to invalid key type.\n");
    exit(EXIT_FAILURE);
}

static int bucket(int capacity, MultiType key)
{
    switch (key.type) {
        case IntType:
            return hash_int(capacity, key.value.i);
        case CharType:
            return (int)key.value.c % capacity;
        case StringType:
            return hash_string(capacity, key.value.s);
        case HtblType:
            return hash_htbl(capacity, (Set*)key.value.p);
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

static void free_entries(Entry* entry, bool deep)
{
    while (entry != NULL) {
        Entry* next = entry->next;
        if (deep) {
            multi_free(entry->key);
            multi_free(entry->value);
        }
        free(entry);
        entry = next;
    }
}

static void hashtable_resize(HashTable* h, int new_capacity)
{
    if (new_capacity < h->size) {
        fprintf(stderr, "Capacity must be greater than hashtable size.\n");
        exit(EXIT_FAILURE);
    }
    Entry** old_array = h->array;
    h->array = (Entry**)calloc(new_capacity, sizeof(Entry*));

    for (int i = 0; i < h->capacity; i++) {
        for (Entry* e = old_array[i]; e != NULL; e = e->next) {
            int b = bucket(new_capacity, e->key);
            h->array[b] = create_entry(e->key, e->value, h->array[b]);
        }
        free_entries(old_array[i], false);
    }
    h->capacity = new_capacity;
    free(old_array);
}

HashTable* hashtable_create(int capacity)
{
    if (capacity < 0) {
        fprintf(stderr, "Capacity must be a positive integer. \n");
        exit(EXIT_FAILURE);
    }
    HashTable* h = (HashTable*)malloc(sizeof(HashTable));
    h->capacity = capacity;
    h->size = 0;
    h->array = (Entry**)calloc(capacity, sizeof(Entry*));
    return h;
}

MultiType hashtable_get(HashTable* h, MultiType key)
{
    int b = bucket(h->capacity, key);
    Entry* entry = find_entry(h->array[b], key);
    return (entry == NULL) ? MULTI_NULL : entry->value;
}

HashTable* hashtable_get_or_create(HashTable* h, MultiType key)
{
    MultiType entry = hashtable_get(h, key);
    switch (entry.type) {
        case NullType: {
            HashTable* new_htbl = hashtable_create(2);
            hashtable_set(h, key, multi_htbl(new_htbl));
            return new_htbl;
        }
        case HtblType:
            return (HashTable*)entry.value.p;
        default:
            fprintf(stderr, "Key does not map to a hashtable.\n");
            exit(EXIT_FAILURE);
    }
}

void hashtable_set(HashTable* h, MultiType key, MultiType value)
{
    int b = bucket(h->capacity, key);
    Entry* entry = find_entry(h->array[b], key);

    if (entry == NULL) {
        h->array[b] = create_entry(key, value, h->array[b]);
        h->size++;
    } else
        entry->value = value;

    if (h->size > HASHTABLE_LOAD_FACTOR * h->capacity)
        hashtable_resize(h, ceil(h->capacity * HASHTABLE_GROWTH_FACTOR));
}

void hashtable_remove(HashTable* h, MultiType key)
{
    Entry** ptr = &h->array[bucket(h->capacity, key)];
    while (*ptr != NULL && !multi_is_equal((*ptr)->key, key))
        ptr = &(*ptr)->next;

    if (*ptr != NULL) {
        Entry* entry = *ptr;
        *ptr = entry->next;
        free(entry);
        h->size--;
    }
    if (h->size < (1 - HASHTABLE_LOAD_FACTOR) * h->capacity)
        hashtable_resize(h, ceil(h->capacity / HASHTABLE_GROWTH_FACTOR));
}

bool hashtable_contains(HashTable* h, MultiType key)
{
    return hashtable_get(h, key).type != NullType;
}

Vector* hashtable_to_vector(HashTable* h)
{
    Vector* v_keys = vector_create(h->size);
    for (int b = 0; b < h->capacity; b++) {
        for (Entry* e = h->array[b]; e != NULL; e = e->next)
            vector_push(v_keys, e->key);
    }
    return v_keys;
}

void hashtable_update(HashTable* h, HashTable* other)
{
    for (int b = 0; b < other->capacity; b++) {
        for (Entry* e = other->array[b]; e != NULL; e = e->next)
            hashtable_set(h, e->key, e->value);
    }
}

void hashtable_print(HashTable* h)
{
    if (h->size == 0)
        puts("{}");
    else {
        puts("{");
        for (int i = 0; i < h->capacity; i++) {
            for (Entry* e = h->array[i]; e != NULL; e = e->next) {
                multi_print(e->key, "  ", ": ");
                multi_print(e->value, "", ";\n");
            }
        }
        puts("}");
    }
}

/* Performs a shallow copy */
HashTable* hashtable_copy(HashTable* h)
{
    HashTable* h_copy = hashtable_create(h->capacity);
    h_copy->size = h->size;
    memcpy(h_copy->array, h->array, h->capacity * sizeof(Entry*));
    return h_copy;
}

bool hashtable_is_equal(HashTable* h, HashTable* other)
{
    if (h->size != other->size)
        return false;

    for (int b = 0; b < h->capacity; b++) {
        for (Entry* e = h->array[b]; e != NULL; e = e->next) {
            MultiType value = hashtable_get(other, e->key);
            if (value.type == NullType || !multi_is_equal(value, e->value))
                return false;
        }
    }
    return true;
}

void hashtable_free(HashTable* h, bool deep)
{
    for (int i = 0; i < h->capacity; i++) {
        free_entries(h->array[i], deep);
        h->array[i] = NULL;
    }
    free(h->array);
    free(h);
}