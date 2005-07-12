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

#include "MIL_Pch.h"

#include "PHY_IndirectFireData.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::sComposanteWeapons::sComposanteWeapons
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
PHY_IndirectFireData::sComposanteWeapons::sComposanteWeapons()
    : bIsFiring_   ( false )
    , weaponsReady_()
{

}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::~sComposanteWeapons
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
PHY_IndirectFireData::sComposanteWeapons::~sComposanteWeapons()
{

}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::sComposanteWeapons::AddWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
inline
void PHY_IndirectFireData::sComposanteWeapons::AddWeapon( PHY_Weapon& weapon )
{
    weapons_.push_back( &weapon );
    if( weapon.IsReady() )
        weaponsReady_.push_back( &weapon );
    else
        bIsFiring_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::PreselectWeapons
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void PHY_IndirectFireData::sComposanteWeapons::PreselectWeapons( T_WeaponsDotationsSet& weaponsDotations, const MT_Float rDist ) const
{
    for ( CIT_WeaponVector itWeapon = weapons_.begin(); itWeapon != weapons_.end(); ++itWeapon )
        if ( (*itWeapon)->GetMinRangeToIndirectFire() <= rDist 
          && (*itWeapon)->GetMaxRangeToIndirectFire() > rDist )
            weaponsDotations.insert( &(*itWeapon)->GetDotationCategory() );
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::RemoveWeaponsNotUsingDotation
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void PHY_IndirectFireData::sComposanteWeapons::RemoveWeaponsNotUsingDotation( const PHY_DotationCategory& dotationCategory )
{
    for( IT_WeaponVector itWeapon = weaponsReady_.begin(); itWeapon != weaponsReady_.end(); )
    {   
        const PHY_Weapon& weapon = **itWeapon;
        if( weapon.GetDotationCategory() == dotationCategory )
            ++itWeapon;
        else
            itWeapon = weaponsReady_.erase( itWeapon );
    }
}

// =============================================================================
// 
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData constructor
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
PHY_IndirectFireData::PHY_IndirectFireData( MIL_AgentPion& firer, MIL_Effect_IndirectFire& effect )
    : bHasWeaponsReady_     ( false )
    , bHasWeaponsNotReady_  ( false )
    , firer_                ( firer )
    , effect_               ( effect )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData destructor
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
PHY_IndirectFireData::~PHY_IndirectFireData()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::AddWeapon
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void PHY_IndirectFireData::AddWeapon( PHY_ComposantePion& firer, PHY_Weapon& weapon )
{
    if( effect_.CanWeaponBeUsed( weapon ) )
    {
        sComposanteWeapons& data = composantesWeapons_[ &firer ];
        data.AddWeapon( weapon );
        bHasWeaponsReady_    |= data.HasWeaponsReady   ();
        bHasWeaponsNotReady_ |= data.IsFiring          ();
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::RemoveFirer
// Created: NLD 2004-10-05
// -----------------------------------------------------------------------------
void PHY_IndirectFireData::RemoveFirer( PHY_ComposantePion& firer )
{
    int nOut = composantesWeapons_.erase( &firer );
    assert( nOut == 1 );
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::PreselectWeapons
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void PHY_IndirectFireData::PreselectWeapons()
{
    if( effect_.GetWeaponDotationCategory() )
        return;
    
    T_WeaponsDotationsSet weaponsDotations;
    for( CIT_ComposanteWeaponsMap it = composantesWeapons_.begin(); it != composantesWeapons_.end(); ++it )
        it->second.PreselectWeapons( weaponsDotations, effect_.GetFlyingDistance() );

    const PHY_RolePion_Dotations& roleDotations            = firer_.GetRole< PHY_RolePion_Dotations >();
    const PHY_DotationCategory*   pChoosenDotationCategory = 0;
          MT_Float                rNbrDotations            = 0;
          
    for ( CIT_WeaponsDotationsSet it = weaponsDotations.begin(); it != weaponsDotations.end(); ++it )
    {
        const MT_Float rNbr = roleDotations.GetDotationValue( **it );
        if( rNbr > rNbrDotations )
        {
            rNbrDotations            = rNbr;
            pChoosenDotationCategory = *it;
        }
    }

    if( pChoosenDotationCategory )
    {
        for( IT_ComposanteWeaponsMap it = composantesWeapons_.begin(); it != composantesWeapons_.end(); ++it )
            it->second.RemoveWeaponsNotUsingDotation( *pChoosenDotationCategory );
        effect_.SetWeaponDotationCategory( *pChoosenDotationCategory );
    }
    else
        composantesWeapons_.clear();
}

// -----------------------------------------------------------------------------
// Name: PHY_IndirectFireData::GetUnusedFirerWeapon
// Created: NLD 2004-10-06
// -----------------------------------------------------------------------------
bool PHY_IndirectFireData::GetUnusedFirerWeapon( PHY_ComposantePion*& pUnusedFirer, PHY_Weapon*& pUnusedFirerWeapon ) const
{
    pUnusedFirerWeapon = 0;
    pUnusedFirer       = 0;
    if( composantesWeapons_.empty() )
        return false;

    CIT_ComposanteWeaponsMap it = composantesWeapons_.begin();
    pUnusedFirerWeapon = it->second.GetUnusedWeapon();
    if( !pUnusedFirerWeapon )
        return false;
    pUnusedFirer = it->first;
    return true;
}
