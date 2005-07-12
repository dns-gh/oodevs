//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentServerConnectionMgr.cpp $
// $Author: Nld $
// $Modtime: 14/02/05 15:59 $
// $Revision: 17 $
// $Workfile: MOS_AgentServerConnectionMgr.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"

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
#include "MOS_MainWindow.h"
#include "MOS_SIMControl.h"
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
        MT_LOG_ERROR_MSG( "Already connected" );
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
        MT_LOG_ERROR_MSG( "Cannot disconnect - not connected to agent server" );
        return false;
    }

    pAgentServer_->GetSession().Close( false );

    MOS_App::GetApp().GetMainWindow().GetSimControl().NotifyConnection( false );

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
    MT_LOG_INFO_MSG( MT_FormatString( "MOS -> AS - Connected to %s", link.GetRemoteAddress().GetAddressAsString().c_str() ).c_str() );

    MOS_AgentServer& agentServer = AssociateAgentServerToSession( link );

    controller_.GetMessageMgr().Enable( agentServer );

    MOS_ASN_MsgCtrlClientAnnouncement asnMsg;
    asnMsg.GetAsnMsg() = MsgCtrlClientAnnouncement::mos_light;
    asnMsg.Send();
    
//    MOS_App::GetApp().GetMainWindow().GetSimControl().NotifyConnection( true );

}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerConnectionMgr::OnNotConnected
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void MOS_AgentServerConnectionMgr::OnNotConnected( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( MT_FormatString( "MOS -> AS - Not connected to %s (reason : %s)", link.GetRemoteAddress().GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );    

    MOS_App::GetApp().GetMainWindow().GetSimControl().NotifyConnection( false );

}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerConnectionMgr::OnConnectionLost
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void MOS_AgentServerConnectionMgr::OnConnectionLost( DIN_Link& link, const DIN_ErrorDescription& reason )
{
    MT_LOG_INFO_MSG( MT_FormatString( "MOS -> AS - Connection to %s lost (reason : %s)", link.GetRemoteAddress().GetAddressAsString().c_str(), reason.GetInfo().c_str() ).c_str() );    
    DissociateAgentServerFromSession( link );

    //$$$ DEVRAIT ETRE CENTRALISE DANS MOS_APP
    MOS_App::GetApp().GetAgentManager().RemoveAllAgents();
    MOS_App::GetApp().GetAgentManager().RemoveAllGtias();
//    MOS_App::GetApp().GetAgentManager().RemoveAllSensors();
    MOS_App::GetApp().GetLineManager ().RemoveAll();
    MOS_App::GetApp().GetDynaObjectManager().RemoveAllDynaObjects();   
    MOS_App::GetApp().GetMainWindow().GetSimControl().NotifyConnection( false );

    const MOS_AgentManager::T_TeamIDVector& teams = MOS_App::GetApp().GetAgentManager().GetTeams();
    for( MOS_AgentManager::CIT_TeamIDVector itTeam = teams.begin(); itTeam != teams.end(); ++itTeam )
        (**itTeam).DestroyAllKnowledges();
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
