"""List DEC functions from C++ code, and save them to a file or compare
them with an existing list.

  $ python listdec.py list src/libraries/simulation_kernel decfunctions.txt

or

  $ python listdec.py check decfunctions.txt src/libraries/simulation_kernel

or

  $ python listdec.py match decfunctions src/libraries/simulation_kernel

to check all functions in decfunctions are defined in the simulation
"""
import sys, os, re

redec = re.compile(
        r'(?:RegisterFunction|RegisterMethod)\s*\(\s*"([^"]+)', re.S)

def listdec(path):
    data = file(path, 'rb').read()
    names = []
    for m in redec.finditer(data):
        names.append(m.group(1))
    return names

def itercpp(path):
    exts = ('.h', '.inl', '.cpp', '.hpp')
    for root, dirs, files in os.walk(path):
        for f in files:
            ext = os.path.splitext(f)[-1]
            if ext not in exts:
                continue
            yield os.path.join(root, f)

def listalldecs(path):
    names = set()
    for f in itercpp(path):
        for n in listdec(f):
            names.add(n)
    return names

def cmdlistdec(args):
    srcpath, outpath = args
    names = listalldecs(srcpath)
    fp = sys.stdout
    close = False
    if outpath != '-':
        fp = file(outpath, 'wb')
        close = True
    msg = """\
# This file was generated automatically by sword project.
# In models: do not overwrite, append new functions to keep the old ones
# around and check them.

"""
    fp.write(msg)
    for n in sorted(names):
        fp.write('%s\n' % n)
    if close:
        fp.close()
    return 0

def parsedeclist(path):
    names = []
    for line in file(path):
        line = line.strip()
        if line and line[0] != '#':
            names.append(line)
    return names

def cmdcheckdec(args):
    declist, srcpath = args
    refnames = parsedeclist(declist)
    names = listalldecs(srcpath)
    if not names:
        sys.stderr.write('error: no DEC function found in source code\n')
        return 1
    # Cannot really blame models for using this one
    names.remove('LoadResourcesFile')
    # DEC_* can be found automatically, it is not necessary to add them
    # to the list in models.
    names = [n for n in names if not n.startswith('DEC_')]
    # models list must be a superset of the actual DEC set
    result = 0
    missing = sorted(set(names) - set(refnames))
    if missing:
        w = sys.stderr.write
        w("""\n
error: upstream mode project does not know about several DEC functions
       defined by the simulation. Please append them to model
       decfunctions.txt reference file
""")
        for n in missing:
            sys.stderr.write('    %s\n' % n)
    return missing and 1 or 0

def cmdmatchdec(args):
    declist, srcpath = args
    names = listalldecs(srcpath)
    refnames = set(parsedeclist(declist))

    w = sys.stderr.write
    if not names:
        w('error: failed to parse DEC registrations\n')
        return 1
    if not refnames:
        w('error: failed to parse DEC list\n')
        return 1

    # Check all used DECs exist in the simulation
    result = 0
    for n in sorted(refnames - names):
        w('error: %s is used by models but not defined in simulation\n' % n)
        result = 1
    return result

_commands = {
    'list': cmdlistdec,
    'check': cmdcheckdec,
    'match': cmdmatchdec,
    }

if __name__ == '__main__':
    args = sys.argv[1:]
    if not args:
        sys.stderr.write('error: command expected\n')
        sys.exit(1)
    cmd, args = args[0], args[1:]
    ret = _commands[cmd](args)
    sys.exit(ret)
