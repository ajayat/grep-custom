/**
 * Implementation of a set as an instance of hashmap 
 * with keys equals to values.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

#include "set.h"
#include "multitype.h"
#include "hashtable.h"

// Public functions

Set* set_create(int capacity) 
{
    return hashtable_create(capacity);
}

void set_add(Set* s, MultiType elem)
{
    hashtable_set(s, elem, elem);
}

bool set_contains(Set* s, MultiType elem)
{
    return hashtable_contains(s, elem);
}

void set_remove(Set* s, MultiType elem)
{
    hashtable_remove(s, elem);
}

MultiType* set_elems(Set* s)
{
    return hashtable_keys(s);
}

void set_free(Set* s)
{
    hashtable_free(s);
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

Set* set_union(Set* s1, Set* s2)
{
    Set* s = set_create(s1->capacity + s2->capacity);
    for (int i = 0; i < s1->capacity; i++) 
    {
        for (Entry* e = s1->array[i]; e != NULL; e = e->next)
            set_add(s, e->key);
    }
    for (int i = 0; i < s2->capacity; i++) 
    {
        for (Entry* e = s2->array[i]; e != NULL; e = e->next)
            set_add(s, e->key);
    }
    hashtable_resize(s, s->size);
    return s;
}

Set* set_intersection(Set* s1, Set* s2)
{
    Set* s = set_create(s1->capacity);
    for (int i = 0; i < s1->capacity; i++) 
    {
        for (Entry* e = s1->array[i]; e != NULL; e = e->next)
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
        for (Entry* e = s1->array[i]; e != NULL; e = e->next) 
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
        for (Entry* e = s1->array[i]; e != NULL; e = e->next)
            if (!set_contains(s2, e->key))
                return false;
    }
    return true;
}
