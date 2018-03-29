#! /usr/bin/python3

import argparse

from AsmEncoder import AsmEncoder
from Assembler import Assembler


def main():
    m = argparse.ArgumentParser()
    m.add_argument("--input", "-i", type=str)
    m.add_argument("--output", "-o", type=str)
    options = m.parse_args()

    asm = AsmEncoder()
    tree = asm.parse(options.input)
    coded = Assembler(tree)
    with open(options.output, "wb") as f:
        f.write(coded.state)


if __name__ == "__main__":
    main()

