#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <stdbool.h>

#include "automaton.h"
#include "parser.h"

extern DFA *brzozowski(DFA *dfa);

extern NFA *thompson(AST *ast);

#endif  // ALGORITHM_H