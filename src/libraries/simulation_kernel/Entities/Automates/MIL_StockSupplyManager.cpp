// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Specialisations/Log/MIL_StockSupplyManager.cpp $
// $Author: Nld $
// $Modtime: 11/05/05 14:54 $
// $Revision: 18 $
// $Workfile: MIL_StockSupplyManager.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_StockSupplyManager.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/Roles/Logistic/PHY_SupplyStockRequestContainer.h"
#include "Entities/Agents/Roles/Logistic/PHY_SupplyStockState.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Formation.h"
#include "protocol/ClientSenders.h"
#include "protocol/SimulationSenders.h"
#include "Network/NET_AsnException.h"
#include <boost/serialization/set.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_StockSupplyManager )
using namespace MsgsSimToClient;
using namespace MsgsClientToSim;

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager constructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
MIL_StockSupplyManager::MIL_StockSupplyManager( MIL_Automate& automate )
    : pAutomate_                  ( &automate )
    , bStockSupplyNeeded_         ( false )
    , pExplicitStockSupplyState_  ( 0 )
    , manualSupplyStates_         ()
    , nTickRcStockSupplyQuerySent_( 0 )
{   
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager constructor
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
MIL_StockSupplyManager::MIL_StockSupplyManager()
    : pAutomate_                  ( 0 )
    , bStockSupplyNeeded_         ( false )
    , pExplicitStockSupplyState_  ( 0 )
    , manualSupplyStates_         ()
    , nTickRcStockSupplyQuerySent_( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager destructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
MIL_StockSupplyManager::~MIL_StockSupplyManager()
{
}

// =============================================================================
// CHEKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::serialize
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
template < typename Archive >
void MIL_StockSupplyManager::serialize( Archive& file, const uint )
{
    file & pAutomate_
         & bStockSupplyNeeded_
         & pExplicitStockSupplyState_
         & manualSupplyStates_
         & nTickRcStockSupplyQuerySent_;
}
  
// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::Update
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::Update()
{
    if( !bStockSupplyNeeded_ || pExplicitStockSupplyState_ )
        return;

    MIL_AutomateLOG* pLogisticSuperior = 0;
    MIL_AutomateLOG* pLogisticManager = pAutomate_->FindLogisticManager();
    if( pLogisticManager )
        pLogisticSuperior = pLogisticManager->GetSuperior();
    if( !pLogisticSuperior )
        return;

    PHY_SupplyStockRequestContainer supplyRequests( *pAutomate_ );
    if(pLogisticManager->GetNominalSuperior() != pLogisticSuperior)
        bStockSupplyNeeded_ = !supplyRequests.Execute( *pLogisticSuperior, *pLogisticManager->GetNominalSuperior(), pExplicitStockSupplyState_ );
    else
        bStockSupplyNeeded_ = !supplyRequests.Execute( *pLogisticSuperior, pExplicitStockSupplyState_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::Clean
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::Clean()
{
    if( pExplicitStockSupplyState_ )
        pExplicitStockSupplyState_->Clean();
    for( CIT_SupplyStockStateSet it = manualSupplyStates_.begin(); it != manualSupplyStates_.end(); ++it )
        (**it).Clean();
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::IsSupplyInProgress
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
bool MIL_StockSupplyManager::IsSupplyInProgress( const PHY_DotationCategory& dotationCategory ) const
{
    if( pExplicitStockSupplyState_ && pExplicitStockSupplyState_->IsSupplying( dotationCategory ) )
        return true;
    
    for( CIT_SupplyStockStateSet it = manualSupplyStates_.begin(); it != manualSupplyStates_.end(); ++it )
    {
        if( (**it).IsSupplying( dotationCategory ) )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::NotifyStockSupplyNeeded
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::NotifyStockSupplyNeeded( const PHY_DotationCategory& dotationCategory )
{
    if( bStockSupplyNeeded_ )
        return;

    if( IsSupplyInProgress( dotationCategory ) )
        return;

    bStockSupplyNeeded_ = true;

    // Pas de RC si log non branchée ou si RC envoyé au tick précédent
    const uint nCurrentTick = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( pAutomate_->GetTC2() && ( nCurrentTick > ( nTickRcStockSupplyQuerySent_ + 1 ) || nTickRcStockSupplyQuerySent_ == 0 ) )
        MIL_Report::PostEvent( *pAutomate_, MIL_Report::eReport_StockSupplyRequest );
    nTickRcStockSupplyQuerySent_ = nCurrentTick;
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::RemoveSupplyStockState
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::RemoveSupplyStockState( const PHY_SupplyStockState& supplyState )
{
    if( &supplyState == pExplicitStockSupplyState_ )
    {
        pExplicitStockSupplyState_ = 0;
        return;
    }

    IT_SupplyStockStateSet it = manualSupplyStates_.find( const_cast< PHY_SupplyStockState* >( &supplyState ) );
    assert( it != manualSupplyStates_.end() );
    manualSupplyStates_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::NotifyStockSupplied
// Created: NLD 2005-01-28
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::NotifyStockSupplied( const PHY_SupplyStockState& supplyState )
{
    MIL_Report::PostEvent( *pAutomate_, MIL_Report::eReport_StockSupplyDone );
    RemoveSupplyStockState( supplyState );
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::NotifyStockSupplyCanceled
// Created: NLD 2005-02-11
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::NotifyStockSupplyCanceled( const PHY_SupplyStockState& supplyState )
{
    MIL_Report::PostEvent( *pAutomate_, MIL_Report::eReport_StockSupplyCanceled );
    RemoveSupplyStockState( supplyState );
    bStockSupplyNeeded_ = true;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::OnReceiveMsgLogSupplyPullFlow
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::OnReceiveMsgLogSupplyPullFlow( const Common::MsgMissionParameters& msg )
{
	unsigned int oid_donneur = msg.elem( 0 ).value().has_automat() ?
	    		msg.elem( 0 ).value().automat().id() : msg.elem( 0 ).value().formation().id();
    MIL_Formation* candidateFormation = MIL_AgentServer::GetWorkspace().GetEntityManager().FindFormation( oid_donneur );
    MIL_Automate* candidateAutomate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( oid_donneur );
    if( !candidateAutomate && !candidateFormation)
        throw NET_AsnException< MsgsSimToClient::MsgLogSupplyPullFlowAck_EnumLogSupplyPullFlow >( MsgsSimToClient::MsgLogSupplyPullFlowAck_EnumLogSupplyPullFlow_error_invalid_provider_pullflow );

    MIL_AutomateLOG* pSupplier = candidateAutomate!=0 ? candidateAutomate->GetBrainLogistic() :
            candidateFormation->GetBrainLogistic();

    if( !pSupplier )
        throw NET_AsnException< MsgsSimToClient::MsgLogSupplyPullFlowAck_EnumLogSupplyPullFlow >( MsgsSimToClient::MsgLogSupplyPullFlowAck_EnumLogSupplyPullFlow_error_invalid_provider_pullflow );

    PHY_SupplyStockRequestContainer supplyRequests( *pAutomate_, msg.elem( 1 ), false );
	if(!supplyRequests.HasRequests())
		throw NET_AsnException< MsgsSimToClient::MsgLogSupplyPullFlowAck_EnumLogSupplyPullFlow >( MsgsSimToClient::MsgLogSupplyPullFlowAck_EnumLogSupplyPullFlow_error_invalid_provider_pullflow );

	PHY_SupplyStockState* pSupplyState = 0;
	supplyRequests.Execute( *pSupplier, pSupplyState );
	if( pSupplyState )
		manualSupplyStates_.insert( pSupplyState );
	else
		throw NET_AsnException< MsgsSimToClient::MsgLogSupplyPullFlowAck_EnumLogSupplyPullFlow >( MsgsSimToClient::MsgLogSupplyPullFlowAck_EnumLogSupplyPullFlow_error_invalid_provider_pullflow );
}


// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::OnReceiveMsgLogSupplyPushFlow
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::OnReceiveMsgLogSupplyPushFlow( const Common::MsgMissionParameters& msg)
{
    unsigned int oid_donneur = msg.elem( 0 ).value().has_automat() ?
    		msg.elem( 0 ).value().automat().id() : msg.elem( 0 ).value().formation().id();
    MIL_Formation* candidateFormation = MIL_AgentServer::GetWorkspace().GetEntityManager().FindFormation( oid_donneur );
    MIL_Automate* candidateAutomate = MIL_AgentServer::GetWorkspace().GetEntityManager().FindAutomate( oid_donneur );
    if( !candidateAutomate && !candidateFormation)
        throw NET_AsnException< MsgsSimToClient::MsgLogSupplyPushFlowAck_EnumLogSupplyPushFlow >( MsgsSimToClient::MsgLogSupplyPushFlowAck_EnumLogSupplyPushFlow_error_invalid_donneur_pushflow );

    MIL_AutomateLOG* pSupplier = candidateAutomate!=0 ? candidateAutomate->GetBrainLogistic() :
            candidateFormation->GetBrainLogistic();

    if( !pSupplier )
        throw NET_AsnException< MsgsSimToClient::MsgLogSupplyPushFlowAck_EnumLogSupplyPushFlow >( MsgsSimToClient::MsgLogSupplyPushFlowAck_EnumLogSupplyPushFlow_error_invalid_donneur_pushflow );

    PHY_SupplyStockRequestContainer supplyRequests( *pAutomate_, msg.elem( 1 ), false );
    if(!supplyRequests.HasRequests())
        throw NET_AsnException< MsgsSimToClient::MsgLogSupplyPushFlowAck_EnumLogSupplyPushFlow >( MsgsSimToClient::MsgLogSupplyPushFlowAck_EnumLogSupplyPushFlow_error_invalid_receveur_pushflow );

    PHY_SupplyStockState* pSupplyState = 0;
    supplyRequests.Execute( *pSupplier, pSupplyState );
    if( pSupplyState )
        manualSupplyStates_.insert( pSupplyState );
    else
        throw NET_AsnException< MsgsSimToClient::MsgLogSupplyPushFlowAck_EnumLogSupplyPushFlow >( MsgsSimToClient::MsgLogSupplyPushFlowAck_EnumLogSupplyPushFlow_error_invalid_donneur_pushflow );
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::SendChangedState
// Created: MIL_StockSupplyManager 2005-01-25
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::SendChangedState() const
{
    if( pExplicitStockSupplyState_ )
        pExplicitStockSupplyState_->SendChangedState();
    for( CIT_SupplyStockStateSet it = manualSupplyStates_.begin(); it != manualSupplyStates_.end(); ++it )
        (**it).SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::SendFullState
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::SendFullState() const
{
    if( pExplicitStockSupplyState_ )
        pExplicitStockSupplyState_->SendFullState();
    for( CIT_SupplyStockStateSet it = manualSupplyStates_.begin(); it != manualSupplyStates_.end(); ++it )
        (**it).SendFullState();
}


