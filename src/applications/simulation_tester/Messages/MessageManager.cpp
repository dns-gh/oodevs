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

#include "simulation_tester_pch.h"
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
    , messageService_ ( *this, networkManager.GetDINEngine(), DIN_ConnectorGuest() )
{
    messageService_.RegisterReceivedMessage( eMsgInit            , *this, & MessageManager::OnReceiveMsgInit                  );
    messageService_.RegisterReceivedMessage( eMsgKnowledgeGroup  , *this, & MessageManager::OnReceiveMsgKnowledgeGroup        );
    messageService_.RegisterReceivedMessage( eMsgArmy            , *this, & MessageManager::OnReceiveMsgTeam                  );
    messageService_.RegisterReceivedMessage( eMsgEnvironmentType , *this, & MessageManager::OnReceiveMsgPawnTerrainType       );
    messageService_.RegisterReceivedMessage( eMsgDebugDrawPoints , *this, & MessageManager::OnReceiveMsgDebugDrawPoints       );
    messageService_.RegisterReceivedMessage( eMsgOutSim          , *this, & MessageManager::OnReceiveMsgOutSim                );

    messageService_.SetCbkOnError( & MessageManager::OnError );
}


//-----------------------------------------------------------------------------
// Name: MessageManager destructor
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
MessageManager::~MessageManager()
{
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
    messageService_.Enable( link );
}


//-----------------------------------------------------------------------------
// Name: MessageManager::Disable
// Created: SBO 2005-05-09
//-----------------------------------------------------------------------------
void MessageManager::Disable( DIN_Link& link )
{
    messageService_.Disable( link );
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
    return DIN::DIN_BufferedMessage( messageService_ );
}

