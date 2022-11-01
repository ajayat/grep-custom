#ifndef MULTITYPE_H
#define MULTITYPE_H

#include <stdbool.h>

enum TypeTag { CharType, StringType, IntType, PointerType };

union MultiTypeValue { char c; int i; char* s; void* p; };

typedef struct MultiType {
    enum TypeTag type;
    union MultiTypeValue value;
} MultiType;

extern bool multi_is_equal(MultiType m1, MultiType m2);

extern char* multi_to_string(MultiType m);

extern MultiType multi_int(int object);

extern MultiType multi_char(char object);

extern MultiType multi_string(char* object);

extern MultiType multi_pointer(void* object);

#endif // MULTITYPE_H