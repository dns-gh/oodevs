#!/usr/bin/python -d
"""Application for generating doxygen compatible proto file
"""

import re, sys

# filters .proto file
def filter( filename ):
    lines = ""
    for line in open( filename, 'r' ):
        # replace import with #include so that references can be resolved by doxygen
        line = line.replace( "import", "#include" )
        # replace package with namespace
        line = re.sub( r"^package (.+);$", r"namespace \1 {", line )
        line = re.sub( r"^(\s*)message", r"\1struct", line )
        # replace . with :: in namespace scopes
        line = re.sub( r"(\s+[_a-zA-Z][-_a-zA-Z0-9]+)\.([_a-zA-Z][-_a-zA-Z0-9]+\s+)", r"\1::\2", line )
        line = line.replace( "}", "};" )
        # replace enumeration ; with ,
        line = re.sub( r"^(\s*\S+\s*\=\s*\S+);", r"\1,", line )
        #field indexes
        line = re.sub( r"^(\s*\S+\s+\S+\s+\S+\s*)\=\s+([0-9]+)", r"\1 /** This field's index in the message is #\2. */", line )
        #field keywords
        line = re.sub( r"^(\s*)required", r"\1required /** @invariant This field is required. */", line )
        line = re.sub( r"^(\s*)optional", r"\1optional /** @invariant This field is optional. */", line )
        # default values
        line = re.sub( r"\[\s*default\s*=\s*(\S+)\s*\];", r"/** @invariant This field's default value is \1. */ = \1;", line )
        lines = lines + line
    # strip last , of enumerations
    lines = re.sub( r",([\r\n]+\})", r"\1", lines )
    # close namespace
    print lines + "}"

if len( sys.argv ) != 2:
    print "usage: ", sys.argv[0], " filename"
    sys.exit( 1 )
filter( sys.argv[1] )
