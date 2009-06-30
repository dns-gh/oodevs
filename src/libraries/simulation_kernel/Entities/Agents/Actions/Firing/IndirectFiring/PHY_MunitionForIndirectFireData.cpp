// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_MunitionForIndirectFireData.cpp $
// $Author: Nld $
// $Modtime: 17/03/05 14:53 $
// $Revision: 2 $
// $Workfile: PHY_MunitionForIndirectFireData.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MunitionForIndirectFireData.h"
#include "Entities/Agents/Units/Weapons/PHY_Weapon.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentPion.h"

// -----------------------------------------------------------------------------
// Name: PHY_MunitionForIndirectFireData constructor
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
PHY_MunitionForIndirectFireData::PHY_MunitionForIndirectFireData( const MIL_AgentPion& firer, const PHY_IndirectFireDotationClass& indirectWeaponCategory, MT_Float rRange )
    : firer_                 ( firer )
    , indirectWeaponCategory_( indirectWeaponCategory )
    , rRange_                ( rRange )
    , pChoosenMunition_      ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MunitionForIndirectFireData destructor
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
PHY_MunitionForIndirectFireData::~PHY_MunitionForIndirectFireData()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MunitionForIndirectFireData::operator()
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void PHY_MunitionForIndirectFireData::operator()( const PHY_ComposantePion& compFirer, PHY_Weapon& weapon )
{
    if( !compFirer.CanFire() || !weapon.CanIndirectFire() )
        return;

    const PHY_DotationCategory_IndirectFire_ABC* pIndirectFireData = weapon.GetDotationCategory().GetIndirectFireData();
    if( !pIndirectFireData || pIndirectFireData->GetIndirectFireDotationCategory() != indirectWeaponCategory_ )
        return;

    if( rRange_ < weapon.GetMinRangeToIndirectFire() || rRange_ > weapon.GetMaxRangeToIndirectFire() )
        return;

    if( pChoosenMunition_ && weapon.GetDotationCategory() == *pChoosenMunition_ )
        return;

    const PHY_RolePion_Dotations& roleDotations = firer_.GetRole< PHY_RolePion_Dotations >();

    if( !pChoosenMunition_ || roleDotations.GetDotationValue( *pChoosenMunition_ ) < roleDotations.GetDotationValue( weapon.GetDotationCategory() ) )
        pChoosenMunition_ = &weapon.GetDotationCategory();
}

// -----------------------------------------------------------------------------
// Name: PHY_MunitionForIndirectFireData::GetChoosenMunition
// Created: NLD 2006-08-08
// -----------------------------------------------------------------------------
const PHY_DotationCategory* PHY_MunitionForIndirectFireData::GetChoosenMunition() const
{
    return pChoosenMunition_;
}
