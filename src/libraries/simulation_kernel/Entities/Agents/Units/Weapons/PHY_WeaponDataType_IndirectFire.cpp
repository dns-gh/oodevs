// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_WeaponDataType_IndirectFire.cpp $
// $Author: Jvt $
// $Modtime: 2/05/05 14:53 $
// $Revision: 3 $
// $Workfile: PHY_WeaponDataType_IndirectFire.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_WeaponDataType_IndirectFire.h"

#include "PHY_WeaponType.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Entities/Effects/MIL_Effect_IndirectFire.h"

#include "Tools/MIL_Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_IndirectFire constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_WeaponDataType_IndirectFire::PHY_WeaponDataType_IndirectFire( const PHY_WeaponType& weaponType, xml::xistream& xis )
    : weaponType_( weaponType )
{
    assert( weaponType_.GetDotationCategory().GetIndirectFireData() != 0 );

    xis >> attribute( "average-speed", rAverageSpeed_ )
        >> attribute( "min-range", rMinRange_ )
        >> attribute( "max-range", rMaxRange_ );

    if( rAverageSpeed_ < 0 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "indirect-fire: average-speed < 0" );
    if( rMinRange_ < 0 )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "indirect-fire: min-range < 0" );
    if( rMaxRange_ < rMinRange_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "indirect-fire: max-range < min-range" );

    rAverageSpeed_ = MIL_Tools::ConvertSpeedMosToSim( rAverageSpeed_ * 3.6 /* m/s -> km/h */ ); //$$$$
    rMinRange_ = MIL_Tools::ConvertMeterToSim( rMinRange_ );
    rMaxRange_ = MIL_Tools::ConvertMeterToSim( rMaxRange_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_IndirectFire destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_WeaponDataType_IndirectFire::~PHY_WeaponDataType_IndirectFire()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_IndirectFire::Fire
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void PHY_WeaponDataType_IndirectFire::Fire( MIL_AgentPion& /*firer*/, MIL_Effect_IndirectFire& effect, uint nNbrAmmoReserved ) const
{
    effect.NotifyAmmoFired( *this, nNbrAmmoReserved );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_IndirectFire::ThrowSmoke
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void PHY_WeaponDataType_IndirectFire::ThrowSmoke( MIL_AgentPion& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, uint nNbrAmmo, PHY_FireResults_ABC& fireResult ) const
{
    weaponType_.GetDotationCategory().ApplyIndirectFireEffect( firer, vSourcePosition, vTargetPosition, nNbrAmmo, fireResult );
}
