import os, sys, unittest
sys.path.append(os.path.join(os.path.dirname(__file__), 'ply-3.4'))
from collections import namedtuple
import protolexer

tokens = protolexer.TOKENS
ParseError = protolexer.ParseError

from ply import yacc

Field = namedtuple('Field', 'quantifier, type, name, index')
Enum = namedtuple('Enum', 'name, values')
EnumValue = namedtuple('EnumValue', 'name, value')
Import = namedtuple('Import', 'path')
Message = namedtuple('Message', 'name, fields, enums, messages')
AnonModule = namedtuple('Module', 'package, imports, enums, messages')
Package = namedtuple('Package', 'name')
Module = namedtuple('Module', 'name, package, imports, enums, messages')

def maptypes(elems, type):
    return dict((e.name, e) for e in elems if isinstance(e, type))

def p_proto_module(p):
    """proto_module : proto_definitions"""
    imports = [e for e in p[1] if isinstance(e, Import)]
    enums = maptypes(p[1], Enum)
    messages = maptypes(p[1], Message)
    packages = [e for e in p[1] if isinstance(e, Package)]
    if len(packages) > 1:
        raise ParseError('more the one package statement found in module')
    package = ([pp.name for pp in packages] + [None])[0]
    p[0] = AnonModule(package, imports, enums, messages)

def p_proto_definitions(p):
    """proto_definitions : proto_definition
                         | proto_definitions proto_definition
    """
    if len(p) <= 2:
        p[0] = [p[1]]
    else:
        p[0] = p[1] + [p[2]]

def p_proto_definition(p):
    """proto_definition : import_definition
                        | message_definition
                        | enum_definition
                        | package_definition
                        | semicolon
    """
    if p[1][0] != 'semicolon':
        p[0] = p[1]

def p_import_definition(p):
    """import_definition : import quoted semicolon"""
    p[0] = Import(p[2])

def p_package_definition(p):
    """package_definition : package token semicolon"""
    p[0] = Package(p[2])

def p_message_definition(p):
    """message_definition : message token message_body"""
    fields = maptypes(p[3], Field)
    enums = maptypes(p[3], Enum)
    messages = maptypes(p[3], Message)
    p[0] = Message(p[2], fields, enums, messages)

def p_message_body(p):
    """message_body : lbrace message_statements rbrace
                    | lbrace rbrace
    """
    p[0] = []
    if len(p) > 3:
        p[0] = p[2]

def p_message_statements(p):
    """message_statements : message_statements message_statement
                          | message_statement
    """
    if len(p) > 2:
        p[0] = p[1] + [p[2]]
    else:
        p[0] = [p[1]]

def p_message_statement(p):
    """message_statement : field_definition
                         | enum_definition
                         | message_definition
    """
    p[0] = p[1]

def p_field_definition(p):
    """field_definition : field_body semicolon
                        | field_body field_default semicolon
    """
    p[0] = Field(*p[1])

def p_field_body(p):
    """field_body : field_quantifier field_type field_name assignment\
                    number
    """
    p[0] = (p[1], p[2], p[3], int(p[5]))

def p_field_name(p):
    """field_name : token
                  | message
    """
    p[0] = p[1]

def p_field_default(p):
    """field_default : lbracket token assignment field_default_value rbracket
    """
    p[0] = None

def p_field_default_value(p):
    """field_default_value : token
                           | number
                           | quoted
    """
    p[0] = p[1]

def p_field_quantifier(p):
    """field_quantifier : token"""
    if p[1] not in ('required', 'optional', 'repeated'):
        raise ParseError('invalid field quantifier')
    p[0] = p[1]

def p_field_type(p):
    """field_type : token"""
    # User defined types cannot be validated until all of them are
    # resolved
    p[0] = p[1]

def p_enum_definition(p):
    """enum_definition : enum token enum_body"""
    p[0] = Enum(p[2], p[3])

def p_enum_body(p):
    """enum_body : lbrace enum_statements rbrace
                 | lbrace rbrace
    """
    if len(p) > 3:
        p[0] = p[2]
    else:
        p[0] = []

def p_enum_statements(p):
    """enum_statements : enum_statements enum_statement
                       | enum_statement
    """
    if len(p) > 2:
        p[0] = p[1] + [p[2]]
    else:
        p[0] = [p[1]]

def p_enum_statement(p):
    """enum_statement : token assignment number semicolon"""
    p[0] = EnumValue(p[1], int(p[3]))

def p_error(p):
    assert False, p

_parser = yacc.yacc()

def parse(modulename, data, eof=True, debug=False):
    lexer = protolexer.Lexer(modulename)
    needmore, parsedtokens = lexer.adddata(data, eof)
    # Strip comments for now
    parsedtokens = [t for t in parsedtokens
                    if t.type not in ('comment',)]

    class PlyLexer:
        def __init__(self, tokens):
            self.tokens = list(reversed(tokens))

        def token(self):
            if not self.tokens:
                return None
            return self.tokens.pop()

    m = _parser.parse(lexer=PlyLexer(parsedtokens), debug=debug)
    m = Module(modulename, m.package, m.imports, m.enums, m.messages)
    return m

class TestParser(unittest.TestCase):
    def test_parser(self):
        s = """\
  // some comment
import "foo.proto";

package spam;

message foobar {
};

enum enum1 {
   value1 = 1;
   value2 = 2;
   }

message baz {
  required uint32 field1 = 1;
  optional MyType field2 = 2;

  enum subenum{
     sub1 = 1;
     sub2 = 2;
  }

  message foo.bar{
     repeated boolean field1 = 1;
     };
  optional Message message = 3;
}
"""
        expected = Module('test', 'spam', [
            Import('foo.proto'),
            ],
            {'enum1': Enum('enum1', [
                EnumValue('value1', 1),
                EnumValue('value2', 2),
                ]),
            },
            {'foobar': Message('foobar', {}, {}, {}),
                'baz': Message('baz', {
                    'field1': Field('required', 'uint32', 'field1', 1),
                    'field2': Field('optional', 'MyType', 'field2', 2),
                    'message': Field('optional', 'Message', 'message', 3),
                },
                {'subenum': Enum('subenum', [
                    EnumValue('sub1', 1),
                    EnumValue('sub2', 2),
                    ]),
                },
                {'foo.bar': Message('foo.bar', {
                    'field1': Field('repeated', 'boolean', 'field1', 1),
                    },
                    {},
                    {}),
                }),
            })
        result = parse('test', s)
        self.maxDiff = None
        self.assertEqual(expected, result)

if __name__ == '__main__':
    unittest.main()
