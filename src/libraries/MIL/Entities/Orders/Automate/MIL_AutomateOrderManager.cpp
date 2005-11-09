//*****************************************************************************
//
// $Created: NLD 2003-04-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Automate/MIL_AutomateOrderManager.cpp $
// $Author: Nld $
// $Modtime: 11/03/05 16:33 $
// $Revision: 6 $
// $Workfile: MIL_AutomateOrderManager.cpp $
//
//*****************************************************************************

#include "MIL_pch.h"

#include "MIL_AutomateOrderManager.h"

#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Automates/MIL_AutomateType.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Orders/Pion/MIL_PionMission_ABC.h"
#include "Entities/Orders/Pion/MIL_PionMissionType.h"
#include "Entities/Orders/Automate/MIL_AutomateMission_ABC.h"
#include "Entities/Orders/Automate/MIL_AutomateMissionType.h"
#include "Entities/Orders/Conduite/MIL_OrderConduiteRequest_ABC.h"
#include "Entities/Orders/Conduite/MIL_OrderConduiteType.h"
#include "Entities/Orders/Conduite/MIL_OrderConduite_ABC.h"
#include "Decision/DEC_ModelAutomate.h"
#include "Decision/DEC_ModelPion.h"
#include "Network/NET_ASN_Messages.h"

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager constructor
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
MIL_AutomateOrderManager::MIL_AutomateOrderManager( MIL_Automate& automate )
    : automate_                  ( automate )
    , pMission_                  ( 0 )
    , bRCMissionFinishedReceived_( false )
    , pReplacementMission_       ( 0 )
    , bNewMissionStarted_        ( false )
{
    
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager destructor
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
MIL_AutomateOrderManager::~MIL_AutomateOrderManager()
{
    StopAllOrders();
    assert( cdtPionMissionSet_.empty() );
}


// =============================================================================
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::Update
// Created: NLD 2003-09-02
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::Update()
{
    bNewMissionStarted_ = false;
    if( pReplacementMission_ )
    {
        MIL_AutomateMission_ABC* pMissionTmp = pReplacementMission_;
        pReplacementMission_ = 0;

        StopAllOrders();
        pMission_ = pMissionTmp;

        NET_ASN_MsgAutomateOrder asnMsg;
        pMission_->Serialize( asnMsg.GetAsnMsg() );
        asnMsg.Send();
        pMission_->CleanAfterSerialization( asnMsg.GetAsnMsg() );

        pMission_->StartMRT();
        SendMsgOrderManagement( pMission_->GetOrderID(), EnumOrderState::started );
        bNewMissionStarted_ = true;
    } 

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
// Name: MIL_AutomateOrderManager::CancelAllOrders
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
void MIL_AutomateOrderManager::CancelAllOrders()
{
    //assert( automate_.IsEmbraye() );

    // Destruction de toutes les missions préparées mais non données par l'automate à ses pions pendant la conduite
    for( IT_PionMissionSet itPionMission = cdtPionMissionSet_.begin(); itPionMission != cdtPionMissionSet_.end(); ++itPionMission )
    {
        (*itPionMission)->Terminate();
        delete *itPionMission;
    }
    cdtPionMissionSet_.clear();

    // Desactivation des comportements de l'automate : MRT + Conduite  
    if( pMission_ )
    {
        pMission_->StopMRT     ();
        pMission_->StopConduite();
        SendMsgOrderManagement( pMission_->GetOrderID(), EnumOrderState::cancelled );
        pMission_->Terminate   ();
        delete pMission_;
        pMission_ = 0;
    }

    // Annulation de tous les ordres des pions
    const MIL_Automate::T_PionVector& pions = automate_.GetPions();
    for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
        (**itPion).GetOrderManager().CancelAllOrders();
    assert( !pMission_ );

    if( pReplacementMission_ )
    {
        pReplacementMission_->Terminate();
        delete pReplacementMission_;
        pReplacementMission_ = 0;
    }
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::StopAllOrders
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
void MIL_AutomateOrderManager::StopAllOrders()
{
    // Destruction de toutes les missions préparées mais non données par l'automate à ses pions pendant la conduite
    for( IT_PionMissionSet itPionMission = cdtPionMissionSet_.begin(); itPionMission != cdtPionMissionSet_.end(); ++itPionMission )
    {
        (*itPionMission)->Terminate();
        delete *itPionMission;
    }
    cdtPionMissionSet_.clear();

    // Desactivation des comportements de l'automate : MRT + Conduite  
    if( pMission_ )
    {
        pMission_->StopMRT     ();
        pMission_->StopConduite();
        SendMsgOrderManagement( pMission_->GetOrderID(), EnumOrderState::stopped );
        pMission_->Terminate   ();
        delete pMission_;
        pMission_ = 0;
    }

    // Si l'automate est embrayé, annulation de tous les ordres des pions
    if( automate_.IsEmbraye() )
    {
        const MIL_Automate::T_PionVector& pions = automate_.GetPions();
        for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
            (**itPion).GetOrderManager().StopAllOrders();
    }
    assert( !pMission_ );

    if( pReplacementMission_ )
    {
        pReplacementMission_->Terminate();
        delete pReplacementMission_;
        pReplacementMission_ = 0;
    }
}


// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::Embraye
// Created: NLD 2003-12-17
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::Embraye()
{
    assert( cdtPionMissionSet_.empty() );
    assert( !pMission_ );

    // Annulation de toutes les missions des subordonnés
    const MIL_Automate::T_PionVector& pions = automate_.GetPions();
    for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
        (**itPion).GetOrderManager().CancelAllOrders();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::Debraye
// Created: NLD 2003-12-17
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::Debraye()
{
    // Destruction de toutes les missions préparées mais non données par l'automate à ses pions pendant la conduite
    for( IT_PionMissionSet itPionMission = cdtPionMissionSet_.begin(); itPionMission != cdtPionMissionSet_.end(); ++itPionMission )
    {
        (*itPionMission)->Terminate();
        delete *itPionMission;
    }
    cdtPionMissionSet_.clear();

    // Desactivation des comportements de l'automate : MRT + Conduite  
    if( pMission_ )
    {
        pMission_->StopMRT     ();
        pMission_->StopConduite();
        SendMsgOrderManagement( pMission_->GetOrderID(), EnumOrderState::cancelled );
        pMission_->Terminate   ();
        delete pMission_;
        pMission_ = 0;
    }

    // NB : Quand l'automate est debraye, les ordres des subordonnés ne sont pas annulés
}

//=============================================================================
// AUTOMATE EVENTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::OnReceiveMsgAutomateOrder
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
void MIL_AutomateOrderManager::OnReceiveMsgAutomateOrder( const ASN1T_MsgAutomateOrder& asnMsg, MIL_MOSContextID nContext )
{
    if( !automate_.IsEmbraye() )
    {
        SendMsgAutomateOrderAck( asnMsg, EnumOrderErrorCode::error_unit_cannot_receive_order, nContext );
        return;
    }

    if( automate_.IsSurrendered() )
    {
        SendMsgAutomateOrderAck( asnMsg, EnumOrderErrorCode::error_unit_surrendered, nContext );
        return;
    }

    // Instanciate and check the new mission
    const MIL_AutomateMissionType* pMissionType = MIL_AutomateMissionType::FindAutomateMissionType( asnMsg.mission );
    if( !( pMissionType && automate_.GetType().GetModel().IsMissionAvailable( *pMissionType ) ) )
    {
        SendMsgAutomateOrderAck( asnMsg, EnumOrderErrorCode::error_invalid_mission, nContext );
        return;
    }
    
    MIL_AutomateMission_ABC& mission = pMissionType->InstanciateMission( automate_ );

    ASN1T_EnumOrderErrorCode nCode = mission.Initialize( asnMsg );
    if( nCode != EnumOrderErrorCode::no_error )
    {
        mission.Terminate();
        delete &mission;
        SendMsgAutomateOrderAck( asnMsg, nCode, nContext );
        return;
    }
    mission.Prepare();

    // The mission is valid : cancel all the previous orders
    CancelAllOrders();

    SendMsgAutomateOrderAck( asnMsg, EnumOrderErrorCode::no_error, nContext );

    // Start the new mission
    assert( !pMission_ );
    pMission_ = &mission;
    pMission_->StartMRT();
    SendMsgOrderManagement( pMission_->GetOrderID(), EnumOrderState::started );
    bNewMissionStarted_ = true;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::OnReceiveAutomateOrder
// Created: NLD 2003-04-09
//-----------------------------------------------------------------------------
void MIL_AutomateOrderManager::OnReceiveAutomateOrder( MIL_AutomateMission_ABC& mission )
{    
    assert( automate_.IsEmbraye() );
    assert( automate_.GetType().GetModel().IsMissionAvailable( mission.GetType() ) );

    // Ordre donné directement par la SIM (Cf. Réfugiés)
    if( pReplacementMission_ )
    {
        pReplacementMission_->Terminate();
        delete pReplacementMission_;
        pReplacementMission_ = 0;
    }
    pReplacementMission_ = &mission;
}

//=============================================================================
// MRT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::MRT_CreatePionMission
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
MIL_PionMission_ABC* MIL_AutomateOrderManager::MRT_CreatePionMission( MIL_AgentPion& pion, const MIL_PionMissionType& missionType )
{
    assert( automate_.IsEmbraye() );
    if( !pMission_ )
    {
        MT_LOG_WARNING( "Automate '" << automate_.GetName() << "' (ID " << automate_.GetID() << ", Model '" << automate_.GetType().GetModel().GetName() << "') has no current mission", 2, "MIL_AutomateOrderManager::MRT_CreatePionMission" );
        return 0;
    }

    if( !pion.GetType().GetModel().IsMissionAvailable( missionType ) )
    {
        MT_LOG_ERROR( "Mission '" << missionType.GetName() << "' not available for pion '" << pion.GetName() << "' (ID " << pion.GetID() << ", Model '" << pion.GetType().GetModel().GetName() << "')", 4, "MIL_AutomateOrderManager::MRT_CreatePionMission" );
        return 0;
    }

    if( pMission_->GetMRT().IsActivated() )
    {
        MT_LOG_ERROR( "MRT already activated for pion '" << pion.GetName() << "' (ID " << pion.GetID() << ", Model '" << pion.GetType().GetModel().GetName() << "') - Mission '" << missionType.GetName() << "'", 4, "MIL_AutomateOrderManager::MRT_CreatePionMission" );
        return 0;
    }
    
    MIL_PionMission_ABC& pionMission = missionType.InstanciateMission( pion );
    if( !pionMission.Initialize( *pMission_ ) )
    {
        MT_LOG_ERROR( "Mission '" << missionType.GetName() << "' initialisation failed for pion '" << pion.GetName() << "' (ID " << pion.GetID() << ", Model '" << pion.GetType().GetModel().GetName() << "')", 4, "MIL_AutomateOrderManager::MRT_CreatePionMission" );
        return 0;
    }

    if( !pMission_->GetMRT().SetMissionForPion( pion, pionMission ) )
    {
        MT_LOG_ERROR( "Pion '" << pion.GetName() << "' (ID " << pion.GetID() << ", Model '" << pion.GetType().GetModel().GetName() << "') has already a mission when giving mission '" << missionType.GetName() << "')", 4, "MIL_AutomateOrderManager::MRT_CreatePionMission" );
        return 0;
    }

    return &pionMission;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::MRT_Validate
// Created: NLD 2003-04-14
//-----------------------------------------------------------------------------
void MIL_AutomateOrderManager::MRT_Validate()
{
    assert( automate_.IsEmbraye() );
    if( !pMission_ )
    {
        MT_LOG_ERROR( "Automate has no current mission", 2, "MIL_AutomateOrderManager::MRT_CreatePionMission" );
        return;
    }

    pMission_->GetMRT().Validate();
    pMission_->StopMRT();

    /*****
    $$$$ TESTS - Devrait avoir échange MOS-SIM avant activation
    ******/
    pMission_->GetMRT().Activate();
    pMission_->StartConduite();
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::CDT_CreatePionMission
// Created: NLD 2003-04-16
//-----------------------------------------------------------------------------
MIL_PionMission_ABC* MIL_AutomateOrderManager::CDT_CreatePionMission( MIL_AgentPion& pion, const MIL_PionMissionType& missionType )
{
    assert( automate_.IsEmbraye() );    

    if( !pMission_ )
    {
        MT_LOG_WARNING( "Automate '" << automate_.GetName() << "' (ID " << automate_.GetID() << ", Model '" << automate_.GetType().GetModel().GetName() << "') has no current mission", 2, "MIL_AutomateOrderManager::CDT_CreatePionMission" );
        return 0;
    }

    if( !pion.GetType().GetModel().IsMissionAvailable( missionType ) )
    {
        MT_LOG_ERROR( "Mission '" << missionType.GetName() << "' not available for pion '" << pion.GetName() << "' (ID " << pion.GetID() << ", Model '" << pion.GetType().GetModel().GetName() << "')", 4, "MIL_AutomateOrderManager::CDT_CreatePionMission" );
        return 0;
    }

    if( !pMission_->GetMRT().IsActivated() )
    {
        MT_LOG_WARNING( "MRT not activated for pion '" << pion.GetName() << "' (ID " << pion.GetID() << ", Model '" << pion.GetType().GetModel().GetName() << "') - Mission '" << missionType.GetName() << "')", 4, "MIL_AutomateOrderManager::CDT_CreatePionMission" );
        return 0;
    }
    
    MIL_PionMission_ABC& pionMission = missionType.InstanciateMission( pion );
    if( !pionMission.Initialize( *pMission_ ) )
    {
        MT_LOG_ERROR( "Mission '" << missionType.GetName() << "' initialisation failed for pion '" << pion.GetName() << "' (ID " << pion.GetID() << ", Model '" << pion.GetType().GetModel().GetName() << "')", 4, "MIL_AutomateOrderManager::CDT_CreatePionMission" );
        return 0;
    }

    bool bOut = cdtPionMissionSet_.insert( &pionMission ).second;
    assert( bOut );

    return &pionMission;    
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::CDT_GivePionMission
// Created: NLD 2003-04-16
//-----------------------------------------------------------------------------
void MIL_AutomateOrderManager::CDT_GivePionMission( MIL_PionMission_ABC& pionMission )
{
    assert( automate_.IsEmbraye() );

    int nOut = cdtPionMissionSet_.erase( &pionMission );
    assert( nOut == 1 );

    MIL_PionOrderManager& pionOrderMgr = pionMission.GetPion().GetOrderManager();
    pionOrderMgr.OnReceivePionOrder( pionMission );
}

// =============================================================================
// CONDUITE
// =============================================================================

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::OnReceiveMsgOrderConduite
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void MIL_AutomateOrderManager::OnReceiveMsgOrderConduite( const ASN1T_MsgOrderConduite& asnMsg, MIL_MOSContextID nCtx )
{
    NET_ASN_MsgOrderConduiteAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg().unit_id  = asnMsg.unit_id;
    asnReplyMsg.GetAsnMsg().order_id = asnMsg.order_id;

    if( automate_.IsSurrendered() )
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
    MIL_OrderConduite_ABC& orderConduite = pOrderConduiteType->InstanciateOrderConduite( automate_.GetKnowledgeGroup() );
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
// Name: MIL_AutomateOrderManager::WaitForOrder
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
void MIL_AutomateOrderManager::WaitForOrder( DIA_Parameters& diaParams )
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

    // Send to mos
    ASN1T_MsgAttenteOrdreConduite_ordres_conduite_element* pOrders = new ASN1T_MsgAttenteOrdreConduite_ordres_conduite_element[ orderConduiteRequestVector_.size() ]; //$$$ RAM

    uint i = 0; 
    for( CIT_ObjectVector itOrder = orderConduiteRequestVector_.begin(); itOrder != orderConduiteRequestVector_.end(); ++itOrder )
        ((MIL_OrderConduiteRequest_ABC&)(**itOrder)).Serialize( pOrders[i++] );

    NET_ASN_MsgAttenteOrdreConduite asnMsg;
    asnMsg.GetAsnMsg().unit_id              = automate_.GetID();
    asnMsg.GetAsnMsg().order_id             = GetCurrentOrderID();
    asnMsg.GetAsnMsg().ordres_conduite.n    = orderConduiteRequestVector_.size();  
    asnMsg.GetAsnMsg().ordres_conduite.elem = pOrders;
    asnMsg.Send();

    delete[] pOrders;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::StopWaitingForOrder
// Created: NLD 2003-01-10
//-----------------------------------------------------------------------------
void MIL_AutomateOrderManager::StopWaitingForOrder()
{
    if( !orderConduiteRequestVector_.empty() )
    {
        NET_ASN_MsgAnnuleAttenteOrdreConduite asnMsg;
        asnMsg.GetAsnMsg().unit_id  = automate_.GetID();
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
// Name: MIL_AutomateOrderManager::LaunchOrderConduite
// Created: NLD 2003-01-09
//-----------------------------------------------------------------------------
bool MIL_AutomateOrderManager::LaunchOrderConduite( MIL_OrderConduite_ABC& orderConduite )
{
    DEC_AutomateDecision& roleDecision = automate_.GetDecision();

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
    const DEC_ModelAutomate& model = automate_.GetType().GetModel();
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

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::SendMsgAutomateOrderAck
// Created: NLD 2003-04-10
//-----------------------------------------------------------------------------
void MIL_AutomateOrderManager::SendMsgAutomateOrderAck( const ASN1T_MsgAutomateOrder& asnMsgAutomateOrder, ASN1T_EnumOrderErrorCode  nErrorCode, MIL_MOSContextID nContext )
{
    NET_ASN_MsgAutomateOrderAck asnReplyMsg;
    asnReplyMsg.GetAsnMsg().order_id             = asnMsgAutomateOrder.order_id;
    asnReplyMsg.GetAsnMsg().oid_unite_executante = asnMsgAutomateOrder.oid_unite_executante;
    asnReplyMsg.GetAsnMsg().error_code           = nErrorCode;
    asnReplyMsg.Send( nContext );    
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::SendMsgOrderManagement
// Created: NLD 2003-09-17
// -----------------------------------------------------------------------------
void MIL_AutomateOrderManager::SendMsgOrderManagement( uint nOrderID, ASN1T_EnumOrderState nOrderState )
{
    // MOS message
    NET_ASN_MsgOrderManagement asnMsg;
    asnMsg.GetAsnMsg().order_id = nOrderID;
    asnMsg.GetAsnMsg().etat     = nOrderState;
    asnMsg.Send();
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::GetMRT
// Created: NLD 2003-04-23
//-----------------------------------------------------------------------------
MIL_AutomateMRT* MIL_AutomateOrderManager::GetMRT() const
{
    if( !pMission_ )
        return 0;
    return &pMission_->GetMRT();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::GetCurrentOrderID
// Created: NLD 2003-12-17
// -----------------------------------------------------------------------------
uint MIL_AutomateOrderManager::GetCurrentOrderID() const
{
    if( pMission_ )
        return pMission_->GetOrderID();
    return (uint)-1;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::GetDirDanger
// Created: JDY 03-05-07
//-----------------------------------------------------------------------------
const MT_Vector2D& MIL_AutomateOrderManager::GetDirDanger() const
{
    if( pMission_ )
        return pMission_->GetDirDanger();
    static const MT_Vector2D vDefaultDirDanger( 0, 1 );
    return vDefaultDirDanger;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::GetFuseau
// Created: NLD 2003-12-23
// -----------------------------------------------------------------------------
const MIL_Fuseau& MIL_AutomateOrderManager::GetFuseau() const
{
    if( pMission_ )
        return pMission_->GetFuseau();
    static const MIL_Fuseau emptyFuseau; 
    return emptyFuseau;
}

//-----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::GetLimas
// Created: NLD 2002-10-09
//-----------------------------------------------------------------------------
const T_LimaFlagedPtrMap& MIL_AutomateOrderManager::GetLimas() const
{
    if( pMission_ )
        return pMission_->GetLimas();
    static const T_LimaFlagedPtrMap emptyLimaVector;
    return emptyLimaVector;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::SetMissionLimaFlag
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool MIL_AutomateOrderManager::SetMissionLimaFlag( const MIL_Lima& lima, bool bValue )
{
    if( automate_.IsEmbraye() )
    {
        const MIL_Automate::T_PionVector& pions = automate_.GetPions();
        for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
            (**itPion).GetOrderManager().SetMissionLimaFlag( lima, bValue );
    }

    if( !pMission_ )
        return false;
    return pMission_->SetLimaFlag( lima, bValue );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateOrderManager::GetMissionLimaFlag
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
bool MIL_AutomateOrderManager::GetMissionLimaFlag( const MIL_Lima& lima ) const
{
    if( !pMission_ )
        return false;
    return pMission_->GetLimaFlag( lima );
}
