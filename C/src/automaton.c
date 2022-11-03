/**
 * Implements DFA and NFA structures
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "automaton.h"
#include "linkedlist.h"
#include "multitype.h"
#include "hashtable.h"

const EPSILON = '\0';

static MultiType dfa_delta_star(DFA* dfa, MultiType state, char* u)
{
    for (int i = 0; u[i] != '\0'; i++)
        state = dfa_delta(dfa, state, u[i]);

    return state;
}

static Set* nfa_epsilon_closure(NFA* nfa, Set* states, char a)
{
    Set* closure = hashtable_copy(states);
    Stack* stack = stack_from_set(closure);

    while (!stack_is_empty(stack))
    {
        MultiType q = stack_pop(stack);
        HashTable* q_trans = double_hashtable_get(nfa->_transitions, q);

        if (!hashtable_contains(q_trans, multi_char(EPSILON)))
            continue;
        // Add to closure all states instantly accessible from q 
        Set* q_closure = double_hashtable_get(q_trans, multi_char(EPSILON));
        MultiType* q_closure_elems = hashtable_keys(q_closure);

        for (int i = 0; i < q_closure->size; i++)
        {
            if (set_contains(closure, q_closure_elems[i]))
                continue;
            stack_push(stack, q_closure_elems[i]);
            set_add(closure, q_closure_elems[i]);
        }
        free(q_closure_elems);
    }
    stack_free(stack);
    return closure;
}

static Set* nfa_delta_states(NFA* nfa, Set* states, char a)
{
    Set* next_states = nfa_epsilon_closure(nfa, states, a);
    MultiType* states_elems = hashtable_keys(states);

    for (int i = 0; i < states->size; i++)
        hashtable_update(next_states, nfa_delta(nfa, states_elems[i], a));

    free(states_elems);
    return next_states;
}

static Set* nfa_delta_star(DFA* dfa, Set* states, char* u)
{
    for (int i = 0; u[i] != '\0'; i++)
        states = nfa_delta_states(dfa, states, u[i]);
        
    return states;
}

DFA* dfa_create(char* alphabet, Set* states, MultiType initial, 
                Set* final, DoubleHashTable* transitions)
{
    DFA* dfa = (DFA*)malloc(sizeof(DFA));
    dfa->alphabet = alphabet;
    dfa->states = states;
    dfa->initial = initial;
    dfa->final = final;
    dfa->_transitions = transitions;
    return dfa;
}

void dfa_set_transition(DFA* dfa, MultiType state, char a, MultiType p)
{
    if (hashtable_contains(dfa->_transitions, state)) {
        HashTable* h = double_hashtable_get(dfa->_transitions, state);
        hashtable_set(h, multi_char(a), p);
    } else {
        HashTable* h = hashtable_create(2);
        hashtable_set(h, multi_char(a), p);
        hashtable_set(dfa->_transitions, state, multi_pointer(h));
    }
}

MultiType dfa_delta(DFA* dfa, MultiType state, char a)
{
    HashTable* h = double_hashtable_get(dfa->_transitions, state);
    return hashtable_get(h, multi_char(a));
}

bool dfa_accept(DFA* dfa, char* u)
{
    MultiType state = dfa_delta_star(dfa, dfa->initial, u);
    return hashtable_contains(dfa->final, state);
}

NFA* dfa_create(char* alphabet, Set* states, Set* initial, 
                Set* final, DoubleHashTable* transitions)
{
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    nfa->alphabet = alphabet;
    nfa->states = states;
    nfa->initial = initial;
    nfa->final = final;
    nfa->_transitions = transitions;
    return nfa;
}

bool nfa_accept(NFA* nfa, char* u)
{
    Set* states = nfa_delta_star(nfa, nfa->initial, u);
    MultiType* states_elems = hashtable_keys(states);
    bool accept = false;

    for (int i = 0; i < states->size; i++) 
    {
        if (set_contains(nfa->final, states_elems[i]))
            accept = true;
    }
    free(states_elems);
    set_free(states);
    return accept;
}
