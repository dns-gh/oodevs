#!/usr/bin/python -d
"""Application for generating doxygen compatible dec file
"""

import re, sys
import os
import fnmatch, fileinput
import shutil
import string
import pprint
import copy

from datetime import date

def enum( **enums ):
    return type('Enum', (), enums)

def strip_newline( line ):
    if line[-1:] == '\n':
        line = line[:-1]
    return line

class DEC_Cleaner:

    def __init__( self ):
        self.STATES = enum(EXIT_BLOCK = 0, PREPARE_BLOCK = 1, IN_BLOCK = 2)
        self.currentStates_ = self.STATES.EXIT_BLOCK
        self.count_ = 0
        self.comments_ = []
        self.blockComments_ = False
    
    def test_match_dec(self, line):
        result = re.match( r"(\w+.*)\s+SWORD_DEC_API::(\w+)(\(.*\)).*", line)
        if result:
            return result.group(1), result.group(2), result.group(3)
        return '', '', ''
        
    def profile_and_keep_line( self, line ):
        
        log_info = 'Checking line [' + str(self.currentStates_) + ', ' + str(self.count_) + ']: ' + line
        # print log_info

        ## clean up comments buffer
        if re.match( r".*}.*", line):
            self.comments_ = []
        
        if self.currentStates_ == self.STATES.EXIT_BLOCK:
            result = re.match( r"^//", line)
            if not result:
                result = re.match( r"(\w+.*)\s+SWORD_DEC_API::(\w+)(\(.*\)).*", line)
                if result:
                    self.currentStates_ = self.STATES.PREPARE_BLOCK
                    return True
                    
        if self.currentStates_ == self.STATES.PREPARE_BLOCK:
            if re.match( r"\s*{.*", line):
                if self.count_ == 0:
                    self.currentStates_ = self.STATES.IN_BLOCK
                    self.count_ = 1
                    return True
        
        if self.currentStates_ == self.STATES.IN_BLOCK:
            if re.match( r".*}", line):
                self.count_ = self.count_ - 1
                if self.count_ == 0:
                    self.currentStates_ = self.STATES.EXIT_BLOCK
                    return True
            if re.match( r"\s*{.*", line):
                self.count_ = self.count_ + 1
            return False
        
        return False
        
    def record_comment( self, line ):
        if re.match( r"/\*.*", line):
            self.blockComments_ = True
        if self.blockComments_ or re.match( r"^//.*", line):
            self.comments_.append(strip_newline(line))
        if re.match( r".*\*/", line):
            self.blockComments_ = False
    
    def inject_comment( self ):
        if self.currentStates_ == self.STATES.PREPARE_BLOCK and len(self.comments_):
            # value = self.comments_
            # self.comments_ = []
            return self.comments_
        return []
        
class DEC_Function_Container:
    def __init__ ( self, ret_value, params, comments ):
        self.ret_value = str(ret_value)
        self.params = str(params)
        self.comments = comments
        
    def update_comment( self, comments ):
        self.comments = self.comments + comments
        
        #print self.comments
        #print "defining { %s function( %s ) : \n }" % (self.ret_value, self.params )
        
    def __str__ ( self ):
        return "{ %s function( %s ) : \n }".format(self.ret_value, self.params)
    
