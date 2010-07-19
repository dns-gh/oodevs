// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/IndirectFiring/PHY_FireResults_Pion.h $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 4 $
// $Workfile: PHY_FireResults_Pion.h $
//
// *****************************************************************************

#ifndef __PHY_FireResults_Pion_h_
#define __PHY_FireResults_Pion_h_

#include "Entities/Actions/PHY_FireResults_ABC.h"

class MIL_Agent_ABC;
class MIL_Population;
class PHY_DotationCategory;

// =============================================================================
// @class  PHY_FireResults_Pion
// Created: JVT 2004-08-03
// =============================================================================
class PHY_FireResults_Pion : public PHY_FireResults_ABC
{
public:
             PHY_FireResults_Pion( const MIL_Agent_ABC& firer, const MIL_Agent_ABC& target );
             PHY_FireResults_Pion( const MIL_Agent_ABC& firer, const MIL_Population& target );
             PHY_FireResults_Pion( const MIL_Agent_ABC& firer, const MT_Vector2D& vTargetPosition, const PHY_DotationCategory& dotationCategory );
    virtual ~PHY_FireResults_Pion();

    //! @name Accessors
    //@{
    unsigned int GetID() const;
    //@}

private:
    //! @name Member data
    //@{
    const MIL_Agent_ABC& firer_;
    const unsigned int nID_;
    //@}
};

#endif // __PHY_FireResults_Pion_h_
