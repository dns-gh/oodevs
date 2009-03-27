// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_FireFunctions.h $
// $Author: RFT $
// $Modtime: 2/05/05 11:39 $
// $Revision: 4 $
// $Workfile: DEC_FireFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_ExtinguishFireFunctions_h_
#define __DEC_ExtinguishFireFunctions_h_

#include "MIL.h"

class DIA_Call_ABC;
class MIL_AgentPion;

// =============================================================================
// Created: RFT 2004-03-31
// =============================================================================
class DEC_ExtinguishFireFunctions
{
public:
    //! @name Functions
    //@{
    static void GetRangeToExtinguish( DIA_Call_ABC& call, MIL_AgentPion& pPion );
    static void AnalyzeFireClass    ( DIA_Call_ABC& call, MIL_AgentPion& pPion );
    //@}
};

#endif // __DEC_ExtinguishFireFunctions_h_
