from typing import Set, Dict, Union, Text

import string
from collections import defaultdict, deque
from dataclasses import dataclass


Letter = str
State = Letter
States = Set[State]

EPSILON: Letter = str()  # représente le mot vide
ASCII_ALPHABET: Text = set(string.printable)


@dataclass
class DFA:
    """
    Représente un automate fini déterministe à 1 état initial
    """

    alphabet: Text
    states: States
    initial: State
    final: States
    __transitions: Dict[State, Dict[Letter, State]]

    def set_transition(self, q: State, a: Letter, p: State) -> None:
        self.__transitions[q][a] = p

    def delta(self, q: State, a: Letter) -> State:
        return self.__transitions[q].get(a, State())

    def _delta_star(self, q: State, u: Text) -> State:
        i = 0
        while i < len(u):
            q = self.delta(q, u[i])
            i += 1
        return q

    def accept(self, word: Text) -> bool:
        return self._delta_star(self.initial, word) in self.final


@dataclass
class NFA:
    """
    Représente un automate fini non déterministe à 1 état initial,
    avec epsilon transitions.
    """

    alphabet: Text
    states: States
    initial: State
    final: States
    __transitions: Dict[State, Dict[Letter, States]]

    def set_transition(self, q: State, a: Letter, p: State) -> None:
        self.__transitions[q][a] = self.__transitions[q].get(a, set()) | {p}

    def _epsilon_closure(self, states: States) -> States:
        queue = deque(states)
        closure = states.copy()
        while len(queue) > 0:
            q = queue.pop()
            for q_eps in self.__transitions[q].get(EPSILON, set()):
                if q_eps not in states:
                    closure.add(q_eps)
                    queue.append(q_eps)
        return closure

    def delta(self, q: State, a: Letter) -> States:
        return self.__transitions[q].get(a, set()) | self._epsilon_closure({q})

    def _delta_states(self, states: States, a: Letter) -> States:
        next_states = self._epsilon_closure(states)
        i = 0
        for q in states:
            next_states |= self.delta(q, a)
            i += 1
        return next_states

    def _delta_star(self, states: States, u: Text) -> States:
        i = 0
        while i < len(u):
            states = self._delta_states(states, u[i])
            i += 1
        return states

    def accept(self, u: Text) -> bool:
        return len(self.final & self._delta_star({self.initial}, u)) > 0


class AutoOccurrences(DFA):
    """
    Représente l'automate des occurrences associé à un motif.
    """

    def __init__(self, pattern: Text):
        self.pattern = pattern
        super().__init__(
            ASCII_ALPHABET, {EPSILON}, EPSILON, {pattern}, defaultdict(dict)
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

    def __longest_suffix(self, word: Text) -> Text:
        if word in self.states:
            return word
        return self.__longest_suffix(word[1:])
