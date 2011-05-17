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
#include "LauncherService.h"
#include "LauncherPublisher.h"
#include "protocol/SimulationSenders.h"
#include "protocol/ClientSenders.h"
namespace launcher
{

    struct ClientMessageHandlerBase : public SwordMessageHandler_ABC, boost::noncopyable
    {
    protected:
        //! @name Constructors/Destructor
        //@{
        ClientMessageHandlerBase(LauncherPublisher& publisher, SwordFacade& client,
                const std::string& exercise, const std::string& session)
            : publisher_(publisher)
            , client_(client)
            , exercise_(exercise)
            , session_(session)
        {}
        //@}
    public:
        //! @name Operations
        //@{
        virtual void OnReceiveMessage( const sword::SimToClient& /*message*/ ) {}
        virtual void OnReceiveMessage( const sword::MessengerToClient& /*message*/ ) {}
        //@}

    protected:
        template <typename T> void Send(T& message)
        {
            message.Send( publisher_ );
        }
        const std::string& exercise() const { return exercise_; }
        const std::string& session() const { return session_; }

    private:
        LauncherPublisher& publisher_;
        SwordFacade& client_;
        std::string exercise_;
        std::string session_;
    };
    struct PauseResumeMessageHandler : public ClientMessageHandlerBase
    {
        //! @name Constructors/Destructor
        //@{
        PauseResumeMessageHandler(LauncherPublisher& publisher, SwordFacade& client,
                const std::string& exercise, const std::string& session)
            : ClientMessageHandlerBase(publisher, client, exercise, session)
        {
        }
        //@}

        virtual void OnReceiveMessage( const sword::SimToClient& message )
        {
            if( message.message().has_control_pause_ack() )
            {
                SessionCommandExecutionResponse response;
                response().set_exercise( exercise() );
                response().set_session( session() );
                response().set_error_code( message.message().control_pause_ack().error_code() == sword::ControlAck::no_error ?
                        sword::SessionCommandExecutionResponse::success :
                        sword::SessionCommandExecutionResponse::session_already_paused);
                response().set_running( false );
                Send( response );
            }
            if( message.message().has_control_resume_ack() )
            {
                SessionCommandExecutionResponse response;
                response().set_exercise( exercise() );
                response().set_session( session() );
                response().set_error_code( message.message().control_pause_ack().error_code() == sword::ControlAck::no_error ?
                        sword::SessionCommandExecutionResponse::success :
                        sword::SessionCommandExecutionResponse::session_already_running);
                response().set_running( false );
                Send( response );
            }
        }
    };
}
using namespace launcher;

// -----------------------------------------------------------------------------
// Name: ProcessService constructor
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
ProcessService::ProcessService( const tools::GeneralConfig& config, const tools::Loader_ABC& fileLoader, const LauncherService& server )
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
    {
        const std::string name = (*it).ascii();
        message.add_exercise(name);
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendSessionList
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void ProcessService::SendSessionList( sword::SessionListResponse& message )
{
    const QStringList exercises = frontend::commands::ListExercises( config_ );
    for( ProcessContainer::iterator it = processes_.begin(); it != processes_.end(); ++it )
    {
        const std::pair<std::string, std::string>& key = it->first;
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
                //supervisorPassword_ = profile.GetPassword().ascii();
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
sword::SessionStartResponse::ErrorCode ProcessService::StartSession( const sword::SessionStartRequest& message )
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
    wrapper->Start(profileCollector.supervisorProfile_, profileCollector.supervisorPassword_);
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
    for( QStringList::const_iterator it = checkpoints.begin(); it != checkpoints.end(); ++it )
        message.add_checkpoint( ( *it ).ascii() );
}

// -----------------------------------------------------------------------------
// Name: ProcessService::ExecuteCommand
// Created: AHC 2011-05-16
// -----------------------------------------------------------------------------
void ProcessService::ExecuteCommand( const std::string& endpoint, const sword::SessionCommandExecutionRequest& message )
{
    ProcessContainer::const_iterator it = processes_.find( std::make_pair(message.exercise(), message.session()) );
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
    boost::shared_ptr<SwordFacade> client( it->second );
    client->RegisterMessageHandler( ++context,
            std::auto_ptr<SwordMessageHandler_ABC>(new PauseResumeMessageHandler(server_.ResolveClient( endpoint ), *client.get(), message.exercise(), message.session() ) ) );
    if( message.set_running() )
    {
        simulation::ControlResume request;
        request.Send( *client );
    }
    else
    {
        simulation::ControlPause request;
        request.Send( *client );
    }
    if( message.has_save_checkpoint() )
    {

        simulation::ControlCheckPointSaveNow request;
        request().set_name( message.save_checkpoint() );
        request.Send( *client );
    }
}
