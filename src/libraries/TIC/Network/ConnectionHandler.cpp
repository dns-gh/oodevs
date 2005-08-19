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

#include "TIC_pch.h"

#include "ConnectionHandler.h"

#include "NetworkManager.h"
#include "Messages/ASN_Messages.h"

using namespace DIN;
using namespace NEK;
using namespace TIC;


//-----------------------------------------------------------------------------
// Name: ConnectionHandler constructor
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
ConnectionHandler::ConnectionHandler( NetworkManager& networkManager )
    : pNetworkManager_ ( &networkManager )
{
    DIN_ConnectionProtocols connProtocols( NEK_Protocols::eTCP, NEK_Protocols::eIPv4 );

	DIN_ConnectorGuest connector( eConnector_SIM_TIC ); // a voir

    pConnService_ = new DIN_ConnectionServiceClientUserCbk< ConnectionHandler >( 
                                      *this
                                    , pNetworkManager_->GetDINEngine()
                                    , connector
                                    , connProtocols
									, eConnMagicTICServerAgentServer /* a voir aussi */
                                    , "TIC Server to agent server"); 

    pConnService_->SetCbkOnConnectionSuccessful( & ConnectionHandler::OnConnected        );
    pConnService_->SetCbkOnConnectionFailed    ( & ConnectionHandler::OnConnectionFailed );
    pConnService_->SetCbkOnConnectionLost      ( & ConnectionHandler::OnConnectionLost   );
}


//-----------------------------------------------------------------------------
// Name: ConnectionHandler destructor
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
ConnectionHandler::~ConnectionHandler()
{
    delete pConnService_;
}

//=============================================================================
// MAIN METHODS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: ConnectionHandler::StartServer
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
bool ConnectionHandler::ConnectToServer( const std::string& strHostName, uint nPort )
{
    if( pNetworkManager_->IsConnected() )
    {
        MT_LOG_ERROR_MSG( "Already Connected" );
        assert( false );
        return false;
    }
    
    NEK_AddressINET addr( strHostName.c_str(), ( uint16 )nPort );

    pConnService_->JoinHost( addr );
    return true;
}


//-----------------------------------------------------------------------------
// Name: ConnectionHandler::DisonnectFromServer
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
bool ConnectionHandler::DisconnectFromServer()
{
    if( ! pNetworkManager_->IsConnected() )
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
    pNetworkManager_->BindLink( link );

    // bind message manager
	pNetworkManager_->GetMessageMgr().Enable( link );
    
	MOS_ASN_MsgCtrlClientAnnouncement asnMsg;
    asnMsg.GetAsnMsg() = MsgCtrlClientAnnouncement::mos_light;
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
    pNetworkManager_->UnbindLink();
    throw std::exception( "Connection lost" );
}
