import unittest

from interpreter.LexicalAnalyzer import tokenize
from interpreter.LexicalAnalyzer import atomize
from interpreter.LexicalAnalyzer import abstract_syntax_tree


class TestLexicalAnalyzer(unittest.TestCase):

    # tokenize unittest
    def test_tokenize_literal(self):
        # empty string returns empty tokens
        self.assertEqual(tokenize(''), [])
        self.assertEqual(tokenize('123'), ['123'])
        self.assertEqual(tokenize('sqrt(123)'), ['sqrt', '(', '123', ')'])
        # binding expression
        self.assertEqual(tokenize('bind num to 1234'), ['(', 'bind', 'num', 'to', '1234', ')'])

    def test_tokenize_list(self):
        self.assertEqual(tokenize('list(1 2 3 4 5 6 7 8 9)'),
                         ['list', '(', '1', '2', '3', '4', '5', '6', '7', '8', '9', ')'])
        self.assertEqual(tokenize('bind array to list(1 2 3 4 5 6 7 8 9)'),
                         ['(', 'bind', 'array', 'to', 'list', '(', '1', '2', '3', '4',
                          '5', '6', '7', '8', '9', ')', ')'])

    def test_tokenize_lambda(self):
        # lambda expressions
        self.assertEqual(tokenize('lambda({x} -> x)'),
                         ['lambda', '(', '{', 'x', '}', '->', 'x', ')'])
        self.assertEqual(tokenize('(lambda({x} -> x))(5)'),
                         ['(','lambda', '(', '{', 'x', '}', '->', 'x', ')',
                          ')', '(', '5', ')'])
        self.assertEqual(tokenize('[(lambda({x} -> lambda({y} -> +(x y))))(1)](4)'),
                         ['[', '(', 'lambda', '(', '{', 'x', '}', '->', 'lambda',
                          '(', '{', 'y', '}', '->', '+', '(', 'x', 'y', ')', ')', ')',
                          ')', '(', '1', ')', ']', '(', '4', ')'])
        
    def test_tokenize_functions(self):
        # function expressions
        self.assertEqual(tokenize('bind add to lambda({x y} -> +(x y))'),
                         ['(', 'bind', 'add', 'to', 'lambda', '(', '{', 'x', 'y',
                          '}', '->', '+', '(', 'x', 'y', ')', ')', ')'])
        self.assertEqual(tokenize("""bind ! to lambda({x} -> 
                                        if(<=(x 1) 
                                            1 
                                            *(x !(-(x 1)))));"""),
                         ['(', 'bind', '!', 'to', 'lambda', '(', '{', 'x', '}', '->',
                          'if', '(', '<=', '(', 'x', '1', ')', '1', '*', '(', 'x', '!',
                          '(', '-', '(', 'x', '1', ')', ')', ')', ')', ')', ')'])

    #TODO: Implement the String testing Suite
    #TODO: Implement the Struct testing Suite

    # atomize unittest
    def test_atomize(self):
        # value testing
        self.assertEqual(atomize('0'), 0)
        self.assertEqual(atomize('123'), 123)
        self.assertEqual(atomize('-123'), -123)
        self.assertEqual(atomize('1.23'), 1.23)
        self.assertEqual(atomize('-1.23'), -1.23)
        self.assertEqual(atomize('abc'), 'abc')
        
        
        
    # abstract_syntax_tree unittest
    def test_abstract_syntax_tree_error(self):
        # check for EOF
        self.assertRaises(SyntaxError, abstract_syntax_tree, [])

    def test_abstract_syntax_tree_literals(self):
        # check for literals
        self.assertEqual(abstract_syntax_tree(['123']), 123)
        self.assertEqual(abstract_syntax_tree(['1.23']), 1.23)
        self.assertEqual(abstract_syntax_tree(['x']), 'x')

    def test_abstract_syntax_tree_expressions(self):
        # check for expressions
        self.assertEqual(abstract_syntax_tree(['sqrt', '(', '123', ')']),
                         ['sqrt', 123])
        self.assertEqual(abstract_syntax_tree(['add', '(', '123', '456', ')']),
                         ['add', 123, 456])
        # nested expressions
        self.assertEqual(abstract_syntax_tree(['add', '(', 'add', '(', '12', '34',
                                               ')', '56', ')']),
                         ['add', ['add', 12, 34], 56])

    def test_abstract_syntax_tree_lambda(self):
        # check for lambda expression
        self.assertEqual(abstract_syntax_tree(['lambda', '(', '{', 'x', '}', '->', 'x', ')']),
                         ['lambda', ['x'], '->', 'x'])
        self.assertEqual(abstract_syntax_tree(['lambda', '(', '{', 'x', 'y', '}', '->',
                                               'f', '(', 'x', 'y', ')', ')']),
                         ['lambda', ['x', 'y'], '->', ['f', 'x', 'y']])
        self.assertEqual(abstract_syntax_tree(['(', 'lambda', '(', '{', 'x', '}', '->',
                                               'x', ')', ')', '(', '5', ')']),
                         [['lambda', ['x'], '->', 'x'], 5])
        self.assertEqual(abstract_syntax_tree(['(', 'lambda', '(', '{', 'x', 'y', '}', '->',
                                               '+', '(', 'x', 'y', ')', ')', ')',
                                               '(', '2', '3', ')']),
                         [['lambda', ['x', 'y'], '->', ['+', 'x', 'y']], 2, 3])
        
    def test_abstract_syntax_tree_binding(self):
        # check for binding
        self.assertEqual(abstract_syntax_tree(['(', 'bind', 'pi', 'to',
                                               '3.1415926', ')']),
                         ['bind', 'pi', 'to', 3.1415926])
        self.assertEqual(abstract_syntax_tree(['(', 'bind', 'add', 'to', 'lambda', '(',
                                               '{', 'x', 'y', '}', '->', '+', '(', 'x',
                                               'y', ')', ')', ')']),
                         ['bind', 'add', 'to', ['lambda', ['x', 'y'], '->',
                                                ['+', 'x', 'y']]])


        
        

        






















    
        
