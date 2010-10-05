// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Launcher.h"
#include "Config.h"
#include "LauncherPublisher.h"
#include "LauncherService.h"
#include "ProcessService.h"
#include "protocol/LauncherSenders.h"
#include "protocol/ProtocolVersionChecker.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Launcher constructor
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
Launcher::Launcher( kernel::Controllers& controllers, const frontend::Config& config )
    : server_( new LauncherService( config.GetLauncherPort() ) )
    , processes_( new ProcessService( controllers, config ) )
{
    server_->RegisterMessage( *this, &Launcher::HandleAdminToLauncher );
}

// -----------------------------------------------------------------------------
// Name: Launcher destructor
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
Launcher::~Launcher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Launcher::Update
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
void Launcher::Update()
{
    server_->Update();
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleAdminToLauncher
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
void Launcher::HandleAdminToLauncher( const std::string& endpoint, const MsgsAdminToLauncher::MsgAdminToLauncher& message )
{
    if( message.message().has_connection_request() )
        HandleConnectionRequest( endpoint, message.message().connection_request() );
    else if( message.message().has_exercise_list_request() )
        HandleExerciseListRequest( endpoint, message.message().exercise_list_request() );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleConnectionRequest
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
void Launcher::HandleConnectionRequest( const std::string& endpoint, const MsgsAdminToLauncher::MsgConnectionRequest& message )
{
    launcher::ConnectionAck response;
    const bool valid = ProtocolVersionChecker( message.client_version() ).CheckCompatibility();
    response().set_error_code( valid ? MsgsLauncherToAdmin::MsgConnectionAck::success : MsgsLauncherToAdmin::MsgConnectionAck::incompatible_protocol_version );
    response().mutable_server_version()->set_value( Version::ProtocolVersion().value() );
//    if( valid )
    {
        response().mutable_dispatcher_address()->set_ip( "127.0.0.1" ); // $$$$ SBO 2010-09-30: ???
        response().mutable_dispatcher_address()->set_port( 33000 );     // $$$$ SBO 2010-09-30: ???
    }
    response.Send( server_->ResolveClient( endpoint ) );
}

// -----------------------------------------------------------------------------
// Name: Launcher::HandleExerciseListRequest
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void Launcher::HandleExerciseListRequest( const std::string& endpoint, const MsgsAdminToLauncher::MsgExercicesListRequest& message )
{
    launcher::ExercicesListResponse response;
    response().set_error_code( MsgsLauncherToAdmin::MsgExercicesListResponse::success );
    processes_->SendExerciseList( response() );
    response.Send( server_->ResolveClient( endpoint ) );
}
