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
    : ServerNetworker( ReadPort( configFile ) )
    , master_        ( master )
    , clients_       ()
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
    ServerNetworker::DenyConnections();
    for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
    {
        Disconnect( it->first );
        delete it->second;
    }
    clients_.clear();
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::ConnectionSucceeded
// Created: AGE 2007-09-07
// -----------------------------------------------------------------------------
void ClientsNetworker::ConnectionSucceeded( const std::string& endpoint )
{
    ServerNetworker::ConnectionSucceeded( endpoint );
    clients_[ endpoint ] = new Client( master_, *this, endpoint );;
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::ConnectionError
// Created: AGE 2007-09-07
// -----------------------------------------------------------------------------
void ClientsNetworker::ConnectionError( const std::string& address, const std::string& error )
{
    ServerNetworker::ConnectionError( address, error );
    delete clients_[ address ];
    clients_.erase( address );
}

// =============================================================================
// MESSAGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::OnReceiveMsgInMaster
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
void ClientsNetworker::OnReceiveMsgInMaster( const std::string& from, const ASN1T_MsgsInMaster& message )
{
    Client* client = clients_[ from ];
    if( client )
        client->OnReceive( message );
}

// -----------------------------------------------------------------------------
// Name: ClientsNetworker::Send
// Created: NLD 2007-01-31
// -----------------------------------------------------------------------------
void ClientsNetworker::Send( const ASN1T_MsgsOutMaster& asnMsg )
{
    for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
        it->second->Send( asnMsg );
}
