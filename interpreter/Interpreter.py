""" Functional Language Interpreter

Interpreter implementation. 
"""


class Interpreter:
    """ Functional Language Interpreter

    TODO: Write description for Interpreter Class
    
    Provides the basic implementation of recursive descent evaluation for
    expressions. Implements library loading and file handling

    Attributes
    ----------
    global_scope: Scope
        a dictionary of Dict[str, Function] with a string indentifier as the
        key and Function as the value
    library_loaded: list
        a list of filename stored to ensure that functions are not loaded
        twice in the global scope
    
    Methods
    -------
    evaluate(ast, scope=global_scope)

    __load_library(filename)

    open_file(filename) 
    """
