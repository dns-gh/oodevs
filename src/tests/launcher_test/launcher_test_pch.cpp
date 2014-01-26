// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "launcher_test_pch.h"
#include <google/protobuf/stubs/common.h>
#include <boost/lexical_cast.hpp>
#include <string>
#include <cstdlib>

unsigned short PORT = 30000;
tools::Path temp_directory;

namespace
{
    tools::Path data_directory;

    unsigned short GetMasaPort( unsigned short defval )
    {
        const char* value = getenv( "MASA_TEST_PORT" );
        if( !value )
            return defval;
        return boost::lexical_cast< unsigned short >( value );
    }

    void set_data_directory( int argc, char* argv[] )
    {
        PORT = GetMasaPort( PORT ) + 1;
        while( argc-- )
        {
            const std::string argument = argv[argc];
            const std::string::size_type n = argument.find( '=' );
            if( n != std::string::npos && argument.substr( 0, n ) == "--data_directory" )
                data_directory = argument.substr( n+1 ).c_str();
            if( n != std::string::npos && argument.substr( 0, n ) == "--temp_directory" )
                temp_directory = argument.substr( n+1 ).c_str();
            if( n != std::string::npos && argument.substr( 0, n ) == "--port_number" )
                PORT = boost::lexical_cast< unsigned short >( argument.substr( n+1 ) );
        }
    }
}

::boost::unit_test::test_suite* init_unit_test_suite( int argc, char* argv[] )
{
    set_data_directory( argc, argv );
    if( temp_directory.IsEmpty() )
        throw MASA_EXCEPTION( "Test --temp_directory option was not supplied" );
    BOOST_MESSAGE( "launcher_test test port: " << PORT );
    return 0;
}

tools::Path BOOST_RESOLVE( const tools::Path& filename )
{
    if( data_directory.IsEmpty() )
        return filename;
    return ( data_directory / filename ).Normalize();
}

struct ProtobufConfig {
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
