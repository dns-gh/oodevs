// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "LauncherClient.h"
#include "ConnectionHandler_ABC.h"
#include "LauncherPublisher.h"
#include "RemoteHost.h"
#include "ResponseHandlerProxy.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Tools.h"
#include "protocol/LauncherSenders.h"
#include <boost/lexical_cast.hpp>

#pragma warning( disable: 4355 )

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: LauncherClient constructor
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
LauncherClient::LauncherClient( kernel::Controller& controller )
    : controller_     ( controller )
    , handler_        ( 0 )
    , publisher_      ( new LauncherPublisher( *this ) )
    , responseHandler_( new ResponseHandlerProxy() )
    , connected_      ( false )
{
    RegisterMessage( *this, &LauncherClient::HandleLauncherToAdmin );
}

// -----------------------------------------------------------------------------
// Name: LauncherClient destructor
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
LauncherClient::~LauncherClient()
{
    ResetConnection();
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::Connect
// Created: SBO 2010-10-01
// -----------------------------------------------------------------------------
void LauncherClient::Connect( const std::string& host, unsigned int port, frontend::ConnectionHandler_ABC& handler )
{
    handler_ = &handler;
    tools::ClientNetworker::Connect( host + ":" + boost::lexical_cast< std::string >( port ), false );
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::ConnectionSucceeded
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
void LauncherClient::ConnectionSucceeded( const std::string& endpoint )
{
    publisher_->SetHost( endpoint );
    responseHandler_->SetMainHandler( boost::shared_ptr< ResponseHandler_ABC >( new RemoteHost( *publisher_, endpoint, controller_ ) ) );
    launcher::ConnectionRequest message;
    message().mutable_client_version()->set_value( sword::ProtocolVersion().value() );
    message.Send( *publisher_ );
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::ConnectionError
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
void LauncherClient::ConnectionError( const std::string& address, const std::string& error )
{
    tools::ClientNetworker::ConnectionError( address, error );
    if( handler_ )
        handler_->OnConnectionLost( error );
    ResetConnection();
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::ConnectionFailed
// Created: SBO 2010-10-22
// -----------------------------------------------------------------------------
void LauncherClient::ConnectionFailed( const std::string& address, const std::string& error )
{
    tools::ClientNetworker::ConnectionFailed( address, error );
    if( handler_ )
        handler_->OnConnectionFailed( error );
    ResetConnection();
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::Connected
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
bool LauncherClient::Connected() const
{
    return connected_;
}

namespace
{
    QString MessageString( const sword::ConnectionResponse::ErrorCode& error )
    {
        switch( error )
        {
        case sword::ConnectionResponse::success:
            return tools::translate( "LauncherClient", "success" );
        case sword::ConnectionResponse::incompatible_protocol_version:
            return tools::translate( "LauncherClient", "incompatible protocol version" );
        default:
            return tools::translate( "LauncherClient", "unknown error" );
        }
    }

    QString MessageString( const sword::SessionStartResponse::ErrorCode& error )
    {
        switch( error )
        {
        case sword::SessionStartResponse::invalid_exercise_name:
            return tools::translate( "LauncherClient", "invalid exercise name" );
        case sword::SessionStartResponse::session_already_running:
            return tools::translate( "LauncherClient", "session already running" );
        case sword::SessionStartResponse::invalid_checkpoint:
            return tools::translate( "LauncherClient", "invalid checkpoint" );
        default:
            return tools::translate( "LauncherClient", "unknown error" );
        }
    }

    QString MessageString( const sword::SessionStopResponse::ErrorCode& error )
    {
        switch( error )
        {
        case sword::SessionStopResponse::invalid_exercise_name:
            return tools::translate( "LauncherClient", "invalid exercise name" );
        case sword::SessionStopResponse::invalid_session_name:
            return tools::translate( "LauncherClient", "invalid session name" );
        case sword::SessionStopResponse::session_not_running:
            return tools::translate( "LauncherClient", "session not running" );
        default:
            return tools::translate( "LauncherClient", "unknown error" );
        }
    }
    QString MessageString( const sword::SessionCommandExecutionResponse::ErrorCode& error )
    {
        switch( error )
        {
        case sword::SessionCommandExecutionResponse::invalid_exercise_name:
            return tools::translate( "LauncherClient", "invalid exercise name" );
        case sword::SessionCommandExecutionResponse::invalid_session_name:
            return tools::translate( "LauncherClient", "invalid session name" );
        case sword::SessionCommandExecutionResponse::session_not_running:
            return tools::translate( "LauncherClient", "session not running" );
        case sword::SessionCommandExecutionResponse::invalid_checkpoint:
            return tools::translate( "LauncherClient", "invalid checkpoint" );
        case sword::SessionCommandExecutionResponse::session_already_paused:
            return tools::translate( "LauncherClient", "session already paused" );
        case sword::SessionCommandExecutionResponse::session_already_running:
            return tools::translate( "LauncherClient", "session already running" );
        default:
            return tools::translate( "LauncherClient", "unknown error" );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::HandleLauncherToAdmin
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void LauncherClient::HandleLauncherToAdmin( const std::string& /*endpoint*/, const sword::LauncherToAdmin& message )
{
    if( message.message().has_connection_response() )
    {
        if( message.message().connection_response().error_code() != sword::ConnectionResponse::success )
        {
            if( handler_ )
                handler_->OnConnectionFailed( tools::translate( "LauncherClient", "Failed to contact launcher service: %1." )
                                                .arg( MessageString( message.message().connection_response().error_code() ) ).ascii() );
            ResetConnection();
        }
        else if( handler_ )
        {
            connected_ = true;
            handler_->OnConnectionSucceeded();
        }
    }
    else if( message.message().has_exercise_list_response() )
        responseHandler_->Handle( message.message().exercise_list_response() );
    else if( message.message().has_profile_list_response() )
        responseHandler_->Handle( message.message().profile_list_response() );
    else if( message.message().has_session_start_response() )
    {
        if( message.message().session_start_response().error_code() != sword::SessionStartResponse::success )
            handler_->OnError( tools::translate( "LauncherClient", "Failed to start session: %1." )
                                .arg( MessageString( message.message().session_start_response().error_code() ) ).ascii() );
        responseHandler_->Handle( message.message().session_start_response() );
    }
    else if( message.message().has_session_stop_response() )
    {
        if( message.message().session_stop_response().error_code() != sword::SessionStopResponse::success )
            handler_->OnError( tools::translate( "LauncherClient", "Failed to stop exercise: %1." )
                                .arg( MessageString( message.message().session_stop_response().error_code() ) ).ascii() );
        responseHandler_->Handle( message.message().session_stop_response() );
    }
    else if( message.message().has_session_command_execution_response() )
    {
        if( message.message().session_stop_response().error_code() != sword::SessionCommandExecutionResponse::success )
            handler_->OnError( tools::translate( "LauncherClient", "Failed to execute command on exercise: %1." )
                                .arg( MessageString( message.message().session_command_execution_response().error_code() ) ).ascii() );
        responseHandler_->Handle( message.message().session_command_execution_response() );
    }
    else if( message.message().has_session_notification() )
        responseHandler_->Handle( message.message().session_notification() );
    else if( message.message().has_session_parameter_change_response() )
        responseHandler_->Handle( message.message().session_parameter_change_response() );
    else if ( message.message().has_session_status() )
        responseHandler_->Handle( message.message().session_status() );
    else if ( message.message().has_connected_profile_list_response() )
        responseHandler_->Handle( message.message().connected_profile_list_response() );
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::QueryExerciseList
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void LauncherClient::QueryExerciseList()
{
    if( Connected() )
    {
        launcher::ExerciseListRequest message;
        message.Send( *publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::ResetConnection
// Created: SBO 2010-10-22
// -----------------------------------------------------------------------------
void LauncherClient::ResetConnection()
{
    Disconnect();
    responseHandler_->ResetMainHandler();
    connected_ = false;
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::Register
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
void LauncherClient::Register( boost::shared_ptr< ResponseHandler_ABC > handler )
{
    responseHandler_->Register( handler );
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::Endpoint
// Created: AHC 2011-05-27
// -----------------------------------------------------------------------------
const std::string& LauncherClient::Endpoint() const
{
    return publisher_->Endpoint();
}
