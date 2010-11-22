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

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: LauncherClient constructor
// Created: SBO 2010-09-29
// -----------------------------------------------------------------------------
LauncherClient::LauncherClient( kernel::Controller& controller )
    : tools::ClientNetworker( "", false )
    , controller_( controller )
    , handler_( 0 )
    , publisher_( new LauncherPublisher( *this ) )
    , responseHandler_( new ResponseHandlerProxy() )
    , connected_( false )
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
    message().mutable_client_version()->set_value( Version::ProtocolVersion().value() );
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
    QString MessageString( const MsgsLauncherToAdmin::MsgConnectionAck::ErrorCode& error )
    {
        switch( error )
        {
        case MsgsLauncherToAdmin::MsgConnectionAck::invalid_connection:
            return tools::translate( "LauncherClient", "invalid connection" );
        case MsgsLauncherToAdmin::MsgConnectionAck::incompatible_protocol_version:
            return tools::translate( "LauncherClient", "incompatible protocol version" );
        case MsgsLauncherToAdmin::MsgConnectionAck::exercise_already_running:
            return tools::translate( "LauncherClient", "exercise already running" );
        default:
            return tools::translate( "LauncherClient", "unknown error" );
        }
    }

    QString MessageString( const MsgsLauncherToAdmin::MsgControlStartAck::ErrorCode& error )
    {
        switch( error )
        {
        case MsgsLauncherToAdmin::MsgControlStartAck::bad_exercise_name:
            return tools::translate( "LauncherClient", "bad exercise name" );
        case MsgsLauncherToAdmin::MsgControlStartAck::exercise_already_running:
            return tools::translate( "LauncherClient", "exercise already running" );
        case MsgsLauncherToAdmin::MsgControlStartAck::invalid_checkpoint:
            return tools::translate( "LauncherClient", "invalid checkpoint" );
        default:
            return tools::translate( "LauncherClient", "unknown error" );
        }
    }

    QString MessageString( const MsgsLauncherToAdmin::MsgControlStopAck::ErrorCode& error )
    {
        switch( error )
        {
        case MsgsLauncherToAdmin::MsgControlStopAck::bad_exercise_name:
            return tools::translate( "LauncherClient", "bad exercise name" );
        case MsgsLauncherToAdmin::MsgControlStopAck::exercise_not_running:
            return tools::translate( "LauncherClient", "exercise not running" );
        default:
            return tools::translate( "LauncherClient", "unknown error" );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::HandleLauncherToAdmin
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void LauncherClient::HandleLauncherToAdmin( const std::string& /*endpoint*/, const MsgsLauncherToAdmin::MsgLauncherToAdmin& message )
{
    if( message.message().has_connection_ack() )
    {
        if( message.message().connection_ack().error_code() != MsgsLauncherToAdmin::MsgConnectionAck::success )
        {
            if( handler_ )
                handler_->OnConnectionFailed( tools::translate( "LauncherClient", "Failed to contact launcher service: %1." )
                                                .arg( MessageString( message.message().connection_ack().error_code() ) ).ascii() );
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
    else if( message.message().has_profiles_description() )
        responseHandler_->Handle( message.message().profiles_description() );
    else if( message.message().has_control_start_ack() )
    {
        if( message.message().control_start_ack().error_code() != MsgsLauncherToAdmin::MsgControlStartAck::success )
            handler_->OnError( tools::translate( "LauncherClient", "Failed to start exercise: %1." )
                                .arg( MessageString( message.message().control_start_ack().error_code() ) ).ascii() );
        responseHandler_->Handle( message.message().control_start_ack() );
    }
    else if( message.message().has_control_stop_ack() )
    {
        if( message.message().control_stop_ack().error_code() != MsgsLauncherToAdmin::MsgControlStopAck::success )
            handler_->OnError( tools::translate( "LauncherClient", "Failed to stop exercise: %1." )
                                .arg( MessageString( message.message().control_stop_ack().error_code() ) ).ascii() );
        responseHandler_->Handle( message.message().control_stop_ack() );
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::QueryExerciseList
// Created: SBO 2010-09-30
// -----------------------------------------------------------------------------
void LauncherClient::QueryExerciseList()
{
    if( Connected() )
    {
        launcher::ExercicesListRequest message;
        message.Send( *publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: LauncherClient::QueryProfileList
// Created: SBO 2010-11-22
// -----------------------------------------------------------------------------
void LauncherClient::QueryProfileList()
{
    if( Connected() )
    {
        launcher::ProfilesListRequest message;
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