//=============================================================================
// ASN
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MessageManager::SendMsgInSim
// Created: NLD 2003-02-24
//-----------------------------------------------------------------------------
void MessageManager::SendMsgInSim( MsgsClientToSim& asnMsg )
{
    if( !workspace_.GetNetworkManager().IsConnected() )
        return;


    asnMsg.SerializeToString( aASNEncodeBuffer_ );

    ASN1PEREncodeBuffer asnPEREncodeBuffer( aASNEncodeBuffer_, sizeof(aASNEncodeBuffer_), TRUE );
    ASN1C_MsgsInSim asnMsgCtrl( asnPEREncodeBuffer, asnMsg );

    if( asnMsgCtrl.Encode() != ASN_OK )
    {
        asnPEREncodeBuffer.PrintErrorInfo();
        assert( false ); //$$$ TMP
        return;
    }

    DIN_BufferedMessage dinMsg = BuildMessage();
    dinMsg.GetOutput().Append( asnPEREncodeBuffer.GetMsgPtr(), asnPEREncodeBuffer.GetMsgLen() );

    messageService_.Send( workspace_.GetNetworkManager().GetLink(), eMsgInSim, dinMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::SendMsgDebugDrawPoints
// Created: SBO 2005-06-17
// -----------------------------------------------------------------------------
void MessageManager::SendMsgDebugDrawPoints( DIN::DIN_BufferedMessage& msg )
{
    messageService_.Send( workspace_.GetNetworkManager().GetLink(), eMsgDebugDrawPoints, msg  );
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
    uint32 nTeamId;
    uint32 nGroupId;
    input >> nTeamId >> nGroupId;

    Team* team = workspace_.GetEntityManager().FindTeam( nTeamId );
    if( team )
    {
        if( !workspace_.GetEntityManager().FindKnowledgeGroup( nGroupId ) )
        {
            KnowledgeGroup& knowledgeGroup = *new KnowledgeGroup( workspace_.GetEntityManager(), *team, nGroupId );
            workspace_.GetEntityManager().Register( knowledgeGroup );
            //MT_LOG_INFO_MSG( "TEST -> MM - Knowledge Group created ID#" << nId );
        }
        else
            MT_LOG_ERROR_MSG( "TEST -> MM - KnowledgeGroup with ID#" << nGroupId << " already created" );
    }
    else
        MT_LOG_ERROR_MSG( "TEST -> MM - Team with ID#" << nTeamId << " does not exists" );
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
void MessageManager::OnReceiveMsgOutSim( DIN_Link& /*linkFrom*/, DIN_Input& input )
{
    unsigned int nAsnMsgSize = input.GetAvailable();

    assert( nAsnMsgSize <= sizeof(aASNDecodeBuffer_) );

    // Fill the asn buffer array
    memcpy( aASNDecodeBuffer_, input.GetBuffer(nAsnMsgSize), nAsnMsgSize );

    // Create the asn msg buffer
//    ASN1PERDecodeBuffer asnPERDecodeBuffer( aASNDecodeBuffer_, nAsnMsgSize, TRUE );
    ::google::protobuf::io::CodedInputStream* inputStream = new ::google::protobuf::io::CodedInputStream(aASNDecodeBuffer_, nAsnMsgSize);
    MsgSimToClient asnMsg;
    // Decode the message
    asnMsg.MergePartialFromCodedStream(inputStream);

    // Decode the message
//    MsgsOutSim asnMsg;
//    ASN1C_MsgsOutSim asnMsgCtrl( asnPERDecodeBuffer, asnMsg );
//    if( asnMsgCtrl.Decode() != ASN_OK )
//    {
//        asnPERDecodeBuffer.PrintErrorInfo();
//        assert( false ); //$$$ TMP
//    }

    unsigned int nCtx = asnMsg.context();

    if (asnMsg.msg().has_msg_change_automate_ack() )
        OnReceiveMsgChangeAutomateAck      ( *asnMsg.msg().msg_change_automate_ack()           , nCtx );
    if (asnMsg.msg().has_msg_change_diplomatie_ack() )
        OnReceiveMsgChangeTeamRelationAck  ( *asnMsg.msg().msg_change_diplomatie_ack()         , nCtx );
    if (asnMsg.msg().has_msg_change_groupe_connaissance_ack() )
        OnReceiveMsgChangeKnowledgeGroupAck( *asnMsg.msg().msg_change_groupe_connaissance_ack,() nCtx );
    if (asnMsg.msg().has_msg_automate_order_ack() )
        OnReceiveMsgAutomatOrderAck        ( *asnMsg.msg().msg_automate_order_ack()            , nCtx );
    if (asnMsg.msg().has_msg_pion_order_ack() )
        OnReceiveMsgUnitOrderAck           ( *asnMsg.msg().msg_pion_order_ack()                , nCtx );
    if (asnMsg.msg().has_msg_set_automate_mode_ack() )
        OnReceiveMsgSetAutomatModeAck      ( *asnMsg.msg().msg_set_automate_mode_ack()         , nCtx );

    if (asnMsg.msg().has_msg_ctrl_info() )
        OnReceiveMsgControlInformation           ( *asnMsg.msg().msg_ctrl_info()                    );
    if (asnMsg.msg().has_msg_ctrl_begin_tick() )
        OnReceiveMsgControlBeginTick             (  asnMsg.msg().msg_ctrl_begin_tick()              );
    if (asnMsg.msg().has_msg_ctrl_end_tick() )
        OnReceiveMsgControlEndTick               ( *asnMsg.msg().msg_ctrl_end_tick()                );
    if (asnMsg.msg().has_msg_ctrl_change_time_factor_ack() )
        OnReceiveMsgControlChangeTimeFactorAck   ( *asnMsg.msg().msg_ctrl_change_time_factor_ack()  );
    if (asnMsg.msg().has_msg_ctrl_send_current_state_begin() )
        OnReceiveMsgControlSendCurrentStateBegin ();
    if (asnMsg.msg().has_msg_ctrl_send_current_state_end() )
        OnReceiveMsgControlSendCurrentStateEnd   ();

    if (asnMsg.msg().has_msg_unit_attributes() )
        OnReceiveMsgUnitAttributes            ( *asnMsg.msg().msg_unit_attributes()              );
    if (asnMsg.msg().has_msg_unit_pathfind() )
        OnReceiveMsgPawnPathFind              ( *asnMsg.msg().msg_unit_pathfind()                );

    if (asnMsg.msg().has_msg_change_automate() )
        OnReceiveMsgChangeAutomat             ( *asnMsg.msg().msg_change_automate()              );
    if (asnMsg.msg().has_msg_pion_creation() )
        OnReceiveMsgPawnCreation              ( *asnMsg.msg().msg_pion_creation()                );
    if (asnMsg.msg().has_msg_automate_creation() )
        OnReceiveMsgAutomatCreation           ( *asnMsg.msg().msg_automate_creation()            );
    if (asnMsg.msg().has_msg_change_diplomatie() )
        OnReceiveMsgChangeTeamRelation        ( *asnMsg.msg().msg_change_diplomatie()            );

    if (asnMsg.msg().has_msg_unit_knowledge_creation() )
        OnReceiveMsgUnitKnowledgeCreation     ( *asnMsg.msg().msg_unit_knowledge_creation()      );
    if (asnMsg.msg().has_msg_unit_knowledge_update() )
        OnReceiveMsgUnitKnowledgeUpdate       ( *asnMsg.msg().msg_unit_knowledge_update()        );
    if (asnMsg.msg().has_msg_unit_knowledge_destruction() )
        OnReceiveMsgUnitKnowledgeDestruction  ( *asnMsg.msg().msg_unit_knowledge_destruction()   );

    if (asnMsg.msg().has_msg_population_knowledge_creation() )
        OnReceiveMsgPopulationKnowledgeCreation     ( *asnMsg.msg().msg_population_knowledge_creation()      );
    if (asnMsg.msg().has_msg_population_knowledge_update() )
        OnReceiveMsgPopulationKnowledgeUpdate       ( *asnMsg.msg().msg_population_knowledge_update()        );
    if (asnMsg.msg().has_msg_population_knowledge_destruction() )
        OnReceiveMsgPopulationKnowledgeDestruction  ( *asnMsg.msg().msg_population_knowledge_destruction()   );

    if (asnMsg.msg().has_msg_object_creation() )
        OnReceiveMsgObjectCreation            ( *asnMsg.msg().msg_object_creation()              );
    if (asnMsg.msg().has_msg_object_update() )
        OnReceiveMsgObjectUpdate              ( *asnMsg.msg().msg_object_update()                );
    if (asnMsg.msg().has_msg_object_destruction() )
        OnReceiveMsgObjectDestruction         ( asnMsg.msg().msg_object_destruction()            );
    if (asnMsg.msg().has_msg_object_knowledge_creation() )
        OnReceiveMsgObjectKnowledgeCreation   ( *asnMsg.msg().msg_object_knowledge_creation()    );
    if (asnMsg.msg().has_msg_object_knowledge_update() )
        OnReceiveMsgObjectKnowledgeUpdate     ( *asnMsg.msg().msg_object_knowledge_update()      );
    if (asnMsg.msg().has_msg_object_knowledge_destruction() )
        OnReceiveMsgObjectKnowledgeDestruction( *asnMsg.msg().msg_object_knowledge_destruction() );

    if (asnMsg.msg().has_msg_population_creation() )
        OnReceiveMsgPopulationCreation        ( *asnMsg.msg().msg_population_creation()          );
    if (asnMsg.msg().has_msg_population_update() )
        OnReceiveMsgPopulationUpdate          ( *asnMsg.msg().msg_population_update()            );
}

//-----------------------------------------------------------------------------
// TIME MANAGEMENT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgControlInformation
// Created: SBO 2005-05-17
//-----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgControlInformation( const MsgControlInformation& asnMsg )
{
    std::stringstream strOutputMsg;
    strOutputMsg << "ControlInformation - Current Tick: " << asnMsg.current_tick()
                 << " - Tick duration : "       << asnMsg.tick_duration
                 << " - Time factor : "         << asnMsg.time_factor;

    workspace_.SetTickDuration( asnMsg.tick_duration() );
    workspace_.SetTick        ( asnMsg.current_tick()  );
    //workspace_.SetTimeFactor  ( asnMsg.time_factor()   );

    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgControlBeginTick
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgControlBeginTick( const MsgControlBeginTick& asnMsg )
{
    workspace_.SetTick( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgControlEndTick
// Created: NLD 2005-06-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgControlEndTick( const MsgControlEndTick& /*asnMsg*/ )
{
    MT_LOG_INFO_MSG( "Tick: " << workspace_.GetTick() << " (next test at " << workspace_.GetScheduler().GetNextExecutionTick() << ")" );
    // save recovery point
    workspace_.SaveRecoveryPoint();
    // run required actions
    workspace_.GetScheduler().Run( workspace_.GetTick() );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgControlChangeTimeFactorAck
// Created: SBO 2005-06-03
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgControlChangeTimeFactorAck( const MsgControlChangeTimeFactorAck& /*asnMsg*/ )
{
/*
    // don't do anything if original command was malformed
    if( asnMsg.error_code() == EnumControlErrorCode::no_error )
    {
        workspace_.SetTimeFactor( asnMsg.time_factor() );
    }
*/
}


// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgControlSendCurrentStateBegin
// Created: SBO 2005-06-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgControlSendCurrentStateBegin()
{
    MT_LOG_INFO_MSG( "Begin current state..." );
}


// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgControlSendCurrentStateEnd
// Created: SBO 2005-06-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgControlSendCurrentStateEnd()
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
// Name: MessageManager::OnReceiveMsgAutomatCreation
// Created: SBO 2005-05-11
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgAutomatCreation( const MsgAutomatCreation& asnMsg )
{
    if( !workspace_.GetEntityManager().FindAutomat( asnMsg.oid_automate() ) )
    {
        // automat creation
        Automat& automat = *new Automat( workspace_, asnMsg );
        workspace_.GetEntityManager().Register( automat );
    }
    else
        MT_LOG_ERROR_MSG( "TEST -> MM - Automat with ID#" << asnMsg.oid_automate() << " already created" );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgUnitCreation
// Created: SBO 2005-05-11
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPawnCreation( const MsgUnitCreation& asnMsg )
{
    if( !workspace_.GetEntityManager().FindPawn( asnMsg.oid_pion() ) )
    {
        Pawn& pawn = *new Pawn( workspace_, asnMsg );
        workspace_.GetEntityManager().Register( pawn );
    }
    else
        MT_LOG_ERROR_MSG( "TEST -> MM - Pawn with ID#" << asnMsg.oid_pion() << " already created" );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgChangeAutomat
// Created: SBO 2005-05-16
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgChangeAutomat( const MsgChangeAutomate& asnMsg )
{
    Pawn* pPawn = workspace_.GetEntityManager().FindPawn( asnMsg.oid_pion() );
    assert( pPawn );

    Automat* pAutomat = workspace_.GetEntityManager().FindAutomat( asnMsg.oid_automate() );
    assert( pAutomat );

    pPawn->OnAutomatChanged( *pAutomat );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgChangeAutomateAck
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgChangeAutomateAck( const MsgChangeAutomateAck& asnMsg, unsigned int /*nCtx*/ )
{
    // don't do anything if original command was malformed
    if( asnMsg.error_code() == EnumOrderErrorCode::no_error )
    {
        Pawn* pPawn = workspace_.GetEntityManager().FindPawn( asnMsg.oid_pion() );
        assert( pPawn );

        Automat* pAutomat = workspace_.GetEntityManager().FindAutomat( asnMsg.oid_automate() );
        assert( pAutomat );

        pPawn->OnAutomatChanged( *pAutomat );
    }
}

//-----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgUnitAttributes
// Created: SBO 2005-05-16
//-----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgUnitAttributes( const MsgUnitAttributes& asnMsg )
{
    Pawn* pPawn = workspace_.GetEntityManager().FindPawn( asnMsg.oid_pion() );
    assert( pPawn );
    pPawn->OnAttributeUpdated( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgChangeTeamRelation
// Created: SBO 2005-05-16
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgChangeTeamRelation( const MsgChangeDiplomacy& asnMsg )
{
    Team* pTeam1 = workspace_.GetEntityManager().FindTeam( asnMsg.oid_camp1() );
    assert( pTeam1 );

    Team* pTeam2 = workspace_.GetEntityManager().FindTeam( asnMsg.oid_camp2() );
    assert( pTeam2 );

    pTeam1->SetRelation( *pTeam2, asnMsg.diplomatie() );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgChangeTeamRelation
// Created: SBO 2005-05-17
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgChangeTeamRelationAck( const MsgChangeDiplomacyAck& asnMsg, unsigned int /*nCtx*/ )
{
    // don't do anything if original command was malformed
    if( asnMsg.error_code() == EnumChangeDiplomacyErrorCode::no_error )
    {
        Team* pTeam1 = workspace_.GetEntityManager().FindTeam( asnMsg.oid_camp1() );
        assert( pTeam1 );

        Team* pTeam2 = workspace_.GetEntityManager().FindTeam( asnMsg.oid_camp2() );
        assert( pTeam2 );

        pTeam1->SetRelation( *pTeam2, asnMsg.diplomatie() );
    }
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgChangeKnowledgeGroupAck
// Created: SBO 2005-05-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgChangeKnowledgeGroupAck( const MsgChangeGroupeConnaissanceAck& asnMsg, unsigned int /*nCtx*/ )
{
    if( asnMsg.error_code() == EnumChangeGroupeConnaissanceErrorCode::no_error )
    {
        Automat* pAutomat = workspace_.GetEntityManager().FindAutomat( asnMsg.oid_automate() );
        assert( pAutomat );

        KnowledgeGroup* pKnowledgeGroup = workspace_.GetEntityManager().FindKnowledgeGroup( asnMsg.oid_groupe_connaissance() );
        assert( pKnowledgeGroup );

        pAutomat->OnKnowledgeGroupChanged( *pKnowledgeGroup );
    }
}

//-----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPawnTerrainType
// Created: SBO 2005-06-15
//-----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPawnPathFind( const MsgUnitPathFind& asnMsg )
{
    Pawn* pPawn = workspace_.GetEntityManager().FindPawn( asnMsg.oid_pion() );
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
void MessageManager::OnReceiveMsgObjectCreation( const MsgObjectCreation& asnMsg )
{
    if( !workspace_.GetEntityManager().FindObject( asnMsg.oid() ) )
    {
        Object& object = *new Object( workspace_, asnMsg );
        workspace_.GetEntityManager().Register( object );
    }
    else
        MT_LOG_ERROR_MSG( "TEST -> MM - Object with ID#" << asnMsg.oid() << " already created" );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgObjectUpdate
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgObjectUpdate( const MsgObjectUpdate& /*asnMsg*/ )
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgObjectDestruction
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgObjectDestruction( const MsgObjectDestruction& asnMsg )
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
void MessageManager::OnReceiveMsgPopulationCreation( const MsgPopulationCreation& asnMsg )
{
    if( !workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population() ) )
    {
        Population& population = *new Population( workspace_, asnMsg );
        workspace_.GetEntityManager().Register( population );
    }
    else
        MT_LOG_ERROR_MSG( "TEST -> MM - Population with ID#" << asnMsg.oid_population() << " already created" );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationUpdate
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationUpdate( const MsgPopulationUpdate& asnMsg )
{
    Population* pPopulation = workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population() );
    assert( pPopulation );
    pPopulation->OnUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationConcentrationCreation
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationConcentrationCreation( const MsgPopulationConcentrationCreation& asnMsg )
{
    Population* pPopulation = workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population() );
    assert( pPopulation );
    pPopulation->OnConcentrationCreated( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationConcentrationUpdate
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationConcentrationUpdate( const MsgPopulationConcentrationUpdate& asnMsg )
{
    Population* pPopulation = workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population() );
    assert( pPopulation );
    pPopulation->OnConcentrationUpdated( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationConcentrationDestruction
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationConcentrationDestruction( const MsgPopulationConcentrationDestruction& asnMsg )
{
    Population* pPopulation = workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population() );
    assert( pPopulation );
    pPopulation->OnConcentrationDestroyed( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationFlowCreation
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationFlowCreation( const MsgPopulationFlowCreation& asnMsg )
{
    Population* pPopulation = workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population() );
    assert( pPopulation );
    pPopulation->OnFlowCreated( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationFlowUpdate
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationFlowUpdate( const MsgPopulationFlowUpdate& asnMsg )
{
    Population* pPopulation = workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population() );
    assert( pPopulation );
    pPopulation->OnFlowUpdated( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationFlowDestruction
// Created: SBO 2005-11-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationFlowDestruction( const MsgPopulationFlowDestruction& asnMsg )
{
    Population* pPopulation = workspace_.GetEntityManager().FindPopulation( asnMsg.oid_population() );
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
void MessageManager::OnReceiveMsgUnitKnowledgeCreation( const MsgUnitKnowledgeCreation& asnMsg )
{
    KnowledgeGroup* pKGroup = workspace_.GetEntityManager().FindKnowledgeGroup( asnMsg.oid_groupe_possesseur() );
    assert( pKGroup );
    pKGroup->OnReceiveMsgUnitKnowledgeCreation( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgUnitKnowledgeUpdate
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgUnitKnowledgeUpdate( const MsgUnitKnowledgeUpdate& asnMsg )
{
    KnowledgeGroup* pKGroup = workspace_.GetEntityManager().FindKnowledgeGroup( asnMsg.oid_groupe_possesseur() );
    assert( pKGroup );
    pKGroup->OnReceiveMsgUnitKnowledgeUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgUnitKnowledgeDestruction
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgUnitKnowledgeDestruction( const MsgUnitKnowledgeDestruction& asnMsg )
{
    KnowledgeGroup* pKGroup = workspace_.GetEntityManager().FindKnowledgeGroup( asnMsg.oid_groupe_possesseur() );
    assert( pKGroup );
    pKGroup->OnReceiveMsgUnitKnowledgeDestruction( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationKnowledgeCreation
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationKnowledgeCreation( const MsgPopulationKnowledgeCreation& asnMsg )
{
    KnowledgeGroup* pKGroup = workspace_.GetEntityManager().FindKnowledgeGroup( asnMsg.oid_groupe_possesseur() );
    assert( pKGroup );
    pKGroup->OnReceiveMsgPopulationKnowledgeCreation( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationKnowledgeUpdate
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationKnowledgeUpdate( const MsgPopulationKnowledgeUpdate& asnMsg )
{
    KnowledgeGroup* pKGroup = workspace_.GetEntityManager().FindKnowledgeGroup( asnMsg.oid_groupe_possesseur() );
    assert( pKGroup );
    pKGroup->OnReceiveMsgPopulationKnowledgeUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgPopulationKnowledgeDestruction
// Created: SBO 2006-01-03
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgPopulationKnowledgeDestruction( const MsgPopulationKnowledgeDestruction& asnMsg )
{
    KnowledgeGroup* pKGroup = workspace_.GetEntityManager().FindKnowledgeGroup( asnMsg.oid_groupe_possesseur() );
    assert( pKGroup );
    pKGroup->OnReceiveMsgPopulationKnowledgeDestruction( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgObjectKnowledgeCreation
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgObjectKnowledgeCreation( const MsgObjectKnowledgeCreation& asnMsg )
{
    Team* pTeam = workspace_.GetEntityManager().FindTeam( asnMsg.oid_camp_possesseur() );
    assert( pTeam );
    pTeam->OnReceiveMsgObjectKnowledgeCreation( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgObjectKnowledgeUpdate
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgObjectKnowledgeUpdate( const MsgObjectKnowledgeUpdate& asnMsg )
{
    Team* pTeam = workspace_.GetEntityManager().FindTeam( asnMsg.oid_camp_possesseur() );
    assert( pTeam );
    pTeam->OnReceiveMsgObjectKnowledgeUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgObjectKnowledgeDestruction
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgObjectKnowledgeDestruction( const MsgObjectKnowledgeDestruction& asnMsg )
{
    Team* pTeam = workspace_.GetEntityManager().FindTeam( asnMsg.oid_camp_possesseur() );
    assert( pTeam );
    pTeam->OnReceiveMsgObjectKnowledgeDestruction( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgUnitOrderAck
// Created: SBO 2005-08-08
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgUnitOrderAck( const MsgUnitOrderAck& asnMsg, unsigned int /*nCtx*/ )
{
    Pawn* pPawn = workspace_.GetEntityManager().FindPawn( asnMsg.oid_unite_executante() );
    assert( pPawn );

    std::stringstream strOutputMsg;
    strOutputMsg << "Agent [" << asnMsg.oid_unite_executante() << "] (" << pPawn->GetType().GetName() << ") "
                 << "UnitOrderAck - " << ASN_Tools::ToString( asnMsg.error_code() );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgAutomatOrderAck
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgAutomatOrderAck( const MsgAutomatOrderAck& asnMsg, unsigned int /*nCtx*/ )
{
    Automat* pAutomat = workspace_.GetEntityManager().FindAutomat( asnMsg.oid_unite_executante() );
    assert( pAutomat );

    std::stringstream strOutputMsg;
    strOutputMsg << "Automat [" << asnMsg.oid_unite_executante() << "] (" << pAutomat->GetType().GetName() << ") "
                 << "AutomatOrderAck - " << ASN_Tools::ToString( asnMsg.error_code() );
    MT_LOG_INFO_MSG( strOutputMsg.str().c_str() );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgSetAutomatModeAck
// Created: SBO 2005-08-24
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgSetAutomatModeAck( const MsgSetAutomatModeAck& /*asnMsg*/, unsigned int /*nCtx*/ )
{
    /*
    Automat* pAutomat = workspace_.GetEntityManager().FindAutomat( asnMsg.unit_id() );
    assert( pAutomat );

    std::stringstream strOutputMsg;
    strOutputMsg << "Automat [" << asnMsg.unit_id() << "] SetMode - " << ASN_Tools::ToString( asnMsg.error_code() );
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
void MessageManager::OnReceiveMsgLimitCreation( const MsgLimitCreation& asnMsg )
{
    TacticalLine_Limit& limit = *new TacticalLine_Limit( asnMsg );
    workspace_.GetTacticalLineManager().Register( limit );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgLimitDestruction
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgLimitDestruction( const MsgLimitDestruction& asnMsg )
{
    TacticalLine_ABC* pLimit = workspace_.GetTacticalLineManager().Find( asnMsg );
    if( pLimit )
        workspace_.GetTacticalLineManager().UnRegister( *pLimit );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgLimaCreation
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgLimaCreation( const MsgLimaCreation& asnMsg )
{
    TacticalLine_Lima& lima = *new TacticalLine_Lima( asnMsg );
    workspace_.GetTacticalLineManager().Register( lima );
}

// -----------------------------------------------------------------------------
// Name: MessageManager::OnReceiveMsgLimaDestruction
// Created: SBO 2005-08-09
// -----------------------------------------------------------------------------
void MessageManager::OnReceiveMsgLimaDestruction( const MsgLimaDestruction& asnMsg )
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
