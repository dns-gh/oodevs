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

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: StartExercise constructor
// Created: AGE 2007-10-04
// -----------------------------------------------------------------------------
StartExercise::StartExercise( const tools::GeneralConfig& config, const QString& exercise, const QString& session, bool attach )
    : SpawnCommand( config, "simulation_app.exe", attach )
    , exercise_ ( exercise.ascii() ) 
    , session_ ( session.ascii() ) 
{
    AddRootDirArgument();
    AddExerciseArgument( exercise );
    AddSessionArgument( session );
}

// -----------------------------------------------------------------------------
// Name: StartExercise constructor
// Created: AGE 2007-10-05
// -----------------------------------------------------------------------------
StartExercise::StartExercise( const tools::GeneralConfig& config, const QString& exercise, const QString& session, const QString& checkpoint, bool attach )
    : SpawnCommand( config, "simulation_app.exe", attach )
    , exercise_ ( exercise.ascii() ) 
    , session_ ( session.ascii() ) 
{
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
}


// -----------------------------------------------------------------------------
// Name: StartExercise::Start
// Created: RDS 2008-08-28
// -----------------------------------------------------------------------------
bool StartExercise::Wait()
{
    // if possible ( i.e the session use a logger ) , wait for the simulation to start ticking
    ConfigurationManipulator configManipulator( config_, exercise_, session_ ); 
    if ( configManipulator.GetValue<bool>("session/config/simulation/debug/@networklogger") )
    {
        unsigned int nPort = configManipulator.GetValue<unsigned int>( "session/config/simulation/debug/@networkloggerport" ) ; 
        listener_.reset( new ExerciseListener( "localhost",  nPort ) ); 
        return listener_->Wait(); 
    }
    return true; 
}

// -----------------------------------------------------------------------------
// Name: StartExercise::GetPercentage
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
unsigned int StartExercise::GetPercentage()
{
    return ( listener_.get() ? listener_->GetPercentage() : 0 ) ;     
}

