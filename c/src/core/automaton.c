/**
 * Implements DFA and NFA structures
 */

#include "automaton.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"
#include "multitype.h"
#include "vector.h"

const char EPSILON = '\0';
const int HT_INIT_SIZE = 2;
const char ALPHABET[] =
    "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";

/* Creates a Deterministic Finite Automaton */
DFA* dfa_create(MultiType initial)
{
    DFA* dfa = (DFA*)malloc(sizeof(DFA));
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
    if (a == EPSILON)
        return state;
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

NFA* dfa_transpose(DFA* dfa)
{
    NFA* nfa_tr = nfa_create();
    nfa_tr->initial = hashtable_copy(dfa->final);
    nfa_tr->final = hashtable_create(2);
    hashtable_set(nfa_tr->final, dfa->initial, dfa->initial);

    Vector* states = hashtable_to_vector(dfa->_transitions);
    for (int i = 0; i < states->size; i++) {
        MultiType q = states->array[i];
        HashTable* transitions = hashtable_get_or_create(dfa->_transitions, q);

        Vector* letters = hashtable_to_vector(transitions);
        for (int j = 0; j < letters->size; j++) {
            MultiType a = letters->array[j];
            MultiType p = hashtable_get(transitions, a);
            nfa_set_transition(nfa_tr, p, (char)a.value.c, q);
        }
        vector_free(letters);
    }
    vector_free(states);
    return nfa_tr;
}

void dfa_free(DFA* dfa, bool deep)
{
    hashtable_free(dfa->final, deep);
    hashtable_free(dfa->_transitions, deep);
    free(dfa);
}

/* Creates a Non-deterministic Finite Automaton */
NFA* nfa_create(void)
{
    NFA* nfa = (NFA*)malloc(sizeof(NFA));
    nfa->initial = hashtable_create(HT_INIT_SIZE);
    nfa->final = hashtable_create(HT_INIT_SIZE);
    nfa->_transitions = hashtable_create(HT_INIT_SIZE);
    return nfa;
}

void nfa_set_transition(NFA* nfa, MultiType state, char a, MultiType p)
{
    HashTable* h = hashtable_get_or_create(nfa->_transitions, state);
    Set* states = hashtable_get_or_create(h, multi_char(a));
    hashtable_set(states, p, p);
    hashtable_set(h, multi_char(a), multi_htbl(states));
}

Set* nfa_delta(NFA* nfa, MultiType state, char a)
{
    HashTable* h = hashtable_get_or_create(nfa->_transitions, state);
    Set* states = hashtable_get_or_create(h, multi_char(a));
    if (a == EPSILON)
        hashtable_set(states, multi_char(EPSILON), multi_char(EPSILON));
    return states;
}

static Set* nfa_epsilon_closure(NFA* nfa, Set* states)
{
    Set* closure = hashtable_create(states->capacity);
    Vector* stack = hashtable_to_vector(states);

    while (stack->size > 0) {
        MultiType q = vector_pop(stack);
        if (hashtable_contains(closure, q))
            continue;

        hashtable_set(closure, q, q);
        Set* q_closure = nfa_delta(nfa, q, EPSILON);
        for (int b = 0; b < q_closure->capacity; b++) {
            for (Entry* e = q_closure->array[b]; e != NULL; e = e->next)
                vector_push(stack, e->key);
        }
        hashtable_free(q_closure, false);
    }
    vector_free(stack);
    return closure;
}

static Set* nfa_delta_states(NFA* nfa, Set* states, char a)
{
    Set* next_states = hashtable_create(HT_INIT_SIZE);

    for (int b = 0; b < states->capacity; b++) {
        for (Entry* e = states->array[b]; e != NULL; e = e->next) {
            if (e->key.type == NullType)
                continue;
            hashtable_update(next_states, nfa_delta(nfa, e->key, a));
        }
    }
    Set* closure = nfa_epsilon_closure(nfa, next_states);
    hashtable_free(next_states, false);
    return closure;
}

static Set* nfa_delta_star(NFA* nfa, Set* states, char* u)
{
    for (int i = 0; u[i] != '\0'; i++) {
        Set* states_temp = nfa_delta_states(nfa, states, u[i]);
        hashtable_free(states, false);
        states = states_temp;
    }
    return states;
}

bool nfa_is_final(NFA* nfa, Set* states)
{
    for (int b = 0; b < states->capacity; b++) {
        for (Entry* e = states->array[b]; e != NULL; e = e->next) {
            if (hashtable_contains(nfa->final, e->key))
                return true;
        }
    }
    return false;
}

bool nfa_accept(NFA* nfa, char* u)
{
    Set* states = nfa_epsilon_closure(nfa, nfa->initial);
    states = nfa_delta_star(nfa, states, u);
    bool accept = nfa_is_final(nfa, states);
    hashtable_free(states, false);
    return accept;
}

DFA* nfa_determinize(NFA* nfa)
{
    DFA* dfa = dfa_create(multi_char(EPSILON));
    Vector* stack = vector_create(HT_INIT_SIZE);
    vector_push(stack, multi_htbl(nfa->initial));

    while (stack->size > 0) {
        MultiType state = vector_pop(stack);
        if (nfa_is_final(nfa, (Set*)state.value.p))
            hashtable_set(dfa->final, state, state);

        for (int i = 0; i < (int)strlen(ALPHABET); i++) {
            Set* p = nfa_delta_states(nfa, (Set*)state.value.p, ALPHABET[i]);
            if (!hashtable_contains(nfa->_transitions, multi_htbl(p)))
                vector_push(stack, multi_htbl(p));
            dfa_set_transition(dfa, state, ALPHABET[i], multi_htbl(p));
        }
    }
    vector_free(stack);
    return dfa;
}

void nfa_free(NFA* nfa, bool deep)
{
    hashtable_free(nfa->initial, deep);
    hashtable_free(nfa->final, deep);
    hashtable_free(nfa->_transitions, deep);
    free(nfa);
}