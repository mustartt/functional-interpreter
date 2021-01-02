""" Functional Language Interpreter

Interpreter implementation. 
"""
import os
import sys
from pathlib import PurePath
from typing import List, Dict, Tuple, Any

from Scope import Scope, IdentifierNotFoundInScope, get_global_scope

class InvalidArgument(Exception):
    """ InvalidArgument is raised when the number of arguement passed to a
    AnonymousFunction does not match the number of arguement required by the
    AnonymousFunction
    """
    pass

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
    binaries_location: PurePath
        the absolute path where the binaries are located. This is also where
        the lang/library should be found in

        
    #TODO: to consolidated
    call_depth: int

    
    Functions 
    ---------
    evaluate(ast, scope=global_scope)

    _load_library(filename)
        
    open_file(filename)

    reset()
    
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
        self.binaries_location = PurePath(sys.argv[0]).parent


    def reset() -> None:
        """ Resets the Interpreter to the original state """
        self.global_scope = get_global_scope()
        self.library_loaded = []
        self.call_depth = 0 # not in a call right now


    def _load_library(self, filename: str) -> str:
        """ Link files together recursively

        TODO: Instead of using string filenames use Path instead
        
        :param filename: the filename to be loaded
        :type  filename: str

        :returns: the string content of the linked files
        :rtype: str
        """

        # check if it is already loaded
        if filename in self.library_loaded:
            return ''
        else:
            self.library_loaded.append(filename)


        # try opening file at current location, otherwise try to find at library
        try: 
            with open(filename, 'rt') as file:
                content = file.read()
        except FileNotFoundError:
            # file is not found under local
            try: 
                with open(self.binaries_location / filename) as file:
                    content = file.read()
            except Exception as err:
                raise FileNotFoundError(f'{filename} is not found in cwd or library')

        start = content.find('load')
        end   = content.find(';', start)

        # recursively links language files together
        while start >= 0:
            filename_toload = content[start + len('load ') : end]
            loaded_content = self._load_library(filename_toload)
            
            # rejoin content
            content = content[:start] + '\n' + loaded_content + '\n' + content[end + 1:]
            
            start = content.find('load')
            end   = content.find(';', start)

        return content


        
    def open_file(self, filename: str) -> List[str]:
        """ open_file load the language file at filename and returns a
        list of statements while parsing out comments
        """
        # link the files together
        content = self._load_library(filename)
        
        # remove comments # -> '\n'
        index = content.find('#')
        while index >= 0:
            end = content.find('\n', index + 1)
            content = content[:index] + content[end+1:]
            index = content.find('#')

        content = re.sub(r'(\t|\s+)', ' ', content)
        return [s.strip() for s in content.split(';') if s.strip() != '']



    def evaluate(self, at, scope=None) -> Any:
        """ Evalues the Action Tree 
        
        evaluate takes a Action Tree and a scope and returns the result of the
        evaluation. 

        :param at: the action tree to be evaluated
        :type  at: nested list
        :param scope: the current scope of the call stack
        :type  scope: dict

        :returns: the evaulated result
        :rtype: Any
        """

        # check if it is currently in global scope
        if not scope:
            scope = self.global_scope

        def check_args(at, n_args: int) -> None:
            if len(at) - 1 != n_args:
                raise InvalidArgument('Invalid number of positional arguments found. '
                                      + f'Expected {len(at) - 1}. Found {n_args}.')

        # evaluation decision tree
        if isinstance(at, str):
            return scope.lookup(at)
        elif not isinstance(at, list):
            return at
        elif at[0] == 'quote':
            check_args(at, 1)
            (_, exp) = at
            return exp
        elif at[0] == 'display':
            check_args(at, 1)
            (_, exp) = at
            # TODO: write to stdout
            return exp
        elif at[0] == 'if':
            check_args(at, 3)
            (_, test, conseq, alt) = at
            exp = (conseq if self.evaluate(test, scope) else alt)
            return self.evaluate(exp, scope)
        elif at[0] == 'bind':
            check_args(at, 3)
            (_, var, _, exp) = at
            scope[var] = self.evaluate(exp, scope)
        elif at[0] == 'lambda':
            check_args(at, 3)
            (_, parms, _, exp) = at
            return AnonymousFunction(parms, exp, scope, self)
        else:
            proc = self.evaluate(at[0], scope)
            args = [self.evaluate(exp, scope) for exp in at[1:]]
            return proc(*args)


