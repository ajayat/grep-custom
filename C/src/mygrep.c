#include <stdlib.h>
#include <stdio.h>  // printf

#include "multitype.h"
#include "hashtable.h"


int main(int argc, char* argv[])
{   
    Set* s = set_create(10);
    set_add(s, multi_int(1));
    set_add(s, multi_string("abc"));
    set_add(s, multi_char("a"));
    set_print(s);
    set_free(s);
    return 0;
}