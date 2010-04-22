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

#ifndef __DEC_DynamicFireFunctions_h_
#define __DEC_DynamicFireFunctions_h_

#include "MIL.h"
#include "Knowledge/DEC_Knowledge_Object.h"

class MIL_AgentPion;

// =============================================================================
// Created: RFT 2004-03-31
// =============================================================================
class DEC_DynamicFireFunctions
{
public:
    //! @name Functions
    //@{
    static float GetRangeToExtinguish       ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > knowledge );
    static unsigned int AnalyzeFireClass    ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > knowledge );
    //@}
};

#endif // __DEC_DynamicFireFunctions_h_
