// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "ClientsNetworker.h"
#include "Client.h"
#include "Config.h"
#include "Plugin_ABC.h"

using namespace dispatcher;

static const unsigned int magicCookie_ = 10;

// -----------------------------------------------------------------------------
// Name: ClientsNetworkerc constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientsNetworker::ClientsNetworker( const Config& config, Plugin_ABC& plugin )
    : ServerNetworker( config.GetNetworkClientsParameters() )
    , plugin_( plugin )
{
    DenyConnections();
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
void ClientsNetworker::Receive( const ASN1T_MsgsSimToClient& message )
{
    if( message.msg.t == T_MsgsSimToClient_msg_msg_control_send_current_state_begin )
        DenyConnections();
    else if( message.msg.t == T_MsgsSimToClient_msg_msg_control_send_current_state_end )
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
    ServerNetworker::ConnectionSucceeded( link  );
    clients_[link] = new Client( *this, link  );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::ConnectionError
// Created: AGE 2007-09-05
// -----------------------------------------------------------------------------
void ClientsNetworker::ConnectionError( const std::string& link , const std::string& reason )
{
    ServerNetworker::ConnectionError( link, reason );
    T_Clients::iterator it = clients_.find( link );
    if( it != clients_.end() && it->second )
    {
        Client* client = it->second;
        plugin_.NotifyClientLeft( *client);
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
void ClientsNetworker::Send( const ASN1T_MsgsSimToClient& asnMsg )
{
    for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
        it->second->Send( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const ASN1T_MsgsAuthenticationToClient& asnMsg )
{
    for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
        it->second->Send( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const ASN1T_MsgsReplayToClient& asnMsg )
{
    for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
        it->second->Send( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const ASN1T_MsgsAarToClient& asnMsg )
{
    for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
        it->second->Send( asnMsg );
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
