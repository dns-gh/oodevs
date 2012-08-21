"""
Parse .proto file and look for consistency issues and unused types.
"""
import sys, os, re, optparse

class Ui:
    def __init__(self):
        self.errors = 0

    def write(self, s):
        sys.stdout.write(s)

    def writeerr(self, s):
        sys.stderr.write(s)

    def error(self, path, lineno, s, line=None):
        line = line and ('   ' + line.rstrip() + '\n') or ''
        self.writeerr('%s:%d: %s\n%s' % (path, lineno, s, line))
        self.errors += 1

def listproto(rootdir):
    """Return a list of paths of .proto found under rootdir."""
    protos = []
    for root, dirs, files in os.walk(rootdir):
        for f in files:
            if f.endswith('.proto'):
                p = os.path.join(rootdir, f)
                protos.append(p)
    return protos

def readproto(path):
    """Parse .proto and yield (n, line) tuples where n is the zero-based line
    number and line is the line including its EOL character. Comments are
    stripped and lines could jump over comment lines.
    """
    incomment = False
    startslash, endslash = False, False
    interesting = r'/\*'
    for i, line in enumerate(file(path)):
        line = line.rstrip()
        kept = []
        s = 0
        while True:
            if incomment:
                e = line.find('*/', s)
                if e < 0:
                    kept.append('\n')
                    break
                s = e + 2
                incomment = False
            else:
                e = line.find('/', s)
                if e >= 0:
                    if e + 1 < len(line):
                        if line[e + 1] == '/':
                            kept.append(line[s:e] + '\n')
                            break
                        if line[e + 1] == '*':
                            kept.append(line[s:e])
                            incomment = True
                            s = e + 2
                    else:
                        kept.append(line[s:e + 2])
                        s = e + 2
                else:
                    kept.append(line[s:] + '\n')
                    break
        yield i, ''.join(kept)

class ParseError(Exception):
    def __init__(self, msg, line, path=None):
        super(ParseError, self).__init__(self, msg)
        self.line = line
        self.path = path 

class Field:
    """A protobuf message field."""
    def __init__(self, quantity, type, name):
        self.quantity = quantity
        self.type = type
        self.name = name

class Message:
    """A protobuf message."""
    def __init__(self, fname, lineno, name, fields, subenums=None,
            submessages=None):
        self.fname = fname
        self.lineno = lineno
        self.name = name
        self.fields = fields
        self.messages = dict((m.name, m) for m in (submessages or []))
        self.enums = dict((e.name, e) for e in (subenums or []))

class Enum:
    """A protobuf enumeration."""
    def __init__(self, name, lines):
        self.name = name
        self.lines = lines

class Module:
    """A protobuf module (or file)."""
    def __init__(self, name, imports, enums, messages):
        self.name = name
        self.imports = imports
        self.messages = messages
        self.enums = enums

def parseenum(lines, i, name):
    """Parse an enumeration definition.

    lines is a list of lines to parse.
    i is the index of the first line in lines following the
    'enum FOO {' line.
    name is the enumeration name.

    Return (i, enum) where i is the index of the first unparsed line and enum
    is an Enum instance.
    """
    enumlines = []
    while True:
        if i >= len(lines):
            raise ParseError('unterminated enum %s' % name, i)
        lineno, line = lines[i]
        i += 1
        if reblank.search(line):
            continue
        if line.lstrip().startswith('}'):
            break
        enumlines.append(line)
    return i, Enum(name, lines)


reblank = re.compile(r'^\s*(//.*)?$')
remessage = re.compile(r'^(\s*)message\s+(\S+)\s*\{(\s*\})?')

def parsemessage(fname, lines, i):
    """Parse a message definition, recursively.

    lines is a list of lines to parse.
    i is the index of the first line to parse, the one with 'message FOO {'.

    Return (i, msg) where i is the index of the next line to parse and msg
    is a Message instance.
    """
    refields = re.compile(
        r'^\s*(optional|required|repeated)'
        r'\s+(\S+)\s+(\S+)\s*=\s*\d+\s*(\[[^]]+\])?\s*;')
    reenum = re.compile(r'^\s+enum\s+(\S+)\s*{')

    srcline, line = lines[i]
    m = remessage.search(line)
    assert m
    name = m.group(2)
    i += 1
    if m.group(3) is not None:
        return i, Message(fname, srcline, name, [], [], [])

    fields = []
    messages = []
    enums = []
    while True:
        if i >= len(lines):
            raise ParseError('unterminated message %s' % name, i)
        lineno, line = lines[i]
        i += 1
        if reblank.search(line):
            continue
        if line.lstrip().startswith('}'):
            break
        m = reenum.search(line)
        if m:
            i, enum = parseenum(lines, i, m.group(1))
            enums.append(enum)
            continue
        m = remessage.search(line)
        if m:
            assert m.group(1), line
            i, msg = parsemessage(fname, lines, i - 1)
            messages.append(msg)
            continue
        m = refields.search(line)
        if m:
            quantity, type, fieldname = m.group(1, 2, 3)
            f = Field(quantity, type, fieldname)
            fields.append(f)
            continue
        raise ParseError('unexpected line %r' % line, i)

    return i, Message(fname, srcline, name, fields, enums, messages)

