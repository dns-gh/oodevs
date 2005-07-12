//*****************************************************************************
//
// $Created: NLD 2003-01-10 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Orders/Pion/MIL_PionOrderManager.cpp $
// $Author: Nld $
// $Modtime: 4/04/05 18:30 $
// $Revision: 10 $
// $Workfile: MIL_PionOrderManager.cpp $
//
//*****************************************************************************

#include "MIL_Pch.h"

#include "MIL_PionOrderManager.h"

#include "MIL_PionMission_ABC.h"
#include "MIL_PionMissionType.h"
#include "Entities/Orders/Conduite/MIL_OrderConduiteRequest_ABC.h"
#include "Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "Entities/Orders/Conduite/MIL_OrderConduite_ABC.h"
#include "Entities/Orders/Automate/MIL_AutomateMRT.h"

#include "Network/NET_ASN_Messages.h"
#include "Decision/DEC_ModelPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.h"

//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager constructor
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
MIL_PionOrderManager::MIL_PionOrderManager( MIL_AgentPion& pion )
    : pion_                      ( pion )
    , pMission_                  ( 0 )
    , pReplacementMission_       ( 0 )
    , bRCMissionFinishedReceived_( false )
{

}

//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager destructor
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
MIL_PionOrderManager::~MIL_PionOrderManager()
{
    StopAllOrders();
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::Update
// Created: NLD 2003-09-02
// -----------------------------------------------------------------------------
void MIL_PionOrderManager::Update( bool bIsDead )
{
    if( bIsDead )
    {
        StopAllOrders();
        return;
    }

    if( pReplacementMission_ )
    {
        MIL_PionMission_ABC* pMissionTmp = pReplacementMission_;
        pReplacementMission_ = 0;

        StopAllOrders();
        pMission_ = pMissionTmp;
        
        if( !pion_.IsPC() )//$$ HACK THALES
        {
            NET_ASN_MsgPionOrder asnMsg;
            pMission_->Serialize( asnMsg.GetAsnMsg() );
            asnMsg.Send();
            pMission_->CleanAfterSerialization( asnMsg.GetAsnMsg() );
        }

        pMission_->Start();
        SendMsgOrderManagement( pMission_->GetOrderID(), EnumOrderState::started );
    } 

    if( bRCMissionFinishedReceived_ )
    {
        StopAllOrders();
        bRCMissionFinishedReceived_ = false;
    }
}


//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::CancelAllOrders
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
void MIL_PionOrderManager::CancelAllOrders()
{
    StopWaitingForOrder();
    if( pMission_ )
    {
        pMission_->Stop();
        SendMsgOrderManagement( pMission_->GetOrderID(), EnumOrderState::cancelled );

        pMission_->Terminate();
        delete pMission_;
        pMission_ = 0;
    }

    if( pReplacementMission_ )
    {
        pReplacementMission_->Terminate();
        delete pReplacementMission_;
        pReplacementMission_ = 0;
    }
}
                   
//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::StopAllOrders
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
void MIL_PionOrderManager::StopAllOrders()
{
    StopWaitingForOrder();
    if( pMission_ )
    {
        pMission_->Stop();
        SendMsgOrderManagement( pMission_->GetOrderID(), EnumOrderState::stopped );

        pMission_->Terminate();
        delete pMission_;
        pMission_ = 0;
    }

    if( pReplacementMission_ )
    {
        pReplacementMission_->Terminate();
        delete pReplacementMission_;
        pReplacementMission_ = 0;
    }
}

//=============================================================================
// ORDRES INITIAUX
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::OnReceivePionOrder
// Created: NLD 2003-04-15
//-----------------------------------------------------------------------------
void MIL_PionOrderManager::OnReceivePionOrder( MIL_PionMission_ABC& pionMission )
{
    // Ordre de l'automate, ou ordre donné directement par la SIM (Cf. Convois)
    if( pReplacementMission_ )
    {
        pReplacementMission_->Terminate();
        delete pReplacementMission_;
        pReplacementMission_ = 0;
    }
    pReplacementMission_ = &pionMission;
}


//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::OnReceiveMsgPionOrder
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
void MIL_PionOrderManager::OnReceiveMsgPionOrder( const ASN1T_MsgPionOrder& asnMsg, MIL_MOSContextID nContext )
{
    // Check if the agent can receive this order (automate must be debraye)
    if( pion_.GetAutomate().IsEmbraye() || pion_.IsDead() )
    {
        SendMsgPionOrderAck( asnMsg, EnumOrderErrorCode::error_unit_cannot_receive_order, nContext );
        return;
    }

    if( pion_.GetRole< PHY_RolePion_Surrender >().IsSurrendered() )
    {
        SendMsgPionOrderAck( asnMsg, EnumOrderErrorCode::error_unit_surrendered, nContext );
        return;
    }

    // Instanciate and check the new mission
    const MIL_PionMissionType* pMissionType = MIL_PionMissionType::FindPionMissionType( asnMsg.mission );
    if( !( pMissionType && pion_.GetType().GetModel().IsMissionAvailable( *pMissionType ) ) )
    {
        SendMsgPionOrderAck( asnMsg, EnumOrderErrorCode::error_invalid_mission, nContext );
        return;
    }
    
    MIL_PionMission_ABC& mission = pMissionType->InstanciateMission( pion_ );

    ASN1T_EnumOrderErrorCode nCode = mission.Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
    {
        SendMsgPionOrderAck( asnMsg, nCode, nContext );
        delete &mission;
        return;
    }
    mission.Prepare();

    // The mission is valid : cancel all the previous orders
    CancelAllOrders();

    SendMsgPionOrderAck( asnMsg, EnumOrderErrorCode::no_error, nContext );

    // Start the new mission
    pMission_ = &mission;
    pMission_->Start();
    SendMsgOrderManagement( pMission_->GetOrderID(), EnumOrderState::started );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::CanRelievePion
// Created: NLD 2004-12-09
// -----------------------------------------------------------------------------
bool MIL_PionOrderManager::CanRelievePion( const MIL_AgentPion& pion ) const
{
    if( pReplacementMission_ || pion_.GetRole< PHY_RolePion_Surrender >().IsSurrendered() )
        return false;

    MIL_PionMission_ABC* pCopyMission = pion.GetOrderManager().pMission_; //$$$ CRADE
    if( !pCopyMission )
        return true;

    const MIL_PionMissionType& copyMissionType = pCopyMission->GetType();
    return pion_.GetType().GetModel().IsMissionAvailable( copyMissionType );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::RelievePion
// Created: NLD 2003-09-30
// -----------------------------------------------------------------------------
bool MIL_PionOrderManager::RelievePion( const MIL_AgentPion& pion )
{
    if( pReplacementMission_ || pion_.GetRole< PHY_RolePion_Surrender >().IsSurrendered() )
        return false;

    MIL_PionMission_ABC* pCopyMission = pion.GetOrderManager().pMission_; //$$$ CRADE
    if( !pCopyMission )
        return true;

    const MIL_PionMissionType& copyMissionType = pCopyMission->GetType();
    if( !pion_.GetType().GetModel().IsMissionAvailable( copyMissionType ) )
        return false;

    pReplacementMission_ = &copyMissionType.InstanciateMission( pion_ );
    if( !pReplacementMission_->Initialize( *pCopyMission ) )
    {
        pReplacementMission_->Terminate();
        delete pReplacementMission_;
        pReplacementMission_ = 0;
        return false;
    }
    return true;
}

// =============================================================================
// CONDUITE
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_OrderManager_ABC::SendOrderConduiteRequestsToDIA
// Created: NLD 2003-07-15
//-----------------------------------------------------------------------------
void MIL_PionOrderManager::SendOrderConduiteRequestsToDIA( DIA_Variable_ABC& diaOrderList, DIA_Variable_ABC& diaStrHint )
{
    diaOrderList.SetValue( orderConduiteRequestVector_ );
    diaStrHint  .SetValue( strWaitForSubOrderHint_     );
}

//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::OnReceiveMsgOrderConduite
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void MIL_PionOrderManager::OnReceiveMsgOrderConduite( const ASN1T_MsgOrderConduite& asnMsg, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgOrderConduiteAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg().unit_id  = asnMsg.unit_id;
    asnReplyMsg.GetAsnMsg().order_id = asnMsg.order_id;

    if( pion_.GetRole< PHY_RolePion_Surrender >().IsSurrendered() )
    {
        asnReplyMsg.GetAsnMsg().error_code = EnumOrderErrorCode::error_unit_surrendered;
        asnReplyMsg.Send( nCtx );
        return;        
    }

    // Create the order conduite
    const MIL_OrderConduiteType* pOrderConduiteType = MIL_OrderConduiteType::FindOrderConduiteType( asnMsg.order_conduite );
    if( !pOrderConduiteType )
    {
        asnReplyMsg.GetAsnMsg().error_code = EnumOrderErrorCode::error_invalid_order_conduite;
        asnReplyMsg.Send( nCtx );
        return;
    }
    MIL_OrderConduite_ABC& orderConduite = pOrderConduiteType->InstanciateOrderConduite( pion_.GetKnowledgeGroup() );
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
// Name: MIL_PionOrderManager::OnReceiveOrderConduite
// Created: NLD 2003-04-15
//-----------------------------------------------------------------------------
void MIL_PionOrderManager::OnReceiveOrderConduite( DIA_Parameters& diaParams )
{
    if( !pMission_ )
    {
        MT_LOG_ERROR( "Agent has no initial order ", 1, "MIL_OrderManager_ABC::OnReceivePionOrderConduite" );
        return;
    }

    if( pion_.GetAutomate().IsEmbraye() )
    {
        MT_LOG_ERROR( "Agent cannot receive order conduite from automate ", 2, "MIL_OrderManager_ABC::OnReceivePionOrderConduite" );
        return;
    }
    
    const MIL_OrderConduiteType* pOrderConduiteType = MIL_OrderConduiteType::FindOrderConduiteType( diaParams[1].ToId() ); // param 0 is the current pion
    assert( pOrderConduiteType );
    MIL_OrderConduite_ABC& orderConduite = pOrderConduiteType->InstanciateOrderConduite( pion_.GetKnowledgeGroup() );
    orderConduite.Initialize( diaParams, 2 );
    if( !LaunchOrderConduite( orderConduite ) )
    {
        MT_LOG_ERROR( "Invalid order conduite", 2, "MIL_OrderManager_ABC::OnReceivePionOrderConduite" );
        delete &orderConduite;
        return;
    }
}

//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::WaitForOrder
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void MIL_PionOrderManager::WaitForOrder( DIA_Parameters& diaParams )
{
    orderConduiteRequestVector_.clear();
    strWaitForSubOrderHint_ = "";

    // Parse the order query
    uint nCurParam = 0;
    while( nCurParam < diaParams.GetParameters().size() )
    {
        DIA_Variable_ABC& diaParam = diaParams[ nCurParam ++ ];
        E_VariableType nDIAType = diaParam.Type();
        if( nDIAType == eId )
        {
            const MIL_OrderConduiteType* pOrderConduiteType = MIL_OrderConduiteType::FindOrderConduiteType( diaParam.ToId() );
            assert( pOrderConduiteType );
            assert( pOrderConduiteType->CanBeRequested() );
            MIL_OrderConduiteRequest_ABC& orderConduiteReq = pOrderConduiteType->InstanciateOrderConduiteRequest();
            orderConduiteReq.Initialize( diaParams, nCurParam );
            orderConduiteRequestVector_.push_back( &orderConduiteReq );
        }
        else if( nDIAType == eString )
            strWaitForSubOrderHint_ = diaParam.ToString();
        else
            assert( false );
    }

    // If the automate is disabled, send an ASN message to the Anibas
    if( !pion_.GetAutomate().IsEmbraye() )
    {
        // Send to mos
        ASN1T_MsgAttenteOrdreConduite_ordres_conduite_element* pOrders = new ASN1T_MsgAttenteOrdreConduite_ordres_conduite_element[ orderConduiteRequestVector_.size() ]; //$$$ RAM
    
        uint i = 0; 
        for( CIT_ObjectVector itOrder = orderConduiteRequestVector_.begin(); itOrder != orderConduiteRequestVector_.end(); ++itOrder )
            ((MIL_OrderConduiteRequest_ABC&)(**itOrder)).Serialize( pOrders[i++] );
    
        NET_ASN_MsgAttenteOrdreConduite asnMsg;
        asnMsg.GetAsnMsg().unit_id              = pion_.GetID();
        asnMsg.GetAsnMsg().order_id             = GetCurrentOrderID();
        asnMsg.GetAsnMsg().ordres_conduite.n    = orderConduiteRequestVector_.size();  
        asnMsg.GetAsnMsg().ordres_conduite.elem = pOrders;
        asnMsg.Send();

        delete[] pOrders;
    }
}

//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::StopWaitingForOrder
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
void MIL_PionOrderManager::StopWaitingForOrder()
{
    if( !orderConduiteRequestVector_.empty() )
    {
        NET_ASN_MsgAnnuleAttenteOrdreConduite asnMsg;
        asnMsg.GetAsnMsg().unit_id  = pion_.GetID();
        asnMsg.GetAsnMsg().order_id = GetCurrentOrderID();
        asnMsg.Send();
    }
    for( IT_ObjectVector itOrder = orderConduiteRequestVector_.begin(); itOrder != orderConduiteRequestVector_.end() ; ++itOrder )
    {
        MIL_OrderConduiteRequest_ABC& order = static_cast< MIL_OrderConduiteRequest_ABC& >( **itOrder );
        order.Terminate();
        delete &order;
    }       
    orderConduiteRequestVector_.clear();
    strWaitForSubOrderHint_ = "";
}

//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::LaunchOrderConduite
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
bool MIL_PionOrderManager::LaunchOrderConduite( MIL_OrderConduite_ABC& orderConduite )
{
    DEC_RolePion_Decision& roleDecision = pion_.GetRole< DEC_RolePion_Decision >();

    // Ordres de conduite spécifiés
    for( IT_ObjectVector itOrderConduiteReq = orderConduiteRequestVector_.begin(); itOrderConduiteReq != orderConduiteRequestVector_.end(); ++itOrderConduiteReq )
    {   
        if( orderConduite.IsAnAnswerToRequest( (MIL_OrderConduiteRequest_ABC&)**itOrderConduiteReq ) )
        {
            orderConduite.Launch( roleDecision, strWaitForSubOrderHint_ );
            StopWaitingForOrder();
            return true;
        }
    }

    // Ordres de conduite par défaut
    const DEC_ModelPion& model = pion_.GetType().GetModel();
    if(     orderConduite.GetType().IsAvailableWithoutMission() || ( pMission_ && orderConduite.GetType().IsAvailableForAllMissions() ) 
        ||  ( pMission_ && model.IsOrderConduiteAvailableForMission( pMission_->GetType(), orderConduite.GetType() ) ) )
    {
        orderConduite.Launch( roleDecision, "" );
        StopWaitingForOrder();
        return true;
    }
    return false;
}


//=============================================================================
// TOOLS
//=============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::GetCurrentOrderID
// Created: NLD 2003-12-17
// -----------------------------------------------------------------------------
uint MIL_PionOrderManager::GetCurrentOrderID() const
{
    if( pMission_ )
        return pMission_->GetOrderID();
    return (uint)-1;
}

//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::GetDirDanger
// Created: JDY 03-05-07
//-----------------------------------------------------------------------------
const MT_Vector2D& MIL_PionOrderManager::GetDirDanger() const
{
    if( pMission_ )
        return pMission_->GetDirDanger();
    static const MT_Vector2D vDefaultDirDanger( 0, 1 );
    return vDefaultDirDanger;
}


// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::GetFuseau
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
const MIL_Fuseau& MIL_PionOrderManager::GetFuseau() const
{
    //$$$ BEAH
    const MIL_AutomateMRT* pMRT = pion_.GetAutomate().GetOrderManager().GetMRT();
    if( pMRT && !pMRT->IsActivated() )
        return pMRT->GetFuseauForPion( pion_ );
    if( pMission_ )
        return pMission_->GetFuseau();
    static const MIL_Fuseau emptyFuseau; //$$$ CRADE
    return emptyFuseau;
}

//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::GetLimas
// Created: NLD 2002-10-09
//-----------------------------------------------------------------------------
const T_LimaFlagedPtrMap& MIL_PionOrderManager::GetLimas() const
{
    if( pMission_ )
        return pMission_->GetLimas();
    static const T_LimaFlagedPtrMap emptyLimaVector;
    return emptyLimaVector;
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::SetMissionLimaFlag
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool MIL_PionOrderManager::SetMissionLimaFlag( const MIL_Lima& lima, bool bValue )
{
    if( !pMission_ )
        return false;
    return pMission_->SetLimaFlag( lima, bValue );
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::GetMissionLimaFlag
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool MIL_PionOrderManager::GetMissionLimaFlag( const MIL_Lima& lima ) const
{
    if( !pMission_ )
        return false;
    return pMission_->GetLimaFlag( lima );
}

//-----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::SendMsgPionOrderAck
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
void MIL_PionOrderManager::SendMsgPionOrderAck( const ASN1T_MsgPionOrder& asnMsgPionOrder, ASN1T_EnumOrderErrorCode  nErrorCode, MIL_MOSContextID nContext )
{
    NET_ASN_MsgPionOrderAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg().order_id             = asnMsgPionOrder.order_id;
    asnReplyMsg.GetAsnMsg().oid_unite_executante = asnMsgPionOrder.oid_unite_executante;
    asnReplyMsg.GetAsnMsg().error_code           = nErrorCode;
    asnReplyMsg.Send( nContext );    
}

// -----------------------------------------------------------------------------
// Name: MIL_PionOrderManager::SendMsgOrderManagement
// Created: NLD 2003-09-17
// -----------------------------------------------------------------------------
void MIL_PionOrderManager::SendMsgOrderManagement( uint nOrderID, ASN1T_EnumOrderState nOrderState )
{
    // MOS message
    NET_ASN_MsgOrderManagement asnMsg;
    asnMsg.GetAsnMsg().order_id = nOrderID;
    asnMsg.GetAsnMsg().etat     = nOrderState;
    asnMsg.Send();
}


