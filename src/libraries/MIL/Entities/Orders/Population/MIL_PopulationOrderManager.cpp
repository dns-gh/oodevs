//*****************************************************************************
//
// $Created: NLD 2003-04-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Population/MIL_PopulationOrderManager.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:33 $
// $Revision: 6 $
// $Workfile: MIL_PopulationOrderManager.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_PopulationOrderManager.h"

#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationType.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Entities/Orders/Population/MIL_PopulationMission_ABC.h"
#include "Entities/Orders/Population/MIL_PopulationMissionType.h"
#include "Entities/Orders/Conduite/MIL_OrderConduiteRequest_ABC.h"
#include "Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "Entities/Orders/Conduite/MIL_OrderConduite_ABC.h"
#include "Decision/DEC_ModelPopulation.h"
#include "Network/NET_ASN_Messages.h"

//-----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager constructor
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
MIL_PopulationOrderManager::MIL_PopulationOrderManager( MIL_Population& population )
    : population_                ( population )
    , pMission_                  ( 0 )
    , bRCMissionFinishedReceived_( false )
    , bNewMissionStarted_        ( false )
{
    
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager destructor
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
MIL_PopulationOrderManager::~MIL_PopulationOrderManager()
{
    StopAllOrders();
}

// =============================================================================
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::Update
// Created: NLD 2003-09-02
// -----------------------------------------------------------------------------
void MIL_PopulationOrderManager::Update()
{
    bNewMissionStarted_ = false;
    if( bRCMissionFinishedReceived_ )
    {
        StopAllOrders();      
        bRCMissionFinishedReceived_ = false;
    }
}

// =============================================================================
// TOOLS
// =============================================================================

//$$$NLDJVT$$ FACTORISER

//-----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::CancelAllOrders
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
void MIL_PopulationOrderManager::CancelAllOrders()
{
    if( pMission_ )
    {
        pMission_->Stop();
        SendMsgOrderManagement( pMission_->GetOrderID(), EnumOrderState::cancelled );
        pMission_->Terminate();
        delete pMission_;
        pMission_ = 0;
    }
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::StopAllOrders
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
void MIL_PopulationOrderManager::StopAllOrders()
{
    if( pMission_ )
    {
        pMission_->Stop();
        SendMsgOrderManagement( pMission_->GetOrderID(), EnumOrderState::cancelled );
        pMission_->Terminate();
        delete pMission_;
        pMission_ = 0;
    }
}

//=============================================================================
// EVENTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::OnReceiveMsgPopulationOrder
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
void MIL_PopulationOrderManager::OnReceiveMsgPopulationOrder( const ASN1T_MsgPopulationOrder& asnMsg, MIL_MOSContextID nContext )
{
    // Instanciate and check the new mission
    const MIL_PopulationMissionType* pMissionType = MIL_PopulationMissionType::FindPopulationMissionType( asnMsg.mission );
    if( !( pMissionType && population_.GetType().GetModel().IsMissionAvailable( *pMissionType ) ) )
    {
        SendMsgPopulationOrderAck( asnMsg, EnumOrderErrorCode::error_invalid_mission, nContext );
        return;
    }
    
    MIL_PopulationMission_ABC& mission = pMissionType->InstanciateMission( population_ );

    ASN1T_EnumOrderErrorCode nCode = mission.Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
    {
        mission.Terminate();
        delete &mission;
        SendMsgPopulationOrderAck( asnMsg, nCode, nContext );
        return;
    }
    
    // The mission is valid : cancel all the previous orders
    CancelAllOrders();

    SendMsgPopulationOrderAck( asnMsg, EnumOrderErrorCode::no_error, nContext );

    // Start the new mission
    assert( !pMission_ );
    pMission_ = &mission;
    pMission_->Start();
    SendMsgOrderManagement( pMission_->GetOrderID(), EnumOrderState::started );
    bNewMissionStarted_ = true;
}

// =============================================================================
// CONDUITE
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::OnReceiveMsgOrderConduite
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void MIL_PopulationOrderManager::OnReceiveMsgOrderConduite( const ASN1T_MsgOrderConduite& asnMsg, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgOrderConduiteAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg().unit_id  = asnMsg.unit_id;
    asnReplyMsg.GetAsnMsg().order_id = asnMsg.order_id;

    // Create the order conduite
    const MIL_OrderConduiteType* pOrderConduiteType = MIL_OrderConduiteType::FindOrderConduiteType( asnMsg.order_conduite );
    if( !pOrderConduiteType )
    {
        asnReplyMsg.GetAsnMsg().error_code = EnumOrderErrorCode::error_invalid_order_conduite;
        asnReplyMsg.Send( nCtx );
        return;
    }
    MIL_OrderConduite_ABC& orderConduite = pOrderConduiteType->InstanciateOrderConduite();
    ASN1T_EnumOrderErrorCode nCode = orderConduite.Initialize( asnMsg.order_conduite );
    if( nCode != EnumOrderErrorCode::no_error )
    {
        asnReplyMsg.GetAsnMsg().error_code = nCode;
        asnReplyMsg.Send( nCtx );
        delete &orderConduite;
        return;
    }

    if( !LaunchOrderConduite( orderConduite ) )
    {
        asnReplyMsg.GetAsnMsg().error_code = EnumOrderErrorCode::error_invalid_order_conduite;
        asnReplyMsg.Send( nCtx );
        delete &orderConduite;
        return;
    }

    asnReplyMsg.GetAsnMsg().error_code = EnumOrderErrorCode::no_error;
    asnReplyMsg.Send( nCtx );
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::OnReceiveOrderConduite
// Created: NLD 2003-04-15
//-----------------------------------------------------------------------------
void MIL_PopulationOrderManager::OnReceiveOrderConduite( DIA_Parameters& diaParams )
{
    const MIL_OrderConduiteType* pOrderConduiteType = MIL_OrderConduiteType::FindOrderConduiteType( diaParams[1].ToId() ); // param 0 is the current population
    assert( pOrderConduiteType );
    MIL_OrderConduite_ABC& orderConduite = pOrderConduiteType->InstanciateOrderConduite();
    orderConduite.Initialize( diaParams, 2 );
    if( !LaunchOrderConduite( orderConduite ) )
    {
        MT_LOG_ERROR( "Invalid order conduite", 2, "MIL_OrderManager_ABC::OnReceivePopulationOrderConduite" );
        delete &orderConduite;
        return;
    }
}

//-----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::LaunchOrderConduite
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
bool MIL_PopulationOrderManager::LaunchOrderConduite( MIL_OrderConduite_ABC& orderConduite )
{
    DEC_PopulationDecision& decision = population_.GetDecision();

    const DEC_ModelPopulation& model = population_.GetType().GetModel();
    if(     orderConduite.GetType().IsAvailableWithoutMission() || ( pMission_ && orderConduite.GetType().IsAvailableForAllMissions() ) 
        ||  ( pMission_ && model.IsOrderConduiteAvailableForMission( pMission_->GetType(), orderConduite.GetType() ) ) )
    {
        orderConduite.Launch( decision, "" );
        return true;
    }
    return false;
}

//=============================================================================
// TOOLS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::SendMsgPopulationOrderAck
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
void MIL_PopulationOrderManager::SendMsgPopulationOrderAck( const ASN1T_MsgPopulationOrder& asnMsgPopulationOrder, ASN1T_EnumOrderErrorCode  nErrorCode, MIL_MOSContextID nContext )
{
    NET_ASN_MsgPopulationOrderAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg().order_id             = asnMsgPopulationOrder.order_id;
    asnReplyMsg.GetAsnMsg().oid_unite_executante = asnMsgPopulationOrder.oid_unite_executante;
    asnReplyMsg.GetAsnMsg().error_code           = nErrorCode;
    asnReplyMsg.Send( nContext );    
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::SendMsgOrderManagement
// Created: NLD 2003-09-17
// -----------------------------------------------------------------------------
void MIL_PopulationOrderManager::SendMsgOrderManagement( uint nOrderID, ASN1T_EnumOrderState nOrderState )
{
    // MOS message
    NET_ASN_MsgOrderManagement asnMsg;
    asnMsg.GetAsnMsg().order_id = nOrderID;
    asnMsg.GetAsnMsg().etat     = nOrderState;
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: MIL_PopulationOrderManager::GetCurrentOrderID
// Created: NLD 2003-12-17
// -----------------------------------------------------------------------------
uint MIL_PopulationOrderManager::GetCurrentOrderID() const
{
    if( pMission_ )
        return pMission_->GetOrderID();
    return (uint)-1;
}

