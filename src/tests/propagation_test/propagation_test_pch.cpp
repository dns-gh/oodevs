// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "propagation_test_pch.h"
#include <string>

namespace
{
    std::string data_directory;

    void set_data_directory( int argc, char* argv[] )
    {
        while( argc-- )
        {
            const std::string argument = argv[argc];
            const std::string::size_type n = argument.find( '=' );
            if( n != std::string::npos && argument.substr( 0, n ) == "--data_directory" )
                data_directory = argument.substr( n+1 );
        }
    }
}

::boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    set_data_directory( argc, argv );
    return 0;
}

tools::Path BOOST_RESOLVE( const tools::Path& filename )
{
    if( data_directory.empty() )
        return filename;
    return ( tools::Path::FromUTF8( data_directory ) / filename ).Normalize();
}

