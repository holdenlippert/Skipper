#!/usr/bin/env python
import sys
import unittest
import re
from collections import namedtuple

# <prog>  -> <line><line>*
# <line>  -> <line'> <end>
# <line'> -> <bind> | <expr>
# <end>   -> newline | EOF
# <bind>  -> symbol <expr>
# <expr>  -> symbol | <expr> ( <expr> )

Bind = namedtuple('Bind', ['identifier', 'expression'])
App = namedtuple('App', ['function', 'argument'])
Symbol = namedtuple('Symbol', ['name', 'params'])
Program = namedtuple('Program', ['lines'])

Token = namedtuple('Token', ['type', 'value'])


class ParserError(Exception):
    pass

class InterpreterError(Exception):
    pass

class Lexer:
    def __init__(self, program):
        self.program = str(program)
        self.tokenize()

    def tokenize(self):
        self.tokens = []
        patterns = [('id', re.compile(r'\b\w+\b')),
                    ('leftparen', re.compile(r'\(')),
                    ('rightparen', re.compile(r'\)')),
                    ('newline', re.compile(r'\n')),
                    ('space', re.compile(r' +')),
		    ('semicolon', re.compile(r';'))]
        index = 0
        while index < len(self.program):
            for name, pattern in patterns:
                match = pattern.match(self.program, index)
                if match:
                    break
            else:
                raise ParserError("Lexer error!")
            if name != 'space' and name != 'semicolon':
                self.tokens.append(Token(name, match.group()))
            index = match.start() + 1

    def pop(self):
        return self.tokens.pop(0) if self.tokens else Token("eof", "")

    def peek(self):
        return self.tokens[0] if self.tokens else Token("eof", "")

    def push(self, token):
        self.tokens.insert(0, token)


class Parser:
    def __init__(self, program):
        self.lexer = Lexer(program)

    def parse(self):
        lines = []
        while self.lexer.peek().type != "eof":
            lines.append(self.parseline())
        return Program(lines)

    def parseline(self):
        line = self.parselinecontents()
        self.parselineend()
        return line

    def parselinecontents(self):
        id = self.lexer.pop()
        if id.type != "id":
            raise ParserError("Parser error, expected id!")
        if self.lexer.peek().type == "id":
            ast = Bind(id, self.parseexpr())
        else:
            self.lexer.push(id)
            ast = self.parseexpr()
        return ast

    def parselineend(self):
        if self.lexer.peek().type in ["newline", "eof"]:
            self.lexer.pop()
        else:
            raise ParserError("Parser error, expected EOF or newline!")

    def parseexpr(self):
        expr = self.lexer.pop()
        if expr.type != "id":
            raise ParserError("Parser error, expected id, got " + expr.type + "!")
        expr = Symbol(expr.value, [])
        while self.lexer.peek().type == "leftparen":
            self.lexer.pop()
            expr = App(expr, self.parseexpr())
            if self.lexer.peek().type != "rightparen":
                raise ParserError("Parser error, expected rightparen!")
            self.lexer.pop()
        return expr


class Interpreter:
    def __init__(self, program):
        self.lines = Parser(program).parse().lines
        self.symbols = {}
        self.output = ""

    def interpret(self):
        for line in self.lines:
            self.interpretline(line)
        return self.output

    def interpretline(self, line):
        if isinstance(line, Bind):
            self.interpretbind(line)
        elif isinstance(line, App):
            self.interpretapp(line)
        else:
            raise Exception("Not Bind or App: " + str(line))

    def interpretbind(self, bind):
        raise Exception("Have not implemented bind.")

    def interpretapp(self, app):
        stack = [app]
        while stack:
            print stack
            app = stack.pop()
            if isinstance(app.function, Symbol):
                print "found symbol", app.function
                if app.function.name == 'K':
                    if app.function.params:
                        if isinstance(app.function.params[0], App):
                            stack.append(app.function.params[0])
                        else:
                            break
                    else:
                        app.function.params.append(app.argument)
            elif isinstance(app.function, App):
                print "found another app", app.function
                stack.append(app)
                stack.append(app.function)
            else:
                raise Exception("found: " + str(app.function))


def test():
    lines = ['I i', 'I i(i)', 'I S(K)(K)', 'S(K)(K) I', 'S(K)(K)', 'I']
    lines = ['S(p)(I)(I)']
    lines = ['K(p)(I)']
    for i, line in enumerate(lines):
        print "Test", i + 1
        print Interpreter(line).interpret()
        print "\n\n\n"

def main(args):
    with open(args[1]) as f:
        for line in f.readlines():
            print Interpreter(line).interpret()

if __name__ == "__main__":
    sys.exit(main(sys.argv))
