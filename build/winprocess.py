import sys, os, subprocess, shutil, ctypes, re
import optparse

# We need all these things to work around python 2.x subprocess not
# handling unicode arguments.
_kernel32 = ctypes.windll.kernel32
_BOOL = ctypes.c_long
_WORD = ctypes.c_ushort
_DWORD = ctypes.c_ulong
_LPCSTR = _LPSTR = ctypes.c_char_p
_LPCWSTR = _LPWSTR = ctypes.c_wchar_p
_LPDWORD = ctypes.POINTER(_DWORD)
_HANDLE = ctypes.c_void_p

_CREATE_NO_WINDOW = 0x08000000

_kernel32.CreateProcessA.argtypes = [_LPCWSTR, _LPCWSTR, ctypes.c_void_p,
    ctypes.c_void_p, _BOOL, _DWORD, ctypes.c_void_p, _LPCWSTR, ctypes.c_void_p,
    ctypes.c_void_p]
_kernel32.CreateProcessA.restype = _BOOL

_kernel32.WaitForSingleObject.argtypes = [_HANDLE, _DWORD]
_kernel32.WaitForSingleObject.restype = _DWORD

_kernel32.GetExitCodeProcess.argstype = [_HANDLE, _LPDWORD]
_kernel32.GetExitCodeProcess.restype = _BOOL

class _STARTUPINFO(ctypes.Structure):
    _fields_ = [('cb', _DWORD),
                ('lpReserved', _LPSTR),
                ('lpDesktop', _LPSTR),
                ('lpTitle', _LPSTR),
                ('dwX', _DWORD),
                ('dwY', _DWORD),
                ('dwXSize', _DWORD),
                ('dwYSize', _DWORD),
                ('dwXCountChars', _DWORD),
                ('dwYCountChars', _DWORD),
                ('dwFillAttribute', _DWORD),
                ('dwFlags', _DWORD),
                ('wShowWindow', _WORD),
                ('cbReserved2', _WORD),
                ('lpReserved2', ctypes.c_char_p),
                ('hStdInput', _HANDLE),
                ('hStdOutput', _HANDLE),
                ('hStdError', _HANDLE)]

class _PROCESS_INFORMATION(ctypes.Structure):
    _fields_ = [('hProcess', _HANDLE),
                ('hThread', _HANDLE),
                ('dwProcessId', _DWORD),
                ('dwThreadId', _DWORD)]

def call(args, cwd):
    exe = unicode(args[0])
    cmdline = unicode(subprocess.list2cmdline(args))
    cwd = unicode(cwd)
    si = _STARTUPINFO()
    si.cb = ctypes.sizeof(_STARTUPINFO)
    pi = _PROCESS_INFORMATION()
    res = _kernel32.CreateProcessW(exe, cmdline, None, None, False,
            _CREATE_NO_WINDOW, None, cwd, ctypes.byref(si), ctypes.byref(pi))
    if not res:
        raise ctypes.WinError()
    res = _kernel32.WaitForSingleObject(pi.hProcess, -1)
    if res:
        raise ctypes.WinError()
    code = _DWORD()
    res = _kernel32.GetExitCodeProcess(pi.hProcess, ctypes.pointer(code))
    if not res:
        raise ctypes.WinError()
    return int(code.value)
