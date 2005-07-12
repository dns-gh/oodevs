// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Supply/PHY_RolePionLOGConvoy_Supply.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 11:48 $
// $Revision: 3 $
// $Workfile: PHY_RolePionLOGConvoy_Supply.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_RolePionLOGConvoy_Supply.h"
#include "PHY_StockConvoy.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePionLOGConvoy_Supply, "PHY_RolePionLOGConvoy_Supply" )

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply constructor
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
PHY_RolePionLOGConvoy_Supply::PHY_RolePionLOGConvoy_Supply( MT_RoleContainer& role, const MIL_AgentPion& pion )
    : PHY_RolePion_Supply( role )
    , pPion_             ( &pion )
    , pConvoy_           ( 0 )
{

}


// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply constructor
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
PHY_RolePionLOGConvoy_Supply::PHY_RolePionLOGConvoy_Supply()
    : PHY_RolePion_Supply()
    , pPion_             ( 0 )
    , pConvoy_           ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply destructor
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
PHY_RolePionLOGConvoy_Supply::~PHY_RolePionLOGConvoy_Supply()
{
}

// =============================================================================
// CHECKPOINTS
// =============================================================================
// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePionLOGConvoy_Supply::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RolePion_Supply >( *this )
         & const_cast< MIL_AgentPion*& >( pPion_ )
         & pConvoy_;
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::Update
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::Update( bool bIsDead )
{
    assert( pPion_ );
    
    if ( bIsDead )
        GetRole< PHY_RolePion_Location >().Follow( pPion_->GetAutomate().GetPionPC() );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::Clean
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::Clean()
{

}

// =============================================================================
// CONVOY
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::AssignConvoy
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::AssignConvoy( PHY_StockConvoy& convoy )
{
    assert( !pConvoy_ );
    pConvoy_ = &convoy;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::UnassignConvoy
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::UnassignConvoy( PHY_StockConvoy& convoy )
{
    assert( pConvoy_ == &convoy );
    pConvoy_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoySupply
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
bool PHY_RolePionLOGConvoy_Supply::ConvoySupply() const
{
    if( !pConvoy_ )
        return true;

    return pConvoy_->Supply();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyIsSupplyDone
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
bool PHY_RolePionLOGConvoy_Supply::ConvoyIsSupplyDone() const
{
    if( !pConvoy_ )
        return true;
    
    return pConvoy_->IsSupplyDone();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyGetSuppliedAutomate
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
const MIL_Automate* PHY_RolePionLOGConvoy_Supply::ConvoyGetSuppliedAutomate() const
{
    if( !pConvoy_ )
        return 0;

    return pConvoy_->GetSuppliedAutomate();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyEndMission
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::ConvoyEndMission()
{
    if( pConvoy_ )
        pConvoy_->EndMission();
}

// =============================================================================
// EVENTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::NotifyComposanteChanged
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::NotifyComposanteChanged( PHY_ComposantePion& composante )
{
    if( !pConvoy_ || composante.GetState() != PHY_ComposanteState::dead_ )
        return;

    pConvoy_->NotifyComposanteChanged( composante );
}
