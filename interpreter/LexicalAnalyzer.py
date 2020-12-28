""" Lexical Analyzer

The lexical analyzer module for the interpreted language. This module implements
the parsing for Abstract Syntax Tree

This module can be imported as a module and contains the following functions:
    * parse       : returns the parsed Abstract Syntax Tree
    * tokenize    : returns the program parsed as a list of tokens
    * syntax_tree : generates the Abstract Syntax Tree from list of tokens
    * atomize     : parses tokens to the appropriate datatype
"""

import re
from typing import List, Any


def tokenize(program: str) -> List[str]:
    pass


def abstract_syntax_tree(tokens: List[str]) -> List[Any]:
    pass


def parse(program: str) -> List[Any]:
    return abstract_syntax_tree(tokenize(program))


