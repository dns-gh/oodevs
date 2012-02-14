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
#include "frontend/AttachCommand.h"
#include "frontend/commands.h"
#include "frontend/Config.h"
#include "frontend/CreateSession.h"
#include "frontend/Profile.h"
#include "frontend/ProcessWrapper.h"
#include "frontend/ProfileVisitor_ABC.h"
#include "frontend/StartExercise.h"
#include "frontend/StartDispatcher.h"
#include "frontend/StartReplay.h"
#include <QtCore/qstringlist.h>
#include "client_proxy/SwordMessageHandler_ABC.h"
#include "SwordFacade.h"
#include "Config.h"
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
#include "protocol/MessengerSenders.h"
#include <boost/foreach.hpp>
#include <boost/filesystem/convenience.hpp>
#include <boost/filesystem/operations.hpp>
#include <windows.h>
#include <tlhelp32.h>
#include <fstream>

using namespace launcher;
namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: ProcessService constructor
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
ProcessService::ProcessService( const launcher::Config& config, const tools::Loader_ABC& fileLoader, const LauncherService& server )
    : config_    ( config )
    , fileLoader_( fileLoader )
    , server_    ( server )
{
    if( !config.GetTestMode() )
        CheckForRunningProcesses();
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
        for( ProcessContainer::iterator it = processes_.begin(); it != processes_.end(); )
        {
            if( it->second->IsRunning() )
            {
                process = it->second;
                break;
            }
            else
                it = processes_.erase( it );
        }
        if( process.get() )
            process->Stop( false );
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::CreatePermanentHandlers
// Created: JSR 2011-12-12
// -----------------------------------------------------------------------------
void ProcessService::CreatePermanentHandlers( const std::string& endpoint )
{
    boost::recursive_mutex::scoped_lock locker( mutex_ );
    for( ProcessContainer::iterator it = processes_.begin(); it != processes_.end(); ++it )
    {
        boost::shared_ptr< SwordFacade >& facade = it->second;
        if( facade.get() )
        {
            facade->SetEndpoint( endpoint );
            const std::string& exercise = it->first.first;
            const std::string& session  = it->first.second;
            facade->ClearPermanentMessageHandler();
            facade->AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC >( new NotificationMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
            facade->AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC >( new ControlInformationMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
            facade->AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC >( new ControlEndTickMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
            facade->AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC >( new SessionStatusMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
        }
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
        message.add_exercise( ( *it ).ascii() );
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

// -----------------------------------------------------------------------------
// Name: ProcessService::SendRunningExercices
// Created: SLI 2011-07-22
// -----------------------------------------------------------------------------
void ProcessService::SendRunningExercices( const std::string& endpoint ) const
{
    for( ProcessContainer::const_iterator it = processes_.begin(); it != processes_.end(); ++it )
    {
        const std::pair< std::string, std::string >& key = it->first;
        if( IsRunning( key.first, key.second ) )
        {
            SessionStatus message;
            message().set_exercise( key.first );
            message().set_session( key.second );
            message().set_status( sword::SessionStatus::running );
            message.Send( *server_.ResolveClient( endpoint ) );
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
                supervisorProfile_ = profile.GetLogin().ascii();
                supervisorPassword_ = profile.GetPassword().ascii();
                found_ = true;
            }
        }
        std::string supervisorProfile_;
        std::string supervisorPassword_;
        bool found_;
    };

    std::string GetSessionTmpFilename()
    {
        char filename[ MAX_PATH ];
        if( GetModuleFileNameA( 0, filename, MAX_PATH ) )
        {
            bfs::path filepath = bfs::path( filename, boost::filesystem::native ).branch_path() / "~~launcher.tmp";
            return( filepath.native_file_string() );
        }
        return std::string();
    }
}

namespace
{
    void CopyDirectory( const bfs::path& from, const bfs::path& to )
    {
        bfs::directory_iterator end;
        for( bfs::directory_iterator it( from ); it != end; ++it )
        {
            if( bfs::is_directory( *it ) )
            {
                bfs::path dest( to / it->leaf() );
                bfs::create_directories( dest );
                CopyDirectory( *it, dest );
            }
            else
                bfs::copy_file( *it, to / it->leaf() );
        }
    }
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
    if( !processes_.empty() ) // un seul process autoris�
        return sword::SessionStartResponse::session_already_running;
    if( message.has_checkpoint() && ! frontend::commands::CheckpointExists( config_, exercise, session, checkpoint ) )
        return sword::SessionStartResponse::invalid_checkpoint;

    if( !message.has_checkpoint() && ( message.type() == sword::SessionStartRequest::simulation || message.type() == sword::SessionStartRequest::dispatch ) )
    {
        const bfs::path dir( config_.BuildSessionDir( exercise, session ), bfs::native );
        try
        {
            if( bfs::exists( dir ) )
            {
                // backup and remove old session
                const bfs::path dirBackup( config_.BuildSessionDir( exercise, "backup" ), bfs::native ) / session;
                if( bfs::exists( dirBackup ) )
                    bfs::remove_all( dirBackup );
                bfs::create_directories( dirBackup );
                CopyDirectory( dir, dirBackup );
                bfs::remove_all( dir );
            }
        }
        catch( ... )
        {
            // NOTHING
        }
    }

    bool isDispatcher = ( message.type() == sword::SessionStartRequest::dispatch );
    {
        frontend::CreateSession action( config_, exercise, session );
        action.SetDefaultValues();
        for( int i = 0; i < message.parameter().size() ; ++i )
        {
            const sword::SessionParameter& parameter = message.parameter( i );
            if( parameter.key() == "session/config/dispatcher/saver/@enable" && parameter.value() == "false" )
                action.RemoveOption( "session/config/dispatcher/plugins/recorder" );
            else
            {
                action.SetOption( parameter.key(), parameter.value() );
                if( message.type() == sword::SessionStartRequest::simulation && parameter.key() == "session/config/simulation/dispatcher/@embedded" && parameter.value() == "true")
                    isDispatcher = true;
            }
        }
        action.Commit();
    }

    boost::shared_ptr< frontend::SpawnCommand > command;
    if( message.type() == sword::SessionStartRequest::simulation )
        command.reset( new frontend::StartExercise( config_, exercise.c_str(), session.c_str(), checkpoint.c_str(), false, false, endpoint, true ) );
    else if( message.type() == sword::SessionStartRequest::dispatch )
        command.reset( new frontend::StartDispatcher( config_, false, exercise.c_str(), session.c_str(), checkpoint.c_str(), "", endpoint, true ) );
    else
        command.reset( new frontend::StartReplay( config_, exercise.c_str(), session.c_str(), 10001, false, endpoint, true ) );

    SupervisorProfileCollector profileCollector;
    frontend::Profile::VisitProfiles( config_, fileLoader_, exercise, profileCollector );

    boost::shared_ptr< SwordFacade > wrapper( new SwordFacade( server_, endpoint, isDispatcher ) );
    {
        boost::recursive_mutex::scoped_lock locker( mutex_ );
        processes_[ std::make_pair( exercise, session ) ] = wrapper;
    }
    wrapper->Start( *this, command, profileCollector.supervisorProfile_, profileCollector.supervisorPassword_, config_ );
    wrapper->AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC >( new NotificationMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
    wrapper->AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC >( new ControlInformationMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
    wrapper->AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC >( new ControlEndTickMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );
    wrapper->AddPermanentMessageHandler( std::auto_ptr< MessageHandler_ABC >( new SessionStatusMessageHandler( server_.ResolveClient( endpoint ), exercise, session ) ) );

    try
    {
        std::ofstream file( GetSessionTmpFilename().c_str() );
        if( file.is_open() )
        {
            file << session.c_str() << "\n"
                 << exercise.c_str() << "\n"
                 << checkpoint.c_str() << "\n"
                 << profileCollector.supervisorProfile_.c_str() << "\n"
                 << profileCollector.supervisorPassword_.c_str() << "\n";
            file.close();
        }
    }
    catch( ... )
    {
    }
    return sword::SessionStartResponse::success;
}

// -----------------------------------------------------------------------------
// Name: ProcessService::StopExercise
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
sword::SessionStopResponse::ErrorCode ProcessService::StopSession( const sword::SessionStopRequest& message )
{
    const std::string name = message.exercise();
    ProcessContainer::iterator it = processes_.find( std::make_pair( message.exercise(), message.session() ) );
    if( it != processes_.end() )
    {
        boost::shared_ptr< SwordFacade > process( it->second );
        process->Stop();
        processes_.erase( it );
        std::remove( GetSessionTmpFilename().c_str() );
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
    ProcessContainer::const_iterator it = processes_.find( std::make_pair( exercise, session ) );
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

// -----------------------------------------------------------------------------
// Name: ProcessService::NotifyError
// Created: SBO 2010-12-09
// -----------------------------------------------------------------------------
void ProcessService::NotifyError( const std::string& error, std::string commanderEndpoint /*= ""*/ )
{
    if ( !commanderEndpoint.empty() )
{
        LauncherPublisher& publisher = *server_.ResolveClient( commanderEndpoint );
        for( ProcessContainer::iterator it = processes_.begin(); it != processes_.end(); ++it )
        {
            const frontend::SpawnCommand* command = it->second->GetProcess()->GetCommand();
            if( command && command->GetCommanderEndpoint() == commanderEndpoint )
            {
                SessionStatus statusMessage;
                statusMessage().set_status( sword::SessionStatus::breakdown );
                statusMessage().set_breakdown_information( error );
                statusMessage().set_exercise( command->GetExercise() );
                statusMessage().set_session( command->GetSession() );
                statusMessage.Send( publisher );
                processes_.erase( it );
                break;
            }
        }
    }
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
        if( it->first.first == message.exercise() )
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
    if( ! frontend::commands::ExerciseExists( config_, exercice ) )
        message.set_error_code( sword::CheckpointListResponse::invalid_exercise_name );
    else if( ! frontend::commands::SessionExists( config_, exercice, session ) )
        message.set_error_code( sword::CheckpointListResponse::invalid_session_name );
    else
    {
        message.set_error_code( sword::CheckpointListResponse::success );
        const QStringList checkpoints = frontend::commands::ListCheckpoints( config_, exercice, session );
        for( QStringList::const_iterator it = checkpoints.begin(); it != checkpoints.end(); ++it )
            message.add_checkpoint( ( *it ).ascii() );
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::RemoveCheckpoint
// Created: LGY 2011-05-19
// -----------------------------------------------------------------------------
void ProcessService::RemoveCheckpoint( sword::CheckpointDeleteResponse& message, const std::vector< std::string >& checkpoints,
                                       const std::string& exercice, const std::string& session )
{
    if( ! frontend::commands::ExerciseExists( config_, exercice ) )
        message.set_error_code( sword::CheckpointDeleteResponse::invalid_exercise_name );
    else if( ! frontend::commands::SessionExists( config_, exercice, session ) )
        message.set_error_code( sword::CheckpointDeleteResponse::invalid_session_name );
    else
    {
        message.set_error_code( sword::CheckpointDeleteResponse::success );
        const std::vector< std::string > result = frontend::commands::RemoveCheckpoint( config_, exercice, session, checkpoints );
        ProcessContainer::const_iterator it = processes_.find( std::make_pair( exercice, session ) );
        boost::shared_ptr< SwordFacade > client;
        if( it != processes_.end() && it->second->IsConnected() )
            client = it->second;
        BOOST_FOREACH( const std::string& name, result )
        {
            message.add_checkpoint( name );
            if( client.get() )
            {
                simulation::ControlCheckPointDeleteRequest request;
                request().set_checkpoint( name );
                request.Send( *client );
            }
        }
    }
}

namespace
{
    template< typename T, typename U >
    void SendErrorMessage( LauncherPublisher& publisher, const std::string& exercise, const std::string& session, U error )
    {
        T message;
        message().set_exercise( exercise );
        message().set_session( session );
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
    ProcessContainer::const_iterator it = processes_.find( std::make_pair( message.exercise(), message.session() ) );
    static int context = 1;
    if( processes_.end() == it )
        return SendErrorMessage< SessionCommandExecutionResponse >( *server_.ResolveClient( endpoint ), message.exercise(), message.session(), sword::SessionCommandExecutionResponse::invalid_session_name );
    if( !it->second->IsConnected() )
        return SendErrorMessage< SessionCommandExecutionResponse >( *server_.ResolveClient( endpoint ), message.exercise(), message.session(), sword::SessionCommandExecutionResponse::session_not_running );
    boost::shared_ptr< SwordFacade > client( it->second );
    if( message.has_set_running() )
    {
        ExecutePauseResume( endpoint, message.exercise(), message.session(), message.set_running(), context, *client );
        ++context;
    }
    if( message.has_save_checkpoint() )
    {
        SaveCheckpoint( message.save_checkpoint(), *client );
        SessionCommandExecutionResponse response;
        response().set_exercise( message.exercise() );
        response().set_session( message.session() );
        response().set_error_code( sword::SessionCommandExecutionResponse::success );
        response().set_saved_checkpoint( message.save_checkpoint() );
        response.Send( *server_.ResolveClient( endpoint ) );
    }
    if( message.has_time_change() )
    {
        ExecuteChangeTime( endpoint, message.exercise(), message.session(), message.time_change().data(), context, *client );
        ++context;
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::CheckForRunningProcesses
// Created: JSR 2011-12-13
// -----------------------------------------------------------------------------
void ProcessService::CheckForRunningProcesses()
{
    HANDLE hProcessSnap = INVALID_HANDLE_VALUE;
    hProcessSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
    if( hProcessSnap == INVALID_HANDLE_VALUE )
        return;
    PROCESSENTRY32 pe32 = { 0 };
    pe32.dwSize = sizeof( PROCESSENTRY32 );
    if( !Process32First( hProcessSnap, &pe32 ) )
    {
        CloseHandle( hProcessSnap );
        return;
    }
    static const std::string simulationApp = "simulation_app.exe";
    static const std::string dispatcherApp = "dispatcher_app.exe";
    static const std::string replayerApp = "replayer_app.exe";
    do
    {
        if( pe32.szExeFile == simulationApp || pe32.szExeFile == dispatcherApp || pe32.szExeFile == replayerApp )
        {
            std::string session;
            std::string exercise;
            std::string checkpoint;
            std::string login;
            std::string password;
            try
            {
                std::ifstream file( GetSessionTmpFilename().c_str() );
                if( file.is_open() )
                {
                    std::getline( file, session );
                    std::getline( file, exercise );
                    std::getline( file, checkpoint );
                    std::getline( file, login );
                    std::getline( file, password );
                    file.close();
                }
            }
            catch( ... )
            {
                CloseHandle( hProcessSnap );
                return;
            }
            boost::shared_ptr< frontend::SpawnCommand > command;
            command.reset( new frontend::AttachCommand( config_, pe32.th32ProcessID, false, exercise, session ) );
            boost::shared_ptr< SwordFacade > wrapper( new SwordFacade( server_, "", pe32.szExeFile == dispatcherApp ) );
            {
                boost::recursive_mutex::scoped_lock locker( mutex_ );
                processes_[ std::make_pair( exercise, session ) ] = wrapper;
            }
            wrapper->Start( *this, command, login, password, config_, true );
            break;
        }
    }
    while( Process32Next( hProcessSnap, &pe32 ) );
    CloseHandle( hProcessSnap );
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
// Name: ProcessService::ExecuteChangeTime
// Created: LGY 2011-06-22
// -----------------------------------------------------------------------------
void ProcessService::ExecuteChangeTime( const std::string& endpoint, const std::string& exercise, const std::string& session,
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
void ProcessService::SaveCheckpoint( const std::string& name, SwordFacade& facade )
{
    simulation::ControlCheckPointSaveNow request;
    request().set_name( name );
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
    ProcessContainer::const_iterator it = processes_.find( std::make_pair( message.exercise(), message.session() ) );
    static int context = 1;
    if( processes_.end() == it )
        return SendErrorMessage< SessionParameterChangeResponse >( *server_.ResolveClient( endpoint ), message.exercise(), message.session(), sword::SessionParameterChangeResponse::invalid_session_name );
    if( !it->second->IsConnected() )
        return SendErrorMessage< SessionParameterChangeResponse >( *server_.ResolveClient( endpoint ), message.exercise(), message.session(), sword::SessionParameterChangeResponse::session_not_running );

    boost::shared_ptr< SwordFacade > client( it->second );
    SessionParameterChangeResponse parameterChangeResponse;
    parameterChangeResponse().set_error_code( sword::SessionParameterChangeResponse::success );
    parameterChangeResponse().set_exercise( message.exercise() );
    parameterChangeResponse().set_session( message.session() );
    if( message.has_acceleration_factor() )
    {
        simulation::ControlChangeTimeFactor request;
        request().set_time_factor( message.acceleration_factor() );
        request.Send( *client, 0 );
        parameterChangeResponse().set_acceleration_factor( message.acceleration_factor() );
    }
    if( message.has_checkpoint_frequency() )
    {
        simulation::ControlCheckPointSetFrequency request;
        request().set_frequency( message.checkpoint_frequency() );
        request.Send( *client, 0 );
        parameterChangeResponse().set_checkpoint_frequency( message.checkpoint_frequency() );
    }
    parameterChangeResponse.Send( *server_.ResolveClient( endpoint ) );
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendConnectedProfiles
// Created: AHC 2011-05-20
// -----------------------------------------------------------------------------
void ProcessService::SendConnectedProfiles( const std::string& endpoint, const sword::ConnectedProfileListRequest& message )
{
    ProcessContainer::const_iterator it = processes_.find( std::make_pair( message.exercise(), message.session() ) );
    static int context = 1;
    if( processes_.end() == it )
        return SendErrorMessage< ConnectedProfileListResponse >( *server_.ResolveClient( endpoint ), message.exercise(), message.session(), sword::ConnectedProfileListResponse::invalid_session_name );
    if( !it->second->IsConnected() )
        return SendErrorMessage< ConnectedProfileListResponse >( *server_.ResolveClient( endpoint ), message.exercise(), message.session(), sword::ConnectedProfileListResponse::session_not_running );
    boost::shared_ptr< SwordFacade > client( it->second );
    client->RegisterMessageHandler( context, std::auto_ptr< MessageHandler_ABC >( new ConnectedProfilesMessageHandler( server_.ResolveClient( endpoint ), message.exercise(), message.session() ) ) );
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
    if ( !endpoint.empty() )
    {
        LauncherPublisher& publisher = *server_.ResolveClient( endpoint );
        for( ProcessContainer::iterator it = processes_.begin(); it != processes_.end(); ++it )
        {
            const frontend::SpawnCommand* command = it->second->GetProcess()->GetCommand();
            if( command && command->GetCommanderEndpoint() == endpoint )
            {
                SessionStatus statusMessage;
                std::string exercise = command->GetExercise();
                std::string session = command->GetSession();
                statusMessage().set_status( IsRunning( exercise, session )? sword::SessionStatus::running : sword::SessionStatus::not_running );
                statusMessage().set_exercise( exercise );
                statusMessage().set_session( session );
                statusMessage.Send( publisher );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ProcessService::SendSessionNotification
// Created: LGY 2011-11-09
// -----------------------------------------------------------------------------
void ProcessService::SendSessionNotification( const sword::SessionNotificationRequest& message ) const
{
    ProcessContainer::const_iterator it = processes_.find( std::make_pair( message.exercise(), message.session() ) );
    if( it != processes_.end() )
    {
        boost::shared_ptr< SwordFacade > client( it->second );
        plugins::messenger::ClientObjectCreationRequest message;
        message().set_name( "NotifModificationAnnuaire" );
        message().set_persistent( false );
        message.Send( *client );
    }
}
