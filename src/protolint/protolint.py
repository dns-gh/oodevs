"""
Parse .proto file and look for consistency issues and unused types.
"""
import sys, os, re, optparse
import protoparser

class Ui:
    def __init__(self):
        self.errors = 0

    def write(self, s):
        sys.stderr.write(s)

    def writeerr(self, s):
        sys.stderr.write(s)
        sys.stderr.flush()

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

def _findtype(kind, scope, t):
    if t in _knowntypes:
        return t
    if t in scope:
        return scope[t]
    raise Exception('unknown %s type %s' % (kind, t))

def _makescope(scope, msg):
    if isinstance(msg, protoparser.Message):
        scope = dict(scope)
        scope.update(msg.messages)
        scope.update(msg.enums)
    return scope

def _isenum(t):
    return isinstance(t, protoparser.Enum)

def _isknowntype(t):
    return isinstance(t, basestring) and t in _knowntypes

def _ismessage(t):
    return not(_isknowntype(t) or _isenum(t))

def _comparemessages(ui, idx, oldname, oldtype, oldscope, newname, newtype,
        newscope, seen, exclude):
    def writeerr(msg):
        ui.writeerr('%s:\n  %s@%d -> %s\n  %s@%d -> %s\n'
            % (msg,
               '.'.join(oldname.split('.')[:-1]), idx, oldtype,
               '.'.join(newname.split('.')[:-1]), idx, newtype))
        return 1

    ret = 0
    if (oldtype, newtype) in seen or exclude(oldname) or exclude(newname):
        return 0
    oldt = _findtype('old', oldscope, oldtype)
    newt = _findtype('new', newscope, newtype)
    if not _ismessage(oldt) or not _ismessage(newt):
        if oldtype != newtype:
            return writeerr('types differ')
        if _isenum(oldt):
            if not _isenum(newt):
                return writeerr('error: old field is an enum but not the new one')
            # Old enum values must exist in the new one
            oldvalues, newvalues = [set(v.value for v in t.values)
                for t in (oldt, newt)]
            delta = oldvalues - newvalues
            if delta:
                writeerr('error: old enumeration has values not in new one')
                for v in sorted(delta):
                    name = [e for e in oldt.values if e.value == v][0].name
                    ui.writeerr('    %s = %s\n' % (name, v))
                    ret += 1
        elif _isenum(newt):
            return writeerr('error: new field is an enum but not the old one')
        return ret

    seen.append((oldtype, newtype))
    oldscope = _makescope(oldscope, oldt)
    newscope = _makescope(newscope, newt)
    oldindexes = dict((f.index, f) for f in oldt.fields.itervalues())
    newindexes = dict((f.index, f) for f in newt.fields.itervalues())
    for f in newt.fields.itervalues():
        if f.index not in oldindexes:
            if f.quantifier == 'required':
                ui.writeerr('error: required field added:\n  %s\n'
                    % (newname + '.' + f.name))
                ret += 1
            continue
        oldf = oldindexes[f.index]
        oldtype = _gettype('old', oldscope, oldf.type)
        newtype = _gettype('new', newscope, f.type)
        newn = newname + '.' + newtype
        oldn = oldname + '.' + oldtype
        ret += _comparemessages(ui, f.index, oldn, oldtype, oldscope,
                newn, newtype, newscope, seen, exclude)
    for f in oldt.fields.itervalues():
        if f.index in newindexes:
            continue
        if f.quantifier == 'required':
            ui.writeerr('error: required field removed:\n  %s\n'
                % (oldname + '.' + f.name))
            ret += 1
    seen.pop()
    return ret

def makematcher(excluded):
    if not excluded:
        return lambda s: False
    r = ['.*'.join(re.escape(s) for s in e.split('*')) for e in excluded]
    r = '^(' + '|'.join(r) + ')$'
    r = re.compile(r, re.I)
    return r.search

def cmdcompare(ui, args):
    parser = optparse.OptionParser(
        usage='compare [OPTIONS] OLDDIR NEWDIR')
    parser.add_option("-X", "--exclude", action='append', default=[],
        help="exclude field from analysis")
    opts, args = parser.parse_args(args)
    matcher = makematcher(opts.exclude)

    olddir, newdir = args
    ui.write('comparing:\n  %s\n  %s\n' % (olddir, newdir))
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
            ui.writeerr('error: cannot find %s in old messages\n' % m)
            err = 1
            continue
        ret += _comparemessages(ui, 0, m, m, oldtypes, m, m, newtypes,
                [], matcher)
    return ret

def cmdlist(ui, args):
    parser = optparse.OptionParser(usage='list DIR')
    opts, args = parser.parse_args(args)

    protodir, = args
    modules = parsemodules(ui, protodir)
    names = []
    for module in modules:
        for nm, m in module.messages.iteritems():
            if nm not in _roottypes or 'Content' not in m.messages:
                continue
            for mm in m.messages['Content'].fields.values():
                names.append('.'.join([module.name, nm, mm.type]))
    for n in sorted(names):
        ui.write('%s\n' % n)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        parser.writeerr('mapproto: at least 2 arguments expected')

    cmd = sys.argv[1]
    args = sys.argv[2:]
    cmds = {
        'check': cmdcheck,
        'compare': cmdcompare,
        'list': cmdlist,
        'naming': cmdnaming,
        'style': cmdstyle,
        'todo': cmdtodo,
        }
    ui = Ui()
    if cmd not in cmds:
        ui.writeerr('unknown command %s' % cmd)
    res = cmds[cmd](ui, args) or 0
    sys.exit(res and 1 or 0)

