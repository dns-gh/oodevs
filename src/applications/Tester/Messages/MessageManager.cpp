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

#include "Tester_Pch.h"
#include "MessageManager.h"

#include "Types.h"
#include "TestManager.h"
#include "Network/NetworkManager.h"
#include "Network/ConnectionHandler.h"

#include "Entities/EntityManager.h"
#include "Entities/KnowledgeGroup.h"
#include "Entities/Team.h"
#include "Entities/Automat.h"
#include "Entities/Pawn.h"
#include "Entities/Object.h"

#include "Entities/TacticalLines/TacticalLineManager.h"
#include "Entities/TacticalLines/TacticalLine_Limit.h"
#include "Entities/TacticalLines/TacticalLine_Lima.h"

#include "Actions/Scheduler.h"
#include "TestSets/TestSet_ABC.h"

#include "ASN_Messages.h"

using namespace TEST;
using namespace DIN;
using namespace NEK;

//-----------------------------------------------------------------------------
// Name: MessageManager constructor
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
MessageManager::MessageManager( NetworkManager& networkManager )
    : bPaused_          ( false )
    , bReceivingState_  ( true )
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
	if ( !TestManager::GetTestManager().GetNetworkManager().IsConnected() )
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

    pMessageService_->Send( TestManager::GetTestManager().GetNetworkManager().GetLink(), eMsgMosSim, dinMsg );
}