def parsemodule(ui, path):
    """Parse a single protobuf file and return a Module instance."""
    fname = os.path.basename(path)
    name = os.path.splitext(fname)[0]

    re_package = re.compile(r'^package\s+(\S+)')
    re_enum = re.compile(r'^enum\s+(\S+)\s*{')
    re_import = re.compile(r'^import\s+\"([^"]+)')

    messages = []
    enums = []
    imports = []
    i = 0
    lines = list(readproto(path))
    errors = []
    while i < len(lines):
        lineno, line = lines[i]
        if reblank.search(line) or re_package.search(line):
            i += 1
            continue
        m = remessage.search(line)
        if m:
            assert not m.group(1)
            i, msg = parsemessage(fname, lines, i)
            messages.append(msg)
            continue
        m = re_enum.search(line)
        if m:
            i, enum = parseenum(lines, i + 1, m.group(1))
            enums.append(enum)
            continue
        m = re_import.search(line)
        if m:
            imports.append(m.group(1))
            i += 1
            continue
        if not errors or not errors[-1]:
            errors.append([])
        errors[-1].append(line.rstrip())
        i += 1
    if errors:
        errors = ['\n'.join(e) for e in errors]
        for err in errors:
            ui.writeerr('%s: unexpected lines found:\n%s\n' % (name, err))
        raise ParseError('unexpected lines found, aborting', 0)
    return Module(name, imports, enums, messages)

def parsemodules(ui, rootdir):
    """List and parse all .proto under rootdir, return a list of Module
    instances.
    """
    modules = []
    for p in sorted(listproto(rootdir)):
        pname = os.path.splitext(os.path.basename(p))[0]
        try:
            module = parsemodule(ui, p)
        except ParseError, e:
            raise ParseError(e.args[1], e.line, pname)
        modules.append(module)
    return modules

class Writer:
    def __init__(self):
        self.indentation = 0
        self.lines = []

    def indent(self):
        self.indentation += 2

    def dedent(self):
        self.indentation -= 2

    def w(self, s):
        self.lines.append(self.indentation*' ' + s + '\n')

def printenum(w, enum):
    w.w('enum %s' % enum.name)

def printmessage(w, message):
    w.w('message %s' % message.name)
    if message.enums:
        w.indent()
        for n, e in sorted(message.enums.items()):
            printenum(w, e)
        w.dedent()
    if message.messages:
        w.indent()
        for n, m in sorted(message.messages.items()):
            printmessage(w, m)
        w.dedent()

def printmodule(w, module):
    w.w('module %s' % module.name)
    if module.messages:
        w.indent()
        for m in module.messages:
            printmessage(w, m)
        w.dedent()

def listtypes(modules):
    types = {}
    def addtype(parts, t):
        name = '.'.join(parts)
        if name in types:
            raise Exception('duplicated type: %s' % name)
        types[name] = t

    def listmsgtypes(parts, m):
        for e in m.enums.itervalues():
            addtype(parts + [e.name], e)
        for mm in m.messages.itervalues():
            addtype(parts + [mm.name], mm)
            listmsgtypes(parts + [mm.name], mm)

    for m in modules:
        for e in m.enums:
            addtype([e.name], e)
        for mm in m.messages:
            addtype([mm.name], mm)
            listmsgtypes([mm.name], mm)

    return types

def itermessages(modules):
    """Enumerate all messages in all modules.

    Yield (parts, msg) where parts is a list of parent component names,
    including the msg name, and msg is a Message instance.
    """
    for m in modules:
        pendings = [([mm.name], mm) for mm in m.messages]
        while pendings:
            parts, mm = pendings.pop()
            pendings.extend((parts + [cm.name], cm)
                    for cm in mm.messages.values())
            yield parts, mm

