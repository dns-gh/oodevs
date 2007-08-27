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
using namespace DIN;

static const unsigned int magicCookie_ = 10;

// -----------------------------------------------------------------------------
// Name: ClientsNetworkerc constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientsNetworker::ClientsNetworker( const Config& config, Plugin_ABC& plugin )
    : ServerNetworker_ABC( magicCookie_, config.GetNetworkClientsParameters() )
    , plugin_( plugin )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker destructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientsNetworker::~ClientsNetworker()
{
    // NOTHING
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

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::DenyConnections
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void ClientsNetworker::DenyConnections()
{
    ServerNetworker_ABC::DenyConnections();
    for( CIT_ClientSet it = clients_.begin(); it != clients_.end(); ++it )
        (**it).Disconnect();
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::AllowConnections
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void ClientsNetworker::AllowConnections()
{
    ServerNetworker_ABC::AllowConnections();
}

// =============================================================================
// CONNECTION CALLBACKS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::OnConnectionReceived
// Created: NLD 2002-07-12
// -----------------------------------------------------------------------------
void ClientsNetworker::OnConnectionReceived( DIN_Server& server, DIN_Link& link )
{
    ServerNetworker_ABC::OnConnectionReceived( server, link );
    clients_.insert( new Client( GetMessageService(), link ) );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::OnConnectionLost
// Created: NLD 2002-07-12
// -----------------------------------------------------------------------------
void ClientsNetworker::OnConnectionLost( DIN_Server& server, DIN_Link& link, const DIN_ErrorDescription& reason )
{
    ServerNetworker_ABC::OnConnectionLost( server, link, reason );
    Client& client = Client::GetClientFromLink( link );
    plugin_.NotifyClientLeft( client );
    clients_.erase( &client );
    delete &client;
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
    // $$$$ AGE 2007-08-24: perf issues ?
    for( CIT_ClientSet it = clients_.begin(); it != clients_.end(); ++it )
        (**it).Send( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const ASN1T_MsgsAuthenticationToClient& asnMsg )
{
    for( CIT_ClientSet it = clients_.begin(); it != clients_.end(); ++it )
        (**it).Send( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const ASN1T_MsgsReplayToClient& asnMsg )
{
    for( CIT_ClientSet it = clients_.begin(); it != clients_.end(); ++it )
        (**it).Send( asnMsg );
}
