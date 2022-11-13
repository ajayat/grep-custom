from typing import List, Dict, Union, Optional, Set

import string
from collections import defaultdict, deque
from dataclasses import dataclass


State = Union[str, int]

EPSILON: str = str()  # représente le mot vide
ALPHABET: List[str] = set(string.ascii_letters)
EMPTY_STATE: State = None


@dataclass
class DFA:
    """
    Représente un automate fini déterministe à 1 état initial
    """
    initial: State
    final: Set[State]
    _transitions: Dict[State, Dict[str, State]]

    def set_transition(self, q: State, a: str, p: State) -> None:
        self._transitions[q][a] = p

    def delta(self, q: State, a: str) -> State:
        if a == EPSILON:
            return q
        return self._transitions[q].get(a, EMPTY_STATE)

    def _delta_star(self, q: State, u: str) -> State:
        for c in u:
            q = self.delta(q, c)
        return q

    def accept(self, word: str) -> bool:
        return self._delta_star(self.initial, word) in self.final

    def transpose(self) -> "NFA":
        nfa = NFA(self.final.copy(), {self.initial}, defaultdict(dict))
        for q in self._transitions:
            for a in self._transitions[q]:
                nfa.set_transition(self._transitions[q][a], a, q)
        return nfa


@dataclass
class NFA:
    """
    Représente un automate fini non déterministe avec epsilon transitions.
    """

    initial: Set[State]
    final: Set[State]
    _transitions: Dict[State, Dict[str, Set[State]]]

    def set_transition(self, q: State, a: str, p: State) -> None:
        self._transitions[q][a] = self._transitions[q].get(a, set()) | {p}

    def delta(self, q: State, a: str) -> Set[State]:
        states = self._transitions[q].get(a, set())
        if a == EPSILON:
            states.add(EPSILON)
        return states

    def _epsilon_closure(self, states: Set[State]) -> Set[State]:
        closure = set()
        stack = deque(states)
        while len(stack) > 0:
            q = stack.pop()
            if q not in closure:
                closure.add(q)
                stack.extend(self.delta(q, EPSILON))
        return closure

    def _delta_states(self, states: Set[State], a: str) -> Set[State]:
        next_states = set()
        for q in states:
            next_states |= self.delta(q, a)
        return self._epsilon_closure(next_states)

    def _delta_star(self, states: Set[State], u: str) -> Set[State]:
        for c in u:
            states = self._delta_states(states, c)
            print(states)
        return states

    def _is_final(self, states: Set[State]) -> bool:
        return any(q in self.final for q in states)

    def accept(self, u: str) -> bool:
        print(self.initial)
        states = self._delta_star(self._epsilon_closure(self.initial), u)
        return self._is_final(states)

    def determinize(self) -> DFA:
        dfa = DFA(set(), set(), defaultdict(dict))
        dfa.initial = frozenset(self._epsilon_closure(self.initial))

        stack = deque([dfa.initial])
        while len(stack) > 0:
            state = stack.pop()
            if self._is_final(state):
                dfa.final.add(state)

            for a in ALPHABET:
                p = frozenset(self._delta_states(state, a))
                if p not in dfa._transitions:
                    stack.append(p)
                dfa.set_transition(state, a, p)
        return dfa
