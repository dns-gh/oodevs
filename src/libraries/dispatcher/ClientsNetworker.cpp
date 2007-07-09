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

#include "Network_Def.h"
#include "Dispatcher.h"
#include "Client.h"
#include "Replayer.h"
#include "Loader.h"
#include "Config.h"
#include "tools/AsnMessageDecoder.h"
#include "tools/AsnMessageEncoder.h"
#include "xeumeuleu/xml.h"
#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"

using namespace dispatcher;
using namespace tools;
using namespace DIN;

static const unsigned int magicCookie_ = 10;

// -----------------------------------------------------------------------------
// Name: ClientsNetworkerc constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientsNetworker::ClientsNetworker( Dispatcher& dispatcher, const Config& config )
    : ServerNetworker_ABC( magicCookie_, config.GetNetworkClientsParameters() )
    , dispatcher_        ( &dispatcher )
    , replayer_          ( 0 )
{
    GetMessageService().RegisterReceivedMessage( eMsgClientToSim           , *this, &ClientsNetworker::OnReceiveMsgClientToSim            );
    GetMessageService().RegisterReceivedMessage( eMsgClientToMiddle        , *this, &ClientsNetworker::OnReceiveMsgClientToMiddle         );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker constructor
// Created: AGE 2007-04-10
// -----------------------------------------------------------------------------
ClientsNetworker::ClientsNetworker( Replayer& replayer, const Config& config )
    : ServerNetworker_ABC( magicCookie_, config.GetNetworkClientsParameters() )
    , dispatcher_        ( 0 )
    , replayer_          ( &replayer )
{
    GetMessageService().RegisterReceivedMessage( eMsgClientToSim           , *this, &ClientsNetworker::OnReceiveMsgClientToSim            );
    GetMessageService().RegisterReceivedMessage( eMsgClientToMiddle        , *this, &ClientsNetworker::OnReceiveMsgClientToMiddle         );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker destructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientsNetworker::~ClientsNetworker()
{
    // NOTHING
}

// =============================================================================
// MAIN
// =============================================================================

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

    Client* pClient = 0;
    // $$$$ AGE 2007-07-05: n'importe quoi
    if( dispatcher_ )
        pClient = new Client( dispatcher_->GetModel(), dispatcher_->GetProfileManager(), dispatcher_->GetSimulationNetworker(), GetMessageService(), link );
    else
        pClient = new Client( replayer_->GetModel(), replayer_->GetProfiles(), replayer_->GetLoader(), GetMessageService(), link );
    clients_.insert( pClient );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::OnConnectionLost
// Created: NLD 2002-07-12
// -----------------------------------------------------------------------------
void ClientsNetworker::OnConnectionLost( DIN_Server& server, DIN_Link& link, const DIN_ErrorDescription& reason )
{
    ServerNetworker_ABC::OnConnectionLost( server, link, reason );

    Client& client = Client::GetClientFromLink( link );
    clients_.erase( &client );
    delete &client;
}

// =============================================================================
// RECEIVED MESSAGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::OnReceiveMsgClientToSim
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void ClientsNetworker::OnReceiveMsgClientToSim( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input )
{
    try
    {
        AsnMessageDecoder< ASN1T_MsgsClientToSim, ASN1C_MsgsClientToSim > asnDecoder( input );
        Client::GetClientFromLink( linkFrom ).OnReceive( asnDecoder.GetAsnMsg() );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception catched: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::OnReceiveMsgClientToMiddle
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void ClientsNetworker::OnReceiveMsgClientToMiddle( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input )
{
    try
    {
        AsnMessageDecoder< ASN1T_MsgsClientToMiddle, ASN1C_MsgsClientToMiddle > asnDecoder( input );
        Client::GetClientFromLink( linkFrom ).OnReceive( asnDecoder.GetAsnMsg() );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception catched: " << exception.what() );
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
    try
    {
        AsnMessageEncoder< ASN1T_MsgsSimToClient, ASN1C_MsgsSimToClient > asnEncoder( GetMessageService(), asnMsg );
        for( CIT_ClientSet it = clients_.begin(); it != clients_.end(); ++it )
            (**it).Send( asnMsg, asnEncoder.GetDinMsg() );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception catched: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: NLD 2007-04-24
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const ASN1T_MsgsMiddleToClient& asnMsg )
{
    try
    {
        AsnMessageEncoder< ASN1T_MsgsMiddleToClient, ASN1C_MsgsMiddleToClient > asnEncoder( GetMessageService(), asnMsg );
        for( CIT_ClientSet it = clients_.begin(); it != clients_.end(); ++it )
            (**it).Send( asnMsg, asnEncoder.GetDinMsg() );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception catched: " << exception.what() );
    }
}
