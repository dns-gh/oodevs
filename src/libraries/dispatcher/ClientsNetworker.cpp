// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "ClientsNetworker.h"
#include "Client.h"
#include "Config.h"
#include "Plugin_ABC.h"
#include "Services.h"
#include <MT/MT_Logger/MT_Logger_lib.h>
#include "protocol/protocol.h"

using namespace dispatcher;

static const unsigned int magicCookie_ = 10;

// -----------------------------------------------------------------------------
// Name: ClientsNetworkerc constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientsNetworker::ClientsNetworker( const Config& config, Plugin_ABC& plugin, const Services& services )
    : ServerNetworker( config.GetNetworkClientsParameters() )
    , plugin_( plugin )
    , services_( services )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker destructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientsNetworker::~ClientsNetworker()
{
    for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Receive
// Created: AGE 2007-07-09
// -----------------------------------------------------------------------------
void ClientsNetworker::Receive( const MsgsSimToClient::MsgSimToClient& wrapper )
{
    if( wrapper.message().has_control_send_current_state_begin() )
        DenyConnections();
    else if( wrapper.message().has_control_send_current_state_end() )
        AllowConnections();
}

// =============================================================================
// CONNECTION CALLBACKS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::ConnectionSucceeded
// Created: NLD 2002-07-12
// -----------------------------------------------------------------------------
void ClientsNetworker::ConnectionSucceeded( const std::string& link  )
{
    MT_LOG_INFO_MSG( "Connection received from client '" << link << "'" );
    ServerNetworker::ConnectionSucceeded( link  );
    Client* newClient = clients_[link] = new Client( *this, link );
    services_.Send( *newClient );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::ConnectionFailed
// Created: AGE 2008-03-13
// -----------------------------------------------------------------------------
void ClientsNetworker::ConnectionFailed( const std::string& address, const std::string& error )
{
    MT_LOG_INFO_MSG( "Bad connection received from client '" << address << "' (" << error << ")" );
    ServerNetworker::ConnectionFailed( address, error );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::ConnectionError
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
void ClientsNetworker::ConnectionError( const std::string& link , const std::string& reason )
{
    MT_LOG_INFO_MSG( "Connection to '" << link << "' lost (" << reason << ")" );
    ServerNetworker::ConnectionError( link, reason );
    T_Clients::iterator it = clients_.find( link );
    if( it != clients_.end() && it->second )
    {
        Client* client = it->second;
        plugin_.NotifyClientLeft( *client );
        clients_.erase( it );
        delete client;
    }
}

// =============================================================================
// SENT MESSAGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const MsgsSimToClient::MsgSimToClient& asnMsg )
{
    try
    {
        for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
            it->second->Send( asnMsg );
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const MsgsAuthenticationToClient::MsgAuthenticationToClient& asnMsg )
{
    try
    {
        for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
            it->second->Send( asnMsg );
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const MsgsReplayToClient::MsgReplayToClient& asnMsg )
{
    try
    {
        for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
            it->second->Send( asnMsg );
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const MsgsAarToClient::MsgAarToClient& asnMsg )
{
    try
    {
        for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
            it->second->Send( asnMsg );
        plugin_.Receive( asnMsg );
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: RDS 2008-04-04
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const MsgsMessengerToClient::MsgMessengerToClient& asnMsg )
{
    try
    {
        for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
            it->second->Send( asnMsg );
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const MsgsDispatcherToClient::MsgDispatcherToClient& msg )
{
    try
    {
        for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
            it->second->Send( msg );
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( "exception caught: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::GetProfile
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
Profile_ABC& ClientsNetworker::GetProfile( const std::string& )
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::GetPublisher
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
ClientPublisher_ABC& ClientsNetworker::GetPublisher( const std::string& link )
{
    CIT_Clients it = clients_.find( link );
    if( it == clients_.end() || !it->second )
        throw std::runtime_error( link + " is not a valid client" );
    return *it->second;
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::GetEndpoint
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
std::string ClientsNetworker::GetEndpoint() const
{
    return "";
}
