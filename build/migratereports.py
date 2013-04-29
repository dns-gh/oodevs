import os, sys, re

class Ui(object):
    def error( self, s ):
        sys.stderr.write( s )

# Parses Reports.xml, checks if the report's id exists in Types_CR and
# then adds a key attribute in the xml with Types_CR value. Warns if any
# Types_CR ids is not used in Reports.xml
def migrate_reports( ui, reports, crs, outdir ):
    ret = 0
    with open( reports, "r" ) as sources:
        lines = sources.readlines()
    with open( outdir + "/" + os.path.basename( reports ), "w" ) as sources:
        for line in lines:
            match = re.search( "(.*report.*)(id\s*=\s*[\"\'])(\d+)([\"\'].*)",
                line )
            if match:
                id = int( match.group( 3 ) )
                if not id in crs:
                    ui.error( "Could not find id '" + str( id ) + "'\n" )
                    ret = 1
                    sources.write( line )
                else:
                    key = crs[ id ]
                    sources.write( match.expand( r'\1key="' + key + r'" \2\3\4' )
                            + "\n" )
                    crs.pop( id )
            else:
                sources.write( line )
    for ( id,key ) in crs.items():
        ui.error( "Warning: id '" + str( id ) + "': " + key +
                " was not found in " + reports + "\n" )
        ret = 1
    return ret

# Parses Types_CR.lua or Types_RC.bms to add all report_key = report_id
# in a dictionnary.
def parse_cr( crs, types_cr, ui ):
    ret = 0
    with open( types_cr, "r" ) as lines:
        for line in lines:
            match = re.search( "(\w+)\s*=\s*(\d+)", line )
            if match and match.group( 1 ) != "eNbr":
                id = int( match.group( 2 ) )
                key = match.group( 1 )
                if id in crs and crs[ id ] != key:
                    ui.error( "id '" + str( id ) + "' already defined but key '"
                        + key + "' in " + types_cr
                        + " is different from key already defined '"
                        + crs[ id ] + "'\n" )
                    ret = 1
                else:
                    crs[ id ] = key
    return ret

if __name__ == "__main__":
    ui = Ui()
    args = sys.argv[1:]
    if len( args ) < 4:
        sys.stderr.write( "error: bms/Types_RC.bms, dia5/Types_CR.lua, "
             "physical/Reports.xml and out directory expected\n" )
        sys.exit( 1 )
    dia4_cr, dia5_cr, report, outdir = args[0], args[1], args[2], args[3]
    if not os.path.exists( outdir ):
        os.makedirs( outdir )
    ret = 0
    crs = {}
    for dia in (dia4_cr, dia5_cr):
        ret |= parse_cr( crs, dia, ui )
    ret |= migrate_reports( ui, report, crs, outdir )
    sys.exit( ret )