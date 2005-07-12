// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_SmokeData.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 2 $
// $Workfile: PHY_SmokeData.cpp $
//
// *****************************************************************************

#include "MIL_Pch.h"

#include "PHY_SmokeData.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_SmokeData constructor
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
PHY_SmokeData::PHY_SmokeData( const MIL_AgentPion& firer, const PHY_IndirectFireDotationClass& indirectWeaponCategory, uint nNbrAmmo )
    : firer_                 ( firer )
    , indirectWeaponCategory_( indirectWeaponCategory )
    , nNbrAmmo_              ( nNbrAmmo )
    , pWeapon_               ( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: PHY_SmokeData destructor
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
PHY_SmokeData::~PHY_SmokeData()
{

}

// -----------------------------------------------------------------------------
// Name: PHY_SmokeData::AddWeapon
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
bool PHY_SmokeData::AddWeapon( PHY_Weapon& weapon )
{
    assert( weapon.CanIndirectFire() );
    if( pWeapon_ )
        return true;

    const PHY_DotationCategory_IndirectFire_ABC* pIndirectFireData = weapon.GetDotationCategory().GetIndirectFireData();
    if( !pIndirectFireData || pIndirectFireData->GetIndirectFireDotationCategory() != indirectWeaponCategory_ )
        return false;

    if( firer_.GetRole< PHY_RolePion_Dotations >().GetDotationValue( weapon.GetDotationCategory() ) < nNbrAmmo_ )
        return false;
    
    pWeapon_ = &weapon;
    return true;
}
