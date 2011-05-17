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
#include "StartDispatcher.h"
#include "StartEdxl.h"
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
StartExercise::StartExercise( const tools::GeneralConfig& config, const QString& exercise, const QString& session, bool attach )
    : SpawnCommand( config, "simulation_app.exe", attach )
    , exercise_ ( exercise.ascii() )
    , session_ ( session.ascii() )
    , configManipulator_ ( new ConfigurationManipulator( config_, exercise_, session_ ) )
    , percentage_( 0 )
{
    if( ! HasEmbeddedDispatcher( *configManipulator_ ) )
    {
        std::string dispatcher_path( GetEmbeddedDispatcherPath( *configManipulator_ ) );
        dispatcher_.reset( new frontend::StartDispatcher( config, attach, QString( dispatcher_path.c_str() ), exercise, session ) );
    }
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument( session );
    edxl_.reset( new StartEdxl( config, exercise, session, attach ) );
}

// -----------------------------------------------------------------------------
// Name: StartExercise constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartExercise::StartExercise( const tools::GeneralConfig& config, const QString& exercise, const QString& session, const QString& checkpoint, bool attach )
    : SpawnCommand( config, "simulation_app.exe", attach )
    , exercise_ ( exercise.ascii() )
    , session_ ( session.ascii() )
    , configManipulator_ ( new ConfigurationManipulator( config_, exercise_, session_ ) )
    , percentage_( 0 )
{
    if( ! HasEmbeddedDispatcher( *configManipulator_ ) )
    {
        std::string dispatcher_path( GetEmbeddedDispatcherPath( *configManipulator_ ) );
        std::auto_ptr< StartDispatcher > dispatcher( new frontend::StartDispatcher( config, attach, QString( dispatcher_path.c_str() ), exercise, session, checkpoint ) );
        dispatcher_ = dispatcher;
    }
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument( session );
    if( !checkpoint.isEmpty() )
        AddArgument( "--checkpoint=" + checkpoint );
    edxl_.reset( new StartEdxl( config, exercise, session, attach ) );
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

    edxl_->Start();
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
std::string StartExercise::GetStartedExercise() const
{
    return GetPercentage() == 100 ? exercise_ : std::string();
}

