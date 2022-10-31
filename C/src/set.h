#ifndef SET_H
#define SET_H

typedef HashTable Set;

extern Set* set_create(int capacity);

extern void set_add(Set* s, MultiType key);

extern bool set_contains(Set* s, MultiType key);

extern void set_remove(Set* s, MultiType key);

extern void set_print(Set* s);

extern void set_free(Set* s);

extern Set* set_union(Set* s1, Set* s2);

extern Set* set_intersection(Set* s1, Set* s2);

extern Set* set_difference(Set* s1, Set* s2);

extern bool set_is_subset(Set* s1, Set* s2);

#endif // SET_H