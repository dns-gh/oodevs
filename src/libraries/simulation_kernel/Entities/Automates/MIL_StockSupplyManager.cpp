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
#include "MIL_Time_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Logistic/SupplyRequestContainer.h"
#include "Entities/Agents/Roles/Logistic/SupplyStockRequestBuilder.h"
#include "Entities/Agents/Roles/Logistic/SupplyRequestHierarchyDispatcher.h"
#include "Entities/Agents/Roles/Logistic/SupplyStockPullFlowRequestBuilder.h"
#include "Entities/Agents/Roles/Logistic/SupplyRequestManualDispatcher.h"
#include "Entities/Agents/Roles/Logistic/SupplyConsign_ABC.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Formation.h"
#include "protocol/ClientSenders.h"
#include "protocol/SimulationSenders.h"
#include "Network/NET_AsnException.h"
#include <boost/serialization/set.hpp>
#include <boost/range/algorithm.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_StockSupplyManager )

using namespace sword;

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager constructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
MIL_StockSupplyManager::MIL_StockSupplyManager( MIL_Automate& automate )
    : pAutomate_           ( &automate )
    , bSupplyNeeded_       ( false )
    , supplyRequestBuilder_( new logistic::SupplyStockRequestBuilder( automate, *this ) )
    , autoSupplyRequest_   ( new logistic::SupplyRequestContainer( supplyRequestBuilder_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager constructor
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
MIL_StockSupplyManager::MIL_StockSupplyManager()
    : pAutomate_    ( 0 )
    , bSupplyNeeded_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager destructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
MIL_StockSupplyManager::~MIL_StockSupplyManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_SupplyManager >( *this );
    file >> pAutomate_;
    file >> bSupplyNeeded_;
    assert( pAutomate_ );
    supplyRequestBuilder_.reset( new logistic::SupplyStockRequestBuilder( *pAutomate_, *this ) );
    autoSupplyRequest_.reset( new logistic::SupplyRequestContainer( supplyRequestBuilder_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_SupplyManager >( *this );
    file << pAutomate_;
    file << bSupplyNeeded_;
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::Update
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::Update()
{
    autoSupplyRequest_->Update();
    for( auto it = manualSupplyRequests_.begin(); it != manualSupplyRequests_.end(); )
        (*it)->Update() ? it = manualSupplyRequests_.erase( it ) : ++it;
    if( !bSupplyNeeded_ )
        return;
    MIL_AutomateLOG* logisticManager = pAutomate_->FindLogisticManager();
    if( logisticManager )
    {
        logistic::SupplyRequestHierarchyDispatcher dispatcher( logisticManager->GetLogisticHierarchy() );
        bSupplyNeeded_ = !autoSupplyRequest_->Execute( dispatcher );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::Clean
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::Clean()
{
    autoSupplyRequest_->Clean();
    boost::for_each( manualSupplyRequests_, boost::mem_fn( &logistic::SupplyRequestContainer::Clean ) );
    MIL_SupplyManager::Clean();
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::NotifyStockSupplyNeeded
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::NotifyStockSupplyNeeded( const PHY_DotationCategory& dotationCategory )
{
    if( bSupplyNeeded_ || IsSupplyInProgress( dotationCategory ) )
        return;
    bSupplyNeeded_ = true;
    if( SendSupplyNeededReport() )
        MIL_Report::PostEvent( *pAutomate_, report::eRC_DemandeRavitaillementStocks );
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::ResetAutoConsignForConvoyPion
// Created: JSR 2013-02-08
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::ResetAutoConsignForConvoyPion( const MIL_AgentPion& pion )
{
    if( autoSupplyRequest_.get() )
        autoSupplyRequest_->ResetConsignsForConvoyPion( pion );
    for( auto it = scheduledSupplies_.begin(); it != scheduledSupplies_.end(); )
        if( (*it)->ResetConsignsForConvoyPion( pion ) )
            it = scheduledSupplies_.erase( it );
        else
            ++it;
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::ResetAutoConsignForProvider
// Created: JSR 2013-02-14
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::ResetAutoConsignForProvider( const MIL_Agent_ABC& pion )
{
    if( autoSupplyRequest_.get() )
        autoSupplyRequest_->ResetConsignsForProvider( pion );
    for( auto it = scheduledSupplies_.begin(); it != scheduledSupplies_.end(); )
        if( (*it)->ResetConsignsForProvider( pion ) )
            it = scheduledSupplies_.erase( it );
        else
            ++it;
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::ResetAllConsigns
// Created: JSR 2013-02-14
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::ResetAllConsigns()
{
    if( autoSupplyRequest_.get() )
        autoSupplyRequest_->ResetConsign();
    for( auto it = scheduledSupplies_.begin(); it != scheduledSupplies_.end(); ++it )
        (*it)->ResetConsign();
    scheduledSupplies_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::IsSupplyInProgress
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
bool MIL_StockSupplyManager::IsSupplyInProgress( const PHY_DotationCategory& dotationCategory ) const
{
    return boost::find_if( scheduledSupplies_,
            boost::bind( &logistic::SupplyConsign_ABC::IsSupplying, _1, boost::cref( dotationCategory ), boost::cref( *this ) ) )
        != scheduledSupplies_.end();
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::GetPosition
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
const MT_Vector2D& MIL_StockSupplyManager::GetPosition() const
{
    return pAutomate_->GetPosition();
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::GetPC
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
const MIL_AgentPion* MIL_StockSupplyManager::GetPC() const
{
    return pAutomate_->GetPionPC();
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::OnSupplyScheduled
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::OnSupplyScheduled( boost::shared_ptr< logistic::SupplyConsign_ABC > supplyConsign )
{
    scheduledSupplies_.insert( supplyConsign );
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::OnSupplyCanceled
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::OnSupplyCanceled( boost::shared_ptr< logistic::SupplyConsign_ABC > supplyConsign )
{
    MIL_Report::PostEvent( *pAutomate_, report::eRC_RavitaillementStockAnnule );
    bSupplyNeeded_ = true;
    scheduledSupplies_.erase( supplyConsign );
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::OnSupplyDone
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::OnSupplyDone( boost::shared_ptr< logistic::SupplyConsign_ABC > supplyConsign )
{
    MIL_Report::PostEvent( *pAutomate_, report::eRC_RavitaillementStockEffectue );
    scheduledSupplies_.erase( supplyConsign );
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::OnSupplyConvoyArriving
// Created: NLD 2011-09-13
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::OnSupplyConvoyArriving( boost::shared_ptr< logistic::SupplyConsign_ABC > supplyConsign )
{
    MIL_AutomateLOG* logisticBase = pAutomate_->FindLogisticManager();
    if( logisticBase )
        logisticBase->OnSupplyConvoyArriving( supplyConsign );
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::OnSupplyConvoyLeaving
// Created: NLD 2011-09-13
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::OnSupplyConvoyLeaving( boost::shared_ptr< logistic::SupplyConsign_ABC > supplyConsign )
{
    MIL_AutomateLOG* logisticBase = pAutomate_->FindLogisticManager();
    if( logisticBase )
        logisticBase->OnSupplyConvoyLeaving( supplyConsign );
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::Serialize
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::Serialize( sword::AutomatId& msg ) const
{
    msg.set_id( pAutomate_->GetID() );
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::BelongsToLogisticBase
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
bool MIL_StockSupplyManager::BelongsToLogisticBase( const MIL_AutomateLOG& logisticBase ) const
{
    const MIL_AutomateLOG* logisticManager = pAutomate_->FindLogisticManager();
    return logisticManager && logisticManager == &logisticBase;
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::OnReceiveLogSupplyPullFlow
// Created: NLD 2005-02-04
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::OnReceiveLogSupplyPullFlow( const sword::PullFlowParameters& parameters, logistic::SupplySupplier_ABC& supplier )
{
    boost::shared_ptr< logistic::SupplyRequestBuilder_ABC > builder( new logistic::SupplyStockPullFlowRequestBuilder( parameters, *pAutomate_, supplier ) );
    boost::shared_ptr< logistic::SupplyRequestContainer > requestContainer( new logistic::SupplyRequestContainer( builder ) );
    logistic::SupplyRequestManualDispatcher dispatcher( supplier );
    requestContainer->Execute( dispatcher );
    manualSupplyRequests_.push_back( requestContainer );
    if( !dispatcher.AllowSupply() )
        MIL_Report::PostEvent( *pAutomate_, report::eRC_SupplierUnavailable );
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::SendChangedState
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::SendChangedState() const
{
    autoSupplyRequest_->SendChangedState();
    boost::for_each( manualSupplyRequests_, boost::mem_fn( &logistic::SupplyRequestContainer::SendChangedState ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::SendFullState
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::SendFullState() const
{
    autoSupplyRequest_->SendFullState();
    boost::for_each( manualSupplyRequests_, boost::mem_fn( &logistic::SupplyRequestContainer::SendFullState ) );
}
