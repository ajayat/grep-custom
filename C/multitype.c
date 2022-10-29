/**
 * Implementation of a multi-type data structure to handle 
 * variables of different types.
 * https://en.wikipedia.org/wiki/Tagged_union
 */
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#include "multitype.h"


bool multi_is_equal(MultiType m1, MultiType m2) 
{
    if (m1.type != m2.type) {
        return false;
    }
    switch (m1.type) {
        case CharType:
            return m1.value.c == m2.value.c;
        case StringType:
            return strcmp(m1.value.s, m2.value.s) == 0;
        case IntType:
            return m1.value.i == m2.value.i;
        case PointerType:
            return m1.value.p == m2.value.p;
        default:
            return false;
    }
}

char* multi_to_string(MultiType m, char* buffer) 
{
    switch (m.type) {
        case CharType:
            sprintf(buffer, "%c", m.value.c);
            break;
        case StringType:
            sprintf(buffer, "%s", m.value.s);
            break;
        case IntType:
            sprintf(buffer, "%d", m.value.i);
            break;
        case PointerType:
            sprintf(buffer, "%p", m.value.p);
            break;
        default:
            sprintf(buffer, "Unknown type");
    }
    return buffer;
}

MultiType multi_int(int object) 
{
    return (MultiType){IntType, {.i = object}};
}

MultiType multi_char(char object) 
{
    return (MultiType){CharType, {.c = object}};
}

MultiType multi_string(char* object) 
{
    return (MultiType){StringType, {.s = object}};
}

MultiType multi_pointer(void* object) 
{
    return (MultiType){PointerType, {.p = object}};
}