import sys, os, shutil, ctypes, re
import optparse
import tools


def search(pattern,logfile,failiffound,displayresults):
    tools.writeinfo('Search %s in %r\n' % (pattern,logfile))
    f=open(logfile, 'r')
    content = f.read()
    f.close()

    matches = re.findall(pattern, content)

    if displayresults:
        for match in matches:
            tools.writeinfo("%s\n" % repr(match)[1:-1])

    if ( failiffound and matches ) or ( not failiffound and not matches ):
        tools.writeinfo("Invalid log file\n")
        return 1

    tools.writeinfo("Valid log file\n")
    return 0


usage = "%prog [OPTIONS] PATTERN LOGFILES..."
parser = optparse.OptionParser(
        usage=usage)
parser.add_option("--fail-if-found", action='store_true',
                  help="return -1 if string is found, elsewise it return -1 if the string is NOT found")
parser.add_option("--display-result", action='store_true',
                  help="hide regexp result")

if __name__ == '__main__':
    opts, args = parser.parse_args()
    args = [tools.cyg2win(p) for p in args]

    if len(args) < 2:
        parser.error("this script require at least a pattern and one log file")

    pattern = args[0]
    args = args[1:]

    for logfile in args:
        if search(pattern,logfile,opts.fail_if_found,opts.display_result) == 1:
            sys.exit(-1)
    sys.exit(0)

