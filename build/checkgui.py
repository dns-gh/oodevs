# Simple utility to test that win32 gui applications start
import ctypes
import optparse
import os
import subprocess
import sys
import time

from ctypes import *
from ctypes.wintypes import *

_WNDENUMPROC = ctypes.WINFUNCTYPE(BOOL, HANDLE, LPARAM)
_WM_CLOSE = 0x0010

TIMEOUT_S = 60
TIMESTEP_MS = 100

parser = optparse.OptionParser(
        usage = "check_start_gui.py [--run_dir <dir>] <app.exe> <args>*" )
parser.add_option("-r", "--run_dir", action="store", type="string",
        dest="run_dir", help="Run directory")
parser.add_option("--logfile", help="log file path")

def split_args():
    sub = sys.argv[:1]
    args = []
    is_sub = True
    # does not support flags but we don't care
    for arg in sys.argv[1:]:
        if is_sub:
            if arg == "--":
                is_sub = False
            else:
                sub.append(arg)
        else:
            args.append(arg)
    return sub, args

def display_log(status, logfile):
    try:
        log = file(logfile, 'rb').read()
        sys.stderr.write(log)
        sys.stderr.write('\n')
        sys.stderr.write("error code 0x%X\n" % (status & 0xFFFFFFFF))
    except (IOError, OSError):
        pass

def main(opts, args):
    if len(args) < 1:
        parser.error("missing application argument")

    if opts.run_dir:
        if not os.path.isdir(opts.run_dir):
            sys.stderr.write("Invalid directory: " + opts.run_dir)
            sys.exit(-1)
    if opts.logfile and os.path.isfile(opts.logfile):
        os.unlink(opts.logfile)

    null = open(os.devnull, "w")
    proc = subprocess.Popen(args, cwd = opts.run_dir,
            stdout = null, stderr = null)
    kill = True
    try:
        wait_for_startup(proc.pid, TIMEOUT_S*1000)
        wait_for_shutdown(proc.pid, TIMEOUT_S*1000)
        proc.wait()
        kill = False
        if proc.returncode and opts.logfile:
            display_log(proc.returncode, opts.logfile)
        return proc.returncode
    finally:
        if kill:
            proc.kill()
        display_log(-1, opts.logfile)
        null.close()

def get_hwnds(pid):
    hwnds = []
    def callback(hwnd, param):
        hwnd_pid = DWORD()
        windll.user32.GetWindowThreadProcessId(hwnd, ctypes.byref(hwnd_pid))
        if pid == hwnd_pid.value:
            hwnds.append(hwnd)
        return True
    windll.user32.EnumWindows(_WNDENUMPROC(callback), 0)
    return hwnds

def wait_for_startup(pid, timeout_ms):
    skip = 0
    while True:
        hwnds = get_hwnds(pid)
        if len(hwnds):
            return
        skip += TIMESTEP_MS
        if skip > timeout_ms:
            raise Exception("Timeout on startup")
        time.sleep(TIMESTEP_MS/1000.0)

def wait_for_shutdown(pid, timeout_ms):
    done = {}
    skip = 0
    while True:
        hwnds = get_hwnds(pid)
        for hwnd in hwnds:
            if hwnd in done:
                continue
            windll.user32.PostMessageA(hwnd, _WM_CLOSE, 0, 0)
            done[hwnd] = True
        if not len(hwnds):
            return
        skip += TIMESTEP_MS
        if skip > timeout_ms:
            raise Exception("Timeout on shutdown")
        time.sleep(TIMESTEP_MS/1000.0)

if __name__ == '__main__':
    try:
        sub, args = split_args()
        opts, _ = parser.parse_args(args = sub)
        rpy = main(opts, args)
        if rpy:
            sys.stderr.write("Unable to start " + os.path.basename(args[0]))
            sys.exit(-1)
    except Exception as err:
        sys.stderr.write(str(err))
        sys.exit(-1)
