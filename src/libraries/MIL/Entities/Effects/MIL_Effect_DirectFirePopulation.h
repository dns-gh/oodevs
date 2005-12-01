// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Effects/MIL_Effect_DirectFirePopulation.h $
// $Author: Age $
// $Modtime: 29/11/04 14:56 $
// $Revision: 3 $
// $Workfile: MIL_Effect_DirectFirePopulation.h $
//
// *****************************************************************************

#ifndef __MIL_Effect_DirectFirePopulation_h_
#define __MIL_Effect_DirectFirePopulation_h_

#include "MIL.h"

#include "MIL_Effect_ABC.h"

class MIL_PopulationElement_ABC;
class PHY_FireResults_ABC;

// =============================================================================
// @class  MIL_Effect_DirectFirePopulation
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Effect_DirectFirePopulation : public MIL_Effect_ABC
{
    MT_COPYNOTALLOWED( MIL_Effect_DirectFirePopulation )

public:
             MIL_Effect_DirectFirePopulation( MIL_PopulationElement_ABC& target, uint nNbrAmmoFired, PHY_FireResults_ABC& fireResult );
    virtual ~MIL_Effect_DirectFirePopulation();

    //! @name Operations
    //@{
    virtual bool Execute();
    //@}

private:
          MIL_PopulationElement_ABC& target_; 
    const uint                       nNbrAmmoFired_;
          PHY_FireResults_ABC&       fireResult_;
};

#include "MIL_Effect_DirectFirePopulation.inl"

#endif // __MIL_Effect_DirectFirePopulation_h_
