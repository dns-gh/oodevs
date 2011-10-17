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
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Logistic/SupplyRequestContainer.h"
#include "Entities/Agents/Roles/Logistic/SupplyStockRequestBuilder.h"
#include "Entities/Agents/Roles/Logistic/SupplyRequestHierarchyDispatcher.h"
#include "Entities/Agents/Roles/Logistic/SupplyStockPullFlowRequestBuilder.h"
#include "Entities/Agents/Roles/Logistic/SupplyRequestManualDispatcher.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/MIL_Formation.h"
#include "protocol/ClientSenders.h"
#include "protocol/SimulationSenders.h"
#include "Network/NET_AsnException.h"
#include <boost/serialization/set.hpp>
#include <boost/foreach.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_StockSupplyManager )
using namespace sword;
using namespace sword;

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager constructor
// Created: NLD 2004-12-21
// -----------------------------------------------------------------------------
MIL_StockSupplyManager::MIL_StockSupplyManager( MIL_Automate& automate )
    : pAutomate_                  ( &automate )
    , bStockSupplyNeeded_         ( false )
    , supplyRequestBuilder_       ( new logistic::SupplyStockRequestBuilder( automate, *this ) )
    , autoSupplyRequest_          ( new logistic::SupplyRequestContainer( supplyRequestBuilder_ ) )
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
// Name: MIL_StockSupplyManager::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> pAutomate_
         >> bStockSupplyNeeded_
         >> nTickRcStockSupplyQuerySent_;

    // $$ TMP
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
    file << pAutomate_
         << bStockSupplyNeeded_
         << nTickRcStockSupplyQuerySent_;
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
    autoSupplyRequest_->Update();
    for( T_SupplyRequests::iterator it = manualSupplyRequests_.begin(); it != manualSupplyRequests_.end(); )
        (*it)->Update() ? it = manualSupplyRequests_.erase( it ) : ++it;

    if( !bStockSupplyNeeded_ )
        return;

    MIL_AutomateLOG* logisticManager = pAutomate_->FindLogisticManager();
    if( logisticManager && logisticManager->GetLogisticHierarchy().HasSuperior() )
    {
        logistic::SupplyRequestHierarchyDispatcher dispatcher( logisticManager->GetLogisticHierarchy() );
        bStockSupplyNeeded_ = !autoSupplyRequest_->Execute( dispatcher );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::Clean
// Created: NLD 2005-01-31
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::Clean()
{
    autoSupplyRequest_->Clean();
    BOOST_FOREACH( T_SupplyRequests::value_type& data, manualSupplyRequests_ )
        data->Clean();
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::IsSupplyInProgress
// Created: NLD 2005-12-14
// -----------------------------------------------------------------------------
bool MIL_StockSupplyManager::IsSupplyInProgress( const PHY_DotationCategory& dotationCategory ) const
{
    if( autoSupplyRequest_->IsSupplying( dotationCategory ) )
        return true;
    BOOST_FOREACH( const T_SupplyRequests::value_type& data, manualSupplyRequests_ )
        if( data->IsSupplying( dotationCategory ) )
            return true;
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

    // Pas de RC si RC envoyé au tick précédent
    const unsigned int nCurrentTick = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    if( nCurrentTick > ( nTickRcStockSupplyQuerySent_ + 1 ) || nTickRcStockSupplyQuerySent_ == 0 )
        MIL_Report::PostEvent( *pAutomate_, MIL_Report::eReport_StockSupplyRequest );
    nTickRcStockSupplyQuerySent_ = nCurrentTick;
}

// =============================================================================
// SupplyRecipient_ABC
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::GetPosition
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
const MT_Vector2D& MIL_StockSupplyManager::GetPosition() const
{
    return pAutomate_->GetPionPC().GetRole< PHY_RoleInterface_Location >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::GetPC
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
const MIL_AgentPion& MIL_StockSupplyManager::GetPC() const
{
    return pAutomate_->GetPionPC();
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::OnSupplyCanceled
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::OnSupplyCanceled()
{
    MIL_Report::PostEvent( *pAutomate_, MIL_Report::eReport_StockSupplyCanceled );
    bStockSupplyNeeded_ = true; //$$ ..
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::OnSupplyDone
// Created: NLD 2005-01-25
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::OnSupplyDone()
{
    MIL_Report::PostEvent( *pAutomate_, MIL_Report::eReport_StockSupplyDone );
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::OnSupplyConvoyArriving
// Created: NLD 2011-09-13
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::OnSupplyConvoyArriving( boost::shared_ptr< const logistic::SupplyConsign_ABC > supplyConsign )
{
    MIL_AutomateLOG* logisticBase = pAutomate_->FindLogisticManager();
    if( logisticBase )
        logisticBase->OnSupplyConvoyArriving( supplyConsign );
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::OnSupplyConvoyLeaving
// Created: NLD 2011-09-13
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::OnSupplyConvoyLeaving( boost::shared_ptr< const logistic::SupplyConsign_ABC > supplyConsign )
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

// =============================================================================
// Network
// =============================================================================

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
}

// -----------------------------------------------------------------------------
// Name: MIL_Automate::SendChangedState
// Created: MIL_StockSupplyManager 2005-01-25
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::SendChangedState() const
{
    autoSupplyRequest_->SendChangedState();
    BOOST_FOREACH( const T_SupplyRequests::value_type& data, manualSupplyRequests_ )
        data->SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: MIL_StockSupplyManager::SendFullState
// Created: NLD 2005-02-01
// -----------------------------------------------------------------------------
void MIL_StockSupplyManager::SendFullState() const
{
    autoSupplyRequest_->SendFullState();
    BOOST_FOREACH( const T_SupplyRequests::value_type& data, manualSupplyRequests_ )
        data->SendFullState();
}
