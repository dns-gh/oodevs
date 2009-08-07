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

class DEC_Decision_ABC;
class DEC_Knowledge_Agent;
class MIL_AgentPion;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_KnowledgeAgentFunctions
{
public:
    //! @name Functions
    //@{
    static int   GetNatureAtlas    ( const DEC_Knowledge_Agent* pKnowledge );
    static float GetDangerosity    ( const MIL_AgentPion& callerAgent, const DEC_Knowledge_Agent* pKnowledge );
    static bool  IsPerceivedByAgent( const MIL_AgentPion& callerAgent, const DEC_Knowledge_Agent* pKnowledge );
    static int   IsAnEnemy         ( const MIL_AgentPion& callerAgent, const DEC_Knowledge_Agent* pKnowledge );
    static bool  IsMoving          ( const DEC_Knowledge_Agent* pKnowledge );
    static bool  IsPerceivingAgent ( const MIL_AgentPion& callerAgent, const DEC_Knowledge_Agent* pKnowledge );
    static bool  IsInDetectionCone ( const MIL_AgentPion& callerAgent, const DEC_Knowledge_Agent* pKnowledge, const MT_Vector2D* direction, MT_Float angle );
    static bool  KillOfficers      ( DEC_Knowledge_Agent* pKnowledge );

    static int   GetMaxPerceptionLevelForKnowledgeGroup ( const DEC_Knowledge_Agent* pKnowledge );
    static float GetDangerosityOnPion                   ( const DEC_Knowledge_Agent* pKnowledge, const DEC_Decision_ABC* pTarget );
    static float GetDangerosityOnKnowledge              ( const DEC_Knowledge_Agent* pSource, const DEC_Knowledge_Agent* pTarget );
    static const MT_Vector2D* GetPosition               ( const DEC_Knowledge_Agent* pKnowledge );
    static float GetOperationalState                    ( const DEC_Knowledge_Agent* pKnowledge );
    static bool  GetMajorOperationalState               ( const DEC_Knowledge_Agent* pKnowledge );
    static bool  IsFlying                               ( const DEC_Knowledge_Agent* pKnowledge );
    static bool  IsKnowledgeValid                       ( const DEC_Knowledge_Agent* pKnowledge );
    static bool  IsPrisoner                             ( const DEC_Knowledge_Agent* pKnowledge );
    static bool  IsDead                                 ( const DEC_Knowledge_Agent* pKnowledge );
    static int   Lock                                   ( DEC_Knowledge_Agent* pKnowledge );
    static void  Unlock                                 ( DEC_Knowledge_Agent* pKnowledge );
    //@}
};

#include "DEC_KnowledgeAgentFunctions.inl"

#endif // __DEC_KnowledgeAgentFunctions_h_
