import sys, os

def _sanitize(s):
    if isinstance(s, unicode):
        return s.encode('ascii', 'ignore')
    return s

def writeinfo(s):
    sys.stdout.write(_sanitize(s))
    sys.stdout.flush()

def writeerr(s):
    sys.stdout.flush()
    sys.stderr.write(_sanitize(s))
    sys.stderr.flush()

def cyg2win(cygpath):
    if cygpath.startswith('/cygdrive/'):
        path = cygpath[len('/cygdrive/'):]
        path = path[0] + ':' + path[1:]
        return path
    if cygpath.startswith('/home/'):
        path = cygpath[len('/home/'):]
        # Lazy but usually works
        path = 'c:/cygwin/home/' + path
        return path
    return cygpath
