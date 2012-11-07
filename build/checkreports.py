import os, sys, re
import xml.etree.ElementTree as ETree

class Ui(object):
    def error(self, s):
        sys.stderr.write(s)

class ParseError(Exception):
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
    names = [re.escape(n) for n in names]
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
    return result, decreports

def parseluaids(ui, path):
    """Parse report identifiers defined in Lua."""
    relua = re.compile(r'^\s*(eRC_\S+)\s*=\s*(\d+)')
    rids = {}
    rnames = {}
    result = 0
    for line in file(path):
        m = relua.search(line)
        if not m:
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

class ReportParam(object):
    def __init__(self, type, values=None):
        self.type = type
        self.values = sorted(values or [])

class Report(object):
    def __init__(self, id, msg, params):
        self.id = id
        self.msg = msg
        self.params = params

def parsephysical(ui, path):
    result = 0
    reports = {}
    tree = ETree.parse(path)
    xpath = './report'
    for e in tree.findall(xpath):
        id = int(e.attrib['id'])
        msg = e.attrib['message'].encode('utf-8')
        params = []
        for p in e.findall('parameter'):
            values = []
            type = p.attrib['type']
            for v in p.findall('value'):
                vid = int(v.attrib['id'])
                vname = v.attrib['name'].encode('utf-8')
                values.append((vid, vname))
            params.append(ReportParam(type, values))
        r = Report(id, msg, params)
        if r.id in reports:
            ui.error('error: duplicate found for physical report %s\n' % r.id)
            result = 1
        reports[r.id] = r
    return result, reports

def checkphyids(ui, phyname, phids, othername, otherids):
    result = 0;
    for rid in otherids:
        if rid not in phyids:
            ui.error('error: cannot find %s %d in physical db %s\n'
                    % (othername, rid, phyname))
            result = 1
    return result

if __name__ == '__main__':
    ui = Ui()
    swordpath = sys.argv[1]
    result = 0
    res, cppids = parsecpp(ui, swordpath)
    if res:
        result = 1

    luapath = os.path.join(swordpath,
            'data/data/models/ada/decisional/dia5/Types_CR.lua')
    res, luaids = parseluaids(ui, luapath)
    if res:
        result = 1

    intpath = os.path.join(swordpath,
            'data/app-data/resources/integration')
    res, intnames = parseintegration(ui, intpath)
    if res:
        result = 1

    res, intids = checkluaintegration(ui, luaids, intnames)
    if res:
        result = 1

    # Cross-check cpp/integration ids with several physical dbs
    phypaths = [
        'data/data/models/ada/physical/scipio/Reports.xml',
        'data/data/models/ada/physical/worldwide/Reports.xml',
        ]
    for p in phypaths:
        phypath = os.path.join(swordpath, p)
        phyname = os.path.split(os.path.dirname(phypath))[1]
        res, phyids = parsephysical(ui, phypath)
        if res:
            result = 1
        if checkphyids(ui, phyname, phyids, 'sim report',
                set(cppids.values())):
            result = 1
        if checkphyids(ui, phyname, phyids, 'integration report',
                set(intids.values())):
            result = 1

    sys.exit(result)

