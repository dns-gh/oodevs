import sys, os, subprocess, shutil, ctypes, re
import optparse
import winprocess
import searchinlog
import tools

def transform_arguments(arg, prefix, suffix):
    result = unicode(prefix + arg)
    if arg.find("exercise=") != -1 or arg.find("session=") != -1 or arg.find("root-dir=") != -1:
        result += unicode(suffix)
    return result


def retrieve_log_file(args,logfile):
    simparser = optparse.OptionParser()
    simparser.add_option("--root-dir")
    simparser.add_option("--exercise")
    simparser.add_option("--session")
    simparser.add_option("--simulation-address")
    simparser.add_option("--dispatcher-address")
    simparser.add_option("--legacy")
    simparser.add_option("--test", action='store_true')
    opts, arguments = simparser.parse_args( args )
    return opts.root_dir + '/exercises/' + opts.exercise + '/sessions/' + opts.session + '/' + logfile

parser = optparse.OptionParser(
        usage='%prog [OPTIONS] CWD EXE ARG...')
parser.add_option("--prefix", default="--",
                  help="prefix to each arguments")
parser.add_option("--append-unicode-suffix", action='store_true',
                  help="append the suffix to exercise, root-dir and session argument")
parser.add_option("--suffix", default=u" \u3091",
                  help="unicode character to append to session, root-dir and exercise arguments")

if __name__ == '__main__':
    opts, args = parser.parse_args()
    args = [tools.cyg2win(p) for p in args]
    cwd, exe = args[:2]
    args = args[2:]
    args = [transform_arguments(arg,opts.prefix,opts.suffix if opts.append_unicode_suffix else '') for arg in args]
    args.insert(0,exe)

    tools.writeinfo('Executing %s\n' % ' '.join(('%r' % a)[2:-1] for a in args)) #doesn't display unicode characters

    ret = -1
    try:
        ret = winprocess.call(args, cwd)
        # check if no error is thrown by directIA
        ret = searchinlog.search('.*functERR.*',retrieve_log_file(args[1:],'Sim.log'),True,True) if ret == 0 else ret
        # check if order file is executed
        protobuf_file = retrieve_log_file(args[1:],'Protobuf.log')
        ret = searchinlog.search('.*automat_order.*',protobuf_file,False,False) if ret == 0 else ret
    except:
        tools.writeerr('Error calling %s in %s\n' % (args, cwd))

    sys.exit(ret)
