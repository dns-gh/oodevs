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

#ifndef __DEC_NbcFunctions_h_
#define __DEC_NbcFunctions_h_

#include "MIL.h"
#include "Knowledge/DEC_Knowledge_Object.h"

class MIL_AgentPion;

// =============================================================================
// Created: RFT 2004-03-31
// =============================================================================
class DEC_NbcFunctions
{
public:
    //! @name Functions
    //@{
    static int AnalyzeNbcAgent     ( MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > knowledge );
    static void DecontaminateAgent  ( MIL_AgentPion& callerAgent );
    static void DecontaminateSite   ( MIL_AgentPion& callerAgent );
    //@}
};

#endif // __DEC_NbcFunctions_h_
