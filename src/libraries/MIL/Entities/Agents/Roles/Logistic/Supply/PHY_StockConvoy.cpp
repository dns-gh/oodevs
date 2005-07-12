// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_StockConvoy.cpp $
// $Author: Nld $
// $Modtime: 14/04/05 10:09 $
// $Revision: 6 $
// $Workfile: PHY_StockConvoy.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_StockConvoy.h"
#include "PHY_SupplyStockConsign.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Logistic/Supply/PHY_RolePion_Supply.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Orders/Pion/MIL_PionMissionType.h"
#include "Entities/Orders/Pion/MIL_PionMission_ABC.h"

BOOST_CLASS_EXPORT_GUID( PHY_StockConvoy, "PHY_StockConvoy" )

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy constructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_StockConvoy::PHY_StockConvoy( PHY_SupplyStockConsign& consign )
    : PHY_Convoy_ABC    ( consign )
    , pConsign_         ( &consign )
    , pPionConvoy_      ( 0 )
    , bMissionActivated_( false )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy constructor
// Created: JVT 2005-03-31
// -----------------------------------------------------------------------------
PHY_StockConvoy::PHY_StockConvoy()
    : PHY_Convoy_ABC    ()
    , pConsign_         ( 0 )
    , pPionConvoy_      ( 0 )
    , bMissionActivated_( false )
{
}
    
// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy destructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_StockConvoy::~PHY_StockConvoy()
{
    if( pPionConvoy_ )
    {
        pPionConvoy_->GetOrderManager().CancelAllOrders();

        for( CIT_TransporterVector it = transporters_.begin(); it != transporters_.end(); ++it )
            it->pTransporterPion_->GetRole< PHY_RolePion_Composantes >().UndoLendComposante( pPionConvoy_->GetRole< PHY_RolePion_Composantes >(), *it->pTransporter_ );

        assert( pCommander_ );
        assert( pCommanderPion_ );
        pCommanderPion_->GetRole< PHY_RolePion_Composantes >().UndoLendComposante( pPionConvoy_->GetRole< PHY_RolePion_Composantes >(), *pCommander_ );

        pPionConvoy_->GetRole< PHY_RolePion_Supply >().UnassignConvoy( *this );
        MIL_AgentServer::GetWorkspace().GetEntityManager().DestroyPion( *pPionConvoy_ );

        transporters_.clear();
        pCommander_     = 0;
        pCommanderPion_ = 0;
    }
}

// =============================================================================
// CHECKPOINTS
// =============================================================================


// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void PHY_StockConvoy::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< PHY_Convoy_ABC >( *this )
         >> pConsign_
         >> pPionConvoy_
         >> bMissionActivated_;

    if( bMissionActivated_ )
        ActivateConvoyMission();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentPionLOGConvoy::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void PHY_StockConvoy::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << boost::serialization::base_object< PHY_Convoy_ABC >( *this )
         << pConsign_
         << pPionConvoy_
         << bMissionActivated_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::GetLoadingTime
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
uint PHY_StockConvoy::GetLoadingTime() const
{
    uint nLoadingTime = 0;

    for ( CIT_TransporterVector it = transporters_.begin(); it != transporters_.end(); ++it )
        nLoadingTime = std::max( nLoadingTime, it->pTransporter_->GetConvoyTransporterLoadingTime() );

    return nLoadingTime;
}

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::GetUnloadingTime
// Created: NLD 2005-01-27
// -----------------------------------------------------------------------------
uint PHY_StockConvoy::GetUnloadingTime() const
{
    uint nUnloadingTime = 0;
    
    for ( CIT_TransporterVector it = transporters_.begin(); it != transporters_.end(); ++it )
        nUnloadingTime = std::max( nUnloadingTime, it->pTransporter_->GetConvoyTransporterUnloadingTime() );

    return nUnloadingTime;
}

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::Form
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
bool PHY_StockConvoy::Form()
{
    if( !PHY_Convoy_ABC::Form() )
        return false;

    assert( pConvoyAgentType_ );
    assert( !pPionConvoy_ );
    assert( pConsign_ );
    
    pPionConvoy_ = &MIL_AgentServer::GetWorkspace().GetEntityManager().CreatePion( *pConvoyAgentType_, pConsign_->GetSupplyingAutomate(), pConsign_->GetSupplyingAutomate().GetPionPC().GetRole< PHY_RolePion_Location >().GetPosition() );
    pPionConvoy_->GetRole< PHY_RolePion_Supply >().AssignConvoy( *this );
    for( CIT_TransporterVector it = transporters_.begin(); it != transporters_.end(); ++it )
    {
        it->pTransporterPion_->GetRole< PHY_RolePion_Supply      >().StopUsingForLogistic( *it->pTransporter_ );
        it->pTransporterPion_->GetRole< PHY_RolePion_Composantes >().LendComposante      ( pPionConvoy_->GetRole< PHY_RolePion_Composantes >(), *it->pTransporter_ );
    }
    assert( pCommander_ );
    pCommanderPion_->GetRole< PHY_RolePion_Supply      >().StopUsingForLogistic( *pCommander_ );
    pCommanderPion_->GetRole< PHY_RolePion_Composantes >().LendComposante      ( pPionConvoy_->GetRole< PHY_RolePion_Composantes >(), *pCommander_ );
    return true;
}

// =============================================================================
// REAL CONVOY OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::ActivateConvoyMission
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_StockConvoy::ActivateConvoyMission()
{
    assert( pConvoyMissionType_ );
    assert( pPionConvoy_ );

    MIL_PionMission_ABC& mission = pConvoyMissionType_->InstanciateMission( *pPionConvoy_ );
    mission.Initialize(); // $$$$$ Initialize peut renvoyer false $$$$$$ <= checker que pions convois ont la mission à l'init et que la mission ne prend aucun paramètre
    mission.Prepare   ();
    pPionConvoy_->GetOrderManager().OnReceivePionOrder( mission );
    bMissionActivated_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::DesactivateConvoyMission
// Created: NLD 2005-04-14
// -----------------------------------------------------------------------------
void PHY_StockConvoy::DesactivateConvoyMission()
{
    bMissionActivated_ = false;
}

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::Supply
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
bool PHY_StockConvoy::Supply()
{
    assert( pConsign_ );
    return pConsign_->ConvoySupply();
}

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::EndMission
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_StockConvoy::EndMission()
{
    assert( pConsign_ );
    return pConsign_->ConvoyEndMission();
}

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::IsSupplyDone
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
bool PHY_StockConvoy::IsSupplyDone() const
{
    assert( pConsign_ );
    return pConsign_->IsSupplyDone();
}

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::NotifyComposanteChanged
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_StockConvoy::NotifyComposanteChanged( PHY_ComposantePion& composante )
{
    CIT_TransporterVector itTransporter;
    for( itTransporter = transporters_.begin(); itTransporter != transporters_.end(); ++itTransporter )
    {
        if( itTransporter->pTransporter_ == &composante )
            break;
    }

    if( itTransporter == transporters_.end() )
        return;

    assert( itTransporter->pTransporter_ );
    assert( pConsign_ );
    pConsign_->RemoveConvoyedStock( itTransporter->pTransporter_->GetConvoyTransporterUCapacity() );
}
 
