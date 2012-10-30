// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_test_pch.h"
#include <google/protobuf/stubs/common.h>
#include <string>

#pragma warning( disable : 4996 ) // 'std::_Swap_ranges': Function call with parameters that may be unsafe

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

std::string BOOST_RESOLVE( const std::string& filename )
{
    if( data_directory.empty() )
        return filename;
    return data_directory + '/' + filename;
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

namespace plugins
{
namespace hla
{
    std::vector< char > MakeUniqueId( const std::string& s )
    {
        return MakeUniqueIdGen< 11 >( s );
    }
    NETN_UUID MakeNetnUid( const std::string& s )
    {
        return NETN_UUID( MakeUniqueIdGen< 16 >( s ) ) ;
    }
}
}

BOOST_GLOBAL_FIXTURE( ProtobufConfig );
