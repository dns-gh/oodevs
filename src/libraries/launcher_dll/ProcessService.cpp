// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "ProcessService.h"
#include "frontend/commands.h"
#include "frontend/Config.h"
#include "frontend/CreateSession.h"
#include "frontend/ProcessWrapper.h"
#include "frontend/Profile.h"
#include "frontend/ProfileVisitor_ABC.h"
#include "frontend/StartExercise.h"
#include "frontend/StartReplay.h"
#include "tools/Loader_ABC.h"
#include <qstringlist.h>

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: ProcessService constructor
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
ProcessService::ProcessService( const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader )
    : config_( config )
    , fileLoader_( fileLoader )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProcessService destructor
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
ProcessService::~ProcessService()
{
    while( ! processes_.empty() )
    {
        boost::shared_ptr< frontend::ProcessWrapper > process;
        boost::recursive_mutex::scoped_lock locker( mutex_ );
        for( std::map< std::string, boost::weak_ptr< frontend::ProcessWrapper > >::iterator it = processes_.begin(); it != processes_.end(); ++it )
            if( ! it->second.expired() )
            {
                boost::shared_ptr< frontend::ProcessWrapper > weak( it->second );
                process = weak;
                break;
            }
        if( process.get() )
            process->Stop();
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendExerciseList
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void ProcessService::SendExerciseList( sword::ExercicesListResponse& message )
{
    const QStringList exercises = frontend::commands::ListExercises( config_ );
    for( QStringList::const_iterator it = exercises.begin(); it != exercises.end(); ++it )
    {
        const std::string name = (*it).ascii();
        sword::Exercise& exercise = *message.mutable_exercise()->Add();
        exercise.set_name( name );
        exercise.set_running( IsRunning( name ) );
        //if( exercise.running() ) // $$$$ LDC 2011-03-10: Why? If port is not set, replay will be unable to connect to port 0.
        exercise.set_port( 10001 ); // $$$$ SBO 2010-10-26: TODO: handle port configuration/multiple exercises
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::StartExercise
// Created: SBO 2010-10-07
// -----------------------------------------------------------------------------
sword::ControlStartExerciseAck::ErrorCode ProcessService::StartExercise( const sword::ControlStartExercise& message )
{
    const std::string session = "multiplayer"; // $$$$ SBO 2010-10-28: TODO: add session to message
    const std::string exercise = message.exercise().name();
    const std::string checkpoint = message.has_checkpoint() ? message.checkpoint() : "";
    if( ! frontend::commands::ExerciseExists( config_, exercise ) )
        return sword::ControlStartExerciseAck::bad_exercise_name;
    if( IsRunning( exercise ) )
        return sword::ControlStartExerciseAck::exercise_already_running;
    if( message.has_checkpoint() && ! frontend::commands::CheckpointExists( config_, exercise, session, checkpoint ) )
        return sword::ControlStartExerciseAck::invalid_checkpoint;
    {
        frontend::CreateSession action( config_, exercise, session );
        action.SetDefaultValues();
        action.SetOption( "session/config/dispatcher/plugins/shield/@server", "30001" );
        action.SetOption( "session/config/simulation/time/@paused", true );
        action.SetOption( "session/config/simulation/time/@factor", 1 );
        action.SetOption( "session/config/simulation/checkpoint/@frequency", "600s" );
        action.SetOption( "session/config/simulation/checkpoint/@keep", "10" );
        if( message.has_use_after_action_analysis() && message.use_after_action_analysis() )
            action.SetOption( "session/config/dispatcher/plugins/recorder", "" );
        else
            action.RemoveOption( "session/config/dispatcher/plugins/recorder" );
        action.Commit();
    }
    boost::shared_ptr< frontend::SpawnCommand > command;
    if( message.mode() == sword::ControlStartExercise::play )
        command.reset( new frontend::StartExercise( config_, exercise.c_str(), session.c_str(), checkpoint.c_str(), true ) );
    else
        command.reset( new frontend::StartReplay( config_, exercise.c_str(), session.c_str(), 10001, true ) );
    boost::shared_ptr< frontend::ProcessWrapper > wrapper( new frontend::ProcessWrapper( *this, command ) );
    {
        boost::recursive_mutex::scoped_lock locker( mutex_ );
        processes_[ exercise ] = wrapper;
    }
    wrapper->Start();
    return sword::ControlStartExerciseAck::success;
}

// -----------------------------------------------------------------------------
// Name: ProcessService::StopExercise
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
sword::ControlStopExerciseAck::ErrorCode ProcessService::StopExercise( const sword::ControlStopExercise& message )
{
    const std::string name = message.exercise().name();
    std::map< std::string, boost::weak_ptr< frontend::ProcessWrapper > >::iterator it = processes_.find( name );
    if( it != processes_.end() && ! it->second.expired() )
    {
        boost::shared_ptr< frontend::ProcessWrapper > process( it->second );
        process->Stop();
        return sword::ControlStopExerciseAck::success;
    }
    return frontend::commands::ExerciseExists( config_, name ) ? sword::ControlStopExerciseAck::exercise_not_running
                                                               : sword::ControlStopExerciseAck::bad_exercise_name;
}

// -----------------------------------------------------------------------------
// Name: ProcessService::IsRunning
// Created: SBO 2010-10-07
// -----------------------------------------------------------------------------
bool ProcessService::IsRunning( const std::string& exercise ) const
{
    std::map< std::string, boost::weak_ptr< frontend::ProcessWrapper > >::const_iterator it = processes_.find( exercise );
    return it != processes_.end() && ! it->second.expired();
}

// -----------------------------------------------------------------------------
// Name: ProcessService::NotifyStopped
// Created: SBO 2010-11-04
// -----------------------------------------------------------------------------
void ProcessService::NotifyStopped()
{
    boost::recursive_mutex::scoped_lock locker( mutex_ );
    for( std::map< std::string, boost::weak_ptr< frontend::ProcessWrapper > >::iterator it = processes_.begin(); it != processes_.end(); )
        if( it->second.expired() )
            it = processes_.erase( it );
        else
            ++it;
}

// -----------------------------------------------------------------------------
// Name: ProcessService::NotifyError
// Created: SBO 2010-12-09
// -----------------------------------------------------------------------------
void ProcessService::NotifyError( const std::string& /*error*/ )
{
    // $$$$ SBO 2010-12-09: Log error message
    NotifyStopped();
}

namespace
{
    struct ProfileCollector : public frontend::ProfileVisitor_ABC
    {
        explicit ProfileCollector( sword::ProfileDescriptionList& message )
            : message_( message )
        {}
        virtual void Visit( const frontend::Profile& profile )
        {
            profile.Send( *message_.add_elem() );
        }
        sword::ProfileDescriptionList& message_;
    };
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendProfileList
// @brief fills 'message' with profiles from first running exercise...
//        exercise name should be added to profile list request
// Created: SBO 2010-11-19
// -----------------------------------------------------------------------------
void ProcessService::SendProfileList( sword::ProfileDescriptionList& message )
{
    boost::recursive_mutex::scoped_lock locker( mutex_ );
    for( std::map< std::string, boost::weak_ptr< frontend::ProcessWrapper > >::const_iterator it = processes_.begin(); it != processes_.end(); ++it )
        if( !it->second.expired() )
        {
            ProfileCollector collector( message );
            frontend::Profile::VisitProfiles( config_, fileLoader_, it->first, collector );
            return;
        }
}
