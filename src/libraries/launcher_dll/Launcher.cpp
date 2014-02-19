// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "launcher_dll_pch.h"
#include "Launcher.h"
#include "Config.h"
#include "LauncherPublisher.h"
#include "LauncherService.h"
#include "ProcessService.h"
#include "tools/NullFileLoaderObserver.h"
#include "tools/DefaultLoader.h"
#include "protocol/LauncherSenders.h"
#include "protocol/Version.h"

using namespace launcher;


// -----------------------------------------------------------------------------
// Name: Launcher constructor
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
Launcher::Launcher( const Config& config )
    : fileLoaderObserver_( new tools::NullFileLoaderObserver() )
    , fileLoader_        ( new tools::DefaultLoader( *fileLoaderObserver_ ) )
    , server_            ( new LauncherService( config.GetLauncherPort() ) )
    , processes_         ( new ProcessService( config, *fileLoader_, *server_ ) )
{
    server_->RegisterMessage( *this, &Launcher::HandleAdminToLauncher );
}

// -----------------------------------------------------------------------------
// Name: Launcher destructor
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
Launcher::~Launcher()
{
    server_.reset();
}

// -----------------------------------------------------------------------------
// Name: Launcher::Update
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
void Launcher::Update()
{
    if( server_.get() )
        server_->Update();
    if( processes_.get() )
        processes_->Update();
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleAdminToLauncher
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
void Launcher::HandleAdminToLauncher( const std::string& endpoint, const sword::AdminToLauncher& message )
{
    if( message.message().has_connection_request() )
        HandleRequest( endpoint, message.message().connection_request() );
    else if( message.message().has_exercise_list_request() )
        HandleRequest( endpoint, message.message().exercise_list_request() );
    else if( message.message().has_session_start_request() )
        HandleRequest( endpoint, message.message().session_start_request() );
    else if( message.message().has_session_stop_request() )
        HandleRequest( endpoint, message.message().session_stop_request() );
    else if( message.message().has_session_list_request() )
        HandleRequest( endpoint, message.message().session_list_request() );
    else if( message.message().has_session_parameter_change_request() )
        HandleRequest( endpoint, message.message().session_parameter_change_request() );
    else if( message.message().has_checkpoint_list_request() )
        HandleRequest( endpoint, message.message().checkpoint_list_request() );
    else if( message.message().has_checkpoint_delete_request() )
        HandleRequest( endpoint, message.message().checkpoint_delete_request() );
    else if( message.message().has_session_notification() )
        HandleRequest( endpoint, message.message().session_notification() );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleRequest
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
void Launcher::HandleRequest( const std::string& endpoint, const sword::ConnectionRequest& message )
{
    ConnectionResponse response;
    const bool valid = sword::CheckCompatibility( message.client_version() );
    response().set_error_code( valid ? sword::ConnectionResponse::success : sword::ConnectionResponse::incompatible_protocol_version );
    response().mutable_server_version()->set_value( sword::ProtocolVersion().value() );
    response.Send( server_->ResolveClient( endpoint ) );
    processes_->SendSessionsStatuses( endpoint );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleRequest
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void Launcher::HandleRequest( const std::string& endpoint, const sword::ExerciseListRequest& /*message*/ )
{
    ExerciseListResponse response;
    processes_->SendExerciseList( response() );
    response.Send( server_->ResolveClient( endpoint ) );
    processes_->SendRunningExercices( endpoint );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleRequest
// Created: SBO 2010-10-06
// -----------------------------------------------------------------------------
void Launcher::HandleRequest( const std::string& endpoint, const sword::SessionStartRequest& message )
{
    SessionStartResponse response;
    response().set_exercise( message.exercise() );
    response().set_session( message.session() );
    response().set_error_code( processes_->StartSession( endpoint, message ) );
    switch( message.type() )
    {
        case sword::SessionStartRequest::simulation:
            response().set_type( sword::SessionStartResponse::simulation );
            break;
        case sword::SessionStartRequest::dispatch:
            response().set_type( sword::SessionStartResponse::dispatch );
            break;
        case sword::SessionStartRequest::replay:
            response().set_type( sword::SessionStartResponse::replay );
            break;
    }
    response().set_checkpoint( message.has_checkpoint() ? message.checkpoint() : "" );
    response.Send( server_->ResolveClient( endpoint ) );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleRequest
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void Launcher::HandleRequest( const std::string& endpoint, const sword::SessionStopRequest& message )
{
    SessionStopResponse response;
    response().set_exercise( message.exercise() );
    response().set_session( message.session() );
    response().set_error_code( processes_->StopSession( message ) );
    response.Send( server_->ResolveClient( endpoint ) );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleRequest
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void Launcher::HandleRequest( const std::string& endpoint, const sword::SessionListRequest& message )
{
    SessionListResponse response;
    response().set_exercise( message.exercise() );
    processes_->SendSessionList( response() );
    response().set_error_code( sword::SessionListResponse::success );
    response.Send( server_->ResolveClient( endpoint ) );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleRequest
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void Launcher::HandleRequest( const std::string& endpoint, const sword::SessionParameterChangeRequest& message )
{
    processes_->ChangeParameter( endpoint, message );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleRequest
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void Launcher::HandleRequest( const std::string& endpoint, const sword::CheckpointListRequest& message )
{
    CheckpointListResponse response;
    response().set_exercise( message.exercise() );
    response().set_session( message.session() );
    processes_->SendCheckpointList( response(), tools::Path::FromUTF8( message.exercise() ), tools::Path::FromUTF8( message.session() ) );
    response.Send( server_->ResolveClient( endpoint ) );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleRequest
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void Launcher::HandleRequest( const std::string& endpoint, const sword::CheckpointDeleteRequest& message )
{
    CheckpointDeleteResponse response;
    response().set_exercise( message.exercise() );
    response().set_session( message.session() );
    tools::Path::T_Paths checkpoints;
    for( int i = 0; i < message.checkpoint_size(); ++i )
        checkpoints.push_back( tools::Path::FromUTF8( message.checkpoint( i ) ) );
    processes_->RemoveCheckpoint( response(), checkpoints, tools::Path::FromUTF8( message.exercise() ), tools::Path::FromUTF8( message.session() ) );
    response.Send( server_->ResolveClient( endpoint ) );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleRequest
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void Launcher::HandleRequest( const std::string& /*endpoint*/, const sword::SessionNotificationRequest& /*message*/ )
{
    // TODO AHC
}
