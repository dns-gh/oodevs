// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "ServerNetworker_ABC.h"

#include "DIN/MessageService/DIN_MessageServiceUserCbk.h"
#include "DIN/ConnectionService/DIN_ConnectionServiceServerUserCbk.h"

using namespace network;
using namespace DIN;
using namespace NEK;

// -----------------------------------------------------------------------------
// Name: ServerNetworker_ABC constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ServerNetworker_ABC::ServerNetworker_ABC( unsigned short port )
    : dinEngine_        ( new DIN_Engine() )
    , connectionService_( new DIN_ConnectionServiceServerUserCbk< ServerNetworker_ABC >( *this, *dinEngine_, DIN_ConnectorHost(), DIN_ConnectionProtocols( NEK_Protocols::eTCP, NEK_Protocols::eIPv4 ), 1  ) )
    , messageService_   ( new DIN_MessageServiceUserCbk         < ServerNetworker_ABC >( *this, *dinEngine_, DIN_ConnectorHost() ) )
    , serverAddress_    ( new NEK_AddressINET( port ) )
    , pServer_          ( 0 )
{
    messageService_->SetCbkOnError( &ServerNetworker_ABC::OnErrorReceivingMessage );

    connectionService_->SetCbkOnConnectionReceived( &ServerNetworker_ABC::OnConnectionReceived );
    connectionService_->SetCbkOnConnectionFailed  ( &ServerNetworker_ABC::OnConnectionFailed   );
    connectionService_->SetCbkOnConnectionLost    ( &ServerNetworker_ABC::OnConnectionLost     );
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker_ABC destructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ServerNetworker_ABC::~ServerNetworker_ABC()
{
    DenyConnections();
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ServerNetworker_ABC::Update
// Created: NLD 2007-01-24
// -----------------------------------------------------------------------------
void ServerNetworker_ABC::Update()
{
    dinEngine_->Update();
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker_ABC::DenyConnections
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void ServerNetworker_ABC::DenyConnections()
{
    if( pServer_ )
    {
        connectionService_->DestroyHost( *pServer_ );
        pServer_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker_ABC::AllowConnections
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void ServerNetworker_ABC::AllowConnections()
{
    if( pServer_ )
        DenyConnections();
    pServer_ = &connectionService_->CreateHost( *serverAddress_ );
}

// =============================================================================
// CONNECTION CALLBACKS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ServerNetworker_ABC::OnConnectionReceived
// Created: NLD 2002-07-12
// -----------------------------------------------------------------------------
void ServerNetworker_ABC::OnConnectionReceived( DIN_Server& /*server*/, DIN_Link& link )
{
    MT_LOG_INFO_MSG( MT_FormatString( "Connection received from client '%s'", link.GetRemoteAddress().GetAddressAsString().c_str() ) );
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker_ABC::OnConnectionFailed
// Created: NLD 2002-07-12
// -----------------------------------------------------------------------------
void ServerNetworker_ABC::OnConnectionFailed( DIN_Server& /*server*/, const NEK_Address_ABC& address, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( MT_FormatString( "Bad connection received from client '%s' (reason : %s)", address.GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: ServerNetworker_ABC::OnConnectionLost
// Created: NLD 2002-07-12
// -----------------------------------------------------------------------------
void ServerNetworker_ABC::OnConnectionLost( DIN_Server& /*server*/, DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( MT_FormatString( "Connection to client '%s' lost (reason : %s)", link.GetRemoteAddress().GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );
}

// =============================================================================
// MESSAGES CALLBACKS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ServerNetworker_ABC::OnErrorReceivingMessage
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
bool ServerNetworker_ABC::OnErrorReceivingMessage( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& info )
{
    MT_LOG_INFO_MSG( MT_FormatString( "Error while receiving message from client '%s' : %s", link.GetRemoteAddress().GetAddressAsString().c_str(), info.GetInfo().c_str() ).c_str() );
    return false;    
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DIN::DIN_MessageServiceUserCbk< ServerNetworker_ABC >& ServerNetworker_ABC::GetMessageService
// Created: NLD 2007-01-24
// -----------------------------------------------------------------------------
DIN::DIN_MessageServiceUserCbk< ServerNetworker_ABC >& ServerNetworker_ABC::GetMessageService() const
{
    return *messageService_;
}
