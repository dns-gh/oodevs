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
#include "network/AsnMessageDecoder.h"
#include "network/AsnMessageEncoder.h"
#include "xeumeuleu/xml.h"
#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"

using namespace dispatcher;
using namespace network;
using namespace DIN;

namespace 
{
    unsigned short ReadPort( const std::string& configFile )
    {
        unsigned short port;
        xml::xifstream xis( configFile );
        xis >> xml::start( "config" )
                >> xml::start( "dispatcher" )
                    >> xml::start( "network" )
                        >> xml::attribute( "server", port );
        return port;
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientsNetworker::ClientsNetworker( Dispatcher& dispatcher, const std::string& configFile )
    : ServerNetworker_ABC( ReadPort( configFile ) )
    , dispatcher_        ( dispatcher )
    , clients_          ()
{
    GetMessageService().RegisterReceivedMessage( eMsgOutClient             , *this, &ClientsNetworker::OnReceiveMsgOutClient              );
    GetMessageService().RegisterReceivedMessage( eMsgEnableUnitVisionCones , *this, &ClientsNetworker::OnReceiveMsgEnableUnitVisionCones  );
    GetMessageService().RegisterReceivedMessage( eMsgDisableUnitVisionCones, *this, &ClientsNetworker::OnReceiveMsgDisableUnitVisionCones );
    GetMessageService().RegisterReceivedMessage( eMsgEnableProfiling       , *this, &ClientsNetworker::OnReceiveMsgEnableProfiling        );
    GetMessageService().RegisterReceivedMessage( eMsgDisableProfiling      , *this, &ClientsNetworker::OnReceiveMsgDisableProfiling       );
    GetMessageService().RegisterReceivedMessage( eMsgUnitMagicAction       , *this, &ClientsNetworker::OnReceiveMsgUnitMagicAction        );
    GetMessageService().RegisterReceivedMessage( eMsgDebugDrawPoints       , *this, &ClientsNetworker::OnReceiveMsgDebugDrawPoints        );
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

    Client* pClient = new Client( dispatcher_, GetMessageService(), link );
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
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception catched: " << exception.what() );
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
        AsnMessageEncoder< ASN1T_MsgsInClient, ASN1C_MsgsInClient > asnEncoder( GetMessageService(), asnMsg );
        for( CIT_ClientSet it = clients_.begin(); it != clients_.end(); ++it )
            (**it).Send( asnMsg, asnEncoder.GetDinMsg() );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception catched: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Dispatch
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
void ClientsNetworker::Dispatch( unsigned int nMsgID, const DIN::DIN_Input& dinMsg )
{
    DIN_BufferedMessage copiedMsg( GetMessageService() );
    copiedMsg.GetOutput().Append( dinMsg.GetBuffer( 0 ), dinMsg.GetAvailable() );

    for( CIT_ClientSet it = clients_.begin(); it != clients_.end(); ++it )
        (**it).Send( nMsgID, copiedMsg );
}