class DEC_Recorder:
    
    # 1. Read binding definition and records associated files and C++ function
    # 2. Copy .cpp files implementing binded functions
    # 3. Implement a common .h file gathering every dec functions
    # 4. Replace and cleanup function implementation
    
    def __init__ (self, dir_export):
        self.files_def_ = {}
        self.dir_export_ = dir_export
        os.mkdir(self.dir_export_)
        self.groups_ = dict() 
        
    # read bindings and record the file pointer
    def record_registered_brain( self, filename ):
        for line in open( filename, 'r' ):
            result = len(line) > 0 \
                and re.match( r'.*brain\[\s*"(\w+)".*\&(\w+::\w+).*', line)  \
                or re.match( r'.*brain\.Register\(\s*"(\w+)".*\&(\w+::\w+).*', line)
            if result:
                self.files_def_[result.group(2)] = result.group(1)

    # find . -name "*.cpp" | xargs grep -e "brain\.Register(" -e "brain\[.*=" | grep -v "\.svn"  | cut -d: -f1 | sort -u    
    def log_files( self, filename ):
        for line in open( filename, 'r' ):
            self.record_registered_brain(strip_newline(line))
    
    def update_implementation( self, filename, impl_function, dec_function ):
        export_def = ( "", "" )
        printer = ""
        cleaner = DEC_Cleaner()
        
        for line in fileinput.input(filename):
            cleaner.record_comment( line )
            
            value = False
            result = re.match( r"^//", line) or re.match( r";$", line)
            if not result:
                result = re.match( r"(\w+.*)\s+" + impl_function + r"(\(.*\)).*", line )
                if result:
                    export_def = result.group(1), result.group(2)
                    line = re.sub( r"(\w+.*)\s+" + impl_function + r"(\(.*\)).*", r"\1 SWORD_DEC_API::" + dec_function + r"\2", line )
                    printer = printer + line
                    value = True
                    
            if cleaner.profile_and_keep_line(line):
                ret_value, params = export_def
                self.record_entry(ret_value, dec_function, params, cleaner.inject_comment())
        
    
    def record_entry( self, ret_value, dec_function, params, comments ):
        dec_group = re.sub( r"DEC_([a-zA-Z0-9]+)_.*", r"\1", dec_function )
        if dec_group == dec_function:
            dec_group = "Misc."
        if not dec_group in self.groups_:
            self.groups_[ dec_group ] = dict()
        if not str(dec_function) in self.groups_[ dec_group ]:
            self.groups_[ dec_group ][ str(dec_function) ] = DEC_Function_Container( ret_value, params, comments )
        else:
            self.groups_[ dec_group ][ str(dec_function) ].update_comment( comments )
                
    # search for the file reference to copy it to dir_copy
    def search_and_copy( self, dir_ref, dir_copy, filename ):        
        for root, dirs, files in os.walk( dir_ref ):
            result = re.match( r".*\.svn.*", root)
            if not result:
                for file in fnmatch.filter(files, filename):
                    shutil.copy(os.path.join(root, file), dir_copy) 
    
    def check_exist_copy( self, dir_copy, filename ):
        return os.path.isfile(os.path.join(dir_copy, filename));
    
    def extract_file_name( self, signature ):
        result = re.match( r"(\w+)::(\w+)", signature)
        return not result or result.group(1) + ".cpp"
    
    def write_header_file( self, filename ):
        sequence = []
        sorted_groups = [x for x in self.groups_.iteritems()] 
        sorted_groups.sort( key=lambda x: x[0] ) # sort by key
        for name, value in sorted_groups:
            sequence.append( "//! @name %s \n" % name )
            sequence.append( "//@{ \n" )
            
            sorted_functions = [x for x in value.iteritems()] 
            sorted_functions.sort( key=lambda x: x[0] ) # sort by key
            for func_name, func_info in sorted_functions:
                sequence.append( "\n" )
                for comment in func_info.comments:
                    sequence.append( "%s\n" % comment )
                sequence.append( "    %s %s%s;" % (func_info.ret_value, func_name, func_info.params ) )
                sequence.append( "\n" )
            sequence.append( "//@} \n" )
        
        self.sword_api_h_file = open(os.path.join(self.dir_export_, filename), 'w');
        self.sword_api_h_file.writelines( [
            '// *****************************************************************************\n',
            '//\n',
            '// This file is part of a MASA library or program.\n',
            '// Refer to the included end-user license agreement for restrictions.\n',
            '// \n',
            '// Generated for DEC API on %s \n' % str(date.today()),
            '// Copyright (c) 2012 MASA Group\n',
            '//\n',
            '// *****************************************************************************\n' ] )
            
        self.sword_api_h_file.writelines( [ 
            "#ifndef __SWORD_DEC_API_\n",
            "# define __SWORD_DEC_API_\n",
            "\n",
            "class SWORD_DEC_API\n",
            "{\n",
            "    public:\n" ] )
        self.sword_api_h_file.writelines( sequence )
        self.sword_api_h_file.writelines( [
            "};\n",
            "\n",
            "#endif __SWORD_DEC_API_\n" ] )
        
        self.sword_api_h_file.close()
        
    # investigate definitions and update documented files
    def export_implementation( self, dir ):
        for impl_function, dec_function in self.files_def_.iteritems():
            file = self.extract_file_name( impl_function )
            if not self.check_exist_copy( self.dir_export_, file):
                self.search_and_copy( dir, self.dir_export_, file )
            self.update_implementation( os.path.join(self.dir_export_, file), impl_function, dec_function)
        self.write_header_file( "sword_dec_api.h" )        
        
    def log_print( self ):
        pp = pprint.PrettyPrinter(indent=4)
        pp.pprint( self.groups_ )

        
if len( sys.argv ) != 4 and len( sys.argv ) != 2:
    print "usage: ", sys.argv[0], " def-file path-dir out-dir"
    sys.exit( 1 )

if len( sys.argv ) == 4:
    print "run python script: %s %s %s %s" % ( sys.argv[0], sys.argv[1], sys.argv[2], sys.argv[3] ) 
    recorder = DEC_Recorder(str(sys.argv[3]))
    recorder.log_files(str(sys.argv[1]))
    recorder.export_implementation(str(sys.argv[2]))
    
elif sys.argv[1] == "test":
    cleaner = DEC_Cleaner()
    _, dec, _ = cleaner.test_match_dec("std::vector< DEC_Decision_ABC* > SWORD_DEC_API::DEC_Automate_PionsGenie( const MIL_Automate& callerAutomate )")
    print dec
    
    dec_group = re.sub( r"DEC_(\w+)_.*", r"\1", "DEC_Connaissances_UnitesEnnemiesVivantesPercuesDansFuseau" )
    print dec_group
    dec_group = re.sub( r"DEC_(\w+)_.*", r"\1", "SetConnaissances_" )
    print dec_group
    
    # filter("./dec_api/definition/DEC_GeometryFunctions.cpp")
