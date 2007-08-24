// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "ClientNetworker_ABC.h"
#include "DIN/ConnectionService/DIN_ConnectionServiceClientUserCbk.h"
#include <boost/bind.hpp>

using namespace tools;
using namespace DIN;
using namespace NEK;

namespace
{
    NEK::NEK_AddressINET* BuildAddress( const std::string& host )
    {
        return new NEK::NEK_AddressINET( host.substr( 0, host.find_first_of( ':' ) ), (uint16)atoi( host.substr( host.find_first_of( ':' ) + 1 ).c_str() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker_ABC constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientNetworker_ABC::ClientNetworker_ABC( unsigned int magicCookie, const std::string& host /* = "" */ )
    : dinEngine_        ( new DIN_Engine() )
    , connectionService_( new DIN_ConnectionServiceClientUserCbk< ClientNetworker_ABC >( *this, *dinEngine_, DIN_ConnectorGuest(), DIN_ConnectionProtocols( NEK_Protocols::eTCP, NEK_Protocols::eIPv4 ), magicCookie ) )
    , messageService_   ( new ObjectMessageService( *dinEngine_, DIN_ConnectorGuest() ) )   
    , serverAddress_    ( 0 )
    , session_          ( 0 )
{ 
    messageService_   ->SetCallbackOnError( boost::bind( &ClientNetworker_ABC::OnErrorReceivingMessage, this, _1, _2 ) );
    connectionService_->SetCbkOnConnectionSuccessful( &ClientNetworker_ABC::OnConnected        );
    connectionService_->SetCbkOnConnectionFailed    ( &ClientNetworker_ABC::OnConnectionFailed );
    connectionService_->SetCbkOnConnectionLost      ( &ClientNetworker_ABC::OnConnectionLost   );

    if( !host.empty() )
    {
        serverAddress_.reset( BuildAddress( host ) );
        connectionService_->JoinHost( *serverAddress_ );
    }
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker_ABC destructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
ClientNetworker_ABC::~ClientNetworker_ABC()
{
    // NOTHING
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ClientNetworker_ABC::Connect
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
bool ClientNetworker_ABC::Connect( const std::string& host )
{
    if( IsConnected() )
        return false;
    
    serverAddress_.reset( BuildAddress( host ) );
    connectionService_->JoinHost( *serverAddress_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker_ABC::Disconnect
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
bool ClientNetworker_ABC::Disconnect()
{
    if( ! IsConnected() )
        return false;
    session_->Close( false );
    return true;
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker_ABC::Update
// Created: NLD 2007-01-24
// -----------------------------------------------------------------------------
void ClientNetworker_ABC::Update()
{
    dinEngine_->Update();
}
  
// =============================================================================
// CONNECTION CALLBACKS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ClientNetworker_ABC::OnConnected
// Created: AML 03-04-02
// -----------------------------------------------------------------------------
void ClientNetworker_ABC::OnConnected( DIN_Link& link )
{
    MT_LOG_INFO_MSG( "Connected to '" << link.GetRemoteAddress().GetAddressAsString() << "'" );
    session_ = &link;
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker_ABC::OnConnectionFailed
// Created: AML 03-04-02
// -----------------------------------------------------------------------------
void ClientNetworker_ABC::OnConnectionFailed( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( "Connection to '" << link.GetRemoteAddress().GetAddressAsString() << "' failed (reason : " << reason.GetInfo() << ")" );
    connectionService_->JoinHost( *serverAddress_ );
    session_ = 0;
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker_ABC::OnConnectionLost
// Created: AML 03-04-02
// -----------------------------------------------------------------------------
void ClientNetworker_ABC::OnConnectionLost( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( "Connection to '" << link.GetRemoteAddress().GetAddressAsString() << "' lost (reason : " << reason.GetInfo() << ") - reconnecting" );    
    connectionService_->JoinHost( *serverAddress_ );
    session_ = 0;
}

// =============================================================================
// MESSAGES CALLBACKS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ClientNetworker_ABC::OnErrorReceivingMessage
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
bool ClientNetworker_ABC::OnErrorReceivingMessage( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& info )
{
    MT_LOG_INFO_MSG( "Error while receiving message from simulation '" << link.GetRemoteAddress().GetAddressAsString() << "' : " << info.GetInfo() );
    return false;    
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ClientNetworker_ABC::GetMessageService
// Created: NLD 2007-01-24
// -----------------------------------------------------------------------------
ObjectMessageService& ClientNetworker_ABC::GetMessageService() const
{
    return *messageService_;
}

// -----------------------------------------------------------------------------
// Name: ClientNetworker_ABC::IsConnected
// Created: SBO 2007-01-26
// -----------------------------------------------------------------------------
bool ClientNetworker_ABC::IsConnected() const
{
    return session_ != 0;
}
