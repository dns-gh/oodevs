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
#include <boost/static_assert.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif
#include <iostream>

namespace bpo = boost::program_options;

int main( int argc, char* argv[] )
{
    QT_REQUIRE_VERSION( argc, argv, "4.7.0" );
    QApplication app( argc, argv );
    QObject::connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    try
    {
        timeline::LibraryConfiguration libcfg;
        std::string command;
        std::vector< std::string > cmdargs;
        timeline::Configuration cfg;
        bpo::positional_options_description pos;
        pos.add( "cmdargs", -1 );
        bpo::options_description opts( "options" );
        opts.add_options()
            ( "help",       "print this message" )
            ( "url",        bpo::value( &cfg.url )->default_value( "http://www.google.com" ), "set url target" )
            ( "command",    bpo::value( &command )->default_value( std::string() ), "execute optional command and return" )
            ( "cmdargs",    bpo::value( &cmdargs ), "optional command arguments" )
            ( "server_log", bpo::value( &cfg.server_log ), "output server log filename" )
            ( "client_log", bpo::value( &libcfg.client_log ), "output client log filename" )
            ( "debug_port", bpo::value( &libcfg.debug_port )->default_value( 0 ), "set remote debug port" );
        bpo::variables_map vmap;
        bpo::store( bpo::command_line_parser( argc, argv ).options( opts ).positional( pos ).run(), vmap );
        if( vmap.count( "help" ) )
        {
            std::stringstream stream;
            stream << opts;
            const auto usage = "<pre>" + stream.str() + "</pre>";
            QMessageBox::information( NULL, "Help", usage.c_str() );
            return 0;
        }
        bpo::notify( vmap );
        const auto lib = timeline::Initialize( libcfg );
        Controller controller( cfg );
        if( !command.empty() )
            return controller.Execute( command, cmdargs );
        controller.Show();
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
