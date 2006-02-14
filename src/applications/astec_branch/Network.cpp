// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Network.h"
#include "Net_Def.h"
#include "Types.h"
#include "AgentServerMsgMgr.h"
#include "App.h"
#include "ASN_Messages.h"

#include "DIN/DIN_Engine.h"
#include "DIN/ConnectionService/DIN_ConnectionServiceClientUserCbk.h"

using namespace DIN; using namespace NEK;

// -----------------------------------------------------------------------------
// Name: Network constructor
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
Network::Network( Model& model, Simulation& simu )
    : engine_( new DIN::DIN_Engine() )
    , manager_( new AgentServerMsgMgr( *engine_, model, simu ) )
    , session_( 0 )
{
    DIN_ConnectionProtocols connProtocols( NEK_Protocols::eTCP, NEK_Protocols::eIPv4 );

    DIN_ConnectorGuest connector( eConnector_SIM_MOS );

    pConnService_ = new DIN_ConnectionServiceClientUserCbk< Network >( 
                                      *this
                                    , *engine_
                                    , connector
                                    , connProtocols
                                    , eConnMagicMOSServerAgentServer
                                    , "MOS Server to agent server"); 

    pConnService_->SetCbkOnConnectionSuccessful( OnConnected      );
    pConnService_->SetCbkOnConnectionFailed    ( OnNotConnected   );
    pConnService_->SetCbkOnConnectionLost      ( OnConnectionLost );
}
    
// -----------------------------------------------------------------------------
// Name: Network destructor
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
Network::~Network()
{
    delete pConnService_;
    delete manager_;
    delete engine_;
}

// -----------------------------------------------------------------------------
// Name: Network::Connect
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
bool Network::Connect( const std::string& strHostName, uint16 nPort )
{
    if( IsConnected() )
    {
        MT_LOG_ERROR_MSG( "Déjà connecté" );
        assert( false );
        return false;
    }
    
    NEK_AddressINET addr( strHostName.c_str(), nPort );
    pConnService_->JoinHost( addr );
    return true;
}


// -----------------------------------------------------------------------------
// Name: Network::Disconnect
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
bool Network::Disconnect()
{
    if( ! IsConnected() )
    {
        MT_LOG_ERROR_MSG( "Déconnexion impossible - non connecté à un serveur" );
        return false;
    }
    session_->Close( false );
//    App::GetApp().NotifyConnexionStatusChanged( false );
    return true;
}

// -----------------------------------------------------------------------------
// Name: Network::IsConnected
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
bool Network::IsConnected() const
{
    return session_ != 0;
}

// -----------------------------------------------------------------------------
// Name: Network::Update
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void Network::Update()
{
    engine_->Update();
}

// -----------------------------------------------------------------------------
// Name: Network::GetMessageMgr
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
AgentServerMsgMgr& Network::GetMessageMgr()
{
    return *manager_;
}

// -----------------------------------------------------------------------------
// Name: Network::OnConnected
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void Network::OnConnected( DIN::DIN_Link& link )
{
    MT_LOG_INFO_MSG( "Connecté à " << link.GetRemoteAddress().GetAddressAsString() );
    session_ = &link;
    manager_->Enable( link );

    ASN_MsgCtrlClientAnnouncement asnMsg;
    asnMsg.GetAsnMsg() = MsgCtrlClientAnnouncement::mos_light;
    asnMsg.Send();
    
//    App::GetApp().NotifyConnexionStatusChanged( true );
}

// -----------------------------------------------------------------------------
// Name: Network::OnNotConnected
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void Network::OnNotConnected( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( "Non connecté à " << link.GetRemoteAddress().GetAddressAsString() << " (raison :" << reason.GetInfo() << ")" );   

//    App::GetApp().NotifyConnexionStatusChanged( false );
}

// -----------------------------------------------------------------------------
// Name: Network::OnConnectionLost
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void Network::OnConnectionLost( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( "Connexion à " << link.GetRemoteAddress().GetAddressAsString() << " perdue (raison: " << reason.GetInfo() << ")" );    
    session_ = 0;

//    App::GetApp().NotifyConnexionLost();
}
