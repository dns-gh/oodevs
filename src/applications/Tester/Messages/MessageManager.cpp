// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-05-09 $
// $Archive: /MVW_v10/Build/SDK/TEST/src/Messages/MessageManager.cpp $
// $Author: Sbo $
// $Modtime: 19/07/05 17:14 $
// $Revision: 20 $
// $Workfile: MessageManager.cpp $
//
// *****************************************************************************

#include "Tester_pch.h"
#include "MessageManager.h"

#include "Types.h"
#include "Workspace.h"
#include "Network/NetworkManager.h"
#include "Network/ConnectionHandler.h"

#include "Entities/EntityManager.h"
#include "Entities/KnowledgeGroup.h"
#include "Entities/Team.h"
#include "Entities/Automat.h"
#include "Entities/Pawn.h"
#include "Entities/Object.h"
#include "Entities/Population.h"

#include "Types/TacticalLineManager.h"
#include "Types/TacticalLines/TacticalLine_Limit.h"
#include "Types/TacticalLines/TacticalLine_Lima.h"

#include "Actions/Scheduler.h"
#include "TestSets/TestSet_ABC.h"
#include "Tools/ASN_Tools.h"

#include "ASN_Messages.h"

using namespace TEST;
using namespace DIN;
using namespace NEK;

//-----------------------------------------------------------------------------
// Name: MessageManager constructor
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
MessageManager::MessageManager( Workspace& workspace, NetworkManager& networkManager )
    : workspace_      ( workspace )
    , bIsInitialized_ ( false )
{
    DIN_ConnectorGuest connector( eConnector_SIM_TEST );
    pMessageService_ = new DIN_MessageServiceUserCbk< MessageManager >( 
                                *this
                              , networkManager.GetDINEngine()
                              , connector
                              , "Msgs TEST Server -> SIM Server" );

    pMessageService_->RegisterReceivedMessage( eMsgInit                 , *this, & MessageManager::OnReceiveMsgInit                  );

    pMessageService_->RegisterReceivedMessage( eMsgKnowledgeGroup       , *this, & MessageManager::OnReceiveMsgKnowledgeGroup        );
    pMessageService_->RegisterReceivedMessage( eMsgArmy                 , *this, & MessageManager::OnReceiveMsgTeam                  );
    
    pMessageService_->RegisterReceivedMessage( eMsgTerrainType          , *this, & MessageManager::OnReceiveMsgPawnTerrainType       );

    pMessageService_->RegisterReceivedMessage( eMsgDebugDrawPoints      , *this, & MessageManager::OnReceiveMsgDebugDrawPoints       );

	pMessageService_->RegisterReceivedMessage( eMsgSimMos               , *this, & MessageManager::OnReceiveMsgSimMos                );
    pMessageService_->RegisterReceivedMessage( eMsgSimMosWithContext    , *this, & MessageManager::OnReceiveMsgSimMosWithContext     );
	
    pMessageService_->SetCbkOnError( MessageManager::OnError );
}


//-----------------------------------------------------------------------------
// Name: MessageManager destructor
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
MessageManager::~MessageManager()
{
    delete pMessageService_;
}

//=============================================================================
// SERVICE ACTIVATION
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MessageManager::Enable
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
void MessageManager::Enable( DIN_Link& link )
{
    pMessageService_->Enable( link );
}


//-----------------------------------------------------------------------------
// Name: MessageManager::Disable
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
void MessageManager::Disable( DIN_Link& link )
{
    pMessageService_->Disable( link );
}


//-----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgInit
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgInit( DIN_Link& /*linkFrom*/, DIN_Input& /*input*/ )
{
    // Nothing
}

//-----------------------------------------------------------------------------
// Name: MessageManager::BuildMessage
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
DIN::DIN_BufferedMessage MessageManager::BuildMessage()
{
    assert( pMessageService_ );
    return DIN::DIN_BufferedMessage( *pMessageService_ );
}

