// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "plugins_test_pch.h"
#include <google/protobuf/stubs/common.h>
#include <iostream>
#include <stdexcept>
#include <string>

namespace
{
    tools::Path data_directory;
    tools::Path temp_directory;

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
        data_directory = value.c_str();
    
    value = get_option( argc, argv, "--temp_directory" );
    if( value.empty() )
    {
        const char message[] = "test --temp_directory option was not supplied";
        throw MASA_EXCEPTION( message );
    }
    temp_directory = value.c_str();

    return 0;
}

tools::Path BOOST_RESOLVE( const tools::Path& filename )
{
    if( data_directory.IsEmpty() )
        return filename;
    return ( data_directory / filename ).Normalize();
}

const tools::Path& GetTestTempDirectory()
{
    return temp_directory;
}

namespace
{
    struct ProtobufConfig
    {
        ProtobufConfig()
        {
            GOOGLE_PROTOBUF_VERIFY_VERSION;
        }
        ~ProtobufConfig()
        {
            google::protobuf::ShutdownProtobufLibrary();
        }
    };
    BOOST_GLOBAL_FIXTURE( ProtobufConfig );
}
