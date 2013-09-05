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
#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_IndirectFire constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_WeaponDataType_IndirectFire::PHY_WeaponDataType_IndirectFire( const PHY_WeaponType& weaponType, xml::xistream& xis, double timeFactor )
    : weaponType_( weaponType )
{
    assert( weaponType_.GetDotationCategory().CanBeUsedForIndirectFire() );

    xis >> xml::attribute( "average-speed", rAverageSpeed_ )
        >> xml::attribute( "min-range", rMinRange_ )
        >> xml::attribute( "max-range", rMaxRange_ );

    if( rAverageSpeed_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "indirect-fire: average-speed < 0" );
    if( rMinRange_ < 0 )
        throw MASA_EXCEPTION( xis.context() + "indirect-fire: min-range < 0" );
    if( rMaxRange_ < rMinRange_ )
        throw MASA_EXCEPTION( xis.context() + "indirect-fire: max-range < min-range" );

    rAverageSpeed_ = rAverageSpeed_ * timeFactor; /* m/s -> km/h */
    rMinRange_ = MIL_Tools::ConvertMeterToSim( rMinRange_ );
    rMaxRange_ = MIL_Tools::ConvertMeterToSim( rMaxRange_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_IndirectFire destructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
PHY_WeaponDataType_IndirectFire::~PHY_WeaponDataType_IndirectFire()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_IndirectFire::Fire
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
void PHY_WeaponDataType_IndirectFire::Fire( MIL_Effect_IndirectFire& effect, unsigned int nNbrAmmoReserved ) const
{
    effect.NotifyAmmoFired( *this, nNbrAmmoReserved );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_IndirectFire::ThrowSmoke
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
void PHY_WeaponDataType_IndirectFire::ThrowSmoke( MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmo, PHY_FireResults_ABC& fireResult ) const
{
    weaponType_.GetDotationCategory().ApplyIndirectFireEffect( firer, vSourcePosition, vTargetPosition, nNbrAmmo, fireResult );
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_IndirectFire::GetAverageSpeed
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_IndirectFire::GetAverageSpeed() const
{
    return rAverageSpeed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_IndirectFire::GetMinRange
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_IndirectFire::GetMinRange() const
{
    return rMinRange_;
}

// -----------------------------------------------------------------------------
// Name: PHY_WeaponDataType_IndirectFire::GetMaxRange
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double PHY_WeaponDataType_IndirectFire::GetMaxRange() const
{
    return rMaxRange_;
}
