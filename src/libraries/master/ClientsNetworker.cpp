// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "master_pch.h"

#include "ClientsNetworker.h"

#include "Network_Def.h"
#include "Master.h"
#include "Client.h"
#include "tools/AsnMessageDecoder.h"
#include "tools/AsnMessageEncoder.h"
#include "xeumeuleu/xml.h"
#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"

using namespace master;
using namespace tools;
using namespace DIN;

namespace 
{
    unsigned short ReadPort( const std::string& configFile )
    {
        unsigned short port;
        xml::xifstream xis( configFile );
        xis >> xml::start( "master" )
                >> xml::start( "network" )
                        >> xml::attribute( "server", port );
        return port;
    }
}

static const unsigned int magicCookie_ = 4242;

// -----------------------------------------------------------------------------
// Name: ClientsNetworker constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientsNetworker::ClientsNetworker( Master& master, const std::string& configFile )
    : ServerNetworker_ABC( magicCookie_, ReadPort( configFile ) )
    , master_            ( master )
    , clients_           ()
{
    RegisterMessage( *this, &ClientsNetworker::OnReceiveMsgInMaster );
    AllowConnections();
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

    Client* pClient = new Client( master_, GetMessageService(), link );
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
// MESSAGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::OnReceiveMsgInMaster
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void ClientsNetworker::OnReceiveMsgInMaster( DIN::DIN_Link& linkFrom, const ASN1T_MsgsInMaster& message )
{
    try
    {
        Client::GetClientFromLink( linkFrom ).OnReceive( message );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception catched: " << exception.what() );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: NLD 2007-01-31
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const ASN1T_MsgsOutMaster& asnMsg )
{
    try
    {
        AsnMessageEncoder< ASN1T_MsgsOutMaster, ASN1C_MsgsOutMaster > asnEncoder( GetMessageService(), asnMsg );
        for( CIT_ClientSet it = clients_.begin(); it != clients_.end(); ++it )
            (**it).Send( asnMsg, asnEncoder.GetDinMsg() );
    }
    catch( std::runtime_error& exception )
    {
        MT_LOG_ERROR_MSG( "exception catched: " << exception.what() );
    }
}
