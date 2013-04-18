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
#include "frontend/ProcessWrapper.h"
#include "frontend/ProfileVisitor_ABC.h"
#include "frontend/StartExercise.h"
#include "frontend/StartDispatcher.h"
#include "frontend/StartReplay.h"
#include "client_proxy/SwordMessageHandler_ABC.h"
#include "SwordFacade.h"
#include "Config.h"
#include "CheckpointMessageHandler.h"
#include "PauseResumeMessageHandler.h"
#include "LauncherService.h"
#include "TimeMessageHandler.h"
#include "NotificationMessageHandler.h"
#include "ControlInformationMessageHandler.h"
#include "ControlEndTickMessageHandler.h"
#include "ConnectedProfilesMessageHandler.h"
#include "SessionStatusMessageHandler.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ClientSenders.h"
#include <boost/foreach.hpp>
#include <boost/assign/list_of.hpp>

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
        boost::recursive_mutex::scoped_lock locker( mutex_ );
        for( ProcessContainer::iterator it = processes_.begin(); it != processes_.end(); )
        {
            if( it->second->IsRunning() )
            {
                it->second->Stop();
                break;
            }
            it = processes_.erase( it );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendExerciseList
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void ProcessService::SendExerciseList( sword::ExerciseListResponse& message )
{
    const tools::Path::T_Paths exercises = frontend::commands::ListExercises( config_ );
    for( auto it = exercises.begin(); it != exercises.end(); ++it )
        message.add_exercise( it->ToUTF8() );
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendSessionList
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void ProcessService::SendSessionList( sword::SessionListResponse& message )
{
    for( ProcessContainer::iterator it = processes_.begin(); it != processes_.end(); ++it )
    {
        const std::pair< tools::Path, tools::Path >& key = it->first;
        if( key.first.ToUTF8() == message.exercise() )
            message.add_session( key.second.ToUTF8() );
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendRunningExercices
// Created: SLI 2011-07-22
// -----------------------------------------------------------------------------
void ProcessService::SendRunningExercices( const std::string& endpoint ) const
{
    for( ProcessContainer::const_iterator it = processes_.begin(); it != processes_.end(); ++it )
    {
        const std::pair< tools::Path, tools::Path >& key = it->first;
        if( IsRunning( key.first, key.second ) )
        {
            SessionStatus message;
            message().set_exercise( key.first.ToUTF8() );
            message().set_session( key.second.ToUTF8() );
            message().set_status( sword::SessionStatus::running );
            message.Send( server_.ResolveClient( endpoint ) );
        }
    }
}

namespace
{
    struct SupervisorProfileCollector : public frontend::ProfileVisitor_ABC
    {
        SupervisorProfileCollector()
            : found_( false )
            , supervisorProfile_( "" )
            , supervisorPassword_( "" )
            {}
        void Visit( const frontend::Profile& profile )
        {
            if( !found_ && profile.IsSupervision() )
            {
                supervisorProfile_ = profile.GetLogin().toStdString();
                supervisorPassword_ = profile.GetPassword().toStdString();
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
    const tools::Path session = tools::Path::FromUTF8( message.session() );
    const tools::Path exercise = tools::Path::FromUTF8( message.exercise() );
    const tools::Path checkpoint = tools::Path::FromUTF8( message.has_checkpoint() ? message.checkpoint() : "" );

    if( ! frontend::commands::ExerciseExists( config_, exercise ) )
        return sword::SessionStartResponse::invalid_exercise_name;
    if( IsRunning( exercise, session ) )
        return sword::SessionStartResponse::session_already_running;
    if( message.has_checkpoint() && ! frontend::commands::CheckpointExists( config_, exercise, session, checkpoint ) )
        return sword::SessionStartResponse::invalid_checkpoint;
    {
        frontend::CreateSession action( config_, exercise, session );
        action.SetDefaultValues();
        for( int i = 0; i < message.parameter().size() ; ++i )
        {
            const sword::SessionParameter& parameter = message.parameter( i );
            action.SetOption( parameter.key(), parameter.value() );
        }
        action.Commit();
    }
    boost::shared_ptr< frontend::SpawnCommand > command;
    if( message.type() == sword::SessionStartRequest::simulation )
    {
        std::map< std::string, std::string > arguments = boost::assign::map_list_of( "legacy", "true" )
                                                                                   ( "checkpoint", checkpoint.ToUTF8().c_str() );
        command.reset( new frontend::StartExercise( config_, exercise, session, arguments, true, false, endpoint, "launcher-job" ) );
    }
    else if( message.type() == sword::SessionStartRequest::dispatch )
        command.reset( new frontend::StartDispatcher( config_, true, exercise, session, checkpoint, "", endpoint, "launcher-job" ) );
    else
        command.reset( new frontend::StartReplay( config_, exercise, session, 10001, true, endpoint, "launcher-job" ) );

    SupervisorProfileCollector profileCollector;
    frontend::Profile::VisitProfiles( config_, fileLoader_, exercise, profileCollector );

    boost::shared_ptr< SwordFacade > wrapper( new SwordFacade( message.type() == sword::SessionStartRequest::dispatch ) );
    {
        boost::recursive_mutex::scoped_lock locker( mutex_ );
        processes_[ std::make_pair( exercise, session ) ] = wrapper;
    }
    wrapper->Start( *this, command, profileCollector.supervisorProfile_, profileCollector.supervisorPassword_, config_ );
    wrapper->AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC >( new CheckpointMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
    wrapper->AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC >( new NotificationMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
    wrapper->AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC >( new ControlInformationMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
    wrapper->AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC >( new ControlEndTickMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
    wrapper->AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC >( new SessionStatusMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
    return sword::SessionStartResponse::success;
}

// -----------------------------------------------------------------------------
// Name: ProcessService::StopExercise
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
sword::SessionStopResponse::ErrorCode ProcessService::StopSession( const sword::SessionStopRequest& message )
{
    auto it = processes_.find( std::make_pair( tools::Path::FromUTF8( message.exercise() ), tools::Path::FromUTF8( message.session() ) ) );
    if( it != processes_.end() )
    {
        boost::shared_ptr< SwordFacade > process( it->second );
        process->Stop();
        processes_.erase( it );
        return sword::SessionStopResponse::success;
    }
    return frontend::commands::ExerciseExists( config_, tools::Path::FromUTF8( message.exercise() ) ) ? sword::SessionStopResponse::session_not_running
                                                                                                      : sword::SessionStopResponse::invalid_exercise_name;
}

// -----------------------------------------------------------------------------
// Name: ProcessService::IsRunning
// Created: SBO 2010-10-07
// -----------------------------------------------------------------------------
bool ProcessService::IsRunning( const tools::Path& exercise, const tools::Path& session ) const
{
    auto it = processes_.find( std::make_pair( exercise, session ) );
    return it != processes_.end() && ( config_.GetTestMode() || it->second->IsRunning() );
}

// -----------------------------------------------------------------------------
// Name: ProcessService::NotifyStopped
// Created: SBO 2010-11-04
// -----------------------------------------------------------------------------
void ProcessService::NotifyStopped()
{
    boost::recursive_mutex::scoped_lock locker( mutex_ );
    for( ProcessContainer::iterator it = processes_.begin(); it != processes_.end(); )
        if( !it->second->IsRunning() )
            it = processes_.erase( it );
        else
            ++it;
}

namespace
{
    void NotifyError( const std::string& endpoint,
                      const LauncherService& server,
                      ProcessService::ProcessContainer& processes )
    {
        if( endpoint.empty() )
            return;
        for( auto cur = processes.begin(); cur != processes.end(); ++cur )
        {
            const auto spawns = cur->second->GetProcess()->GetSpawns();
            for( auto it = spawns.begin(); it != spawns.end(); ++it )
            {
                auto cmd = *it;
                if( cmd->GetCommanderEndpoint() != endpoint )
                    continue;
                SessionStatus msg;
                msg().set_status( sword::SessionStatus::not_running );
                msg().set_exercise( cmd->GetExercise().ToUTF8() );
                msg().set_session( cmd->GetSession().ToUTF8() );
                msg.Send( server.ResolveClient( endpoint ) );
                processes.erase( cur );
                return;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::NotifyError
// Created: SBO 2010-12-09
// -----------------------------------------------------------------------------
void ProcessService::NotifyError( const std::string& /*error*/, const std::string& endpoint )
{
    ::NotifyError( endpoint, server_, processes_ );
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
        if( it->first.first.ToUTF8() == message.exercise() )
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
void ProcessService::SendCheckpointList( sword::CheckpointListResponse& message, const tools::Path& exercice, const tools::Path& session )
{
    if( !frontend::commands::ExerciseExists( config_, exercice ) )
        message.set_error_code( sword::CheckpointListResponse::invalid_exercise_name );
    else if( ! frontend::commands::SessionExists( config_, exercice, session ) )
        message.set_error_code( sword::CheckpointListResponse::invalid_session_name );
    else
    {
        message.set_error_code( sword::CheckpointListResponse::success );
        const tools::Path::T_Paths checkpoints = frontend::commands::ListCheckpoints( config_, exercice, session );
        for( auto it = checkpoints.begin(); it != checkpoints.end(); ++it )
            message.add_checkpoint( it->ToUTF8() );
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::RemoveCheckpoint
// Created: LGY 2011-05-19
// -----------------------------------------------------------------------------
void ProcessService::RemoveCheckpoint( sword::CheckpointDeleteResponse& message, const tools::Path::T_Paths& checkpoints,
                                       const tools::Path& exercice, const tools::Path& session )
{
    if( !frontend::commands::ExerciseExists( config_, exercice ) )
        message.set_error_code( sword::CheckpointDeleteResponse::invalid_exercise_name );
    else if( ! frontend::commands::SessionExists( config_, exercice, session ) )
        message.set_error_code( sword::CheckpointDeleteResponse::invalid_session_name );
    else
    {
        message.set_error_code( sword::CheckpointDeleteResponse::success );
        const tools::Path::T_Paths result = frontend::commands::RemoveCheckpoint( config_, exercice, session, checkpoints );
        BOOST_FOREACH( const tools::Path& name, result )
            message.add_checkpoint( name.ToUTF8() );
    }
}

namespace
{
    template< typename T, typename U >
    void SendErrorMessage( LauncherPublisher& publisher, const tools::Path& exercise, const tools::Path& session, U error )
    {
        T message;
        message().set_exercise( exercise.ToUTF8() );
        message().set_session( session.ToUTF8() );
        message().set_error_code( error );
        message.Send( publisher );
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::ExecuteCommand
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void ProcessService::ExecuteCommand( const std::string& endpoint, const sword::SessionCommandExecutionRequest& message )
{
    const tools::Path exercise = tools::Path::FromUTF8( message.exercise() );
    const tools::Path session = tools::Path::FromUTF8( message.session() );
    ProcessContainer::const_iterator it = processes_.find( std::make_pair( exercise, session ) );
    static int context = 1;
    if( processes_.end() == it )
        return SendErrorMessage< SessionCommandExecutionResponse >( server_.ResolveClient( endpoint ), exercise, session, sword::SessionCommandExecutionResponse::invalid_session_name );
    if( !it->second->IsConnected() )
        return SendErrorMessage< SessionCommandExecutionResponse >( server_.ResolveClient( endpoint ), exercise, session, sword::SessionCommandExecutionResponse::session_not_running );
    boost::shared_ptr< SwordFacade > client( it->second );
    if( message.has_set_running() )
    {
        ExecutePauseResume( endpoint, exercise, session, message.set_running(), context, *client );
        ++context;
    }
    if( message.has_save_checkpoint() )
        SaveCheckpoint( tools::Path::FromUTF8( message.save_checkpoint() ), *client );
    if( message.has_time_change() )
    {
        ExecuteChangeTime( endpoint, exercise, session, message.time_change().data(), context, *client );
        ++context;
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::ExecutePauseResume
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
void ProcessService::ExecutePauseResume( const std::string& endpoint, const tools::Path& exercise, const tools::Path& session,
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
// Name: ProcessService::ExecuteChangeTime
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
void ProcessService::ExecuteChangeTime( const std::string& endpoint, const tools::Path& exercise, const tools::Path& session,
                                        const std::string& date, int context, SwordFacade& facade )
{
    facade.RegisterMessageHandler( context,
        std::auto_ptr< MessageHandler_ABC >( new TimeMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
    simulation::ControlDateTimeChange request;
    request().mutable_date_time()->set_data( date );
    request.Send( facade, context );
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SaveCheckpoint
// Created: LGY 2011-05-18
// -----------------------------------------------------------------------------
void ProcessService::SaveCheckpoint( const tools::Path& name, SwordFacade& facade )
{
    simulation::ControlCheckPointSaveNow request;
    request().set_name( name.ToUTF8() );
    request.Send( facade );
}

// -----------------------------------------------------------------------------
// Name: ProcessService::Update
// Created: AHC 2010-05-19
// -----------------------------------------------------------------------------
void ProcessService::Update()
{
    for( ProcessContainer::iterator it = processes_.begin(); processes_.end() != it; ++it )
        it->second->Update();
}

// -----------------------------------------------------------------------------
// Name: ProcessService::ChangeParameter
// Created: AHC 2011-05-20
// -----------------------------------------------------------------------------
void ProcessService::ChangeParameter( const std::string& endpoint, const sword::SessionParameterChangeRequest& message )
{
    const tools::Path exercise = tools::Path::FromUTF8( message.exercise() );
    const tools::Path session = tools::Path::FromUTF8( message.session() );
    auto it = processes_.find( std::make_pair( exercise, session ) );
    static int context = 1;
    if( processes_.end() == it )
        return SendErrorMessage< SessionParameterChangeResponse >( server_.ResolveClient( endpoint ), exercise, session, sword::SessionParameterChangeResponse::invalid_session_name );
    if( !it->second->IsConnected() )
        return SendErrorMessage< SessionParameterChangeResponse >( server_.ResolveClient( endpoint ), exercise, session, sword::SessionParameterChangeResponse::session_not_running );

    boost::shared_ptr< SwordFacade > client( it->second );
    if( message.has_acceleration_factor() )
    {
        simulation::ControlChangeTimeFactor request;
        request().set_time_factor( message.acceleration_factor() );
        request.Send( *client, 0 );
    }
    if( message.has_checkpoint_frequency() )
    {
        simulation::ControlCheckPointSetFrequency request;
        request().set_frequency( message.checkpoint_frequency() );
        request.Send( *client, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendConnectedProfiles
// Created: AHC 2011-05-20
// -----------------------------------------------------------------------------
void ProcessService::SendConnectedProfiles( const std::string& endpoint, const sword::ConnectedProfileListRequest& message )
{
    const tools::Path exercise = tools::Path::FromUTF8( message.exercise() );
    const tools::Path session = tools::Path::FromUTF8( message.session() );
    auto it = processes_.find( std::make_pair( exercise, session ) );
    static int context = 1;
    if( processes_.end() == it )
        return SendErrorMessage< ConnectedProfileListResponse >( server_.ResolveClient( endpoint ), exercise, session, sword::ConnectedProfileListResponse::invalid_session_name );
    if( !it->second->IsConnected() )
        return SendErrorMessage< ConnectedProfileListResponse >( server_.ResolveClient( endpoint ), exercise, session, sword::ConnectedProfileListResponse::session_not_running );
    boost::shared_ptr< SwordFacade > client( it->second );
    client->RegisterMessageHandler( context, std::auto_ptr< MessageHandler_ABC >( new ConnectedProfilesMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
    authentication::ConnectedProfilesRequest request;
    request.Send( *client, context );
    ++context;
}
// -----------------------------------------------------------------------------
// Name: ProcessService::SendSessionsStatuses
// Created: RPD 2011-09-12
// -----------------------------------------------------------------------------
void ProcessService::SendSessionsStatuses( const std::string& endpoint )
{
    if( endpoint.empty() )
        return;
    LauncherPublisher& publisher = server_.ResolveClient( endpoint );
    for( ProcessContainer::iterator cur = processes_.begin(); cur != processes_.end(); ++cur )
    {
        const auto spawns = cur->second->GetProcess()->GetSpawns();
        for( auto it = spawns.begin(); it != spawns.end(); ++it )
        {
            auto cmd = *it;
            if( cmd->GetCommanderEndpoint() != endpoint )
                continue;
            SessionStatus msg;
            const tools::Path exercise = cmd->GetExercise();
            const tools::Path session = cmd->GetSession();
            msg().set_status( IsRunning( exercise, session )? sword::SessionStatus::running : sword::SessionStatus::not_running );
            msg().set_exercise( exercise.ToUTF8() );
            msg().set_session( session.ToUTF8() );
            msg.Send( publisher );
        }
    }
}
