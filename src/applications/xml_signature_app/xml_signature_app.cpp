// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/convenience.hpp>
#include <tools/XmlCrc32Signature.h>

namespace bfs = boost::filesystem;

void SignDirectory( const bfs::path& root/*, const std::string& name, zip::ozipfile& zos, bool recursive*/ )
{
    bfs::directory_iterator end;
    for( bfs::directory_iterator it( root ); it != end; ++it )
    {
        const bfs::path child = *it;
        if( bfs::is_regular_file( child ) && child.extension() == ".xml" )
            tools::WriteXmlCrc32Signature( child.string() );
        else if( bfs::is_directory( child ) )
            SignDirectory( child );
    }
}

int main( int argc, char** argv )
{
    std::cout << "Usage: xml_signature path" << std::endl;
    if( argc < 2 )
        return 1;
    std::string path( argv[ 1 ] );
    SignDirectory( bfs::path( path, bfs::native ) );
    return 0;
}
