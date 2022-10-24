from typing import Set, Dict, Tuple
import string


class ADC1:
    """
    Représente un automate fini déterministe et complet à 1 état initial
    """

    def __init__(self):
        self.alphabet: Set[str] = set()
        self.states: Set[str] = set()
        self.initial: str = ""
        self.final: Set[str] = set()
        self.__transitions: Dict[Tuple[str, str], str] = {}

    def add_transition(self, state: str, letter: str, value: str) -> None:
        self.__transitions[(state, letter)] = value

    def _delta(self, state: str, letter: str) -> str:
        return self.__transitions[(state, letter)]

    def _delta_etoile(self, state: str, word: str) -> str:
        for c in word:
            state = self._delta(state, c)
        return state

    def accept(self, word: str) -> bool:
        return self._delta_etoile(self.initial, word) in self.final


class AutoOccurrences(ADC1):
    """
    Représente l'automate des occurrences associé à un motif.
    """

    def __init__(self, pattern: str):
        super().__init__()
        self.pattern = pattern
        self.alphabet = set(string.printable)
        self._create_states()
        self.final = {pattern}
        self._create_transitions()

    def _create_states(self) -> None:
        for i in range(len(self.pattern) + 1):
            prefix = self.pattern[:i]
            self.states.add(prefix)

    def _create_transitions(self) -> None:
        for p in self.states:
            for a in self.alphabet:
                self.add_transition(p, a, self._longest_suffix(p + a))

    def _longest_suffix(self, word: str) -> str:
        for i in range(len(word)):
            if word[i:] in self.states:
                return word[i:]
        return ""
