# Edit Windows executable/DLLs resources
#
import sys, os, ctypes, struct, binascii, shutil, datetime, optparse
import pickle, pprint
import ctypes.wintypes

class Abort(Exception):
    pass

_WORD = ctypes.c_ushort
_DWORD = ctypes.c_ulong

LoadLibraryEx = ctypes.windll.kernel32.LoadLibraryExW
FreeLibrary = ctypes.windll.kernel32.FreeLibrary
EnumResourceNames = ctypes.windll.kernel32.EnumResourceNamesA
EnumResourceNameCallback = ctypes.WINFUNCTYPE(
    ctypes.wintypes.BOOL,
    ctypes.wintypes.HMODULE,
    ctypes.wintypes.LONG,
    ctypes.wintypes.LONG,
    ctypes.wintypes.LONG)
EnumResourceLanguages = ctypes.windll.kernel32.EnumResourceLanguagesA
EnumResourceLanguagesCallback = ctypes.WINFUNCTYPE(
    ctypes.wintypes.BOOL,
    ctypes.wintypes.HMODULE,
    ctypes.wintypes.LONG,
    ctypes.wintypes.LONG,
    ctypes.wintypes.WORD,
    ctypes.wintypes.LONG)
FindResource = ctypes.windll.kernel32.FindResourceA
LoadResource = ctypes.windll.kernel32.LoadResource
FreeResource = ctypes.windll.kernel32.FreeResource
SizeofResource = ctypes.windll.kernel32.SizeofResource
LockResource = ctypes.windll.kernel32.LockResource
UnlockResource = lambda x: None # hehe
CloseHandle = ctypes.windll.kernel32.CloseHandle
LoadString = ctypes.windll.user32.LoadStringA
BeginUpdateResource = ctypes.windll.kernel32.BeginUpdateResourceA
EndUpdateResource = ctypes.windll.kernel32.EndUpdateResourceA
UpdateResource = ctypes.windll.kernel32.UpdateResourceA
GetLastError = ctypes.windll.kernel32.GetLastError

# resource types
RT_CURSOR = 1
RT_BITMAP = 2
RT_ICON = 3
RT_MENU = 4
RT_DIALOG = 5
RT_STRING = 6
RT_FONTDIR = 7
RT_FONT = 8
RT_ACCELERATOR = 9
RT_RCDATA = 10
RT_MESSAGETABLE = 11
RT_VERSION = 16
RT_DLGINCLUDE = 17
RT_PLUGPLAY = 19
RT_VXD = 20
RT_ANICURSOR = 21
RT_ANIICON = 22
RT_HTML = 23
RT_MANIFEST = 24
RT_GROUP_CURSOR = RT_CURSOR + 11
RT_GROUP_ICON = RT_ICON + 11

_resnames = ("RT_CURSOR RT_BITMAP RT_ICON RT_MENU RT_DIALOG RT_STRING RT_FONTDIR "
    "RT_FONT RT_ACCELERATOR RT_RCDATA RT_MESSAGETABLE RT_VERSION RT_DLGINCLUDE "
    "RT_PLUGPLAY RT_VXD RT_ANICURSOR RT_ANIICON RT_HTML RT_MANIFEST RT_GROUP_CURSOR "
    "RT_GROUP_ICON")
resnames = {}
for n in _resnames.split():
    resnames[n.lower()] = globals()[n]

# LoadLibrary flags
DONT_RESOLVE_DLL_REFERENCES = 0x1
LOAD_LIBRARY_AS_DATAFILE = 0x2
LOAD_LIBRARY_AS_IMAGE_RESOURCE = 0x20

