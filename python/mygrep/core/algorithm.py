from itertools import count
from collections import defaultdict

from .automaton import NFA, EPSILON


def brzozowski(dfa):
    """
    DMinimize dfa automaton with the Brzozowski algorithm.
    """
    return dfa.transpose().determinize().transpose().determinize()


def thompson(ast, state=count(0)):
    """
    Construct a NFA from an AST with the Thompson algorithm.
    """
    match ast.tag:
        case "CharGroup":
            initial, final = next(state), next(state)
            nfa = NFA({initial}, {final}, defaultdict(dict))
            for c in ast.childs:
                nfa.set_transition(initial, c, final)
        case "Union":
            nfa, nfa2 = thompson(ast.childs[0]), thompson(ast.childs[1])
            nfa._transitions.update(nfa2._transitions)
            nfa.initial |= nfa2.initial
            nfa.final |= nfa2.final
        case "Concat":
            nfa, nfa2 = thompson(ast.childs[0]), thompson(ast.childs[1])
            nfa._transitions.update(nfa2._transitions)
            for f in nfa.final:
                for i in nfa2.initial:
                    nfa.set_transition(f, EPSILON, i)
            nfa.final = nfa2.final
        case "Star":
            nfa = thompson(ast.childs[0])
            for f in nfa.final:
                for i in nfa.initial:
                    nfa.set_transition(i, EPSILON, f)
                    nfa.set_transition(f, EPSILON, i)
    return nfa
