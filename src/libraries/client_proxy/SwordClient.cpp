// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "client_proxy_pch.h"
#include "SwordClient.h"
#include "ConnectionHandlerProxy.h"
#include "MessageSender.h"
#include "SwordMessageHandler_ABC.h"
#include "protocol/AarSenders.h"
#include "protocol/AuthenticationSenders.h"
#include "protocol/DispatcherSenders.h"
#include "protocol/MessengerSenders.h"
#include "protocol/ReplaySenders.h"
#include "protocol/SimulationSenders.h"
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: SwordClient constructor
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
SwordClient::SwordClient( const std::string& host, unsigned short port, const std::string& profile, const std::string& password )
    : tools::ClientNetworker( "", false )
    , host_                 ( host )
    , port_                 ( port )
    , profile_              ( profile )
    , password_             ( password )
    , loggedIn_             ( false )
    , connectionHandler_    ( new ConnectionHandlerProxy() )
{
    RegisterMessage( *this, &SwordClient::HandleAarToClient );
    RegisterMessage( *this, &SwordClient::HandleAuthenticationToClient );
    RegisterMessage( *this, &SwordClient::HandleDispatcherToClient );
    RegisterMessage( *this, &SwordClient::HandleMessengerToClient );
    RegisterMessage( *this, &SwordClient::HandleReplayToClient );
    RegisterMessage( *this, &SwordClient::HandleSimToClient );
}

// -----------------------------------------------------------------------------
// Name: SwordClient destructor
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
SwordClient::~SwordClient()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SwordClient::Connect
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::Connect( SwordConnectionHandler_ABC* handler )
{
    connectionHandler_->SetHandler( handler );
    tools::ClientNetworker::Connect( host_ + ":" + boost::lexical_cast< std::string >( port_ ), true );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::ConnectionSucceeded
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::ConnectionSucceeded( const std::string& endpoint )
{
    tools::ClientNetworker::ConnectionSucceeded( endpoint );
    connectionHandler_->OnConnectionSucceeded( endpoint );
    publisher_.reset( new MessageSender( *this, endpoint ) );
    {
        authentication::AuthenticationRequest message;
        message().mutable_version()->set_value( sword::ProtocolVersion().value() );
        message().set_login( profile_ );
        message().set_password( password_ );
        message.Send( *publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: SwordClient::ConnectionFailed
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::ConnectionFailed( const std::string& endpoint, const std::string& error )
{
    tools::ClientNetworker::ConnectionFailed( endpoint, error );
    connectionHandler_->OnConnectionFailed( endpoint, error );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::ConnectionError
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::ConnectionError( const std::string& endpoint, const std::string& error )
{
    tools::ClientNetworker::ConnectionError( endpoint, error );
    connectionHandler_->OnConnectionError( endpoint, error );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::HandleAarToClient
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::HandleAarToClient( const std::string& /*endpoint*/, const sword::AarToClient& /*message*/ )
{
    // NOTHING
}

namespace
{
    std::string ToString( const sword::AuthenticationResponse::ErrorCode& code )
    {
        if( code == sword::AuthenticationResponse::invalid_login )
            return "invalid login";
        if( code == sword::AuthenticationResponse::too_many_connections )
            return "too many connections";
        if( code == sword::AuthenticationResponse::mismatched_protocol_version )
            return "mismatched protocol version";
        return "unknown error";
    }
}

// -----------------------------------------------------------------------------
// Name: SwordClient::HandleAuthenticationToClient
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::HandleAuthenticationToClient( const std::string& /*endpoint*/, const sword::AuthenticationToClient& message )
{
    if( message.message().has_authentication_response() )
    {
        const std::string login( message.message().authentication_response().profile().login() );
        loggedIn_ = message.message().authentication_response().error_code() == sword::AuthenticationResponse::success;
        if( loggedIn_ )
            connectionHandler_->OnAuthenticationSucceeded( login );
        else
            connectionHandler_->OnAuthenticationFailed( profile_, ToString( message.message().authentication_response().error_code() ) );
    }
    UpdateHandlers( message );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::HandleDispatcherToClient
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::HandleDispatcherToClient( const std::string& /*endpoint*/, const sword::DispatcherToClient& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SwordClient::HandleMessengerToClient
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::HandleMessengerToClient( const std::string& /*endpoint*/, const sword::MessengerToClient& message )
{
    UpdateHandlers( message );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::HandleReplayToClient
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::HandleReplayToClient( const std::string& /*endpoint*/, const sword::ReplayToClient& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SwordClient::HandleSimToClient
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::HandleSimToClient( const std::string& /*endpoint*/, const sword::SimToClient& message )
{
    UpdateHandlers( message );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::RegisterMessageHandler
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::RegisterMessageHandler( SwordMessageHandler_ABC& handler )
{
    messageHandlers_.push_back( &handler );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::UnregisterMessageHandler
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::UnregisterMessageHandler( SwordMessageHandler_ABC& handler )
{
    for( T_Handlers::iterator it = messageHandlers_.begin(); it != messageHandlers_.end(); ++it )
        if( *it == &handler )
        {
            messageHandlers_.erase( it );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: SwordClient::SendMessage
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void SwordClient::SendMessage( const sword::ClientToSim& message ) const
{
    if( publisher_.get() )
        publisher_->Send( message );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::SendMessage
// Created: AHC 2011-05-23
// -----------------------------------------------------------------------------
void SwordClient::SendMessage( const sword::ClientToAuthentication& message ) const
{
    if( publisher_.get() )
        publisher_->Send( message );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::UpdateHandlers
// Created: LGY 2011-05-19
// -----------------------------------------------------------------------------
template< typename T >
void SwordClient::UpdateHandlers( const T& message )
{
    if( loggedIn_ )
        for( T_Handlers::iterator it = messageHandlers_.begin(); it != messageHandlers_.end(); ++it )
            (*it)->OnReceiveMessage( message );
}
