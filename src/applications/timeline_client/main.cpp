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
#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <boost/program_options.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif
#include <iostream>

namespace bpo = boost::program_options;

int main( int argc, char* argv[] )
{
    if( timeline::core::SpawnClient() )
        return 0;
    try
    {
        timeline::core::Configuration cfg;
        bpo::positional_options_description pos;
        pos.add( "widget", 1 );
        pos.add( "uuid", 1 );
        pos.add( "url", 1 );
        bpo::options_description opts( "options" );
        opts.add_options()
            ( "help",           "print this message" )
            ( "widget",         bpo::value( &cfg.wid )->required(), "set parent widget id" )
            ( "uuid",           bpo::value( &cfg.uuid )->required(), "set parent uuid" )
            ( "url",            bpo::value( &cfg.url )->required(), "set url target" )
            ( "log",            bpo::value( &cfg.log ), "optional log file" )
            ( "debug_port",     bpo::value( &cfg.debug_port )->default_value( 0 ), "set remote debug port" );
        bpo::variables_map args;
        bpo::store( bpo::command_line_parser( argc, argv ).options( opts ).positional( pos ).run(), args );
        if( args.count( "help" ) )
        {
            std::cout << opts << std::endl;
            return 0;
        }
        bpo::notify( args );
        return timeline::core::MakeClient( cfg )->Run();
    }
    catch( const std::exception& err )
    {
        std::cerr << err.what() << std::endl;
    }
    catch( ... )
    {
        std::cerr << "Critical failure: Unexpected exception" << std::endl;
    }
    return -1;
}