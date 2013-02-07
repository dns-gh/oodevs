// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "geostore_test_pch.h"
#include <tools/Exception.h>

std::string temp_directory;

namespace
{
    std::string data_directory;

    void parse_options( int argc, char* argv[] )
    {
        while( argc-- )
        {
            const std::string argument = argv[argc];
            const std::string::size_type n = argument.find( '=' );
            if( n != std::string::npos && argument.substr( 0, n ) == "--data_directory" )
                data_directory = argument.substr( n+1 );
            if( n != std::string::npos && argument.substr( 0, n ) == "--temp_directory" )
                temp_directory = argument.substr( n+1 );
        }
    }
}

::boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    parse_options( argc, argv );
    if( temp_directory.empty() )
        throw MASA_EXCEPTION( "Test --temp_directory option was not supplied" );
    return 0;
}


std::string BOOST_RESOLVE( const std::string& filename )
{
    if( data_directory.empty() )
        return filename;
    return data_directory + '/' + filename;
}
