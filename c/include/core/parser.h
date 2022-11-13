#ifndef PARSER_H
#define PARSER_H

#include "vector.h"

typedef enum ASTTag { CharGroup, Concat, Union, Star } ASTTag;

typedef struct AST {
    enum ASTTag tag;
    Vector *childs;
} AST;

extern AST *ast_create(ASTTag tag, int argc, ...);

extern AST *ast_nth_child(AST *ast, int n);

extern char ast_leaf_child(AST *ast, int n);

extern void ast_free(AST *ast);

extern AST *parse(char *regex);

#endif  // PARSER_H