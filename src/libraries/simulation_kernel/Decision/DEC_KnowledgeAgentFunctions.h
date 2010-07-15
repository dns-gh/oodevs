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
#include "Knowledge/DEC_Knowledge_Urban.h"

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
    static int   GetNatureAtlas    ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static float GetDangerosity    ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static float GetPotentialAttrition( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< MT_Vector2D > position );
    static bool  IsPerceivedByAgent( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static int   IsAnEnemy         ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static int   IsAFriend         ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  IsMoving          ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  IsPerceivingAgent ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  IsInDetectionCone ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, const MT_Vector2D* direction, MT_Float angle );
    static bool  KillOfficers      ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );

    static int   GetMaxPerceptionLevelForKnowledgeGroup ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static int   GetCurrentPerceptionLevel              ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static float GetDangerosityOnPion                   ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, const DEC_Decision_ABC* pTarget );
    static float GetDangerosityOnKnowledge              ( boost::shared_ptr< DEC_Knowledge_Agent > pSource, boost::shared_ptr< DEC_Knowledge_Agent > pTarget );
    static boost::shared_ptr< MT_Vector2D > GetPositionPtr( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static float GetOperationalState                    ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static float GetROE                                    ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  GetMajorOperationalState               ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  IsFlying                               ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  IsKnowledgeValid                       ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  IsPrisoner                             ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  IsDead                                 ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static int   Lock                                   ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void  Unlock                                 ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );

    static double   GetMaterialComposantesProtectionLevel( boost::shared_ptr< DEC_Knowledge_Agent > pTarget,  boost::shared_ptr< DEC_Knowledge_Urban > pUrbanBlock );

    static bool  IsIlluminated                          ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  IsDefinitivelyIlluminated              ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  CanBeIlluminate                        ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    //@}
};

#endif // __DEC_KnowledgeAgentFunctions_h_
