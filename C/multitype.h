#ifndef MULTITYPE
#define MULTITYPE

#include <assert.h>

enum TypeTag { CharType, StringType, IntType, PointerType};

union MultiTypeValue { char c; int i; char* s; void* p; };

typedef struct MultiType {
    enum TypeTag type;
    union MultiTypeValue value;
} MultiType;

extern bool multi_is_equal(MultiType m1, MultiType m2);

extern char* multi_to_string(MultiType m, char* buffer);

extern inline MultiType multi_int(int object);

extern inline MultiType multi_char(char object);

extern inline MultiType multi_string(char* object);

extern inline MultiType multi_pointer(void* object);

#endif // MULTITYPE