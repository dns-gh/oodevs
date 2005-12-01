// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_PopulationFunctions.h $
// $Author: Jvt $
// $Modtime: 11/05/05 10:42 $
// $Revision: 17 $
// $Workfile: DEC_PopulationFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_PopulationFunctions_h_
#define __DEC_PopulationFunctions_h_

#include "MIL.h"

class MIL_Population;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_PopulationFunctions
{
public:
    //! @name Functions
    //@{  
    // Test / debug
    static void Debug          ( DIA_Call_ABC& call, const MIL_Population& callerPopulation );
//    static void Trace          ( DIA_Call_ABC& call, const MIL_Population& callerPopulation );
//    static void DebugDrawPoints( DIA_Call_ABC& call, const MIL_Population& callerPopulation );
//    static void DebugDrawPoint ( DIA_Call_ABC& call, const MIL_Population& callerPopulation );

    // Effects
    static void SetPionMaxSpeed  ( DIA_Call_ABC& call, MIL_Population& callerPopulation );
    static void ResetPionMaxSpeed( DIA_Call_ABC& call, MIL_Population& callerPopulation );
    static void SetAttitude      ( DIA_Call_ABC& call, MIL_Population& callerPopulation );

    // Knowledge
    static void GetKnowledgeAgentRoePopulation( DIA_Call_ABC& call, const MIL_Population& callerPopulation );
    static void GetPionsAttacking             ( DIA_Call_ABC& call, const MIL_Population& callerPopulation );
    //@}    
};

#endif // __DEC_PopulationFunctions_h_
