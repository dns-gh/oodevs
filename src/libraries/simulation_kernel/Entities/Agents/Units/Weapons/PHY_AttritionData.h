// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Units/Weapons/PHY_AttritionData.h $
// $Author: Nld $
// $Modtime: 19/10/04 13:54 $
// $Revision: 1 $
// $Workfile: PHY_AttritionData.h $
//
// *****************************************************************************

#ifndef __PHY_AttritionData_h_
#define __PHY_AttritionData_h_

#include "MT_Tools/MT_Random.h"

namespace xml
{
    class xistream;
}

class PHY_ComposanteState;

// =============================================================================
// @class  PHY_AttritionData
// Created: JVT 2004-08-03
// =============================================================================
class PHY_AttritionData
{
public:
              PHY_AttritionData();
     explicit PHY_AttritionData( xml::xistream& xis );
              PHY_AttritionData( const PHY_AttritionData& rhs );
     virtual ~PHY_AttritionData();

    //! @name Operations
    //@{
    PHY_AttritionData&         operator=             ( const PHY_AttritionData& rhs );
    MT_Float                   GetScore              () const;
    const PHY_ComposanteState& ComputeComposanteState( MT_Float urbanProtection ) const; 
    //@}

private:
    MT_Float rDestroyed_;
    MT_Float rReparableWithEvacuation_;
    MT_Float rReparableWithoutEvacuation_;

    MT_Float rScore_;

private:
    static MT_Random randomGenerator_;
};

#endif // __PHY_AttritionData_h_
