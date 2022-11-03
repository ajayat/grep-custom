from typing import Set, Dict

import string
from collections import defaultdict, deque
from dataclasses import dataclass


Letter = str
State = Letter
States = Set[State]

EPSILON: Letter = str()  # représente le mot vide
ASCII_ALPHABET: str = set(string.printable)


@dataclass
class DFA:
    """
    Représente un automate fini déterministe à 1 état initial
    """

    alphabet: str
    states: States
    initial: State
    final: States
    __transitions: Dict[State, Dict[Letter, State]]

    def set_transition(self, q: State, a: Letter, p: State) -> None:
        self.__transitions[q][a] = p

    def delta(self, q: State, a: Letter) -> State:
        return self.__transitions[q].get(a, State())

    def _delta_star(self, q: State, u: str) -> State:
        for c in u:
            q = self.delta(q, c)
        return q

    def accept(self, word: str) -> bool:
        return self._delta_star(self.initial, word) in self.final


@dataclass
class NFA:
    """
    Représente un automate fini non déterministe avec epsilon transitions.
    """

    alphabet: str
    states: States
    initial: States
    final: States
    __transitions: Dict[State, Dict[Letter, States]]

    def set_transition(self, q: State, a: Letter, p: State) -> None:
        self.__transitions[q][a] = self.__transitions[q].get(a, set()) | {p}

    def _epsilon_closure(self, states: States) -> States:
        queue = deque(states)  # stack for DFS
        closure = states.copy()
        while len(queue) > 0:
            q = queue.pop()
            for q_eps in self.__transitions[q].get(EPSILON, set()):
                if q_eps not in closure:
                    closure.add(q_eps)
                    queue.append(q_eps)
        return closure

    def delta(self, q: State, a: Letter) -> States:
        return self.__transitions[q].get(a, set()) | self._epsilon_closure({q})

    def _delta_states(self, states: States, a: Letter) -> States:
        next_states = self._epsilon_closure(states)
        for q in states:
            next_states |= self.delta(q, a)
        return next_states

    def _delta_star(self, states: States, u: str) -> States:
        for c in u:
            states = self._delta_states(states, c)
        return states

    def accept(self, u: str) -> bool:
        return len(self.final & self._delta_star(self.initial, u)) > 0


class AutoOccurrences(DFA):
    """
    Représente l'automate des occurrences associé à un motif.
    """

    def __init__(self, pattern: str):
        self.pattern = pattern
        super().__init__(
            ASCII_ALPHABET, {EPSILON}, {EPSILON}, {pattern}, defaultdict(dict)
        )
        self.__create_states()
        self.__create_transitions()

    def __create_states(self) -> None:
        for i in range(1, len(self.pattern) + 1):
            self.states.add(self.pattern[:i])

    def __create_transitions(self) -> None:
        for p in self.states:
            for a in self.alphabet:
                self.set_transition(p, a, self.__longest_suffix(p + a))

    def __longest_suffix(self, word: str) -> str:
        if word in self.states:
            return word
        return self.__longest_suffix(word[1:])
