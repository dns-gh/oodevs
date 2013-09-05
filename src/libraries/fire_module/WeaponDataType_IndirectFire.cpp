// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "WeaponDataType_IndirectFire.h"
#include <xeumeuleu/xml.hpp>

using namespace sword::fire;

// -----------------------------------------------------------------------------
// Name: WeaponDataType_IndirectFire constructor
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
WeaponDataType_IndirectFire::WeaponDataType_IndirectFire( xml::xistream& xis, double timeFactor )
{
    xis >> xml::attribute( "average-speed", rAverageSpeed_ )
        >> xml::attribute( "min-range", rMinRange_ )
        >> xml::attribute( "max-range", rMaxRange_ );
    if( rAverageSpeed_ < 0 )
        throw xml::exception( xis.context() + "indirect-fire: average-speed < 0" );
    if( rMinRange_ < 0 )
        throw xml::exception( xis.context() + "indirect-fire: min-range < 0" );
    if( rMaxRange_ < rMinRange_ )
        throw xml::exception( xis.context() + "indirect-fire: max-range < min-range" );
    rAverageSpeed_ = rAverageSpeed_ * timeFactor; /* m/s -> km/h */
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_IndirectFire::Fire
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
//void WeaponDataType_IndirectFire::Fire( MIL_Effect_IndirectFire& effect, unsigned int nNbrAmmoReserved ) const
//{
//    effect.NotifyAmmoFired( *this, nNbrAmmoReserved );
//}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_IndirectFire::ThrowSmoke
// Created: NLD 2004-10-21
// -----------------------------------------------------------------------------
//void WeaponDataType_IndirectFire::ThrowSmoke( MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmo, PHY_FireResults_ABC& fireResult ) const
//{
//    weaponType_.GetDotationCategory().ApplyIndirectFireEffect( firer, vSourcePosition, vTargetPosition, nNbrAmmo, fireResult );
//}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_IndirectFire::GetAverageSpeed
// Created: NLD 2004-10-11
// -----------------------------------------------------------------------------
double WeaponDataType_IndirectFire::GetAverageSpeed() const
{
    return rAverageSpeed_;
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_IndirectFire::GetMinRange
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double WeaponDataType_IndirectFire::GetMinRange() const
{
    return rMinRange_;
}

// -----------------------------------------------------------------------------
// Name: WeaponDataType_IndirectFire::GetMaxRange
// Created: JVT 2005-05-02
// -----------------------------------------------------------------------------
double WeaponDataType_IndirectFire::GetMaxRange() const
{
    return rMaxRange_;
}