def getresources(path, rtypes):
    """Return a list of resources found in path which types match the input
    rtypes list. Each resource is a (type, name, languages, data) tuple.
    """
    path = unicode(path)
    module = LoadLibraryEx(path, 0, DONT_RESOLVE_DLL_REFERENCES
            | LOAD_LIBRARY_AS_DATAFILE | LOAD_LIBRARY_AS_IMAGE_RESOURCE)
    if not module:
        raise Exception("Can't read resources from file %s: %r"
                % (path, GetLastError()))

    manifests = []
    def callback(hModule, lpType, lpName, lParam):
        hResource = FindResource(hModule, lpName, lpType)
        size = SizeofResource(hModule, hResource)
        hData = LoadResource(hModule, hResource)
        try:
            ptr = LockResource(hData)
            try:
                data = ctypes.string_at(ptr, size)
            finally:
                UnlockResource(hData)
        finally:
            FreeResource(hData)

        languages = []
        def langcallback(hModule, lpType, lpName, wIDLanguage, lParam):
            languages.append(wIDLanguage)
            return True
        EnumResourceLanguages(hModule, lpType, lpName,
            EnumResourceLanguagesCallback(langcallback), None)
        manifests.append((lpType, lpName, languages, data))
        return True

    for rtype in rtypes:
        EnumResourceNames(module, rtype, EnumResourceNameCallback(callback), None)
    FreeLibrary(module)
    return manifests

def setresources(path, resources):
    """Update the resources of target module with supplied ones. Input resources
    are in the same format than those returned by getresources.
    """
    handle = BeginUpdateResource(path, False)
    if not handle:
        raise Exception('BeginUpdateResource failed with %d' % GetLastError())
    for type, name, langs, data in resources:
        for lang in langs:
            ldata = 0
            if data:
                ldata = len(data)
            ret = UpdateResource(handle, type, name, lang, data, ldata)
            if not ret:
                raise Exception('UpdateResource failed with %d' % GetLastError())
    ret = EndUpdateResource(handle, False)
    if not ret:
        raise Exception('EndUpdateResource failed with %d' % GetLastError())

# Generic header (minus the variable length szKey field) found in every
# field of RT_VERSION nodes.
class VS_VERSION_INFO(ctypes.Structure):
    _fields_ = [('wLength', _WORD),
                ('wValueLength', _WORD),
                ('wType', _WORD),
                ]
    _pack_ = 4

class VS_FIXEDFILEINFO(ctypes.Structure):
    _fields_ = [
        ('dwSignature', _DWORD),
        ('dwStrucVersion', _DWORD),
        ('dwFileVersionMS', _DWORD),
        ('dwFileVersionLS', _DWORD),
        ('dwProductVersionMS', _DWORD),
        ('dwProductVersionLS', _DWORD),
        ('dwFileFlagMask', _DWORD),
        ('dwFileFlag', _DWORD),
        ('dwFileOS', _DWORD),
        ('dwFileType', _DWORD),
        ('dwFileSubtype', _DWORD),
        ('dwFileDateMS', _DWORD),
        ('dwFileDateLS', _DWORD),
    ]
    _pack_ = 4

def readwchar(data, offset=0):
    """Read a nul-terminated WCHAR string starting at data. Return the string
    and the remaining data. Also consume padding bytes so returned data starts
    on a DWORD boundary.
    """
    key = ctypes.wstring_at(data)
    keylen = len(key) + 1
    data = data[2*keylen:]
    padding = (2*keylen + offset) % 4
    data = data[padding:]
    return key, data

class Header(object):
    """Wrap a VS_VERSION_INFO and its key strings."""

    def __init__(self, length, valuelength, type, key):
        self.length = length
        self.valuelength = valuelength
        self.type = type
        self.key = key

    def __repr__(self):
        return ('Header %s len: %d, vlen: %d, type: %d'
                % (self.key, self.length, self.valuelength, self.type))

