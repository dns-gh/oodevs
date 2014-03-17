// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Effects/MIL_Effect_PopulationFire.h $
// $Author: Age $
// $Modtime: 29/11/04 14:56 $
// $Revision: 3 $
// $Workfile: MIL_Effect_PopulationFire.h $
//
// *****************************************************************************

#ifndef __MIL_Effect_PopulationFire_h_
#define __MIL_Effect_PopulationFire_h_

#include "MIL_Effect_ABC.h"

class MIL_Agent_ABC;
class MIL_Population;
class MIL_PopulationType;
class MIL_PopulationAttitude;
class PHY_Composante_ABC;
class PHY_FireResults_ABC;

// =============================================================================
// @class  MIL_Effect_PopulationFire
// Created: JVT 2004-08-03
// =============================================================================
class MIL_Effect_PopulationFire : public MIL_Effect_ABC
{
public:
             MIL_Effect_PopulationFire( MIL_Population& population, const MIL_PopulationType& populationType, const MIL_PopulationAttitude& populationAttitude, MIL_Agent_ABC& target, PHY_Composante_ABC& compTarget, PHY_FireResults_ABC& fireResult, double armedIndividuals );
    virtual ~MIL_Effect_PopulationFire();

    //! @name Operations
    //@{
    virtual bool Execute();
    //@}

private:
    MIL_Population& population_;
    const MIL_PopulationType& populationType_;
    const MIL_PopulationAttitude& populationAttitude_;
    MIL_Agent_ABC& target_;
    PHY_Composante_ABC& compTarget_;
    PHY_FireResults_ABC& fireResult_;
    double armedIndividuals_;
};

#endif // __MIL_Effect_PopulationFire_h_
