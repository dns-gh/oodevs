import sys, os, re, unittest

# Lexer states
ST_NORMAL = 'ST_NORMAL'
ST_QUOTED = 'ST_QUOTED'
ST_LINECMT = 'ST_LINECMT'
ST_CMT = 'ST_CMT'

class NeedMore(Exception):
    pass

class ParseError(Exception):
    def __init__(self, lineno, msg):
        super(Exception, self).__init__(msg)
        self.lineno = lineno

# Token types
TK_TOKEN = 'token'
TK_QUOTED = 'quoted'
TK_COMMENT = 'comment'
TK_SEMICOLON = 'semicolon'
TK_ASSIGNMENT = 'assignment'
TK_LBRACE = 'lbrace'
TK_RBRACE = 'rbrace'
TK_LBRACKET = 'lbracket'
TK_RBRACKET = 'rbracket'
TK_NUMBER = 'number'

_specialchars = {
    ';': TK_SEMICOLON,
    '=': TK_ASSIGNMENT,
    '[': TK_LBRACKET,
    ']': TK_RBRACKET,
    '{': TK_LBRACE,
    '}': TK_RBRACE,
    }

_keywords = set(['package', 'import', 'message', 'enum'])

TOKENS = [
    TK_TOKEN, TK_QUOTED, TK_NUMBER,
    ] + [c for c in _specialchars.values()] + list(_keywords)

rechar = re.compile(r'^[a-zA-Z0-9\-_\.]$')
renum = re.compile(r'^[0-9]+$')

class Token:
    def __init__(self, type=TK_TOKEN, value='', lineno=0, module=''):
        self.value = value
        self.type = type
        self.lineno = lineno
        self.module = module

    def __str__(self):
        return '%s(%r)%s@%d' % (self.type, self.value, self.module, self.lineno)

class Lexer:
    def __init__(self, module=''):
        self._state = ST_NORMAL
        self._input = ''
        self._pos = 0
        self._line = 0
        self._module = module
        self._parsefn = {
            ST_NORMAL: self._parsenormal,
            ST_LINECMT: self._parselinecmt,
            ST_CMT: self._parsecmt,
            ST_QUOTED: self._parsequoted,
            }
        self._tokens = []
        self._newtoken()

    def adddata(self, data, eof):
        self._input += data
        self._tokens = []
        needmore = False
        try:
            self._parse(eof)
        except NeedMore:
            needmore = True
        return needmore, self._tokens

    def _error(self, msg):
        raise ParseError(self._line, msg)

    def _newtoken(self, type=TK_TOKEN, value=''):
        self._token = Token(type, value, lineno=self._line, module=self._module)

    def _pushtoken(self):
        t = self._token
        if t.type == TK_TOKEN and not t.value:
            return
        if t.type == TK_TOKEN:
            # Look for reserved keyword
            if t.value in _keywords:
                t.type = t.value
            elif renum.search(t.value):
                t.type = TK_NUMBER
        self._tokens.append(self._token)
        self._newtoken()

    def _parse(self, eof):
        while self._state:
            if self._pos >= len(self._input):
                if not eof:
                    raise NeedMore()
            self._parsefn[self._state](eof)

    def _parselinecmt(self, eof):
        if self._pos >= len(self._input):
            self._pushtoken()
            self._state = None
            return
        c = self._input[self._pos]
        self._token.value += c
        self._pos += 1
        if c == '\n':
            self._line += 1
            self._pushtoken()
            self._state = ST_NORMAL

    def _parsecmt(self, eof):
        if self._pos >= len(self._input):
           self._error('unfinished comment')
        c = self._input[self._pos]
        if c == '*':
            if self._pos + 1 >= len(self._input):
                if not eof:
                    raise NeedMore()
                self._error('unfinished comment')
            c2 = self._input[self._pos + 1]
            if c2 == '/':
                self._token.value += c + c2
                self._pushtoken()
                self._pos += 2
                self._state = ST_NORMAL
                return
        if c == '\n':
            self._line += 1
        self._token.value += c
        self._pos += 1

    def _parsequoted(self, eof):
        if self._pos >= len(self._input):
            if not eof:
                raise NeedMore()
            self._error('unterminated quoted string')
        c = self._input[self._pos]
        # Not sure what the exact rules are, forbid multiline quoted
        # strings
        if c == '\n':
            self._error('multiline quoted string')
        elif c == '\\':
            if self._pos + 1 >= len(self._input):
                if not eof:
                    raise NeedMore()
                self._error('unterminated quoted string')
            c2 = self._input[self._pos + 1]
            self._token.value += c2
            self._pos += 2
        elif c == '"':
            self._pushtoken()
            self._state = ST_NORMAL
            self._pos += 1
        else:
            self._token.value += c
            self._pos += 1

    def _parsenormal(self, eof):
        if self._pos >= len(self._input):
            self._pushtoken()
            self._state = None
            return
        c = self._input[self._pos]
        if c == '\n':
            self._line += 1
            self._pos += 1
        elif c == '"':
            self._pushtoken()
            self._pos += 1
            self._newtoken(TK_QUOTED, '')
            self._state = ST_QUOTED
        elif c == '/':
            if self._pos + 1 >= len(self._input):
                if not eof:
                    raise NeedMore()
                self._error('stray trailing slash')
            c2 = self._input[self._pos + 1]
            if c2 in '/*':
                self._pos += 2
                self._pushtoken()
                self._newtoken(TK_COMMENT, c + c2)
                if c2 == '/':
                    self._state = ST_LINECMT
                else:
                    self._state = ST_CMT
            else:
                self._error('unexpected character: %s' % (c + c2))
        elif c in ' \t\r':
            self._pushtoken()
            self._pos += 1
        elif c in _specialchars:
            self._pushtoken()
            self._newtoken(_specialchars[c], c)
            self._pushtoken()
            self._pos += 1
        elif rechar.search(c):
            self._token.value += c
            self._pos += 1
        else:
            self._error('unexpected character: %s' % c)

