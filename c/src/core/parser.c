#include "parser.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>  // strlen

#include "automaton.h"
#include "stack.h"
#include "vector.h"

AST *ast_create(ASTTag tag, int arity, int argc, ...)
{
    AST *ast = (AST *)malloc(sizeof(AST));
    ast->tag = tag;
    ast->arity = arity;
    if (tag == CharGroup)
        ast->childs.c = calloc(arity, sizeof(char));
    else
        ast->childs.a = calloc(arity, sizeof(AST *));

    va_list args;
    va_start(args, argc);
    switch (tag) {
        case CharGroup:
            for (int i = 0; i < argc; i++)
                ast->childs.c[0] = va_arg(args, int);
            break;
        case Star:
            ast->childs.a[0] = va_arg(args, AST *);
            break;
        case Concat:
        case Union:
            ast->childs.a[0] = va_arg(args, AST *);
            ast->childs.a[1] = va_arg(args, AST *);
            break;
    }
    va_end(args);
    return ast;
}

void ast_free(AST *ast)
{
    if (ast->tag == CharGroup)
        free(ast->childs.c);
    else {
        for (int i = 0; i < ast->arity; i++)
            ast_free(ast->childs.a[i]);
        free(ast->childs.a);
    }
}

void ast_print(AST *ast, int indent)
{
    for (int i = 0; i < indent; i++)
        printf("  ");
    printf("%s\n", AST_TAG_STR[ast->tag]);

    if (ast->tag == CharGroup) {
        for (int i = 0; i < indent + 1; i++)
            printf("  ");
        for (int i = 0; i < ast->arity; i++)
            printf("%c", ast->childs.c[i]);
        printf("\n");
    } else {
        for (int i = 0; i < ast->arity; i++)
            ast_print(ast->childs.a[i], indent + 1);
    }
}

/* Constructs an AST from a regex in postfixe form */
AST *parse(char *regex)
{
    Stack *stack = stack_create();

    for (int i = 0; regex[i] != '\0'; i++) {
        AST *ast;
        switch (regex[i]) {
            case '@': {
                AST *right = (AST *)stack_pop(stack).value.p;
                AST *left = (AST *)stack_pop(stack).value.p;
                ast = ast_create(Concat, 2, 2, left, right);
                break;
            }
            case '*': {
                AST *child = (AST *)stack_pop(stack).value.p;
                ast = ast_create(Star, 1, 1, child);
                break;
            }
            case '.': {
                ast = ast_create(CharGroup, strlen(ALPHABET), 0);
                for (int i = 0; i < ast->arity; i++)
                    ast->childs.c[i] = ALPHABET[i];
                break;
            }
            case '|': {
                AST *right = (AST *)stack_pop(stack).value.p;
                AST *left = (AST *)stack_pop(stack).value.p;
                ast = ast_create(Union, 2, 2, left, right);
                break;
            }
            case '?': {
                AST *child = (AST *)stack_pop(stack).value.p;
                ast = ast_create(Union, 2, 2, EPSILON, child);
                break;
            }
            default:
                ast = ast_create(CharGroup, 1, 1, regex[i]);
        }
        stack_push(stack, multi_pointer(ast));
    }
    AST *ast = (AST *)stack_pop(stack).value.p;
    stack_free(stack);
    return ast;
}
