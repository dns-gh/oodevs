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
#include <boost/serialization/set.hpp>

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
// Name: MIL_DotationSupplyManager::Update
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::Update()
{
    supplyRequests_->Update();
    if( bSupplyNeeded_ )
    {
        logistic::SupplyRequestHierarchyDispatcher dispatcher( pAutomate_->GetLogisticHierarchy(), bDotationSupplyExplicitlyRequested_ );
        bSupplyNeeded_ = !supplyRequests_->Execute( dispatcher );
    }
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
        const_cast< logistic::SupplyConsign_ABC* >( it->get() )->ResetConsign();
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
        if( const_cast< logistic::SupplyConsign_ABC* >( it->get() )->ResetConsignsForConvoyPion( pion ) )
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
    if( HasDotationSupplyNeededNotified( dotationCategory ) )
        return;
    bSupplyNeeded_ = true;
    if( SendSupplyNeededReport() )
        MIL_Report::PostEvent( *pAutomate_, report::eRC_DemandeRavitaillementDotations );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::HasDotationSupplyNeededNotified
// Created: MMC 2013-04-24
// -----------------------------------------------------------------------------
bool MIL_DotationSupplyManager::HasDotationSupplyNeededNotified( const PHY_DotationCategory& dotationCategory )
{
    return ( bSupplyNeeded_ || IsSupplyInProgress( dotationCategory ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::IsSupplyInProgress
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
bool MIL_DotationSupplyManager::IsSupplyInProgress( const PHY_DotationCategory& dotationCategory ) const
{
    return boost::find_if( scheduledSupplies_,
            boost::bind( &logistic::SupplyConsign_ABC::IsSupplying, _1, boost::cref( dotationCategory ), boost::cref( *this ) ) )
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
void MIL_DotationSupplyManager::OnSupplyScheduled( boost::shared_ptr< logistic::SupplyConsign_ABC > supplyConsign )
{
    scheduledSupplies_.insert( supplyConsign );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::OnSupplyCanceled
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::OnSupplyCanceled( boost::shared_ptr< logistic::SupplyConsign_ABC > supplyConsign )
{
    MIL_Report::PostEvent( *pAutomate_, report::eRC_RavitaillementDotationsAnnule );
    bSupplyNeeded_ = true;
    scheduledSupplies_.erase( supplyConsign );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::OnSupplyDone
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::OnSupplyDone( boost::shared_ptr< logistic::SupplyConsign_ABC > supplyConsign )
{
    MIL_Report::PostEvent( *pAutomate_, report::eRC_RavitaillementDotationsEffectue );
    scheduledSupplies_.erase( supplyConsign );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::OnSupplyConvoyArriving
// Created: NLD 2011-09-13
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::OnSupplyConvoyArriving( boost::shared_ptr< logistic::SupplyConsign_ABC > supplyConsign )
{
    MIL_AutomateLOG* logisticBase = pAutomate_->FindLogisticManager();
    if( logisticBase )
        logisticBase->OnSupplyConvoyArriving( supplyConsign );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::OnSupplyConvoyLeaving
// Created: NLD 2011-09-13
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::OnSupplyConvoyLeaving( boost::shared_ptr< logistic::SupplyConsign_ABC > supplyConsign )
{
    MIL_AutomateLOG* logisticBase = pAutomate_->FindLogisticManager();
    if( logisticBase )
        logisticBase->OnSupplyConvoyLeaving( supplyConsign );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::Serialize
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::Serialize( sword::AutomatId& msg ) const
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
