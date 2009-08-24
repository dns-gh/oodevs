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

#include "simulation_kernel_pch.h"
#include "PHY_SmokeData.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Dotations/PHY_RoleInterface_Dotations.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"

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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SmokeData destructor
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
PHY_SmokeData::~PHY_SmokeData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_SmokeData::operator()
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void PHY_SmokeData::operator()( const PHY_ComposantePion& compFirer, PHY_Weapon& weapon )
{
    if( pWeapon_ || !compFirer.CanFire() || !weapon.CanIndirectFire() )
        return;
    const PHY_DotationCategory_IndirectFire_ABC* pIndirectFireData = weapon.GetDotationCategory().GetIndirectFireData();
    if( !pIndirectFireData || pIndirectFireData->GetIndirectFireDotationCategory() != indirectWeaponCategory_ )
        return;
    if( firer_.GetRole< PHY_RoleInterface_Dotations >().GetDotationValue( weapon.GetDotationCategory() ) < nNbrAmmo_ )
        return;
    pWeapon_ = &weapon;
}

// -----------------------------------------------------------------------------
// Name: PHY_SmokeData::GetWeapon
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
PHY_Weapon* PHY_SmokeData::GetWeapon() const
{
    return pWeapon_;
}
