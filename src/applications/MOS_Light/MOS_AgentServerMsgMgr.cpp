//*****************************************************************************
// 
// $Created: NLD 2002-07-12 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentServerMsgMgr.cpp $
// $Author: Nld $
// $Modtime: 5/04/05 11:18 $
// $Revision: 128 $
// $Workfile: MOS_AgentServerMsgMgr.cpp $
// 
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_AgentServerMsgMgr.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentServerMsgMgr.inl"
#endif

#include "MOS_AgentServerController.h"
#include "MOS_AgentServerConnectionMgr.h"

#include "MOS_Agent.h"
#include "MOS_AgentManager.h"
#include "MOS_DynaObject.h"
#include "MOS_DynaObjectManager.h"
#include "MOS_App.h"
#include "MOS_MOSServer.h"
#include "MOS_LineManager.h"
#include "MOS_Lima.h"
#include "MOS_Limit.h"
#include "MOS_World.h"
#include "MOS_AgentEditor.h"
#include "MOS_SimControl.h"
#include "MOS_ASN_Messages.h"
#include "MOS_Net_Def.h"
#include "MOS_AgentListView.h"
#include "MOS_Flot.h"
#include "MOS_History.h"
#include "MOS_TestManager.h"
#include "MOS_WorldWidget.h"
#include "MOS_GLWidget.h"
#include "MOS_Gtia.h"
#include "MOS_Team.h"
#include "MOS_LogMaintenanceConsign.h"
#include "MOS_LogSupplyConsign.h"
#include "MOS_LogMedicalConsign.h"

//$$$ TMP
#include "MOS_MainWindow.h"

