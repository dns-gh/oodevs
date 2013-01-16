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
    reports = []
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
                if line == '}':
                    state = 2
                    break
                ui.error('error: cannot parse report enum value:\n    %s\n'
                        % (line,))
                result = 1
                continue
            if m.group(1) != m.group(2):
                ui.error( 'In ' + headerpath + ': report name "' + m.group(1) + '" different from its key "' + m.group(2) + '"' );
            reports.append(m.group(1))
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
        'src/libraries/simulation_kernel/Entities/Orders/MIL_DecisionalReport.h')

    redecstart = re.compile(r'^\s*namespace\s+report\s*$')
    redecval = re.compile(r'^\s*const\s+MIL_DecisionalReport\s*(eRC_[^\s,]+)\s*\(\s*"(eRC[^\s,]+)\"\s*\)\s*;')
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

def parselua(ui, path):
    """Parse report identifiers defined in Lua."""
    relua = re.compile(r'^\s*(eRC_\S+|eNbr)\s*=\s*("\S+"|\d+)')
    rnames = []
    result = 0
    for line in file(path):
        line = line.strip()
        m = relua.search(line)
        if not m:
            if line and not line.startswith('--'):
                ui.error('error: unknown lua line:\n    %s\n' % line)
                result = 1
            continue
        rname = m.group(1)
        if rname == 'eNbr':
            continue
        if '"' + m.group(1) + '"' != m.group(2):
            ui.error( 'In ' + path + ': report name "' + m.group(1) + '" different from its key "' + m.group(2) + '"' );
        if rname in rnames:
            ui.error('error: ' + rname +' already defined in ' + path )
            result = 1
        rnames.append(rname)
    return result, rnames

def parseintegrationfile(ui, path):
    """Extract indentifiers looking like report identifiers from Lua files."""
    rnames = set()
    data = file(path, 'rb').read()
    rerc = re.compile(r'eRC_[a-zA-Z0-9_]+')
    for m in rerc.finditer(data):
        rnames.add(m.group(0))
    return rnames

def parseintegration(ui, intpath):
    """Extract all possible report identifiers in integration subtree"""
    rnames = set()
    for root, dirs, files in os.walk(intpath):
        for f in files:
            p = os.path.join(root, f)
            rnames.update(parseintegrationfile(ui, p))
    result = 0
    if not rnames:
        ui.error('error: no report identifier found in integration layer\n')
        result = 1
    return result, rnames

def checkluaintegration(ui, luanames, intnames):
    """Cross check reports indentifiers integration layer and decisional"""
    result = 0
    wrongintnames = []
    for r in intnames:
        if r not in luanames:
            ui.error('error: integration uses an unknown report: %s\n' % r)
            result = 1
            continue
        wrongintnames.append(r)
    return result, wrongintnames

def checkluacpp(ui, luanames, cppnames):
    result = 0
    for r in sorted(set(cppnames).difference(set(luanames))):
        ui.error('error: lua and c++ names differ: %s\n' % r)
        result = 1
    return result

def parsereportlist(path):
    reports = {}
    simnames = set()
    for line in file(path):
        line = line.strip()
        if not line or line[0] == '#':
            continue
        name, code, sim = [p.strip() for p in line.split()]
        reports[name] = int(code)
        if sim == 'sim':
            simnames.add(name)
    return reports, simnames

def cmdcheck(ui, args):
    swordpath, reportpath = args
    reports, simnames = parsereportlist(reportpath)
    result = 0
    res, cppnames = parsecpp(ui, swordpath)
    if res:
        result = 1

    luapath = os.path.join(swordpath,
            'data/data/models/ada/decisional/dia5/Types_CR.lua')
    res, luanames = parselua(ui, luapath)
    if res:
        result = 1

    if checkluacpp(ui, luanames, cppnames):
        result = 1
    intpath = os.path.join(swordpath,
            'data/app-data/resources/integration')
    res, intnames = parseintegration(ui, intpath)
    if res:
        result = 1
    res, wrongintnames = checkluaintegration(ui, luanames, intnames)
    if res:
        result = 1

    # Check the list of simulation reports owned by models is a superset
    # of the real one
    simreports = set(wrongintnames)
    simreports.update(cppnames)
    for n in simreports:
        if n not in reports:
            ui.error('error: %s is not defined in models reports.txt\n'
                    % n)
            result = 1
            continue
    for n in (set(cppnames) - simnames):
        ui.error('error: %s is not marked as a simulation identifier in '
                'models reports.txt\n' % n)
    return result

def cmddump(ui, args):
    swordpath, outpath = args
    res, cppids = parsecpp(ui, swordpath)
    if res:
        raise Abort('failed to parse c++ enumerations')
    luapath = os.path.join(swordpath,
            'data/data/models/ada/decisional/dia5/Types_CR.lua')
    res, luaids = parselua(ui, luapath)
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
        fp.write('%s\t%d\t%s\n' % (n, simids[n],
            n in cppids and 'sim' or 'notsim'))
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
