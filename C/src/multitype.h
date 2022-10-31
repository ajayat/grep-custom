#ifndef MULTITYPE_H
#define MULTITYPE_H

enum TypeTag { CharType, StringType, IntType, PointerType, EmptyType };

union MultiTypeValue { char c; int i; char* s; void* p; };

typedef struct MultiType {
    enum TypeTag type;
    union MultiTypeValue value;
} MultiType;

const MultiType MULTI_EMPTY = (MultiType){ EmptyType };

extern bool multi_is_equal(MultiType m1, MultiType m2);

extern char* multi_to_string(MultiType m);

extern inline MultiType multi_int(int object);

extern inline MultiType multi_char(char object);

extern inline MultiType multi_string(char* object);

extern inline MultiType multi_pointer(void* object);

#endif // MULTITYPE_H