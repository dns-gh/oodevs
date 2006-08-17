// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_KnowledgeAgentFunctions.h $
// $Author: Jvt $
// $Modtime: 23/05/05 16:32 $
// $Revision: 8 $
// $Workfile: DEC_KnowledgeAgentFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_KnowledgeAgentFunctions_h_
#define __DEC_KnowledgeAgentFunctions_h_

#include "MIL.h"

class MIL_AgentPion;
class DEC_Knowledge_Agent;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_KnowledgeAgentFunctions
{
public:
    //! @name Functions
    //@{
    static void GetNatureWeapon                       ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetNatureAtlas                        ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void GetDangerosity                        ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsPerceivedByAgent                    ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsPerceivedByKnowledgeGroup           ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsAnEnemy                             ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsMoving                              ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsPerceivingAgent                     ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsInSameDirection                     ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void IsInDetectionCone                     ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );
    static void KillOfficers                          ( DIA_Call_ABC& call, const MIL_AgentPion& callerAgent );

    template< typename T > static void GetMaxPerceptionLevelForKnowledgeGroup( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void GetDangerosityOnPion                  ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void GetDangerosityOnKnowledge             ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void GetSpeed                              ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void GetPosition                           ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void GetOperationalState                   ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void GetMajorOperationalState              ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void IsFlying                              ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void IsKnowledgeValid                      ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void IsSurrendered                         ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void IsPrisoner                            ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void IsRefugee                             ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void IsRefugeeManaged                      ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void IsMilitia                             ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void IsTerrorist                           ( DIA_Call_ABC& call, const T& caller );   
    template< typename T > static void IsDead                                ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void Lock                                  ( DIA_Call_ABC& call, const T& caller );
    template< typename T > static void Unlock                                ( DIA_Call_ABC& call, const T& caller );
    //@}
};

#include "DEC_KnowledgeAgentFunctions.inl"

#endif // __DEC_KnowledgeAgentFunctions_h_
