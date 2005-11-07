// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Actions/Firing/PopulationFiring/PHY_PopulationFireResults.h $
// $Author: Nld $
// $Modtime: 12/04/05 17:19 $
// $Revision: 4 $
// $Workfile: PHY_PopulationFireResults.h $
//
// *****************************************************************************

#ifndef __PHY_PopulationFireResults_h_
#define __PHY_PopulationFireResults_h_

#include "MIL.h"

#include "Entities/Agents/Actions/Firing/PHY_FireResults_ABC.h"

class MIL_Population;

// =============================================================================
// @class  PHY_PopulationFireResults
// Created: JVT 2004-08-03
// =============================================================================
class PHY_PopulationFireResults : public PHY_FireResults_ABC
{
    MT_COPYNOTALLOWED( PHY_PopulationFireResults )

public:
    PHY_PopulationFireResults( const MIL_Population& firer );
    virtual ~PHY_PopulationFireResults();

private:
    const MIL_Population& firer_;
    const uint            nID_;
};

#include "PHY_PopulationFireResults.inl"

#endif // __PHY_PopulationFireResults_h_
