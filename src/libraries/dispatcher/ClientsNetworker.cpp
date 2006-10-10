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
#include "AsnMessageDecoder.h"
#include "AsnMessageEncoder.h"

using namespace dispatcher;
using namespace DIN;
using namespace NEK;

// -----------------------------------------------------------------------------
// Name: ClientsNetworker constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientsNetworker::ClientsNetworker( Dispatcher& dispatcher, unsigned short nPort )
    : Networker_ABC     ( dispatcher )
    , serverAddress_    ( nPort )
    , connectionService_( *this, dinEngine_, DIN_ConnectorHost(), DIN_ConnectionProtocols( NEK_Protocols::eTCP, NEK_Protocols::eIPv4 ), 1 )
    , messageService_   ( *this, dinEngine_, DIN_ConnectorHost() )
    , pServer_          ( 0 )
    , clients_          ()
{
    messageService_.SetCbkOnError( &ClientsNetworker::OnErrorReceivingMessage );
    messageService_.RegisterReceivedMessage( eMsgOutClient             , *this, &ClientsNetworker::OnReceiveMsgOutClient              );
    messageService_.RegisterReceivedMessage( eMsgEnableUnitVisionCones , *this, &ClientsNetworker::OnReceiveMsgEnableUnitVisionCones  );
    messageService_.RegisterReceivedMessage( eMsgDisableUnitVisionCones, *this, &ClientsNetworker::OnReceiveMsgDisableUnitVisionCones );
    messageService_.RegisterReceivedMessage( eMsgEnableProfiling       , *this, &ClientsNetworker::OnReceiveMsgEnableProfiling        );
    messageService_.RegisterReceivedMessage( eMsgDisableProfiling      , *this, &ClientsNetworker::OnReceiveMsgDisableProfiling       );
    messageService_.RegisterReceivedMessage( eMsgUnitMagicAction       , *this, &ClientsNetworker::OnReceiveMsgUnitMagicAction        );
    messageService_.RegisterReceivedMessage( eMsgDebugDrawPoints       , *this, &ClientsNetworker::OnReceiveMsgDebugDrawPoints        );

    connectionService_.SetCbkOnConnectionReceived( &ClientsNetworker::OnConnectionReceived    );
    connectionService_.SetCbkOnConnectionFailed  ( &ClientsNetworker::OnBadConnectionReceived );
    connectionService_.SetCbkOnConnectionLost    ( &ClientsNetworker::OnConnectionLost        );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker destructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientsNetworker::~ClientsNetworker()
{
    DenyConnections();
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::DenyConnections
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void ClientsNetworker::DenyConnections()
{
    if( pServer_ )
    {
        connectionService_.DestroyHost( *pServer_ );
        pServer_ = 0;
    }

    for( CIT_ClientSet it = clients_.begin(); it != clients_.end(); ++it )
        (**it).Disconnect();
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::AllowConnections
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void ClientsNetworker::AllowConnections()
{
    if( pServer_ )
        DenyConnections();

    pServer_ = &connectionService_.CreateHost( serverAddress_ );
}

// =============================================================================
// CONNECTION CALLBACKS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::OnConnectionReceived
// Created: NLD 2002-07-12
// -----------------------------------------------------------------------------
void ClientsNetworker::OnConnectionReceived( DIN_Server& /*server*/, DIN_Link& link )
{
    MT_LOG_INFO_MSG( MT_FormatString( "Connection received from client '%s'", link.GetRemoteAddress().GetAddressAsString().c_str() ) );

    Client* pClient = new Client( dispatcher_, messageService_, link );
    clients_.insert( pClient );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::OnBadConnectionReceived
// Created: NLD 2002-07-12
// -----------------------------------------------------------------------------
void ClientsNetworker::OnBadConnectionReceived( DIN_Server& /*server*/, const NEK_Address_ABC& address, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( MT_FormatString( "Bad connection received from client '%s' (reason : %s)", address.GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::OnConnectionLost
// Created: NLD 2002-07-12
// -----------------------------------------------------------------------------
void ClientsNetworker::OnConnectionLost( DIN_Server& /*server*/, DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( MT_FormatString( "Connection to client '%s' lost (reason : %s)", link.GetRemoteAddress().GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );

    Client& client = Client::GetClientFromLink( link );
    clients_.erase( &client );
    delete &client;
}

// =============================================================================
// RECEIVED MESSAGES
// =============================================================================

#define DECLARE_DIN_CALLBACK( MSG )                                                           \
    void ClientsNetworker::OnReceiveMsg##MSG( DIN::DIN_Link& linkFrom, DIN::DIN_Input& msg )  \
    {                                                                                         \
        Client::GetClientFromLink( linkFrom ).OnReceive( eMsg##MSG, msg );                    \
    }

DECLARE_DIN_CALLBACK( EnableUnitVisionCones  )
DECLARE_DIN_CALLBACK( DisableUnitVisionCones )
DECLARE_DIN_CALLBACK( EnableProfiling        )
DECLARE_DIN_CALLBACK( DisableProfiling       )
DECLARE_DIN_CALLBACK( UnitMagicAction        )
DECLARE_DIN_CALLBACK( DebugDrawPoints        )

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::OnErrorReceivingMessage
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
bool ClientsNetworker::OnErrorReceivingMessage( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& info )
{
    MT_LOG_INFO_MSG( MT_FormatString( "Error while receiving message from client '%s' : %s", link.GetRemoteAddress().GetAddressAsString().c_str(), info.GetInfo().c_str() ).c_str() );
    return false;    
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::OnReceiveMsgOutClient
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void ClientsNetworker::OnReceiveMsgOutClient( DIN::DIN_Link& linkFrom, DIN::DIN_Input& input )
{
    try
    {
        AsnMessageDecoder< ASN1T_MsgsOutClient, ASN1C_MsgsOutClient > asnDecoder( input );
        Client::GetClientFromLink( linkFrom ).OnReceive( asnDecoder.GetAsnMsg() );
    }
    catch( std::runtime_error& /*exception*/ )
    {
    }
}

// =============================================================================
// SENT MESSAGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Dispatch
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void ClientsNetworker::Dispatch( const ASN1T_MsgsInClient& asnMsg )
{
    try
    {
        AsnMessageEncoder< ASN1T_MsgsInClient, ASN1C_MsgsInClient > asnEncoder( messageService_, asnMsg );
        for( CIT_ClientSet it = clients_.begin(); it != clients_.end(); ++it )
            (**it).Send( asnMsg, asnEncoder.GetDinMsg() );
    }
    catch( std::runtime_error& /*exception*/ )
    {
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Dispatch
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void ClientsNetworker::Dispatch( unsigned int nMsgID, const DIN::DIN_Input& dinMsg )
{
    DIN_BufferedMessage copiedMsg( messageService_ );
    copiedMsg.GetOutput().Append( dinMsg.GetBuffer( 0 ), dinMsg.GetAvailable() );

    for( CIT_ClientSet it = clients_.begin(); it != clients_.end(); ++it )
        (**it).Send( nMsgID, copiedMsg );
}
