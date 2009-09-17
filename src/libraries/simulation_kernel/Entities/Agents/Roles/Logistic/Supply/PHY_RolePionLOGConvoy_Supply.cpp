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

#include "simulation_kernel_pch.h"
#include "PHY_RolePionLOGConvoy_Supply.h"
#include "PHY_StockConvoy.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Automates/MIL_Automate.h"

BOOST_CLASS_EXPORT_GUID( PHY_RolePionLOGConvoy_Supply, "PHY_RolePionLOGConvoy_Supply" )

template< typename Archive >
void save_construct_data( Archive& archive, const PHY_RolePionLOGConvoy_Supply* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* const pion = &role->pion_;
    archive << pion;
}

template< typename Archive >
void load_construct_data( Archive& archive, PHY_RolePionLOGConvoy_Supply* role, const unsigned int /*version*/ )
{
	MIL_AgentPion* pion;
    archive >> pion;
    ::new( role )PHY_RolePionLOGConvoy_Supply( *pion );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply constructor
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
PHY_RolePionLOGConvoy_Supply::PHY_RolePionLOGConvoy_Supply( MIL_AgentPion& pion )
    : pion_             ( pion )
    , pConvoy_           ( 0 )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply destructor
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
PHY_RolePionLOGConvoy_Supply::~PHY_RolePionLOGConvoy_Supply()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::serialize
// Created: JVT 2005-03-30
// -----------------------------------------------------------------------------
template< typename Archive >
void PHY_RolePionLOGConvoy_Supply::serialize( Archive& file, const uint )
{
    file & boost::serialization::base_object< PHY_RoleInterface_Supply >( *this )
		 & pConvoy_;
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::Update
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::Update( bool bIsDead )
{ 
    if( bIsDead )
        pion_.GetRole< PHY_RoleInterface_Location >().Follow( pion_.GetAutomate().GetPionPC() );
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::Clean
// Created: NLD 2005-02-09
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::Clean()
{
    // NOTHING
}

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
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyLoad
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
bool PHY_RolePionLOGConvoy_Supply::ConvoyLoad() const
{
    if( !pConvoy_ )
        return true;
    return pConvoy_->Load();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyUnload
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
bool PHY_RolePionLOGConvoy_Supply::ConvoyUnload() const
{
    if( !pConvoy_ )
        return true;
    return pConvoy_->Unload();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyIsLoadingDone
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
bool PHY_RolePionLOGConvoy_Supply::ConvoyIsLoadingDone() const
{
    if( !pConvoy_ )
        return true;    
    return pConvoy_->IsLoadingDone();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyIsUnloadingDone
// Created: NLD 2005-12-16
// -----------------------------------------------------------------------------
bool PHY_RolePionLOGConvoy_Supply::ConvoyIsUnloadingDone() const
{
    if( !pConvoy_ )
        return true;
    return pConvoy_->IsUnloadingDone();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyGetSupplyingAutomate
// Created: NLD 2006-07-31
// -----------------------------------------------------------------------------
const MIL_AutomateLOG* PHY_RolePionLOGConvoy_Supply::ConvoyGetSupplyingAutomate() const
{
    if( !pConvoy_ )
        return 0;
    return &pConvoy_->GetSupplyingAutomate();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyGetConvoyingAutomate
// Created: NLD 2006-07-31
// -----------------------------------------------------------------------------
const MIL_AutomateLOG* PHY_RolePionLOGConvoy_Supply::ConvoyGetConvoyingAutomate() const
{
    if( !pConvoy_ )
        return 0;
    return &pConvoy_->GetConvoyingAutomate();
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ConvoyGetSuppliedAutomate
// Created: NLD 2006-07-31
// -----------------------------------------------------------------------------
const MIL_Automate* PHY_RolePionLOGConvoy_Supply::ConvoyGetSuppliedAutomate() const
{
    if( !pConvoy_ )
        return 0;
    return &pConvoy_->GetSuppliedAutomate();
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

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::ModifySpeed
// Created: NLD 2007-02-05
// -----------------------------------------------------------------------------
MT_Float PHY_RolePionLOGConvoy_Supply::ModifySpeed( MT_Float rSpeed ) const
{
    if( !pConvoy_ )
        return rSpeed;
    return pConvoy_->ModifySpeed( rSpeed );
}

// -----------------------------------------------------------------------------
// Name: PHY_RolePionLOGConvoy_Supply::NotifyComposanteChanged
// Created: NLD 2005-02-10
// -----------------------------------------------------------------------------
void PHY_RolePionLOGConvoy_Supply::NotifyComposanteChanged( PHY_ComposantePion& composante )
{
    if( !pConvoy_ || composante.GetState() != PHY_ComposanteState::dead_ )
        return;

    pConvoy_->NotifyConveyorDestroyed( composante );
}
