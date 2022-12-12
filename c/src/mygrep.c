#include <stdio.h>  // printf
#include <stdlib.h>
#include <string.h>  // strlen

#include "algorithm.h"
#include "automaton.h"
#include "parser.h"

int main(int argc, char* argv[])
{
    AST* ast = parse(".*");
    ast_print(ast, 0);
    NFA* nfa = thompson(ast);
    if (nfa_accept(nfa, "a"))
        printf("Accepted\n");
    else
        printf("Rejected\n");
    ast_free(ast);
    DFA* dfa = brzozowski(nfa_determinize(nfa));
    nfa_free(nfa, true);
    dfa_free(dfa, true);
    return 0;
}