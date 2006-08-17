// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_MunitionForIndirectFireData.h $
// $Author: Nld $
// $Modtime: 17/03/05 14:52 $
// $Revision: 2 $
// $Workfile: PHY_MunitionForIndirectFireData.h $
//
// *****************************************************************************

#ifndef __PHY_MunitionForIndirectFireData_h_
#define __PHY_MunitionForIndirectFireData_h_

#include "MIL.h"

class MIL_AgentPion;
class PHY_Weapon;
class PHY_ComposantePion;
class PHY_IndirectFireDotationClass;

// =============================================================================
// @class  PHY_MunitionForIndirectFireData
// Created: JVT 2004-08-03
// =============================================================================
class PHY_MunitionForIndirectFireData
{
    MT_COPYNOTALLOWED( PHY_MunitionForIndirectFireData )

public:
     PHY_MunitionForIndirectFireData( const MIL_AgentPion& firer, const PHY_IndirectFireDotationClass& indirectWeaponCategory, MT_Float rRange );
    ~PHY_MunitionForIndirectFireData();

    //! @name Operations
    //@{
    void        operator()( const PHY_ComposantePion& compFirer, PHY_Weapon& weapon );

    const PHY_DotationCategory* GetChoosenMunition() const;
    //@}

private:
    const MIL_AgentPion&                    firer_;
    const PHY_IndirectFireDotationClass& indirectWeaponCategory_;
    const MT_Float                          rRange_;
    const PHY_DotationCategory*             pChoosenMunition_;
};

#include "PHY_MunitionForIndirectFireData.inl"

#endif // __PHY_MunitionForIndirectFireData_h_
