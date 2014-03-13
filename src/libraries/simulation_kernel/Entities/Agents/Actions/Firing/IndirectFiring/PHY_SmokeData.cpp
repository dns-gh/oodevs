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
#include "DefaultDotationComputer.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Weapons/PHY_WeaponType.h"

using namespace firing;

// -----------------------------------------------------------------------------
// Name: PHY_SmokeData constructor
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
PHY_SmokeData::PHY_SmokeData( MIL_Agent_ABC& firer, const PHY_IndirectFireDotationClass& indirectWeaponCategory )
    : firer_( firer )
    , indirectWeaponCategory_( indirectWeaponCategory )
    , pWeapon_( 0 )
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

    const PHY_DotationCategory& category = weapon.GetDotationCategory();
    if( !category.CanBeUsedForIndirectFire() || !category.HasIndirectWeaponCategory( indirectWeaponCategory_ ) )
        return;

    dotation::DefaultDotationComputer dotationComputer;
    firer_.Execute< dotation::DotationComputer_ABC >( dotationComputer );
    if( dotationComputer.GetDotationValue( weapon.GetDotationCategory() ) < weapon.GetType().GetNbrAmmoPerBurst() )
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
