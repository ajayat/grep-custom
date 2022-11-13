#include <stdio.h>  // printf
#include <stdlib.h>
#include <string.h>  // strlen

#include "algorithm.h"
#include "automaton.h"
#include "parser.h"

int main(int argc, char* argv[])
{
    AST* ast = parse("ab@*");
    NFA* nfa = thompson(ast);
    ast_free(ast);
    DFA* dfa = brzozowski(nfa_determinize(nfa));
    nfa_free(nfa, true);

    dfa_free(dfa, true);
    return 0;
}