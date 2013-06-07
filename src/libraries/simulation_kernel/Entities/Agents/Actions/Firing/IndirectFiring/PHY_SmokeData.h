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

#include "FireData_ABC.h"

class MIL_Agent_ABC;
class PHY_Weapon;
class PHY_ComposantePion;
class PHY_IndirectFireDotationClass;

namespace firing
{
// =============================================================================
// @class  PHY_SmokeData
// Created: JVT 2004-08-03
// =============================================================================
class PHY_SmokeData : public FireData_ABC
{
public:
             PHY_SmokeData( MIL_Agent_ABC& firer, const PHY_IndirectFireDotationClass& indirectWeaponCategory );
    virtual ~PHY_SmokeData();

    //! @name Operations
    //@{
    virtual void operator()( const PHY_ComposantePion& compFirer, PHY_Weapon& weapon );
    PHY_Weapon* GetWeapon () const;
    //@}

private:
    //! @name Member data
    //@{
    MIL_Agent_ABC& firer_;
    const PHY_IndirectFireDotationClass& indirectWeaponCategory_;
    PHY_Weapon* pWeapon_;
    //@}
};

} // namespace firing

#endif // __PHY_SmokeData_h_
