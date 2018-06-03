import math
import numbers
import random

class TokenKind:
    NUMBER = 0
    NAME = 1
    PLUS = 2
    MINUS = 3
    ASTERISK = 4
    SLASH = 5
    LEFT_PAREN = 6
    RIGHT_PAREN = 7
    COMMA = 8
    END = 9

token_table = {
    '+': TokenKind.PLUS,
    '-': TokenKind.MINUS,
    '*': TokenKind.ASTERISK,
    '/': TokenKind.SLASH,
    '(': TokenKind.LEFT_PAREN,
    ')': TokenKind.RIGHT_PAREN,
    ',': TokenKind.COMMA
}

def kind_to_str(kind):
    return list(token_table.keys())[list(token_table.values()).index(kind)]

class Token:
    def __init__(self, kind, value=None):
        self._kind = kind
        self._value = value

    @property
    def kind(self):
        return self._kind

    @property
    def value(self):
        return self._value

    def __repr__(self):
        return str(self._kind)

class Expression:
    def __repr__(self):
        return '<Expression {0}>'.format(str(self))

class BinaryExpression(Expression):
    def __init__(self, left, operator, right):
        self._left = left
        self._operator = operator
        self._right = right

    @property
    def left(self):
        return self._left

    @property
    def right(self):
        return self._right

    @property
    def operator(self):
        return self._operator

    def __str__(self):
        return '({0} {1} {2})'.format(
                self._left, kind_to_str(self._operator), self._right)

class UnaryExpression(Expression):
    def __init__(self, operator, operand):
        self._operator = operator
        self._operand = operand

    @property
    def operator(self):
        return self._operator

    @property
    def operand(self):
        return self._operand

    def __str__(self):
        return '{0}{1}'.format(kind_to_str(self._operator), self._operand)

class LiteralExpression(Expression):
    def __init__(self, value):
        self._value = value

    @property
    def value(self):
        return self._value

    def __repr__(self):
        return str(self._value)

class NumberExpression(LiteralExpression):
    pass

class NameExpression(LiteralExpression):
    pass

class CallExpression(Expression):
    def __init__(self, left, args):
        self._left = left
        self._args = args

    @property
    def left(self):
        return self._left

    @property
    def args(self):
        return self._args

    def __str__(self):
        return '{0}({1})'.format(self._left, ', '.join(map(str, self._args)))

class PrefixParselet:
    def __init__(self, precedence):
        raise NotImplementedError

    def parse(self, parser, token):
        raise NotImplementedError

    @property
    def precedence(self):
        raise NotImplementedError

class NumberParselet(PrefixParselet):
    def __init__(self):
        pass

    def parse(self, parser, token):
        return NumberExpression(token.value)

class NameParselet(PrefixParselet):
    def __init__(self):
        pass

    def parse(self, parser, token):
        return NameExpression(token.value)

class PrefixOperatorParselet(PrefixParselet):
    def __init__(self, precedence):
        self._precedence = precedence

    def parse(self, parser, token):
        operand = parser.parse(self.precedence)
        return UnaryExpression(token.kind, operand)

    @property
    def precedence(self):
        return self._precedence

class GroupParselet(PrefixParselet):
    def __init__(self):
        pass

    def parse(self, parser, token):
        expression = parser.parse()
        parser.consume(TokenKind.RIGHT_PAREN)
        return expression

class InfixParselet:
    def __init__(self, precedence, is_right):
        raise NotImplementedError

    @property
    def precedence(self):
        raise NotImplementedError

    def parse(self, parser, left, token):
        raise NotImplementedError

class InfixOperatorParselet(InfixParselet):
    def __init__(self, precedence, is_right):
        self._precedence = precedence
        self._is_right = is_right

    @property
    def precedence(self):
        return self._precedence

    def parse(self, parser, left, token):
        right = parser.parse(self._precedence - (1 if self._is_right else 0))
        return BinaryExpression(left, token.kind, right)

class CallParselet(InfixParselet):
    def __init__(self, precedence):
        self._precedence = precedence

    def parse(self, parser, left, token):
        args = []

        if not parser.match(TokenKind.RIGHT_PAREN):
            while True:
                args.append(parser.parse())
                if not parser.match(TokenKind.COMMA):
                    break

            parser.consume(TokenKind.RIGHT_PAREN)

        return CallExpression(left=left, args=args)

    @property
    def precedence(self):
        return self._precedence

class PostfixOperatorParselet(InfixParselet):
    def __init__(self, precedence):
        self._precedence = precedence

    def parse(self, parser, left, token):
        return UnaryExpression(token.kind, left)

    @property
    def precedence(self):
        return self._precedence

