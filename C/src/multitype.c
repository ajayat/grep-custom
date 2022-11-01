/**
 * Implementation of a multi-type data structure to handle 
 * variables of different types.
 * https://en.wikipedia.org/wiki/Tagged_union
 */

#include <stdlib.h>
#include <stdio.h>  // for printf, sprintf
#include <stdbool.h>  // bool
#include <string.h>  // strcmp

#include "multitype.h"

// Public functions

bool multi_is_equal(MultiType m1, MultiType m2) 
{
    if (m1.type != m2.type)
        return false;

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
            goto InvalidType;
    }
    InvalidType:
        puts("Type of arguments are invalid.");
        exit(EXIT_FAILURE);
}

char* multi_to_string(MultiType m) 
{
    switch (m.type) {
        case IntType: {
            static char buffer[21]; // 4-bytes integer can be 20 digits long
            sprintf(buffer, "%d", m.value.i);
            return buffer;
        }
        case CharType: {
            static char buffer[2]; // 1 bit char + null terminator
            sprintf(buffer, "%c", m.value.c);
            return buffer;
        }
        case StringType:
            return m.value.s;

        case PointerType: {
            static char buffer[21]; // Pointer is a 8-bytes integer 
            sprintf(buffer, "%p", m.value.p);
            return buffer;
        }
        default:
            goto InvalidType;
    }
    InvalidType:
        puts("MultiType value cannot be converted to string");
        exit(EXIT_FAILURE);
}

MultiType multi_int(int object) 
{
    return (MultiType){ IntType, { .i = object } };
}

MultiType multi_char(char object) 
{
    return (MultiType){ CharType, { .c = object } };
}

MultiType multi_string(char* object) 
{
    return (MultiType){ StringType, { .s = object } };
}

MultiType multi_pointer(void* object) 
{
    return (MultiType){ PointerType, {.p = object} };
}
