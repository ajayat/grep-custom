#ifndef AUTOMATON_H
#define AUTOMATON_H

#include <stdbool.h>

#include "vector.h"
#include "hashtable.h"
#include "multitype.h"

extern const char EPSILON;

/**
 * Deterministic Finite Automaton with 1 initial state
 */
typedef struct DFA {
    char* alphabet;
    MultiType initial;
    Set* final;
    HashTable* _transitions; // (state -> letter -> state)
} DFA;

extern DFA* dfa_create(char* alphabet, MultiType initial);

extern void dfa_set_transition(DFA* dfa, MultiType state, char a, MultiType p);

extern MultiType dfa_delta(DFA* dfa, MultiType state, char a);

extern bool dfa_accept(DFA* dfa, char* word);

extern void dfa_free(DFA* dfa);

/**
 * Non-deterministic Finite Automaton that accepts epsilon transitions
 */
typedef struct NFA {
    char* alphabet;
    Set* initial;
    Set* final;
    HashTable* _transitions; // (state -> letter -> Set of state)
} NFA;

extern NFA* nfa_create(char* alphabet);

extern void nfa_set_transition(NFA* nfa, MultiType state, char a, MultiType p);

extern Set* nfa_delta(NFA* nfa, MultiType state, char a);

extern bool nfa_accept(NFA* nfa, char* word);

extern void nfa_free(NFA* nfa);

#endif // AUTOMATON_H