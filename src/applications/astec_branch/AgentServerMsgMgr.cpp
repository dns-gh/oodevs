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
#include "LineManager.h"
#include "Lima.h"
#include "Limit.h"
#include "ASN_Messages.h"
#include "Net_Def.h"
#include "KnowledgeGroup.h"
#include "Team.h"
#include "Meteo_Manager.h"
#include "Tools.h"
#include "MsgRecorder.h"
#include "LogMaintenanceConsign.h"
#include "LogMedicalConsign.h"
#include "LogSupplyConsign.h"
#include "Population.h"
#include "PopulationKnowledge.h"
#include "Simulation.h"
#include "Model.h"
#include "AgentsModel.h"
#include "TeamsModel.h"
#include "KnowledgeGroupsModel.h"
#include "ObjectsModel.h"
#include "LogisticsModel.h"
#include "LimitsModel.h"
#include "FiresModel.h"
#include "App.h"
#include "DIN_Types.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr constructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
AgentServerMsgMgr::AgentServerMsgMgr( DIN::DIN_Engine& engine, Model& model, Simulation& simu )
    : model_           ( model ) 
    , simulation_      ( simu )
    , session_         ( 0 )
    , bReceivingState_ ( true )
    , msgRecorder_     ( * new MsgRecorder( *this ) )
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

    pMessageService_->SetCbkOnError( AgentServerMsgMgr::OnError );
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

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::IsPaused
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
bool AgentServerMsgMgr::IsPaused() const
{
    return simulation_.IsPaused();
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
    dinMsg << agent.GetId();
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
    // $$$$ AGE 2006-02-10: 
    SendMsgEnableUnitVisionCones();
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomateCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomateCreation( const ASN1T_MsgAutomateCreation& message )
{
    model_.agents_.CreateAgent( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPionCreation
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPionCreation( const ASN1T_MsgPionCreation& message )
{
    model_.agents_.CreateAgent( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgKnowledgeGroup
// Created: NLD 2004-09-09
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgKnowledgeGroup( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    unsigned long id, team;
    input >> id >> team; // $$$$ AGE 2006-02-15: Pkoi dans cet ordre la ndd ?
    model_.teams_.Get( team ).CreateKnowledgeGroup( id );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgArmy
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgArmy( DIN::DIN_Link& , DIN::DIN_Input& input )
{
    model_.teams_.CreateTeam( input );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgProfilingValues
// Created: NLD 2002-10-14
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgProfilingValues( DIN_Link& /*linkFrom*/, DIN_Input& )
{
    // $$$$ AGE 2006-02-10: NOTHING
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgTrace
// Created: NLD 2003-01-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgTrace( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    uint32 nAgentID;
    input >> nAgentID;
    model_.agents_.FindAllAgent( nAgentID )->Update( TraceMessage( input ) );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints
// Created: NLD 2005-03-22
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgDebugDrawPoints( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    MIL_AgentID nAgentID;
    input >> nAgentID;
    model_.agents_.FindAllAgent( nAgentID )->Update( DebugPointsMessage( input ) );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitVisionCones
// Created: NLD 2003-02-12
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitVisionCones( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    MIL_AgentID nAgentID;
    input >> nAgentID;
    model_.agents_.GetAgent( nAgentID ).Update( VisionConesMessage( input ) );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitInterVisibility( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    MIL_AgentID nAgentID;
    input >> nAgentID;
    model_.agents_.GetAgent( nAgentID ).Update( DetectionMessage( input ) );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationInterVisibility( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    MIL_AgentID nAgentID;
    input >> nAgentID;
    model_.agents_.GetAgent( nAgentID ).Update( ConcentrationDetectionMessage( input ) );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowInterVisibility( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    MIL_AgentID nAgentID;
    input >> nAgentID;
    model_.agents_.GetAgent( nAgentID ).Update( FlowDetectionMessage( input ) );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationCollision
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationCollision( DIN::DIN_Link& , DIN::DIN_Input& input )
{
    MIL_AgentID nAgentID;
    input >> nAgentID;
    model_.agents_.GetAgent( nAgentID ).Update( PopulationCollisionMessage( input ) );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility
// Created: NLD 2003-03-17
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectInterVisibility( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    MIL_AgentID nAgentID;
    input >> nAgentID;
    model_.agents_.GetAgent( nAgentID ).Update( ObjectDetectionMessage( input ) );
}

//=============================================================================
// ASN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::SendMsgMosSim
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::SendMsgMosSim( ASN1T_MsgsMosSim& message )
{
    if( ! session_ )
        return;
    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );

    ASN1C_MsgsMosSim asnMsgCtrl( asnPEREncodeBuffer, message );

    if( asnMsgCtrl.Encode() != ASN_OK )
    {
        asnPEREncodeBuffer.PrintErrorInfo();
        assert( false ); //$$$ TMP
        return;
    }

    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg.GetOutput().Append( asnPEREncodeBuffer.GetMsgPtr(), asnPEREncodeBuffer.GetMsgLen() );

    Send( eMsgMosSim, dinMsg );

    msgRecorder_.OnNewMsg( message.t, asnPEREncodeBuffer );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::SendMsgMosSimWithContext
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::SendMsgMosSimWithContext( ASN1T_MsgsMosSimWithContext& message, MIL_MOSContextID nCtx )
{
    if( ! session_ )
        return;

    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );

    ASN1C_MsgsMosSimWithContext asnMsgCtrl( asnPEREncodeBuffer, message );

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

    msgRecorder_.OnNewMsgWithContext( message.t, nCtx, asnPEREncodeBuffer );
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

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::CheckAcknowledge
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
template< typename T >
bool AgentServerMsgMgr::CheckAcknowledge( const char* type, const T& message )
{
    MT_LOG_INFO( type << "- Code: " << Tools::ToString( message ), eReceived, 0 );
    return message == EnumObjectErrorCode::no_error;
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlPauseAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlPauseAck( const ASN1T_MsgCtrlPauseAck& message )
{
    if( CheckAcknowledge( "CtrlPauseAck", message ) )
        simulation_.Pause( true );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlResumeAck
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlResumeAck( const ASN1T_MsgCtrlResumeAck& message )
{
    if( CheckAcknowledge( "CtrlResumeAck", message ) )
        simulation_.Pause( false );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlChangeTimeFactorAck
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlChangeTimeFactorAck( const ASN1T_MsgCtrlChangeTimeFactorAck& message )
{
    if( CheckAcknowledge( "CtrlTimeFactorAck", message.error_code ) )
        simulation_.ChangeSpeed( (int)message.time_factor );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlInfo
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlInfo( const ASN1T_MsgCtrlInfo& message )
{
    simulation_.Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlBeginTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlBeginTick( const ASN1T_MsgCtrlBeginTick& message )
{
    simulation_.BeginTick( message );
    msgRecorder_.OnTimeTick( simulation_.GetTime() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlEndTick
// Created: NLD 2004-04-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlEndTick( const ASN1T_MsgCtrlEndTick& /*message*/ )
{
    simulation_.EndTick();
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitAttributes
// Created: NLD 2003-02-27
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitAttributes( const ASN1T_MsgUnitAttributes& message )
{
    model_.agents_.GetAgent( message.oid_pion ).Update( message );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitDotations
// Created: NLD 2003-03-06
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitDotations( const ASN1T_MsgUnitDotations& message )
{
    model_.agents_.GetAgent( message.oid_pion ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementCreation( const ASN1T_MsgLogMaintenanceTraitementEquipementCreation& message )
{
    model_.logistics_.CreateMaintenanceConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementDestruction( const ASN1T_MsgLogMaintenanceTraitementEquipementDestruction& message )
{
    model_.logistics_.DeleteMaintenanceConsign( message.oid_consigne );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceTraitementEquipementUpdate( const ASN1T_MsgLogMaintenanceTraitementEquipementUpdate& message )
{
    model_.logistics_.GetMaintenanceConsign( message.oid_consigne ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogMaintenanceEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogMaintenanceEtat( const ASN1T_MsgLogMaintenanceEtat& message )
{
    model_.agents_.GetAgent( message.oid_pion ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainCreation( const ASN1T_MsgLogSanteTraitementHumainCreation& message )
{
    model_.logistics_.CreateMedicalConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainDestruction( const ASN1T_MsgLogSanteTraitementHumainDestruction& message )
{
    model_.logistics_.DeleteMedicalConsign( message.oid_consigne );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSanteTraitementHumainUpdate( const ASN1T_MsgLogSanteTraitementHumainUpdate& message )
{
    model_.logistics_.GetMedicalConsign( message.oid_consigne ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogSanteEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogSanteEtat( const ASN1T_MsgLogSanteEtat& message )
{
    model_.agents_.GetAgent( message.oid_pion ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementCreation
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementCreation( const ASN1T_MsgLogRavitaillementTraitementCreation& message )
{
    model_.logistics_.CreateSupplyConsign( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementDestruction
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementDestruction( const ASN1T_MsgLogRavitaillementTraitementDestruction& message )
{
    model_.logistics_.DeleteSupplyConsign( message.oid_consigne );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementUpdate
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementTraitementUpdate( const ASN1T_MsgLogRavitaillementTraitementUpdate& message )
{
    model_.logistics_.GetSupplyConsign( message.oid_consigne ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementEtat
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementEtat( const ASN1T_MsgLogRavitaillementEtat&  message )
{
    model_.agents_.GetAgent( message.oid_pion ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementQuotas
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementQuotas( const ASN1T_MsgLogRavitaillementQuotas& message )
{
    model_.agents_.GetAgent( message.oid_automate ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementChangeQuotaAck( const ASN1T_MsgLogRavitaillementChangeQuotasAck& message, MIL_MOSContextID )
{
//    std::stringstream strOutputMsg;
//    strOutputMsg << "MSG Received : MsgLogRavitaillementChangeQuotaAck - Code ";
//    switch( message )
//    {
//        case MsgLogRavitaillementChangeQuotasAck::error_invalid_donneur : strOutputMsg << "error_invalid_donneur"; break;
//        case MsgLogRavitaillementChangeQuotasAck::error_invalid_receveur: strOutputMsg << "error_invalid_receveur"; break;
//        case MsgLogRavitaillementChangeQuotasAck::no_error:               strOutputMsg << "no_error"; break;
//        default:
//            assert( false );
//    }
//    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLogRavitaillementPousserFluxAck
// Created: AGE 2005-04-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLogRavitaillementPousserFluxAck( const ASN1T_MsgLogRavitaillementPousserFluxAck& message, MIL_MOSContextID )
{
//    std::stringstream strOutputMsg;
//    strOutputMsg << "MSG Received : MsgLogRavitaillementPousserFlux - Code ";
//    switch( message )
//    {
//        case MsgLogRavitaillementPousserFluxAck::error_invalid_donneur        : strOutputMsg << "error_invalid_donneur"; break;
//        case MsgLogRavitaillementPousserFluxAck::error_invalid_receveur       : strOutputMsg << "error_invalid_receveur"; break;
//        case MsgLogRavitaillementPousserFluxAck::error_ravitaillement_en_cours: strOutputMsg << "error_ravitaillement_en_cours"; break;
//        case MsgLogRavitaillementPousserFluxAck::no_error                     : strOutputMsg << "no_error"; break;
//        default:
//            assert( false );
//    }
//    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitPathFind
// Created: NLD 2003-03-31
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitPathFind( const ASN1T_MsgUnitPathFind& message )
{
    model_.agents_.GetAgent( message.oid_pion ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitMagicActionAck( const ASN1T_MsgUnitMagicActionAck& message, MIL_MOSContextID /*nCtx*/ )
{
    CheckAcknowledge( "UnitMagicActionAck", message.error_code );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectMagicActionAck
// Created: NLD 2003-03-05
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectMagicActionAck( const ASN1T_MsgObjectMagicActionAck& message, MIL_MOSContextID /*nCtx*/ )
{
    CheckAcknowledge( "ObjectMagicActionAck", message.error_code );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlMeteoGlobalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlMeteoGlobalAck()
{
    CheckAcknowledge( "CtrlMeteoGlobaleAck", EnumObjectErrorCode::no_error );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCtrlMeteoLocalAck
// Created: NLD 2003-08-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCtrlMeteoLocalAck()
{
    CheckAcknowledge( "CtrlMeteoLocaleAck", EnumObjectErrorCode::no_error );
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
    if( bUseMosLimits_ )
        model_.limits_.UpdateToSim(); // $$$$ AGE 2006-02-15: WTF
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
void AgentServerMsgMgr::OnReceiveMsgLimitCreationAck( const ASN1T_MsgLimitCreationAck& message, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimitCreationAck - Code: " << Tools::ToString( message.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( message.error_code == EnumInfoContextErrorCode::no_error )
    {
        TacticalLine_ABC* limit = model_.limits_.Find( message.oid );
        if( limit )
            limit->Update( message );
        else
            Limit::idManager_.LockIdentifier( message.oid ); // $$$$ AGE 2006-02-15: dégueu
    }
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitUpdateAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitUpdateAck( const ASN1T_MsgLimitUpdateAck& message, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimitUpdateAck - Code: " << Tools::ToString( message.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( message.error_code == EnumInfoContextErrorCode::no_error )
    {
        TacticalLine_ABC* limit = model_.limits_.Find( message.oid );
        if( limit )
            limit->Update( message );
    }
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitDestructionAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitDestructionAck( const ASN1T_MsgLimitDestructionAck& message, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimitDestructionAck - Code: " << Tools::ToString( message.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( message.error_code == EnumInfoContextErrorCode::no_error )
        model_.limits_.DeleteLimit( message.oid );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaCreationAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaCreationAck( const ASN1T_MsgLimaCreationAck& message, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimaCreationAck - Code: " << Tools::ToString( message.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( message.error_code == EnumInfoContextErrorCode::no_error )
    {
        TacticalLine_ABC* lima = model_.limits_.Find( message.oid );
        if( lima )
            lima->Update( message );
        else
            Lima::idManager_.LockIdentifier( message.oid ); // $$$$ AGE 2006-02-15: pareil qu'au dessus
    }
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaUpdateAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaUpdateAck( const ASN1T_MsgLimaUpdateAck& message, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimaUpdateAck - Code: " << Tools::ToString( message.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( message.error_code == EnumInfoContextErrorCode::no_error )
    {
        TacticalLine_ABC* limit = model_.limits_.Find( message.oid );
        if( limit )
            limit->Update( message );
    }
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaDestructionAck
// Created: NLD 2003-02-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaDestructionAck( const ASN1T_MsgLimaDestructionAck& message, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "LimaDestructionAck - Code: " << Tools::ToString( message.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

    if( message.error_code == EnumInfoContextErrorCode::no_error )
        model_.limits_.DeleteLima( message.oid );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitCreation
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitCreation( const ASN1T_MsgLimitCreation& message )
{
    // If we are receiving those lines while initializing from the sim, use those lines instead of our lines.
    if( bReceivingState_  && bUseMosLimits_ )
    {
        bUseMosLimits_ = false;
        model_.limits_.UseSimTacticalLines();
        model_.limits_.Create( message );
    }
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimitDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimitDestruction( const ASN1T_MsgLimitDestruction& message )
{
    model_.limits_.DeleteLimit( message );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaCreation
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaCreation( const ASN1T_MsgLimaCreation& message )
{
    // If we are receiving those lines while initializing from the sim, use those lines instead of our lines.
    if( bReceivingState_  && bUseMosLimits_ )
    {
        bUseMosLimits_ = false;
        model_.limits_.UseSimTacticalLines();
        model_.limits_.Create( message );
    }
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgLimaDestruction
// Created: NLD 2003-04-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgLimaDestruction( const ASN1T_MsgLimaDestruction& message )
{
    model_.limits_.DeleteLima( message );
}

//=============================================================================
// ORDRES
//=============================================================================

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomateOrderAck
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomateOrderAck( const ASN1T_MsgAutomateOrderAck& message, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "AutomateOrderAck - Code: " << Tools::ToString( message.error_code );
    MT_LOG_INFO( strOutputMsg.str().c_str(), eReceived, 0 );

//APE    App::GetApp().GetMainWindow().GetAgentEditor().NotifyAgentMissionAck( message.error_code );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPionOrderAck
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPionOrderAck( const ASN1T_MsgPionOrderAck& message, MIL_MOSContextID /*nCtx*/ )
{
    MT_LOG_INFO( "Agent[" << message.oid_unite_executante << "] PionOrderAck - Code: " << Tools::ToString( message.error_code ), eReceived, 0 );
    model_.agents_.FindAllAgent( message.oid_unite_executante )->Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgOrderConduiteAck
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgOrderConduiteAck( const ASN1T_MsgOrderConduiteAck& message, MIL_MOSContextID /*nCtx*/ )
{
    MT_LOG_INFO( "OrderConduiteAck - Code: " << Tools::ToString( message.error_code ), eReceived, 0 );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgCR
// Created: NLD 2002-09-02
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgCR( const ASN1T_MsgCR& message )
{    
    model_.agents_.FindAllAgent( message.unit_id )->Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgOrderManagement
// Created: NLD 2003-03-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgOrderManagement( const ASN1T_MsgOrderManagement& message )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "OrderManagement - Etat: ";
    switch( message.etat )
    {
        case EnumOrderState::started  : strOutputMsg << "Démarré"; break;
        case EnumOrderState::stopped  : strOutputMsg << "Arrêté"; break;
        case EnumOrderState::cancelled: strOutputMsg << "Annulé"; break;
    }
    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );
}


//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgAutomateMRT
// Created: NLD 2003-05-13
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgAutomateMRT( const ASN1T_MsgAutomateMRT& message )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "AutomateMRT - Automate " << message.automate_id;

    std::stringstream strOutputMsg2;
    for( uint i = 0; i < message.missions.n; ++i )
    {
        ASN1T_MsgPionOrder& asnPionOrder = message.missions.elem[i];

        strOutputMsg2 << "Pion " << asnPionOrder.oid_unite_executante
                     << " - Mission " << asnPionOrder.mission.t << endl;

        model_.agents_.FindAllAgent( asnPionOrder.oid_unite_executante )->Update( asnPionOrder );
    }
    MT_LOG_INFO( strOutputMsg.str(), eReceived, strOutputMsg2.str().c_str() );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPionOrder
// Created: NLD 2003-10-01
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPionOrder( const ASN1T_MsgPionOrder& message )
{
    MT_LOG_INFO( "PionOrder - Mission " << message.mission.t, eReceived, 0 );
    model_.agents_.FindAllAgent( message.oid_unite_executante )->Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgWaitForOrderConduite
// Created: NLD 2003-04-04
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgWaitForOrderConduite( const ASN1T_MsgAttenteOrdreConduite& message )
{
    model_.agents_.FindAllAgent( message.unit_id )->Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgSetAutomateModeAck
// Created: NLD 2003-04-11
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgSetAutomateModeAck( const ASN1T_MsgSetAutomateModeAck& message, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "SetAutomateModeAck - Etat: ";
    switch( message.error_code )
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
/** @param  message
    @param  nCtx
    */
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeAutomateAck( const ASN1T_MsgChangeAutomateAck& message, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "ChangeAutomateAck - Etat: ";
    switch( message.error_code )
    {
    case EnumChangeAutomateErrorCode::no_error                 : strOutputMsg << "no_error"; break;
    case EnumChangeAutomateErrorCode::error_camps_incompatibles: strOutputMsg << "error_camps_incompatibles"; break;
    case EnumChangeAutomateErrorCode::error_invalid_automate   : strOutputMsg << "error_invalid_automate"; break;
    case EnumChangeAutomateErrorCode::error_invalid_pion       : strOutputMsg << "error_invalid_pion"; break;
    }

    if( message.error_code == EnumOrderErrorCode::no_error )
    {
        model_.agents_.GetAgent( message.oid_pion ).Update( message );
    }

    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgChangeAutomate
/** @param  message
*/
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeAutomate( const ASN1T_MsgChangeAutomate& message )
{
    model_.agents_.GetAgent( message.oid_pion ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgChangeDiplomacyAck
/** @param  message
    @param  nCtx
    */
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeDiplomacyAck( const ASN1T_MsgChangeDiplomatieAck& message, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "ChangeDiplomacyAck( - Etat: ";
    switch( message.error_code )
    {
    case EnumChangeDiplomatieErrorCode::no_error           : strOutputMsg << "no_error"; break;
    case EnumChangeDiplomatieErrorCode::error_invalid_camp : strOutputMsg << "error_invalid_camp"; break;
    }

    model_.teams_.GetTeam( message.oid_camp1 ).Update( message );
    model_.teams_.GetTeam( message.oid_camp2 ).Update( message );

    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgChangeGroupeConnaissanceAck
/** @param  message
    @param  nCtx
    */
// Created: APE 2004-10-27
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeGroupeConnaissanceAck( const ASN1T_MsgChangeGroupeConnaissanceAck& message, MIL_MOSContextID )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "ChangeGroupeConnaissanceAck - Etat: ";
    switch( message.error_code )
    {
    case EnumChangeGroupeConnaissanceErrorCode::no_error                          : strOutputMsg << "no_error"; break;
    case EnumChangeGroupeConnaissanceErrorCode::error_invalid_automate            : strOutputMsg << "error_invalid_automate"; break;
    case EnumChangeGroupeConnaissanceErrorCode::error_invalid_camp                : strOutputMsg << "error_invalid_camp"; break;
    case EnumChangeGroupeConnaissanceErrorCode::error_invalid_groupe_connaissance : strOutputMsg << "error_invalid_groupe_connaissance"; break;
    }

    if( message.error_code == EnumChangeGroupeConnaissanceErrorCode::no_error )
    {
        model_.agents_.GetAgent( message.oid_automate ).Update( message );
    }

    MT_LOG_INFO( strOutputMsg.str(), eReceived, 0 );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgChangeLiensLogistiquesAck
// Created: AGE 2005-04-06
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeLiensLogistiquesAck( const ASN1T_MsgChangeLiensLogistiquesAck& message, MIL_MOSContextID )
{
//    std::stringstream strOutputMsg;
//    strOutputMsg << "MSG Received : ChangeLiensLogistiquesAck - Etat: ";
//    switch( message.error_code )
//    {
//        case EnumChangeLiensLogistiquesErrorCode::error_invalid_automate               : strOutputMsg << "error_invalid_automate"; break;
//        case EnumChangeLiensLogistiquesErrorCode::error_invalid_automate_maintenance   : strOutputMsg << "error_invalid_automate_maintenance"; break;
//        case EnumChangeLiensLogistiquesErrorCode::error_invalid_automate_ravitaillement: strOutputMsg << "error_invalid_automate_ravitaillement"; break;
//        case EnumChangeLiensLogistiquesErrorCode::error_invalid_automate_sante         : strOutputMsg << "error_invalid_automate_sante"; break;
//        case EnumChangeLiensLogistiquesErrorCode::error_invalid_automate_tc2           : strOutputMsg << "error_invalid_automate_tc2"; break;
//        case EnumChangeLiensLogistiquesErrorCode::error_unit_surrendered               : strOutputMsg << "error_unit_surrendered"; break;
//        case EnumChangeLiensLogistiquesErrorCode::no_error                             : strOutputMsg << "no_error"; break;
//    }
//    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );

    model_.agents_.GetAgent( message.oid_automate ).Update( message );
}

// =============================================================================
// KNOWLEDGES
// =============================================================================

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeCreation( const ASN1T_MsgUnitKnowledgeCreation& message )
{
    model_.knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeUpdate
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeUpdate( const ASN1T_MsgUnitKnowledgeUpdate& message )
{
    model_.knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& message )
{
    model_.knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeCreation
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeCreation( const ASN1T_MsgObjectKnowledgeCreation& message )
{
    model_.teams_.Get( message.oid_camp_possesseur ).Update( message );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeUpdate
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeUpdate( const ASN1T_MsgObjectKnowledgeUpdate& message )
{
    model_.teams_.Get( message.oid_camp_possesseur ).Update( message );
}


// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeDestruction
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& message )
{
    model_.teams_.Get( message.oid_camp_possesseur ).Update( message );
}

// =============================================================================
// POPULATION KNOWLEDGE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeCreation
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeCreation( const ASN1T_MsgPopulationKnowledgeCreation& message )
{
    model_.knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}
    
// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeUpdate( const ASN1T_MsgPopulationKnowledgeUpdate& message )
{
    model_.knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}
    
// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeDestruction
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationKnowledgeDestruction( const ASN1T_MsgPopulationKnowledgeDestruction& message )
{
    model_.knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeCreation
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeCreation( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& message )
{
    model_.knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeUpdate
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& message )
{
    model_.knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeDestruction
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationConcentrationKnowledgeDestruction( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& message )
{
    model_.knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeCreation
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeCreation( const ASN1T_MsgPopulationFluxKnowledgeCreation& message )
{
    model_.knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeUpdate
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeUpdate( const ASN1T_MsgPopulationFluxKnowledgeUpdate& message )
{
    model_.knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeDestruction
// Created: SBO 2005-10-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationFlowKnowledgeDestruction( const ASN1T_MsgPopulationFluxKnowledgeDestruction& message )
{
    model_.knowledgeGroups_.Get( message.oid_groupe_possesseur ).Update( message );
}

//=============================================================================
// OBJECTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectCreation
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectCreation( const ASN1T_MsgObjectCreation& message )
{
    model_.objects_.CreateObject( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectUpdate
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectUpdate( const ASN1T_MsgObjectUpdate& message )
{
    model_.objects_.GetObject( message.oid ).Update( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgObjectDestruction
// Created: NLD 2003-04-29
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgObjectDestruction( const ASN1T_MsgObjectDestruction& message )
{
    model_.objects_.DeleteObject( message );
}

//=============================================================================
// TIRS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStartPionFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStartPionFire( const ASN1T_MsgStartPionFire& message )
{
    Agent& src = model_.agents_.GetAgent( message.tireur );
    src.Update( message );
    model_.fires_.AddFire( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStopPionFire
// Created: NLD 2003-04-30
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStopPionFire( const ASN1T_MsgStopPionFire& message )
{
    Agent_ABC* src = model_.fires_.FindFirer( message );
    if( src )
        src->Update( message );
    model_.fires_.RemoveFire( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStartFireEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStartFireEffect( const ASN1T_MsgStartFireEffect& message )
{
    // $$$$ AGE 2006-03-13: 
//    MT_LOG_INFO( "Start ammunition meteo effect - ID: " << message.oid_effet << " Type:" << message.type, eReceived, 0 );
//    App::GetApp().GetWeatherManager().RegisterAmmoMeteoEffect( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStopFireEffect
// Created: JVT 04-03-25
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStopFireEffect( const ASN1T_MsgStopFireEffect& message )
{
    // $$$$ AGE 2006-03-13: 
//    MT_LOG_INFO( "Stop ammunition meteo effect - ID: " << message, eReceived, 0 );
//    App::GetApp().GetWeatherManager().UnregisterAmmoMeteoEffect( message );
}

//-----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgExplosion
// Created: NLD 2003-05-28
//-----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgExplosion( const ASN1T_MsgExplosion& message )
{
    // fire results
    model_.objects_.GetObject( message.oid_objet ).Update( message );
    MT_LOG_INFO( "Explosion" << " - ID objet " << message.oid_objet, eReceived, 0 );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStartPopulationFire
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStartPopulationFire( const ASN1T_MsgStartPopulationFire& message )
{
    Population& src = model_.agents_.GetPopulation( message.oid_src );
    src.Update( message );
    model_.fires_.AddFire( message );
}
    
// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgStopPopulationFire
// Created: SBO 2005-12-05
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgStopPopulationFire( const ASN1T_MsgStopPopulationFire& message )
{
    Agent_ABC* src = model_.fires_.FindFirer( message );
    if( src )
        src->Update( message );
    model_.fires_.RemoveFire( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgChangeDiplomatie
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgChangeDiplomatie( const ASN1T_MsgChangeDiplomatie& message )
{
    model_.teams_.Get( message.oid_camp1 ).Update( message );
    model_.teams_.Get( message.oid_camp2 ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationCreation( const ASN1T_MsgPopulationCreation& message )
{
	model_.agents_.CreatePopulation( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationUpdate( const ASN1T_MsgPopulationUpdate& message )
{
	model_.agents_.GetPopulation( message.oid_population ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationConcentrationCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationConcentrationCreation( const ASN1T_MsgPopulationConcentrationCreation& message )
{
    model_.agents_.GetPopulation( message.oid_population ).Update( message );
	}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationConcentrationDestruction
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationConcentrationDestruction( const ASN1T_MsgPopulationConcentrationDestruction& message )
{
    model_.agents_.GetPopulation( message.oid_population ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationConcentrationUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationConcentrationUpdate( const ASN1T_MsgPopulationConcentrationUpdate& message )
{
    model_.agents_.GetPopulation( message.oid_population ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationFluxCreation
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationFluxCreation( const ASN1T_MsgPopulationFluxCreation& message )
{
    model_.agents_.GetPopulation( message.oid_population ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationFluxDestruction
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationFluxDestruction  ( const ASN1T_MsgPopulationFluxDestruction& message )
{
    model_.agents_.GetPopulation( message.oid_population ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnMsgPopulationFluxUpdate
// Created: HME 2005-09-28
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnMsgPopulationFluxUpdate( const ASN1T_MsgPopulationFluxUpdate& message )
{
    model_.agents_.GetPopulation( message.oid_population ).Update( message );
}

// -----------------------------------------------------------------------------
// Name: AgentServerMsgMgr::OnReceiveMsgPopulationMagicActionAck
// Created: SBO 2005-11-02
// -----------------------------------------------------------------------------
void AgentServerMsgMgr::OnReceiveMsgPopulationMagicActionAck( const ASN1T_MsgPopulationMagicActionAck& message, MIL_MOSContextID /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "PopulationMagicActionAck - Code: " << Tools::ToString( message.error_code );
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
    ASN1T_MsgsSimMos message;
    ASN1C_MsgsSimMos asnMsgCtrl( asnPERDecodeBuffer, message );
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

    switch( message.t )
    {
        case T_MsgsSimMos_msg_ctrl_info:                            OnReceiveMsgCtrlInfo                  ( *message.u.msg_ctrl_info                           ); break;
        case T_MsgsSimMos_msg_ctrl_begin_tick:                      OnReceiveMsgCtrlBeginTick             (  message.u.msg_ctrl_begin_tick                     ); break;
        case T_MsgsSimMos_msg_ctrl_end_tick:                        OnReceiveMsgCtrlEndTick               ( *message.u.msg_ctrl_end_tick                       ); break;
        case T_MsgsSimMos_msg_ctrl_stop_ack:                        break;
        case T_MsgsSimMos_msg_ctrl_pause_ack:                       OnReceiveMsgCtrlPauseAck              ( message.u.msg_ctrl_pause_ack                       ); break;
        case T_MsgsSimMos_msg_ctrl_resume_ack:                      OnReceiveMsgCtrlResumeAck             ( message.u.msg_ctrl_resume_ack                      ); break;
        case T_MsgsSimMos_msg_ctrl_change_time_factor_ack:          OnReceiveMsgCtrlChangeTimeFactorAck   ( *message.u.msg_ctrl_change_time_factor_ack         ); break;
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

        case T_MsgsSimMos_msg_limit_creation:                       OnReceiveMsgLimitCreation             ( *message.u.msg_limit_creation                      ); break;
        case T_MsgsSimMos_msg_limit_destruction:                    OnReceiveMsgLimitDestruction          ( message.u.msg_limit_destruction                    ); break;
        case T_MsgsSimMos_msg_lima_creation:                        OnReceiveMsgLimaCreation              ( *message.u.msg_lima_creation                       ); break;
        case T_MsgsSimMos_msg_lima_destruction:                     OnReceiveMsgLimaDestruction           ( message.u.msg_lima_destruction                     ); break;

        case T_MsgsSimMos_msg_unit_knowledge_creation:              OnReceiveMsgUnitKnowledgeCreation     ( *message.u.msg_unit_knowledge_creation             ); break;
        case T_MsgsSimMos_msg_unit_knowledge_update:                OnReceiveMsgUnitKnowledgeUpdate       ( *message.u.msg_unit_knowledge_update               ); break;
        case T_MsgsSimMos_msg_unit_knowledge_destruction:           OnReceiveMsgUnitKnowledgeDestruction  ( *message.u.msg_unit_knowledge_destruction          ); break;

        case T_MsgsSimMos_msg_unit_attributes:                      OnReceiveMsgUnitAttributes            ( *message.u.msg_unit_attributes                     ); break;
        case T_MsgsSimMos_msg_unit_dotations:                       OnReceiveMsgUnitDotations             ( *message.u.msg_unit_dotations                      ); break;
        case T_MsgsSimMos_msg_unit_pathfind:                        OnReceiveMsgUnitPathFind              ( *message.u.msg_unit_pathfind                       ); break;

        case T_MsgsSimMos_msg_start_pion_fire:                      OnReceiveMsgStartPionFire             ( *message.u.msg_start_pion_fire                     ); break;
        case T_MsgsSimMos_msg_stop_pion_fire:                       OnReceiveMsgStopPionFire              ( *message.u.msg_stop_pion_fire                      ); break;
        case T_MsgsSimMos_msg_start_population_fire:                OnReceiveMsgStartPopulationFire       ( *message.u.msg_start_population_fire               ); break;
        case T_MsgsSimMos_msg_stop_population_fire:                 OnReceiveMsgStopPopulationFire        ( *message.u.msg_stop_population_fire                ); break;

        case T_MsgsSimMos_msg_explosion:                            OnReceiveMsgExplosion                 ( *message.u.msg_explosion                           ); break;
        case T_MsgsSimMos_msg_cr:                                   OnReceiveMsgCR                        ( *message.u.msg_cr                                  ); break;
        case T_MsgsSimMos_msg_start_fire_effect:                    OnReceiveMsgStartFireEffect           ( *message.u.msg_start_fire_effect ); break;
        case T_MsgsSimMos_msg_stop_fire_effect:                     OnReceiveMsgStopFireEffect            ( message.u.msg_stop_fire_effect ); break;

        case T_MsgsSimMos_msg_order_management:                     OnReceiveMsgOrderManagement           ( *message.u.msg_order_management                    ); break;
        case T_MsgsSimMos_msg_attente_ordre_conduite:               OnReceiveMsgWaitForOrderConduite      ( *message.u.msg_attente_ordre_conduite              ); break;
        case T_MsgsSimMos_msg_annule_attente_ordre_conduite:        break; //$$$ TODO
        case T_MsgsSimMos_msg_automate_mrt:                         OnReceiveMsgAutomateMRT               ( *message.u.msg_automate_mrt                        ); break;
        case T_MsgsSimMos_msg_pion_order:                           OnReceiveMsgPionOrder                 ( *message.u.msg_pion_order ); break;

        case T_MsgsSimMos_msg_object_creation:                      OnReceiveMsgObjectCreation            ( *message.u.msg_object_creation                     ); break;
        case T_MsgsSimMos_msg_object_update:                        OnReceiveMsgObjectUpdate              ( *message.u.msg_object_update                       ); break;
        case T_MsgsSimMos_msg_object_destruction:                   OnReceiveMsgObjectDestruction         ( message.u.msg_object_destruction                   ); break;
        case T_MsgsSimMos_msg_object_knowledge_creation:            OnReceiveMsgObjectKnowledgeCreation   ( *message.u.msg_object_knowledge_creation           ); break;
        case T_MsgsSimMos_msg_object_knowledge_update:              OnReceiveMsgObjectKnowledgeUpdate     ( *message.u.msg_object_knowledge_update             ); break;
        case T_MsgsSimMos_msg_object_knowledge_destruction:         OnReceiveMsgObjectKnowledgeDestruction( *message.u.msg_object_knowledge_destruction        ); break;

        case T_MsgsSimMos_msg_change_automate:                      OnReceiveMsgChangeAutomate            ( *message.u.msg_change_automate ); break;

        case T_MsgsSimMos_msg_pion_creation:                        OnReceiveMsgPionCreation              ( *message.u.msg_pion_creation                       ); break;
        case T_MsgsSimMos_msg_automate_creation:                    OnReceiveMsgAutomateCreation          ( *message.u.msg_automate_creation                   ); break;
        case T_MsgsSimMos_msg_change_diplomatie:                    OnReceiveMsgChangeDiplomatie          ( *message.u.msg_change_diplomatie                   ); break;

        case T_MsgsSimMos_msg_log_maintenance_traitement_equipement_creation:    OnReceiveMsgLogMaintenanceTraitementEquipementCreation   ( *message.u.msg_log_maintenance_traitement_equipement_creation ); break;
        case T_MsgsSimMos_msg_log_maintenance_traitement_equipement_destruction: OnReceiveMsgLogMaintenanceTraitementEquipementDestruction( *message.u.msg_log_maintenance_traitement_equipement_destruction ); break;
        case T_MsgsSimMos_msg_log_maintenance_traitement_equipement_update:      OnReceiveMsgLogMaintenanceTraitementEquipementUpdate     ( *message.u.msg_log_maintenance_traitement_equipement_update ); break;
        case T_MsgsSimMos_msg_log_maintenance_etat:                              OnReceiveMsgLogMaintenanceEtat( *message.u.msg_log_maintenance_etat ); break;

        case T_MsgsSimMos_msg_log_ravitaillement_traitement_creation:    OnReceiveMsgLogRavitaillementTraitementCreation   ( *message.u.msg_log_ravitaillement_traitement_creation ); break;
        case T_MsgsSimMos_msg_log_ravitaillement_traitement_destruction: OnReceiveMsgLogRavitaillementTraitementDestruction( *message.u.msg_log_ravitaillement_traitement_destruction ); break;
        case T_MsgsSimMos_msg_log_ravitaillement_traitement_update:      OnReceiveMsgLogRavitaillementTraitementUpdate     ( *message.u.msg_log_ravitaillement_traitement_update ); break;
        case T_MsgsSimMos_msg_log_ravitaillement_etat:                   OnReceiveMsgLogRavitaillementEtat( *message.u.msg_log_ravitaillement_etat ); break;
        case T_MsgsSimMos_msg_log_ravitaillement_quotas:                 OnReceiveMsgLogRavitaillementQuotas               (  *message.u.msg_log_ravitaillement_quotas ); break;


        case T_MsgsSimMos_msg_log_sante_traitement_humain_creation:    OnReceiveMsgLogSanteTraitementHumainCreation   ( *message.u.msg_log_sante_traitement_humain_creation ); break;
        case T_MsgsSimMos_msg_log_sante_traitement_humain_destruction: OnReceiveMsgLogSanteTraitementHumainDestruction( *message.u.msg_log_sante_traitement_humain_destruction ); break;
        case T_MsgsSimMos_msg_log_sante_traitement_humain_update:      OnReceiveMsgLogSanteTraitementHumainUpdate     ( *message.u.msg_log_sante_traitement_humain_update ); break;
        case T_MsgsSimMos_msg_log_sante_etat:                          OnReceiveMsgLogSanteEtat( *message.u.msg_log_sante_etat ); break;

        case T_MsgsSimMos_msg_population_creation                       : OnMsgPopulationCreation                ( *message.u.msg_population_creation ); break;
        case T_MsgsSimMos_msg_population_update                         : OnMsgPopulationUpdate                  ( *message.u.msg_population_update ); break;
        case T_MsgsSimMos_msg_population_concentration_creation         : OnMsgPopulationConcentrationCreation   ( *message.u.msg_population_concentration_creation ); break;
        case T_MsgsSimMos_msg_population_concentration_destruction      : OnMsgPopulationConcentrationDestruction( *message.u.msg_population_concentration_destruction ); break;
        case T_MsgsSimMos_msg_population_concentration_update           : OnMsgPopulationConcentrationUpdate     ( *message.u.msg_population_concentration_update ); break;
        case T_MsgsSimMos_msg_population_flux_creation                  : OnMsgPopulationFluxCreation            ( *message.u.msg_population_flux_creation ); break;
        case T_MsgsSimMos_msg_population_flux_destruction               : OnMsgPopulationFluxDestruction         ( *message.u.msg_population_flux_destruction ); break;
        case T_MsgsSimMos_msg_population_flux_update                    : OnMsgPopulationFluxUpdate              ( *message.u.msg_population_flux_update ); break;

        case T_MsgsSimMos_msg_population_knowledge_creation                  : OnReceiveMsgPopulationKnowledgeCreation                ( *message.u.msg_population_knowledge_creation                  ); break;
        case T_MsgsSimMos_msg_population_knowledge_update                    : OnReceiveMsgPopulationKnowledgeUpdate                  ( *message.u.msg_population_knowledge_update                    ); break;
        case T_MsgsSimMos_msg_population_knowledge_destruction               : OnReceiveMsgPopulationKnowledgeDestruction             ( *message.u.msg_population_knowledge_destruction               ); break;
        case T_MsgsSimMos_msg_population_concentration_knowledge_creation    : OnReceiveMsgPopulationConcentrationKnowledgeCreation   ( *message.u.msg_population_concentration_knowledge_creation    ); break;
        case T_MsgsSimMos_msg_population_concentration_knowledge_update      : OnReceiveMsgPopulationConcentrationKnowledgeUpdate     ( *message.u.msg_population_concentration_knowledge_update      ); break;
        case T_MsgsSimMos_msg_population_concentration_knowledge_destruction : OnReceiveMsgPopulationConcentrationKnowledgeDestruction( *message.u.msg_population_concentration_knowledge_destruction ); break;
        case T_MsgsSimMos_msg_population_flux_knowledge_creation             : OnReceiveMsgPopulationFlowKnowledgeCreation            ( *message.u.msg_population_flux_knowledge_creation             ); break;
        case T_MsgsSimMos_msg_population_flux_knowledge_update               : OnReceiveMsgPopulationFlowKnowledgeUpdate              ( *message.u.msg_population_flux_knowledge_update               ); break;
        case T_MsgsSimMos_msg_population_flux_knowledge_destruction          : OnReceiveMsgPopulationFlowKnowledgeDestruction         ( *message.u.msg_population_flux_knowledge_destruction          ); break;

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
    ASN1T_MsgsSimMosWithContext message;
    ASN1C_MsgsSimMosWithContext asnMsgCtrl( asnPERDecodeBuffer, message );
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

    switch( message.t )
    {
        case T_MsgsSimMosWithContext_msg_limit_creation_ack:                     OnReceiveMsgLimitCreationAck                ( *message.u.msg_limit_creation_ack                  , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_limit_update_ack:                       OnReceiveMsgLimitUpdateAck                  ( *message.u.msg_limit_update_ack                    , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_limit_destruction_ack:                  OnReceiveMsgLimitDestructionAck             ( *message.u.msg_limit_destruction_ack               , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_lima_creation_ack:                      OnReceiveMsgLimaCreationAck                 ( *message.u.msg_lima_creation_ack                   , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_lima_update_ack:                        OnReceiveMsgLimaUpdateAck                   ( *message.u.msg_lima_update_ack                     , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_lima_destruction_ack:                   OnReceiveMsgLimaDestructionAck              ( *message.u.msg_lima_destruction_ack                , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_automate_order_ack:                     OnReceiveMsgAutomateOrderAck                ( *message.u.msg_automate_order_ack                  , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_pion_order_ack:                         OnReceiveMsgPionOrderAck                    ( *message.u.msg_pion_order_ack                      , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_order_conduite_ack:                     OnReceiveMsgOrderConduiteAck                ( *message.u.msg_order_conduite_ack                  , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_unit_magic_action_ack:                  OnReceiveMsgUnitMagicActionAck              ( *message.u.msg_unit_magic_action_ack               , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_set_automate_mode_ack:                  OnReceiveMsgSetAutomateModeAck              ( *message.u.msg_set_automate_mode_ack               , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_change_automate_ack:                    OnReceiveMsgChangeAutomateAck               ( *message.u.msg_change_automate_ack                 , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_change_diplomatie_ack:                  OnReceiveMsgChangeDiplomacyAck              ( *message.u.msg_change_diplomatie_ack               , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_change_groupe_connaissance_ack:         OnReceiveMsgChangeGroupeConnaissanceAck     ( *message.u.msg_change_groupe_connaissance_ack      , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_object_magic_action_ack:                OnReceiveMsgObjectMagicActionAck            ( *message.u.msg_object_magic_action_ack             , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_change_liens_logistiques_ack:           OnReceiveMsgChangeLiensLogistiquesAck       ( *message.u.msg_change_liens_logistiques_ack        , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_log_ravitaillement_pousser_flux_ack:    OnReceiveMsgLogRavitaillementPousserFluxAck (  message.u.msg_log_ravitaillement_pousser_flux_ack , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_log_ravitaillement_change_quotas_ack:   OnReceiveMsgLogRavitaillementChangeQuotaAck (  message.u.msg_log_ravitaillement_change_quotas_ack, nCtx ); break;
        case T_MsgsSimMosWithContext_msg_population_magic_action_ack:            OnReceiveMsgPopulationMagicActionAck        ( *message.u.msg_population_magic_action_ack         , nCtx ); break;

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