//=============================================================================
// ASN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MessageManager::SendMsgMosSim
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void MessageManager::SendMsgMosSim( ASN1T_MsgsMosSim& asnMsg )
{
	if ( !workspace_.GetNetworkManager().IsConnected() )
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

    pMessageService_->Send( workspace_.GetNetworkManager().GetLink(), eMsgMosSim, dinMsg );
}


//-----------------------------------------------------------------------------
// Name: MessageManager::SendMsgMosSimWithContext
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void MessageManager::SendMsgMosSimWithContext( ASN1T_MsgsMosSimWithContext& asnMsg, T_NetContextId nCtx )
{
    if ( !workspace_.GetNetworkManager().IsConnected() )
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

    pMessageService_->Send( workspace_.GetNetworkManager().GetLink(), eMsgMosSimWithContext, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::SendMsgMosSim
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MessageManager::SendMsgMosSim( ASN1OCTET* pMsg, int nMsgLength )
{
    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg.GetOutput().Append( pMsg, nMsgLength );

    pMessageService_->Send( workspace_.GetNetworkManager().GetLink(), eMsgMosSim, dinMsg );
}


// -----------------------------------------------------------------------------
// Name: MessageManager::SendMsgMosSimWithContext
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MessageManager::SendMsgMosSimWithContext( ASN1OCTET* pMsg, int nMsgLength, T_NetContextId nCtx )
{
    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg << nCtx;

    dinMsg.GetOutput().Append( pMsg, nMsgLength );

    pMessageService_->Send( workspace_.GetNetworkManager().GetLink(), eMsgMosSimWithContext, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::SendMsgDebugDrawPoints
// Created: SBO 2005-06-17
// -----------------------------------------------------------------------------
void MessageManager::SendMsgDebugDrawPoints( DIN::DIN_BufferedMessage& msg )
{
    pMessageService_->Send( workspace_.GetNetworkManager().GetLink(), eMsgDebugDrawPoints, msg  );
}

//=============================================================================
// DIN
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgKnowledgeGroup
// Created: SBO 2005-05-16
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgKnowledgeGroup( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    uint32 nId;
    input >> nId;
    if( !workspace_.GetEntityManager().FindKnowledgeGroup( nId ) )
    {
        KnowledgeGroup& knowledgeGroup = *new KnowledgeGroup( workspace_.GetEntityManager(), nId, input );
        workspace_.GetEntityManager().Register( knowledgeGroup );
        //MT_LOG_INFO_MSG( "TEST -> MM - Knowledge Group created ID#" << nId );
    }
    else
        MT_LOG_ERROR_MSG( "TEST -> MM - Knowledge Group with ID#" << nId << " already created" );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgArmy
// Created: NLD 2005-02-14
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgTeam( DIN::DIN_Link& /*linkForm*/, DIN::DIN_Input& input )
{
    uint32 nId;
    input >> nId;
    if( !workspace_.GetEntityManager().FindTeam( nId ) )
    {
        Team& team = *new Team( workspace_.GetEntityManager(), nId, input );
        workspace_.GetEntityManager().Register( team );
        //MT_LOG_INFO_MSG( "TEST -> MM - Team created ID#" << nId );
    }
    else
        MT_LOG_ERROR_MSG( "TEST -> MM - Team with ID#" << nId << " already created" );
}


//=============================================================================
// ASN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MessageManager constructor
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgSimMos( DIN_Link& /*linkFrom*/, DIN_Input& input )
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

    switch( asnMsg.t )
    {
		case T_MsgsSimMos_msg_ctrl_info:                     OnReceiveMsgCtrlInfo                  ( *asnMsg.u.msg_ctrl_info                    ); break;
        case T_MsgsSimMos_msg_ctrl_begin_tick:               OnReceiveMsgCtrlBeginTick             (  asnMsg.u.msg_ctrl_begin_tick              ); break;
        case T_MsgsSimMos_msg_ctrl_end_tick:                 OnReceiveMsgCtrlEndTick               ( *asnMsg.u.msg_ctrl_end_tick                ); break;
        case T_MsgsSimMos_msg_ctrl_change_time_factor_ack:   OnReceiveMsgCtrlChangeTimeFactorAck   ( *asnMsg.u.msg_ctrl_change_time_factor_ack  ); break;
        case T_MsgsSimMos_msg_ctrl_send_current_state_begin: OnReceiveMsgCtrlSendCurrentStateBegin (); break;
        case T_MsgsSimMos_msg_ctrl_send_current_state_end:   OnReceiveMsgCtrlSendCurrentStateEnd   (); break;

        case T_MsgsSimMos_msg_unit_attributes:               OnReceiveMsgUnitAttributes            ( *asnMsg.u.msg_unit_attributes              ); break;
        case T_MsgsSimMos_msg_unit_dotations:                OnReceiveMsgUnitDotations             ( *asnMsg.u.msg_unit_dotations               ); break;
        case T_MsgsSimMos_msg_unit_pathfind:                 OnReceiveMsgPawnPathFind              ( *asnMsg.u.msg_unit_pathfind                ); break;

        case T_MsgsSimMos_msg_change_automate:               OnReceiveMsgChangeAutomat             ( *asnMsg.u.msg_change_automate              ); break;
        case T_MsgsSimMos_msg_pion_creation:                 OnReceiveMsgPawnCreation              ( *asnMsg.u.msg_pion_creation                ); break;
        case T_MsgsSimMos_msg_automate_creation:             OnReceiveMsgAutomatCreation           ( *asnMsg.u.msg_automate_creation            ); break;
		case T_MsgsSimMos_msg_change_diplomatie:             OnReceiveMsgChangeTeamRelation        ( *asnMsg.u.msg_change_diplomatie            ); break;  

        case T_MsgsSimMos_msg_unit_knowledge_creation:       OnReceiveMsgUnitKnowledgeCreation     ( *asnMsg.u.msg_unit_knowledge_creation      ); break;
        case T_MsgsSimMos_msg_unit_knowledge_update:         OnReceiveMsgUnitKnowledgeUpdate       ( *asnMsg.u.msg_unit_knowledge_update        ); break;
        case T_MsgsSimMos_msg_unit_knowledge_destruction:    OnReceiveMsgUnitKnowledgeDestruction  ( *asnMsg.u.msg_unit_knowledge_destruction   ); break;

        case T_MsgsSimMos_msg_population_knowledge_creation:       OnReceiveMsgPopulationKnowledgeCreation     ( *asnMsg.u.msg_population_knowledge_creation      ); break;
        case T_MsgsSimMos_msg_population_knowledge_update:         OnReceiveMsgPopulationKnowledgeUpdate       ( *asnMsg.u.msg_population_knowledge_update        ); break;
        case T_MsgsSimMos_msg_population_knowledge_destruction:    OnReceiveMsgPopulationKnowledgeDestruction  ( *asnMsg.u.msg_population_knowledge_destruction   ); break;

        case T_MsgsSimMos_msg_object_creation:               OnReceiveMsgObjectCreation            ( *asnMsg.u.msg_object_creation              ); break;
        case T_MsgsSimMos_msg_object_update:                 OnReceiveMsgObjectUpdate              ( *asnMsg.u.msg_object_update                ); break;
        case T_MsgsSimMos_msg_object_destruction:            OnReceiveMsgObjectDestruction         ( asnMsg.u.msg_object_destruction            ); break;
        case T_MsgsSimMos_msg_object_knowledge_creation:     OnReceiveMsgObjectKnowledgeCreation   ( *asnMsg.u.msg_object_knowledge_creation    ); break; 
        case T_MsgsSimMos_msg_object_knowledge_update:       OnReceiveMsgObjectKnowledgeUpdate     ( *asnMsg.u.msg_object_knowledge_update      ); break; 
        case T_MsgsSimMos_msg_object_knowledge_destruction:  OnReceiveMsgObjectKnowledgeDestruction( *asnMsg.u.msg_object_knowledge_destruction ); break; 

        case T_MsgsSimMos_msg_population_creation:           OnReceiveMsgPopulationCreation        ( *asnMsg.u.msg_population_creation          ); break;
        case T_MsgsSimMos_msg_population_update:             OnReceiveMsgPopulationUpdate          ( *asnMsg.u.msg_population_update            ); break;
        default:
			;
	}
}

//-----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgSimMosWithContext
// Created: NLD 2003-02-26
//-----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgSimMosWithContext( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    T_NetContextId nCtx;
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

    switch( asnMsg.t )
    {
        case T_MsgsSimMosWithContext_msg_change_automate_ack:            OnReceiveMsgChangeAutomateAck      ( *asnMsg.u.msg_change_automate_ack           , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_change_diplomatie_ack:          OnReceiveMsgChangeTeamRelationAck  ( *asnMsg.u.msg_change_diplomatie_ack         , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_change_groupe_connaissance_ack: OnReceiveMsgChangeKnowledgeGroupAck( *asnMsg.u.msg_change_groupe_connaissance_ack, nCtx ); break;
        case T_MsgsSimMosWithContext_msg_automate_order_ack:             OnReceiveMsgAutomateOrderAck       ( *asnMsg.u.msg_automate_order_ack            , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_pion_order_ack:                 OnReceiveMsgPionOrderAck           ( *asnMsg.u.msg_pion_order_ack                , nCtx ); break;
        case T_MsgsSimMosWithContext_msg_set_automate_mode_ack:          OnReceiveMsgSetAutomatModeAck      ( *asnMsg.u.msg_set_automate_mode_ack         , nCtx ); break;
        default:
            ;
    }
}


//-----------------------------------------------------------------------------
// TIME MANAGEMENT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgCtrlInfo
// Created: SBO 2005-05-17
//-----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgCtrlInfo( const ASN1T_MsgCtrlInfo& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "CtrlInfo - Current Tick: " << asnMsg.current_tick 
                 << " - Tick duration : "       << asnMsg.tick_duration
                 << " - Time factor : "         << asnMsg.time_factor;

    workspace_.SetTickDuration( asnMsg.tick_duration );
    workspace_.SetTick        ( asnMsg.current_tick  );
    //workspace_.SetTimeFactor  ( asnMSg.time_factor   );
    
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgCtrlBeginTick
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgCtrlBeginTick( const ASN1T_MsgCtrlBeginTick& asnMsg )
{
    workspace_.SetTick( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgCtrlEndTick
// Created: NLD 2005-06-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgCtrlEndTick( const ASN1T_MsgCtrlEndTick& /*asnMsg*/ )
{
    MT_LOG_INFO_MSG( "Tick: " << workspace_.GetTick() << " (next test at " << workspace_.GetScheduler().GetNextExecutionTick() << ")" );
    // save recovery point
    workspace_.SaveRecoveryPoint();
    // run required actions
    workspace_.GetScheduler().Run( workspace_.GetTick() );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgCtrlChangeTimeFactorAck
// Created: SBO 2005-06-03
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgCtrlChangeTimeFactorAck( const ASN1T_MsgCtrlChangeTimeFactorAck& /*asnMsg*/ )
{
/*
    // don't do anything if original command was malformed
    if( asnMsg.error_code == EnumCtrlErrorCode::no_error )
    {
        workspace_.SetTimeFactor( asnMsg.time_factor );
    }
*/
}


// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgCtrlSendCurrentStateBegin
// Created: SBO 2005-06-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgCtrlSendCurrentStateBegin()
{
    MT_LOG_INFO_MSG( "Begin current state..." );
}


// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgCtrlSendCurrentStateEnd
// Created: SBO 2005-06-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgCtrlSendCurrentStateEnd()
{
    MT_LOG_INFO_MSG( "End current state..." );
    // change time factor
    workspace_.SetTimeFactor();
    // create default limits
    workspace_.GetTacticalLineManager().UpdateToSim();
    if( !bIsInitialized_ )
    {
        // load tests
        workspace_.GetTestSet().Load( workspace_ );
        // save recovery point
        workspace_.SaveRecoveryPoint();
        // run scheduler
        workspace_.GetScheduler().RecoverIfNeeded( workspace_.GetTick() );
        workspace_.GetScheduler().Run( workspace_.GetTick() );
        bIsInitialized_ = true;
    }
}

//-----------------------------------------------------------------------------
// ENTITIES MANAGEMENT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgAutomateCreation
// Created: SBO 2005-05-11
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgAutomatCreation( const ASN1T_MsgAutomateCreation& asnMsg )
{
    if( !workspace_.GetEntityManager().FindAutomat( asnMsg.oid_automate ) )
    {
        // automat creation
        Automat& automat = *new Automat( workspace_, asnMsg );
        workspace_.GetEntityManager().Register( automat );
    }
    else
        MT_LOG_ERROR_MSG( "TEST -> MM - Automat with ID#" << asnMsg.oid_automate << " already created" );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPionCreation
// Created: SBO 2005-05-11
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPawnCreation( const ASN1T_MsgPionCreation& asnMsg )
{
    if( !workspace_.GetEntityManager().FindPawn( asnMsg.oid_pion ) )
    {
        Pawn& pawn = *new Pawn( workspace_, asnMsg );
        workspace_.GetEntityManager().Register( pawn );
    }
    else
        MT_LOG_ERROR_MSG( "TEST -> MM - Pawn with ID#" << asnMsg.oid_pion << " already created" );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgChangeAutomat
// Created: SBO 2005-05-16
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgChangeAutomat( const ASN1T_MsgChangeAutomate& asnMsg )
{
    Pawn* pPawn = workspace_.GetEntityManager().FindPawn( asnMsg.oid_pion );
    assert( pPawn );

    Automat* pAutomat = workspace_.GetEntityManager().FindAutomat( asnMsg.oid_automate );
    assert( pAutomat );
    
    pPawn->OnAutomatChanged( *pAutomat );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgChangeAutomateAck
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgChangeAutomateAck( const ASN1T_MsgChangeAutomateAck& asnMsg, T_NetContextId /*nCtx*/ )
{
    // don't do anything if original command was malformed
    if( asnMsg.error_code == EnumOrderErrorCode::no_error )
    {
        Pawn* pPawn = workspace_.GetEntityManager().FindPawn( asnMsg.oid_pion );
        assert( pPawn );

        Automat* pAutomat = workspace_.GetEntityManager().FindAutomat( asnMsg.oid_automate );
        assert( pAutomat );
        
        pPawn->OnAutomatChanged( *pAutomat );
    }
}

//-----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgUnitAttributes
// Created: SBO 2005-05-16
//-----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgUnitAttributes( const ASN1T_MsgUnitAttributes& asnMsg )
{
    Pawn* pPawn = workspace_.GetEntityManager().FindPawn( asnMsg.oid_pion );
    assert( pPawn );
    pPawn->OnAttributeUpdated( asnMsg );
}

//-----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgUnitDotations
// Created: SBO 2005-05-16
//-----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgUnitDotations( const ASN1T_MsgUnitDotations& asnMsg )
{
    Pawn* pPawn = workspace_.GetEntityManager().FindPawn( asnMsg.oid_pion );
    assert( pPawn );
    pPawn->OnAttributeUpdated( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgChangeTeamRelation
// Created: SBO 2005-05-16
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgChangeTeamRelation( const ASN1T_MsgChangeDiplomatie& asnMsg )
{
    Team* pTeam1 = workspace_.GetEntityManager().FindTeam( asnMsg.oid_camp1 );
    assert( pTeam1 );

    Team* pTeam2 = workspace_.GetEntityManager().FindTeam( asnMsg.oid_camp2 );
    assert( pTeam2 );

    pTeam1->SetRelation( *pTeam2, asnMsg.diplomatie );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgChangeTeamRelation
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgChangeTeamRelationAck( const ASN1T_MsgChangeDiplomatieAck& asnMsg, T_NetContextId /*nCtx*/ )
{
    // don't do anything if original command was malformed
    if( asnMsg.error_code == EnumChangeDiplomatieErrorCode::no_error )
    {
        Team* pTeam1 = workspace_.GetEntityManager().FindTeam( asnMsg.oid_camp1 );
        assert( pTeam1 );

        Team* pTeam2 = workspace_.GetEntityManager().FindTeam( asnMsg.oid_camp2 );
        assert( pTeam2 );

        pTeam1->SetRelation( *pTeam2, asnMsg.diplomatie );
    }
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgChangeKnowledgeGroupAck
// Created: SBO 2005-05-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgChangeKnowledgeGroupAck( const ASN1T_MsgChangeGroupeConnaissanceAck& asnMsg, T_NetContextId /*nCtx*/ )
{
    if( asnMsg.error_code == EnumChangeGroupeConnaissanceErrorCode::no_error )
    {
        Automat* pAutomat = workspace_.GetEntityManager().FindAutomat( asnMsg.oid_automate );
        assert( pAutomat );

        KnowledgeGroup* pKnowledgeGroup = workspace_.GetEntityManager().FindKnowledgeGroup( asnMsg.oid_groupe_connaissance );
        assert( pKnowledgeGroup );

        pAutomat->OnKnowledgeGroupChanged( *pKnowledgeGroup );
    }
}

//-----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPawnTerrainType
// Created: SBO 2005-06-15
//-----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPawnPathFind( const ASN1T_MsgUnitPathFind& asnMsg )
{
    Pawn* pPawn = workspace_.GetEntityManager().FindPawn( asnMsg.oid_pion );
    assert( pPawn );
    pPawn->OnReceivePathfind( asnMsg );
}

//-----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPawnTerrainType
// Created: SBO 2005-06-15
//-----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPawnTerrainType( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& input )
{
    uint32 nPawnId;
    input >> nPawnId;
    Pawn* pPawn = workspace_.GetEntityManager().FindPawn( nPawnId );
    assert( pPawn );
    pPawn->OnReceiveTerrainType( input );
}

//-----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPawnTerrainType
// Created: SBO 2005-06-15
//-----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgDebugDrawPoints( DIN::DIN_Link& /*linkFrom*/, DIN::DIN_Input& /*input*/ )
{
    //std::cout << "DEBUG: draw points" << std::endl;
}


// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgObjectCreation
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgObjectCreation( const ASN1T_MsgObjectCreation& asnMsg )
{
    if( !workspace_.GetEntityManager().FindObject( asnMsg.oid ) )
    {
        Object& object = *new Object( workspace_, asnMsg );
        workspace_.GetEntityManager().Register( object );
    }
    else
        MT_LOG_ERROR_MSG( "TEST -> MM - Object with ID#" << asnMsg.oid << " already created" );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgObjectUpdate
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgObjectUpdate( const ASN1T_MsgObjectUpdate& /*asnMsg*/ )
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgObjectDestruction
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgObjectDestruction( const ASN1T_MsgObjectDestruction& asnMsg )
{
    Object* pObject = workspace_.GetEntityManager().FindObject( asnMsg );
    
    if( pObject )
    {
        workspace_.GetEntityManager().Unregister( *pObject );
        delete pObject;
    }
    else
        MT_LOG_ERROR_MSG( "TEST -> MM - Deleting object with ID#" << asnMsg << " which does not exist" );
}

// =============================================================================
// POPULATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationCreation
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationCreation( const ASN1T_MsgPopulationCreation& asnMsg )
{
    if( !workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population ) )
    {
        Population& population = *new Population( workspace_, asnMsg );
        workspace_.GetEntityManager().Register( population );
    }
    else
        MT_LOG_ERROR_MSG( "TEST -> MM - Population with ID#" << asnMsg.oid_population << " already created" );
}
    
// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationUpdate
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationUpdate( const ASN1T_MsgPopulationUpdate& asnMsg )
{
    Population* pPopulation = workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population );
    assert( pPopulation );
    pPopulation->OnUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationConcentrationCreation
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationConcentrationCreation( const ASN1T_MsgPopulationConcentrationCreation& asnMsg )
{
    Population* pPopulation = workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population );
    assert( pPopulation );
    pPopulation->OnConcentrationCreated( asnMsg );
}
    
// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationConcentrationUpdate
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationConcentrationUpdate( const ASN1T_MsgPopulationConcentrationUpdate& asnMsg )
{
    Population* pPopulation = workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population );
    assert( pPopulation );
    pPopulation->OnConcentrationUpdated( asnMsg );
}
    
// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationConcentrationDestruction
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationConcentrationDestruction( const ASN1T_MsgPopulationConcentrationDestruction& asnMsg )
{
    Population* pPopulation = workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population );
    assert( pPopulation );
    pPopulation->OnConcentrationDestroyed( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationFlowCreation
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationFlowCreation( const ASN1T_MsgPopulationFluxCreation& asnMsg )
{
    Population* pPopulation = workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population );
    assert( pPopulation );
    pPopulation->OnFlowCreated( asnMsg );
}
    
// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationFlowUpdate
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationFlowUpdate( const ASN1T_MsgPopulationFluxUpdate& asnMsg )
{
    Population* pPopulation = workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population );
    assert( pPopulation );
    pPopulation->OnFlowUpdated( asnMsg );
}
    
// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationFlowDestruction
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationFlowDestruction( const ASN1T_MsgPopulationFluxDestruction& asnMsg )
{
    Population* pPopulation = workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population );
    assert( pPopulation );
    pPopulation->OnFlowDestroyed( asnMsg );
}

// =============================================================================
// KNOWLEDGE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgUnitKnowledgeCreation
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgUnitKnowledgeCreation( const ASN1T_MsgUnitKnowledgeCreation& asnMsg )
{
    KnowledgeGroup* pKGroup = workspace_.GetEntityManager().FindKnowledgeGroup( asnMsg.oid_groupe_possesseur );
    assert( pKGroup );
    pKGroup->OnReceiveMsgUnitKnowledgeCreation( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgUnitKnowledgeUpdate
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgUnitKnowledgeUpdate( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg )
{
    KnowledgeGroup* pKGroup = workspace_.GetEntityManager().FindKnowledgeGroup( asnMsg.oid_groupe_possesseur );
    assert( pKGroup );
    pKGroup->OnReceiveMsgUnitKnowledgeUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgUnitKnowledgeDestruction
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg )
{
    KnowledgeGroup* pKGroup = workspace_.GetEntityManager().FindKnowledgeGroup( asnMsg.oid_groupe_possesseur );
    assert( pKGroup );
    pKGroup->OnReceiveMsgUnitKnowledgeDestruction( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationKnowledgeCreation
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationKnowledgeCreation( const ASN1T_MsgPopulationKnowledgeCreation& asnMsg )
{
    KnowledgeGroup* pKGroup = workspace_.GetEntityManager().FindKnowledgeGroup( asnMsg.oid_groupe_possesseur );
    assert( pKGroup );
    pKGroup->OnReceiveMsgPopulationKnowledgeCreation( asnMsg );
}
    
// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationKnowledgeUpdate
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationKnowledgeUpdate( const ASN1T_MsgPopulationKnowledgeUpdate& asnMsg )
{
    KnowledgeGroup* pKGroup = workspace_.GetEntityManager().FindKnowledgeGroup( asnMsg.oid_groupe_possesseur );
    assert( pKGroup );
    pKGroup->OnReceiveMsgPopulationKnowledgeUpdate( asnMsg );
}
    
// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationKnowledgeDestruction
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationKnowledgeDestruction( const ASN1T_MsgPopulationKnowledgeDestruction& asnMsg )
{
    KnowledgeGroup* pKGroup = workspace_.GetEntityManager().FindKnowledgeGroup( asnMsg.oid_groupe_possesseur );
    assert( pKGroup );
    pKGroup->OnReceiveMsgPopulationKnowledgeDestruction( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgObjectKnowledgeCreation
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgObjectKnowledgeCreation( const ASN1T_MsgObjectKnowledgeCreation& asnMsg )
{
    Team* pTeam = workspace_.GetEntityManager().FindTeam( asnMsg.oid_camp_possesseur );
    assert( pTeam );
    pTeam->OnReceiveMsgObjectKnowledgeCreation( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgObjectKnowledgeUpdate
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgObjectKnowledgeUpdate( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    Team* pTeam = workspace_.GetEntityManager().FindTeam( asnMsg.oid_camp_possesseur );
    assert( pTeam );
    pTeam->OnReceiveMsgObjectKnowledgeUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgObjectKnowledgeDestruction
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg )
{
    Team* pTeam = workspace_.GetEntityManager().FindTeam( asnMsg.oid_camp_possesseur );
    assert( pTeam );
    pTeam->OnReceiveMsgObjectKnowledgeDestruction( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPionOrderAck
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPionOrderAck( const ASN1T_MsgPionOrderAck& asnMsg, T_NetContextId /*nCtx*/ )
{
    Pawn* pPawn = workspace_.GetEntityManager().FindPawn( asnMsg.oid_unite_executante );
    assert( pPawn );

    std::stringstream strOutputMsg;
    strOutputMsg << "Agent [" << asnMsg.oid_unite_executante << "] (" << pPawn->GetType().GetName() << ") "
                 << "PionOrderAck - " << ASN_Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgAutomateOrderAck
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgAutomateOrderAck( const ASN1T_MsgAutomateOrderAck& asnMsg, T_NetContextId /*nCtx*/ )
{
    Automat* pAutomat = workspace_.GetEntityManager().FindAutomat( asnMsg.oid_unite_executante );
    assert( pAutomat );

    std::stringstream strOutputMsg;
    strOutputMsg << "Automat [" << asnMsg.oid_unite_executante << "] (" << pAutomat->GetType().GetName() << ") "
                 << "AutomatOrderAck - " << ASN_Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgSetAutomatModeAck
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgSetAutomatModeAck( const ASN1T_MsgSetAutomateModeAck& /*asnMsg*/, T_NetContextId /*nCtx*/ )
{
    /*
    Automat* pAutomat = workspace_.GetEntityManager().FindAutomat( asnMsg.unit_id );
    assert( pAutomat );

    std::stringstream strOutputMsg;
    strOutputMsg << "Automat [" << asnMsg.unit_id << "] SetMode - " << ASN_Tools::ToString( asnMsg.error_code );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
    */
}

//-----------------------------------------------------------------------------
// LIMITS/LIMAS MANAGEMENT
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgLimitCreation
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgLimitCreation( const ASN1T_MsgLimitCreation& asnMsg )
{
    TacticalLine_Limit& limit = *new TacticalLine_Limit( asnMsg );
    workspace_.GetTacticalLineManager().Register( limit );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgLimitDestruction
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgLimitDestruction( const ASN1T_MsgLimitDestruction& asnMsg )
{
    TacticalLine_ABC* pLimit = workspace_.GetTacticalLineManager().Find( asnMsg );
    if( pLimit )
        workspace_.GetTacticalLineManager().UnRegister( *pLimit );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgLimaCreation
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgLimaCreation( const ASN1T_MsgLimaCreation& asnMsg )
{
    TacticalLine_Lima& lima = *new TacticalLine_Lima( asnMsg );
    workspace_.GetTacticalLineManager().Register( lima );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgLimaDestruction
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgLimaDestruction( const ASN1T_MsgLimaDestruction& asnMsg )
{
    TacticalLine_ABC* pLima = workspace_.GetTacticalLineManager().Find( asnMsg );
    if( pLima )
        workspace_.GetTacticalLineManager().Register( *pLima );
}

//-----------------------------------------------------------------------------
// ERROR MANAGEMENT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MessageManager::OnError
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
bool MessageManager::OnError( DIN::DIN_Link& /*link*/, const DIN::DIN_ErrorDescription& /*info*/ )
{
    return false;
}
