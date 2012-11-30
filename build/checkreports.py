import os, sys, re
import xml.etree.ElementTree as ETree

class Ui(object):
    def error(self, s):
        sys.stderr.write(s)

class ParseError(Exception):
    pass

class Abort(Exception):
    pass

def parseenum(ui, headerpath, lineno, lines, restart, reval):
    """Return (lineno, reports) where lineno is the last line number read by
    the enumeration parser, and reports is a mapping of enumeration symbols
    to integer values.

    Parse a C++ enumeration definition.
    """
    reports = {}
    state = 0
    result = 0
    for i in xrange(lineno, len(lines)):
        line = lines[i]
        if state == 0:
            if restart.search(line):
                state = 1
        elif state == 1:
            m = reval.search(line)
            if not m:
                line = line.strip()
                if not line or line == '{':
                    continue
                if line == '};':
                    state = 2
                    break
                ui.error('error: cannot parse report enum value:\n    %s\n'
                        % (line,))
                result = 1
                continue
            reports[m.group(1)] = int(m.group(2))
    if state != 2:
        ui.error('error: reached end of %s before seeing all expected enums'
                % headerpath)
        result = 1
    if result:
        raise ParseError('failed to parse c++ header')
    return i, reports

def _makeregexps(names):
    # Sort from longest to shortest to match longest first.
    names = [re.escape(n) for n in sorted(names, reverse=True)]
    regex = '(' + '|'.join(names) + ')'
    regex = re.compile(regex)
    return [regex]

def searchenums(ui, enums, cppdir):
    """Return the set of input enums not matched in c++ files in cppdir tree"""
    regexps = _makeregexps(enums)
    missing = set(enums)
    for root, dirs, files in os.walk(cppdir):
        if not missing:
            break
        for f in files:
            if not missing:
                break
            if f.lower() == 'mil_report.h':
                continue
            ext = os.path.splitext(f)[-1].lower()
            if ext not in ('.h', '.cpp', '.inl'):
                continue
            p = os.path.join(root, f)
            data = file(p, 'rb').read()
            for r in regexps:
                for m in r.finditer(data):
                    missing.discard(m.group(1))
    return missing

def parsecpp(ui, swordpath):
    """Check the consistency of reports enumerations in simulation_kernel."""
    result = 0
    headerpath = os.path.join(swordpath,
        'src/libraries/simulation_kernel/Entities/Orders/MIL_Report.h')

    redecstart = re.compile(r'^\s*enum\s+E_DecisionalReport\s*$')
    redecval = re.compile(r'^\s*(eRC_[^\s,]+)\s*=\s*(\d+)\s*,?\s*(//|$)')
    lines = list(file(headerpath))
    result = 0
    decreports = parseenum(ui, headerpath, 0, lines, redecstart, redecval)[1]
    if not decreports:
        ui.error('error: no E_DecisionReport definition found\n')
        result = 1

    if False:
        # Disable this by default, this is not really an error but saves
        # time when you have to manually edit reports.
        cppdir = os.path.join(swordpath, 'src')
        missing = searchenums(ui, decreports, cppdir)
        if missing:
            result = 1
            for m in missing:
                ui.error('error: %s is unused in C++ code\n' % m)

    return result, decreports

def parseluaids(ui, path):
    """Parse report identifiers defined in Lua."""
    relua = re.compile(r'^\s*(eRC_\S+|eNbr)\s*=\s*(\d+)')
    rids = {}
    rnames = {}
    result = 0
    for line in file(path):
        line = line.strip()
        m = relua.search(line)
        if not m:
            if line and not line.startswith('--'):
                ui.error('error: unknown lua line:\n    %s\n' % line)
                result = 1
            continue
        rname, rid = m.group(1, 2)
        rid = int(rid)
        if rname in rnames:
            ui.error('error: %s/%d collides with %s/%d\n' % (rname, rid,
                rname, rnames[rname]))
            result = 1
        if rid in rids:
            ui.error('error: %s/%d collides with %s/%d\n' % (rname, rid,
                rids[rid], rid))
            result = 1
        if rname == 'eNbr':
            continue
        rnames[rname] = rid
        rids[rid] = rname
    return result, rnames

def parseintegrationfile(ui, path):
    """Extract indentifiers looking like report identifiers from Lua files."""
    rids = set()
    data = file(path, 'rb').read()
    rerc = re.compile(r'eRC_[a-zA-Z0-9_]+')
    for m in rerc.finditer(data):
        rids.add(m.group(0))
    return rids

