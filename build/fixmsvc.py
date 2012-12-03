import os
import sys
import optparse
import xml.etree.ElementTree as xml

parser = optparse.OptionParser(
        usage="machin.py" )

ns = "{http://schemas.microsoft.com/developer/msbuild/2003}"

def write_file(root, filename):
    data = xml.tostring(root, encoding="utf-8")
    data = data.replace("\n", "\r\n")
    fh = open(filename, "wb")
    fh.write(data)
    fh.close()

def remove_cmakelists(root):
    for group in root.findall(ns + "ItemGroup"):
        for custom in group.findall(ns + "CustomBuild[@Include]"):
            if custom.attrib["Include"].endswith("CMakeLists.txt"):
                root.remove(group)

def merge_conditions(root, tag, include_end):
    for group in root.findall(ns + "ItemGroup/" + ns + tag + "[@Include]"):
        if not group.attrib["Include"].endswith(include_end):
            continue
        childs = {}
        conditions = {}
        for it in list(group):
            if "Condition" not in it.attrib:
                continue
            if it.tag in conditions:
                if conditions[it.tags] != it.text:
                    continue
            group.remove(it)
            del it.attrib["Condition"]
            childs[it.tag] = it
        for it in childs.values():
            group.append(it)

def fix_vcxproj(filename):
    tree = xml.parse(filename)
    root = tree.getroot()
    remove_cmakelists(root)
    # remove Debug & Release configurations
    merge_conditions(root, "ClCompile", ".cpp")
    merge_conditions(root, "CustomBuild", ".rule")
    # factorize precompiled headers
    # rename RelWithDebInfo to Release
    # add working directory
    write_file(root, filename)

def remove_cmakelists_filter(root):
    remove = True
    for group in root.findall(ns + "ItemGroup"):
        sub = group.findall(ns + "CustomBuild[@Include]")
        single = len(sub) <= 1
        remove &= single
        for custom in sub:
            if custom.attrib["Include"].endswith("CMakeLists.txt"):
                if single:
                    root.remove(group)
                else:
                    group.remove(custom)
    if not remove:
        return
    for group in root.findall(ns + "ItemGroup"):
        for sub in group.findall(ns + "Filter[@Include='cmake']"):
            group.remove(sub)

def fix_filters(filename):
    tree = xml.parse(filename)
    root = tree.getroot()
    remove_cmakelists_filter(root)
    write_file(root, filename)

def main(args, opts):
    xml.register_namespace("", 'http://schemas.microsoft.com/developer/msbuild/2003')
    root, dirs, files = os.walk(args[0]).next()
    for filename in sorted(files):
        full = os.path.join(root, filename)
        if full.endswith(".vcxproj"):
            fix_vcxproj(full)
        elif full.endswith(".vcxproj.filters"):
            fix_filters(full)
    return 0

if __name__ == '__main__':
    opts, args = parser.parse_args()
    sys.exit(main(args, opts))
