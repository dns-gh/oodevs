import os, sys, re

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
            rid = len(reports)
            reports[rid] = m.group(1)
    if state != 2:
        ui.error('error: reached end of %s before seeing all expected enums'
                % headerpath)
        result = 1
    if result:
        raise ParseError('failed to parse c++ header')
    return i, reports

def parsecppheader(ui, headerpath):
    """Parse the engine and decisional enumerations in MIL_Report.h."""
    reengstart = re.compile(r'^\s*enum\s+E_EngineReport\s*$')
    reengval = re.compile(r'^\s*(eReport_\S+|eNbrReport),?\s*(//|$)')
    redecstart = re.compile(r'^\s*enum\s+E_DecisionalReport\s*$')
    redecval = re.compile(r'^\s*((?:eRC_|eRc_)\S+|eLast),?\s*(//|$)')
    lines = list(file(headerpath))
    i = 0
    i, engreports = parseenum(ui, headerpath, i, lines, reengstart, reengval)
    i, decreports = parseenum(ui, headerpath, i, lines, redecstart, redecval)
    result = 0
    if not engreports:
        ui.error('error: no E_EngineReport definition found\n')
        result = 1
    if not decreports:
        ui.error('error: no E_DecisionReport definition found\n')
        result = 1
    return result, engreports, decreports

def parsecppsource(ui, cpppath):
    """Parse the engine to decisional enumeration mappings in MIL_Report.cpp"""
    rengs = {}
    rdecs = {}
    result = 0
    remap = re.compile(r'\s*diaEvents_\[\s*([^\]]+?)\s*\]\s*=\s*([^\s;]+)')
    for line in file(cpppath):
        m = remap.search(line)
        if not m:
            continue
        reng, rdec = m.group(1, 2)
        if reng in rengs:
            ui.error('error: %s is mapped more than once\n' % reng)
            result = 1
        if rdec in rdecs:
            ui.error('error: %s is mapped more than once\n' % rdec)
            result = 1
        rengs[reng] = rdec
        rdecs[rdec] = reng
    if not rengs:
        ui.error('error: no engine to decisional mapping found\n')
        result = 1
    return result, rengs

def parsecpp(ui, swordpath):
    """Check the consistency of reports enumerations in simulation_kernel."""
    result = 0
    headerpath = os.path.join(swordpath,
        'src/libraries/simulation_kernel/Entities/Orders/MIL_Report.h')
    res, eng, dec = parsecppheader(ui, headerpath)
    if res:
        result = 1
    sourcepath = headerpath[:-1] + 'cpp'
    res, engmap = parsecppsource(ui, sourcepath)
    if res:
        result = 1
    return result

if __name__ == '__main__':
    ui = Ui()
    swordpath = sys.argv[1]
    result = parsecpp(ui, swordpath)
    sys.exit(result)

