// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include <timeline/api.h>

#include "Controller.h"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <QtGui>
#include <boost/program_options.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif
#include <iostream>

namespace bpo = boost::program_options;

int main( int argc, char* argv[] )
{
    if( timeline::SpawnServer() )
        return 0;

    QT_REQUIRE_VERSION( argc, argv, "4.7.0" );
    QApplication app( argc, argv );
    QObject::connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    try
    {
        timeline::Configuration cfg;
        bpo::positional_options_description pos;
        pos.add( "binary", 1 );
        pos.add( "url", 1 );
        bpo::options_description opts( "options" );
        opts.add_options()
            ( "help",       "print this message" )
            ( "binary",     bpo::value( &cfg.binary )->required(), "set client binary when using external process" )
            ( "url",        bpo::value( &cfg.url )->required(), "set url target" )
            ( "external",   bpo::value( &cfg.external )->default_value( true ), "use external process" )
            ( "debug_port", bpo::value( &cfg.debug_port )->default_value( 0 ), "set remote debug port" );
        bpo::variables_map args;
        bpo::store( bpo::command_line_parser( argc, argv ).options( opts ).positional( pos ).run(), args );
        if( args.count( "help" ) )
        {
            std::stringstream stream;
            stream << opts;
            const auto usage = "<pre>" + stream.str() + "</pre>";
            QMessageBox::information( NULL, "Help", usage.c_str() );
            return 0;
        }
        bpo::notify( args );
#ifdef _WIN64
        if( !cfg.external )
            throw std::exception( "Unable to disable external process in 64-bit mode" );
#endif

        cfg.rundir = ".";
        if( !cfg.binary.IsRegularFile() )
            throw std::runtime_error( QString( "invalid file %1" ).arg( argv[1] ).toStdString() );

        timeline::Controller controller( cfg );
        return app.exec();
    }
    catch( const std::exception& err )
    {
        QMessageBox::critical( NULL, "Critical failure", err.what() );
        return -1;
    }
    catch( ... )
    {
        QMessageBox::critical( NULL, "Critical failure", "Unexpected error" );
        return -1;
    }
}
