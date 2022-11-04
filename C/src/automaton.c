/**
 * Implements DFA and NFA structures
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "automaton.h"
#include "vector.h"
#include "multitype.h"
#include "hashtable.h"

const char EPSILON = '\0';
const int HT_INIT_SIZE = 2;

static const Set* ascii_alphabet(void)
{
    const Set* alphabet = set_create(128 * (float)(1 / HASHTABLE_LOAD_FACTOR));
    for (int i = 0; i < 128; i++) {
        MultiType elem = multi_char((char)i);
        hashtable_add(alphabet, elem, elem);
    }
    return alphabet;
}

/* Creates a Deterministic Finite Automaton */
DFA* dfa_create(char* alphabet, MultiType initial)
{
    DFA* dfa = (DFA*)malloc(sizeof(DFA));
    dfa->alphabet = alphabet;
    dfa->initial = initial;
    dfa->final = hashtable_create(HT_INIT_SIZE);
    dfa->_transitions = hashtable_create(HT_INIT_SIZE);
    return dfa;
}

void dfa_set_transition(DFA* dfa, MultiType state, char a, MultiType p)
{
    HashTable* h = hashtable_get_or_create(dfa->_transitions, state);
    hashtable_set(h, multi_char(a), p);
}

MultiType dfa_delta(DFA* dfa, MultiType state, char a)
{
    HashTable* h = hashtable_get_or_create(dfa->_transitions, state);
    return hashtable_get(h, multi_char(a));
}

static MultiType dfa_delta_star(DFA* dfa, MultiType state, char* u)
{
    for (int i = 0; u[i] != '\0'; i++)
        state = dfa_delta(dfa, state, u[i]);

    return state;
}

bool dfa_accept(DFA* dfa, char* u)
{
    MultiType state = dfa_delta_star(dfa, dfa->initial, u);
    return hashtable_contains(dfa->final, state);
}

/* Creates a Non-deterministic Finite Automaton */
NFA* nfa_create(char* alphabet)
{
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    nfa->alphabet = alphabet;
    nfa->initial = hashtable_create(HT_INIT_SIZE);
    nfa->final = hashtable_create(HT_INIT_SIZE);
    nfa->_transitions = hashtable_create(HT_INIT_SIZE);
    return nfa;
}

void nfa_set_transition(NFA* nfa, MultiType state, char a, MultiType p)
{
    HashTable* h = hashtable_get_or_create(nfa->_transitions, state);
    Set* states = hashtable_get_or_create(h, a);
    hashtable_add(states, p, p);
    hashtable_set(h, multi_char(a), states);
}

Set* nfa_delta(NFA* nfa, MultiType state, char a)
{
    HashTable* h = hashtable_get_or_create(nfa->_transitions, state);
    return hashtable_get_or_create(h, multi_char(a));
}

static Set* nfa_epsilon_closure(NFA* nfa, Set* states)
{   
    Set* closure = hashtable_copy(states);
    Vector* stack = hashtable_to_vector(states);

    while (stack->size > 0)
    {
        MultiType q = vector_pop(stack);
        if (!hashtable_contains(closure, q))
            continue;

        hashtable_add(closure, q, q);
        vector_extend(stack, hashtable_to_vector(nfa_delta(nfa, q, EPSILON)));
    }
    vector_free(stack);
    return closure;
}

static Set* nfa_delta_states(NFA* nfa, Set* states, char a)
{
    Set* next_states = hashtable_create(HT_INIT_SIZE);
    Vector* v_states = hashtable_to_vector(states);

    for (int i = 0; i < states->size; i++)
        hashtable_update(next_states, nfa_delta(nfa, v_states->array[i], a));

    Set* closure = nfa_epsilon_closure(nfa, next_states);
    vector_free(v_states);
    hashtable_free(next_states);
    return closure;
}

static Set* nfa_delta_star(DFA* dfa, Set* states, char* u)
{
    for (int i = 0; u[i] != '\0'; i++)
    {
        Set* states_temp = nfa_delta_states(dfa, states, u[i]);
        hashtable_free(states);
        states = states_temp;
    }
    return states;
}

bool nfa_is_final(NFA* nfa, Set* states)
{
    bool is_final = false;
    Vector* v_states = hashtable_to_vector(states);
    for (int i = 0; i < states->size; i++) 
    {
        if (set_contains(nfa->final, v_states[i]))
            is_final = true;
    }
    vector_free(v_states);
    return is_final;
}

bool nfa_accept(NFA* nfa, char* u)
{
    Set* initial_closure = nfa_epsilon_closure(nfa, nfa->initial);
    Set* states = nfa_delta_star(nfa, initial_closure, u);
    bool accept = nfa_is_final(nfa, states);
    hashtable_free(initial_closure);
    hashtable_free(states);
    return accept;
}
