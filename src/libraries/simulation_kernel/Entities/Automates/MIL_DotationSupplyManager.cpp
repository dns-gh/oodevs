// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/MIL_Automate.cpp $
// $Author: Nld $
// $Modtime: 12/05/05 17:39 $
// $Revision: 32 $
// $Workfile: MIL_Automate.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_DotationSupplyManager.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Logistic/SupplyRequestContainer.h"
#include "Entities/Agents/Roles/Logistic/SupplyDotationRequestBuilder.h"
#include "Entities/Agents/Roles/Logistic/SupplyRequestHierarchyDispatcher.h"
#include "Entities/Agents/Roles/Logistic/SupplyConsign_ABC.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Checkpoints/SerializationTools.h"
#include "protocol/ClientSenders.h"
#include <boost/range/algorithm.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_DotationSupplyManager )

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_DotationSupplyManager::MIL_DotationSupplyManager( MIL_Automate& automate )
    : pAutomate_                         ( &automate )
    , bSupplyNeeded_                     ( false )
    , bDotationSupplyExplicitlyRequested_( false )
    , supplyRequestBuilder_              ( new logistic::SupplyDotationRequestBuilder( automate, *this ) )
    , supplyRequests_                    ( new logistic::SupplyRequestContainer( supplyRequestBuilder_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager constructor
// Created: JVT 2005-03-15
// -----------------------------------------------------------------------------
MIL_DotationSupplyManager::MIL_DotationSupplyManager()
    : pAutomate_                         ( 0 )
    , bSupplyNeeded_                     ( false )
    , bDotationSupplyExplicitlyRequested_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_DotationSupplyManager::~MIL_DotationSupplyManager()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_SupplyManager >( *this );
    file >> pAutomate_;
    file >> bSupplyNeeded_;
    file >> bDotationSupplyExplicitlyRequested_;
    file >> supplyRequestBuilder_;
    file >> supplyRequests_;
    file >> scheduledSupplies_;
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_SupplyManager >( *this );
    file << pAutomate_;
    file << bSupplyNeeded_;
    file << bDotationSupplyExplicitlyRequested_;
    file << supplyRequestBuilder_;
    file << supplyRequests_;
    file << scheduledSupplies_;
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::RequestDotationSupply
// Created: NLD 2005-03-03
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::RequestDotationSupply()
{
    bSupplyNeeded_ = true;
    bDotationSupplyExplicitlyRequested_ = true;
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::IsSupplyManual
// Created: BAX 2014-02-27
// -----------------------------------------------------------------------------
bool MIL_DotationSupplyManager::IsSupplyManual() const
{
    if( !pAutomate_ )
        return false;
    auto brain = pAutomate_->GetLogisticHierarchy().GetPrimarySuperior();
    return brain && brain->IsSupplyManual();
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::Update
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::Update()
{
    supplyRequests_->Update();
    if( !bSupplyNeeded_ )
        return;
    logistic::SupplyRequestHierarchyDispatcher dispatcher( pAutomate_->GetLogisticHierarchy(), bDotationSupplyExplicitlyRequested_ );
    bSupplyNeeded_ = !supplyRequests_->Execute( dispatcher );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::Clean
// Created: NLD 2005-01-07
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::Clean()
{
    bDotationSupplyExplicitlyRequested_ = false;
    supplyRequests_->Clean();
    MIL_SupplyManager::Clean();
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::ResetAllConsigns
// Created: JSR 2013-02-20
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::ResetAllConsigns()
{
    supplyRequests_->ResetConsign();
    for( auto it = scheduledSupplies_.begin(); it != scheduledSupplies_.end(); ++it )
        (*it)->ResetConsign();
    scheduledSupplies_.clear();
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::DeleteRequestsForRequester
// Created: JSR 2013-03-01
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::DeleteRequestsForRequester( MIL_AgentPion& pion )
{
    supplyRequests_->DeleteRequestsForRequester( pion );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::ResetConsignsForConvoyPion
// Created: JSR 2013-02-20
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::ResetConsignsForConvoyPion( const MIL_AgentPion& pion )
{
    supplyRequests_->ResetConsignsForConvoyPion( pion );
    for( auto it = scheduledSupplies_.begin(); it != scheduledSupplies_.end(); )
        if( (*it)->ResetConsignsForConvoyPion( pion ) )
            it = scheduledSupplies_.erase( it );
        else
            ++it;
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::NotifyDotationSupplyNeeded
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::NotifyDotationSupplyNeeded( const PHY_DotationCategory& dotationCategory )
{
    const bool wasNeeded = bSupplyNeeded_;
    bSupplyNeeded_ |= !IsSupplyManual() && !IsSupplyInProgress( dotationCategory );
    if( !wasNeeded && bSupplyNeeded_ && SendSupplyNeededReport() )
        MIL_Report::PostEvent( *pAutomate_, report::eRC_DemandeRavitaillementDotations );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::IsSupplyInProgress
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
bool MIL_DotationSupplyManager::IsSupplyInProgress( const PHY_DotationCategory& dotationCategory ) const
{
    return boost::find_if( scheduledSupplies_,
        [&]( const T_Supplies::value_type& consign )
        {
            return consign->IsSupplying( dotationCategory, *this );
        } )
        != scheduledSupplies_.end();
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::GetPosition
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
const MT_Vector2D& MIL_DotationSupplyManager::GetPosition() const
{
    return pAutomate_->GetPosition();
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::GetPC
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
const MIL_AgentPion* MIL_DotationSupplyManager::GetPC() const
{
    return pAutomate_->GetPionPC();
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::OnSupplyScheduled
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::OnSupplyScheduled( const boost::shared_ptr< logistic::SupplyConsign_ABC >& consign )
{
    scheduledSupplies_.insert( consign );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::OnSupplyCanceled
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::OnSupplyCanceled( const boost::shared_ptr< logistic::SupplyConsign_ABC >& consign )
{
    MIL_Report::PostEvent( *pAutomate_, report::eRC_RavitaillementDotationsAnnule );
    bSupplyNeeded_ = true;
    scheduledSupplies_.erase( consign );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::OnSupplyDone
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::OnSupplyDone( const boost::shared_ptr< logistic::SupplyConsign_ABC >& consign )
{
    MIL_Report::PostEvent( *pAutomate_, report::eRC_RavitaillementDotationsEffectue );
    scheduledSupplies_.erase( consign );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::OnSupplyConvoyArriving
// Created: NLD 2011-09-13
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::OnSupplyConvoyArriving( const boost::shared_ptr< logistic::SupplyConsign_ABC >& consign )
{
    MIL_AutomateLOG* logisticBase = pAutomate_->FindLogisticManager();
    if( logisticBase )
        logisticBase->OnSupplyConvoyArriving( consign );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::OnSupplyConvoyLeaving
// Created: NLD 2011-09-13
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::OnSupplyConvoyLeaving( const boost::shared_ptr< logistic::SupplyConsign_ABC >& consign )
{
    MIL_AutomateLOG* logisticBase = pAutomate_->FindLogisticManager();
    if( logisticBase )
        logisticBase->OnSupplyConvoyLeaving( consign );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::Serialize
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::Serialize( sword::Id& msg ) const
{
    msg.set_id( pAutomate_->GetID() );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::BelongsToLogisticBase
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
bool MIL_DotationSupplyManager::BelongsToLogisticBase( const MIL_AutomateLOG& logisticBase ) const
{
    const MIL_AutomateLOG* logisticManager = pAutomate_->FindLogisticManager();
    return logisticManager && logisticManager == &logisticBase;
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::SendChangedState
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::SendChangedState() const
{
    supplyRequests_->SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::SendFullState
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::SendFullState() const
{
    supplyRequests_->SendFullState();
}