#include <sstream>

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
MOS_AgentServerMsgMgr::MOS_AgentServerMsgMgr( MOS_AgentServerController& controller )
    : MOS_AgentServerMgr_ABC( controller )
    , nCurrentServerTime_   ( 0 )
    , bPaused_              ( false )
{
    DIN_ConnectorGuest connector( eConnector_SIM_MOS );
    pMessageService_ = new DIN_MessageServiceUserCbk<MOS_AgentServerMsgMgr>( *this, controller.GetDINEngine(), connector, "Msgs MOS Server -> Agent Server" );

    pMessageService_->RegisterReceivedMessage( eMsgInit                      , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgInit                );
    pMessageService_->RegisterReceivedMessage( eMsgProfilingValues           , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgProfilingValues     );

    pMessageService_->RegisterReceivedMessage( eMsgUnitTrace              , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgUnitTrace             );
    pMessageService_->RegisterReceivedMessage( eMsgUnitVisionCones        , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgUnitVisionCones       );
    pMessageService_->RegisterReceivedMessage( eMsgUnitInterVisibility    , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility   );
    pMessageService_->RegisterReceivedMessage( eMsgObjectInterVisibility  , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility );
    pMessageService_->RegisterReceivedMessage( eMsgKnowledgeGroup         , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgKnowledgeGroup        );
    pMessageService_->RegisterReceivedMessage( eMsgArmy                   , *this, & MOS_AgentServerMsgMgr::OnReceiveMsgArmy                  );
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
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::SendMsgEnableProfiling
// Created: NLD 2005-03-15
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::SendMsgEnableProfiling()
{
    MOS_AgentServer& agentServer = controller_.GetConnectionMgr().GetAgentServer();
    pMessageService_->Send( agentServer.GetSession(), eMsgEnableProfiling );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::SendMsgDisableProfiling
// Created: NLD 2005-03-15
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::SendMsgDisableProfiling()
{
    MOS_AgentServer& agentServer = controller_.GetConnectionMgr().GetAgentServer();
    pMessageService_->Send( agentServer.GetSession(), eMsgDisableProfiling );
}

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

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgInit
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgInit( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    /*uint32 nNbrGtias_;
    input >> nNbrGtias_;
    for( uint nTmp = 0; nTmp < nNbrGtias_; ++nTmp )
    {
        MOS_Gtia* pGtia = new MOS_Gtia();
        pGtia->Initialize( input );
        MOS_App::GetApp().GetAgentManager().RegisterGtia( *pGtia );
    }

    uint32 nNbrAgents_;
    input >> nNbrAgents_;
    for( nTmp = 0; nTmp < nNbrAgents_; ++nTmp )
    {
        MOS_Agent* pAgent = new MOS_Agent(); //$$$ TMP
        pAgent->Initialize( input );
        MOS_App::GetApp().GetAgentManager().RegisterAgent( *pAgent );
    }

    /*
    uint32 nNbrSensors;
    input >> nNbrSensors;
    for( nTmp = 0; nTmp < nNbrSensors; ++nTmp )
    {
        MOS_SensorCapacity* pSensor = new MOS_SensorCapacity(); //$$$ TMP
        pSensor->Initialize( input );
        MOS_App::GetApp().GetAgentManager().RegisterSensor( *pSensor );
    }*/

    uint8 nTmp;
    input >> nTmp;
    MOS_App::GetApp().GetMainWindow().GetAgentEditor().NotifyEnableUnitVisionCones( nTmp );

    input >> nTmp;
    MOS_App::GetApp().GetMainWindow().GetAgentEditor().NotifyEnableProfiling( nTmp );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgAutomateCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgAutomateCreation( const ASN1T_MsgAutomateCreation& asnMsg )
{
    if( !MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate ) )
        new MOS_Agent( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgPionCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgPionCreation( const ASN1T_MsgPionCreation& asnMsg )
{
    if( !MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion ) )
        new MOS_Agent( asnMsg );
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
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgKnowledgeGroup
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgKnowledgeGroup( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    uint32 nID;
    input >> nID;    
    if( !MOS_App::GetApp().GetAgentManager().FindGtia( nID ) )
        new MOS_Gtia( nID, input ); //$$ DEGUEU
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgArmy
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgArmy( DIN::DIN_Link& , DIN::DIN_Input& input )
{
    uint32 nID;
    input >> nID;    
    if( !MOS_App::GetApp().GetAgentManager().FindTeam( nID ) )
        new MOS_Team( nID, input ); //$$ DEGUEU
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

    //MOS_App::GetApp().GetMainWindow().GetGraphWindow().AddValues( rMainLoopTime, rPerceptionTime, rDecisionTime, rActionTime  );
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
    if( pAgent )
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
    if( pAgent )
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
    if( pAgent )
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
    if( pAgent )
        pAgent->OnReceiveMsgObjectInterVisibility( input );
}



//=============================================================================
// ASN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::SendMsgMosSimWithContext
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::SendMsgMosSimWithContext( ASN1T_MsgsMosSimWithContext& asnMsg, MIL_MOSContextID nCtx )
{
    if ( !MOS_App::GetApp().GetMOSServer().GetController().GetConnectionMgr().IsConnectedToAgentServer() )
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
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::SendMsgMosSim
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::SendMsgMosSim( ASN1T_MsgsMosSim& asnMsg )
{
    if ( !MOS_App::GetApp().GetMOSServer().GetController().GetConnectionMgr().IsConnectedToAgentServer() )
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
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlPauseAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlPauseAck( const ASN1T_MsgCtrlPauseAck& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : CtrlPauseAck - Code: ";
    PrintCtrlMsgErrorCode( strOutputMsg, asnMsg );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    bPaused_ = true;
    MOS_App::GetApp().GetMainWindow().GetSimControl().NotifyStatePause( bPaused_ );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlResumeAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlResumeAck( const ASN1T_MsgCtrlResumeAck& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : CtrlResumeAck - Code: ";
    PrintCtrlMsgErrorCode( strOutputMsg, asnMsg );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    bPaused_ = false;
    MOS_App::GetApp().GetMainWindow().GetSimControl().NotifyStatePause( bPaused_ );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlChangeTimeFactorAck
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlChangeTimeFactorAck( const ASN1T_MsgCtrlChangeTimeFactorAck& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : CtrlChangeTimeFactorAck - Time factor: " << (int)asnMsg.time_factor << " Code: ";
    PrintCtrlMsgErrorCode( strOutputMsg, asnMsg.error_code );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlInfo
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlInfo( const ASN1T_MsgCtrlInfo& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : CtrlInfo - Current Tick: " << asnMsg.current_tick 
                 << " - Tick duration : "       << asnMsg.tick_duration
                 << " - Time factor: "          << asnMsg.time_factor
                 << " - Exercice ID: "          << asnMsg.id_exercice
                 << " - Checkpoint Frequency: " << asnMsg.checkpoint_frequence;

    nTickDuration_ = asnMsg.tick_duration;
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlBeginTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlBeginTick( const ASN1T_MsgCtrlBeginTick& asnMsg )
{
    MOS_App::GetApp().NotifyTimeHasChanged( asnMsg * nTickDuration_ );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlEndTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlEndTick( const ASN1T_MsgCtrlEndTick& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "Last tick duration : " << asnMsg.tick_duration << "ms - Pathfinds : " << asnMsg.nb_pathfinds_courts << '/' << asnMsg.nb_pathfinds_longs;
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgUnitAttributes
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgUnitAttributes( const ASN1T_MsgUnitAttributes& asnMsg )
{
    MIL_AgentID nAgentID = asnMsg.oid_pion;
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( nAgentID );
    if( pAgent )
        pAgent->OnAttributeUpdated( asnMsg );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgUnitDotations
// Created: NLD 2003-03-06
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgUnitDotations( const ASN1T_MsgUnitDotations& asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    if( pAgent )
        pAgent->OnAttributeUpdated( asnMsg );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgUnitPathFind
// Created: NLD 2003-03-31
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgUnitPathFind( const ASN1T_MsgUnitPathFind& asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    if( pAgent )
        pAgent->OnReceiveMsgPathFind( asnMsg );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgUnitMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgUnitMagicActionAck( const ASN1T_MsgUnitMagicActionAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : UnitMagicActionAck - Code: ";
    PrintUnitAttrMsgErrorCode( strOutputMsg, asnMsg.error_code );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgObjectMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgObjectMagicActionAck( const ASN1T_MsgObjectMagicActionAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : ObjectMagicActionAck - Code: ";
    PrintObjectMsgErrorCode( strOutputMsg, asnMsg.error_code );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlMeteoGlobalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlMeteoGlobalAck()
{
    MT_LOG_INFO_MSG( "MSG Received : CtrlMeteoGlobaleAck" );
}
       
// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlMeteoLocalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlMeteoLocalAck()
{
    MT_LOG_INFO_MSG( "MSG Received : CtrlMeteoLocaleAck" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateBegin
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateBegin()
{
    MT_LOG_INFO_MSG( "MSG Received : CtrlSendCurrentStateBegin" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateEnd
// Created: NLD 2003-10-09
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCtrlSendCurrentStateEnd()
{
    MT_LOG_INFO_MSG( "MSG Received : CtrlSendCurrentStateEnd" );
    MOS_App::GetApp().GetMainWindow().GetSimControl().NotifyConnection( true );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSaveBegin
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSaveBegin()
{
    MT_LOG_INFO_MSG( "MSG Received : CtrlCheckPointSaveBegin" );
}
                                                                
// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSaveEnd
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSaveEnd()
{
    MT_LOG_INFO_MSG( "MSG Received : CtrlCheckPointSaveEnd" );
}
 
// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointLoadBegin
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointLoadBegin()
{
    MT_LOG_INFO_MSG( "MSG Received : CtrlCheckPointBeginEnd" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointLoadEnd
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointLoadEnd()
{
    MT_LOG_INFO_MSG( "MSG Received : CtrlCheckPointLoadEnd" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSetFrequencyAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSetFrequencyAck()
{
    MT_LOG_INFO_MSG( "MSG Received : CtrlCheckPointSetFrequencyAck" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCheckPointSaveNowAck()
{
    MT_LOG_INFO_MSG( "MSG Received : CtrlCheckPointSetSaveNowAck" );
}

//=============================================================================
// LIMAS / LIMITS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimitCreationAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimitCreationAck( const ASN1T_MsgLimitCreationAck& asnMsg, MIL_MOSContextID nCtx )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : LimitCreationAck - Code: ";
    PrintInfoContextMsgErrorCode( strOutputMsg, asnMsg.error_code );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        MOS_Limit* pLimit = MOS_App::GetApp().GetLineManager().FindLimit( asnMsg.oid );
        if( pLimit )
        {
            assert( pLimit == (MOS_Limit*)nCtx );
            pLimit->SetNetworkState( eStateLimitRegistered );
            pLimit->SetState( eStateLimitOk );
        }
        else
            MOS_Limit::idManager_.LockIdentifier( asnMsg.oid );
    }
//    else
//        MOS_App::GetApp().GetLineManager().RemoveLimit( asnMsg.oid );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimitUpdateAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimitUpdateAck( const ASN1T_MsgLimitUpdateAck& asnMsg, MIL_MOSContextID nCtx )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : LimitUpdateAck - Code: ";
    PrintInfoContextMsgErrorCode( strOutputMsg, asnMsg.error_code );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        MOS_Limit* pLimit = MOS_App::GetApp().GetLineManager().FindLimit( asnMsg.oid );
        if( pLimit )
        {
            assert( pLimit == (MOS_Limit*)nCtx );
            pLimit->SetNetworkState( eStateLimitRegistered );
            pLimit->SetState( eStateLimitOk );
        }
        // else => ack pour un autre MOS
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimitDestructionAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimitDestructionAck( const ASN1T_MsgLimitDestructionAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : LimitDestructionAck - Code: ";
    PrintInfoContextMsgErrorCode( strOutputMsg, asnMsg.error_code );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        MOS_Limit* pLimit = MOS_App::GetApp().GetLineManager().FindLimit( asnMsg.oid );
        if( pLimit )
            MOS_App::GetApp().GetLineManager().RemoveLimit( asnMsg.oid );
        else
            MOS_Limit::idManager_.ReleaseIdentifier( asnMsg.oid );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimaCreationAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimaCreationAck( const ASN1T_MsgLimaCreationAck& asnMsg, MIL_MOSContextID nCtx )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : LimaCreationAck - Code: ";
    PrintInfoContextMsgErrorCode( strOutputMsg, asnMsg.error_code );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );


    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        MOS_Lima* pLima = MOS_App::GetApp().GetLineManager().FindLima( asnMsg.oid );
        if( pLima )
        {
            assert( pLima == (MOS_Lima*)nCtx );
            pLima->SetNetworkState( eStateLimaRegistered );
            pLima->SetState( eStateLimaOk );
        }
        else
            MOS_Lima::idManager_.LockIdentifier( asnMsg.oid );
    }
//    else
//        MOS_App::GetApp().GetLineManager().RemoveLima( asnMsg.oid );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimaUpdateAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimaUpdateAck( const ASN1T_MsgLimaUpdateAck& asnMsg, MIL_MOSContextID nCtx )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : LimaUpdateAck - Code: ";
    PrintInfoContextMsgErrorCode( strOutputMsg, asnMsg.error_code );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        MOS_Lima* pLima = MOS_App::GetApp().GetLineManager().FindLima( asnMsg.oid );
        if( pLima )
        {
            assert( pLima == (MOS_Lima*)nCtx );
            pLima->SetNetworkState( eStateLimaRegistered );
            pLima->SetState( eStateLimaOk );
        }
        // else => ack pour un autre MOS
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimaDestructionAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimaDestructionAck( const ASN1T_MsgLimaDestructionAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : LimaDestructionAck - Code: ";
    PrintInfoContextMsgErrorCode( strOutputMsg, asnMsg.error_code );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    if( asnMsg.error_code == EnumInfoContextErrorCode::no_error )
    {
        MOS_Lima* pLima = MOS_App::GetApp().GetLineManager().FindLima( asnMsg.oid );
        if( pLima )
            MOS_App::GetApp().GetLineManager().RemoveLima( asnMsg.oid );
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
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : LimitCreation";
    if( !MOS_App::GetApp().GetLineManager().FindLimit( asnMsg.oid ) )
        new MOS_Limit( asnMsg );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}   


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimitDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimitDestruction( const ASN1T_MsgLimitDestruction& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : LimitDestruction";
    MOS_App::GetApp().GetLineManager().RemoveLimit( asnMsg );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimaCreation
// Created: NLD 2005-01-19
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimaCreation( const ASN1T_MsgLimaCreation& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : LimaCreation";
    if( !MOS_App::GetApp().GetLineManager().FindLima( asnMsg.oid ) )
        new MOS_Lima( asnMsg );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}   

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLimaDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLimaDestruction( const ASN1T_MsgLimitDestruction& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : LimaDestruction";
    MOS_App::GetApp().GetLineManager().RemoveLima( asnMsg );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
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
    strOutputMsg << "MSG Received : AutomateOrderAck - Code: ";
    PrintOrderMsgErrorCode( strOutputMsg, asnMsg.error_code );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    MOS_App::GetApp().GetMainWindow().GetAgentEditor().NotifyAgentMissionAck( asnMsg.error_code );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgPionOrderAck
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgPionOrderAck( const ASN1T_MsgPionOrderAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << MT_FormatString( "Agent[%d]", asnMsg.oid_unite_executante );
    strOutputMsg << "MSG Received : PionOrderAck - Code: ";
    PrintOrderMsgErrorCode( strOutputMsg, asnMsg.error_code );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
    if( asnMsg.error_code != EnumOrderErrorCode::no_error )
    {
        MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_unite_executante );
        if ( pAgent )
        {
            MOS_App::GetApp().GetMainWindow().GetHistory().SetLastOrderState( *pAgent, eMissionError, MOS_App::GetApp().GetTime() );
            MOS_App::GetApp().GetTestManager().SayMissionFinished( *pAgent, MOS_App::GetApp().GetTime() );
        }
    }
    MOS_App::GetApp().GetMainWindow().GetAgentEditor().NotifyAgentMissionAck( asnMsg.error_code );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgOrderConduiteAck
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgOrderConduiteAck( const ASN1T_MsgOrderConduiteAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : OrderConduiteAck - Code: ";
    PrintOrderMsgErrorCode( strOutputMsg, asnMsg.error_code );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgCR
// Created: NLD 2002-09-02
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgCR( const ASN1T_MsgCR& asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.unit_id );
    if( pAgent )
    {
        pAgent->OnReceiveMsgCR( asnMsg );
        //MT_LOG_INFO_MSG( "RC received." );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgOrderManagement
// Created: NLD 2003-03-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgOrderManagement( const ASN1T_MsgOrderManagement& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : OrderManagement - Etat: ";
    switch( asnMsg.etat )
    {
        case EnumOrderState::started  : 
        {
            strOutputMsg << "Started";
            MOS_App::GetApp().GetMainWindow().GetHistory().SetLastOrderState( *(MOS_Agent*)asnMsg.order_id, eMissionStarted, MOS_App::GetApp().GetTime() );
        }
        break;

        case EnumOrderState::stopped  : 
        {
            strOutputMsg << "Stopped";
            MOS_App::GetApp().GetMainWindow().GetHistory().SetLastOrderState( *(MOS_Agent*)asnMsg.order_id, eMissionStoped, MOS_App::GetApp().GetTime() );
            MOS_App::GetApp().GetTestManager().SayMissionFinished( *(MOS_Agent*)asnMsg.order_id, MOS_App::GetApp().GetTime() );
        }
        break;

        case EnumOrderState::cancelled: 
        {
            strOutputMsg << "Cancelled"; 
            MOS_App::GetApp().GetMainWindow().GetHistory().SetLastOrderState( *(MOS_Agent*)asnMsg.order_id, eMissionCanceled, MOS_App::GetApp().GetTime() );
            MOS_App::GetApp().GetTestManager().SayMissionFinished( *(MOS_Agent*)asnMsg.order_id, MOS_App::GetApp().GetTime() );
        }
        break;
    }
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgAutomateMRT
// Created: NLD 2003-05-13
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgAutomateMRT( const ASN1T_MsgAutomateMRT& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : AutomateMRT - Automate " << asnMsg.automate_id << " - BEGIN";
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    for( uint i = 0; i < asnMsg.missions.n; ++i )
    {
        ASN1T_MsgPionOrder& asnPionOrder = asnMsg.missions.elem[i];

        std::stringstream strOutputMsg;
        strOutputMsg << "Pion " << asnPionOrder.oid_unite_executante
                     << " - Mission " << asnPionOrder.mission.t;
        MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
    }
    MT_LOG_INFO_MSG( "MSG Received : AutomateMRT - END" );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgPionOrder
// Created: NLD 2003-10-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgPionOrder( const ASN1T_MsgPionOrder& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : PionOrder - Mission " << asnMsg.mission.t;
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgAutomateOrder
// Created: NLD 2003-10-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgAutomateOrder( const ASN1T_MsgAutomateOrder& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : AutomateOrder - Mission " << asnMsg.mission.t;
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgWaitForOrderConduite
// Created: NLD 2003-04-04
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgWaitForOrderConduite( const ASN1T_MsgAttenteOrdreConduite& asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.unit_id );
    if( pAgent )
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
    strOutputMsg << "MSG Received : SetAutomateModeAck - Etat: ";
    switch( asnMsg.error_code )
    {
        case EnumSetAutomateModeErrorCode::no_error                : strOutputMsg << "No error"; break;
        case EnumSetAutomateModeErrorCode::error_already_debraye   : strOutputMsg << "Error : Déjà debrayé"; break;
        case EnumSetAutomateModeErrorCode::error_already_embraye   : strOutputMsg << "Error : Déjà embrayé"; break;
        case EnumSetAutomateModeErrorCode::error_invalid_unit      : strOutputMsg << "Error : Invalid unit"; break;
        case EnumSetAutomateModeErrorCode::error_unit_not_automate : strOutputMsg << "Error : Unit not automate"; break;
        case EnumSetAutomateModeErrorCode::error_unit_surrendered  : strOutputMsg << "Error : Unit surrendered"; break;
    }
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgChangeAutomateAck
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgChangeAutomateAck( const ASN1T_MsgChangeAutomateAck& asnMsg, MIL_MOSContextID nCtx )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : ChangeAutomateAck - Etat: ";
    switch( asnMsg.error_code )
    {
        case EnumChangeAutomateErrorCode::no_error                 : strOutputMsg << "no_error"; break;
        case EnumChangeAutomateErrorCode::error_camps_incompatibles: strOutputMsg << "error_camps_incompatibles"; break;
        case EnumChangeAutomateErrorCode::error_invalid_automate   : strOutputMsg << "error_invalid_automate"; break;
        case EnumChangeAutomateErrorCode::error_invalid_pion       : strOutputMsg << "error_invalid_pion"; break;
    }

    MOS_AgentListView& agentListView = MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView();
    if( asnMsg.error_code == EnumChangeAutomateErrorCode::no_error )
        agentListView.AcceptAgentChangeAutomate( nCtx );
    else
        agentListView.CancelAgentChangeAutomate( nCtx );

    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgChangeDiplomacyAck
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgChangeDiplomacyAck( const ASN1T_MsgChangeDiplomatieAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : ChangeDiplomacyAck - Etat: ";
    switch( asnMsg.error_code )
    {
        case EnumChangeDiplomatieErrorCode::no_error           : strOutputMsg << "no_error"; break;
        case EnumChangeDiplomatieErrorCode::error_invalid_camp : strOutputMsg << "error_invalid_camp"; break;
    }
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgChangeGroupeConnaissanceAck
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgChangeGroupeConnaissanceAck( const ASN1T_MsgChangeGroupeConnaissanceAck& asnMsg, MIL_MOSContextID nCtx )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : ChangeGroupeConnaissanceAck - Etat: ";
    switch( asnMsg.error_code )
    {
        case EnumChangeGroupeConnaissanceErrorCode::no_error                          : strOutputMsg << "no_error"; break;
        case EnumChangeGroupeConnaissanceErrorCode::error_invalid_automate            : strOutputMsg << "error_invalid_automate"; break;
        case EnumChangeGroupeConnaissanceErrorCode::error_invalid_camp                : strOutputMsg << "error_invalid_camp"; break;
        case EnumChangeGroupeConnaissanceErrorCode::error_invalid_groupe_connaissance : strOutputMsg << "error_invalid_groupe_connaissance"; break;
    }

    MOS_AgentListView& agentListView = MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView();
    if( asnMsg.error_code == EnumChangeGroupeConnaissanceErrorCode::no_error )
        agentListView.AcceptAutomateChangeKnowledgeGroup( nCtx );
    else
        agentListView.CancelAutomateChangeKnowledgeGroup( nCtx );

    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgChangeLiensLogistiquesAck
// Created: NLD 2005-01-18
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgChangeLiensLogistiquesAck( const ASN1T_MsgChangeLiensLogistiquesAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
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

    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
    if( pAgent )
        pAgent->OnLogisticLinksChanged( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgChangeAutomate
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgChangeAutomate( const ASN1T_MsgChangeAutomate& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : ChangeAutomate - Pion: " << asnMsg.oid_pion << " - Automate: " << asnMsg.oid_automate;
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    MOS_Agent* pPion = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    MOS_Agent* pAutomate = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
    if( pPion && pAutomate )
        pPion->OnAutomateChanged( *pAutomate );
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

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementCreation
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementCreation( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& asnMsg )
{
    if( MOS_App::GetApp().GetAgentManager().FindMaintenanceConsign( asnMsg.oid_consigne ) )
    {
        assert( false );
        return;
    }
    MOS_App::GetApp().GetAgentManager().RegisterMaintenanceConsign( *new MOS_LogMaintenanceConsign( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementDestruction
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementDestruction( const ASN1T_MsgLogMaintenanceTraitementEquipementDestruction& asnMsg )
{
    MOS_LogMaintenanceConsign* pConsign = MOS_App::GetApp().GetAgentManager().FindMaintenanceConsign( asnMsg.oid_consigne );
    assert( pConsign );
    MOS_App::GetApp().GetAgentManager().UnregisterMaintenanceConsign( *pConsign );
    delete pConsign;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementUpdate
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementUpdate( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& asnMsg )
{
    MOS_LogMaintenanceConsign* pConsign = MOS_App::GetApp().GetAgentManager().FindMaintenanceConsign( asnMsg.oid_consigne );
    assert( pConsign );
    pConsign->OnReceiveMsgUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceEtat
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogMaintenanceEtat( const ASN1T_MsgLogMaintenanceEtat&  asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pAgent );
    pAgent->OnReceiveMsgLogMaintenanceEtat( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementCreation
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementCreation( const ASN1T_MsgLogRavitaillementTraitementCreation& asnMsg )
{
    if( MOS_App::GetApp().GetAgentManager().FindSupplyConsign( asnMsg.oid_consigne ) )
    {
        assert( false );
        return;
    }
    MOS_App::GetApp().GetAgentManager().RegisterSupplyConsign( *new MOS_LogSupplyConsign( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementDestruction
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementDestruction( const ASN1T_MsgLogRavitaillementTraitementDestruction& asnMsg )
{
    MOS_LogSupplyConsign* pConsign = MOS_App::GetApp().GetAgentManager().FindSupplyConsign( asnMsg.oid_consigne );
    assert( pConsign );
    MOS_App::GetApp().GetAgentManager().UnregisterSupplyConsign( *pConsign );
    delete pConsign;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementUpdate
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementUpdate( const ASN1T_MsgLogRavitaillementTraitementUpdate& asnMsg )
{
    MOS_LogSupplyConsign* pConsign = MOS_App::GetApp().GetAgentManager().FindSupplyConsign( asnMsg.oid_consigne );
    assert( pConsign );
    pConsign->OnReceiveMsgUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementEtat
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementEtat( const ASN1T_MsgLogRavitaillementEtat&  asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pAgent );
    pAgent->OnReceiveMsgLogSupplyEtat( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementQuotas
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementQuotas( const ASN1T_MsgLogRavitaillementQuotas& asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_automate );
    assert( pAgent );
    pAgent->OnReceiveMsgLogSupplyQuotas( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainCreation
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainCreation( const ASN1T_MsgLogSanteTraitementHumainCreation& asnMsg )
{
    if( MOS_App::GetApp().GetAgentManager().FindMedicalConsign( asnMsg.oid_consigne ) )
    {
        assert( false );
        return;
    }
    MOS_App::GetApp().GetAgentManager().RegisterMedicalConsign( *new MOS_LogMedicalConsign( asnMsg ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainDestruction
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainDestruction( const ASN1T_MsgLogSanteTraitementHumainDestruction& asnMsg )
{
    MOS_LogMedicalConsign* pConsign = MOS_App::GetApp().GetAgentManager().FindMedicalConsign( asnMsg.oid_consigne );
    assert( pConsign );
    MOS_App::GetApp().GetAgentManager().UnregisterMedicalConsign( *pConsign );
    delete pConsign;
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainUpdate
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainUpdate( const ASN1T_MsgLogSanteTraitementHumainUpdate& asnMsg )
{
    MOS_LogMedicalConsign* pConsign = MOS_App::GetApp().GetAgentManager().FindMedicalConsign( asnMsg.oid_consigne );
    assert( pConsign );
    pConsign->OnReceiveMsgUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteEtat
// Created: NLD 2004-12-30
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogSanteEtat( const ASN1T_MsgLogSanteEtat&  asnMsg )
{
    MOS_Agent* pAgent = MOS_App::GetApp().GetAgentManager().FindAgent( asnMsg.oid_pion );
    assert( pAgent );
    pAgent->OnReceiveMsgLogMedicalEtat( asnMsg );
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
    MOS_DynaObjectManager& objectManager = MOS_App::GetApp().GetDynaObjectManager();
    if( objectManager.FindDynaObject( asnMsg.oid ) != 0 )
        return;

    MOS_DynaObject* pObject = new MOS_DynaObject(); //$$$ TMP
    pObject->Initialize( asnMsg );
    objectManager.RegisterDynaObject( *pObject );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgObjectUpdate
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgObjectUpdate( const ASN1T_MsgObjectUpdate& asnMsg )
{
    MOS_DynaObject* pObject = MOS_App::GetApp().GetDynaObjectManager().FindDynaObject( asnMsg.oid );
    if( pObject )
        pObject->Update( asnMsg );
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgObjectDestruction
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgObjectDestruction( const ASN1T_MsgObjectDestruction& asnMsg )
{
    MOS_DynaObjectManager& dynaObjectManager = MOS_App::GetApp().GetDynaObjectManager();

    MOS_DynaObject* pObject = dynaObjectManager.FindDynaObject( asnMsg );
    if( ! pObject )
        return;

    dynaObjectManager.UnregisterDynaObject( *pObject );
    pObject->Terminate();   //$$$ TMP - devrait être fait par le manager
    delete pObject;         //$$$ TMP - devrait être fait par le manager
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
    strOutputMsg << "MSG Received : StartDirectFire - ID: " << asnMsg.oid_tir
                 << " - ID source " << asnMsg.oid_src
                 << " - ID dest "   << asnMsg.oid_dst;
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    MOS_AgentManager& agentManager = MOS_App::GetApp().GetAgentManager();
    MOS_Agent* pAgentSrc = agentManager.FindAgent( asnMsg.oid_src );
    MOS_Agent* pAgentDst = agentManager.FindAgent( asnMsg.oid_dst );
    if( pAgentDst == 0 || pAgentSrc == 0 )
        return;

    MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().AddConflict( asnMsg.oid_tir, pAgentSrc->GetPos(), pAgentDst->GetPos() );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgStartIndirectFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgStartIndirectFire( const ASN1T_MsgStartIndirectFire& asnMsg )
{
    std::stringstream strOutputMsg;
    
    strOutputMsg << "MSG Received : StartIndirectFire - ID: " << asnMsg.oid_tir << " - ID source " << asnMsg.oid_src;
    std::string strTmp( (const char*)asnMsg.position.data, asnMsg.position.numocts );
    strOutputMsg << " - Pos dest " << strTmp;                    
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    MOS_AgentManager& agentManager = MOS_App::GetApp().GetAgentManager();
    MOS_Agent* pAgentSrc = agentManager.FindAgent( asnMsg.oid_src );
    if( pAgentSrc == 0 )
        return;

    MT_Vector2D vPos;
    MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( strTmp, vPos );

    MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().AddConflict( asnMsg.oid_tir, pAgentSrc->GetPos(), vPos );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgStopDirectFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgStopDirectFire( const ASN1T_MsgStopDirectFire& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : StopDirectFire - ID: " << asnMsg.oid_tir;
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().DeleteConflict( asnMsg.oid_tir );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgStopIndirectFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgStopIndirectFire( const ASN1T_MsgStopIndirectFire& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : StopIndirectFire - ID: " << asnMsg.oid_tir;
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().DeleteConflict( asnMsg.oid_tir );
}



//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgStartFireEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgStartFireEffect( const ASN1T_MsgStartFireEffect& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : Start ammunition meteo effect - ID: " << asnMsg.oid_effet << " Type:" << asnMsg.type;
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    MOS_App::GetApp().GetRawVisionData().RegisterAmmoMeteoEffect( asnMsg );
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgStopFireEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgStopFireEffect( const ASN1T_MsgStopFireEffect& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : Stop ammunition meteo effect - ID: " << asnMsg;
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    MOS_App::GetApp().GetRawVisionData().UnregisterAmmoMeteoEffect( asnMsg );
}



//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgExplosion
// Created: NLD 2003-05-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgExplosion( const ASN1T_MsgExplosion& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "MSG Received : Explosion"
                 << " - ID objet " << asnMsg.oid_objet;
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );    
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck( const ASN1T_MsgLogRavitaillementChangeQuotasAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
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
// Name: MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementPousserFlux
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::OnReceiveMsgLogRavitaillementPousserFlux( const ASN1T_MsgLogRavitaillementPousserFluxAck& asnMsg, MIL_MOSContextID /*nCtx*/ )
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
    std::cout << "BEGIN MSG DUMP =>" << std::endl;
    asnPERDecodeBuffer.SetTrace( true );
    asnMsgCtrl.Print( "MsgsSimMos" );
    std::cout << "END MSG DUMP =>" << std::endl;
#endif

    switch( asnMsg.t )
    {
        //case T_MsgsSimMos_msgCtrlStopAck:                         OnReceiveMsgCtrlStopAck( mosServer, asnMsg.u.msgCtrlStopAck )
        case T_MsgsSimMos_msg_ctrl_pause_ack:                       OnReceiveMsgCtrlPauseAck              ( asnMsg.u.msg_ctrl_pause_ack                       ); break;
        case T_MsgsSimMos_msg_ctrl_resume_ack:                      OnReceiveMsgCtrlResumeAck             ( asnMsg.u.msg_ctrl_resume_ack                      ); break;
        case T_MsgsSimMos_msg_ctrl_change_time_factor_ack:          OnReceiveMsgCtrlChangeTimeFactorAck   ( *asnMsg.u.msg_ctrl_change_time_factor_ack         ); break;
        case T_MsgsSimMos_msg_ctrl_info:                            OnReceiveMsgCtrlInfo                  ( *asnMsg.u.msg_ctrl_info                           ); break;
        case T_MsgsSimMos_msg_ctrl_begin_tick:                      OnReceiveMsgCtrlBeginTick             (  asnMsg.u.msg_ctrl_begin_tick                     ); break;
        case T_MsgsSimMos_msg_ctrl_end_tick:                        OnReceiveMsgCtrlEndTick               ( *asnMsg.u.msg_ctrl_end_tick                       ); break;
        case T_MsgsSimMos_msg_unit_attributes:                      OnReceiveMsgUnitAttributes            ( *asnMsg.u.msg_unit_attributes                     ); break;
        case T_MsgsSimMos_msg_unit_dotations:                       OnReceiveMsgUnitDotations             ( *asnMsg.u.msg_unit_dotations                      ); break;
        case T_MsgsSimMos_msg_unit_pathfind:                        OnReceiveMsgUnitPathFind              ( *asnMsg.u.msg_unit_pathfind                       ); break;
        case T_MsgsSimMos_msg_cr:                                   OnReceiveMsgCR                        ( *asnMsg.u.msg_cr                                  ); break;
//        case T_MsgsSimMos_msg_unit_knowledge_update_or_creation:    OnReceiveMsgUnitKnowledgeUpdate       ( *asnMsg.u.msg_unit_knowledge_update_or_creation   ); break; 
//        case T_MsgsSimMos_msg_unit_knowledge_destruction:           OnReceiveMsgUnitKnowledgeDestruction  ( *asnMsg.u.msg_unit_knowledge_destruction          ); break; 

        case T_MsgsSimMos_msg_pion_creation:                        OnReceiveMsgPionCreation              ( *asnMsg.u.msg_pion_creation                       ); break;
        case T_MsgsSimMos_msg_automate_creation:                    OnReceiveMsgAutomateCreation          ( *asnMsg.u.msg_automate_creation                   ); break;
        case T_MsgsSimMos_msg_change_diplomatie:                    OnReceiveMsgChangeDiplomatie          ( *asnMsg.u.msg_change_diplomatie                   ); break;  

        case T_MsgsSimMos_msg_unit_knowledge_creation:              OnReceiveMsgUnitKnowledgeCreation     ( *asnMsg.u.msg_unit_knowledge_creation             ); break;
        case T_MsgsSimMos_msg_unit_knowledge_update:                OnReceiveMsgUnitKnowledgeUpdate       ( *asnMsg.u.msg_unit_knowledge_update               ); break;
        case T_MsgsSimMos_msg_unit_knowledge_destruction:           OnReceiveMsgUnitKnowledgeDestruction  ( *asnMsg.u.msg_unit_knowledge_destruction          ); break;

        case T_MsgsSimMos_msg_start_direct_fire:                    OnReceiveMsgStartDirectFire           ( *asnMsg.u.msg_start_direct_fire                   ); break;
        case T_MsgsSimMos_msg_start_indirect_fire:                  OnReceiveMsgStartIndirectFire         ( *asnMsg.u.msg_start_indirect_fire                 ); break;
        case T_MsgsSimMos_msg_stop_direct_fire:                     OnReceiveMsgStopDirectFire            ( *asnMsg.u.msg_stop_direct_fire                    ); break;
        case T_MsgsSimMos_msg_stop_indirect_fire:                   OnReceiveMsgStopIndirectFire          ( *asnMsg.u.msg_stop_indirect_fire                  ); break;
        case T_MsgsSimMos_msg_explosion:                            OnReceiveMsgExplosion                 ( *asnMsg.u.msg_explosion                           ); break;
        case T_MsgsSimMos_msg_order_management:                     OnReceiveMsgOrderManagement           ( *asnMsg.u.msg_order_management                    ); break;
        case T_MsgsSimMos_msg_attente_ordre_conduite:               OnReceiveMsgWaitForOrderConduite      ( *asnMsg.u.msg_attente_ordre_conduite              ); break;
        case T_MsgsSimMos_msg_annule_attente_ordre_conduite:        break; //$$$ TODO                                                                         
        case T_MsgsSimMos_msg_limit_creation:                       OnReceiveMsgLimitCreation             ( *asnMsg.u.msg_limit_creation                      ); break;
        case T_MsgsSimMos_msg_limit_destruction:                    OnReceiveMsgLimitDestruction          ( asnMsg.u.msg_limit_destruction                    ); break;
        case T_MsgsSimMos_msg_lima_creation:                        OnReceiveMsgLimaCreation              ( *asnMsg.u.msg_lima_creation                       ); break;
        case T_MsgsSimMos_msg_lima_destruction:                     OnReceiveMsgLimaDestruction           ( asnMsg.u.msg_lima_destruction                     ); break;
        case T_MsgsSimMos_msg_object_creation:                      OnReceiveMsgObjectCreation            ( *asnMsg.u.msg_object_creation                     ); break;
        case T_MsgsSimMos_msg_object_update:                        OnReceiveMsgObjectUpdate              ( *asnMsg.u.msg_object_update                       ); break;
        case T_MsgsSimMos_msg_object_destruction:                   OnReceiveMsgObjectDestruction         ( asnMsg.u.msg_object_destruction                   ); break;
        case T_MsgsSimMos_msg_automate_mrt:                         OnReceiveMsgAutomateMRT               ( *asnMsg.u.msg_automate_mrt                        ); break;
        case T_MsgsSimMos_msg_object_knowledge_creation:            OnReceiveMsgObjectKnowledgeCreation   ( *asnMsg.u.msg_object_knowledge_creation           ); break; 
        case T_MsgsSimMos_msg_object_knowledge_update:              OnReceiveMsgObjectKnowledgeUpdate     ( *asnMsg.u.msg_object_knowledge_update             ); break; 
        case T_MsgsSimMos_msg_object_knowledge_destruction:         OnReceiveMsgObjectKnowledgeDestruction( *asnMsg.u.msg_object_knowledge_destruction        ); break; 
        case T_MsgsSimMos_msg_ctrl_meteo_globale_ack:               OnReceiveMsgCtrlMeteoGlobalAck        (); break; 
        case T_MsgsSimMos_msg_ctrl_meteo_locale_ack:                OnReceiveMsgCtrlMeteoLocalAck         (); break; 
        case T_MsgsSimMos_msg_ctrl_checkpoint_save_begin:           OnReceiveMsgCheckPointSaveBegin       (); break; 
        case T_MsgsSimMos_msg_ctrl_checkpoint_save_end:             OnReceiveMsgCheckPointSaveEnd         (); break; 
        case T_MsgsSimMos_msg_ctrl_checkpoint_load_begin:           OnReceiveMsgCheckPointLoadBegin       (); break;
        case T_MsgsSimMos_msg_ctrl_checkpoint_load_end:             OnReceiveMsgCheckPointLoadEnd         (); break;
        case T_MsgsSimMos_msg_ctrl_checkpoint_set_frequency_ack:    OnReceiveMsgCheckPointSetFrequencyAck (); break;
        case T_MsgsSimMos_msg_ctrl_checkpoint_save_now_ack:         OnReceiveMsgCheckPointSaveNowAck      (); break;
        case T_MsgsSimMos_msg_pion_order:                           OnReceiveMsgPionOrder                 ( *asnMsg.u.msg_pion_order ); break;
        case T_MsgsSimMos_msg_automate_order:                       OnReceiveMsgAutomateOrder             ( *asnMsg.u.msg_automate_order ); break;
        case T_MsgsSimMos_msg_ctrl_send_current_state_begin:        OnReceiveMsgCtrlSendCurrentStateBegin (); break;
        case T_MsgsSimMos_msg_ctrl_send_current_state_end:          OnReceiveMsgCtrlSendCurrentStateEnd   (); break;
        case T_MsgsSimMos_msg_change_automate:                     OnReceiveMsgChangeAutomate            ( *asnMsg.u.msg_change_automate ); break;
        case T_MsgsSimMos_msg_start_fire_effect:                    OnReceiveMsgStartFireEffect           ( *asnMsg.u.msg_start_fire_effect ); break;
        case T_MsgsSimMos_msg_stop_fire_effect:                     OnReceiveMsgStopFireEffect            ( asnMsg.u.msg_stop_fire_effect ); break;

        case T_MsgsSimMos_msg_log_maintenance_traitement_equipement_creation:    OnReceiveMsgLogMaintenanceTraitementEquipementCreation   ( *asnMsg.u.msg_log_maintenance_traitement_equipement_creation ); break;
        case T_MsgsSimMos_msg_log_maintenance_traitement_equipement_destruction: OnReceiveMsgLogMaintenanceTraitementEquipementDestruction( *asnMsg.u.msg_log_maintenance_traitement_equipement_destruction ); break;
        case T_MsgsSimMos_msg_log_maintenance_traitement_equipement_update:      OnReceiveMsgLogMaintenanceTraitementEquipementUpdate     ( *asnMsg.u.msg_log_maintenance_traitement_equipement_update ); break;
        case T_MsgsSimMos_msg_log_maintenance_etat                      :        OnReceiveMsgLogMaintenanceEtat( *asnMsg.u.msg_log_maintenance_etat ); break;
        
        case T_MsgsSimMos_msg_log_ravitaillement_traitement_creation:    OnReceiveMsgLogRavitaillementTraitementCreation   ( *asnMsg.u.msg_log_ravitaillement_traitement_creation ); break;
        case T_MsgsSimMos_msg_log_ravitaillement_traitement_destruction: OnReceiveMsgLogRavitaillementTraitementDestruction( *asnMsg.u.msg_log_ravitaillement_traitement_destruction ); break;
        case T_MsgsSimMos_msg_log_ravitaillement_traitement_update:      OnReceiveMsgLogRavitaillementTraitementUpdate     ( *asnMsg.u.msg_log_ravitaillement_traitement_update ); break;
        case T_MsgsSimMos_msg_log_ravitaillement_etat                      :        OnReceiveMsgLogRavitaillementEtat( *asnMsg.u.msg_log_ravitaillement_etat ); break;
        case T_MsgsSimMos_msg_log_ravitaillement_quotas                : OnReceiveMsgLogRavitaillementQuotas               (  *asnMsg.u.msg_log_ravitaillement_quotas ); break;
            

        case T_MsgsSimMos_msg_log_sante_traitement_humain_creation:    OnReceiveMsgLogSanteTraitementHumainCreation   ( *asnMsg.u.msg_log_sante_traitement_humain_creation ); break;
        case T_MsgsSimMos_msg_log_sante_traitement_humain_destruction: OnReceiveMsgLogSanteTraitementHumainDestruction( *asnMsg.u.msg_log_sante_traitement_humain_destruction ); break;
        case T_MsgsSimMos_msg_log_sante_traitement_humain_update:      OnReceiveMsgLogSanteTraitementHumainUpdate     ( *asnMsg.u.msg_log_sante_traitement_humain_update ); break;
        case T_MsgsSimMos_msg_log_sante_etat                      :        OnReceiveMsgLogSanteEtat( *asnMsg.u.msg_log_sante_etat ); break;

        default:
            assert( false );
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
    std::cout << "BEGIN MSG DUMP =>" << std::endl;
    asnPERDecodeBuffer.SetTrace( true );
    asnMsgCtrl.Print( "MsgsSimMos" );
    std::cout << "END MSG DUMP =>" << std::endl;
#endif

    switch( asnMsg.t )
    {
        case T_MsgsSimMosWithContext_msg_limit_creation_ack:                    OnReceiveMsgLimitCreationAck                ( *asnMsg.u.msg_limit_creation_ack                  , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_limit_update_ack:                      OnReceiveMsgLimitUpdateAck                  ( *asnMsg.u.msg_limit_update_ack                    , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_limit_destruction_ack:                 OnReceiveMsgLimitDestructionAck             ( *asnMsg.u.msg_limit_destruction_ack               , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_lima_creation_ack:                     OnReceiveMsgLimaCreationAck                 ( *asnMsg.u.msg_lima_creation_ack                   , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_lima_update_ack:                       OnReceiveMsgLimaUpdateAck                   ( *asnMsg.u.msg_lima_update_ack                     , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_lima_destruction_ack:                  OnReceiveMsgLimaDestructionAck              ( *asnMsg.u.msg_lima_destruction_ack                , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_automate_order_ack:                    OnReceiveMsgAutomateOrderAck                ( *asnMsg.u.msg_automate_order_ack                  , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_pion_order_ack:                        OnReceiveMsgPionOrderAck                    ( *asnMsg.u.msg_pion_order_ack                      , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_order_conduite_ack:                    OnReceiveMsgOrderConduiteAck                ( *asnMsg.u.msg_order_conduite_ack                  , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_unit_magic_action_ack:                 OnReceiveMsgUnitMagicActionAck              ( *asnMsg.u.msg_unit_magic_action_ack               , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_set_automate_mode_ack:                 OnReceiveMsgSetAutomateModeAck              ( *asnMsg.u.msg_set_automate_mode_ack               , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_change_automate_ack:                   OnReceiveMsgChangeAutomateAck               ( *asnMsg.u.msg_change_automate_ack                 , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_change_diplomatie_ack:                 OnReceiveMsgChangeDiplomacyAck              ( *asnMsg.u.msg_change_diplomatie_ack               , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_change_groupe_connaissance_ack:        OnReceiveMsgChangeGroupeConnaissanceAck     ( *asnMsg.u.msg_change_groupe_connaissance_ack      , nCtx ); break;       
        case T_MsgsSimMosWithContext_msg_change_liens_logistiques_ack:          OnReceiveMsgChangeLiensLogistiquesAck       ( *asnMsg.u.msg_change_liens_logistiques_ack        , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_object_magic_action_ack:               OnReceiveMsgObjectMagicActionAck            ( *asnMsg.u.msg_object_magic_action_ack             , nCtx ); break; 
        case T_MsgsSimMosWithContext_msg_log_ravitaillement_change_quotas_ack:  OnReceiveMsgLogRavitaillementChangeQuotaAck (  asnMsg.u.msg_log_ravitaillement_change_quotas_ack, nCtx ); break; 
        case T_MsgsSimMosWithContext_msg_log_ravitaillement_pousser_flux_ack:   OnReceiveMsgLogRavitaillementPousserFlux    (  asnMsg.u.msg_log_ravitaillement_pousser_flux_ack , nCtx ); break; 

        default:
            assert( false );
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::OnError
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
bool MOS_AgentServerMsgMgr::OnError( DIN::DIN_Link& /*link*/, const DIN::DIN_ErrorDescription& info )
{
    //MT_LOG_INFO_MSG( MT_FormatString( "MOS -> AS - Message service error : %s", info.GetInfo().c_str() ).c_str() );
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::PrintInfoContextMsgErrorCode
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::PrintInfoContextMsgErrorCode( std::stringstream& msg, const ASN1T_EnumInfoContextErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumInfoContextErrorCode::no_error                     : msg << "No error"; break;
        case EnumInfoContextErrorCode::error_invalid_id             : msg << "Invalid ID"; break;
        case EnumInfoContextErrorCode::error_invalid_geometry       : msg << "Invalid geometry" ; break;
        case EnumInfoContextErrorCode::error_invalid_lima_function  : msg << "Invalid lima" ; break;
        default:
            assert( false );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::PrintCtrlMsgErrorCode
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::PrintCtrlMsgErrorCode( std::stringstream& msg, const ASN1T_EnumCtrlErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumCtrlErrorCode::no_error                 : msg << "No error"; break;
        case EnumCtrlErrorCode::error_already_started    : msg << "Already started"; break;
        case EnumCtrlErrorCode::error_not_started        : msg << "Not started"; break;
        case EnumCtrlErrorCode::error_not_paused         : msg << "Not paused"; break;
        case EnumCtrlErrorCode::error_already_paused     : msg << "Already paused"; break;
        case EnumCtrlErrorCode::error_invalid_time_factor: msg << "Invalid time factor"; break;

        default:
            assert( false );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::PrintOrderMsgErrorCode
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::PrintOrderMsgErrorCode( std::stringstream& msg, const ASN1T_EnumOrderErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumOrderErrorCode::no_error                               : msg << "No error"; break;
        case EnumOrderErrorCode::error_invalid_unit                     : msg << "Error : Invalid unit"; break;
        case EnumOrderErrorCode::error_invalid_limit                    : msg << "Error : Invalid limit"; break;
        case EnumOrderErrorCode::error_invalid_lima                     : msg << "Error : Invalid lima"; break;
        case EnumOrderErrorCode::error_invalid_mission                  : msg << "Error : Invalid mission"; break;
        case EnumOrderErrorCode::error_invalid_mission_parameters       : msg << "Error : Invalid mission parameters"; break;
        case EnumOrderErrorCode::error_unit_cannot_receive_order        : msg << "Error : Unit cannot receive this order"; break;
        case EnumOrderErrorCode::error_invalid_order_conduite           : msg << "Error : Invalid ordre de conduite"; break;
        case EnumOrderErrorCode::error_invalid_order_mission            : msg << "Error : Invalid ordre de mission"; break;
        case EnumOrderErrorCode::error_invalid_order_initial            : msg << "Error : Invalid ordre initial"; break;
        case EnumOrderErrorCode::error_invalid_order_conduite_parameters: msg << "Error : Invalid ordre de conduite parameters"; break;
        case EnumOrderErrorCode::error_unit_surrendered                 : msg << "Error : unit surrendered"; break;
        default:
            assert( false );
    }
}

   
//-----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::PrintUnitAttrMsgErrorCode
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::PrintUnitAttrMsgErrorCode( std::stringstream& msg, const ASN1T_EnumUnitAttrErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumUnitAttrErrorCode::no_error               : msg << "No error"; break;
        case EnumUnitAttrErrorCode::error_invalid_unit     : msg << "Invalid unit"; break;
        case EnumUnitAttrErrorCode::error_automate_embraye : msg << "Error : automate embraye"; break;
        case EnumUnitAttrErrorCode::error_invalid_attribute: msg << "Error : attribut invalide"; break;
        case EnumUnitAttrErrorCode::error_unit_surrendered : msg << "Error : unit surrendered" ; break;

        default:
            assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: MOS_AgentServerMsgMgr::PrintObjectMsgErrorCode
// Created: NLD 2003-08-06
// -----------------------------------------------------------------------------
void MOS_AgentServerMsgMgr::PrintObjectMsgErrorCode( std::stringstream& msg, const ASN1T_EnumObjectErrorCode& nCode )
{
    switch( nCode )
    {
        case EnumObjectErrorCode::no_error                          : msg << "No error"; break;
        case EnumObjectErrorCode::error_invalid_object              : msg << "Error : Invalid object"; break;
        case EnumObjectErrorCode::error_invalid_id                  : msg << "Error : Invalid id"; break;
        case EnumObjectErrorCode::error_invalid_camp                : msg << "Error : Invalid camp"; break;
        case EnumObjectErrorCode::error_invalid_localisation        : msg << "Error : Invalid localisation"; break;
        case EnumObjectErrorCode::error_missing_specific_attributes : msg << "Error : Missing specific attributes"; break;
        case EnumObjectErrorCode::error_invalid_specific_attributes : msg << "Error : Invalid specific attributes"; break;

        default:
            assert( false );
    }
}
