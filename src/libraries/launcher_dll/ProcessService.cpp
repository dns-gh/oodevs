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
void ProcessService::SendExerciseList( sword::ExerciseListResponse& message )
{
    const QStringList exercises = frontend::commands::ListExercises( config_ );
    for( QStringList::const_iterator it = exercises.begin(); it != exercises.end(); ++it )
    {
        const std::string name = (*it).ascii();
        message.add_exercise(name);;
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::StartExercise
// Created: SBO 2010-10-07
// -----------------------------------------------------------------------------
sword::SessionStartResponse::ErrorCode ProcessService::StartSession( const sword::SessionStartRequest& message )
{
    const std::string session = "multiplayer"; // $$$$ SBO 2010-10-28: TODO: add session to message
    const std::string exercise = message.exercise();
    const std::string checkpoint = message.has_checkpoint() ? message.checkpoint() : "";
    if( ! frontend::commands::ExerciseExists( config_, exercise ) )
        return sword::SessionStartResponse::invalid_exercise_name;
    if( IsRunning( exercise ) )
        return sword::SessionStartResponse::session_already_running;
    if( message.has_checkpoint() && ! frontend::commands::CheckpointExists( config_, exercise, session, checkpoint ) )
        return sword::SessionStartResponse::invalid_checkpoint;
    {
        frontend::CreateSession action( config_, exercise, session );
        action.SetDefaultValues();
        for(int i=0; i<message.parameter().size() ; ++i)
        {
            const sword::SessionParameter& parameter = message.parameter(i);
            action.SetOption( parameter.key(), parameter.value() );
        }
//        if( message.has_use_after_action_analysis() && message.use_after_action_analysis() )
//            action.SetOption( "session/config/dispatcher/plugins/recorder", "" );
//        else
//            action.RemoveOption( "session/config/dispatcher/plugins/recorder" );
        action.Commit();
    }
    boost::shared_ptr< frontend::SpawnCommand > command;
    if( message.type() == sword::SessionStartRequest::simulation )
        command.reset( new frontend::StartExercise( config_, exercise.c_str(), session.c_str(), checkpoint.c_str(), true ) );
    else
        command.reset( new frontend::StartReplay( config_, exercise.c_str(), session.c_str(), 10001, true ) );
    boost::shared_ptr< frontend::ProcessWrapper > wrapper( new frontend::ProcessWrapper( *this, command ) );
    {
        boost::recursive_mutex::scoped_lock locker( mutex_ );
        processes_[ exercise ] = wrapper;
    }
    wrapper->Start();
    return sword::SessionStartResponse::success;
}

// -----------------------------------------------------------------------------
// Name: ProcessService::StopExercise
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
sword::SessionStopResponse::ErrorCode ProcessService::StopSession( const sword::SessionStopRequest& message )
{
    const std::string name = message.exercise();
    std::map< std::string, boost::weak_ptr< frontend::ProcessWrapper > >::iterator it = processes_.find( name );
    if( it != processes_.end() && ! it->second.expired() )
    {
        boost::shared_ptr< frontend::ProcessWrapper > process( it->second );
        process->Stop();
        return sword::SessionStopResponse::success;
    }
    return frontend::commands::ExerciseExists( config_, name ) ? sword::SessionStopResponse::session_not_running
                                                               : sword::SessionStopResponse::invalid_exercise_name;
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
        explicit ProfileCollector( sword::ProfileListResponse& message )
            : message_( message )
        {}
        virtual void Visit( const frontend::Profile& profile )
        {
            profile.Send( *message_.add_profile() );
        }
        sword::ProfileListResponse& message_;
    };
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendProfileList
// @brief fills 'message' with profiles from first running exercise...
//        exercise name should be added to profile list request
// Created: SBO 2010-11-19
// -----------------------------------------------------------------------------
void ProcessService::SendProfileList( sword::ProfileListResponse& message )
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
