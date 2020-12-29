""" Functional Language Interpreter

Interpreter implementation. 
"""

class Function(object):
    """ Function and Procedure
    TODO: Write the documentation
    """

    def __init__(self, parameters, body, scope):
        self.parameters = parameters
        self.body = body
        self.scope = scope

    def __call__(self, *args):
        pass

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
    
    Functions
    ---------
    evaluate(ast, scope=global_scope)

    __load_library(filename)
        
    open_file(filename)
    
    """

    def __init__(self):
        pass

    def __load_library(self, filename: str) -> str:
        """ Link files together recursively
        
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

    def evaluate(self, ast, scope=self.global_scope):
        pass

    



























