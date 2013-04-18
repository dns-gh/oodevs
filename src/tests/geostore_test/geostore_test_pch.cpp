// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "geostore_test_pch.h"
#include <tools/Exception.h>

tools::Path temp_directory;

namespace
{
    tools::Path data_directory;

    void parse_options( int argc, char* argv[] )
    {
        while( argc-- )
        {
            const std::string argument = argv[argc];
            const std::string::size_type n = argument.find( '=' );
            if( n != std::string::npos && argument.substr( 0, n ) == "--data_directory" )
                data_directory = argument.substr( n+1 ).c_str();
            if( n != std::string::npos && argument.substr( 0, n ) == "--temp_directory" )
                temp_directory = argument.substr( n+1 ).c_str();
        }
    }
}

::boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    parse_options( argc, argv );
    if( temp_directory.IsEmpty() )
        throw MASA_EXCEPTION( "Test --temp_directory option was not supplied" );
    return 0;
}


tools::Path BOOST_RESOLVE( const tools::Path& filename )
{
    if( data_directory.IsEmpty() )
        return filename;
    return data_directory / filename;
}