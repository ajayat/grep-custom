/**
 * Implementation of a set as an instance of hashmap 
 * with keys equals to values.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "set.h"
#include "multitype.c"
#include "hashtable.c"

// Public functions

Set* set_create(int capacity) 
{
    return hashtable_create(capacity);
}

void set_add(Set* s, MultiType key)
{
    hashtable_set(s, key, key);
}

bool set_contains(Set* s, MultiType key)
{
    return hashtable_contains(s, key);
}

void set_remove(Set* s, MultiType key)
{
    hashtable_remove(s, key);
}

MultiType* set_keys(Set* s)
{
    return hashtable_keys(s);
}

void set_free(Set* s)
{
    hashtable_free(s);
}

void set_print(Set* s)
{
    puts("{ ");
    for (int i = 0; i < s->capacity; i++) 
    {
        for (LinkedEntry* e = s->array[i]; e != NULL; e = e->next)
            printf("%s; ", multi_to_string(e->key));
    }
    puts("}");
}

Set* set_union(Set* s1, Set* s2)
{
    Set* s = set_create(s1->capacity + s2->capacity);
    for (int i = 0; i < s1->capacity; i++) 
    {
        for (LinkedEntry* e = s1->array[i]; e != NULL; e = e->next)
            set_add(s, e->key);
    }
    for (int i = 0; i < s2->capacity; i++) 
    {
        for (LinkedEntry* e = s2->array[i]; e != NULL; e = e->next)
            set_add(s, e->key);
    }
    hashtable_resize(s, s->size);
    return s;
}

Set* set_intersection(Set* s1, Set* s2)
{
    Set* s = set_create(min(s1->capacity, s2->capacity));
    for (int i = 0; i < s1->capacity; i++) 
    {
        for (LinkedEntry* e = s1->array[i]; e != NULL; e = e->next)
            if (set_contains(s2, e->key))
                set_add(s, e->key);
    }
    hashtable_resize(s, s->size);
    return s;
}

Set* set_difference(Set* s1, Set* s2)
{
    Set* s = set_create(s1->capacity);
    for (int i = 0; i < s1->capacity; i++) 
    {
        for (LinkedEntry* e = s1->array[i]; e != NULL; e = e->next) 
            if (!set_contains(s2, e->key))
                set_add(s, e->key);
    }
    hashtable_resize(s, s->size);
    return s;
}

bool set_is_subset(Set* s1, Set* s2)
{
    for (int i = 0; i < s1->capacity; i++) 
    {
        for (LinkedEntry* e = s1->array[i]; e != NULL; e = e->next)
            if (!set_contains(s2, e->key))
                return false;
    }
    return true;
}
