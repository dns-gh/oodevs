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

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_DotationSupplyManager )

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager constructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_DotationSupplyManager::MIL_DotationSupplyManager( MIL_Automate& automate )
    : pAutomate_                         ( &automate )
    , bDotationSupplyNeeded_             ( false )
    , bDotationSupplyExplicitlyRequested_( false )
    , nTickRcDotationSupplyQuerySent_    ( 0 )
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
    , bDotationSupplyNeeded_             ( false )
    , bDotationSupplyExplicitlyRequested_( false )
    , nTickRcDotationSupplyQuerySent_    ( 0 )
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

// =============================================================================
// CHEKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::serialize
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> pAutomate_
         >> bDotationSupplyNeeded_
         >> bDotationSupplyExplicitlyRequested_
         >> nTickRcDotationSupplyQuerySent_;

    // $$ TMP
    assert( pAutomate_ );
    supplyRequestBuilder_.reset( new logistic::SupplyDotationRequestBuilder( *pAutomate_, *this ) );
    supplyRequests_.reset( new logistic::SupplyRequestContainer( supplyRequestBuilder_ ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::serialize
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << pAutomate_
         << bDotationSupplyNeeded_
         << bDotationSupplyExplicitlyRequested_
         << nTickRcDotationSupplyQuerySent_;
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
    if( bDotationSupplyNeeded_ && pAutomate_->GetLogisticHierarchy().HasSuperior() )
    {
        logistic::SupplyRequestHierarchyDispatcher dispatcher( pAutomate_->GetLogisticHierarchy(), bDotationSupplyExplicitlyRequested_ );
        bDotationSupplyNeeded_ = !supplyRequests_->Execute( dispatcher );
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
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::NotifyDotationSupplyNeeded
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::NotifyDotationSupplyNeeded( const PHY_DotationCategory& dotationCategory )
{
    if( bDotationSupplyNeeded_ || IsSupplyInProgress( dotationCategory ) )
        return;
    bDotationSupplyNeeded_ = true;

    // Pas de RC si RC envoyé au tick précédent
    const unsigned int nCurrentTick = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( ( nCurrentTick > ( nTickRcDotationSupplyQuerySent_ + 1 ) || nTickRcDotationSupplyQuerySent_ == 0 ) )
        MIL_Report::PostEvent( *pAutomate_, MIL_Report::eRC_DemandeRavitaillementDotations );
    nTickRcDotationSupplyQuerySent_ = nCurrentTick;
}

// -----------------------------------------------------------------------------
// Name: MIL_DotationSupplyManager::RequestDotationSupply
// Created: NLD 2005-03-03
// -----------------------------------------------------------------------------
void MIL_DotationSupplyManager::RequestDotationSupply()
{
    bDotationSupplyNeeded_              = true;
    bDotationSupplyExplicitlyRequested_ = true;
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
// Name: MIL_StockSupplyManager::OnSupplyScheduled
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
    bDotationSupplyNeeded_ = true;
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

// =============================================================================
// Network
// =============================================================================

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
