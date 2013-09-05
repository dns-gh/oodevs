// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_WeaponDataType_IndirectFire.h $
// $Author: Jvt $
// $Modtime: 2/05/05 13:40 $
// $Revision: 3 $
// $Workfile: PHY_WeaponDataType_IndirectFire.h $
//
// *****************************************************************************

#ifndef __PHY_WeaponDataType_IndirectFire_h_
#define __PHY_WeaponDataType_IndirectFire_h_

namespace xml
{
    class xistream;
}

class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_Effect_IndirectFire;
class PHY_FireResults_ABC;
class PHY_WeaponType;
class MT_Vector2D;

// =============================================================================
// @class  PHY_WeaponDataType_IndirectFire
// Created: JVT 2004-08-03
// =============================================================================
class PHY_WeaponDataType_IndirectFire : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             PHY_WeaponDataType_IndirectFire( const PHY_WeaponType& weaponType, xml::xistream& xis, double timeFactor );
    virtual ~PHY_WeaponDataType_IndirectFire();
    //@}

    //! @name Accessors
    //@{
    double GetAverageSpeed() const;
    double GetMinRange() const;
    double GetMaxRange() const;
    double GetSmokeDuration() const;
    //@}

    //! @name Operations
    //@{
    void Fire( MIL_Effect_IndirectFire& effect, unsigned int nNbrAmmoReserved ) const;
    void ThrowSmoke( MIL_Agent_ABC& firer, const MT_Vector2D& vSourcePosition, const MT_Vector2D& vTargetPosition, unsigned int nNbrAmmo, PHY_FireResults_ABC& fireResult ) const;
    //@}

private:
    //! @name Member data
    //@{
    const PHY_WeaponType& weaponType_;
    double rAverageSpeed_;
    double rMinRange_;
    double rMaxRange_;
    //@}
};

#endif // __PHY_WeaponDataType_IndirectFire_h_
