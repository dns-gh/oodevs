// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/PopulationFiring/PHY_FireResults_Population.h $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 4 $
// $Workfile: PHY_FireResults_Population.h $
//
// *****************************************************************************

#ifndef __PHY_FireResults_Population_h_
#define __PHY_FireResults_Population_h_

#include "MIL.h"

#include "Entities/Actions/PHY_FireResults_ABC.h"

class MIL_Population;

// =============================================================================
// @class  PHY_FireResults_Population
// Created: JVT 2004-08-03
// =============================================================================
class PHY_FireResults_Population : public PHY_FireResults_ABC
{

public:
    explicit PHY_FireResults_Population( const MIL_Population& firer );
    virtual ~PHY_FireResults_Population();

private:
    const MIL_Population& firer_;
    const uint            nID_;
};

#include "PHY_FireResults_Population.inl"

#endif // __PHY_FireResults_Population_h_
