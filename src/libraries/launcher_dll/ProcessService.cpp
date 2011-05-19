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
#include "frontend/Profile.h"
#include "frontend/ProfileVisitor_ABC.h"
#include "frontend/StartExercise.h"
#include "frontend/StartDispatcher.h"
#include "frontend/StartReplay.h"
#include "client_proxy/SwordMessageHandler_ABC.h"
#include "SwordFacade.h"
#include "Config.h"
#include "CheckpointMessageHandler.h"
#include "PauseResumeMessageHandler.h"
#include "StatusMessageHandler.h"
#include "LauncherService.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ClientSenders.h"
#include <boost/foreach.hpp>

using namespace launcher;

// -----------------------------------------------------------------------------
// Name: ProcessService constructor
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
ProcessService::ProcessService( const launcher::Config& config, const tools::Loader_ABC& fileLoader, const LauncherService& server )
    : config_    ( config )
    , fileLoader_( fileLoader )
    , server_    ( server )
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
        boost::shared_ptr< SwordFacade > process;
        boost::recursive_mutex::scoped_lock locker( mutex_ );
        for( ProcessContainer::iterator it = processes_.begin(); it != processes_.end(); ++it )
            if( ! it->second.expired() )
            {
                boost::shared_ptr< SwordFacade > weak( it->second );
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
        message.add_exercise( (*it).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendSessionList
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void ProcessService::SendSessionList( sword::SessionListResponse& message )
{
    for( ProcessContainer::iterator it = processes_.begin(); it != processes_.end(); ++it )
    {
        const std::pair< std::string, std::string >& key = it->first;
        if( key.first == message.exercise() )
            message.add_session( key.second );
    }
}

namespace
{
    struct SupervisorProfileCollector : public frontend::ProfileVisitor_ABC
    {
        SupervisorProfileCollector()
            : found_(false)
            , supervisorProfile_("")
            , supervisorPassword_("")
            {}
        void Visit( const frontend::Profile& profile )
        {
            if(!found_ && profile.IsSupervision() )
            {
                supervisorProfile_ = profile.GetLogin().ascii();
                supervisorPassword_ = profile.GetPassword().ascii();
                found_ = true;
            }
        }
        std::string supervisorProfile_;
        std::string supervisorPassword_;
        bool found_;
    };
}
// -----------------------------------------------------------------------------
// Name: ProcessService::StartExercise
// Created: SBO 2010-10-07
// -----------------------------------------------------------------------------
sword::SessionStartResponse::ErrorCode ProcessService::StartSession( const std::string& endpoint, const sword::SessionStartRequest& message )
{
    const std::string session = message.session();
    const std::string exercise = message.exercise();
    const std::string checkpoint = message.has_checkpoint() ? message.checkpoint() : "";

    if( ! frontend::commands::ExerciseExists( config_, exercise ) )
        return sword::SessionStartResponse::invalid_exercise_name;
    if( IsRunning( exercise, session ) )
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
        action.Commit();
    }
    boost::shared_ptr< frontend::SpawnCommand > command;
    if( message.type() == sword::SessionStartRequest::simulation )
        command.reset( new frontend::StartExercise( config_, exercise.c_str(), session.c_str(), checkpoint.c_str(), true ) );
    else if( message.type() == sword::SessionStartRequest::dispatch )
    {
        command.reset( new frontend::StartDispatcher( config_, true, exercise.c_str(), session.c_str(), checkpoint.c_str() ) );
    }
    else
        command.reset( new frontend::StartReplay( config_, exercise.c_str(), session.c_str(), 10001, true ) );

    SupervisorProfileCollector profileCollector;
    frontend::Profile::VisitProfiles( config_, fileLoader_, exercise, profileCollector );

    boost::shared_ptr< SwordFacade > wrapper( new SwordFacade( *this, command, message.type() == sword::SessionStartRequest::dispatch ) );
    {
        boost::recursive_mutex::scoped_lock locker( mutex_ );
        processes_[ std::make_pair(exercise, session) ] = wrapper;
    }
    wrapper->Start(profileCollector.supervisorProfile_, profileCollector.supervisorPassword_, config_.GetTestMode() );
    wrapper->AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC >( new CheckpointMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
    wrapper->AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC >( new StatusMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
    return sword::SessionStartResponse::success;
}

// -----------------------------------------------------------------------------
// Name: ProcessService::StopExercise
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
sword::SessionStopResponse::ErrorCode ProcessService::StopSession( const sword::SessionStopRequest& message )
{
    const std::string name = message.exercise();
    ProcessContainer::iterator it = processes_.find( std::make_pair(message.exercise(), message.session() ) );
    if( it != processes_.end() && ! it->second.expired() )
    {
        boost::shared_ptr< SwordFacade > process( it->second );
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
bool ProcessService::IsRunning( const std::string& exercise, const std::string& session ) const
{
    ProcessContainer::const_iterator it = processes_.find( std::make_pair(exercise, session) );
    return it != processes_.end() && ! it->second.expired();
}

// -----------------------------------------------------------------------------
// Name: ProcessService::NotifyStopped
// Created: SBO 2010-11-04
// -----------------------------------------------------------------------------
void ProcessService::NotifyStopped()
{
    boost::recursive_mutex::scoped_lock locker( mutex_ );
    for( ProcessContainer::iterator it = processes_.begin(); it != processes_.end(); )
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
    for( ProcessContainer::const_iterator it = processes_.begin(); it != processes_.end(); ++it )
        if( !it->second.expired() )
        {
            ProfileCollector collector( message );
            frontend::Profile::VisitProfiles( config_, fileLoader_, it->first.first, collector );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendCheckpointList
// Created: LGY 2011-05-17
// -----------------------------------------------------------------------------
void ProcessService::SendCheckpointList( sword::CheckpointListResponse& message, const std::string& exercice, const std::string& session )
{
    const QStringList checkpoints = frontend::commands::ListCheckpoints( config_, exercice, session );
    if( ! frontend::commands::ExerciseExists( config_, exercice ) )
        message.set_error_code( sword::CheckpointListResponse::invalid_exercise_name );
    else if( ! frontend::commands::SessionExists( config_, exercice, session ) )
        message.set_error_code( sword::CheckpointListResponse::invalid_session_name );
    else
        for( QStringList::const_iterator it = checkpoints.begin(); it != checkpoints.end(); ++it )
            message.add_checkpoint( ( *it ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ProcessService::RemoveCheckpoint
// Created: LGY 2011-05-19
// -----------------------------------------------------------------------------
void ProcessService::RemoveCheckpoint( sword::CheckpointDeleteResponse& message, const boost::optional< std::string >& checkpoint,
                                       const std::string& exercice, const std::string& session )
{
    if( ! frontend::commands::ExerciseExists( config_, exercice ) )
        message.set_error_code( sword::CheckpointDeleteResponse::invalid_exercise_name );
    else if( ! frontend::commands::SessionExists( config_, exercice, session ) )
        message.set_error_code( sword::CheckpointDeleteResponse::invalid_session_name );
    else
    {
        const std::vector< std::string > result = frontend::commands::RemoveCheckpoint( config_, exercice, session, checkpoint );
        BOOST_FOREACH( const std::string& name, result )
            message.add_checkpoint( name );
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::ExecuteCommand
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void ProcessService::ExecuteCommand( const std::string& endpoint, const sword::SessionCommandExecutionRequest& message )
{
    ProcessContainer::const_iterator it = processes_.find( std::make_pair( message.exercise(), message.session() ) );
    static int context = 1;

    if( processes_.end() == it )
    {
        SessionCommandExecutionResponse response;
        response().set_exercise( message.exercise() );
        response().set_session( message.session() );
        response().set_error_code( sword::SessionCommandExecutionResponse::invalid_session_name );
        response.Send( server_.ResolveClient( endpoint ) );
        return;
    }
    if( it->second.expired() || !it->second.lock()->IsConnected() )
    {
        SessionCommandExecutionResponse response;
        response().set_exercise( message.exercise() );
        response().set_session( message.session() );
        response().set_error_code( sword::SessionCommandExecutionResponse::session_not_running );
        response.Send( server_.ResolveClient( endpoint ) );
        return;
    }
    boost::shared_ptr< SwordFacade > client( it->second );
    if( message.has_set_running() )
        ExecutePauseResume( endpoint, message.exercise(), message.session(), message.set_running(), context, *client );
    ++context;
    if( message.has_save_checkpoint() )
        SaveCheckpoint( message.save_checkpoint(), *client );
}

// -----------------------------------------------------------------------------
// Name: ProcessService::ExecutePauseResume
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
void ProcessService::ExecutePauseResume( const std::string& endpoint, const std::string& exercise, const std::string& session,
                                         bool running, int context, SwordFacade& facade )
{
    facade.RegisterMessageHandler( context,
        std::auto_ptr< MessageHandler_ABC >( new PauseResumeMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
    if( running )
    {
        simulation::ControlResume request;
        request.Send( facade, context );
    }
    else
    {
        simulation::ControlPause request;
        request.Send( facade, context );
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SaveCheckpoint
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
void ProcessService::SaveCheckpoint( const std::string& name, SwordFacade& facade )
{
    simulation::ControlCheckPointSaveNow request;
    request().set_name( name );
    request.Send( facade );
}
