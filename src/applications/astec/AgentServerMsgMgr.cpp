//*****************************************************************************
//
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentServerMsgMgr.cpp $
// $Author: Nld $
// $Modtime: 8/07/05 15:57 $
// $Revision: 25 $
// $Workfile: AgentServerMsgMgr.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "AgentServerMsgMgr.h"
#include "Agent.h"
#include "Agent_ABC.h"
#include "AgentManager.h"
#include "Object_ABC.h"
#include "ObjectManager.h"
#include "Object_Factory.h"
#include "App.h"
#include "LineManager.h"
#include "Lima.h"
#include "Limit.h"
#include "World.h"
#include "ASN_Messages.h"
#include "Net_Def.h"
#include "Gtia.h"
#include "Team.h"
#include "Meteo_Manager.h"
#include "Tools.h"
#include "MsgRecorder.h"
#include "LogMaintenanceConsign.h"
#include "LogMedicalConsign.h"
#include "LogSupplyConsign.h"
#include "Population.h"
#include "PopulationKnowledge.h"
#include "GLTool.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
AgentServerMsgMgr::AgentServerMsgMgr( DIN::DIN_Engine& engine )
    : session_         ( 0 )
    , bPaused_         ( false )
    , bReceivingState_ ( true )
    , msgRecorder_     ( * new MsgRecorder( *this ) )
    , nFirstTick_      ( 0 )
{
    DIN_ConnectorGuest connector( eConnector_SIM_MOS );
    pMessageService_ = new DIN_MessageServiceUserCbk<AgentServerMsgMgr>(
        *this, engine, connector, "Msgs MOS Server -> Agent Server" );

    pMessageService_->RegisterReceivedMessage( eMsgInit                                  , *this, & AgentServerMsgMgr::OnReceiveMsgInit                );
    pMessageService_->RegisterReceivedMessage( eMsgProfilingValues                       , *this, & AgentServerMsgMgr::OnReceiveMsgProfilingValues     );

    pMessageService_->RegisterReceivedMessage( eMsgTrace                                 , *this, & AgentServerMsgMgr::OnReceiveMsgTrace                );
    pMessageService_->RegisterReceivedMessage( eMsgUnitVisionCones                       , *this, & AgentServerMsgMgr::OnReceiveMsgUnitVisionCones     );
    pMessageService_->RegisterReceivedMessage( eMsgUnitInterVisibility                   , *this, & AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility );
    pMessageService_->RegisterReceivedMessage( eMsgObjectInterVisibility                 , *this, & AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility );
    pMessageService_->RegisterReceivedMessage( eMsgPopulationConcentrationInterVisibility, *this, & AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationInterVisibility );
    pMessageService_->RegisterReceivedMessage( eMsgPopulationFlowInterVisibility         , *this, & AgentServerMsgMgr::OnReceiveMsgPopulationFlowInterVisibility );
    pMessageService_->RegisterReceivedMessage( eMsgKnowledgeGroup                        , *this, & AgentServerMsgMgr::OnReceiveMsgKnowledgeGroup        );
    pMessageService_->RegisterReceivedMessage( eMsgArmy                                  , *this, & AgentServerMsgMgr::OnReceiveMsgArmy        );
    pMessageService_->RegisterReceivedMessage( eMsgDebugDrawPoints                       , *this, & AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints       );
    pMessageService_->RegisterReceivedMessage( eMsgPopulationCollision                   , *this, & AgentServerMsgMgr::OnReceiveMsgPopulationCollision );
    pMessageService_->RegisterReceivedMessage( eMsgSimMos           , *this, & AgentServerMsgMgr::OnReceiveMsgSimMos            );
    pMessageService_->RegisterReceivedMessage( eMsgSimMosWithContext, *this, & AgentServerMsgMgr::OnReceiveMsgSimMosWithContext );

    pMessageService_->SetCbkOnError( &AgentServerMsgMgr::OnError );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
AgentServerMsgMgr::~AgentServerMsgMgr()
{
    delete pMessageService_;
    delete &msgRecorder_;
}

//=============================================================================
// SERVICE ACTIVATION
//=============================================================================

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Enable
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Enable( DIN::DIN_Link& session )
{
    pMessageService_->Enable( session );
    session_ = &session;
}

//=============================================================================
// MESSAGES : MISC
//=============================================================================

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( unsigned int id, DIN::DIN_BufferedMessage& message )
{
    if( ! session_ )
        throw std::runtime_error( "Not connected" );
    pMessageService_->Send( *session_, id, message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::Send
// Created: AGE 2006-02-08
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::Send( unsigned int id )
{
    if( ! session_ )
        throw std::runtime_error( "Not connected" );
    pMessageService_->Send( *session_, id );
}   

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::SendMsgEnableUnitVisionCones
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::SendMsgEnableUnitVisionCones()
{
    Send( eMsgEnableUnitVisionCones );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::SendMsgDisableUnitVisionCones
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::SendMsgDisableUnitVisionCones()
{
    Send( eMsgDisableUnitVisionCones );
}

enum E_UnitMagicAction
{
    eUnitMagicActionDestroyComposante = 0
};

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::SendMsgUnitMagicActionDestroyComposante
// Created: NLD 2004-03-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::SendMsgUnitMagicActionDestroyComposante( const Agent& agent )
{
    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg << agent.GetID();
    dinMsg << (uint8)eUnitMagicActionDestroyComposante;

    Send( eMsgUnitMagicAction, dinMsg );

    std::stringstream strOutputMsg;
    strOutputMsg << "Demande endommagement pion "  << agent.GetName();
    MT_LOG_INFO( strOutputMsg.str().c_str(), eSent, 0 );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgInit
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgInit( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    uint8 nSendUnitVisionConesState;
    input >> nSendUnitVisionConesState;

//APE    App::GetApp().GetMainWindow().GetAgentEditor().NotifyEnableUnitVisionCones( nSendUnitVisionConesState );

//$$$$$$$$$
    SendMsgEnableUnitVisionCones();
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomateCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomateCreation( const ASN1T_MsgAutomateCreation& asnMsg )
{
    App::GetApp().GetAgentManager().CreateAgent( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPionCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPionCreation( const ASN1T_MsgPionCreation& asnMsg )
{
    App::GetApp().GetAgentManager().CreateAgent( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgKnowledgeGroup
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgKnowledgeGroup( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    uint32 nId;
    input >> nId;
    uint32 nTeamID;
    input >> nTeamID;
    Team* pTeam = App::GetApp().GetAgentManager().FindTeam( nTeamID );
    assert( pTeam != 0 );
    pTeam->CreateGtia( nId );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgArmy
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgArmy( DIN::DIN_Link& , DIN::DIN_Input& input )
{
    uint32 nID;
    input >> nID;
    App::GetApp().GetAgentManager().CreateTeam( nID, input );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfilingValues
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfilingValues( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    double rPerceptionTime;
    double rDecisionTime;
    double rActionTime;
    double rMainLoopTime;

    input >> rPerceptionTime;
    input >> rDecisionTime;
    input >> rActionTime;
    input >> rMainLoopTime;

//$$$APE    App::GetApp().GetMainWindow().GetGraphWindow().AddValues( rMainLoopTime, rPerceptionTime, rDecisionTime, rActionTime  );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgTrace
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgTrace( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    uint32 nID;
    input >> nID;

    Agent_ABC* pTarget = App::GetApp().GetAgentManager().FindAllAgent( nID );
    assert( pTarget );
    pTarget->OnReceiveTraceMsg( input );

}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    MIL_AgentID nAgentID;
    input >> nAgentID;

    Agent_ABC* pAgent = App::GetApp().GetAgentManager().FindAllAgent( nAgentID );
    if( pAgent )
        pAgent->OnReceiveDebugDrawPointsMsg( input );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitVisionCones
// Created: NLD 2003-02-12
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitVisionCones( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    MIL_AgentID nAgentID;

    input >> nAgentID;

    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( nAgentID );
    assert( pAgent );
    pAgent->OnReceiveMsgVisionCones( input );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    MIL_AgentID nAgentID;

    input >> nAgentID;

    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( nAgentID );
    assert( pAgent );
    pAgent->OnReceiveMsgUnitInterVisibility( input );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationInterVisibility( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    MIL_AgentID nAgentID;

    input >> nAgentID;

    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( nAgentID );
    assert( pAgent );
    pAgent->OnReceiveMsgPopulationConcentrationInterVisibility( input );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowInterVisibility( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    MIL_AgentID nAgentID;

    input >> nAgentID;

    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( nAgentID );
    assert( pAgent );
    pAgent->OnReceiveMsgPopulationFlowInterVisibility( input );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationCollision
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationCollision( DIN::DIN_Link& , DIN::DIN_Input& input )
{
    MIL_AgentID nAgentID;

    input >> nAgentID;

    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( nAgentID );
    assert( pAgent );
    pAgent->OnReceiveMsgPopulationCollision( input );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    MIL_AgentID nAgentID;

    input >> nAgentID;

    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( nAgentID );
    assert( pAgent );
    pAgent->OnReceiveMsgObjectInterVisibility( input );
}


//=============================================================================
// ASN
//=============================================================================


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::SendMsgMosSim
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::SendMsgMosSim( ASN1T_MsgsMosSim& asnMsg )
{
    if( !App::GetApp().GetNetwork().IsConnected() )
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

    Send( eMsgMosSim, dinMsg );

    msgRecorder_.OnNewMsg( asnMsg.t, asnPEREncodeBuffer );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::SendMsgMosSimWithContext
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::SendMsgMosSimWithContext( ASN1T_MsgsMosSimWithContext& asnMsg, MIL_MOSContextID nCtx )
{
    if( !App::GetApp().GetNetwork().IsConnected() )
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

    Send( eMsgMosSimWithContext, dinMsg );

    msgRecorder_.OnNewMsgWithContext( asnMsg.t, nCtx, asnPEREncodeBuffer );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::SendMsgMosSim
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::SendMsgMosSim( ASN1OCTET* pMsg, int nMsgLength )
{
    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg.GetOutput().Append( pMsg, nMsgLength );
    Send( eMsgMosSim, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::SendMsgMosSimWithContext
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::SendMsgMosSimWithContext( ASN1OCTET* pMsg, int nMsgLength, MIL_MOSContextID nCtx )
{
    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg << nCtx;

    dinMsg.GetOutput().Append( pMsg, nMsgLength );

    Send( eMsgMosSimWithContext, dinMsg );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlPauseAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlPauseAck( const ASN1T_MsgCtrlPauseAck& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "CtrlPauseAck - Code: " << Tools::ToString( asnMsg );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    bPaused_ = true;
    App::GetApp().NotifyPauseStatusChanged( bPaused_ );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlResumeAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlResumeAck( const ASN1T_MsgCtrlResumeAck& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "CtrlResumeAck - Code: "  << Tools::ToString( asnMsg );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    bPaused_ = false;
    App::GetApp().NotifyPauseStatusChanged( bPaused_ );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlChangeTimeFactorAck
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlChangeTimeFactorAck( const ASN1T_MsgCtrlChangeTimeFactorAck& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "CtrlChangeTimeFactorAck - Facteur temps: " << (int)asnMsg.time_factor << " Code: " << Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    App::GetApp().NotifySpeedChanged( (int)asnMsg.time_factor );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlInfo
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlInfo( const ASN1T_MsgCtrlInfo& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "CtrlInfo - Current Tick: " << asnMsg.current_tick
                 << " - Tick duration : "       << asnMsg.tick_duration
                 << " - Time factor: "          << asnMsg.time_factor
                 << " - Exercice ID: "          << asnMsg.id_exercice
                 << " - Checkpoint Frequency: " << asnMsg.checkpoint_frequence;

    nTickDuration_ = asnMsg.tick_duration;
    nFirstTick_ = asnMsg.current_tick;
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    App::GetApp().NotifySpeedChanged( asnMsg.time_factor );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlBeginTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlBeginTick( const ASN1T_MsgCtrlBeginTick& asnMsg )
{
    //$$$ C'est du caca que de calculer le temps comme ca!
    App::GetApp().NotifyTimeChanged( asnMsg * nTickDuration_ );
    App::GetApp().NotifyTickStartEnd( true, asnMsg );
    msgRecorder_.OnTimeTick( App::GetApp().GetTime() );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlEndTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlEndTick( const ASN1T_MsgCtrlEndTick& asnMsg )
{
    App::GetApp().NotifyTickStartEnd( false, asnMsg.current_tick );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitAttributes
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitAttributes( const ASN1T_MsgUnitAttributes& asnMsg )
{
    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pAgent );
    pAgent->OnAttributeUpdated( asnMsg );
    App::GetApp().NotifyAgentUpdated( *pAgent );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitDotations
// Created: NLD 2003-03-06
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitDotations( const ASN1T_MsgUnitDotations& asnMsg )
{
    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pAgent );
    pAgent->OnAttributeUpdated( asnMsg );
    App::GetApp().NotifyAgentUpdated( *pAgent );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementCreation( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& asnMsg )
{
    App::GetApp().GetAgentManager().RegisterConsign( *new LogMaintenanceConsign( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementDestruction( const ASN1T_MsgLogMaintenanceTraitementEquipementDestruction& asnMsg )
{
    LogMaintenanceConsign* pConsign = App::GetApp().GetAgentManager().FindMaintenanceConsign( asnMsg.oid_consigne );
    assert( pConsign );
    Agent& agent           = pConsign->GetPion();
    Agent* pAgentHandling  = pConsign->GetPionLogHandling();
    App::GetApp().GetAgentManager().DeleteMaintenanceConsign( asnMsg.oid_consigne );
    App::GetApp().NotifyAgentUpdated( agent );
    if( pAgentHandling )
        App::GetApp().NotifyAgentUpdated( *pAgentHandling );

}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementUpdate( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& asnMsg )
{
    LogMaintenanceConsign* pConsign = App::GetApp().GetAgentManager().FindMaintenanceConsign( asnMsg.oid_consigne );
    assert( pConsign );
    pConsign->OnReceiveMsgUpdate( asnMsg );
    App::GetApp().NotifyLogisticConsignUpdated( *pConsign );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceEtat( const ASN1T_MsgLogMaintenanceEtat& asnMsg )
{
    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pAgent );
    pAgent->OnReceiveMsgLogMaintenanceEtat( asnMsg );
    App::GetApp().NotifyAgentUpdated( *pAgent );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainCreation( const ASN1T_MsgLogSanteTraitementHumainCreation& asnMsg )
{
    App::GetApp().GetAgentManager().RegisterConsign( *new LogMedicalConsign( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainDestruction( const ASN1T_MsgLogSanteTraitementHumainDestruction& asnMsg )
{
    LogMedicalConsign* pConsign = App::GetApp().GetAgentManager().FindMedicalConsign( asnMsg.oid_consigne );
    assert( pConsign );
    Agent& agent          = pConsign->GetPion();
    Agent* pAgentHandling = pConsign->GetPionLogHandling();
    App::GetApp().GetAgentManager().DeleteMedicalConsign( asnMsg.oid_consigne );
    App::GetApp().NotifyAgentUpdated( agent );
    if( pAgentHandling )
        App::GetApp().NotifyAgentUpdated( *pAgentHandling );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainUpdate( const ASN1T_MsgLogSanteTraitementHumainUpdate& asnMsg )
{
    LogMedicalConsign* pConsign = App::GetApp().GetAgentManager().FindMedicalConsign( asnMsg.oid_consigne );
    assert( pConsign );
    pConsign->OnReceiveMsgUpdate( asnMsg );
    App::GetApp().NotifyLogisticConsignUpdated( *pConsign );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSanteEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSanteEtat( const ASN1T_MsgLogSanteEtat& asnMsg )
{
    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pAgent );
    pAgent->OnReceiveMsgLogMedicalEtat( asnMsg );
    App::GetApp().NotifyAgentUpdated( *pAgent );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementCreation( const ASN1T_MsgLogRavitaillementTraitementCreation& asnMsg )
{
    App::GetApp().GetAgentManager().RegisterConsign( *new LogSupplyConsign( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementDestruction( const ASN1T_MsgLogRavitaillementTraitementDestruction& asnMsg )
{
    LogSupplyConsign* pConsign = App::GetApp().GetAgentManager().FindSupplyConsign( asnMsg.oid_consigne );
    assert( pConsign );
    Agent& agent           = pConsign->GetPion();
    Agent* pAgentHandling  = pConsign->GetAutomateLogHandling();
    Agent* pAgentConvoying = pConsign->GetPionLogConvoying();
    App::GetApp().GetAgentManager().DeleteSupplyConsign( asnMsg.oid_consigne );
    App::GetApp().NotifyAgentUpdated( agent );
    if( pAgentHandling )
        App::GetApp().NotifyAgentUpdated( *pAgentHandling );
    if( pAgentConvoying )
        App::GetApp().NotifyAgentUpdated( *pAgentConvoying );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementUpdate( const ASN1T_MsgLogRavitaillementTraitementUpdate& asnMsg )
{
    LogSupplyConsign* pConsign = App::GetApp().GetAgentManager().FindSupplyConsign( asnMsg.oid_consigne );
    assert( pConsign );
    pConsign->OnReceiveMsgUpdate( asnMsg );
    App::GetApp().NotifyLogisticConsignUpdated( *pConsign );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementEtat( const ASN1T_MsgLogRavitaillementEtat&  asnMsg )
{
    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pAgent );
    pAgent->OnReceiveMsgLogSupplyEtat( asnMsg );
    App::GetApp().NotifyAgentUpdated( *pAgent );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementQuotas
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementQuotas( const ASN1T_MsgLogRavitaillementQuotas& asnMsg )
{
    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
    assert( pAgent );
    pAgent->OnReceiveMsgLogSupplyQuotas( asnMsg );
    App::GetApp().NotifyAgentUpdated( *pAgent );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck( const ASN1T_MsgLogRavitaillementChangeQuotasAck& asnMsg, MIL_MOSContextID )
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
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementPousserFluxAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementPousserFluxAck( const ASN1T_MsgLogRavitaillementPousserFluxAck& asnMsg, MIL_MOSContextID )
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
// Name: AgentServerMsgMgr::OnReceiveMsgUnitPathFind
// Created: NLD 2003-03-31
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitPathFind( const ASN1T_MsgUnitPathFind& asnMsg )
{
    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pAgent );

    pAgent->OnReceiveMsgPathFind( asnMsg );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitMagicActionAck( const ASN1T_MsgUnitMagicActionAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "UnitMagicActionAck - Code: " << Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectMagicActionAck( const ASN1T_MsgObjectMagicActionAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "ObjectMagicActionAck - Code: " << Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlMeteoGlobalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlMeteoGlobalAck()
{
    MT_LOG_INFO( "CtrlMeteoGlobaleAck", eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlMeteoLocalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlMeteoLocalAck()
{
    MT_LOG_INFO( "CtrlMeteoLocaleAck", eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateBegin
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateBegin()
{
    MT_LOG_INFO( "CtrlSendCurrentStateBegin", eReceived, 0 );
    bReceivingState_ = true;
    bUseMosLimits_ = true;
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateEnd
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateEnd()
{
    MT_LOG_INFO( "CtrlSendCurrentStateEnd", eReceived, 0 );
    bReceivingState_ = false;

    // If we are using our own lines, upload them to the sim.
    if( bUseMosLimits_ )
        App::GetApp().GetLineManager().UpdateToSim();
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSaveBegin
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSaveBegin()
{
    MT_LOG_INFO( "CtrlCheckPointSaveBegin", eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSaveEnd
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSaveEnd()
{
    MT_LOG_INFO( "CtrlCheckPointSaveEnd", eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointLoadBegin
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointLoadBegin()
{
    MT_LOG_INFO( "CtrlCheckPointBeginEnd", eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointLoadEnd
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointLoadEnd()
{
    MT_LOG_INFO( "CtrlCheckPointLoadEnd", eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSetFrequencyAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSetFrequencyAck()
{
    MT_LOG_INFO( "CtrlCheckPointSetFrequencyAck", eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck()
{
    MT_LOG_INFO( "CtrlCheckPointSetSaveNowAck", eReceived, 0 );
}


//=============================================================================
// LIMAS / LIMITS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitCreationAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitCreationAck( const ASN1T_MsgLimitCreationAck& asnMsg, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimitCreationAck - Code: " << Tools::ToString( asnMsg.error_code ) << " (id: " << asnMsg.oid << ")";
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        TacticalLine_ABC* pLimit = App::GetApp().GetLineManager().FindLine( asnMsg.oid );
        if( pLimit )
        {
            pLimit->SetNetworkState( TacticalLine_ABC::eNetworkStateRegistered );
            pLimit->SetState( TacticalLine_ABC::eStateOk );
        }
        else
            Limit::idManager_.LockIdentifier( asnMsg.oid );
    }
    else
    {

    }
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitUpdateAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitUpdateAck( const ASN1T_MsgLimitUpdateAck& asnMsg, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimitUpdateAck - Code: " << Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        TacticalLine_ABC* pLimit = App::GetApp().GetLineManager().FindLine( asnMsg.oid );
        if( pLimit )
        {
            pLimit->SetNetworkState( TacticalLine_ABC::eNetworkStateRegistered );
            pLimit->SetState( TacticalLine_ABC::eStateOk );
        }
        // If the limit was not found, it's a message for another MOS
    }
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitDestructionAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitDestructionAck( const ASN1T_MsgLimitDestructionAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimitDestructionAck - Code: " << Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        TacticalLine_ABC* pLimit = App::GetApp().GetLineManager().FindLine( asnMsg.oid );
        if( pLimit )
        {
            App::GetApp().NotifyTacticalLineDeleted( *pLimit );
            App::GetApp().GetLineManager().DeleteLine( asnMsg.oid );
        }
//        else
//            Limit::idManager_.ReleaseIdentifier( asnMsg.oid );
    }
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaCreationAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaCreationAck( const ASN1T_MsgLimaCreationAck& asnMsg, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimaCreationAck - Code: " << Tools::ToString( asnMsg.error_code ) << " (id: " << asnMsg.oid << ")";
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        TacticalLine_ABC* pLima = App::GetApp().GetLineManager().FindLine( asnMsg.oid );
        if( pLima )
        {
            pLima->SetNetworkState( TacticalLine_ABC::eNetworkStateRegistered );
            pLima->SetState( TacticalLine_ABC::eStateOk );
        }
        else
        {
            // If the limit was not found, it's a message for another MOS. Lock the identifier.
            Lima::idManager_.LockIdentifier( asnMsg.oid );
        }
    }
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaUpdateAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaUpdateAck( const ASN1T_MsgLimaUpdateAck& asnMsg, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimaUpdateAck - Code: " << Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        TacticalLine_ABC* pLima = App::GetApp().GetLineManager().FindLine( asnMsg.oid );
        if( pLima )
        {
            pLima->SetNetworkState( TacticalLine_ABC::eNetworkStateRegistered );
            pLima->SetState( TacticalLine_ABC::eStateOk );
        }
        // If the limit was not found, it's a message for another MOS.
    }
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaDestructionAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaDestructionAck( const ASN1T_MsgLimaDestructionAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimaDestructionAck - Code: " << Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        TacticalLine_ABC* pLima = App::GetApp().GetLineManager().FindLine( asnMsg.oid );
        if( pLima )
        {
            App::GetApp().NotifyTacticalLineDeleted( *pLima );
            App::GetApp().GetLineManager().DeleteLine( asnMsg.oid );
        }
//        else
//            Lima::idManager_.ReleaseIdentifier( asnMsg.oid );
    }
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitCreation
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitCreation( const ASN1T_MsgLimitCreation& asnMsg )
{
    // If we are receiving those lines while initializing from the sim, use those lines instead of our lines.
    if( bReceivingState_  && bUseMosLimits_ )
    {
        bUseMosLimits_ = false;
        App::GetApp().GetLineManager().UseSimTacticalLines();
    }

    Limit* pLimit = new Limit( asnMsg );
    App::GetApp().GetLineManager().RegisterLine( *pLimit );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitDestruction( const ASN1T_MsgLimitDestruction& asnMsg )
{
    TacticalLine_ABC* pLimit = App::GetApp().GetLineManager().FindLine( asnMsg );
    if( pLimit )
    {
        App::GetApp().NotifyTacticalLineDeleted( *pLimit );
        App::GetApp().GetLineManager().DeleteLine( asnMsg );
    }
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaCreation
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaCreation( const ASN1T_MsgLimaCreation& asnMsg )
{
    // If we are receiving those lines while initializing from the sim, use those lines instead of our lines.
    if( bReceivingState_  && bUseMosLimits_ )
    {
        bUseMosLimits_ = false;
        App::GetApp().GetLineManager().UseSimTacticalLines();
    }

    Lima* pLimit = new Lima( asnMsg );
    App::GetApp().GetLineManager().RegisterLine( *pLimit );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaDestruction( const ASN1T_MsgLimaDestruction& asnMsg )
{
    TacticalLine_ABC* pLima = App::GetApp().GetLineManager().FindLine( asnMsg );
    if( pLima )
    {
        App::GetApp().NotifyTacticalLineDeleted( *pLima );
        App::GetApp().GetLineManager().DeleteLine( asnMsg );
    }
}

//=============================================================================
// ORDRES
//=============================================================================

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomateOrderAck
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomateOrderAck( const ASN1T_MsgAutomateOrderAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "AutomateOrderAck - Code: " << Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

//APE    App::GetApp().GetMainWindow().GetAgentEditor().NotifyAgentMissionAck( asnMsg.error_code );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPionOrderAck
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPionOrderAck( const ASN1T_MsgPionOrderAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << MT_FormatString( "Agent[%d]", asnMsg.oid_unite_executante );
    strOutputMsg << "PionOrderAck - Code: " << Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );
    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_unite_executante );
    pAgent->OnReceiveMission( asnMsg );

}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgOrderConduiteAck
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgOrderConduiteAck( const ASN1T_MsgOrderConduiteAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "OrderConduiteAck - Code: " << Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCR
// Created: NLD 2002-09-02
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCR( const ASN1T_MsgCR& asnMsg )
{
    Agent_ABC* pAgent = App::GetApp().GetAgentManager().FindAllAgent( asnMsg.unit_id );
    assert( pAgent );
    pAgent->OnReceiveMsgCR( asnMsg );

}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgOrderManagement
// Created: NLD 2003-03-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgOrderManagement( const ASN1T_MsgOrderManagement& asnMsg )
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
// Name: AgentServerMsgMgr::OnReceiveMsgAutomateMRT
// Created: NLD 2003-05-13
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomateMRT( const ASN1T_MsgAutomateMRT& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "AutomateMRT - Automate " << asnMsg.automate_id;

    std::stringstream strOutputMsg2;
    for( uint i = 0; i < asnMsg.missions.n; ++i )
    {
        ASN1T_MsgPionOrder& asnPionOrder = asnMsg.missions.elem[i];

        strOutputMsg2 << "Pion " << asnPionOrder.oid_unite_executante
                     << " - Mission " << asnPionOrder.mission.t << endl;

        Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( asnPionOrder.oid_unite_executante );
        pAgent->OnReceiveMission( asnPionOrder );

    }
    MT_LOG_INFO( strOutputMsg.str(), eReceived, strOutputMsg2.str().c_str() );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPionOrder
// Created: NLD 2003-10-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPionOrder( const ASN1T_MsgPionOrder& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "PionOrder - Mission " << asnMsg.mission.t;
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );
    Agent* pAgent = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_unite_executante );
    pAgent->OnReceiveMission( asnMsg );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgSetAutomateModeAck
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgSetAutomateModeAck( const ASN1T_MsgSetAutomateModeAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
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
// Name: AgentServerMsgMgr::OnReceiveMsgChangeAutomateAck
/** @param  asnMsg
    @param  nCtx
    */
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeAutomateAck( const ASN1T_MsgChangeAutomateAck& asnMsg, MIL_MOSContextID )
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
        Agent* pPion = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
        assert( pPion );
        Agent* pAutomate = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
        assert( pAutomate );

        pPion->OnSuperiorChanged( *pAutomate );
        App::GetApp().NotifyAgentReparented( *pPion );
    }

    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgChangeAutomate
/** @param  asnMsg
*/
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeAutomate( const ASN1T_MsgChangeAutomate& asnMsg )
{
    Agent* pPion = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pPion );

    Agent* pAutomate = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
    assert( pAutomate );

    pPion->OnSuperiorChanged( *pAutomate );
    App::GetApp().NotifyAgentReparented( *pPion );

    std::stringstream strOutputMsg;
    strOutputMsg << "ChangeAutomate - Pion: " << pPion->GetName() << " - Automate: " << pAutomate->GetName();
    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgChangeDiplomacyAck
/** @param  asnMsg
    @param  nCtx
    */
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeDiplomacyAck( const ASN1T_MsgChangeDiplomatieAck& asnMsg, MIL_MOSContextID )
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
        Team* pTeam1 = App::GetApp().GetAgentManager().FindTeam( asnMsg.oid_camp1 );
        assert( pTeam1 );
        Team* pTeam2 = App::GetApp().GetAgentManager().FindTeam( asnMsg.oid_camp2 );
        assert( pTeam2 );

        pTeam1->ChangeDiplomaty( *pTeam2, asnMsg.diplomatie );
    }

    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgChangeGroupeConnaissanceAck
/** @param  asnMsg
    @param  nCtx
    */
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeGroupeConnaissanceAck( const ASN1T_MsgChangeGroupeConnaissanceAck& asnMsg, MIL_MOSContextID )
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
        Agent* pAutomate = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
        assert( pAutomate );
        Gtia* pGtia = App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_connaissance );
        assert( pGtia );

        pAutomate->OnSuperiorChanged( *pGtia );
        App::GetApp().NotifyAgentReparented( *pAutomate );
    }

    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgChangeLiensLogistiquesAck
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeLiensLogistiquesAck( const ASN1T_MsgChangeLiensLogistiquesAck& asnMsg, MIL_MOSContextID )
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


    Agent* pAutomate = App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
    assert( pAutomate );
    pAutomate->OnLogisticLinksUpdated( asnMsg );
    App::GetApp().NotifyAgentUpdated( *pAutomate );
}

// =============================================================================
// KNOWLEDGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeCreation( const ASN1T_MsgUnitKnowledgeCreation& asnMsg )
{
    Gtia* pGtia = App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur );
    assert( pGtia );
    if( !pGtia->OnReceiveMsgUnitKnowledgeCreation( asnMsg ) )
        MT_LOG_ERROR_MSG( "Duplicate agent knowledge #" + QString::number( asnMsg.oid_connaissance ) );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeUpdate
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeUpdate( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg )
{
    Gtia* pGtia = App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur );
    assert( pGtia );
    pGtia->OnReceiveMsgUnitKnowledgeUpdate( asnMsg );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg )
{
    Gtia* pGtia = App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur );
    assert( pGtia );
    pGtia->OnReceiveMsgUnitKnowledgeDestruction( asnMsg );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeCreation
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeCreation( const ASN1T_MsgObjectKnowledgeCreation& asnMsg )
{
    Team* pTeam = App::GetApp().GetAgentManager().FindTeam( asnMsg.oid_camp_possesseur );
    assert( pTeam );
    if( !pTeam->OnReceiveMsgObjectKnowledgeCreation( asnMsg ) )
        MT_LOG_ERROR_MSG( "Duplicate object knowledge #" + QString::number( asnMsg.oid_connaissance ) );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeUpdate
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeUpdate( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    Team* pTeam = App::GetApp().GetAgentManager().FindTeam( asnMsg.oid_camp_possesseur );
    assert( pTeam );
    pTeam->OnReceiveMsgObjectKnowledgeUpdate( asnMsg );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeDestruction
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg )
{
    Team* pTeam = App::GetApp().GetAgentManager().FindTeam( asnMsg.oid_camp_possesseur );
    assert( pTeam );
    pTeam->OnReceiveMsgObjectKnowledgeDestruction( asnMsg );
}

// =============================================================================
// POPULATION KNOWLEDGE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeCreation
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeCreation( const ASN1T_MsgPopulationKnowledgeCreation& asnMsg )
{
    Gtia* pGtia = App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur );
    assert( pGtia );
    if( !pGtia->OnReceiveMsgPopulationKnowledgeCreation( asnMsg ) )
        MT_LOG_ERROR_MSG( "Duplicate population knowledge #" + QString::number( asnMsg.oid_connaissance ) );
}
    
// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeUpdate( const ASN1T_MsgPopulationKnowledgeUpdate& asnMsg )
{
    Gtia* pGtia = App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur );
    assert( pGtia );
    pGtia->OnReceiveMsgPopulationKnowledgeUpdate( asnMsg );
}
    
// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeDestruction
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeDestruction( const ASN1T_MsgPopulationKnowledgeDestruction& asnMsg )
{
    Gtia* pGtia = App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur );
    assert( pGtia );
    pGtia->OnReceiveMsgPopulationKnowledgeDestruction( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeCreation
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeCreation( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& asnMsg )
{
    Gtia* pGtia = App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur );
    assert( pGtia );
    PopulationKnowledge* pKnowledge = pGtia->FindPopulationKnowledge( asnMsg.oid_connaissance_population );
    assert( pKnowledge );
    pKnowledge->OnReceiveMsgPopulationConcentrationKnowledgeCreation( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeUpdate
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& asnMsg )
{
    Gtia* pGtia = App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur );
    assert( pGtia );
    PopulationKnowledge* pKnowledge = pGtia->FindPopulationKnowledge( asnMsg.oid_connaissance_population );
    assert( pKnowledge );
    pKnowledge->OnReceiveMsgPopulationConcentrationKnowledgeUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeDestruction
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeDestruction( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& asnMsg )
{
    Gtia* pGtia = App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur );
    assert( pGtia );
    PopulationKnowledge* pKnowledge = pGtia->FindPopulationKnowledge( asnMsg.oid_connaissance_population );
    assert( pKnowledge );
    pKnowledge->OnReceiveMsgPopulationConcentrationKnowledgeDestruction( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeCreation
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeCreation( const ASN1T_MsgPopulationFluxKnowledgeCreation& asnMsg )
{
    Gtia* pGtia = App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur );
    assert( pGtia );
    PopulationKnowledge* pKnowledge = pGtia->FindPopulationKnowledge( asnMsg.oid_connaissance_population );
    assert( pKnowledge );
    pKnowledge->OnReceiveMsgPopulationFlowKnowledgeCreation( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeUpdate
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeUpdate( const ASN1T_MsgPopulationFluxKnowledgeUpdate& asnMsg )
{
    Gtia* pGtia = App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur );
    assert( pGtia );
    PopulationKnowledge* pKnowledge = pGtia->FindPopulationKnowledge( asnMsg.oid_connaissance_population );
    assert( pKnowledge );
    pKnowledge->OnReceiveMsgPopulationFlowKnowledgeUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeDestruction
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeDestruction( const ASN1T_MsgPopulationFluxKnowledgeDestruction& asnMsg )
{
    Gtia* pGtia = App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur );
    assert( pGtia );
    PopulationKnowledge* pKnowledge = pGtia->FindPopulationKnowledge( asnMsg.oid_connaissance_population );
    assert( pKnowledge );
    pKnowledge->OnReceiveMsgPopulationFlowKnowledgeDestruction( asnMsg );
}


//=============================================================================
// OBJECTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectCreation
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectCreation( const ASN1T_MsgObjectCreation& asnMsg )
{
    ObjectManager& objectManager = App::GetApp().GetObjectManager();
    objectManager.CreateObject( asnMsg );
    MT_LOG_INFO( "ObjectCreation - ID: " << asnMsg.oid, eReceived, 0 );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectUpdate
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectUpdate( const ASN1T_MsgObjectUpdate& asnMsg )
{
    Object_ABC* pObject = App::GetApp().GetObjectManager().FindObject( asnMsg.oid );
    assert( pObject );

    pObject->Update( asnMsg );
    App::GetApp().NotifyObjectUpdated( *pObject );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectDestruction
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectDestruction( const ASN1T_MsgObjectDestruction& asnMsg )
{
    ObjectManager& ObjectManager = App::GetApp().GetObjectManager();

    Object_ABC* pObject = ObjectManager.FindObject( asnMsg );
    assert( pObject );

    App::GetApp().NotifyObjectDeleted( *pObject );

    ObjectManager.UnregisterObject( *pObject );
    delete pObject;         //$$$ TMP - devrait être fait par le manager

    MT_LOG_INFO( "ObjectDestruction - ID: " << asnMsg, eReceived, 0 );
}

//=============================================================================
// TIRS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStartPionFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStartPionFire( const ASN1T_MsgStartPionFire& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "StartPionFire - ID: " << asnMsg.oid_tir
                 << " - ID source " << asnMsg.tireur;

    AgentManager& agentManager = App::GetApp().GetAgentManager();
    Agent* pAgentSrc = agentManager.FindAgent( asnMsg.tireur );
    assert( pAgentSrc );

    if( asnMsg.m.munitionPresent )
        strOutputMsg << " - Munition : " << App::GetApp().GetResourceName( asnMsg.munition );

    if( asnMsg.cible.t == T_MsgStartPionFire_cible_pion )
    {
        strOutputMsg << " - ID pion cible " << asnMsg.cible.u.pion;
        
        Agent* pTarget = agentManager.FindAgent( asnMsg.cible.u.pion );
        assert( pTarget );
        agentManager.AddDirectConflict( asnMsg.oid_tir, *pAgentSrc, *pTarget );
    }
    else if( asnMsg.cible.t == T_MsgStartPionFire_cible_position )
    {
        std::string strTmp( (const char*)asnMsg.cible.u.position->data, asnMsg.cible.u.position->numocts );
        MT_Vector2D vPos;
        App::GetApp().GetWorld().MosToSimMgrsCoord( strTmp, vPos );
        strOutputMsg << " - Position " << strTmp;
        agentManager.AddIndirectConflict( asnMsg.oid_tir, *pAgentSrc, vPos );
    }
    else if( asnMsg.cible.t == T_MsgStartPionFire_cible_population )
    {
        strOutputMsg << " - ID population cible " << asnMsg.cible.u.population;
        
        Population* pTarget = agentManager.FindPopulation( asnMsg.cible.u.population );
        assert( pTarget );
        agentManager.AddDirectConflict( asnMsg.oid_tir, *pAgentSrc, *pTarget );
    }

    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );   
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStopPionFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStopPionFire( const ASN1T_MsgStopPionFire& asnMsg )
{
    MT_LOG_INFO( "StopPionFire - ID: " << asnMsg.oid_tir, eReceived, 0 );

    // fire results
    Agent_ABC* pOrigin = App::GetApp().GetAgentManager().FindConflictOrigin( asnMsg.oid_tir );

    if( !pOrigin ) // $$$$ SBO 2005-12-06: happens when Light2 is disconnected after a StartFire
        return;
    for( uint i = 0; i < asnMsg.degats_pions.n; ++i )
        pOrigin->OnReceiveMsgStopFire( asnMsg.degats_pions.elem[ i ] );
    for( uint i = 0; i < asnMsg.degats_populations.n; ++i )
        pOrigin->OnReceiveMsgStopFire( asnMsg.degats_populations.elem[ i ] );

    App::GetApp().GetAgentManager().DeleteConflict( asnMsg.oid_tir );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStartFireEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStartFireEffect( const ASN1T_MsgStartFireEffect& asnMsg )
{
    MT_LOG_INFO( "Start ammunition meteo effect - ID: " << asnMsg.oid_effet << " Type:" << asnMsg.type, eReceived, 0 );
    App::GetApp().GetWeatherManager().RegisterAmmoMeteoEffect( asnMsg );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStopFireEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStopFireEffect( const ASN1T_MsgStopFireEffect& asnMsg )
{
    MT_LOG_INFO( "Stop ammunition meteo effect - ID: " << asnMsg, eReceived, 0 );
    App::GetApp().GetWeatherManager().UnregisterAmmoMeteoEffect( asnMsg );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgExplosion
// Created: NLD 2003-05-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgExplosion( const ASN1T_MsgExplosion& asnMsg )
{
    // fire results
    Object_ABC* pObject = App::GetApp().GetObjectManager().FindObject( asnMsg.oid_objet );
    assert( pObject );
    for( uint i = 0; i < asnMsg.degats_pions.n; ++i )
        pObject->OnReceiveMsgExplosion( asnMsg.degats_pions.elem[ i ] );

    App::GetApp().NotifyObjectExplosion( *pObject );
    MT_LOG_INFO( "Explosion" << " - ID objet " << asnMsg.oid_objet, eReceived, 0 );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStartPopulationFire
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStartPopulationFire( const ASN1T_MsgStartPopulationFire& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "StartPopulationFire - ID: " << asnMsg.oid_tir
                 << " - ID source " << asnMsg.oid_src;

    AgentManager& agentManager = App::GetApp().GetAgentManager();
    Population* pPopulationSrc = agentManager.FindPopulation( asnMsg.oid_src );
    assert( pPopulationSrc );

    agentManager.AddIndirectConflict( asnMsg.oid_tir, *pPopulationSrc, pPopulationSrc->GetPos() );
}
    
// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStopPopulationFire
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStopPopulationFire( const ASN1T_MsgStopPopulationFire&  asnMsg )
{
    MT_LOG_INFO( "StopPopulationFire - ID: " << asnMsg.oid_tir, eReceived, 0 );

    // fire results
    Agent_ABC* pOrigin = App::GetApp().GetAgentManager().FindConflictOrigin( asnMsg.oid_tir );

    if( !pOrigin ) // $$$$ SBO 2005-12-06: happens when Light2 is disconnected after a StartFire
        return;
    for( uint i = 0; i < asnMsg.degats_pions.n; ++i )
        pOrigin->OnReceiveMsgStopFire( asnMsg.degats_pions.elem[ i ] );
    /* population => population
    for( uint i = 0; i < asnMsg.degats_populations.n; ++i )
        pAgent->OnReceiveMsgStopFire( asnMsg.degats_populations.elem[ i ] );
    */

    App::GetApp().GetAgentManager().DeleteConflict( asnMsg.oid_tir );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgChangeDiplomatie
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeDiplomatie( const ASN1T_MsgChangeDiplomatie& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : ChangeDiplomatie - ";

    Team* pArmy = App::GetApp().GetAgentManager().FindTeam( asnMsg.oid_camp1 );
    assert( pArmy );
    strOutputMsg << pArmy->GetName() << " -> ";

    pArmy = App::GetApp().GetAgentManager().FindTeam( asnMsg.oid_camp2 );
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
// Name: AgentServerMsgMgr::OnMsgPopulationCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationCreation( const ASN1T_MsgPopulationCreation& asnMsg )
{
	std::stringstream strOutputMsg;
    strOutputMsg << "Creation de Population"
                 << " - ID  " << asnMsg.oid_population;
	MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	App::GetApp().GetAgentManager().CreatePopulation( asnMsg );

}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationUpdate( const ASN1T_MsgPopulationUpdate& asnMsg )
{
	//std::stringstream strOutputMsg;
    //strOutputMsg << "Population Update"
    //             << " - ID  " << asnMsg.oid_population;
	//MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	Population* pop = App::GetApp().GetAgentManager().FindPopulation( asnMsg.oid_population );
	if ( pop != 0 )
		pop->UpdatePopulation( asnMsg );
    App::GetApp().NotifyPopulationUpdated( *pop );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationConcentrationCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationConcentrationCreation( const ASN1T_MsgPopulationConcentrationCreation& asnMsg )
{
	std::stringstream strOutputMsg;
    strOutputMsg << "Creation d'une concentration"
                 << " - ID  " << asnMsg.oid_concentration;
	MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	Population* pop = App::GetApp().GetAgentManager().FindPopulation( asnMsg.oid_population );
	if ( pop != 0 )
		pop->CreatePopulationConcentration( asnMsg );
    const PopulationConcentration* concentration = pop->FindConcentration( asnMsg.oid_concentration );
    if( concentration != 0 )
        App::GetApp().NotifyPopulationConcentrationCreated( *concentration );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationConcentrationDestruction
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationConcentrationDestruction( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg )
{
	std::stringstream strOutputMsg;
    strOutputMsg << "Destruction d'une concentration"
                 << " - ID  " << asnMsg.oid_population;
	MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	Population* pop = App::GetApp().GetAgentManager().FindPopulation( asnMsg.oid_population );
    if ( pop != 0 )
    {
        const PopulationConcentration* concentration = pop->FindConcentration( asnMsg.oid_concentration );
        if( concentration != 0 )
            App::GetApp().NotifyPopulationConcentrationDeleted( *concentration );
		pop->DeletePopulationConcentration( asnMsg );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationConcentrationUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationConcentrationUpdate( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
	//std::stringstream strOutputMsg;
    //strOutputMsg << "Update d'une concentration"
    //             << " - ID  " << asnMsg.oid_concentration;
	//MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	Population* pop = App::GetApp().GetAgentManager().FindPopulation( asnMsg.oid_population );
	if ( pop != 0 )
		pop->UpdatePopulationConcentration( asnMsg );
    const PopulationConcentration* concentration = pop->FindConcentration( asnMsg.oid_concentration );
    if( concentration != 0 )
        App::GetApp().NotifyPopulationConcentrationUpdated( *concentration );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationFluxCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationFluxCreation( const ASN1T_MsgPopulationFluxCreation& asnMsg )
{
	std::stringstream strOutputMsg;
    strOutputMsg << "Creation d'un flux"
                 << " - ID  " << asnMsg.oid_flux;
	MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	Population* pop = App::GetApp().GetAgentManager().FindPopulation( asnMsg.oid_population );
	if ( pop != 0 )
		pop->CreatePopulationFlow( asnMsg );
    const PopulationFlow* flow = pop->FindFlow( asnMsg.oid_flux );
    if( flow != 0 )
        App::GetApp().NotifyPopulationFlowCreated( *flow );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationFluxDestruction
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationFluxDestruction  ( const ASN1T_MsgPopulationFluxDestruction& asnMsg )
{
	std::stringstream strOutputMsg;
    strOutputMsg << "Destruction d'un flux"
                 << " - ID  " << asnMsg.oid_flux;
	MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	Population* pop = App::GetApp().GetAgentManager().FindPopulation( asnMsg.oid_population );
    if ( pop != 0 )
    {
        const PopulationFlow* flow = pop->FindFlow( asnMsg.oid_flux );
        if( flow != 0 )
            App::GetApp().NotifyPopulationFlowDeleted( *flow );
		pop->DeletePopulationFlow( asnMsg );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationFluxUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationFluxUpdate( const ASN1T_MsgPopulationFluxUpdate& asnMsg )
{
	//std::stringstream strOutputMsg;
    //strOutputMsg << "Update d'un flux"
    //             << " - ID  " << asnMsg.oid_flux;
	//MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
	Population* pop = App::GetApp().GetAgentManager().FindPopulation( asnMsg.oid_population );
	if ( pop != 0 )
		pop->UpdatePopulationFlow( asnMsg );
    const PopulationFlow* flow = pop->FindFlow( asnMsg.oid_flux );
    if( flow != 0 )
        App::GetApp().NotifyPopulationFlowUpdated( *flow );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationMagicActionAck
// Created: SBO 2005-11-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationMagicActionAck( const ASN1T_MsgPopulationMagicActionAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "PopulationMagicActionAck - Code: " << Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );
}

//=============================================================================
// ASN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr constructor
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgSimMos( DIN_Link& /*linkFrom*/, DIN_Input& input )
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

        case T_MsgsSimMos_msg_start_pion_fire:                      OnReceiveMsgStartPionFire             ( *asnMsg.u.msg_start_pion_fire                     ); break;
        case T_MsgsSimMos_msg_stop_pion_fire:                       OnReceiveMsgStopPionFire              ( *asnMsg.u.msg_stop_pion_fire                      ); break;
        case T_MsgsSimMos_msg_start_population_fire:                OnReceiveMsgStartPopulationFire       ( *asnMsg.u.msg_start_population_fire               ); break;
        case T_MsgsSimMos_msg_stop_population_fire:                 OnReceiveMsgStopPopulationFire        ( *asnMsg.u.msg_stop_population_fire                ); break;

        case T_MsgsSimMos_msg_explosion:                            OnReceiveMsgExplosion                 ( *asnMsg.u.msg_explosion                           ); break;
        case T_MsgsSimMos_msg_cr:                                   OnReceiveMsgCR                        ( *asnMsg.u.msg_cr                                  ); break;
        case T_MsgsSimMos_msg_start_fire_effect:                    OnReceiveMsgStartFireEffect           ( *asnMsg.u.msg_start_fire_effect ); break;
        case T_MsgsSimMos_msg_stop_fire_effect:                     OnReceiveMsgStopFireEffect            ( asnMsg.u.msg_stop_fire_effect ); break;

        case T_MsgsSimMos_msg_order_management:                     OnReceiveMsgOrderManagement           ( *asnMsg.u.msg_order_management                    ); break;
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

        case T_MsgsSimMos_msg_population_knowledge_creation                  : OnReceiveMsgPopulationKnowledgeCreation                ( *asnMsg.u.msg_population_knowledge_creation                  ); break;
        case T_MsgsSimMos_msg_population_knowledge_update                    : OnReceiveMsgPopulationKnowledgeUpdate                  ( *asnMsg.u.msg_population_knowledge_update                    ); break;
        case T_MsgsSimMos_msg_population_knowledge_destruction               : OnReceiveMsgPopulationKnowledgeDestruction             ( *asnMsg.u.msg_population_knowledge_destruction               ); break;
        case T_MsgsSimMos_msg_population_concentration_knowledge_creation    : OnReceiveMsgPopulationConcentrationKnowledgeCreation   ( *asnMsg.u.msg_population_concentration_knowledge_creation    ); break;
        case T_MsgsSimMos_msg_population_concentration_knowledge_update      : OnReceiveMsgPopulationConcentrationKnowledgeUpdate     ( *asnMsg.u.msg_population_concentration_knowledge_update      ); break;
        case T_MsgsSimMos_msg_population_concentration_knowledge_destruction : OnReceiveMsgPopulationConcentrationKnowledgeDestruction( *asnMsg.u.msg_population_concentration_knowledge_destruction ); break;
        case T_MsgsSimMos_msg_population_flux_knowledge_creation             : OnReceiveMsgPopulationFlowKnowledgeCreation            ( *asnMsg.u.msg_population_flux_knowledge_creation             ); break;
        case T_MsgsSimMos_msg_population_flux_knowledge_update               : OnReceiveMsgPopulationFlowKnowledgeUpdate              ( *asnMsg.u.msg_population_flux_knowledge_update               ); break;
        case T_MsgsSimMos_msg_population_flux_knowledge_destruction          : OnReceiveMsgPopulationFlowKnowledgeDestruction         ( *asnMsg.u.msg_population_flux_knowledge_destruction          ); break;

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
// Name: AgentServerMsgMgr::OnReceiveMsgSimMosWithContext
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgSimMosWithContext( DIN_Link& /*linkFrom*/, DIN_Input& input )
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
        case T_MsgsSimMosWithContext_msg_population_magic_action_ack:            OnReceiveMsgPopulationMagicActionAck        ( *asnMsg.u.msg_population_magic_action_ack         , nCtx ); break;

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
// Name: AgentServerMsgMgr::OnError
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
bool AgentServerMsgMgr::OnError( DIN::DIN_Link& /*link*/, const DIN::DIN_ErrorDescription& /*info*/ )
{
    //MT_LOG_INFO_MSG( "MOS -> AS - Message service error : " << info.GetInfo().c_str() );
    return false;
}

