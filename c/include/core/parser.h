#ifndef PARSER_H
#define PARSER_H

#include "vector.h"

typedef enum ASTTag { CharGroup, Concat, Union, Star } ASTTag;

static const char *const AST_TAG_STR[] = {
    [CharGroup] = "CharGroup",
    [Concat] = "Concat",
    [Union] = "Union",
    [Star] = "Star",
};

typedef struct AST {
    enum ASTTag tag;
    int arity;
    union {
        char *c;         // CharGroup
        struct AST **a;  // Concat, Union, Star
    } childs;
} AST;

extern AST *ast_create(ASTTag tag, int arity, int argc, ...);

extern void ast_free(AST *ast);

extern void ast_print(AST *ast, int indent);

extern AST *parse(char *regex);

#endif  // PARSER_H