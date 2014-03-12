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
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/MIL_AgentPion.h"

#include "simulation_kernel/AlgorithmsFactories.h"
#include "simulation_kernel/DefaultDotationComputer.h"

using namespace firing;
// -----------------------------------------------------------------------------
// Name: PHY_MunitionForIndirectFireData constructor
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
PHY_MunitionForIndirectFireData::PHY_MunitionForIndirectFireData( MIL_Agent_ABC& firer, const PHY_IndirectFireDotationClass& indirectWeaponCategory, const MT_Vector2D* vTargetPosition  )
    : firer_                 ( firer )
    , indirectWeaponCategory_( indirectWeaponCategory )
    , vTargetPosition_       ( vTargetPosition )
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

    const PHY_DotationCategory& category = weapon.GetDotationCategory();

    if( !category.CanBeUsedForIndirectFire() || !category.HasIndirectWeaponCategory( indirectWeaponCategory_ ) )
        return;

    if( vTargetPosition_ )
    {
        const double rRange = firer_.GetRole< PHY_RoleInterface_Location >().GetPosition().Distance( *vTargetPosition_ );

        if( rRange < weapon.GetMinRangeToIndirectFire() || rRange > weapon.GetMaxRangeToIndirectFire() )
            return;
    }

    if( pChoosenMunition_ && weapon.GetDotationCategory() == *pChoosenMunition_ )
        return;

    dotation::DefaultDotationComputer dotationComputer;
    firer_.Execute< dotation::DotationComputer_ABC >( dotationComputer );

    if( !pChoosenMunition_ || dotationComputer.GetDotationValue( *pChoosenMunition_ ) < dotationComputer.GetDotationValue( weapon.GetDotationCategory() ) )
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
