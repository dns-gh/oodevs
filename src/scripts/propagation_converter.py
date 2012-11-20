import datetime, os, re, sys, xml.etree.ElementTree as xml

def listascfiles(refdate, rootdir):
    namere = re.compile(r'^conc_D\d+_t(\d+)_k\d+\.asc', re.I)
    asc = []
    for root, dirs, files in os.walk(rootdir):
        for f in files:
            m = namere.search(f)
            if not m:
                continue
            minutes = int(m.group(1))
            date = refdate + datetime.timedelta(minutes=minutes)
            asc.append((date, os.path.join(root, f)))
    return asc

def writefile(path, root):
    file = open( os.path.join( path, "propagation.xml" ), 'w')
    xml.ElementTree(root).write(file)
    file.close()

def writeprojection( config, projectionfile):
    if projectionfile:
        projection = xml.Element('projection')
        projection.text = projectionfile
        config.append( projection )

def writeconfig(path, ascfiles, projectionfile):
    config = xml.Element('config')
    writeprojection( config, projectionfile)
    files = xml.Element('files')
    config.append(files)
    for f in ascfiles:
        child = xml.Element('file')
        child.text = f[1]
        child.attrib['time'] = f[0].strftime( "%Y-%m-%dT%H:%M:%S" )
        files.append(child)
    writefile(path, config)

if __name__ == '__main__':
    rootdir = sys.argv[1]
    projectionfile = None
    if len(sys.argv) > 2:
        projectionfile = sys.argv[2]
    refdate = datetime.datetime.strptime('2012-10-10 12:24:48', '%Y-%m-%d %H:%M:%S')
    files = listascfiles(refdate, rootdir)
    writeconfig(rootdir, files, projectionfile)
