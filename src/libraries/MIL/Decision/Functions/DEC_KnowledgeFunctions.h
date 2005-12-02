// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgeFunctions.h $
// $Author: Nld $
// $Modtime: 11/05/05 16:03 $
// $Revision: 7 $
// $Workfile: DEC_KnowledgeFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_KnowledgeFunctions_h_
#define __DEC_KnowledgeFunctions_h_

#include "MIL.h"

class DIA_Call_ABC;
class MIL_AgentPion;
class MIL_Automate;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_KnowledgeFunctions
{
public:
    //! @name Rapport de force
    //@{
    static void GetRapForLocal               ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetRapForGlobal              ( DIA_Call_ABC& call, const MIL_Automate&  callerAgent );
    static void GetAgentsAttacking           ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetPotentiallyDangerousAgents( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetMilitiaAgents             ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetRefugeeAgents             ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetSurrenderedAgents         ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetPerceivedAgents           ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );    
    static void GetClosestObject             ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );    
    static void GetObjectsColliding          ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetPopulationsColliding      ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetPopulationsAttacking      ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );

    template< typename T > static void ShareKnowledgesWith      ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void ShareKnowledgesInZoneWith( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void GetObjects               ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void GetFriendsInZone         ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void GetPopulations           ( DIA_Call_ABC& call, const T& caller );
    //@}
};

#include "DEC_KnowledgeFunctions.inl"

#endif // __DEC_KnowledgeFunctions_h_
