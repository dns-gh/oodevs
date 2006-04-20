// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/decision/functions/DEC_PathFunctions.h $
// $Author: Jvt $
// $Modtime: 11/05/05 16:56 $
// $Revision: 4 $
// $Workfile: DEC_PathFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_PathFunctions_h_
#define __DEC_PathFunctions_h_

#include "MIL.h"

class MIL_AgentPion;
class MIL_Automate;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_PathFunctions
{
public:
    //! @name Functions
    //@{
    static void CreatePathToPoint    ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void CreatePathToPointList( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void DeletePath           ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void GetPathState         ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void GetNextObjectOnPath  ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void ExtrapolatePosition  ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetLastPointOfPath   ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsMovingOnPath       ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    //@}
};

#endif // __DEC_PathFunctions_h_
