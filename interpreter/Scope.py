
from typing import List, Tuple, Dict, Any

class IdentifierNotFoundInScope(Exception):
    """ IdentifierNotFoundInScope Exception

    If the identifer is not found with in the scopes leading up to
    the global scope, then this Exception IdentifierNotFoundInScope
    will be raised to signal to the interpreter to raise error

    """
    pass

class Scope(dict):
    """ Scope
    TODO: Write object implementation details
    """

    def __init__(self, identifiers=(), values=(), parent=None):
        # update the scope dictionary
        self.update(zip(identifiers, values))
        self.parent = parent

    def lookup(self, identifier: str) -> Any:
        """ Recursivly looks up the indentifer is current Scope and parent Scope

        :param identifier: the indentifier to be looked up
        :type  identifier: str

        :returns: The value associated with the identifier
        :type: Any

        :raises: IdentifierNotFoundInScope when identifier is not found
        """
        
        # check for indentifier in self
        if identifier in self:
            return self[identifier]
        elif self.parent == None:
            # is the global scope and indentifier is not found
            raise IdentifierNotFoundInScope(f'The indentifier {identifier} cannot be located.')
        else:
            return self.parent.lookup(identifier)

