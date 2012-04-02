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
    if( ! bfs::exists( root ) )
    {
        std::cout << root << " doesn't exist" << std::endl;
        return;
    }
    for( bfs::directory_iterator it( root ); it != bfs::directory_iterator(); ++it )
    {
        const bfs::path child = *it;
        if( bfs::is_regular_file( child ) && child.extension() == ".xml" )
            tools::WriteXmlCrc32Signature( child.string() );
        else if( bfs::is_directory( child ) )
            SignDirectory( child );
    }
}

int main( int argc, char* argv[] )
{
    if( argc < 2 )
    {
        std::cout << "Usage: xml_signature path [path2] ..." << std::endl;
        return 1;
    }
    for( int i = 1; i < argc; ++i )
        SignDirectory( bfs::path( argv[ i ] ) );
    return 0;
}
