# Simple utility to make checksums of directories and compare them
import difflib
import fnmatch
import hashlib
import optparse
import os
import sys

helpstring="""\
    checksum computes checksums on <src_dir> and prints each file checksum.
    if --check <file> is specified, it compares <src_dir> and snapshots in
    <file> instead, and prints differences if any
"""
usage = "checksum.py <command> <src_dir> <temp_file>"

parser = optparse.OptionParser(
        usage = usage,
        description = helpstring )
parser.add_option("-X", "--exclude", action="append", default=[],
        help="ignore files matching a glob expression")

def writeinfo(s):
    sys.stdout.write(s.encode('ascii', 'ignore'))
    sys.stdout.flush()

def writeerr(s):
    sys.stdout.flush()
    sys.stderr.write(s.encode('ascii', 'ignore'))
    sys.stderr.flush()

def writeerrlines(lines):
    sys.stdout.flush()
    for line in lines:
        sys.stderr.write(line.encode('ascii', 'ignore'))
    sys.stderr.flush()

def get_checksum(filename):
    checksum = hashlib.md5()
    fh = open(filename, "rb")
    while True:
        buf = fh.read(4096)
        if not buf: break
        checksum.update(buf)
    fh.close()
    return checksum.hexdigest()

def parse_paths(src, root, filename):
    prefix = os.path.relpath(root, src)
    full = os.path.join(root, filename)
    key = os.path.normpath(prefix + "/" + filename)
    return full, key

def parse_checksums(src, outpath, matcher):
    fp = file(outpath, 'wb')
    fp.write('%s\n' % os.path.abspath(src).encode('utf-8') )
    for root, dirs, files in os.walk(unicode(src)):
        for filename in sorted(files):
            full, key = parse_paths(src, root, filename)
            if not matcher(full):
                continue
            fp.write('%s;%s\n' % (key.encode('utf-8'), get_checksum(full)))
    return 0

def get_lines(src):
    fh = open(src, "rb")
    lines = fh.readlines()
    fh.close()
    if lines and not lines[-1].endswith("\n"):
        lines[-1] += "\n"
    return lines

def print_diff(src, dst):
    src = get_lines(src)
    dst = get_lines(dst)
    diff = difflib.unified_diff(src, dst)
    writeerrlines(diff)

def check_checksums(src, filename, matcher):
    fh = open(filename, "rb")
    checksums = {}
    dst = fh.readline().rstrip("\r\n")
    for line in fh:
        name, checksum = line.rstrip("\r\n").split(";")
        checksums[name.decode('utf-8')] = checksum
    fh.close()
    err = 0
    for root, dirs, files in os.walk(src):
        for filename in sorted(files):
            full, key = parse_paths(src, root, filename)
            if not matcher(full):
                checksums.pop(key, None)
                continue
            if key not in checksums:
                writeerr("Unknown file " + full + "\n")
                err = -1
                continue
            if checksums[key] != get_checksum(full):
                writeerr("Invalid file " + full + "\n")
                print_diff(os.path.join(dst, key), full)
                err = -1
            del checksums[key]
    for it in checksums:
        full = os.path.join(src, it)
        writeerr("Missing file " + full + "\n")
        err = -1
    return err

def getmatcher(exclude):
    def match(fn):
        return not any(fnmatch.fnmatch(fn, e) for e in exclude)
    return match

if __name__ == '__main__':
    opts, args = parser.parse_args()
    matcher = getmatcher(opts.exclude)

    if len(args) != 3:
        parser.error( usage )

    cmd = args[0]
    inputdir = unicode(args[1])
    filename = unicode(args[2])

    ret = -1
    if not os.path.isdir(inputdir):
        writeerr("Invalid directory: " + inputdir)
        sys.exit(ret)

    if cmd == 'read':
        ret = check_checksums(inputdir, filename, matcher)
    elif cmd == 'write':
        ret = parse_checksums(inputdir, filename, matcher)
    else:
        writeerr('unknown command %s\n' % cmd)
        ret = 1
    sys.exit(ret)
