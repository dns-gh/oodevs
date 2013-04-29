//*****************************************************************************
//
// $Created: SBO 2005-05-09 $
// $Archive: /MVW_v10/Build/SDK/TIC/src/Network/ConnectionHandler.cpp $
// $Author: Nld $
// $Modtime: 8/06/05 14:45 $
// $Revision: 5 $
// $Workfile: ConnectionHandler.cpp $
//
//*****************************************************************************

#include "simulation_tester_pch.h"

#include "ConnectionHandler.h"

#include "NetworkManager.h"
#include "Messages/ASN_Messages.h"

using namespace DIN;
using namespace NEK;
using namespace TEST;


//-----------------------------------------------------------------------------
// Name: ConnectionHandler constructor
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
ConnectionHandler::ConnectionHandler( NetworkManager& networkManager )
    : networkManager_   ( networkManager )
    , connectionService_( *this, networkManager_.GetDINEngine(), DIN_ConnectorGuest(), DIN_ConnectionProtocols( NEK_Protocols::eTCP, NEK_Protocols::eIPv4 ), 1 )
{
    connectionService_.SetCbkOnConnectionSuccessful( & ConnectionHandler::OnConnected        );
    connectionService_.SetCbkOnConnectionFailed    ( & ConnectionHandler::OnConnectionFailed );
    connectionService_.SetCbkOnConnectionLost      ( & ConnectionHandler::OnConnectionLost   );
}


//-----------------------------------------------------------------------------
// Name: ConnectionHandler destructor
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
ConnectionHandler::~ConnectionHandler()
{
}

//=============================================================================
// MAIN METHODS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: ConnectionHandler::StartServer
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
bool ConnectionHandler::ConnectToServer( const std::string& strHostName, unsigned int nPort )
{
    if( networkManager_.IsConnected() )
    {
        MT_LOG_ERROR_MSG( "Already Connected" );
        assert( false );
        return false;
    }

    NEK_AddressINET addr( strHostName.c_str(), ( uint16 )nPort );

    connectionService_.JoinHost( addr );
    return true;
}


//-----------------------------------------------------------------------------
// Name: ConnectionHandler::DisonnectFromServer
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
bool ConnectionHandler::DisconnectFromServer()
{
    if( ! networkManager_.IsConnected() )
    {
        MT_LOG_ERROR_MSG( "Unable to disconnect - not connected to server" );
        return false;
    }
    return true;
}


//=============================================================================
// CONNECTION SERVICE CALLBACKS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: ConnectionHandler::OnConnected
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
void ConnectionHandler::OnConnected( DIN_Link& link )
{
    MT_LOG_INFO_MSG( "Connected to " << link.GetRemoteAddress().GetAddressAsString() );

    // bind link
    networkManager_.BindLink( link );

    // bind message manager
    networkManager_.GetMessageMgr().Enable( link );

    MOS_ASN_MsgCtrlClientAnnouncement asnMsg;
    asnMsg.Send();
}


//-----------------------------------------------------------------------------
// Name: ConnectionHandler::OnConnectionFailed
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
void ConnectionHandler::OnConnectionFailed( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( "Connection to " << link.GetRemoteAddress().GetAddressAsString() << " failed (reason :" << reason.GetInfo() << ")" );
    throw std::exception( "Connection failed" );
}

//-----------------------------------------------------------------------------
// Name: ConnectionHandler::OnConnectionLost
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
void ConnectionHandler::OnConnectionLost( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( "Connection to " << link.GetRemoteAddress().GetAddressAsString() << " lost (reason: " << reason.GetInfo() << ")" );

    // unbind link
    networkManager_.UnbindLink();
    throw std::exception( "Connection lost" );
}