def readheader(data, vlenunit=1):
    """Parse a Header from input data. Return it along with its related
    'value' buffer, 'inner' buffer and remaining data. The value buffer
    immediately follows the header key and is Header.valuelength*vlenunit
    bytes long. The inner buffer follows the value buffer and is made of
    all remaining data owned by the header. Remaining data is aligned on
    DWORD boundary.
    """
    assert len(data) >= ctypes.sizeof(VS_VERSION_INFO)
    ver = VS_VERSION_INFO.from_buffer_copy(data)
    data1 = data[ctypes.sizeof(VS_VERSION_INFO):]
    key, data1 = readwchar(data1, ctypes.sizeof(VS_VERSION_INFO))
    h = Header(ver.wLength, ver.wValueLength, ver.wType, key)
    assert(h.length > 0)
    inner = data[len(data) - len(data1):h.length]
    vlen = ((h.valuelength*vlenunit + 3) // 4) * 4
    value = inner[:vlen]
    inner = inner[vlen:]
    hlen = ((h.length + 3) // 4)*4
    return h, value, inner, data[hlen:]

def parsestringtable(data):
    header, _, inner, remaining = readheader(data)
    strings = []
    while inner:
        h, v, _, inner = readheader(inner, 2)
        v, _ = readwchar(v)
        strings.append((h.key, v))
    return ('stringtable', header, [], strings), remaining

def parsevar(data):
    header, value, _, _ = readheader(data)
    values = []
    assert len(value) % 4 == 0
    for i in xrange(len(value) // 4):
        v = value[4*i:4*(i+1)]
        values.append(v)
    return ('varinfo', header, [], values)

def parseuserinfo(data):
    header, _, inner, remaining = readheader(data)
    kind = header.key
    children = []
    if kind == 'StringFileInfo':
        table, inner = parsestringtable(inner)
        children.append(table)
    elif kind == 'VarFileInfo':
        var = parsevar(inner)
        children.append(var)
    return (kind, header, children, None), remaining

def parseversion(data):
    header, value, inner, _ = readheader(data)
    children = []
    version = VS_FIXEDFILEINFO.from_buffer_copy(value)
    while inner:
        userinfo, inner = parseuserinfo(inner)
        children.append(userinfo)
    return ('version', header, children, version)

def unpackversion(value):
    data = struct.pack('I', value)
    return [ord(c) for c in data[::-1]]

def versiontobytes(s):
    a, b, c, d = (int(c) for c in s.split('.'))
    parts = []
    for pairs in ((a, b), (c, d)):
        parts.append((65536*pairs[0] + pairs[1]))
    return parts

def makefixedversion(template, version):
    f = template
    f.dwFileVersionMS, f.dwFileVersionLS = versiontobytes(version)
    f.dwProductVersionMS, f.dwProductVersionLS = versiontobytes(version)
    return f

def makestring(s):
    s = (unicode(s) + u'\0').encode('utf-16')
    if s.startswith('\xff\xfe'):
        s = s[2:]
    return s

def pad(s, offset=0):
    return s + '\0'*((len(s) + offset) % 4)

def makeheader(key, value, valuelen, type=1):
    baselen = ctypes.sizeof(VS_VERSION_INFO)
    key = pad(key, baselen)
    h = VS_VERSION_INFO()
    h.wLength = baselen + len(key) + len(value)
    h.wValueLength = valuelen
    h.wType = type
    return pad(buffer(h)[:] + key + value)

def makestrings(strings):
    # Make an array of strings
    parts = []
    for k, v in strings:
        k = makestring(k)
        v = makestring(v)
        parts.append(makeheader(k, v, len(v) // 2))
    strings = ''.join(parts)

    table = makeheader(makestring("00000000"), strings, 0)
    info = makeheader(makestring('StringFileInfo'), table, 0)
    return info

def makevarinfo():
    lang = '\x00\x00\x00\x00'
    var = makeheader(makestring('Translation'), lang, len(lang), type=0)
    info = makeheader(makestring('VarFileInfo'), var, 0)
    return info

def makeversion(orig, version, name, filename):
    orig = parseversion(orig)
    fixed = buffer(makefixedversion(orig[-1], version))[:]
    year = datetime.datetime.now().year
    strings = [
        ('CompanyName', 'MASA Group'),
        ('FileDescription', name),
        ('FileVersion', version),
        ('InternalName', name),
        ('LegalCopyright', '(c) MASA group %s' % year),
        ('OriginalFilename', filename),
        ('ProductName', name),
        ('ProductVersion', version),
        ]
    stringinfo = makestrings(strings)
    varinfo = makevarinfo()
    ver = makeheader(makestring('VS_VERSION_INFO'),
            pad(fixed) + pad(stringinfo) + varinfo, len(fixed), type=0)
    return pad(ver)

def updateresources(path, opts):
    filename = opts.original_filename.replace('\\', '/')
    filename = filename.rsplit('/', 1) [-1]
    rtypes = [RT_VERSION]
    oldresources = getresources(path, rtypes)
    newresources = []
    for k in oldresources:
        rtype, name, langs, data = k
        newresources.append((rtype, name, langs, None))
        replacement = makeversion(data, opts.version, opts.product_name,
            filename)
        newresources.append((rtype, name, [1033], replacement))

    setresources(path, newresources)

def editversion(args):
    parser = optparse.OptionParser()
    parser.add_option("--original-filename", help="version OriginalFilename property")
    parser.add_option("--product-name", help="version ProductName property")
    parser.add_option("--version", help="version ProductVersion property")
    opts, args = parser.parse_args(args)

    ret = 0
    for opt in ('original_filename', 'product_name', 'version'):
        if not getattr(opts, opt, None):
            raise Abort('%s was not specified' % opt)
    path, = args
    updateresources(path, opts)

def _loadresources(path, rtypes):
    resources = pickle.load(file(path, 'rb'))
    kept = []
    for r in resources:
        if not rtypes or r[0] in rtypes:
            kept.append(r)
    return kept

def loadresources(path, rtypes):
    try:
        resources = _loadresources(path, rtypes)
    except Exception, e:
        resources = getresources(path, rtypes)
    return resources

def dumpresources(args):
    parser = optparse.OptionParser()
    opts, args = parser.parse_args(args)
    inpath, outpath = args
    rtypes = [RT_VERSION, RT_ICON, RT_GROUP_ICON]
    resources = getresources(inpath, rtypes)
    packed = []
    for k in resources:
        sys.stdout.write('saving %r\n' % (k[:-1],))
        rtype, name, langs, data = k
        packed.append((rtype, name, langs, data))
    pickle.dump(packed, file(outpath, 'wb'))

def makeresfilter(s):
    types = []
    for p in s.split(','):
        p = p.strip()
        try:
            t = int(p)
        except ValueError:
            if p.lower() not in resnames:
                raise Abort('unknown resource type: %s' % p)
            t = resnames[p.lower()]
        types.append(t)
    return set(types)

def copyres(source, dest, rtypes):
    newresources = loadresources(source, rtypes)
    destresources = getresources(dest, rtypes)
    # Remove destination icons first
    destresources = [(k[0], k[1], k[2], None) for k in destresources]
    # Then add source ones
    destresources.extend(newresources)
    setresources(dest, destresources)

def copyicons(args):
    parser = optparse.OptionParser()
    opts, args = parser.parse_args(args)
    source, dest = args
    # Load source icons
    rtypes = [RT_ICON, RT_GROUP_ICON]
    copyres(source, dest, rtypes)

def copyresources(args):
    parser = optparse.OptionParser()
    parser.add_option("--types", help="types of resource to copy")
    opts, args = parser.parse_args(args)
    if opts.types is None:
        raise Abort('no --types specified')
    source, dest = args
    rtypes = makeresfilter(opts.types)
    copyres(source, dest, rtypes)

def printversion(args):
    parser = optparse.OptionParser()
    opts, args = parser.parse_args(args)
    source, = args
    resources = loadresources(source, [RT_VERSION])
    for k in resources:
        ver = parseversion(k[-1])
        ver = pprint.pformat(ver)
        s = 'id=%s name=%s langs=%r\n%s\n\n' % (k[0], k[1], k[2], ver)
        sys.stdout.write(s)

if __name__ == '__main__':
    cmds = {
        'editversion': editversion,
        'copyicons': copyicons,
        'copy': copyresources,
        'dump': dumpresources,
        'print': printversion,
        }
    args = sys.argv[1:]
    cmd = args[0]
    args = args[1:]
    ret = 0
    try:
        if cmd not in cmds:
            raise Abort('unknown command: %s' % cmd)
        ret = cmds[cmd](args)
    except Abort, e:
        sys.stderr.write('error: %s\n' % str(e))
        ret = 1
    sys.exit(ret and 1 or 0)