# Set of builtin protobuf types
_knowntypes = set([
    'int32', 'string', 'float', 'bool', 'uint32', 'double',
    ])

def checkfields(ui, modules, types, roottypes):
    """Enumerate all modules messages and:
    - Check their fields and children fields types are known
    - Find unused types
    - Find unused types with used subtypes.

    Return the two latter as sets of type names
    """
    unusedtypes = set(types)
    for parts, m in itermessages(modules):
        for f in m.fields:
            if f.type in _knowntypes:
                continue
            possibletypes = set()
            for i in xrange(len(parts) + 1):
                possibletypes.add('.'.join(parts[:i] + [f.type]))
            usedtypes = set(t for t in possibletypes if t in types)
            unusedtypes -= usedtypes
            if not usedtypes:
                ui.writeerr('unknown types: %s\n' % max(possibletypes))

    # Root types are not unused, they are just not used in .proto
    unusedtypes = set(u for u in unusedtypes if u not in roottypes)

    # Types are not really unused if one of their subtype is used
    withsubtype = set()
    for u in unusedtypes:
        if any(t for t in types if t.startswith(u + '.')):
            withsubtype.add(u)

    unusedtypes -= withsubtype
    return unusedtypes, withsubtype

# Set all sword root messages
_roottypes = set([
    'AdminToLauncher',
    'ClientToReplay',
    'DispatcherToClient',
    'ReplayToClient',
    'LauncherToAdmin',
    'ClientToMessenger',
    'ClientToAar',
    'MessengerToClient',
    'AuthenticationToClient',
    'ClientToSim',
    'ClientToAuthentication',
    'SimToClient',
    'AarToClient',
    'KnowledgeGroupAck',
    'DispatcherToSim',
    ])

def findtodo(path):
    matching = []
    retodo = re.compile(r'todo', re.I)
    for i, line in enumerate(file(path)):
        if retodo.search(line):
            matching.append((i + 1, line))
    return matching

parser = optparse.OptionParser(
        usage='mapproto.py path (check|todo|style|braces)')

def cmdcheck(ui, rootdir):
    try:
        modules = parsemodules(ui, rootdir)
        w = Writer()
        for m in modules:
            #printmodule(w, m)
            #w.w('')
            pass
        sys.stdout.write(''.join(w.lines))
        types = listtypes(modules)
        unused, subtype = checkfields(ui, modules, types, _roottypes)
        if unused:
            for u in sorted(unused):
                ui.writeerr('unused type: %s\n' % u)
        if subtype:
            for u in sorted(subtype):
                ui.writeerr('unused with used subtype: %s\n' % u)
    except ParseError, e:
        sys.stderr.write('%s(%d): %s\n' % (e.path, e.line, e.args[1]))
        return 1

def cmdtodo(ui, rootdir):
    for path in listproto(rootdir):
        fname = os.path.basename(path)
        for i, l in findtodo(path):
            ui.write('%s:%d: %s\n' % (fname, i, l.rstrip()))

def cmdbraces(ui, rootdir):
    """Check braces are written like:

      message FooBar {

    and not:

      message FooBar
      {

    """
    rebrace = re.compile(r'^\s*(message|enum)\s+\S+[^\{]*$')
    for path in listproto(rootdir):
        fname = os.path.basename(path)
        for i, line in readproto(path):
            if rebrace.search(line):
                ui.error(fname, i + 1, 'invalid brace style', line)
    return ui.errors


def cmdstyle(ui, rootdir):
    refield = re.compile(r'^[a-z0-9_]+$')
    modules = parsemodules(ui, rootdir)
    for parts, m in itermessages(modules):
        for f in m.fields:
            if not refield.search(f.name):
                ui.error(m.fname, m.lineno + 1,
                        '%s should be lower case with underscores'
                         % '.'.join(parts + [f.name]))

if __name__ == '__main__':
    if len(sys.argv) < 2:
        parser.writeerr('mapproto: at least 2 arguments expected')

    args = sys.argv[2:]
    protodir = sys.argv[1]
    cmds = {
        'check': cmdcheck,
        'todo': cmdtodo,
        'style': cmdstyle,
        'braces': cmdbraces,
        }
    ui = Ui()
    res = 0
    for cmd in args:
        if cmd not in cmds:
            parser.writeerr('unknown command %s' % cmd)
        res += (cmds[cmd](ui, protodir) or 0)
    sys.exit(res and 1 or 0)

