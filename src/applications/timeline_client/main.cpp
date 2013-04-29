// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include <timeline_core/api.h>
#include <boost/lexical_cast.hpp>

int main( int argc, char* argv[] )
{
    try
    {
        if( argc != 4 )
            throw std::exception( "usage: timeline_view <wid> <uuid> <url>" );
        timeline::core::Configuration cfg;
        cfg.wid = boost::lexical_cast< int >( argv[1] );
        cfg.uuid = argv[2];
        cfg.url = argv[3];
        return timeline::core::MakeClient( cfg )->Run();
    }
    catch( const std::exception& err )
    {
        // FIXME use a logger
        fprintf( stderr, "%s\n", err.what() );
    }
    catch( ... )
    {
        fprintf( stderr, "Critical failure: Unexpected exception\n" );
    }
    return -1;
}