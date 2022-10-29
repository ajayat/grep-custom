/**
 * Implementation of a hashmap with::
 *    - Dynamic array (use dynamic reallocation)
 *    - Separate chaining using vectors to resolve collision
 *    - Different type for values
 */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "hashmap.h"
#include "multitype.c"

const int HASHMAP_INITIAL_SIZE = 16;
const float HASHMAP_LOAD_FACTOR = 0.75;
const int HASHMAP_GROWTH_FACTOR = 2;


int hash_string(MultiType key_to_hash) 
{
    assert(key_to_hash.type == StringType);
    char* s = key_to_hash.value.s;  // convert MultiType to char*
    int hash = 0;

    for (int i = 0; i < (int)strlen(s); i++) {
        hash += (int)s[i];
    }
    return hash;
}

int hash_int(MultiType key_to_hash) 
{
    assert(key_to_hash.type == IntType);
    return key_to_hash.value.i;
}

static inline int hashmap_bucket(HashMap* h, MultiType key)
{
    return h->hash_function(key) % h->size;
}

static Entry* find_entry(Entry *entry, MultiType key)
{
    while (entry != NULL && !multi_is_equal(entry->key, key)) {
        entry = entry->next;
    }
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
        free(entry);
        entry = next;
    }
}

HashMap* hashmap(int size, int hash_function(MultiType))
{
    HashMap* h = (HashMap*)malloc(sizeof(HashMap));
    h->size = size;
    h->len = 0;
    h->hash_function = hash_function;
    h->array = (Entry**)calloc(size, sizeof(Entry*));
    return h;
}

MultiType hashmap_get(HashMap* h, MultiType key)
{
    int bucket = hashmap_bucket(h, key);
    Entry* entry = find_entry(h->array[bucket], key);

    if (entry == NULL) {
        return multi_pointer(NULL);
    }
    return entry->value;
}

void hashmap_put(HashMap* h, MultiType key, MultiType value)
{
    int bucket = hashmap_bucket(h, key);
    Entry* entry = find_entry(h->array[bucket], key);

    if (entry == NULL) {
        h->array[bucket] = create_entry(key, value, h->array[bucket]);
        h->len++;
    } else {
        entry->value = value;
    }
}

void hashmap_remove(HashMap* h, MultiType key)
{
    int bucket = hashmap_bucket(h, key);
    Entry* entry = find_entry(h->array[bucket], key);

    if (entry != NULL) 
    {
        Entry **p = &h->array[bucket];
        while (*p != entry) {
            p = &(*p)->next;
        }
        *p = entry->next;
        free(entry);
        h->len--;
    }
}

bool hashmap_contains(HashMap* h, MultiType key)
{
    int bucket = hashmap_bucket(h, key);
    return find_entry(h->array[bucket], key) != NULL;
}

void hashmap_print(HashMap* h)
{
    printf("{\n");
    for (int i = 0; i < h->size; i++) 
    {
        for (Entry* e = h->array[i]; e != NULL; e = e->next) 
        {
            char key[10], value[100];
            printf("  %s: %s;\n", 
                   multi_to_string(e->key, key), multi_to_string(e->value, value));
        }
    }
    printf("}\n");
}

void hashmap_free(HashMap* h) 
{
    for (int i = 0; i < h->size; i++) 
    {
        free_entries(h->array[i]);
        h->array[i] = NULL;
    }
    free(h->array);
    free(h);
}


int main()
{
    HashMap* dict = hashmap(10, hash_string);
    hashmap_put(dict, multi_string("a"), multi_string("two"));
    hashmap_put(dict, multi_string("b"), multi_int(3));
    hashmap_print(dict);
    hashmap_remove(dict, multi_string("a"));
    hashmap_free(dict);
}
