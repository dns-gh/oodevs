// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "ProcessService.h"
#include "commands.h"
#include "Config.h"
#include "ConfigurationManipulator.h"
#include "Process_ABC.h"
#include "clients_kernel/Controllers.h"
#include "protocol/LauncherSenders.h"
#include <qstringlist.h>

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: ProcessService constructor
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
ProcessService::ProcessService( kernel::Controllers& controllers, const Config& config )
    : controllers_( controllers )
    , config_( config )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ProcessService destructor
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
ProcessService::~ProcessService()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ProcessService::NotifyUpdated
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void ProcessService::NotifyUpdated( const boost::shared_ptr< Process_ABC >& process )
{
    const std::string exerciseName = process->GetStartedExercise();
    if( !exerciseName.empty() && exerciseList_.find( exerciseName ) == exerciseList_.end() )
    {
        try
        {
            ConfigurationManipulator manipulator( config_, exerciseName, "multiplayer" );
            exerciseList_[ exerciseName ] = manipulator.GetValue< unsigned int >( "session/config/dispatcher/network/@server" );
            runningProcesses_[ exerciseName ] = process;
        }
        catch( ... )
        {
            // $$$$ SBO 2008-10-30: multiplayer session does not exists
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::NotifyDeleted
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void ProcessService::NotifyDeleted( const boost::shared_ptr< Process_ABC >& process )
{
    const std::string exerciseName = process->GetStartedExercise();
    if( !exerciseName.empty() )
    {
        exerciseList_.erase( exerciseName );
        runningProcesses_.erase( exerciseName );
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendExerciseList
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void ProcessService::SendExerciseList( MsgsLauncherToAdmin::MsgExercicesListResponse& message )
{
    const QStringList exercises = commands::ListExercises( config_ );
    for( QStringList::const_iterator it = exercises.begin(); it != exercises.end(); ++it )
    {
        const std::string name = (*it).ascii();
        Common::MsgExercise& exercise = *message.mutable_exercise()->Add();
        exercise.set_name( name );
        exercise.set_running( runningProcesses_.find( name ) != runningProcesses_.end() );
        if( exercise.running() )
            exercise.set_port( exerciseList_[ name ] );
    }
}
