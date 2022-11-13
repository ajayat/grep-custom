#include "parser.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strlen

#include "automaton.h"
#include "vector.h"

AST *ast_create(ASTTag tag, int argc, ...)
{
    AST *ast = (AST *)malloc(sizeof(AST));
    ast->tag = tag;
    ast->childs = vector_create(2);

    va_list args;
    va_start(args, argc);
    switch (tag) {
        case CharGroup:
            for (int i = 0; i < argc; i++)
                vector_push(ast->childs, multi_char(va_arg(args, char)));
            break;
        case Star:
            vector_push(ast->childs, multi_pointer(va_arg(args, AST *)));
            break;
        case Concat:
        case Union:
            vector_push(ast->childs, multi_pointer(va_arg(args, AST *)));
            vector_push(ast->childs, multi_pointer(va_arg(args, AST *)));
            break;
    }
    va_end(args);
    return ast;
}

AST *ast_nth_child(AST *ast, int n)
{
    if (n < 0 || n >= ast->childs->size)
        goto InvalidSize;

    MultiType child = ast->childs->array[n];
    if (child.type != PointerType)
        goto InvalidChild;

    return (AST *)child.value.p;

InvalidSize:
    printf("%dth child does not exists\n", n);
    exit(EXIT_FAILURE);
InvalidChild:
    printf("%dth child is not an AST\n", n);
    exit(EXIT_FAILURE);
}

char ast_leaf_child(AST *ast, int n)
{
    if (ast->tag != CharGroup) {
        puts("AST does not have a Char tag.");
        exit(EXIT_FAILURE);
    }
    return ast->childs->array[n].value.c;
}

void ast_free(AST *ast)
{
    Vector *stack = vector_create(2);
    vector_push(stack, multi_pointer(ast));

    while (stack->size > 0) {
        AST *ast = (AST *)vector_pop(stack).value.p;
        if (ast->tag == CharGroup)
            continue;
        for (int i = 0; i < ast->childs->size; i++)
            vector_push(stack, multi_pointer(ast_nth_child(ast, i)));
        free(ast->childs);
        free(ast);
    }
    free(stack);
}

/* Constructs an AST from a regex in postfixe form */
AST *parse(char *regex)
{
    Vector *stack = vector_create(strlen(regex));

    for (int i = 0; regex[i] != '\0'; i++) {
        AST *ast;
        switch (regex[i]) {
            case '@': {
                AST *right = (AST *)vector_pop(stack).value.p;
                AST *left = (AST *)vector_pop(stack).value.p;
                ast = ast_create(Concat, 2, left, right);
                break;
            }
            case '*': {
                AST *child = (AST *)vector_pop(stack).value.p;
                ast = ast_create(Star, 1, child);
                break;
            }
            case '.': {
                ast = ast_create(CharGroup, 0);
                for (int i = 0; i < strlen(ALPHABET); i++)
                    vector_push(ast->childs, multi_char(ALPHABET[i]));
                break;
            }
            case '|': {
                AST *right = (AST *)vector_pop(stack).value.p;
                AST *left = (AST *)vector_pop(stack).value.p;
                ast = ast_create(Union, 2, left, right);
                break;
            }
            case '?': {
                AST *child = (AST *)vector_pop(stack).value.p;
                ast = ast_create(Union, 2, EPSILON, child);
                break;
            }
            default:
                ast = ast_create(CharGroup, 1, regex[i]);
        }
        vector_push(stack, multi_pointer(ast));
    }
    AST *ast = (AST *)vector_pop(stack).value.p;
    vector_free(stack);
    return ast;
}
