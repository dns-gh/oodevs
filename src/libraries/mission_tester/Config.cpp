// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "mission_tester_pch.h"
#include "Client.h" 
#include "Config.h"
#include "ConsoleLogger.h"
#include "Exercise.h"
#include "Facade.h"
#include "FileLogger.h"
#include "CSVFileLogger.h"
#include "SchedulerFactory.h"
#include "Timeout.h"
#include "client_proxy/SwordMessageHandler_ABC.h"
#include "tools/NullFileLoaderObserver.h"
#pragma warning( push, 0 )
#include <boost/program_options.hpp>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>
#include "Scheduler_ABC.h"

using namespace mission_tester;
namespace bpo = boost::program_options;

namespace
{
    E_EntityType StringToEntityType( const std::string& entityType )
    {
        if( entityType == "agent" )
            return agent;
        if( entityType == "automat" )
            return automat;
        throw std::runtime_error( __FUNCTION__ " unknown entity type '" + entityType + "'" );
    }
}
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
    xml::xifstream xis( configurationFile_, xml::internal_grammar() );
    xis >> xml::start( "configuration" )
          >> xml::start( "initialization" )
            >> xml::attribute( "host", host_ )
            >> xml::attribute( "port", port_ )
            >> xml::attribute( "login", login_ )
            >> xml::attribute( "output", logFile_ )
            >> xml::attribute( "scheduler", scheduler_ )
            >> xml::attribute( "timeout", timeout_ )
            >> xml::optional >> xml::attribute( "password", password_ )
          >> xml::end;
    std::string entityType;
    xis   >> xml::start( "filters" )
            >> xml::attribute( "entity-type", entityType )
            >> xml::attribute( "fragorders", withFragOrders_ )
          >> xml::end;
    entityType_ = StringToEntityType( entityType );

    xibs_.reset( new xml::xibufferstream( xis >> xml::start( "points" ) ) );
}

// -----------------------------------------------------------------------------
// Name: Config destructor
// Created: PHC 2011-04-07
// -----------------------------------------------------------------------------
Config::~Config()
{
    xibs_.release();
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
            facade.AddListener( boost::shared_ptr< Listener_ABC >( new FileLogger( logFile_ ) ) );
            facade.AddListener( boost::shared_ptr< Listener_ABC >( new CSVFileLogger( logFile_ ) ) );
        }
        catch( ... )
        {
            throw std::runtime_error( __FUNCTION__ " unable to create log file '" + logFile_ + "'" );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Config::CreateClient
// Created: HBD 2011-04-20
// -----------------------------------------------------------------------------
std::auto_ptr< Client > Config::CreateClient( SwordMessageHandler_ABC& handler ) const
{
    return std::auto_ptr< Client >( new Client( handler, host_, port_, login_, password_ ) );
}

// -----------------------------------------------------------------------------
// Name: Config::CreateSchedulerFactory
// Created: HBD 2011-04-21
// -----------------------------------------------------------------------------
std::auto_ptr< SchedulerFactory > Config::CreateSchedulerFactory( ) const
{
    return std::auto_ptr< SchedulerFactory >( new SchedulerFactory( scheduler_, withFragOrders_ ) );
}

// -----------------------------------------------------------------------------
// Name: Config::CreateTimeout
// Created: HBD 2011-04-21
// -----------------------------------------------------------------------------
std::auto_ptr< Timeout > Config::CreateTimeout() const
{
    return std::auto_ptr< Timeout >( new Timeout( timeout_ ) );
}

// -----------------------------------------------------------------------------
// Name: Config::CreateExercise
// Created: PHC 2011-04-22
// -----------------------------------------------------------------------------
std::auto_ptr< Exercise > Config::CreateExercise( kernel::EntityResolver_ABC& entities, const kernel::StaticModel& staticModel, Publisher_ABC& publisher ) const
{
    return std::auto_ptr< Exercise >( new Exercise( entities, staticModel, publisher, *xibs_ ) );
}

// -----------------------------------------------------------------------------
// Name: Config::CreateScheduler
// Created: PHC 2011-05-12
// -----------------------------------------------------------------------------
std::auto_ptr< Scheduler_ABC > Config::CreateScheduler( SchedulerFactory& factory ) const
{
    if( entityType_ == agent )
        return factory.CreateAgentScheduler();
    if( entityType_ == automat )
        return factory.CreateAutomatScheduler();
    return 0;
}
