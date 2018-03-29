#! /usr/bin/python3

import argparse

from Disassembler import Disassembler

def main():
    m = argparse.ArgumentParser()
    m.add_argument("--input", "-i", type=str)
    m.add_argument("--output", "-o", type=str)
    options = m.parse_args()

    DisAsm = Disassembler()
    DisAsm.disassembly(options.input, options.output)


if __name__ == "__main__":
    main()
