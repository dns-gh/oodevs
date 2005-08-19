//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentServerConnectionMgr.cpp $
// $Author: Ape $
// $Modtime: 1/03/05 9:53 $
// $Revision: 9 $
// $Workfile: MOS_AgentServerConnectionMgr.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"

#include "MOS_AgentServerConnectionMgr.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentServerConnectionMgr.inl"
#endif

#include "MOS_Config.h"
#include "MOS_AgentServerController.h"
#include "MOS_AgentServer.h"
#include "MOS_AgentServerMsgMgr.h"
#include "MOS_MOSServer.h"
#include "MOS_App.h"
#include "MOS_AgentManager.h"
#include "MOS_LineManager.h"
#include "MOS_DynaObjectManager.h"
#include "MOS_Net_Def.h"
#include "MOS_App.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Team.h"

using namespace DIN;
using namespace NEK;


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerConnectionMgr constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
MOS_AgentServerConnectionMgr::MOS_AgentServerConnectionMgr( MOS_AgentServerController& controller )
    : MOS_AgentServerMgr_ABC( controller )
    , pAgentServer_         ( 0 )
{
    DIN_ConnectionProtocols connProtocols( NEK_Protocols::eTCP, NEK_Protocols::eIPv4 );

    DIN_ConnectorGuest connector( eConnector_SIM_MOS );

    pConnService_ = new DIN_ConnectionServiceClientUserCbk< MOS_AgentServerConnectionMgr >( 
                                      *this
                                    , controller_.GetDINEngine()
                                    , connector
                                    , connProtocols
                                    , eConnMagicMOSServerAgentServer
                                    , "MOS Server to agent server"); 

    pConnService_->SetCbkOnConnectionSuccessful( & MOS_AgentServerConnectionMgr::OnConnected      );
    pConnService_->SetCbkOnConnectionFailed    ( & MOS_AgentServerConnectionMgr::OnNotConnected   );
    pConnService_->SetCbkOnConnectionLost      ( & MOS_AgentServerConnectionMgr::OnConnectionLost );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerConnectionMgr destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
MOS_AgentServerConnectionMgr::~MOS_AgentServerConnectionMgr()
{
    delete pConnService_;
}

//=============================================================================
// MAIN METHODS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerConnectionMgr::StartServer
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
bool MOS_AgentServerConnectionMgr::ConnectToServer( const std::string& strHostName, uint16 nPort )
{
    if( pAgentServer_ )
    {
        MT_LOG_ERROR_MSG( "Déjà connecté" );
        assert( false );
        return false;
    }
    
    NEK_AddressINET addr( strHostName.c_str(), nPort );

    pConnService_->JoinHost( addr );
    return true;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerConnectionMgr::DisonnectFromServer
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
bool MOS_AgentServerConnectionMgr::DisconnectFromServer()
{
    if( !pAgentServer_ )
    {
        MT_LOG_ERROR_MSG( "Déconnexion impossible - non connecté à un serveur" );
        return false;
    }

    pAgentServer_->GetSession().Close( false );

    MOS_App::GetApp().NotifyConnexionStatusChanged( false );

    return true;
}


//=============================================================================
// CONNECTION SERVICE CALLBACKS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerConnectionMgr::OnConnected
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void MOS_AgentServerConnectionMgr::OnConnected( DIN_Link& link )
{
    MT_LOG_INFO_MSG( "Connecté à " << link.GetRemoteAddress().GetAddressAsString() );

    MOS_AgentServer& agentServer = AssociateAgentServerToSession( link );

    controller_.GetMessageMgr().Enable( agentServer );

    MOS_ASN_MsgCtrlClientAnnouncement asnMsg;
    asnMsg.GetAsnMsg() = MsgCtrlClientAnnouncement::mos_light;
    asnMsg.Send();
    
    MOS_App::GetApp().NotifyConnexionStatusChanged( true );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerConnectionMgr::OnNotConnected
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void MOS_AgentServerConnectionMgr::OnNotConnected( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( "Non connecté à " << link.GetRemoteAddress().GetAddressAsString() << " (raison :" << reason.GetInfo() << ")" );   

    MOS_App::GetApp().NotifyConnexionStatusChanged( false );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerConnectionMgr::OnConnectionLost
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void MOS_AgentServerConnectionMgr::OnConnectionLost( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( "Connexion à " << link.GetRemoteAddress().GetAddressAsString() << " perdue (raison: " << reason.GetInfo() << ")" );    
    DissociateAgentServerFromSession( link );

    //$$$ DEVRAIT ETRE CENTRALISE DANS MOS_APP
    MOS_App::GetApp().NotifyConnexionStatusChanged( false );
    MOS_App::GetApp().GetAgentManager().DeleteAllAgents();
    MOS_App::GetApp().GetAgentManager().DeleteAllTeams();
    MOS_App::GetApp().GetLineManager ().OnDeconnexion();
    MOS_App::GetApp().GetDynaObjectManager().DeleteAllDynaObjects();

    const MOS_AgentManager::T_TeamMap& teams = MOS_App::GetApp().GetAgentManager().GetTeams();
    for( MOS_AgentManager::CIT_TeamMap itTeam = teams.begin(); itTeam != teams.end(); ++itTeam )
        itTeam->second->DestroyAllObjectKnowledges();
}

//=============================================================================
// TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerConnectionMgr::AssociateAgentServerToSession
// Created:  NLD 2002-01-10 
//-----------------------------------------------------------------------------
MOS_AgentServer& MOS_AgentServerConnectionMgr::AssociateAgentServerToSession( DIN_Link& link )
{
    assert( !link.GetUserData() );

    pAgentServer_ = agentServerRecycler_.GetElement();
    if( !pAgentServer_ )
        pAgentServer_ = new MOS_AgentServer();

    pAgentServer_->Initialize( link );

    link.SetUserData( pAgentServer_ );

    return *pAgentServer_;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerConnectionMgr::DissociateAgentServerFromSession
// Created:  NLD 2002-01-10 
//-----------------------------------------------------------------------------
void MOS_AgentServerConnectionMgr::DissociateAgentServerFromSession( DIN_Link& link )
{
    assert( link.GetUserData() );

    MOS_AgentServer& agentServer = GetAgentServerFromSession( link );

    pAgentServer_ = 0;

    agentServer.Terminate();

    agentServerRecycler_.AddElement( agentServer );
    link.SetUserData( 0 );
}
