import unittest

from interpreter.LexicalAnalyzer import tokenize
from interpreter.LexicalAnalyzer import atomize


class TestLexicalAnalyzer(unittest.TestCase):

    # tokenize unittest
    def test_tokenize_literal(self):
        # empty string returns empty tokens
        self.assertEqual(tokenize(''), [])
        self.assertEqual(tokenize('123'), ['123'])
        self.assertEqual(tokenize('sqrt(123)'), ['sqrt', '(', '123', ')'])
        # binding expression
        self.assertEqual(tokenize('bind num to 1234'), ['bind', 'num', 'to', '1234'])

    def test_tokenize_list(self):
        self.assertEqual(tokenize('list(1 2 3 4 5 6 7 8 9)'),
                         ['list', '(', '1', '2', '3', '4', '5', '6', '7', '8', '9', ')'])
        self.assertEqual(tokenize('bind array to list(1 2 3 4 5 6 7 8 9)'),
                         ['bind', 'array', 'to', 'list', '(', '1', '2', '3', '4',
                          '5', '6', '7', '8', '9', ')'])

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
                         ['bind', 'add', 'to', 'lambda', '(', '{', 'x', 'y', '}', '->',
                          '+', '(', 'x', 'y', ')', ')'])
        self.assertEqual(tokenize("""bind ! to lambda({x} -> 
                                        if(<=(x 1) 
                                            1 
                                            *(x !(-(x 1)))));"""),
                         ['bind', '!', 'to', 'lambda', '(', '{', 'x', '}', '->',
                          'if', '(', '<=', '(', 'x', '1', ')', '1', '*', '(',
                          'x', '!', '(', '-', '(', 'x', '1', ')', ')', ')', ')', ')'])

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
        
        
        
        

























    
        
