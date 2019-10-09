""" Implements the lexer and parser for my language. """
import re
from collections import namedtuple

#Bind = namedtuple('Bind', ['identifier', 'expression'])
#App = namedtuple('App', ['function', 'argument'])
#Symbol = namedtuple('Symbol', ['name', 'params'])
#Program = namedtuple('Program', ['lines'])

#Token = namedtuple('Token', ['type', 'value'])

class Bind:
    def __init__(self, identifier, expression):
        self.identifier = identifier
        self.expression = expression

class App:
    def __init__(self, function, argument):
        self.function = function
        self.argument = argument

    def __str__(self):
        return "%s(%s)" % (self.function, self.argument)

class Symbol:
    def __init__(self, name, params):
        self.name = name
        self.params = params

    def __str__(self):
        if self.params:
            paramstring = '[' + ', '.join(map(str, self.params)) + ']'
        else:
            paramstring = ''
        return "%s%s" % (self.name, paramstring)

class Program:
    def __init__(self, lines):
        self.lines = lines

class Token:
    def __init__(self, type, value):
        self.type = type
        self.value = value


class ParserError(Exception):
    """ An error caused by a syntax error in the program. """
    pass


class Lexer:
    """ Tokenizes the given program. """
    def __init__(self, program):
        """ Initialize the lexer given an input program. """
        self._program = str(program)
        self._tokenize()

    def _tokenize(self):
        """ Convert the given program to a sequence of tokens. """
        self._tokens = []
        patterns = [('id', re.compile(r'\b\w+\b')),
                    ('leftparen', re.compile(r'\(')),
                    ('rightparen', re.compile(r'\)')),
                    ('newline', re.compile(r'\n+')),
                    ('space', re.compile(r'[ \t]+')),
                    ('semicolon', re.compile(r';'))]
        index = 0
        while index < len(self._program):
            for name, pattern in patterns:
                match = pattern.match(self._program, index)
                if match:
                    break
            else:
                print self._program
                print index
                print self._program[index:]
                raise ParserError('Lexer error!')
            if name != 'space' and name != 'semicolon':
                self._tokens.append(Token(name, match.group()))
            index = match.end()

    def pop(self):
        """ Remove and return the next token in the sequence. """
        return self._tokens.pop(0) if self._tokens else Token('eof', "")

    def peek(self):
        """ Return the next token in the sequence without removing it. """
        return self._tokens[0] if self._tokens else Token('eof', "")

    def push(self, token):
        """ Add a token back into the front of the sequence. """
        self._tokens.insert(0, token)


class Parser:
    def __init__(self, program):
        self.lexer = Lexer(program)

    def parse(self):
        lines = []
        while self.lexer.peek().type != 'eof':
            lines.append(self.parseline())
        return Program(lines)

    def parseline(self):
        line = self.parselinecontents()
        self.parselineend()
        return line

    def parselinecontents(self):
        id = self.lexer.pop()
        if id.type != 'id':
            raise ParserError('Parser error, expected id!')
        if self.lexer.peek().type == 'id':
            ast = Bind(id, self.parseexpr())
        else:
            self.lexer.push(id)
            ast = self.parseexpr()
        return ast

    def parselineend(self):
        if self.lexer.peek().type in ['newline', 'eof']:
            self.lexer.pop()
        else:
            raise ParserError('Parser error, expected EOF or newline!')

    def parseexpr(self):
        expr = self.lexer.pop()
        if expr.type != 'id':
            raise ParserError('Parser error, expected id, got ' + expr.type + '!')
        expr = Symbol(expr.value, [])
        while self.lexer.peek().type == 'leftparen':
            self.lexer.pop()
            expr = App(expr, self.parseexpr())
            if self.lexer.peek().type != 'rightparen':
                raise ParserError('Parser error, expected rightparen!')
            self.lexer.pop()
        return expr
