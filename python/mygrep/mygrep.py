from typing import TextIO, List
import sys

from .core.parser import parse
from .core.algorithm import thompson, brzozowski


def mygrep(pattern: str, text: TextIO) -> None:
    """
    Affiche les lignes de text qui contiennent pattern.

    Args:
        pattern: la chaîne à chercher
        text: le texte à lire
    """
    ast = parse(pattern)
    ast.print()
    nfa = thompson(ast)
    dfa = brzozowski(nfa.determinize())

    for line in text:
        if dfa.accept(line.rstrip('\n')):
            print(line.rstrip('\n'))


def main(argc: int, argv: List[str]) -> None:
    """
    Traitement des arguments de la ligne de commande.
    """
    if argc == 3:
        with open(argv[2], "r") as file:
            mygrep(argv[1], file)
    elif argc == 2:
        mygrep(argv[1], sys.stdin)
    else:
        print("Usage: mygrep.py <pattern> <file>")
