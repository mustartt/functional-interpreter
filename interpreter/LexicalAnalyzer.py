""" Lexical Analyzer

The lexical analyzer module for the interpreted language. This module implements
the parsing for Abstract Syntax Tree

Abstract Syntax Tree
--------------------

#TODO: Add definitions for Abstract Syntax Tree

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

    # test for declaration
    # bind does not require external bracket in language documentation
    if program.startswith('bind'):
        program = '(' + program + ')'

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
     :rtype:   int, float, and str
    """

    # check if token is integer
    try:
        return int(token)
    except:
        pass

    # check if token is float
    try:
        return float(token)
    except:
        pass

    # TODO: checks for struct
    # Somehow unable to parse the datatype
    try:
        return str(token)
    except:
        raise ValueError('Unable to parse datatype.')
        




def abstract_syntax_tree(tokens: List[str]) -> List[Any]:
    """ Parses tokens into a Abstract Syntax Tree

    Parses a list of tokens into a Abstract Syntax Tree. 

    Abstract Syntax Tree Definition
    --------------------------------
    An abstract syntax tree is a nested list of Procedures
    
    [Procedure, Param_1, ..., Param_2]

    The first item of an abstract syntax tree has to be either a
    procedure or a literal if the length of the AST is 1. The parameters
    can either be literal, identifier, AST or procedures.

    A procedure is a lambda expression with parameter and expressions

    ['lambda', ['p_1', ..., 'p_n'], exp]

    The procedure takes the n number of parameters outside and after the
    procedure and applies the positional arguments to their respective
    location in exp. A procedure can just be an indentifier, and the actual
    lambda is stored in a higher scope and subsituted in at runtime evaluation.
    
    :param tokens: a list of string tokens
    :type  tokens: list

    :returns: the generated Abstract Syntax Tree
    :rtype:   List[Any] nested list
    """

    # define the delimiters for the parser
    opening = ['(', '{', '[']
    ending  = [')', '}', ']']

    if len(tokens) == 0:
        raise SyntaxError('Expected an expression, but found none.')

    # get the first token
    token = tokens.pop(0)

    # parse Expression
    if token in opening:
        # Lambda Expression
        result = []
        # parse until closing delimiter is found
        while not tokens[0] in ending:
            subtree = abstract_syntax_tree(tokens)
            result.append(subtree)
        # parse out the closing delimiter
        tokens.pop(0)

        # check if lambda function has parameters attached
        # ex. (lambda({x} -> x))(param)
        if tokens and tokens[0] in opening:
            tokens.pop(0) # pop (
            # parse rest of parameters
            while not tokens[0] in ending:
                subtree = abstract_syntax_tree(tokens)
                result.append(subtree)
            # pop out the closing delimiter
            tokens.pop(0)

        # return subtree
        return result
    # checks for if tokens is empty
    elif tokens and tokens[0] in opening:
        # Function Call
        result = [token]
        tokens.pop(0) # pop the opening delimiter
        while not tokens[0] in ending:
            subtree = abstract_syntax_tree(tokens)
            result.append(subtree)
        # pop out the closing delimiter
        tokens.pop(0)
        return result
    else:
        # a literal
        return atomize(token)


    


def parse(program: str) -> List[Any]:
    """ Parses program text into an Abstract Syntax Tree
    
    :param program: the program text
    :type  program: str

    :returns: the Abstract Syntax Tree parsed from the program text
    :retype: List[Any] Abstract Syntax Tree
    """
    
    tokens = tokenize(program)
    return abstract_syntax_tree(tokens)



















