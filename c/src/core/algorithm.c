#include "algorithm.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "automaton.h"
#include "parser.h"

DFA *brzozowski(DFA *dfa)
{
    NFA *mirror_nfa = dfa_transpose(dfa);
    DFA *mirror_det = nfa_determinize(mirror_nfa);
    nfa_free(mirror_nfa, true);
    NFA *nfa = dfa_transpose(mirror_det);
    dfa_free(mirror_det, true);
    DFA *dfa_minimized = nfa_determinize(nfa);
    nfa_free(nfa, true);
    return dfa_minimized;
}

NFA *thompson(AST *ast)
{
    static int state = 0;

    switch (ast->tag) {
        case CharGroup: {
            NFA *nfa = nfa_create();
            MultiType init = multi_int(state++), final = multi_int(state++);
            nfa->initial = hashtable_create(2);
            hashtable_set(nfa->initial, init, init);
            nfa->final = hashtable_create(2);
            hashtable_set(nfa->final, final, final);

            for (int i = 0; i < ast->arity; i++)
                nfa_set_transition(nfa, init, ast->childs.c[i], final);
            return nfa;
        }
        case Union: {
            NFA *nfa = thompson(ast->childs.a[0]);
            NFA *nfa2 = thompson(ast->childs.a[1]);
            hashtable_update(nfa->_transitions, nfa2->_transitions);
            hashtable_update(nfa->initial, nfa2->initial);
            hashtable_update(nfa->final, nfa2->final);
            nfa_free(nfa2, false);
            return nfa;
        }
        case Concat: {
            NFA *nfa = thompson(ast->childs.a[0]);
            NFA *nfa2 = thompson(ast->childs.a[1]);

            Vector *final = hashtable_to_vector(nfa->final);
            for (int i = 0; i < final->size; i++) {
                Vector *initial = hashtable_to_vector(nfa2->initial);

                for (int j = 0; j < initial->size; j++)
                    nfa_set_transition(nfa, final->array[i], EPSILON,
                                       initial->array[j]);
                vector_free(initial);
            }
            vector_free(final);
            nfa->final = nfa2->final;
            nfa_free(nfa2, false);
            return nfa;
        }
        case Star: {
            NFA *nfa = thompson(ast->childs.a[0]);

            Vector *final = hashtable_to_vector(nfa->final);
            for (int i = 0; i < final->size; i++) {
                MultiType q_f = final->array[i];
                Vector *initial = hashtable_to_vector(nfa->initial);

                for (int j = 0; j < initial->size; j++) {
                    MultiType q_i = initial->array[j];
                    nfa_set_transition(nfa, q_i, EPSILON, q_f);
                    nfa_set_transition(nfa, q_f, EPSILON, q_i);
                }
                vector_free(initial);
            }
            vector_free(final);
            return nfa;
        }
        default:
            fprintf(stderr, "Invalid AST tag");
            exit(EXIT_FAILURE);
    }
}