//-----------------------------------------------------------------------------
// Name: MessageManager::SendMsgMosSimWithContext
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void MessageManager::SendMsgMosSimWithContext( ASN1T_MsgsMosSimWithContext& asnMsg, T_NetContextId nCtx )
{
    if ( !TestManager::GetTestManager().GetNetworkManager().IsConnected() )
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

    pMessageService_->Send( TestManager::GetTestManager().GetNetworkManager().GetLink(), eMsgMosSimWithContext, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::SendMsgMosSim
// Created: APE 2004-10-20
// -----------------------------------------------------------------------------
void MessageManager::SendMsgMosSim( ASN1OCTET* pMsg, int nMsgLength )
{
    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg.GetOutput().Append( pMsg, nMsgLength );

    pMessageService_->Send( TestManager::GetTestManager().GetNetworkManager().GetLink(), eMsgMosSim, dinMsg );
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

    pMessageService_->Send( TestManager::GetTestManager().GetNetworkManager().GetLink(), eMsgMosSimWithContext, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::SendMsgDebugDrawPoints
// Created: SBO 2005-06-17
// -----------------------------------------------------------------------------
void MessageManager::SendMsgDebugDrawPoints( DIN::DIN_BufferedMessage& msg )
{
    pMessageService_->Send( TestManager::GetTestManager().GetNetworkManager().GetLink(), eMsgDebugDrawPoints, msg  );
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
    if( !KnowledgeGroup::Find( nId ) )
    {
        KnowledgeGroup* pKnowledgeGroup = new KnowledgeGroup( nId, input );
        KnowledgeGroup::Register( *pKnowledgeGroup );
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
    if( !Team::Find( nId ) )
    {
        Team* pTeam = new Team( nId, input );
        Team::Register( *pTeam );
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

        case T_MsgsSimMos_msg_object_creation:               OnReceiveMsgObjectCreation            ( *asnMsg.u.msg_object_creation              ); break;
        case T_MsgsSimMos_msg_object_update:                 OnReceiveMsgObjectUpdate              ( *asnMsg.u.msg_object_update                ); break;
        case T_MsgsSimMos_msg_object_destruction:            OnReceiveMsgObjectDestruction         ( asnMsg.u.msg_object_destruction            ); break;
        case T_MsgsSimMos_msg_object_knowledge_creation:     OnReceiveMsgObjectKnowledgeCreation   ( *asnMsg.u.msg_object_knowledge_creation    ); break; 
        case T_MsgsSimMos_msg_object_knowledge_update:       OnReceiveMsgObjectKnowledgeUpdate     ( *asnMsg.u.msg_object_knowledge_update      ); break; 
        case T_MsgsSimMos_msg_object_knowledge_destruction:  OnReceiveMsgObjectKnowledgeDestruction( *asnMsg.u.msg_object_knowledge_destruction ); break; 
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
        case T_MsgsSimMosWithContext_msg_pion_order_ack:                 OnReceiveMsgPionOrderAck           ( *asnMsg.u.msg_pion_order_ack                , nCtx ); break;
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

    TestManager::GetTestManager().SetTickDuration( asnMsg.tick_duration );
    TestManager::GetTestManager().SetTick        ( asnMsg.current_tick  );
    //TestManager::GetTestManager().SetTimeFactor  ( asnMSg.time_factor   );
    
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgCtrlBeginTick
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgCtrlBeginTick( const ASN1T_MsgCtrlBeginTick& asnMsg )
{
    TestManager::GetTestManager().SetTick( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgCtrlEndTick
// Created: NLD 2005-06-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgCtrlEndTick( const ASN1T_MsgCtrlEndTick& /*asnMsg*/ )
{
    TestManager::GetTestManager().GetScheduler().Run();
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
        TestManager::GetTestManager().SetTimeFactor( asnMsg.time_factor );
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
    // load tests
    TestManager::GetTestManager().GetTestSet().Load( TestManager::GetTestManager().GetScheduler() );
    // run scheduler
    TestManager::GetTestManager().GetScheduler().Run();
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
    if( !Automat::Find( asnMsg.oid_automate ) )
    {
        // automat creation
        Automat* pAutomat = new Automat( asnMsg );
        Automat::Register( *pAutomat );
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
    if( !Pawn::Find( asnMsg.oid_pion ) )
    {
        Pawn* pPawn = new Pawn( asnMsg );
        Pawn::Register( *pPawn );
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
    Pawn* pPawn = Pawn::Find( asnMsg.oid_pion );
    assert( pPawn );

    Automat* pAutomat = Automat::Find( asnMsg.oid_automate );
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
        Pawn* pPawn = Pawn::Find( asnMsg.oid_pion );
        assert( pPawn );

        Automat* pAutomat = Automat::Find( asnMsg.oid_automate );
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
    Pawn* pPawn = Pawn::Find( asnMsg.oid_pion );
    assert( pPawn );
    pPawn->OnAttributeUpdated( asnMsg );
}

//-----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgUnitDotations
// Created: SBO 2005-05-16
//-----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgUnitDotations( const ASN1T_MsgUnitDotations& asnMsg )
{
    Pawn* pPawn = Pawn::Find( asnMsg.oid_pion );
    assert( pPawn );
    pPawn->OnAttributeUpdated( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgChangeTeamRelation
// Created: SBO 2005-05-16
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgChangeTeamRelation( const ASN1T_MsgChangeDiplomatie& asnMsg )
{
    Team* pTeam1 = Team::Find( asnMsg.oid_camp1 );
    assert( pTeam1 );

    Team* pTeam2 = Team::Find( asnMsg.oid_camp2 );
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
        Team* pTeam1 = Team::Find( asnMsg.oid_camp1 );
        assert( pTeam1 );

        Team* pTeam2 = Team::Find( asnMsg.oid_camp2 );
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
        Automat* pAutomat = Automat::Find( asnMsg.oid_automate );
        assert( pAutomat );

        KnowledgeGroup* pKnowledgeGroup = KnowledgeGroup::Find( asnMsg.oid_groupe_connaissance );
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
    Pawn* pPawn = Pawn::Find( asnMsg.oid_pion );
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
    Pawn* pPawn = Pawn::Find( nPawnId );
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
    if( !Object::Find( asnMsg.oid ) )
    {
        Object* pObject = new Object( asnMsg );
        Object::Register( *pObject );
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
    Object* pObject = Object::Find( asnMsg );
    
    if( pObject )
    {
        Object::Unregister( *pObject );
        delete pObject;
    }
    else
        MT_LOG_ERROR_MSG( "TEST -> MM - Deleting object with ID#" << asnMsg << " which does not exist" );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgUnitKnowledgeCreation
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgUnitKnowledgeCreation( const ASN1T_MsgUnitKnowledgeCreation& asnMsg )
{
    KnowledgeGroup* pKGroup = KnowledgeGroup::Find( asnMsg.oid_groupe_possesseur );
    assert( pKGroup );
    pKGroup->OnReceiveMsgUnitKnowledgeCreation( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgUnitKnowledgeUpdate
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgUnitKnowledgeUpdate( const ASN1T_MsgUnitKnowledgeUpdate& asnMsg )
{
    KnowledgeGroup* pKGroup = KnowledgeGroup::Find( asnMsg.oid_groupe_possesseur );
    assert( pKGroup );
    pKGroup->OnReceiveMsgUnitKnowledgeUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgUnitKnowledgeDestruction
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgUnitKnowledgeDestruction( const ASN1T_MsgUnitKnowledgeDestruction& asnMsg )
{
    KnowledgeGroup* pKGroup = KnowledgeGroup::Find( asnMsg.oid_groupe_possesseur );
    assert( pKGroup );
    pKGroup->OnReceiveMsgUnitKnowledgeDestruction( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgObjectKnowledgeCreation
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgObjectKnowledgeCreation( const ASN1T_MsgObjectKnowledgeCreation& asnMsg )
{
    Team* pTeam = Team::Find( asnMsg.oid_camp_possesseur );
    assert( pTeam );
    pTeam->OnReceiveMsgObjectKnowledgeCreation( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgObjectKnowledgeUpdate
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgObjectKnowledgeUpdate( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{
    Team* pTeam = Team::Find( asnMsg.oid_camp_possesseur );
    assert( pTeam );
    pTeam->OnReceiveMsgObjectKnowledgeUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgObjectKnowledgeDestruction
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgObjectKnowledgeDestruction( const ASN1T_MsgObjectKnowledgeDestruction& asnMsg )
{
    Team* pTeam = Team::Find( asnMsg.oid_camp_possesseur );
    assert( pTeam );
    pTeam->OnReceiveMsgObjectKnowledgeDestruction( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPionOrderAck
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPionOrderAck( const ASN1T_MsgPionOrderAck& asnMsg, T_NetContextId /*nCtx*/ )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "Agent [" << asnMsg.oid_unite_executante << "] "
                 << "PionOrderAck - Code: " << asnMsg.error_code;
    MT_LOG_ERROR_MSG( strOutputMsg.str().c_str() );
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
    TacticalLine_Limit* pLimit = new TacticalLine_Limit( asnMsg );
    assert( pLimit );
    TacticalLineManager::Register( *pLimit );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgLimitDestruction
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgLimitDestruction( const ASN1T_MsgLimitDestruction& asnMsg )
{
    TacticalLine_ABC* pLimit = TacticalLineManager::Find( asnMsg );
    if( pLimit )
        TacticalLineManager::UnRegister( *pLimit );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgLimaCreation
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgLimaCreation( const ASN1T_MsgLimaCreation& asnMsg )
{
    TacticalLine_Lima* pLima = new TacticalLine_Lima( asnMsg );
    assert( pLima );
    TacticalLineManager::Register( *pLima );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgLimaDestruction
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgLimaDestruction( const ASN1T_MsgLimaDestruction& asnMsg )
{
    TacticalLine_ABC* pLima = TacticalLineManager::Find( asnMsg );
    if( pLima )
        TacticalLineManager::UnRegister( *pLima );
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
