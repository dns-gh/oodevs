// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_Network.h"
#include "MOS_Net_Def.h"
#include "MOS_Types.h"
#include "MOS_AgentServerMsgMgr.h"
#include "MOS_App.h"
#include "MOS_ASN_Messages.h"

#include "DIN/DIN_Engine.h"
#include "DIN/ConnectionService/DIN_ConnectionServiceClientUserCbk.h"

using namespace DIN; using namespace NEK;

// -----------------------------------------------------------------------------
// Name: MOS_Network constructor
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
MOS_Network::MOS_Network()
    : engine_( new DIN::DIN_Engine() )
    , manager_( new MOS_AgentServerMsgMgr( *engine_ ) )
    , session_( 0 )
{
    DIN_ConnectionProtocols connProtocols( NEK_Protocols::eTCP, NEK_Protocols::eIPv4 );

    DIN_ConnectorGuest connector( eConnector_SIM_MOS );

    pConnService_ = new DIN_ConnectionServiceClientUserCbk< MOS_Network >( 
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
// Name: MOS_Network destructor
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
MOS_Network::~MOS_Network()
{
    delete pConnService_;
    delete manager_;
    delete engine_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Network::Connect
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
bool MOS_Network::Connect( const std::string& strHostName, uint16 nPort )
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
// Name: MOS_Network::Disconnect
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
bool MOS_Network::Disconnect()
{
    if( ! IsConnected() )
    {
        MT_LOG_ERROR_MSG( "Déconnexion impossible - non connecté à un serveur" );
        return false;
    }
    session_->Close( false );
    MOS_App::GetApp().NotifyConnexionStatusChanged( false );
    return true;
}

// -----------------------------------------------------------------------------
// Name: MOS_Network::IsConnected
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
bool MOS_Network::IsConnected() const
{
    return session_ != 0;
}

// -----------------------------------------------------------------------------
// Name: MOS_Network::Update
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void MOS_Network::Update()
{
    engine_->Update();
}

// -----------------------------------------------------------------------------
// Name: MOS_Network::GetMessageMgr
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
MOS_AgentServerMsgMgr& MOS_Network::GetMessageMgr()
{
    return *manager_;
}

// -----------------------------------------------------------------------------
// Name: MOS_Network::OnConnected
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void MOS_Network::OnConnected( DIN::DIN_Link& link )
{
    MT_LOG_INFO_MSG( "Connecté à " << link.GetRemoteAddress().GetAddressAsString() );
    session_ = &link;
    manager_->Enable( link );

    MOS_ASN_MsgCtrlClientAnnouncement asnMsg;
    asnMsg.GetAsnMsg() = MsgCtrlClientAnnouncement::mos_light;
    asnMsg.Send();
    
    MOS_App::GetApp().NotifyConnexionStatusChanged( true );
}

// -----------------------------------------------------------------------------
// Name: MOS_Network::OnNotConnected
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void MOS_Network::OnNotConnected( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( "Non connecté à " << link.GetRemoteAddress().GetAddressAsString() << " (raison :" << reason.GetInfo() << ")" );   

    MOS_App::GetApp().NotifyConnexionStatusChanged( false );
}

// -----------------------------------------------------------------------------
// Name: MOS_Network::OnConnectionLost
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void MOS_Network::OnConnectionLost( DIN::DIN_Link& link, const DIN::DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( "Connexion à " << link.GetRemoteAddress().GetAddressAsString() << " perdue (raison: " << reason.GetInfo() << ")" );    
    session_ = 0;

    MOS_App::GetApp().NotifyConnexionLost();
}
