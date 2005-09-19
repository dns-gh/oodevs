// *****************************************************************************
//
// $Created: NLD 2004-07-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_MiscFunctions.h $
// $Author: Nld $
// $Modtime: 23/03/05 16:18 $
// $Revision: 4 $
// $Workfile: DEC_MiscFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_MiscFunctions_h_
#define __DEC_MiscFunctions_h_

#include "MIL.h"
#include "MT_Tools/MT_Random.h"

class MIL_AgentPion;

// =============================================================================
// Created: NLD 2004-07-08
// =============================================================================
class DEC_MiscFunctions
{
public:
    // Communication
    template< typename T > static void RC_Operational( DIA_Call_ABC& call, T& caller );
    template< typename T > static void RC_Message    ( DIA_Call_ABC& call, T& caller );
    template< typename T > static void RC_Warning    ( DIA_Call_ABC& call, T& caller );    
  
    // Reinforcement
    static void Reinforce          ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );            
    static void CancelReinforcement( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );            
    static void IsReinforcing      ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );            
    static void GetReinforcements  ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );

    // Misc
    static void SetCurrentSpeedModificator( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
    static void SetMaxSpeedModificator    ( DIA_Call_ABC& call, MIL_AgentPion& callerAgent );
};

#include "DEC_MiscFunctions.inl"

#endif // __DEC_MiscFunctions_h_
