// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ExerciseService.h"
#include "Multiplayer.h"
#include "clients_kernel/Controllers.h"
#include "frontend/ConfigurationManipulator.h"
#include "frontend/Process_ABC.h"
#include "tools/GeneralConfig.h"

// -----------------------------------------------------------------------------
// Name: ExerciseService constructor
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
ExerciseService::ExerciseService( kernel::Controllers& controllers, tools::GeneralConfig& config )
    : controllers_( controllers )
    , config_     ( config )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ExerciseService destructor
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
ExerciseService::~ExerciseService()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ExerciseService::NotifyCreated
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
void ExerciseService::NotifyCreated( const frontend::Process_ABC& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ExerciseService::NotifyUpdated
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
void ExerciseService::NotifyUpdated( const frontend::Process_ABC& process )
{
    std::string exerciseName = process.GetStartedExercise();
    if( !exerciseName.empty() && exerciseList_.find( exerciseName ) == exerciseList_.end() )
    {
        frontend::ConfigurationManipulator manipulator( config_, exerciseName, MULTIPLAYER_SESSION );
        exerciseList_[ exerciseName ] = manipulator.GetValue< unsigned int >( "session/config/dispatcher/network/@server" );
    }
}
    
// -----------------------------------------------------------------------------
// Name: ExerciseService::NotifyDeleted
// Created: LDC 2008-10-23
// -----------------------------------------------------------------------------
void ExerciseService::NotifyDeleted( const frontend::Process_ABC& process )
{
    std::string exerciseName = process.GetStartedExercise();
    if( !exerciseName.empty() )
        exerciseList_.erase( exerciseName );
}
