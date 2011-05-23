// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_StockConvoy.cpp $
// $Author: Nld $
// $Modtime: 14/04/05 10:09 $
// $Revision: 6 $
// $Workfile: PHY_StockConvoy.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_StockConvoy.h"
#include "PHY_SupplyStockConsign.h"
#include "PHY_Conveyor.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include <xeumeuleu/xml.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_StockConvoy )

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
    // NOTHING
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy destructor
// Created: NLD 2005-01-24
// -----------------------------------------------------------------------------
PHY_StockConvoy::~PHY_StockConvoy()
{
    if( pPionConvoy_ )
    {
        pPionConvoy_->GetOrderManager().CancelMission();
        for( CIT_ConveyorMap it = conveyors_.begin(); it != conveyors_.end(); ++it )
            it->second->UndoLend();
        pPionConvoy_->GetRole< PHY_RoleInterface_Supply >().UnassignConvoy( *this );
        pPionConvoy_->GetAutomate().DestroyPion( *pPionConvoy_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void PHY_StockConvoy::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< PHY_Convoy_ABC >( *this )
         >> pConsign_
         >> pPionConvoy_
         >> bMissionActivated_;

    if( bMissionActivated_ )
        ActivateConvoyMission();
}

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void PHY_StockConvoy::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< PHY_Convoy_ABC >( *this )
         << pConsign_
         << pPionConvoy_
         << bMissionActivated_;
}

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::Form
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
bool PHY_StockConvoy::Form()
{
    assert( pConvoyAgentType_ );
    assert( !pPionConvoy_ );
    assert( pConsign_ );
    assert( pConsign_->GetConvoyer().GetPC() );

    pPionConvoy_ = pConsign_->GetConvoyer().SupplyCreatePionConvoy( *pConvoyAgentType_ );
    if(!pPionConvoy_)
        return false;
    pPionConvoy_->GetRole< PHY_RoleInterface_Supply >().AssignConvoy( *this );
    for( CIT_ConveyorMap it = conveyors_.begin(); it != conveyors_.end(); ++it )
        it->second->LendTo( *pPionConvoy_ );
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::ActivateConvoyMission
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_StockConvoy::ActivateConvoyMission()
{
    assert( pConvoyMissionType_ );
    assert( pPionConvoy_ );
    pPionConvoy_->GetOrderManager().OnReceiveMission( *pConvoyMissionType_ );
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
// Name: PHY_StockConvoy::Load
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
bool PHY_StockConvoy::Load()
{
    assert( pConsign_ );
    return pConsign_->ConvoyLoad();
}

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::Unload
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
bool PHY_StockConvoy::Unload()
{
    assert( pConsign_ );
    return pConsign_->ConvoyUnload();
}

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::IsLoadingDone
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
bool PHY_StockConvoy::IsLoadingDone() const
{
    assert( pConsign_ );
    return pConsign_->IsLoadingDone();
}

// -----------------------------------------------------------------------------
// Name: PHY_StockConvoy::IsUnloadingDone
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
bool PHY_StockConvoy::IsUnloadingDone() const
{
    assert( pConsign_ );
    return pConsign_->IsUnloadingDone();
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
// Name: PHY_StockConvoy::GetPionConvoy
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
const MIL_AgentPion* PHY_StockConvoy::GetPionConvoy() const
{
    return pPionConvoy_;
}
