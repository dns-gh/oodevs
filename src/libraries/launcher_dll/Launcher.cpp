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
    , processes_         ( new ProcessService( config, *fileLoader_ ) )
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
    else if( message.message().has_control_start() )
        HandleRequest( endpoint, message.message().control_start() );
    else if( message.message().has_control_stop() )
        HandleRequest( endpoint, message.message().control_stop() );
    else if( message.message().has_profile_list_request() )
        HandleRequest( endpoint, message.message().profile_list_request() );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleRequest
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
void Launcher::HandleRequest( const std::string& endpoint, const sword::ConnectionRequest& message )
{
    ConnectionAck response;
    const bool valid = sword::CheckCompatibility( message.client_version() );
    response().set_error_code( valid ? sword::ConnectionAck::success : sword::ConnectionAck::incompatible_protocol_version );
    response().mutable_server_version()->set_value( sword::ProtocolVersion().value() );
//    if( valid )
    {
        response().mutable_dispatcher_address()->set_ip( "127.0.0.1" ); // $$$$ SBO 2010-09-30: ???
        response().mutable_dispatcher_address()->set_port( 30001 );     // $$$$ SBO 2010-09-30: ???
    }
    response.Send( server_->ResolveClient( endpoint ) );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleRequest
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void Launcher::HandleRequest( const std::string& endpoint, const sword::ExercicesListRequest& /*message*/ )
{
    ExercicesListResponse response;
    response().set_error_code( sword::ExercicesListResponse::success );
    processes_->SendExerciseList( response() );
    response.Send( server_->ResolveClient( endpoint ) );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleRequest
// Created: SBO 2010-10-06
// -----------------------------------------------------------------------------
void Launcher::HandleRequest( const std::string& endpoint, const sword::ControlStartExercise& message )
{
    ControlStartExerciseAck response;
    response().mutable_exercise()->set_name( message.exercise().name() );
    response().mutable_exercise()->set_port( message.exercise().port() );
    response().set_error_code( processes_->StartExercise( message ) );
    response().mutable_exercise()->set_running( response().error_code() == sword::ControlStartExerciseAck::success );
    response.Send( server_->ResolveClient( endpoint ) );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleRequest
// Created: SBO 2010-10-28
// -----------------------------------------------------------------------------
void Launcher::HandleRequest( const std::string& endpoint, const sword::ControlStopExercise& message )
{
    ControlStopExerciseAck response;
    response().mutable_exercise()->set_name( message.exercise().name() );
    response().mutable_exercise()->set_port( message.exercise().port() );
    response().set_error_code( processes_->StopExercise( message ) );
    response().mutable_exercise()->set_running( false );
    response.Send( server_->ResolveClient( endpoint ) );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleRequest
// Created: SBO 2010-11-19
// -----------------------------------------------------------------------------
void Launcher::HandleRequest( const std::string& endpoint, const sword::ProfilesListRequest& /*message*/ )
{
    ProfileDescriptionList response;
    processes_->SendProfileList( response() );
    response.Send( server_->ResolveClient( endpoint ) );
}
