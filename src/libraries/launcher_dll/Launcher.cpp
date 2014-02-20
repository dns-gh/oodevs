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
// Created: AHC 2011-05-12
// -----------------------------------------------------------------------------
void Launcher::HandleRequest( const std::string& /*endpoint*/, const sword::SessionNotificationRequest& /*message*/ )
{
    // TODO AHC
}
