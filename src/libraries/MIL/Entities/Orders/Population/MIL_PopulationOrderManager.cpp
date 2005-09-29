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
//#include "Entities/Agents/MIL_AgentPion.h"
//#include "Entities/Orders/Pion/MIL_PionMission_ABC.h"
//#include "Entities/Orders/Pion/MIL_PionMissionType.h"
#include "Entities/Orders/Population/MIL_PopulationMission_ABC.h"
#include "Entities/Orders/Population/MIL_PopulationMissionType.h"
#include "Entities/Orders/Conduite/MIL_OrderConduiteRequest_ABC.h"
#include "Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "Entities/Orders/Conduite/MIL_OrderConduite_ABC.h"
#include "Decision/DEC_ModelPopulation.h"
//#include "Decision/DEC_ModelPion.h"
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

