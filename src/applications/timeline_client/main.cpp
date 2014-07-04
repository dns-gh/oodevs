// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include <timeline_core/api.h>
#include <tools/Path.h>
#include <tools/StdFileWrapper.h>
#include <boost/lexical_cast.hpp>
#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <boost/program_options.hpp>
#ifdef _MSC_VER
#pragma warning( pop )
#endif
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <iostream>

namespace bpo = boost::program_options;
namespace bpt = boost::posix_time;

namespace
{

std::function< void( const std::string& )> MakeLogger( const tools::Path& path )
{
    const auto mutex = boost::make_shared< boost::mutex >();
    boost::shared_ptr< tools::Ofstream > output;
    if( !path.IsEmpty() )
    {
        try
        {
            path.Parent().CreateDirectories();
        }
        catch( const std::exception& )
        {
            // Do not stop timeline_client if we fail to create the log file
        }
        output = boost::make_shared< tools::Ofstream >(
            path, std::ios::out | std::ios::binary );
    }
    return [mutex,output]( const std::string& msg )
    {
        boost::lock_guard< boost::mutex > lock( *mutex );
        std::stringstream s;
        s << bpt::to_simple_string( bpt::second_clock::local_time() )
            << " " << msg << std::endl; 
        std::cerr << s.str();
        if( output )
        {
            *output << s.str();
            output->flush();
        }
    };
}

} // namespace

int main( int argc, char* argv[] )
{
    if( timeline::core::SpawnClient() )
        return 0;
    std::function< void( const std::string& ) > logger;
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
            ( "debug_port",     bpo::value( &cfg.debug_port )->default_value( 0 ), "set remote debug port" )
            ( "log_events",     bpo::value( &cfg.log_events )->zero_tokens(), "log events" );
        bpo::variables_map args;
        bpo::store( bpo::command_line_parser( argc, argv ).options( opts ).positional( pos ).run(), args );
        if( args.count( "help" ) )
        {
            std::cout << opts << std::endl;
            return 0;
        }
        bpo::notify( args );
        logger = MakeLogger( tools::Path::FromUTF8( cfg.log ) );
        logger( "Starting timeline_client" );

        return timeline::core::MakeClient( cfg, logger )->Run();
    }
    catch( const std::exception& err )
    {
        std::cerr << err.what() << std::endl;
        if( logger )
            logger( err.what() );
    }
    catch( ... )
    {
        std::cerr << "Critical failure: Unexpected exception" << std::endl;
        if( logger )
            logger( "Critical failure: Unexpected exception" );
    }
    return -1;
}
