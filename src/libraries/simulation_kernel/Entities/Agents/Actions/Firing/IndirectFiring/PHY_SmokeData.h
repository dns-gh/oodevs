// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_SmokeData.h $
// $Author: Nld $
// $Modtime: 17/03/05 14:52 $
// $Revision: 2 $
// $Workfile: PHY_SmokeData.h $
//
// *****************************************************************************

#ifndef __PHY_SmokeData_h_
#define __PHY_SmokeData_h_

class MIL_AgentPion;
class PHY_Weapon;
class PHY_ComposantePion;
class PHY_IndirectFireDotationClass;

// =============================================================================
// @class  PHY_SmokeData
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SmokeData : private boost::noncopyable
{

public:
     PHY_SmokeData( const MIL_AgentPion& firer, const PHY_IndirectFireDotationClass& indirectWeaponCategory, uint nNbrAmmo );
    ~PHY_SmokeData();

    //! @name Operations
    //@{
    void operator()( const PHY_ComposantePion& compFirer, PHY_Weapon& weapon );
    PHY_Weapon* GetWeapon () const;
    //@}

private:
    //! @name Member data
    //@{
    const MIL_AgentPion&                    firer_;
    const PHY_IndirectFireDotationClass& indirectWeaponCategory_;
    const uint                              nNbrAmmo_;
          PHY_Weapon*                       pWeapon_;
    //@}
};

#endif // __PHY_SmokeData_h_
