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
#include "ExerciseListener.h" 
#include "ConfigurationManipulator.h" 
#include "clients_gui/Tools.h"

#include <boost/thread.hpp>
#include <windows.h>
#include <winbase.h>

using namespace frontend;

namespace 
{
    class StartDispatcher : public SpawnCommand
    {
    public:
        StartDispatcher( const tools::GeneralConfig& config, const QString& exercise, const QString& session, bool attach )
            : SpawnCommand( config, "dispatcher_app.exe", attach )
        {
         //   SetWorkingDirectory( "./bin/" );
            AddRootDirArgument();
            AddExerciseArgument( exercise );
            AddSessionArgument( session );
        }

        StartDispatcher( const tools::GeneralConfig& config, const QString& exercise, const QString& session, const QString& checkpoint, bool attach )
            : SpawnCommand( config, "dispatcher_app.exe", attach )
        {
           // SetWorkingDirectory( "./bin/" );
            AddRootDirArgument();
            AddExerciseArgument( exercise );
            AddSessionArgument( session );
            addArgument( "--checkpoint=" + checkpoint );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: StartExercise constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
StartExercise::StartExercise( const tools::GeneralConfig& config, const QString& exercise, const QString& session, bool dispatcher, bool attach )
    : SpawnCommand( config, "simulation_app.exe", attach )
    , exercise_ ( exercise.ascii() )
    , session_ ( session.ascii() ) 
    , configManipulator_( new ConfigurationManipulator( config_, exercise_, session_ ) )
{
    if ( dispatcher )
        dispatcher_.reset( new ::StartDispatcher( config, exercise, session, attach ) );
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument( session );
}

// -----------------------------------------------------------------------------
// Name: StartExercise constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartExercise::StartExercise( const tools::GeneralConfig& config, const QString& exercise, const QString& session, const QString& checkpoint, bool dispatcher, bool attach )
    : SpawnCommand( config, "simulation_app.exe", attach )
    , exercise_ ( exercise.ascii() )
    , session_ ( session.ascii() )
    , configManipulator_( new ConfigurationManipulator( config_, exercise_, session_ ) )
{
    if ( dispatcher )
        dispatcher_.reset( new ::StartDispatcher( config, exercise, session, checkpoint, attach ) );
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument( session );
    addArgument( "--checkpoint=" + checkpoint );
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
        Sleep( 3000 );
    }
    SpawnCommand::Start();
}

// -----------------------------------------------------------------------------
// Name: StartExercise::Start
// Created: RDS 2008-08-28
// -----------------------------------------------------------------------------
bool StartExercise::Wait()
{
    // if possible (i.e the session use a logger), wait for the simulation to start ticking
    if( configManipulator_->GetValue< bool >( "session/config/simulation/debug/@networklogger" ) )
    {
        unsigned int nPort = configManipulator_->GetValue< unsigned int >( "session/config/simulation/debug/@networkloggerport" );
        listener_.reset( new ExerciseListener( "localhost", nPort ) );
        return listener_->Wait();
    }
    return true; 
}

// -----------------------------------------------------------------------------
// Name: StartExercise::GetPercentage
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
unsigned int StartExercise::GetPercentage() const
{
    return listener_.get() ? listener_->GetPercentage() : 0;
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
    return ( GetPercentage() == 100 ) ? exercise_ : std::string();
}