def parseintegration(ui, intpath):
    """Extract all possible report identifiers in integration subtree"""
    rids = set()
    for root, dirs, files in os.walk(intpath):
        for f in files:
            p = os.path.join(root, f)
            rids.update(parseintegrationfile(ui, p))
    result = 0
    if not rids:
        ui.error('error: no report identifier found in integration layer\n')
        result = 1
    return result, rids

def checkluaintegration(ui, luaids, intnames):
    """Cross check reports indentifiers integration layer and decisional"""
    result = 0
    intids = {}
    for r in intids:
        if r not in luaids:
            ui.error('error: integration uses an unknown report: %s\n' % r)
            result = 1
            continue
        intids[r] = luaids[r]
    return result, intids

def checkluacpp(ui, luaids, cppids):
    result = 0
    luanames = dict((v, k) for k,v in luaids.iteritems())
    cppnames = dict((v, k) for k,v in cppids.iteritems())
    for r in sorted(set(cppnames) & set(luanames)):
        if luanames[r] != cppnames[r]:
            ui.error('error: lua and c++ names differ: %s != %s\n' %
                    (luanames[r], cppnames[r]))
            result = 1
    return result

def parsereportlist(path):
    reports = {}
    for line in file(path):
        line = line.strip()
        if not line or line[0] == '#':
            continue
        name, code = line.split('\t', 1)
        reports[name] = int(code)
    return reports

def cmdcheck(ui, args):
    swordpath, reportpath = args
    reports = parsereportlist(reportpath)
    result = 0
    res, cppids = parsecpp(ui, swordpath)
    if res:
        result = 1

    luapath = os.path.join(swordpath,
            'data/data/models/ada/decisional/dia5/Types_CR.lua')
    res, luaids = parseluaids(ui, luapath)
    if res:
        result = 1

    if checkluacpp(ui, luaids, cppids):
        result = 1

    intpath = os.path.join(swordpath,
            'data/app-data/resources/integration')
    res, intnames = parseintegration(ui, intpath)
    if res:
        result = 1

    res, intids = checkluaintegration(ui, luaids, intnames)
    if res:
        result = 1

    # Check the list of simulation reports owned by models is a superset
    # of the real one
    simreports = dict((n, luaids[n]) for n in intids)
    simreports.update(cppids)
    for n, c in simreports.iteritems():
        if n not in reports:
            ui.error('error: %d/%s is not defined in models reports.txt\n'
                    % (c, n))
            result = 1
            continue
        cc = reports[n]
        if cc != c:
            ui.error('error: %s has not the same value in the simulation/'
                    'integration layer and in models (check reports.txt) '
                    '%d != %d\n' % (n, c, cc))
            result = 1
    return result

def cmddump(ui, args):
    swordpath, outpath = args
    res, cppids = parsecpp(ui, swordpath)
    if res:
        raise Abort('failed to parse c++ enumerations')
    luapath = os.path.join(swordpath,
            'data/data/models/ada/decisional/dia5/Types_CR.lua')
    res, luaids = parseluaids(ui, luapath)
    if res:
        raise Abort('failed to parse lua identifiers')

    intpath = os.path.join(swordpath,
            'data/app-data/resources/integration')
    res, intnames = parseintegration(ui, intpath)
    if res:
        raise Abort('failed to parse integration layer')
    intids = dict((n, luaids[n]) for n in intnames)
    simids = dict(intids)
    simids.update(cppids)
    fp = file(outpath, 'wb')
    msg = """\
# This file was generated automatically by sword. Please update it when new
# reports are being emitted by the simulation.

"""
    fp.write(msg)
    for n in sorted(simids):
        fp.write('%s\t%d\n' % (n, simids[n]))
    fp.close()
    return 0

_commands = {
    'check': cmdcheck,
    'dump': cmddump,
    }

if __name__ == '__main__':
    ui = Ui()
    args = sys.argv[1:]
    if not args:
        sys.stderr.write('error: command expected\n')
        sys.exit(1)
    cmd, args = args[0], args[1:]
    try:
        ret = _commands[cmd](ui, args)
    except Abort, e:
        sys.stderr.write('error: %s\n' % e)
        sys.exit(1)
    sys.exit(ret)

