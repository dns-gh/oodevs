// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "StartExercise.h"
#include "StartDispatcher.h"
#include "ConfigurationManipulator.h"
#include "SimulationMonitor.h"
#include "clients_gui/Tools.h"
#include <boost/thread.hpp>

using namespace frontend;

namespace
{
    bool HasEmbeddedDispatcher( const ConfigurationManipulator& config )
    {
        return config.GetValue< bool >( "session/config/simulation/dispatcher/@embedded" );
    }

    std::string GetEmbeddedDispatcherPath( const ConfigurationManipulator& config )
    {
        return config.GetValue< std::string >( "session/config/simulation/dispatcher/@path?" );
    }
}

// -----------------------------------------------------------------------------
// Name: StartExercise constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
StartExercise::StartExercise( const tools::GeneralConfig& config, const QString& exercise, const QString& session, bool attach, bool launchDispatchedIfNotEmbedded /*= true*/, std::string commanderEndpoint /*= ""*/, bool makeSilent /*= false*/ )
    : SpawnCommand( config, "simulation_app.exe", attach, commanderEndpoint, makeSilent )
    , configManipulator_ ( new ConfigurationManipulator( config_, exercise.ascii(), session.ascii() ) )
    , percentage_( 0 )
{
    if( ! HasEmbeddedDispatcher( *configManipulator_ ) && launchDispatchedIfNotEmbedded )
    {
        QString dispatcher_path( GetEmbeddedDispatcherPath( *configManipulator_ ).c_str() );
        dispatcher_.reset( new frontend::StartDispatcher( config, attach, exercise, session, "", dispatcher_path ) );
    }
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument( session );
}

// -----------------------------------------------------------------------------
// Name: StartExercise constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartExercise::StartExercise( const tools::GeneralConfig& config, const QString& exercise, const QString& session, const QString& checkpoint, bool attach, bool launchDispatchedIfNotEmbedded /*= true*/, std::string commanderEndpoint /*= ""*/, bool makeSilent /*= false*/ )
    : SpawnCommand( config, "simulation_app.exe", attach, commanderEndpoint, makeSilent )
    , configManipulator_ ( new ConfigurationManipulator( config_, exercise.ascii(), session.ascii() ) )
    , percentage_( 0 )
{
    if( ! HasEmbeddedDispatcher( *configManipulator_ ) && launchDispatchedIfNotEmbedded )
    {
        QString dispatcher_path( GetEmbeddedDispatcherPath( *configManipulator_ ).c_str() );
        dispatcher_.reset( new frontend::StartDispatcher( config, attach, exercise, session, checkpoint, dispatcher_path ) );
    }
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument( session );
    if( !checkpoint.isEmpty() )
        AddArgument( "--checkpoint=" + checkpoint );
}

// -----------------------------------------------------------------------------
// Name: StartExercise destructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
StartExercise::~StartExercise()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StartExercise::Start
// Created: RDS 2009-08-14
// -----------------------------------------------------------------------------
void StartExercise::Start()
{
    if( dispatcher_.get() )
    {
        dispatcher_->Start();
        boost::this_thread::sleep( boost::posix_time::milliseconds( 3000 ) );
    }
    SpawnCommand::Start();
    boost::this_thread::sleep( boost::posix_time::milliseconds( 1000 ) );
    const std::string host = configManipulator_->GetValue< std::string >( "session/config/gaming/network/@server" );
    SimulationMonitor monitor( host );
    while( !monitor.Connected() && !monitor.TimedOut() )
    {
        monitor.Update();
        percentage_ = std::min< unsigned int >( percentage_ + 1, 99 );
        boost::this_thread::sleep( boost::posix_time::milliseconds( 250 ) );
    }
    percentage_ = 100;
}

// -----------------------------------------------------------------------------
// Name: StartExercise::GetPercentage
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
unsigned int StartExercise::GetPercentage() const
{
    return percentage_;
}

// -----------------------------------------------------------------------------
// Name: StartExercise::GetStatus
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
QString StartExercise::GetStatus() const
{
    if( GetPercentage() < 100 )
        return tools::translate( "StartExercise", "Starting simulation..." );
    return tools::translate( "StartExercise", "Simulation started" );
}
