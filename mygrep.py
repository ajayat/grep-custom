from typing import TextIO, List
import sys

from automaton import AutoOccurrences


def mygrep(pattern: str, text: TextIO) -> None:
    """
    Affiche les lignes de text qui contiennent pattern.

    Args:
        pattern: la chaîne à chercher
        text: le texte à lire
    """
    auto = AutoOccurrences(pattern)
    # On modifie les transitions partant de l'état final
    for a in auto.alphabet:
        auto.add_transition(pattern, a, pattern)

    # On peut désormais rechercher le motif
    for line in text:
        if auto.accept(line):
            print(line, end="")


def main(argc: int, argv: List[str]) -> None:
    if argc == 3:
        with open(argv[2], "r") as file:
            mygrep(argv[1], file)
    elif argc == 2:
        mygrep(argv[1], sys.stdin)
    else:
        print("Usage: mygrep.py <pattern> <file>")


if __name__ == "__main__":
    main(len(sys.argv), sys.argv)