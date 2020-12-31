""" Functional Language Interpreter

Interpreter implementation. 
"""

from typing import List, Dict, Tuple, Any

from Scope import Scope, IdentifierNotFoundInScope, get_global_scope

class AnonymousFunction(object):
    """ Function and Procedure
    TODO: Write the documentation

    Attributes
    ----------
    parameters: list
    body: AbstractSyntaxTree
    scope: Scope

    interpreter: Interpreter
    
    Functions
    ---------
    """

    def __init__(self, parameters, body, scope, interpreter):
        """ Constructor for AnonymousFunction """
        self.parameters = parameters
        self.body = body
        self.scope = scope
        self.interpreter = interpreter

    def __call__(self, *args):
        """ Evaluates AnonymousFunction body
        TODO: write documentation
        """
        call_stack = Scope(self.parameters,
                           args,
                           self.scope)
        # TODO: Maybe increment call_depth in interpreter
        return self.interpreter.evaluate(self.body, call_stack)



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

    #TODO: to consolidated
    call_depth: int

    execution_location: Path
    library_location: Path
    
    Functions 
    ---------
    evaluate(ast, scope=global_scope)

    __load_library(filename)
        
    open_file(filename)
    
    """

    def __init__(self):
        """ Interpreter Constructor
        TODO: Write documentation
        """

        # setup default parameters
        self.global_scope = get_global_scope()
        self.library_loaded = []
        self.call_depth = 0 # not in a call right now

        # interpreter paths setup
        # self.execution_location = ?



    def __load_library(self, filename: str) -> str:
        """ Link files together recursively

        TODO: Instead of using string filenames use Path instead
        
        :param filename: the filename to be loaded
        :type  filename: str

        :returns: the string content of the linked files
        :rtype: str
        """

        # check if it is already loaded
        if filename in self.loaded:
            return ''
        else:
            self.loaded.append(filename)

        # open file
        with open(filename, 'rt') as file:
            content = file.read()

        start = content.find('load')
        end   = content.find(';', start)

        # recursively links language files together
        while start >= 0:
            filename_toload = content[start + len('load ') : end]
            loaded_content = __load_library(filename_toload)
            
            content = content[:start] + '\n' + loaded_content + '\n' + content[end + 1:]
            
            start = content.find('load')
            end   = content.find(';', start)

        return content

        
    def open_file(self, filename: str) -> List[str]:
        pass

    def evaluate(self, x, scope=None):
        # check if it is currently in global scope
        if not scope:
            scope = self.global_scope()

        # evaluation decision tree
        if isinstance(x, str):
            return scope.lookup(x)
        elif not isinstance(x, list):
            return x
        elif x[0] == 'quote':
            (_, exp) = x
            return exp
        elif x[0] == 'if':
            (_, test, conseq, alt) = x
            #print(f'test: {test}? {conseq} : {alt}')
            exp = (conseq if self.evaluate(test, scope) else alt)
            return self.evaluate(exp, scope)
        elif x[0] == 'bind':
            (_, var, _, exp) = x
            scope[var] = self.evaluate(exp, scope)
        elif x[0] == 'lambda':
            (_, parms, _, exp) = x
            return AnonymousFunction(parms, exp, scope, self)
        else:
            proc = self.evaluate(x[0], scope)
            args = [self.evaluate(exp, scope) for exp in x[1:]]
            return proc(*args)

    



























