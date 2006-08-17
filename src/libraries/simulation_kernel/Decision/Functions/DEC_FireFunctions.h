// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_FireFunctions.h $
// $Author: Jvt $
// $Modtime: 2/05/05 11:39 $
// $Revision: 4 $
// $Workfile: DEC_FireFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_FireFunctions_h_
#define __DEC_FireFunctions_h_

#include "MIL.h"

class DIA_Call_ABC;
class MIL_AgentPion;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_FireFunctions
{
public:
    //! @name Functions
    //@{
    static void GetMaxRangeToFireOnEnemy                 ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetMinRangeToFireOnEnemy                 ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetMaxRangeToFireOnEnemyActualPosture    ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetMinRangeToFireOnEnemyActualPosture    ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetMaxRangeToFireOnEnemyWhenUnloaded     ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetMaxRangeToBeFiredByEnemy              ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetMaxRangeToIndirectFire                ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetMinRangeToIndirectFire                ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetMaxRangeToIndirectFireWithoutAmmoCheck( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetMinRangeToIndirectFireWithoutAmmoCheck( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void ThrowSmokeOnKnowledgeAgent               ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    static void GetMunitionForIndirectFire               ( DIA_Call_ABC& call,       MIL_AgentPion& callerAgent );
    //@}
};

#endif // __DEC_FireFunctions_h_
