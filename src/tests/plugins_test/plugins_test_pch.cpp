// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include <string>

namespace
{
    std::string data_directory;

    std::string get_option( int argc, char* argv[], const char* name )
    {
        while( argc-- )
        {
            const std::string argument = argv[argc];
            const std::string::size_type n = argument.find( '=' );
            if( n != std::string::npos && argument.substr( 0, n ) == name )
                return argument.substr( n+1 );
        }
        return "";
    }
}

::boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    std::string value = get_option( argc, argv, "--data_directory" );
    if( !value.empty() )
        data_directory = value;
    return 0;
}

std::string BOOST_RESOLVE( const std::string& filename )
{
    if( data_directory.empty() )
        return filename;
    return data_directory + '/' + filename;
}