class Parser:
    def __init__(self, tokens):
        self._tokens = iter(tokens)
        self._queued_tokens = []
        self._prefix_parselets = {}
        self._infix_parselets = {}

    def register(self, kind, parselet):
        """ Registers a new parselet. """
        if isinstance(parselet, PrefixParselet):
            self._prefix_parselets[kind] = parselet
        elif isinstance(parselet, InfixParselet):
            self._infix_parselets[kind] = parselet
        else:
            raise ValueError('Expected parselet')

    def look_ahead(self, distance):
        """ Fetches and returns next token on the queue.
        Does not remove it from queue, so could be called multiple times.
        """
        while distance >= len(self._queued_tokens):
            try:
                self._queued_tokens.append(next(self._tokens))
            except StopIteration:
                break

        try:
            return self._queued_tokens[distance]
        except IndexError:
            return Token(TokenKind.END)

    def consume(self, expected=None):
        """ Removes token from queue.
        When `expected` provided checks whether tokens match.
        If not, raises an exception.
        """
        token = self.look_ahead(0)
        if expected and token.kind != expected:
            raise RuntimeError('Unexpected token {0}'.format(token))

        return self._queued_tokens.pop(0)

    def match(self, expected):
        """ Return True if current token matches `expected` and removes it from queue.
        Otherwise returns False.
        """
        token = self.look_ahead(0)
        if token.kind != expected:
            return False

        self.consume()
        return True

    @property
    def precedence(self):
        """ Returns precedence for expression being parsed. """
        token = self.look_ahead(0)
        try:
            return self._infix_parselets[token.kind].precedence
        except KeyError:
            return 0

    def parse(self, precedence=0):
        """ Parses expression. """
        token = self.consume()
        if token.kind not in self._prefix_parselets:
            raise RuntimeError('I ain\'t have any parselets for this token')

        left = self._prefix_parselets[token.kind].parse(self, token)
        while precedence < self.precedence:
            token = self.consume()
            left = self._infix_parselets[token.kind].parse(self, left, token)

        return left

def tokenize(string):
    for token in string.split(' '):
        if '.' in token or token.isdigit():
            yield Token(TokenKind.NUMBER, float(token))
        elif token in token_table:
            yield Token(token_table[token])
        else:
            yield Token(TokenKind.NAME, token)

def eval(expr, **context):
    ev = lambda e: eval(e, **context)

    if isinstance(expr, NumberExpression):
        return expr.value
    elif isinstance(expr, NameExpression):
        return context[expr.value]
    elif isinstance(expr, UnaryExpression):
        op = expr.operator
        if op == TokenKind.PLUS:
            return +ev(expr.operand)
        elif op == TokenKind.MINUS:
            return -ev(expr.operand)
    elif isinstance(expr, BinaryExpression):
        left = ev(expr.left)
        right = ev(expr.right)
        op = expr.operator
        if op == TokenKind.PLUS:
            return left + right
        elif op == TokenKind.MINUS:
            return left - right
        elif op == TokenKind.ASTERISK:
            return left * right
        elif op == TokenKind.SLASH:
            return left / right
    elif isinstance(expr, CallExpression):
        if not isinstance(expr.left, NameExpression):
            raise RuntimeError('Invalid call invoke')

        name = expr.left.value
        args = [ev(attr) for attr in expr.args]

        if name in context:
            return context[name](*args)
        elif hasattr(math, name):
            fn = getattr(math, name)
            return fn(*args)
        else:
            raise Exception('Function {0} not declared'.format(name))

    else:
        raise RuntimeError('Could not evaluate expression {}'.format(expr))


def parse(string):
    parser = Parser(tokenize(string))

    parser.register(TokenKind.NUMBER, NumberParselet())
    parser.register(TokenKind.NAME, NameParselet())
    parser.register(TokenKind.LEFT_PAREN, GroupParselet())

    parser.register(TokenKind.PLUS, PrefixOperatorParselet(3))
    parser.register(TokenKind.MINUS, PrefixOperatorParselet(3))

    parser.register(TokenKind.PLUS, InfixOperatorParselet(1, False))
    parser.register(TokenKind.MINUS, InfixOperatorParselet(1, False))
    parser.register(TokenKind.ASTERISK, InfixOperatorParselet(2, False))
    parser.register(TokenKind.SLASH, InfixOperatorParselet(2, False))

    parser.register(TokenKind.LEFT_PAREN, CallParselet(5))

    return parser.parse()

if __name__ == '__main__':
    text = 'sqrt ( rand ( 1 , 200 ) )'
    expr = parse(text)
    result = eval(expr, pi=math.pi, sqrt=lambda x: x**0.5, rand=random.randint)

    print('Expression:'.ljust(20), text)
    print('Parsed:'.ljust(20), expr)
    print('Result:'.ljust(20), result)
