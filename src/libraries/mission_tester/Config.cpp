// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Config.h"
#include "Client.h" 
#include "ConsoleLogger.h"
#include "Facade.h"
#include "FileLogger.h"
#include "SchedulerFactory.h"
#include "Timeout.h"
#include "client_proxy/SwordMessageHandler_ABC.h"
#include "tools/NullFileLoaderObserver.h"
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>

using namespace mission_tester;
namespace bpo = boost::program_options;

// -----------------------------------------------------------------------------
// Name: Config constructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
Config::Config( int argc, char** argv )
    : T_FileLoader( new tools::NullFileLoaderObserver() )
    , tools::ExerciseConfig( *T_FileLoader::member )
{
    bpo::options_description desc( "Tester options" );
    desc.add_options()
        ( "configuration", bpo::value< std::string >( &configurationFile_ ) , "set configuration file" );
    AddOptions( desc );
    Parse( argc, argv );
    xml::xifstream xis( configurationFile_ );
    xis >> xml::start( "configuration" )
        >> xml::content( "host", host_ )
        >> xml::content( "port", port_ )
        >> xml::content( "login", login_ )
        >> xml::content( "output", logFile_ )
        >> xml::content( "scheduler", scheduler_ )
        >> xml::content( "timeout", timeout_ );
    password_ = xis.content( "password", "" );
}

// -----------------------------------------------------------------------------
// Name: Config destructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
Config::~Config()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Config::ConfigureLogging
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
void Config::ConfigureLogging( Facade& facade ) const
{
    facade.AddListener( boost::shared_ptr< Listener_ABC >( new ConsoleLogger() ) );
    if( !logFile_.empty() )
    {
        try
        {
            boost::shared_ptr< Listener_ABC > logger( new FileLogger( logFile_ ) );
            facade.AddListener( logger );
        }
        catch( ... )
        {
            // $$$$ PHC 2011-04-07: Unable to create logging file
        }
    }
}

// -----------------------------------------------------------------------------
// Name: boost::auto_ptr< Client > Config::CreateClient
// Created: HBD 2011-04-20
// -----------------------------------------------------------------------------
std::auto_ptr< Client > Config::CreateClient( SwordMessageHandler_ABC& handler ) const
{
    return new std::auto_ptr< Client >( new Client( handler, host_, port_, login_, password_ ) );
}

// -----------------------------------------------------------------------------
// Name: std::auto_ptr< SchedulerFactory > Config::CreateSchedulerFactory
// Created: HBD 2011-04-21
// -----------------------------------------------------------------------------
std::auto_ptr< SchedulerFactory > Config::CreateSchedulerFactory( ) const
{
    return new std::auto_ptr< SchedulerFactory > ( new SchedulerFactory( scheduler_ ) );
}

// -----------------------------------------------------------------------------
// Name: std::auto_ptr< Timeout > Config::CreateTimeout
// Created: HBD 2011-04-21
// -----------------------------------------------------------------------------
std::auto_ptr< Timeout > Config::CreateTimeout() const
{
    return new std::auto_ptr< Timeout > ( new Timeout( timeout_ ) );
}