class TestLexer(unittest.TestCase):
    def _parse(self, expected, s):
        needmore, tokens = Lexer().adddata(s, True)
        tokens = [(t.type, t.value) for t in tokens]
        self.assertEqual(False, needmore)
        self.assertEqual(expected, tokens)

        lexer = Lexer()
        tokens = []
        for i, c in enumerate(s):
            needmore, t = lexer.adddata(c, (i + 1) == len(s))
            tokens += t
        tokens = [(t.type, t.value) for t in tokens]
        self.assertEqual(expected, tokens)

    def test_comments(self):
        s = """\
// leading single line
    // single line with blank
/** multi
   line
   */
"""
        expected = [
            ('comment', '// leading single line\n'),
            ('comment', '// single line with blank\n'),
            ('comment', '/** multi\n   line\n   */'),
            ]
        self._parse(expected, s)

    def test_normal(self):
        s = """\
      
  message m {
    foo bar2_ = blah;
    optional int bar3 = 1 [ default = "" ];
  }
"""
        expected = [
            ('message', 'message'),
            ('token', 'm'),
            ('lbrace', '{'),
            ('token', 'foo'),
            ('token', 'bar2_'),
            ('assignment', '='),
            ('token', 'blah'),
            ('semicolon', ';'),
            ('token', 'optional'),
            ('token', 'int'),
            ('token', 'bar3'),
            ('assignment', '='),
            ('number', '1'),
            ('lbracket', '['),
            ('token', 'default'),
            ('assignment', '='),
            ('quoted', ''),
            ('rbracket', ']'),
            ('semicolon', ';'),
            ('rbrace', '}'),
            ]
        self._parse(expected, s)

    def test_quoted(self):
        s = """\
   "quoted \\"with space"
"""
        expected = [('quoted', 'quoted "with space')]
        self._parse(expected, s)

if __name__ == '__main__':
    unittest.main()

