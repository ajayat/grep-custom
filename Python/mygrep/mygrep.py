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
    pass


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


if __name__ == "__main__":
    main(len(sys.argv), sys.argv)
