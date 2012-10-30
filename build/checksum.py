# Simple utility to make checksums of directories and compare them
import hashlib
import optparse
import os
import sys

helpstring="""\
    checksum computes checksums on <src_dir> and prints each file checksum.
    if --check <file> is specified, it compares <src_dir> and snapshots in
    <file> instead, and prints differences if any
"""
parser = optparse.OptionParser(
        usage = "checksum.py <src_dir> [--check <file>]",
        description = helpstring )
parser.add_option("-c", "--check", action="store", type="string",
        dest="filename", help="Directory checksum snapshot file")

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

def parse_checksums(src):
    for root, dirs, files in os.walk(src):
        for filename in sorted(files):
            full, key = parse_paths(src, root, filename)
            print key + ";" + get_checksum(full)
    return 0

def check_checksums(src, filename):
    fh = open(filename, "rb")
    checksums = {}
    for line in fh:
        name, checksum = line.rstrip("\r\n").split(";")
        checksums[name] = checksum
    fh.close()
    err = 0
    for root, dirs, files in os.walk(src):
        for filename in sorted(files):
            full, key = parse_paths(src, root, filename)
            if key not in checksums:
                sys.stderr.write("Unknown file " + full + "\n")
                err = -1
                continue
            if checksums[key] != get_checksum(full):
                sys.stderr.write("Invalid file " + full + "\n")
                err = -1
            del checksums[key]
    for it in checksums:
        full = os.path.join(src, it)
        sys.stderr.write("Missing file " + full + "\n")
        err = -1
    return err

if __name__ == '__main__':
    opts, args = parser.parse_args()

    if len(args) != 1:
        parser.error("checksum: missing <src_dir> argument")

    if not os.path.isdir(args[0]):
        sys.stderr.write("Invalid directory: " + args[0])
        sys.exit(-1)

    if opts.filename:
        sys.exit(check_checksums(args[0], opts.filename))
    else:
        sys.exit(parse_checksums(args[0]))
