// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_IndirectFireData.cpp $
// $Author: Jvt $
// $Modtime: 2/05/05 17:35 $
// $Revision: 3 $
// $Workfile: PHY_IndirectFireData.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_IndirectFireData.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"
#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/DefaultDotationComputer.h"
#include "MT_Tools/MT_Logger.h"

using namespace firing;

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::sComposanteWeapons::sComposanteWeapons
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
PHY_IndirectFireData::sComposanteWeapons::sComposanteWeapons()
    : bIsFiring_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::~sComposanteWeapons
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
PHY_IndirectFireData::sComposanteWeapons::~sComposanteWeapons()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::sComposanteWeapons::AddWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void PHY_IndirectFireData::sComposanteWeapons::AddWeapon( PHY_Weapon& weapon )
{
    weapons_.push_back( &weapon );
    if( weapon.IsReady() )
        weaponsReady_.push_back( &weapon );
    else
        bIsFiring_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData constructor
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
PHY_IndirectFireData::PHY_IndirectFireData( MIL_Agent_ABC& firer, MIL_Effect_IndirectFire& effect )
    : bHasWeaponsReady_     ( false )
    , bHasWeaponsNotReady_  ( false )
    , bHasWeaponsAndNoAmmo_ ( false )
    , firer_                ( firer )
    , effect_               ( effect )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData destructor
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
PHY_IndirectFireData::~PHY_IndirectFireData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::AddWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void PHY_IndirectFireData::operator() ( const PHY_ComposantePion& compFirer, PHY_Weapon& weapon )
{
    if( !compFirer.CanFire() || !effect_.CanWeaponBeUsed( weapon ) )
        return;
    dotation::DefaultDotationComputer dotationComputer;
    firer_.Execute< dotation::DotationComputer_ABC >( dotationComputer );
    if( !dotationComputer.HasDotation( weapon.GetDotationCategory() ) )
        bHasWeaponsAndNoAmmo_ = true;
    else
    {
        sComposanteWeapons& data = composantesWeapons_[ &compFirer ];
        data.AddWeapon( weapon );
        bHasWeaponsReady_    |= data.HasWeaponsReady   ();
        bHasWeaponsNotReady_ |= data.IsFiring          ();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::GetUnusedFirerWeapon
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
bool PHY_IndirectFireData::GetUnusedFirerWeapon( const PHY_ComposantePion*& pUnusedFirer, PHY_Weapon*& pUnusedFirerWeapon ) const
{
    pUnusedFirerWeapon = 0;
    pUnusedFirer       = 0;
    if( composantesWeapons_.empty() )
        return false;
    auto it = composantesWeapons_.begin();
    pUnusedFirerWeapon = it->second.GetUnusedWeapon();
    if( !pUnusedFirerWeapon )
        return false;
    pUnusedFirer = it->first;
    return true;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::HasWeaponsAndNoAmmo
// Created: NLD 2004-11-30
// -----------------------------------------------------------------------------
bool PHY_IndirectFireData::HasWeaponsAndNoAmmo() const
{
    return bHasWeaponsAndNoAmmo_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::HasWeaponsReady
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
bool PHY_IndirectFireData::HasWeaponsReady() const
{
    return bHasWeaponsReady_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::HasWeaponsNotReady
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
bool PHY_IndirectFireData::HasWeaponsNotReady() const
{
    return bHasWeaponsNotReady_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::sComposanteWeapons::HasWeaponsNotReady
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
bool PHY_IndirectFireData::sComposanteWeapons::HasWeaponsReady() const
{
    return !weaponsReady_.empty();
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::IsFiring
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
bool PHY_IndirectFireData::sComposanteWeapons::IsFiring() const
{
    return bIsFiring_;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::sComposanteWeapons::GetUnusedWeapon
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
PHY_Weapon* PHY_IndirectFireData::sComposanteWeapons::GetUnusedWeapon() const
{
    if( weaponsReady_.empty() )
        return 0;
    return weaponsReady_.front();
}
// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::RemoveFirer
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void PHY_IndirectFireData::RemoveFirer( const PHY_ComposantePion& firer )
{
    if( composantesWeapons_.erase( &firer ) != 1 )
        MT_LOG_ERROR_MSG( __FUNCTION__ << " : Erase failed" );
}
