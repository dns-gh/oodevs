//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentServerMsgMgr.cpp $
// $Author: Nld $
// $Modtime: 8/07/05 15:57 $
// $Revision: 25 $
// $Workfile: MOS_AgentServerMsgMgr.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_AgentServerMsgMgr.h"

#include "MOS_AgentServerController.h"
#include "MOS_AgentServerConnectionMgr.h"
#include "MOS_Agent.h"
#include "MOS_Agent_ABC.h"
#include "MOS_AgentManager.h"
#include "MOS_Object_ABC.h"
#include "MOS_ObjectManager.h"
#include "MOS_Object_Factory.h"
#include "MOS_App.h"
#include "MOS_MOSServer.h"
#include "MOS_LineManager.h"
#include "MOS_Lima.h"
#include "MOS_Limit.h"
#include "MOS_World.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Net_Def.h"
#include "MOS_Gtia.h"
#include "MOS_Team.h"
#include "MOS_Meteo_Manager.h"
#include "MOS_Tools.h"
#include "MOS_MsgRecorder.h"
#include "MOS_LogMaintenanceConsign.h"
#include "MOS_LogMedicalConsign.h"
#include "MOS_LogSupplyConsign.h"
#include "MOS_Population.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
MOS_AgentServerMsgMgr::MOS_AgentServerMsgMgr( MOS_AgentServerController& controller )
    : MOS_AgentServerMgr_ABC( controller )
    , bPaused_              ( false )
    , bReceivingState_      ( true )
    , msgRecorder_          ( * new MOS_MsgRecorder( *this ) )
{
    DIN_ConnectorGuest connector( eConnector_SIM_MOS );
    pMessageService_ = new DIN_MessageServiceUserCbk<MOS_AgentServerMsgMgr>( *this, controller.GetDINEngine(), connector, "Msgs MOS Server -> Agent Server" );

    pMessageService_->RegisterReceivedMessage( eMsgInit                   , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgInit                );
    pMessageService_->RegisterReceivedMessage( eMsgProfilingValues        , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgProfilingValues     );

    pMessageService_->RegisterReceivedMessage( eMsgUnitTrace              , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgUnitTrace           );
    pMessageService_->RegisterReceivedMessage( eMsgUnitVisionCones        , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgUnitVisionCones     );
    pMessageService_->RegisterReceivedMessage( eMsgUnitInterVisibility    , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility );
    pMessageService_->RegisterReceivedMessage( eMsgObjectInterVisibility  , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility );
    pMessageService_->RegisterReceivedMessage( eMsgKnowledgeGroup         , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgKnowledgeGroup        );
    pMessageService_->RegisterReceivedMessage( eMsgArmy                   , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgArmy        );
    pMessageService_->RegisterReceivedMessage( eMsgDebugDrawPoints        , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints       );

    pMessageService_->RegisterReceivedMessage( eMsgSimMos           , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgSimMos            );
    pMessageService_->RegisterReceivedMessage( eMsgSimMosWithContext, *this, & MOS_AgentServerMsgMgr::OnReceiveMsgSimMosWithContext );

    pMessageService_->SetCbkOnError( MOS_AgentServerMsgMgr::OnError );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
MOS_AgentServerMsgMgr::~MOS_AgentServerMsgMgr()
{
    delete pMessageService_;
    delete &msgRecorder_;
}

//=============================================================================
// SERVICE ACTIVATION
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::Enable
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::Enable( MOS_AgentServer& agentServer )
{
    pMessageService_->Enable( agentServer.GetSession() );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::Disable
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::Disable( MOS_AgentServer& agentServer )
{
    pMessageService_->Disable( agentServer.GetSession() );
}



//=============================================================================
// MESSAGES : MISC
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::SendMsgEnableUnitVisionCones
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::SendMsgEnableUnitVisionCones()
{
    MOS_AgentServer& agentServer = controller_.GetConnectionMgr().GetAgentServer();
    pMessageService_->Send( agentServer.GetSession(), eMsgEnableUnitVisionCones );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::SendMsgDisableUnitVisionCones
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::SendMsgDisableUnitVisionCones()
{
    MOS_AgentServer& agentServer = controller_.GetConnectionMgr().GetAgentServer();
    pMessageService_->Send( agentServer.GetSession(), eMsgDisableUnitVisionCones );
}

enum E_UnitMagicAction
{
    eUnitMagicActionDestroyComposante = 0
};

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::SendMsgUnitMagicActionDestroyComposante
// Created: NLD 2004-03-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::SendMsgUnitMagicActionDestroyComposante( const MOS_Agent& agent )
{
    MOS_AgentServer& agentServer = controller_.GetConnectionMgr().GetAgentServer();

    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg << agent.GetAgentID();
    dinMsg << (uint8)eUnitMagicActionDestroyComposante;

    pMessageService_->Send( agentServer.GetSession(), eMsgUnitMagicAction, dinMsg );

    std::stringstream strOutputMsg;
    strOutputMsg << "Demande endommagement pion "  << agent.GetName();
    MT_LOG_INFO( strOutputMsg.str().c_str(), eSent, 0 );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgInit
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgInit( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    uint8 nSendUnitVisionConesState;
    input >> nSendUnitVisionConesState;

//APE    MOS_App::GetApp().GetMainWindow().GetAgentEditor().NotifyEnableUnitVisionCones( nSendUnitVisionConesState );

//$$$$$$$$$
    SendMsgEnableUnitVisionCones();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgAutomateCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgAutomateCreation( const ASN1T_MsgAutomateCreation& asnMsg )
{
    MOS_App::GetApp().GetAgentManager().CreateAgent( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgPionCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgPionCreation( const ASN1T_MsgPionCreation& asnMsg )
{
    MOS_App::GetApp().GetAgentManager().CreateAgent( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgKnowledgeGroup
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgKnowledgeGroup( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    uint32 nId;
    input >> nId;
    uint32 nTeamID;
    input >> nTeamID;
    MOS_Team* pTeam = MOS_App::GetApp().GetAgentManager().FindTeam( nTeamID );
    assert( pTeam != 0 );
    pTeam->CreateGtia( nId );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgArmy
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgArmy( DIN::DIN_Link& , DIN::DIN_Input& input )
{
    uint32 nID;
    input >> nID;
    MOS_App::GetApp().GetAgentManager().CreateTeam( nID, input );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgProfilingValues
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgProfilingValues( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    double rPerceptionTime;
    double rDecisionTime;
    double rActionTime;
    double rMainLoopTime;

    input >> rPerceptionTime;
    input >> rDecisionTime;
    input >> rActionTime;
    input >> rMainLoopTime;

//$$$APE    MOS_App::GetApp().GetMainWindow().GetGraphWindow().AddValues( rMainLoopTime, rPerceptionTime, rDecisionTime, rActionTime  );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgUnitTrace
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgUnitTrace( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    MIL_AgentID nAgentID;
    input >> nAgentID;

    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( nAgentID );
    assert( pAgent );
    pAgent->OnReceiveTraceMsg( input );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    MIL_AgentID nAgentID;
    input >> nAgentID;

    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( nAgentID );
    if( pAgent )
        pAgent->OnReceiveDebugDrawPointsMsg( input );

}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgUnitVisionCones
// Created: NLD 2003-02-12
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgUnitVisionCones( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    MIL_AgentID nAgentID;

    input >> nAgentID;

    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( nAgentID );
    assert( pAgent );
    pAgent->OnReceiveMsgVisionCones( input );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    MIL_AgentID nAgentID;

    input >> nAgentID;

    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( nAgentID );
    assert( pAgent );
    pAgent->OnReceiveMsgUnitInterVisibility( input );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    MIL_AgentID nAgentID;

    input >> nAgentID;

    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( nAgentID );
    assert( pAgent );
    pAgent->OnReceiveMsgObjectInterVisibility( input );
}


//=============================================================================
// ASN
//=============================================================================


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::SendMsgMosSim
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::SendMsgMosSim( ASN1T_MsgsMosSim& asnMsg )
{
    if( !MOS_App::GetApp().GetMOSServer().GetController().GetConnectionMgr().IsConnectedToAgentServer() )
        return;

    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );

    ASN1C_MsgsMosSim asnMsgCtrl( asnPEREncodeBuffer, asnMsg );

    if( asnMsgCtrl.Encode() != ASN_OK )
    {
        asnPEREncodeBuffer.PrintErrorInfo();
        assert( false ); //$$$ TMP
        return;
    }

    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg.GetOutput().Append( asnPEREncodeBuffer.GetMsgPtr(), asnPEREncodeBuffer.GetMsgLen() );

    MOS_AgentServer& agentServer = controller_.GetConnectionMgr().GetAgentServer();
    pMessageService_->Send( agentServer.GetSession(), eMsgMosSim, dinMsg );

    msgRecorder_.OnNewMsg( asnMsg.t, asnPEREncodeBuffer );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::SendMsgMosSimWithContext
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::SendMsgMosSimWithContext( ASN1T_MsgsMosSimWithContext& asnMsg, MIL_MOSContextID nCtx )
{
    if( !MOS_App::GetApp().GetMOSServer().GetController().GetConnectionMgr().IsConnectedToAgentServer() )
        return;

    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );

    ASN1C_MsgsMosSimWithContext asnMsgCtrl( asnPEREncodeBuffer, asnMsg );

    if( asnMsgCtrl.Encode() != ASN_OK )
    {
        asnPEREncodeBuffer.PrintErrorInfo();
        assert( false ); //$$$ TMP
        return;
    }

    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg << nCtx;

    dinMsg.GetOutput().Append( asnPEREncodeBuffer.GetMsgPtr(), asnPEREncodeBuffer.GetMsgLen() );

    MOS_AgentServer& agentServer = controller_.GetConnectionMgr().GetAgentServer();
    pMessageService_->Send( agentServer.GetSession(), eMsgMosSimWithContext, dinMsg );

    msgRecorder_.OnNewMsgWithContext( asnMsg.t, nCtx, asnPEREncodeBuffer );
}




// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::SendMsgMosSim
/** @param  pMsg
    @param  nMsgLength
    */
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::SendMsgMosSim( ASN1OCTET* pMsg, int nMsgLength )
{
    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg.GetOutput().Append( pMsg, nMsgLength );

    MOS_AgentServer& agentServer = controller_.GetConnectionMgr().GetAgentServer();
    pMessageService_->Send( agentServer.GetSession(), eMsgMosSim, dinMsg );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::SendMsgMosSimWithContext
/** @param  pMsg
    @param  nMsgLength
    @param  nCtx
*/
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::SendMsgMosSimWithContext( ASN1OCTET* pMsg, int nMsgLength, MIL_MOSContextID nCtx )
{
    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg << nCtx;

    dinMsg.GetOutput().Append( pMsg, nMsgLength );

    MOS_AgentServer& agentServer = controller_.GetConnectionMgr().GetAgentServer();
    pMessageService_->Send( agentServer.GetSession(), eMsgMosSimWithContext, dinMsg );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlPauseAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlPauseAck( const ASN1T_MsgCtrlPauseAck& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "CtrlPauseAck - Code: " << MOS_Tools::ToString( asnMsg );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    bPaused_ = true;
    MOS_App::GetApp().NotifyPauseStatusChanged( bPaused_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlResumeAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlResumeAck( const ASN1T_MsgCtrlResumeAck& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "CtrlResumeAck - Code: "  << MOS_Tools::ToString( asnMsg );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    bPaused_ = false;
    MOS_App::GetApp().NotifyPauseStatusChanged( bPaused_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlChangeTimeFactorAck
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlChangeTimeFactorAck( const ASN1T_MsgCtrlChangeTimeFactorAck& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "CtrlChangeTimeFactorAck - Facteur temps: " << (int)asnMsg.time_factor << " Code: " << MOS_Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    MOS_App::GetApp().NotifySpeedChanged( (int)asnMsg.time_factor );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlInfo
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlInfo( const ASN1T_MsgCtrlInfo& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "CtrlInfo - Current Tick: " << asnMsg.current_tick
                 << " - Tick duration : "       << asnMsg.tick_duration
                 << " - Time factor: "          << asnMsg.time_factor
                 << " - Exercice ID: "          << asnMsg.id_exercice
                 << " - Checkpoint Frequency: " << asnMsg.checkpoint_frequence;

    nTickDuration_ = asnMsg.tick_duration;
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    MOS_App::GetApp().NotifySpeedChanged( asnMsg.time_factor );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlBeginTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlBeginTick( const ASN1T_MsgCtrlBeginTick& asnMsg )
{
    //$$$ C'est du caca que de calculer le temps comme ca!
    MOS_App::GetApp().NotifyTimeChanged( asnMsg * nTickDuration_ );
    MOS_App::GetApp().NotifyTickStartEnd( true );
    msgRecorder_.OnTimeTick( MOS_App::GetApp().GetTime() );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlEndTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlEndTick( const ASN1T_MsgCtrlEndTick& /*asnMsg*/ )
{
    MOS_App::GetApp().NotifyTickStartEnd( false );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgUnitAttributes
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgUnitAttributes( const ASN1T_MsgUnitAttributes& asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pAgent );
    pAgent->OnAttributeUpdated( asnMsg );
    MOS_App::GetApp().NotifyAgentUpdated( *pAgent );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgUnitDotations
// Created: NLD 2003-03-06
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgUnitDotations( const ASN1T_MsgUnitDotations& asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pAgent );
    pAgent->OnAttributeUpdated( asnMsg );
    MOS_App::GetApp().NotifyAgentUpdated( *pAgent );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementCreation( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& asnMsg )
{
    MOS_App::GetApp().GetAgentManager().RegisterConsign( *new MOS_LogMaintenanceConsign( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementDestruction( const ASN1T_MsgLogMaintenanceTraitementEquipementDestruction& asnMsg )
{
    MOS_App::GetApp().GetAgentManager().DeleteMaintenanceConsign( asnMsg.oid_consigne );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementUpdate( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& asnMsg )
{
    MOS_LogMaintenanceConsign* pConsign = MOS_App::GetApp().GetAgentManager().FindMaintenanceConsign( asnMsg.oid_consigne );
    assert( pConsign );
    pConsign->OnReceiveMsgUpdate( asnMsg );
    MOS_App::GetApp().NotifyLogisticConsignUpdated( *pConsign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceEtat( const ASN1T_MsgLogMaintenanceEtat& asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pAgent );
    pAgent->OnReceiveMsgLogMaintenanceEtat( asnMsg );
    MOS_App::GetApp().NotifyAgentUpdated( *pAgent );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainCreation( const ASN1T_MsgLogSanteTraitementHumainCreation& asnMsg )
{
    MOS_App::GetApp().GetAgentManager().RegisterConsign( *new MOS_LogMedicalConsign( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainDestruction( const ASN1T_MsgLogSanteTraitementHumainDestruction& asnMsg )
{
    MOS_App::GetApp().GetAgentManager().DeleteMedicalConsign( asnMsg.oid_consigne );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainUpdate( const ASN1T_MsgLogSanteTraitementHumainUpdate& asnMsg )
{
    MOS_LogMedicalConsign* pConsign = MOS_App::GetApp().GetAgentManager().FindMedicalConsign( asnMsg.oid_consigne );
    assert( pConsign );
    pConsign->OnReceiveMsgUpdate( asnMsg );
    MOS_App::GetApp().NotifyLogisticConsignUpdated( *pConsign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteEtat( const ASN1T_MsgLogSanteEtat& asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pAgent );
    pAgent->OnReceiveMsgLogMedicalEtat( asnMsg );
    MOS_App::GetApp().NotifyAgentUpdated( *pAgent );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementCreation( const ASN1T_MsgLogRavitaillementTraitementCreation& asnMsg )
{
    MOS_App::GetApp().GetAgentManager().RegisterConsign( *new MOS_LogSupplyConsign( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementDestruction( const ASN1T_MsgLogRavitaillementTraitementDestruction& asnMsg )
{
    MOS_App::GetApp().GetAgentManager().DeleteSupplyConsign( asnMsg.oid_consigne );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementUpdate( const ASN1T_MsgLogRavitaillementTraitementUpdate& asnMsg )
{
    MOS_LogSupplyConsign* pConsign = MOS_App::GetApp().GetAgentManager().FindSupplyConsign( asnMsg.oid_consigne );
    assert( pConsign );
    pConsign->OnReceiveMsgUpdate( asnMsg );
    MOS_App::GetApp().NotifyLogisticConsignUpdated( *pConsign );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementEtat( const ASN1T_MsgLogRavitaillementEtat&  asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pAgent );
    pAgent->OnReceiveMsgLogSupplyEtat( asnMsg );
    MOS_App::GetApp().NotifyAgentUpdated( *pAgent );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementQuotas
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementQuotas( const ASN1T_MsgLogRavitaillementQuotas& asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
    assert( pAgent );
    pAgent->OnReceiveMsgLogSupplyQuotas( asnMsg );
    MOS_App::GetApp().NotifyAgentUpdated( *pAgent );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck( const ASN1T_MsgLogRavitaillementChangeQuotasAck& asnMsg, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : MsgLogRavitaillementChangeQuotaAck - Code ";
    switch( asnMsg )
    {
        case MsgLogRavitaillementChangeQuotasAck::error_invalid_donneur : strOutputMsg << "error_invalid_donneur"; break;
        case MsgLogRavitaillementChangeQuotasAck::error_invalid_receveur: strOutputMsg << "error_invalid_receveur"; break;
        case MsgLogRavitaillementChangeQuotasAck::no_error:               strOutputMsg << "no_error"; break;
        default:
            assert( false );
    }
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementPousserFluxAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementPousserFluxAck( const ASN1T_MsgLogRavitaillementPousserFluxAck& asnMsg, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : MsgLogRavitaillementPousserFlux - Code ";
    switch( asnMsg )
    {
        case MsgLogRavitaillementPousserFluxAck::error_invalid_donneur        : strOutputMsg << "error_invalid_donneur"; break;
        case MsgLogRavitaillementPousserFluxAck::error_invalid_receveur       : strOutputMsg << "error_invalid_receveur"; break;
        case MsgLogRavitaillementPousserFluxAck::error_ravitaillement_en_cours: strOutputMsg << "error_ravitaillement_en_cours"; break;
        case MsgLogRavitaillementPousserFluxAck::no_error                     : strOutputMsg << "no_error"; break;
        default:
            assert( false );
    }
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgUnitPathFind
// Created: NLD 2003-03-31
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgUnitPathFind( const ASN1T_MsgUnitPathFind& asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pAgent );

    pAgent->OnReceiveMsgPathFind( asnMsg );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgUnitMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgUnitMagicActionAck( const ASN1T_MsgUnitMagicActionAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "UnitMagicActionAck - Code: " << MOS_Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgObjectMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgObjectMagicActionAck( const ASN1T_MsgObjectMagicActionAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "ObjectMagicActionAck - Code: " << MOS_Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlMeteoGlobalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlMeteoGlobalAck()
{
    MT_LOG_INFO( "CtrlMeteoGlobaleAck", eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlMeteoLocalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlMeteoLocalAck()
{
    MT_LOG_INFO( "CtrlMeteoLocaleAck", eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateBegin
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateBegin()
{
    MT_LOG_INFO( "CtrlSendCurrentStateBegin", eReceived, 0 );
    bReceivingState_ = true;
    bUseMosLimits_ = true;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateEnd
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateEnd()
{
    MT_LOG_INFO( "CtrlSendCurrentStateEnd", eReceived, 0 );
    bReceivingState_ = false;

    // If we are using our own lines, upload them to the sim.
    if( bUseMosLimits_ )
        MOS_App::GetApp().GetLineManager().UpdateToSim();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSaveBegin
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSaveBegin()
{
    MT_LOG_INFO( "CtrlCheckPointSaveBegin", eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSaveEnd
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSaveEnd()
{
    MT_LOG_INFO( "CtrlCheckPointSaveEnd", eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointLoadBegin
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointLoadBegin()
{
    MT_LOG_INFO( "CtrlCheckPointBeginEnd", eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointLoadEnd
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointLoadEnd()
{
    MT_LOG_INFO( "CtrlCheckPointLoadEnd", eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSetFrequencyAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSetFrequencyAck()
{
    MT_LOG_INFO( "CtrlCheckPointSetFrequencyAck", eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck()
{
    MT_LOG_INFO( "CtrlCheckPointSetSaveNowAck", eReceived, 0 );
}


//=============================================================================
// LIMAS / LIMITS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimitCreationAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimitCreationAck( const ASN1T_MsgLimitCreationAck& asnMsg, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimitCreationAck - Code: " << MOS_Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        MOS_TacticalLine_ABC* pLimit = MOS_App::GetApp().GetLineManager().FindLine( asnMsg.oid );
        if( pLimit )
        {
            pLimit->SetNetworkState( MOS_TacticalLine_ABC::eNetworkStateRegistered );
            pLimit->SetState( MOS_TacticalLine_ABC::eStateOk );
        }
        else
            MOS_Limit::idManager_.LockIdentifier( asnMsg.oid );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimitUpdateAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimitUpdateAck( const ASN1T_MsgLimitUpdateAck& asnMsg, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimitUpdateAck - Code: " << MOS_Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        MOS_TacticalLine_ABC* pLimit = MOS_App::GetApp().GetLineManager().FindLine( asnMsg.oid );
        if( pLimit )
        {
            pLimit->SetNetworkState( MOS_TacticalLine_ABC::eNetworkStateRegistered );
            pLimit->SetState( MOS_TacticalLine_ABC::eStateOk );
        }
        // If the limit was not found, it's a message for another MOS
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimitDestructionAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimitDestructionAck( const ASN1T_MsgLimitDestructionAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimitDestructionAck - Code: " << MOS_Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        MOS_TacticalLine_ABC* pLimit = MOS_App::GetApp().GetLineManager().FindLine( asnMsg.oid );
        if( pLimit )
        {
            MOS_App::GetApp().NotifyTacticalLineDeleted( *pLimit );
            MOS_App::GetApp().GetLineManager().DeleteLine( asnMsg.oid );
        }
        else
            MOS_Limit::idManager_.ReleaseIdentifier( asnMsg.oid );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimaCreationAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimaCreationAck( const ASN1T_MsgLimaCreationAck& asnMsg, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimaCreationAck - Code: " << MOS_Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        MOS_TacticalLine_ABC* pLima = MOS_App::GetApp().GetLineManager().FindLine( asnMsg.oid );
        if( pLima )
        {
            pLima->SetNetworkState( MOS_TacticalLine_ABC::eNetworkStateRegistered );
            pLima->SetState( MOS_TacticalLine_ABC::eStateOk );
        }
        else
        {
            // If the limit was not found, it's a message for another MOS. Lock the identifier.
            MOS_Lima::idManager_.LockIdentifier( asnMsg.oid );
        }
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimaUpdateAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimaUpdateAck( const ASN1T_MsgLimaUpdateAck& asnMsg, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimaUpdateAck - Code: " << MOS_Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        MOS_TacticalLine_ABC* pLima = MOS_App::GetApp().GetLineManager().FindLine( asnMsg.oid );
        if( pLima )
        {
            pLima->SetNetworkState( MOS_TacticalLine_ABC::eNetworkStateRegistered );
            pLima->SetState( MOS_TacticalLine_ABC::eStateOk );
        }
        // If the limit was not found, it's a message for another MOS.
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimaDestructionAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimaDestructionAck( const ASN1T_MsgLimaDestructionAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimaDestructionAck - Code: " << MOS_Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        MOS_TacticalLine_ABC* pLima = MOS_App::GetApp().GetLineManager().FindLine( asnMsg.oid );
        if( pLima )
        {
            MOS_App::GetApp().NotifyTacticalLineDeleted( *pLima );
            MOS_App::GetApp().GetLineManager().DeleteLine( asnMsg.oid );
        }
        else
            MOS_Lima::idManager_.ReleaseIdentifier( asnMsg.oid );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimitCreation
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimitCreation( const ASN1T_MsgLimitCreation& asnMsg )
{
    // If we are receiving those lines while initializing from the sim, use those lines instead of our lines.
    if( bReceivingState_  && bUseMosLimits_ )
    {
        bUseMosLimits_ = false;
        MOS_App::GetApp().GetLineManager().DeleteAll();
    }

    MOS_Limit* pLimit = new MOS_Limit( asnMsg );
    MOS_App::GetApp().GetLineManager().RegisterLine( *pLimit );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimitDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimitDestruction( const ASN1T_MsgLimitDestruction& asnMsg )
{
    MOS_TacticalLine_ABC* pLimit = MOS_App::GetApp().GetLineManager().FindLine( asnMsg );
    if( pLimit )
    {
        MOS_App::GetApp().NotifyTacticalLineDeleted( *pLimit );
        MOS_App::GetApp().GetLineManager().DeleteLine( asnMsg );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimaCreation
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimaCreation( const ASN1T_MsgLimaCreation& asnMsg )
{
    // If we are receiving those lines while initializing from the sim, use those lines instead of our lines.
    if( bReceivingState_  && bUseMosLimits_ )
    {
        bUseMosLimits_ = false;
        MOS_App::GetApp().GetLineManager().DeleteAll();
    }

    MOS_Lima* pLimit = new MOS_Lima( asnMsg );
    MOS_App::GetApp().GetLineManager().RegisterLine( *pLimit );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimaDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimaDestruction( const ASN1T_MsgLimaDestruction& asnMsg )
{
    MOS_TacticalLine_ABC* pLima = MOS_App::GetApp().GetLineManager().FindLine( asnMsg );
    if( pLima )
    {
        MOS_App::GetApp().NotifyTacticalLineDeleted( *pLima );
        MOS_App::GetApp().GetLineManager().DeleteLine( asnMsg );
    }
}

//=============================================================================
// ORDRES
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgAutomateOrderAck
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgAutomateOrderAck( const ASN1T_MsgAutomateOrderAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "AutomateOrderAck - Code: " << MOS_Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

//APE    MOS_App::GetApp().GetMainWindow().GetAgentEditor().NotifyAgentMissionAck( asnMsg.error_code );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgPionOrderAck
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgPionOrderAck( const ASN1T_MsgPionOrderAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << MT_FormatString( "Agent[%d]", asnMsg.oid_unite_executante );
    strOutputMsg << "PionOrderAck - Code: " << MOS_Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgOrderConduiteAck
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgOrderConduiteAck( const ASN1T_MsgOrderConduiteAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "OrderConduiteAck - Code: " << MOS_Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCR
// Created: NLD 2002-09-02
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCR( const ASN1T_MsgCR& asnMsg )
{
    MOS_Agent_ABC* pAgent = MOS_App::GetApp().GetAgentManager().FindAllAgent( asnMsg.unit_id );
    assert( pAgent );
    pAgent->OnReceiveMsgCR( asnMsg );

}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgOrderManagement
// Created: NLD 2003-03-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgOrderManagement( const ASN1T_MsgOrderManagement& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "OrderManagement - Etat: ";
    switch( asnMsg.etat )
    {
        case EnumOrderState::started  :
        {
            strOutputMsg << "Démarré";
        }
        break;

        case EnumOrderState::stopped  :
        {
            strOutputMsg << "Arrêté";
        }
        break;

        case EnumOrderState::cancelled:
        {
            strOutputMsg << "Annulé";
        }
        break;
    }
    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgAutomateMRT
// Created: NLD 2003-05-13
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgAutomateMRT( const ASN1T_MsgAutomateMRT& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "AutomateMRT - Automate " << asnMsg.automate_id;

    std::stringstream strOutputMsg2;
    for( uint i = 0; i < asnMsg.missions.n; ++i )
    {
        ASN1T_MsgPionOrder& asnPionOrder = asnMsg.missions.elem[i];

        strOutputMsg2 << "Pion " << asnPionOrder.oid_unite_executante
                     << " - Mission " << asnPionOrder.mission.t << endl;

    }
    MT_LOG_INFO( strOutputMsg.str(), eReceived, strOutputMsg2.str().c_str() );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgPionOrder
// Created: NLD 2003-10-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgPionOrder( const ASN1T_MsgPionOrder& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "PionOrder - Mission " << asnMsg.mission.t;
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgWaitForOrderConduite
// Created: NLD 2003-04-04
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgWaitForOrderConduite( const ASN1T_MsgAttenteOrdreConduite& asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.unit_id );
    assert( pAgent );
    pAgent->OnReceiveMsgWaitForOrderConduite( asnMsg );
    //$$$ CRADE
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgSetAutomateModeAck
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgSetAutomateModeAck( const ASN1T_MsgSetAutomateModeAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "SetAutomateModeAck - Etat: ";
    switch( asnMsg.error_code )
    {
        case EnumSetAutomateModeErrorCode::no_error                : strOutputMsg << "No error"; break;
        case EnumSetAutomateModeErrorCode::error_already_debraye   : strOutputMsg << "Déjà debrayé"; break;
        case EnumSetAutomateModeErrorCode::error_already_embraye   : strOutputMsg << "Déjà embrayé"; break;
        case EnumSetAutomateModeErrorCode::error_invalid_unit      : strOutputMsg << "Invalid unit"; break;
        case EnumSetAutomateModeErrorCode::error_unit_not_automate : strOutputMsg << "Unit not automate"; break;
    }
    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgChangeAutomateAck
/** @param  asnMsg
    @param  nCtx
    */
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgChangeAutomateAck( const ASN1T_MsgChangeAutomateAck& asnMsg, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "ChangeAutomateAck - Etat: ";
    switch( asnMsg.error_code )
    {
    case EnumChangeAutomateErrorCode::no_error                 : strOutputMsg << "no_error"; break;
    case EnumChangeAutomateErrorCode::error_camps_incompatibles: strOutputMsg << "error_camps_incompatibles"; break;
    case EnumChangeAutomateErrorCode::error_invalid_automate   : strOutputMsg << "error_invalid_automate"; break;
    case EnumChangeAutomateErrorCode::error_invalid_pion       : strOutputMsg << "error_invalid_pion"; break;
    }

    if( asnMsg.error_code == EnumOrderErrorCode::no_error )
    {
        MOS_Agent* pPion = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
        assert( pPion );
        MOS_Agent* pAutomate = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
        assert( pAutomate );

        pPion->OnSuperiorChanged( *pAutomate );
        MOS_App::GetApp().NotifyAgentReparented( *pPion );
    }

    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgChangeAutomate
/** @param  asnMsg
*/
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgChangeAutomate( const ASN1T_MsgChangeAutomate& asnMsg )
{
    MOS_Agent* pPion = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pPion );

    MOS_Agent* pAutomate = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
    assert( pAutomate );

    pPion->OnSuperiorChanged( *pAutomate );
    MOS_App::GetApp().NotifyAgentReparented( *pPion );

    std::stringstream strOutputMsg;
    strOutputMsg << "ChangeAutomate - Pion: " << pPion->GetName() << " - Automate: " << pAutomate->GetName();
    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgChangeDiplomacyAck
/** @param  asnMsg
    @param  nCtx
    */
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgChangeDiplomacyAck( const ASN1T_MsgChangeDiplomatieAck& asnMsg, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "ChangeDiplomacyAck( - Etat: ";
    switch( asnMsg.error_code )
    {
    case EnumChangeDiplomatieErrorCode::no_error           : strOutputMsg << "no_error"; break;
    case EnumChangeDiplomatieErrorCode::error_invalid_camp : strOutputMsg << "error_invalid_camp"; break;
    }

    if( asnMsg.error_code == EnumChangeDiplomatieErrorCode::no_error )
    {
        MOS_Team* pTeam1 = MOS_App::GetApp().GetAgentManager().FindTeam( asnMsg.oid_camp1 );
        assert( pTeam1 );
        MOS_Team* pTeam2 = MOS_App::GetApp().GetAgentManager().FindTeam( asnMsg.oid_camp2 );
        assert( pTeam2 );

        pTeam1->ChangeDiplomaty( *pTeam2, asnMsg.diplomatie );
    }

    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgChangeGroupeConnaissanceAck
/** @param  asnMsg
    @param  nCtx
    */
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgChangeGroupeConnaissanceAck( const ASN1T_MsgChangeGroupeConnaissanceAck& asnMsg, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "ChangeGroupeConnaissanceAck - Etat: ";
    switch( asnMsg.error_code )
    {
    case EnumChangeGroupeConnaissanceErrorCode::no_error                          : strOutputMsg << "no_error"; break;
    case EnumChangeGroupeConnaissanceErrorCode::error_invalid_automate            : strOutputMsg << "error_invalid_automate"; break;
    case EnumChangeGroupeConnaissanceErrorCode::error_invalid_camp                : strOutputMsg << "error_invalid_camp"; break;
    case EnumChangeGroupeConnaissanceErrorCode::error_invalid_groupe_connaissance : strOutputMsg << "error_invalid_groupe_connaissance"; break;
    }

    if( asnMsg.error_code == EnumChangeGroupeConnaissanceErrorCode::no_error )
    {
        MOS_Agent* pAutomate = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
        assert( pAutomate );
        MOS_Gtia* pGtia = MOS_App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_connaissance );
        assert( pGtia );

        pAutomate->OnSuperiorChanged( *pGtia );
        MOS_App::GetApp().NotifyAgentReparented( *pAutomate );
    }

    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgChangeLiensLogistiquesAck
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgChangeLiensLogistiquesAck( const ASN1T_MsgChangeLiensLogistiquesAck& asnMsg, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : ChangeLiensLogistiquesAck - Etat: ";
    switch( asnMsg.error_code )
    {
        case EnumChangeLiensLogistiquesErrorCode::error_invalid_automate               : strOutputMsg << "error_invalid_automate"; break;
        case EnumChangeLiensLogistiquesErrorCode::error_invalid_automate_maintenance   : strOutputMsg << "error_invalid_automate_maintenance"; break;
        case EnumChangeLiensLogistiquesErrorCode::error_invalid_automate_ravitaillement: strOutputMsg << "error_invalid_automate_ravitaillement"; break;
        case EnumChangeLiensLogistiquesErrorCode::error_invalid_automate_sante         : strOutputMsg << "error_invalid_automate_sante"; break;
        case EnumChangeLiensLogistiquesErrorCode::error_invalid_automate_tc2           : strOutputMsg << "error_invalid_automate_tc2"; break;
        case EnumChangeLiensLogistiquesErrorCode::error_unit_surrendered               : strOutputMsg << "error_unit_surrendered"; break;
        case EnumChangeLiensLogistiquesErrorCode::no_error                             : strOutputMsg << "no_error"; break;
    }
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );


    MOS_Agent* pAutomate = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
    assert( pAutomate );
    pAutomate->OnLogisticLinksUpdated( asnMsg );
    MOS_App::GetApp().NotifyAgentUpdated( *pAutomate );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeCreation( const ASN1T_MsgUnitKnowledgeCreation& asnMsg )
{
    MOS_Gtia* pGtia = MOS_App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur );
    assert( pGtia );
    pGtia->OnReceiveMsgUnitKnowledgeCreation( asnMsg );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeUpdate
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeUpdate( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg )
{
    MOS_Gtia* pGtia = MOS_App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur );
    assert( pGtia );
    pGtia->OnReceiveMsgUnitKnowledgeUpdate( asnMsg );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg )
{
    MOS_Gtia* pGtia = MOS_App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur );
    assert( pGtia );
    pGtia->OnReceiveMsgUnitKnowledgeDestruction( asnMsg );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeCreation
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeCreation( const ASN1T_MsgObjectKnowledgeCreation& asnMsg )
{
    MOS_Team* pTeam = MOS_App::GetApp().GetAgentManager().FindTeam( asnMsg.oid_camp_possesseur );
    assert( pTeam );
    pTeam->OnReceiveMsgObjectKnowledgeCreation( asnMsg );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeUpdate
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeUpdate( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    MOS_Team* pTeam = MOS_App::GetApp().GetAgentManager().FindTeam( asnMsg.oid_camp_possesseur );
    assert( pTeam );
    pTeam->OnReceiveMsgObjectKnowledgeUpdate( asnMsg );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeDestruction
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg )
{
    MOS_Team* pTeam = MOS_App::GetApp().GetAgentManager().FindTeam( asnMsg.oid_camp_possesseur );
    assert( pTeam );
    pTeam->OnReceiveMsgObjectKnowledgeDestruction( asnMsg );
}


//=============================================================================
// OBJECTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgObjectCreation
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgObjectCreation( const ASN1T_MsgObjectCreation& asnMsg )
{
    MOS_ObjectManager& objectManager = MOS_App::GetApp().GetObjectManager();
    objectManager.CreateObject( asnMsg );
    MT_LOG_INFO( "ObjectCreation - ID: " << asnMsg.oid, eReceived, 0 );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgObjectUpdate
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgObjectUpdate( const ASN1T_MsgObjectUpdate& asnMsg )
{
    MOS_Object_ABC* pObject = MOS_App::GetApp().GetObjectManager().FindObject( asnMsg.oid );
    assert( pObject );

    pObject->Update( asnMsg );
    MOS_App::GetApp().NotifyObjectUpdated( *pObject );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgObjectDestruction
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgObjectDestruction( const ASN1T_MsgObjectDestruction& asnMsg )
{
    MOS_ObjectManager& ObjectManager = MOS_App::GetApp().GetObjectManager();

    MOS_Object_ABC* pObject = ObjectManager.FindObject( asnMsg );
    assert( pObject );

    MOS_App::GetApp().NotifyObjectDeleted( *pObject );

    ObjectManager.UnregisterObject( *pObject );
    delete pObject;         //$$$ TMP - devrait être fait par le manager

    MT_LOG_INFO( "ObjectDestruction - ID: " << asnMsg, eReceived, 0 );
}

//=============================================================================
// TIRS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgStartDirectFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgStartDirectFire( const ASN1T_MsgStartDirectFire& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "StartDirectFire - ID: " << asnMsg.oid_tir
                 << " - ID source " << asnMsg.oid_src
                 << " - ID dest "   << asnMsg.oid_dst;
    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );

    MOS_AgentManager& agentManager = MOS_App::GetApp().GetAgentManager();
    MOS_Agent* pAgentSrc = agentManager.FindAgent( asnMsg.oid_src );
    MOS_Agent* pAgentDst = agentManager.FindAgent( asnMsg.oid_dst );

    assert( pAgentDst != 0 && pAgentSrc != 0 );
    agentManager.AddDirectConflict( asnMsg.oid_tir, *pAgentSrc, *pAgentDst );
//    QSound::play( "conflict.wav" );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgStartIndirectFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgStartIndirectFire( const ASN1T_MsgStartIndirectFire& asnMsg )
{
    std::stringstream strOutputMsg;

    strOutputMsg << "StartIndirectFire - ID: " << asnMsg.oid_tir << " - ID source " << asnMsg.oid_src;
    std::string strTmp( (const char*)asnMsg.position.data, asnMsg.position.numocts );
    strOutputMsg << " - Pos dest " << strTmp;
    strOutputMsg << " - Munition : " << MOS_App::GetApp().GetResourceName( asnMsg.munition );
    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );

    MOS_AgentManager& agentManager = MOS_App::GetApp().GetAgentManager();
    MOS_Agent* pAgentSrc = agentManager.FindAgent( asnMsg.oid_src );
    assert( pAgentSrc != 0 );

    MT_Vector2D vPos;
    MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( strTmp, vPos );

    agentManager.AddIndirectConflict( asnMsg.oid_tir, *pAgentSrc, vPos );
//    QSound::play( "indirectconflict.wav" );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgStopDirectFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgStopDirectFire( const ASN1T_MsgStopDirectFire& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "StopDirectFire - ID: " << asnMsg.oid_tir;
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    // fire results
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindConflictOrigin( asnMsg.oid_tir );
    assert( pAgent );
    pAgent->OnReceiveMsgStopFire( asnMsg.resultat );

    MOS_App::GetApp().GetAgentManager().DeleteConflict( asnMsg.oid_tir );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgStopIndirectFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgStopIndirectFire( const ASN1T_MsgStopIndirectFire& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "StopIndirectFire - ID: " << asnMsg.oid_tir;
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    // fire results
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindConflictOrigin( asnMsg.oid_tir );
    assert( pAgent );
    for( uint i = 0; i < asnMsg.resultats.n; ++i )
        pAgent->OnReceiveMsgStopFire( asnMsg.resultats.elem[ i ] );

    MOS_App::GetApp().GetAgentManager().DeleteConflict( asnMsg.oid_tir );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgStartFireEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgStartFireEffect( const ASN1T_MsgStartFireEffect& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "Start ammunition meteo effect - ID: " << asnMsg.oid_effet << " Type:" << asnMsg.type;
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    MOS_App::GetApp().GetWeatherManager().RegisterAmmoMeteoEffect( asnMsg );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgStopFireEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgStopFireEffect( const ASN1T_MsgStopFireEffect& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "Stop ammunition meteo effect - ID: " << asnMsg;
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    MOS_App::GetApp().GetWeatherManager().UnregisterAmmoMeteoEffect( asnMsg );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgExplosion
// Created: NLD 2003-05-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgExplosion( const ASN1T_MsgExplosion& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "Explosion"
                 << " - ID objet " << asnMsg.oid_objet;

    // fire results
    MOS_Object_ABC* pObject = MOS_App::GetApp().GetObjectManager().FindObject( asnMsg.oid_objet );
    assert( pObject );
    for( uint i = 0; i < asnMsg.resultats.n; ++i )
        pObject->OnReceiveMsgExplosion( asnMsg.resultats.elem[ i ] );

    MOS_App::GetApp().NotifyObjectExplosion( *pObject );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgChangeDiplomatie
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgChangeDiplomatie( const ASN1T_MsgChangeDiplomatie& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : ChangeDiplomatie - ";

    MOS_Team* pArmy = MOS_App::GetApp().GetAgentManager().FindTeam( asnMsg.oid_camp1 );
    assert( pArmy );
    strOutputMsg << pArmy->GetName() << " -> ";

    pArmy = MOS_App::GetApp().GetAgentManager().FindTeam( asnMsg.oid_camp2 );
    assert( pArmy );
    strOutputMsg << pArmy->GetName() << " = ";

    switch( asnMsg.diplomatie )
    {
    case EnumDiplomatie::inconnu : strOutputMsg << "Inconnu"; break;
    case EnumDiplomatie::ami     : strOutputMsg << "Ami"; break;
    case EnumDiplomatie::neutre  : strOutputMsg << "Neutre"; break;
    case EnumDiplomatie::ennemi  : strOutputMsg << "Ennemi"; break;
    default:
        assert( false );
    }

    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnMsgPopulationCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnMsgPopulationCreation( const ASN1T_MsgPopulationCreation& asnMsg )
{
	std::stringstream strOutputMsg;
    strOutputMsg << "Creation de Population"
                 << " - ID  " << asnMsg.oid_population;
	MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	MOS_App::GetApp().GetAgentManager().CreatePopulation( asnMsg );

}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnMsgPopulationUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnMsgPopulationUpdate( const ASN1T_MsgPopulationUpdate& asnMsg )
{
	std::stringstream strOutputMsg;
    strOutputMsg << "Population Update"
                 << " - ID  " << asnMsg.oid_population;
	MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	MOS_Population* pop = MOS_App::GetApp().GetAgentManager().FindPopulation( asnMsg.oid_population );
	if ( pop != 0 )
		pop->UpdatePopulation( asnMsg );
    MOS_App::GetApp().NotifyPopulationUpdated( *pop );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnMsgPopulationConcentrationCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnMsgPopulationConcentrationCreation( const ASN1T_MsgPopulationConcentrationCreation& asnMsg )
{
	std::stringstream strOutputMsg;
    strOutputMsg << "Creation d'une concentration"
                 << " - ID  " << asnMsg.oid_concentration;
	MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	MOS_Population* pop = MOS_App::GetApp().GetAgentManager().FindPopulation( asnMsg.oid_population );
	if ( pop != 0 )
		pop->CreatePopulationConcentration( asnMsg );
    MOS_App::GetApp().NotifyPopulationUpdated( *pop );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnMsgPopulationConcentrationDestruction
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnMsgPopulationConcentrationDestruction( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg )
{
	std::stringstream strOutputMsg;
    strOutputMsg << "Destruction d'une concentration"
                 << " - ID  " << asnMsg.oid_population;
	MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	MOS_Population* pop = MOS_App::GetApp().GetAgentManager().FindPopulation( asnMsg.oid_population );
	if ( pop != 0 )
		pop->DeletePopulationConcentration( asnMsg );
    MOS_App::GetApp().NotifyPopulationUpdated( *pop );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnMsgPopulationConcentrationUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnMsgPopulationConcentrationUpdate( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
	std::stringstream strOutputMsg;
    strOutputMsg << "Update d'une concentration"
                 << " - ID  " << asnMsg.oid_concentration;
	MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	MOS_Population* pop = MOS_App::GetApp().GetAgentManager().FindPopulation( asnMsg.oid_population );
	if ( pop != 0 )
		pop->UpdatePopulationConcentration( asnMsg );
    MOS_App::GetApp().NotifyPopulationUpdated( *pop );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnMsgPopulationFluxCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnMsgPopulationFluxCreation( const ASN1T_MsgPopulationFluxCreation& asnMsg )
{
	std::stringstream strOutputMsg;
    strOutputMsg << "Creation d'un flux"
                 << " - ID  " << asnMsg.oid_flux;
	MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	MOS_Population* pop = MOS_App::GetApp().GetAgentManager().FindPopulation( asnMsg.oid_population );
	if ( pop != 0 )
		pop->CreatePopulationFlux( asnMsg );
    MOS_App::GetApp().NotifyPopulationUpdated( *pop );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnMsgPopulationFluxDestruction
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnMsgPopulationFluxDestruction  ( const ASN1T_MsgPopulationFluxDestruction& asnMsg )
{
	std::stringstream strOutputMsg;
    strOutputMsg << "Destruction d'un flux"
                 << " - ID  " << asnMsg.oid_flux;
	MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	MOS_Population* pop = MOS_App::GetApp().GetAgentManager().FindPopulation( asnMsg.oid_population );
	if ( pop != 0 )
		pop->DeletePopulationFlux( asnMsg );
    MOS_App::GetApp().NotifyPopulationUpdated( *pop );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnMsgPopulationFluxUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnMsgPopulationFluxUpdate( const ASN1T_MsgPopulationFluxUpdate& asnMsg )
{
	std::stringstream strOutputMsg;
    strOutputMsg << "Update d'un flux"
                 << " - ID  " << asnMsg.oid_flux;
	MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	MOS_Population* pop = MOS_App::GetApp().GetAgentManager().FindPopulation( asnMsg.oid_population );
	if ( pop != 0 )
		pop->UpdatePopulationFlux( asnMsg );
    MOS_App::GetApp().NotifyPopulationUpdated( *pop );
}

//=============================================================================
// ASN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr constructor
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgSimMos( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    uint nAsnMsgSize = input.GetAvailable();

    assert( nAsnMsgSize <= sizeof(aASNDecodeBuffer_) );

    // Fill the asn buffer array
    memcpy( aASNDecodeBuffer_, input.GetBuffer(nAsnMsgSize), nAsnMsgSize );

    // Create the asn msg buffer
    ASN1PERDecodeBuffer asnPERDecodeBuffer( aASNDecodeBuffer_, nAsnMsgSize, TRUE );

    // Decode the message
    ASN1T_MsgsSimMos asnMsg;
    ASN1C_MsgsSimMos asnMsgCtrl( asnPERDecodeBuffer, asnMsg );
    if( asnMsgCtrl.Decode() != ASN_OK )
    {
        asnPERDecodeBuffer.PrintErrorInfo();
        assert( false ); //$$$ TMP
    }

#ifdef _DEBUG
//    std::cout << "BEGIN MSG DUMP =>" << std::endl;
//    asnPERDecodeBuffer.SetTrace( true );
//    asnMsgCtrl.Print( "MsgsSimMos" );
//    std::cout << "END MSG DUMP =>" << std::endl;
#endif

    switch( asnMsg.t )
    {
        case T_MsgsSimMos_msg_ctrl_info:                            OnReceiveMsgCtrlInfo                  ( *asnMsg.u.msg_ctrl_info                           ); break;
        case T_MsgsSimMos_msg_ctrl_begin_tick:                      OnReceiveMsgCtrlBeginTick             (  asnMsg.u.msg_ctrl_begin_tick                     ); break;
        case T_MsgsSimMos_msg_ctrl_end_tick:                        OnReceiveMsgCtrlEndTick               ( *asnMsg.u.msg_ctrl_end_tick                       ); break;
        case T_MsgsSimMos_msg_ctrl_stop_ack:                        break;
        case T_MsgsSimMos_msg_ctrl_pause_ack:                       OnReceiveMsgCtrlPauseAck              ( asnMsg.u.msg_ctrl_pause_ack                       ); break;
        case T_MsgsSimMos_msg_ctrl_resume_ack:                      OnReceiveMsgCtrlResumeAck             ( asnMsg.u.msg_ctrl_resume_ack                      ); break;
        case T_MsgsSimMos_msg_ctrl_change_time_factor_ack:          OnReceiveMsgCtrlChangeTimeFactorAck   ( *asnMsg.u.msg_ctrl_change_time_factor_ack         ); break;
        case T_MsgsSimMos_msg_ctrl_meteo_globale_ack:               OnReceiveMsgCtrlMeteoGlobalAck        (); break;
        case T_MsgsSimMos_msg_ctrl_meteo_locale_ack:                OnReceiveMsgCtrlMeteoLocalAck         (); break;
        case T_MsgsSimMos_msg_ctrl_checkpoint_save_begin:           OnReceiveMsgCheckPointSaveBegin       (); break;
        case T_MsgsSimMos_msg_ctrl_checkpoint_save_end:             OnReceiveMsgCheckPointSaveEnd         (); break;
        case T_MsgsSimMos_msg_ctrl_checkpoint_load_begin:           OnReceiveMsgCheckPointLoadBegin       (); break;
        case T_MsgsSimMos_msg_ctrl_checkpoint_load_end:             OnReceiveMsgCheckPointLoadEnd         (); break;
        case T_MsgsSimMos_msg_ctrl_checkpoint_set_frequency_ack:    OnReceiveMsgCheckPointSetFrequencyAck (); break;
        case T_MsgsSimMos_msg_ctrl_checkpoint_save_now_ack:         OnReceiveMsgCheckPointSaveNowAck      (); break;
        case T_MsgsSimMos_msg_ctrl_send_current_state_begin:        OnReceiveMsgCtrlSendCurrentStateBegin (); break;
        case T_MsgsSimMos_msg_ctrl_send_current_state_end:          OnReceiveMsgCtrlSendCurrentStateEnd   (); break;

        case T_MsgsSimMos_msg_limit_creation:                       OnReceiveMsgLimitCreation             ( *asnMsg.u.msg_limit_creation                      ); break;
        case T_MsgsSimMos_msg_limit_destruction:                    OnReceiveMsgLimitDestruction          ( asnMsg.u.msg_limit_destruction                    ); break;
        case T_MsgsSimMos_msg_lima_creation:                        OnReceiveMsgLimaCreation              ( *asnMsg.u.msg_lima_creation                       ); break;
        case T_MsgsSimMos_msg_lima_destruction:                     OnReceiveMsgLimaDestruction           ( asnMsg.u.msg_lima_destruction                     ); break;

        case T_MsgsSimMos_msg_unit_knowledge_creation:              OnReceiveMsgUnitKnowledgeCreation     ( *asnMsg.u.msg_unit_knowledge_creation             ); break;
        case T_MsgsSimMos_msg_unit_knowledge_update:                OnReceiveMsgUnitKnowledgeUpdate       ( *asnMsg.u.msg_unit_knowledge_update               ); break;
        case T_MsgsSimMos_msg_unit_knowledge_destruction:           OnReceiveMsgUnitKnowledgeDestruction  ( *asnMsg.u.msg_unit_knowledge_destruction          ); break;

        case T_MsgsSimMos_msg_unit_attributes:                      OnReceiveMsgUnitAttributes            ( *asnMsg.u.msg_unit_attributes                     ); break;
        case T_MsgsSimMos_msg_unit_dotations:                       OnReceiveMsgUnitDotations             ( *asnMsg.u.msg_unit_dotations                      ); break;
        case T_MsgsSimMos_msg_unit_pathfind:                        OnReceiveMsgUnitPathFind              ( *asnMsg.u.msg_unit_pathfind                       ); break;

        case T_MsgsSimMos_msg_start_direct_fire:                    OnReceiveMsgStartDirectFire           ( *asnMsg.u.msg_start_direct_fire                   ); break;
        case T_MsgsSimMos_msg_start_indirect_fire:                  OnReceiveMsgStartIndirectFire         ( *asnMsg.u.msg_start_indirect_fire                 ); break;
        case T_MsgsSimMos_msg_stop_direct_fire:                     OnReceiveMsgStopDirectFire            ( *asnMsg.u.msg_stop_direct_fire                    ); break;
        case T_MsgsSimMos_msg_stop_indirect_fire:                   OnReceiveMsgStopIndirectFire          ( *asnMsg.u.msg_stop_indirect_fire                  ); break;
        case T_MsgsSimMos_msg_explosion:                            OnReceiveMsgExplosion                 ( *asnMsg.u.msg_explosion                           ); break;
        case T_MsgsSimMos_msg_cr:                                   OnReceiveMsgCR                        ( *asnMsg.u.msg_cr                                  ); break;
        case T_MsgsSimMos_msg_start_fire_effect:                    OnReceiveMsgStartFireEffect           ( *asnMsg.u.msg_start_fire_effect ); break;
        case T_MsgsSimMos_msg_stop_fire_effect:                     OnReceiveMsgStopFireEffect            ( asnMsg.u.msg_stop_fire_effect ); break;

        case T_MsgsSimMos_msg_order_management:                     OnReceiveMsgOrderManagement           ( *asnMsg.u.msg_order_management                    ); break;
        case T_MsgsSimMos_msg_attente_ordre_conduite:               OnReceiveMsgWaitForOrderConduite      ( *asnMsg.u.msg_attente_ordre_conduite              ); break;
        case T_MsgsSimMos_msg_annule_attente_ordre_conduite:        break; //$$$ TODO
        case T_MsgsSimMos_msg_automate_mrt:                         OnReceiveMsgAutomateMRT               ( *asnMsg.u.msg_automate_mrt                        ); break;
        case T_MsgsSimMos_msg_pion_order:                           OnReceiveMsgPionOrder                 ( *asnMsg.u.msg_pion_order ); break;

        case T_MsgsSimMos_msg_object_creation:                      OnReceiveMsgObjectCreation            ( *asnMsg.u.msg_object_creation                     ); break;
        case T_MsgsSimMos_msg_object_update:                        OnReceiveMsgObjectUpdate              ( *asnMsg.u.msg_object_update                       ); break;
        case T_MsgsSimMos_msg_object_destruction:                   OnReceiveMsgObjectDestruction         ( asnMsg.u.msg_object_destruction                   ); break;
        case T_MsgsSimMos_msg_object_knowledge_creation:            OnReceiveMsgObjectKnowledgeCreation   ( *asnMsg.u.msg_object_knowledge_creation           ); break;
        case T_MsgsSimMos_msg_object_knowledge_update:              OnReceiveMsgObjectKnowledgeUpdate     ( *asnMsg.u.msg_object_knowledge_update             ); break;
        case T_MsgsSimMos_msg_object_knowledge_destruction:         OnReceiveMsgObjectKnowledgeDestruction( *asnMsg.u.msg_object_knowledge_destruction        ); break;

        case T_MsgsSimMos_msg_change_automate:                      OnReceiveMsgChangeAutomate            ( *asnMsg.u.msg_change_automate ); break;

        case T_MsgsSimMos_msg_pion_creation:                        OnReceiveMsgPionCreation              ( *asnMsg.u.msg_pion_creation                       ); break;
        case T_MsgsSimMos_msg_automate_creation:                    OnReceiveMsgAutomateCreation          ( *asnMsg.u.msg_automate_creation                   ); break;
        case T_MsgsSimMos_msg_change_diplomatie:                    OnReceiveMsgChangeDiplomatie          ( *asnMsg.u.msg_change_diplomatie                   ); break;

        case T_MsgsSimMos_msg_log_maintenance_traitement_equipement_creation:    OnReceiveMsgLogMaintenanceTraitementEquipementCreation   ( *asnMsg.u.msg_log_maintenance_traitement_equipement_creation ); break;
        case T_MsgsSimMos_msg_log_maintenance_traitement_equipement_destruction: OnReceiveMsgLogMaintenanceTraitementEquipementDestruction( *asnMsg.u.msg_log_maintenance_traitement_equipement_destruction ); break;
        case T_MsgsSimMos_msg_log_maintenance_traitement_equipement_update:      OnReceiveMsgLogMaintenanceTraitementEquipementUpdate     ( *asnMsg.u.msg_log_maintenance_traitement_equipement_update ); break;
        case T_MsgsSimMos_msg_log_maintenance_etat:                              OnReceiveMsgLogMaintenanceEtat( *asnMsg.u.msg_log_maintenance_etat ); break;

        case T_MsgsSimMos_msg_log_ravitaillement_traitement_creation:    OnReceiveMsgLogRavitaillementTraitementCreation   ( *asnMsg.u.msg_log_ravitaillement_traitement_creation ); break;
        case T_MsgsSimMos_msg_log_ravitaillement_traitement_destruction: OnReceiveMsgLogRavitaillementTraitementDestruction( *asnMsg.u.msg_log_ravitaillement_traitement_destruction ); break;
        case T_MsgsSimMos_msg_log_ravitaillement_traitement_update:      OnReceiveMsgLogRavitaillementTraitementUpdate     ( *asnMsg.u.msg_log_ravitaillement_traitement_update ); break;
        case T_MsgsSimMos_msg_log_ravitaillement_etat:                   OnReceiveMsgLogRavitaillementEtat( *asnMsg.u.msg_log_ravitaillement_etat ); break;
        case T_MsgsSimMos_msg_log_ravitaillement_quotas:                 OnReceiveMsgLogRavitaillementQuotas               (  *asnMsg.u.msg_log_ravitaillement_quotas ); break;


        case T_MsgsSimMos_msg_log_sante_traitement_humain_creation:    OnReceiveMsgLogSanteTraitementHumainCreation   ( *asnMsg.u.msg_log_sante_traitement_humain_creation ); break;
        case T_MsgsSimMos_msg_log_sante_traitement_humain_destruction: OnReceiveMsgLogSanteTraitementHumainDestruction( *asnMsg.u.msg_log_sante_traitement_humain_destruction ); break;
        case T_MsgsSimMos_msg_log_sante_traitement_humain_update:      OnReceiveMsgLogSanteTraitementHumainUpdate     ( *asnMsg.u.msg_log_sante_traitement_humain_update ); break;
        case T_MsgsSimMos_msg_log_sante_etat:                          OnReceiveMsgLogSanteEtat( *asnMsg.u.msg_log_sante_etat ); break;

        case T_MsgsSimMos_msg_population_creation                       : OnMsgPopulationCreation                ( *asnMsg.u.msg_population_creation ); break;
        case T_MsgsSimMos_msg_population_update                         : OnMsgPopulationUpdate                  ( *asnMsg.u.msg_population_update ); break;
        case T_MsgsSimMos_msg_population_concentration_creation         : OnMsgPopulationConcentrationCreation   ( *asnMsg.u.msg_population_concentration_creation ); break;
        case T_MsgsSimMos_msg_population_concentration_destruction      : OnMsgPopulationConcentrationDestruction( *asnMsg.u.msg_population_concentration_destruction ); break;
        case T_MsgsSimMos_msg_population_concentration_update           : OnMsgPopulationConcentrationUpdate     ( *asnMsg.u.msg_population_concentration_update ); break;
        case T_MsgsSimMos_msg_population_flux_creation                  : OnMsgPopulationFluxCreation            ( *asnMsg.u.msg_population_flux_creation ); break;
        case T_MsgsSimMos_msg_population_flux_destruction               : OnMsgPopulationFluxDestruction         ( *asnMsg.u.msg_population_flux_destruction ); break;
        case T_MsgsSimMos_msg_population_flux_update                    : OnMsgPopulationFluxUpdate              ( *asnMsg.u.msg_population_flux_update ); break;

        default:
            {
#ifdef _DEBUG
                std::cout << "BEGIN MSG DUMP =>" << std::endl;
                asnPERDecodeBuffer.SetTrace( true );
                asnMsgCtrl.Print( "MsgsSimMos" );
                std::cout << "END MSG DUMP =>" << std::endl;
#endif
            }
//            assert( false );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgSimMosWithContext
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgSimMosWithContext( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    MIL_MOSContextID nCtx;
    input >> nCtx;

    uint nAsnMsgSize = input.GetAvailable();

    assert( nAsnMsgSize <= sizeof(aASNDecodeBuffer_) );

    // Fill the asn buffer array
    memcpy( aASNDecodeBuffer_, input.GetBuffer(nAsnMsgSize), nAsnMsgSize );

    // Create the asn msg buffer
    ASN1PERDecodeBuffer asnPERDecodeBuffer( aASNDecodeBuffer_, nAsnMsgSize, TRUE );

    // Decode the message
    ASN1T_MsgsSimMosWithContext asnMsg;
    ASN1C_MsgsSimMosWithContext asnMsgCtrl( asnPERDecodeBuffer, asnMsg );
    if( asnMsgCtrl.Decode() != ASN_OK )
    {
        asnPERDecodeBuffer.PrintErrorInfo();
        assert( false ); //$$$ TMP
    }





#ifdef _DEBUG
//    std::cout << "BEGIN MSG DUMP =>" << std::endl;
//    asnPERDecodeBuffer.SetTrace( true );
//    asnMsgCtrl.Print( "MsgsSimMos" );
//    std::cout << "END MSG DUMP =>" << std::endl;
#endif

    switch( asnMsg.t )
    {
        case T_MsgsSimMosWithContext_msg_limit_creation_ack:                     OnReceiveMsgLimitCreationAck                ( *asnMsg.u.msg_limit_creation_ack                  , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_limit_update_ack:                       OnReceiveMsgLimitUpdateAck                  ( *asnMsg.u.msg_limit_update_ack                    , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_limit_destruction_ack:                  OnReceiveMsgLimitDestructionAck             ( *asnMsg.u.msg_limit_destruction_ack               , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_lima_creation_ack:                      OnReceiveMsgLimaCreationAck                 ( *asnMsg.u.msg_lima_creation_ack                   , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_lima_update_ack:                        OnReceiveMsgLimaUpdateAck                   ( *asnMsg.u.msg_lima_update_ack                     , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_lima_destruction_ack:                   OnReceiveMsgLimaDestructionAck              ( *asnMsg.u.msg_lima_destruction_ack                , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_automate_order_ack:                     OnReceiveMsgAutomateOrderAck                ( *asnMsg.u.msg_automate_order_ack                  , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_pion_order_ack:                         OnReceiveMsgPionOrderAck                    ( *asnMsg.u.msg_pion_order_ack                      , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_order_conduite_ack:                     OnReceiveMsgOrderConduiteAck                ( *asnMsg.u.msg_order_conduite_ack                  , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_unit_magic_action_ack:                  OnReceiveMsgUnitMagicActionAck              ( *asnMsg.u.msg_unit_magic_action_ack               , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_set_automate_mode_ack:                  OnReceiveMsgSetAutomateModeAck              ( *asnMsg.u.msg_set_automate_mode_ack               , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_change_automate_ack:                    OnReceiveMsgChangeAutomateAck               ( *asnMsg.u.msg_change_automate_ack                 , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_change_diplomatie_ack:                  OnReceiveMsgChangeDiplomacyAck              ( *asnMsg.u.msg_change_diplomatie_ack               , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_change_groupe_connaissance_ack:         OnReceiveMsgChangeGroupeConnaissanceAck     ( *asnMsg.u.msg_change_groupe_connaissance_ack      , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_object_magic_action_ack:                OnReceiveMsgObjectMagicActionAck            ( *asnMsg.u.msg_object_magic_action_ack             , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_change_liens_logistiques_ack:           OnReceiveMsgChangeLiensLogistiquesAck       ( *asnMsg.u.msg_change_liens_logistiques_ack        , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_log_ravitaillement_pousser_flux_ack:    OnReceiveMsgLogRavitaillementPousserFluxAck (  asnMsg.u.msg_log_ravitaillement_pousser_flux_ack , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_log_ravitaillement_change_quotas_ack:   OnReceiveMsgLogRavitaillementChangeQuotaAck (  asnMsg.u.msg_log_ravitaillement_change_quotas_ack, nCtx ); break;

//        case T_MsgsSimMosWithContext_msg_population_order_ack  : break; //$$$ TODO        

        default:
            {
#ifdef _DEBUG
                std::cout << "BEGIN MSG DUMP =>" << std::endl;
                asnPERDecodeBuffer.SetTrace( true );
                asnMsgCtrl.Print( "MsgsSimMos" );
                std::cout << "END MSG DUMP =>" << std::endl;
#endif
//                assert( false );
            }
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnError
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
bool MOS_AgentServerMsgMgr::OnError( DIN::DIN_Link& /*link*/, const DIN::DIN_ErrorDescription& /*info*/ )
{
    //MT_LOG_INFO_MSG( "MOS -> AS - Message service error : " << info.GetInfo().c_str() );
    return false;
}

