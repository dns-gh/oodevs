// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "StartExercise.h"

#include "ConfigurationManipulator.h"
#include "DebugConfig.h"
#include "SimulationMonitor.h"
#include "StartDispatcher.h"
#include "clients_kernel/Tools.h"
#include "tools/IpcQueue.h"

#include <boost/date_time/posix_time/posix_time_types.hpp>
#include <boost/thread.hpp>

using namespace frontend;

namespace
{
    bool HasEmbeddedDispatcher( const ConfigurationManipulator& config )
    {
        return config.GetValue< bool >( "session/config/simulation/dispatcher/@embedded" );
    }

    tools::Path GetEmbeddedDispatcherPath( const ConfigurationManipulator& config )
    {
        return tools::Path::FromUTF8( config.GetValue< std::string >( "session/config/simulation/dispatcher/@path?" ) );
    }
}

// -----------------------------------------------------------------------------
// Name: StartExercise constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartExercise::StartExercise( const tools::GeneralConfig& config,
                              const tools::Path& exercise,
                              const tools::Path& session,
                              bool launchDispatchedIfNotEmbedded,
                              const std::string& name,
                              const tools::Path& checkpoint,
                              const frontend::DebugConfig& debug )
    : SpawnCommand( config, MakeBinaryName( "simulation_app" ), name )
    , exercise_ ( exercise )
    , session_ ( session )
    , configManipulator_ ( new ConfigurationManipulator( config, exercise_, session_ ) )
    , percentage_( 0 )
{
    if( ! HasEmbeddedDispatcher( *configManipulator_ ) && launchDispatchedIfNotEmbedded )
    {
        tools::Path dispatcher_path = GetEmbeddedDispatcherPath( *configManipulator_ );
        dispatcher_.reset( new frontend::StartDispatcher( config, exercise, session,
                checkpoint, dispatcher_path, "dispatcher" ) );
    }

    AddRootArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument( session );

    if( !checkpoint.IsEmpty() )
        AddArgument( "checkpoint", checkpoint.ToUTF8() );
    if( !debug.sim.integrationDir.IsEmpty() )
        AddArgument( "integration-dir", debug.sim.integrationDir.ToUTF8() );
    if( !debug.sim.pathfindDumpDir.IsEmpty() )
        AddArgument( "dump-pathfinds", debug.sim.pathfindDumpDir.ToUTF8() );
    if( !debug.sim.pathfindFilter.empty() )
        AddArgument( "filter-pathfinds", debug.sim.pathfindFilter );
    if( debug.sim.decProfiling )
        AddArgument( "--profile-dec" );
}

// -----------------------------------------------------------------------------
// Name: StartExercise destructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
StartExercise::~StartExercise()
{
    Stop();
    dispatcher_.reset();
}

// -----------------------------------------------------------------------------
// Name: StartExercise::Stop
// Created: BAX 2013-09-01
// -----------------------------------------------------------------------------
void StartExercise::Stop()
{
    if( dispatcher_.get() )
        dispatcher_->Stop();
    if( tools::ipc::Queue::Send( tools::ipc::IPC_COMMAND_STOP, GetPid() ) )
        Wait( boost::posix_time::seconds( 10 ) );
    SpawnCommand::Stop();
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
    while( !monitor.Connected() )
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

// -----------------------------------------------------------------------------
// Name: StartExercise::GetStartedExercise
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
tools::Path StartExercise::GetStartedExercise() const
{
    return GetPercentage() == 100 ? exercise_ : tools::Path();
}

// -----------------------------------------------------------------------------
// Name: StartExercise::GetExercise
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
tools::Path StartExercise::GetExercise() const
{
    return exercise_;
}

// -----------------------------------------------------------------------------
// Name: StartExercise::GetSession
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
tools::Path StartExercise::GetSession() const
{
    return session_;
}

