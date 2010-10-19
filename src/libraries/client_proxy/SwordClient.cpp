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
#include "protocol/ProtocolVersionChecker.h"
#include <boost/lexical_cast.hpp>

// -----------------------------------------------------------------------------
// Name: SwordClient constructor
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
SwordClient::SwordClient( const std::string& host, unsigned short port, const std::string& profile )
    : tools::ClientNetworker( "", false )
    , host_( host )
    , port_( port )
    , profile_( profile )
    , loggedIn_( false )
    , connectionHandler_( new ConnectionHandlerProxy() )
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
        message().mutable_version()->set_value( ProtocolVersionChecker::GetCurrentProtocolVersion() );
        message().set_login( profile_ );
        message().set_password( "" );
        message.Send( *publisher_ );
    }
}

// -----------------------------------------------------------------------------
// Name: SwordClient::ConnectionFailed
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::ConnectionFailed( const std::string& endpoint, const std::string& error )
{
    connectionHandler_->OnConnectionFailed( endpoint, error );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::ConnectionError
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::ConnectionError( const std::string& endpoint, const std::string& error )
{
    connectionHandler_->OnConnectionError( endpoint, error );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::HandleAarToClient
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::HandleAarToClient( const std::string& /*endpoint*/, const MsgsAarToClient::MsgAarToClient& /*message*/ )
{
    // NOTHING
}

namespace
{
    std::string ToString( const MsgsAuthenticationToClient::MsgAuthenticationResponse::ErrorCode& code )
    {
        if( code == MsgsAuthenticationToClient::MsgAuthenticationResponse::invalid_login )
            return "invalid login";
        if( code == MsgsAuthenticationToClient::MsgAuthenticationResponse::too_many_connections )
            return "too many connections";
        if( code == MsgsAuthenticationToClient::MsgAuthenticationResponse::mismatched_protocol_version )
            return "mismatched protocol version";
        return "unknown error";
    }
}

// -----------------------------------------------------------------------------
// Name: SwordClient::HandleAuthenticationToClient
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::HandleAuthenticationToClient( const std::string& /*endpoint*/, const MsgsAuthenticationToClient::MsgAuthenticationToClient& message )
{
    if( message.message().has_authentication_response() )
    {
        const std::string login( message.message().authentication_response().profile().login() );
        loggedIn_ = message.message().authentication_response().error_code() == MsgsAuthenticationToClient::MsgAuthenticationResponse::success;
        if( loggedIn_ )
            connectionHandler_->OnAuthenticationSucceeded( login );
        else
            connectionHandler_->OnAuthenticationFailed( login, ToString( message.message().authentication_response().error_code() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: SwordClient::HandleDispatcherToClient
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::HandleDispatcherToClient( const std::string& /*endpoint*/, const MsgsDispatcherToClient::MsgDispatcherToClient& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SwordClient::HandleMessengerToClient
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::HandleMessengerToClient( const std::string& /*endpoint*/, const MsgsMessengerToClient::MsgMessengerToClient& message )
{
    if( loggedIn_ )
        for( T_Handlers::iterator it = messageHandlers_.begin(); it != messageHandlers_.end(); ++it )
            (*it)->OnReceiveMessage( message );
}

// -----------------------------------------------------------------------------
// Name: SwordClient::HandleReplayToClient
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::HandleReplayToClient( const std::string& /*endpoint*/, const MsgsReplayToClient::MsgReplayToClient& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SwordClient::HandleSimToClient
// Created: SEB 2010-10-12
// -----------------------------------------------------------------------------
void SwordClient::HandleSimToClient( const std::string& /*endpoint*/, const MsgsSimToClient::MsgSimToClient& message )
{
    if( loggedIn_ )
        for( T_Handlers::iterator it = messageHandlers_.begin(); it != messageHandlers_.end(); ++it )
            (*it)->OnReceiveMessage( message );
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
void SwordClient::SendMessage( const MsgsClientToSim::MsgClientToSim& message ) const
{
    if( publisher_.get() )
        publisher_->Send( message );
}
