// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include <timeline_core/api.h>

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <QtCore>
#include <cef_app.h>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

namespace
{
int submain( int argc, char* argv[] )
{
    QCoreApplication app( argc, argv );
    try
    {
        if( argc != 4 )
            throw std::exception( "usage: timeline_view <wid> <uuid> <url>" );
        timeline::core::Configuration cfg;
        bool valid;
        cfg.wid = QString( argv[1] ).toInt( &valid, 10 );
        if( !valid )
            throw std::runtime_error( QString( "unable to convert %1 to integer" ).arg( argv[1] ).toStdString() );
        cfg.uuid = argv[2];
        cfg.url = argv[3];
        auto core = timeline::core::MakeContext( cfg );
        return app.exec();
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
}

int main( int argc, char* argv[] )
{
    CefSettings settings;
    settings.multi_threaded_message_loop = true;
    CefInitialize( settings, 0 );
    submain( argc, argv );
    CefShutdown();
}