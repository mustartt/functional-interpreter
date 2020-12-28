""" Lexical Analyzer

The lexical analyzer module for the interpreted language. This module implements
the parsing for Abstract Syntax Tree

This module can be imported as a module and contains the following function:
    * parse       : returns the parsed Abstract Syntax Tree

    Functions
    ---------
    * tokenize    : returns the program parsed as a list of tokens
    * syntax_tree : generates the Abstract Syntax Tree from list of tokens
    * atomize     : parses tokens to the appropriate datatype
"""


import re

from typing import List, Any



def tokenize(program: str) -> List[str]:
    """ Tokenizes the program text into a list of Tokens

    Preformat program text and then parses the program into the a list of
    tokens based on spaces and brackets. Statements are postfixed by ';'

    :param program: the program text to be tokenized
    :type  program: str

    :returns: a list of strings of tokens
    :rtype: list
    """

    # Performatting the program text to remove tabs and returns
    program = re.sub(r'[\n\t]', '', program.replace(';', ''))

    # define language seperator
    seperator = ['(', ')', '[', ']', '{', '}']
    for sep in seperator:
        program = program.replace(sep, f' {sep} ')

    # remove duplicate spaces
    program = re.sub(r'\s+', ' ', program)

    # tokenize program
    return program.split()
    


def atomize(token: str) -> Any:
    """ Parse string token to respective Python Datatype

     * If the string token is a int, parse it to an integer
     * If the string token is a float, parse it to a float
     * TODO: Implement Parsing for structs
     * Else keep the token as a string

     Requires: token is not empty

     :param token: a string token to be parsed
     :type  token: str

     :returns: the parsed Python Datatype
     :rtype: int, float, and str
    """

    try:
        # check if token is integer
        return int(token)
    except:
        try:
            # check if token is float
            return float(token)
        except:
            try:
                # TODO: checks for struct
                return str(token)
            except:
                # Somehow unable to parse the datatype
                raise ValueError('Unable to parse datatype.')
        


def abstract_syntax_tree(tokens: List[str]) -> List[Any]:
    pass
    

def parse(program: str) -> List[Any]:
    """ Parses program text into an Abstract Syntax Tree
    
    :param program: the program text
    :type  program: str

    :returns: the Abstract Syntax Tree parsed from the program text
    :retype: List[Any] Abstract Syntax Tree
    """
    
    return abstract_syntax_tree(tokenize(program))



















