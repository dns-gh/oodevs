"""
Parse .proto file and look for consistency issues and unused types.
"""
import sys, os, re, optparse
import protoparser

class Ui:
    def __init__(self):
        self.errors = 0

    def write(self, s):
        sys.stdout.write(s)

    def writeerr(self, s):
        sys.stderr.write(s)

    def error(self, path, lineno, s, line=None):
        line = line and ('   ' + line.rstrip() + '\n') or ''
        if lineno is None:
            lineno = ''
        else:
            lineno = '%d:' % lineno
        self.writeerr('%s:%s %s\n%s' % (path, lineno, s, line))
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
        data = file(p, 'r').read()
        module = protoparser.parse(pname, data, debug=0)
        modules.append(module)
    return modules

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
        for e in m.enums.itervalues():
            addtype([e.name], e)
        for mm in m.messages.itervalues():
            addtype([mm.name], mm)
            listmsgtypes([mm.name], mm)

    return types

def itermessages(modules):
    """Enumerate all messages in all modules.

    Yield (parts, msg) where parts is a list of parent component names,
    including the msg name, and msg is a Message instance.
    """
    for m in modules:
        pendings = [([mm.name], mm) for mm in m.messages.itervalues()]
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
        for f in m.fields.itervalues():
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
        usage='mapproto.py path (check|todo|naming|style)')

def cmdcheck(ui, args):
    rootdir, = args
    try:
        modules = parsemodules(ui, rootdir)
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

def cmdtodo(ui, args):
    rootdir, = args
    for path in listproto(rootdir):
        fname = os.path.basename(path)
        for i, l in findtodo(path):
            ui.write('%s:%d: %s\n' % (fname, i, l.rstrip()))

def cmdstyle(ui, args):
    rootdir, = args
    rebrace = re.compile(r'^\s*(message|enum)\s+\S+[^\{]*$')
    # Fix $$$ or FIXME, etc.
    refixme = re.compile(r'(\${3,}|FIXME)', re.I)
    # Match invalid doxygen comments
    rebadcmt = re.compile(r'//!?\s')
    respace = re.compile('^(\s+)\S')
    for path in listproto(rootdir):
        fname = os.path.basename(path)
        # Check opening braces are on the definition line
        for i, line in readproto(path):
            if rebrace.search(line):
                ui.error(fname, i + 1, 'invalid brace style', line)
            m = respace.search(line)
            if m:
                if (len(m.group(1)) % 4) != 0:
                    ui.error(fname, i + 1, 'indent with 4 spaces blocks', line)

        # Check lines are at most 80 characters long
        for i, line in enumerate(file(path)):
            if len(line.rstrip('\n\r')) > 80:
                ui.error(fname, i + 1, 'line is too long')
            if refixme.search(line):
                ui.error(fname, i + 1, 'convert $$$/FIXME in @todo')
            if '\t' in line:
                ui.error(fname, i + 1, 'expand tabs to 4 spaces')
            if '//' in line and not '://' in line:
                if not rebadcmt.search(line):
                    ui.error(fname, i + 1, 'comment must be // or //!', line)
                lline = line.lstrip()
                if not lline.startswith('//'):
                    # Single line comments must not be preceded by code
                    ui.error(fname, i + 1,
                        'comments must be on a separate line')

    return ui.errors


def cmdnaming(ui, args):
    rootdir, = args
    refield = re.compile(r'^[a-z0-9_]+$')
    modules = parsemodules(ui, rootdir)
    for parts, m in itermessages(modules):
        for f in m.fields.itervalues():
            if not refield.search(f.name):
                ui.error(f.name, None,
                        '%s.%s should be lower case with underscores'
                         % (m.name, '.'.join(parts + [f.name])))

def _gettype(kind, scope, t):
    if t in scope or t in _knowntypes:
        return t 
    raise Exception('unknown %s type %s' % (kind, t))

def makescope(scope, msg):
    scope = dict(scope)
    scope.update(msg.messages)
    scope.update(msg.enums)
    return scope

def _comparemessages(ui, idx, oldname, oldtype, oldscope, newname, newtype, newscope, seen):
    ret = 0
    if (oldtype, newtype) in seen:
        return 0
    if oldtype in _knowntypes or newtype in _knowntypes:
        if oldtype != newtype:
            ui.writeerr('types differ:\n  %s/%s@%d\n  %s/%s@%d\n'
                    % (oldname, oldtype, idx, newname, newtype, idx))
            return 1
        return 0
    oldt = oldscope[oldtype]
    newt = newscope[newtype]
    if isinstance(oldt, protoparser.Enum) or isinstance(newtype, protoparser.Enum):
        return 0
    seen.append((oldtype, newtype))
    oldscope = makescope(oldscope, oldt)
    newscope = makescope(newscope, newt)
    oldindexes = dict((f.index, f) for f in oldt.fields.itervalues())
    for f in newt.fields.itervalues():
        if f.index not in oldindexes:
            continue
        oldf = oldindexes[f.index]
        oldtype = _gettype('old', oldscope, oldf.type)
        newtype = _gettype('new', newscope, f.type)
        newn = newname + '.' + newtype
        oldn = oldname + '.' + oldtype
        ret += _comparemessages(ui, f.index, oldn, oldtype, oldscope,
                newn, newtype, newscope, seen)
    seen.pop()
    return ret

def cmdcompare(ui, args):
    olddir, newdir = args
    oldmodules = parsemodules(ui, olddir)
    oldtypes = listtypes(oldmodules)
    newmodules = parsemodules(ui, newdir)
    newtypes = listtypes(newmodules)
    seen = set()
    ret = 0
    for m in newtypes:
        if m not in _roottypes:
            continue
        if m not in oldtypes: 
            ui.writeerr('cannot find %s in old messages\n' % m)
            err = 1
            continue
        ret += _comparemessages(ui, 0, m, m, oldtypes, m, m, newtypes, [])
    return ret

if __name__ == '__main__':
    if len(sys.argv) < 2:
        parser.writeerr('mapproto: at least 2 arguments expected')

    cmd = sys.argv[1]
    args = sys.argv[2:]
    cmds = {
        'check': cmdcheck,
        'compare': cmdcompare,
        'todo': cmdtodo,
        'naming': cmdnaming,
        'style': cmdstyle,
        }
    ui = Ui()
    if cmd not in cmds:
        ui.writeerr('unknown command %s' % cmd)
    res = cmds[cmd](ui, args) or 0
    sys.exit(res and 1 or 0)

