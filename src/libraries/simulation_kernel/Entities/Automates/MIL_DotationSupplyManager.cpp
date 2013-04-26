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
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "MIL_AgentServer.h"
#include "protocol/ClientSenders.h"
#include <boost/serialization/shared_ptr.hpp>

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
// Name: MIL_DotationSupplyManager::serialize
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< MIL_SupplyManager >( *this );
    file >> pAutomate_;
    file >> bSupplyNeeded_;
    file >> bDotationSupplyExplicitlyRequested_;
    file >> supplyRequestBuilder_;
    logistic::SupplyRequestContainer* ptr;
    file >> ptr;
    supplyRequests_.reset( ptr );
    size_t scheduleSize;
    file >> scheduleSize;
    for( size_t i = 0; i < scheduleSize; ++i )
    {
        boost::shared_ptr< logistic::SupplyConsign_ABC > ptr;
        file >> ptr;
        scheduledSupplies_.insert( ptr );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::serialize
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< MIL_SupplyManager >( *this );
    file << pAutomate_;
    file << bSupplyNeeded_;
    file << bDotationSupplyExplicitlyRequested_;
    file << supplyRequestBuilder_;
    logistic::SupplyRequestContainer* ptr = supplyRequests_.get();
    file << ptr;
    size_t scheduleSize = scheduledSupplies_.size();
    file << scheduleSize;
    for( auto it = scheduledSupplies_.begin(); it != scheduledSupplies_.end(); ++it )
    {
        boost::shared_ptr< logistic::SupplyConsign_ABC > ptr( boost::const_pointer_cast< logistic::SupplyConsign_ABC >( *it ) );
        file << ptr;
    }
}

// =============================================================================
// OPERATIONS
// =============================================================================

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
// Name: MIL_DotationSupplyManager::NotifyDotationSupplyNeeded
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::NotifyDotationSupplyNeeded( const PHY_DotationCategory& dotationCategory )
{
    if( HasDotationSupplyNeededNotified( dotationCategory ) )
        return;
    bSupplyNeeded_ = true;
    if( SendSupplyNeededReport() )
        MIL_Report::PostEvent( *pAutomate_, MIL_Report::eRC_DemandeRavitaillementDotations );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::RequestDotationSupply
// Created: NLD 2005-03-03
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::RequestDotationSupply()
{
    bSupplyNeeded_              = true;
    bDotationSupplyExplicitlyRequested_ = true;
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
    for( T_Supplies::const_iterator it = scheduledSupplies_.begin(); it != scheduledSupplies_.end(); ++it )
        if( (*it)->IsSupplying( dotationCategory, *this ) )
            return true;
    return false;
}

// =============================================================================
// SupplyRecipient_ABC
// =============================================================================

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
void MIL_DotationSupplyManager::OnSupplyScheduled( boost::shared_ptr< const logistic::SupplyConsign_ABC > supplyConsign )
{
    scheduledSupplies_.insert( supplyConsign );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::OnSupplyCanceled
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::OnSupplyCanceled( boost::shared_ptr< const logistic::SupplyConsign_ABC > supplyConsign )
{
    MIL_Report::PostEvent( *pAutomate_, MIL_Report::eRC_RavitaillementDotationsAnnule );
    bSupplyNeeded_ = true;
    scheduledSupplies_.erase( supplyConsign );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::OnSupplyDone
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::OnSupplyDone( boost::shared_ptr< const logistic::SupplyConsign_ABC > supplyConsign )
{
    MIL_Report::PostEvent( *pAutomate_, MIL_Report::eRC_RavitaillementDotationsEffectue );
    scheduledSupplies_.erase( supplyConsign );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::OnSupplyConvoyArriving
// Created: NLD 2011-09-13
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::OnSupplyConvoyArriving( boost::shared_ptr< const logistic::SupplyConsign_ABC > supplyConsign )
{
    MIL_AutomateLOG* logisticBase = pAutomate_->FindLogisticManager();
    if( logisticBase )
        logisticBase->OnSupplyConvoyArriving( supplyConsign );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::OnSupplyConvoyLeaving
// Created: NLD 2011-09-13
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::OnSupplyConvoyLeaving( boost::shared_ptr< const logistic::SupplyConsign_ABC > supplyConsign )
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
