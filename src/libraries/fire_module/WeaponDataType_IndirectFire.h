// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef fire_module_WeaponDataType_IndirectFire_h
#define fire_module_WeaponDataType_IndirectFire_h

#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace sword
{
namespace wrapper
{
    class View;
}

namespace fire
{
    //class WeaponType;

// =============================================================================
/** @class  WeaponDataType_IndirectFire
    @brief  Weapon data type for indirect fire
*/
// Created: JVT 2004-08-03
// =============================================================================
class WeaponDataType_IndirectFire : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    WeaponDataType_IndirectFire( /*const WeaponType& weaponType,*/ xml::xistream& xis, double timeFactor );
    //@}

    //! @name Accessors
    //@{
    double GetAverageSpeed() const;
    double GetMinRange() const;
    double GetMaxRange() const;
    //@}

    //! @name Operations
    //@{
    //void Fire( MIL_Effect_IndirectFire& effect, unsigned int nNbrAmmoReserved ) const;
    //void ThrowSmoke( MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmo, PHY_FireResults_ABC& fireResult ) const;
    //@}

private:
    //! @name Member data
    //@{
    //const PHY_WeaponType& weaponType_;
    double rAverageSpeed_;
    double rMinRange_;
    double rMaxRange_;
    //@}
};

}
}

#endif // fire_module_WeaponDataType_IndirectFire_h
