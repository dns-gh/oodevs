import sys, os, subprocess, optparse

parser = optparse.OptionParser()
parser.add_option("--run-dir", help="run directory")
parser.add_option("--logfile", help="log file path")

if __name__ == '__main__':
    # Separate script arguments from application ones
    args, appargs = [], []
    isarg = True
    for arg in sys.argv[1:]:
        if arg == '--':
            isarg = False
        elif isarg:
            args.append(arg)
        else:
            appargs.append(arg)

    opts, args = parser.parse_args(args)
    rundir = None
    if opts.run_dir:
        if not os.path.exists(opts.run_dir):
            raise Exception('run directory does not exist %s' % opts.run_dir)
        rundir = opts.run_dir
    if opts.logfile and os.path.isfile(opts.logfile):
        os.unlink(opts.logfile)

    p = subprocess.Popen(appargs, stderr=subprocess.STDOUT, cwd=rundir)
    stdout = p.communicate()[0]
    if p.returncode:
        sys.stderr.write((stdout or '') + '\n')
        if opts.logfile:
            try:
                log = file(opts.logfile, 'rb').read()
                sys.stderr.write(log + '\n')
            except (IOError, OSError):
                pass
        sys.stderr.write('%r exited with %r\n' % (appargs, p.returncode))
    sys.exit(p.returncode)

