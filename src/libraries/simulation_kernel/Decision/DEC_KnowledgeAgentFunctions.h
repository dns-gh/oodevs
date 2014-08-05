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

class DEC_Decision_ABC;
class DEC_Knowledge_Agent;
class MIL_AgentPion;
class MIL_Entity_ABC;
class MIL_Mission_ABC;
class MT_Vector2D;
class MIL_UrbanObject_ABC;

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
    static bool  IsInDetectionCone ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, const MT_Vector2D* direction, double angle );
    static bool  IsInCrowd         ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  KillOfficers      ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );

    static int   GetMaxPerceptionLevelForKnowledgeGroup ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static int   GetCurrentPerceptionLevel              ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static float GetDangerosityOnPion                   ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, const DEC_Decision_ABC* pTarget );
    static float GetDangerosityOnKnowledge              ( boost::shared_ptr< DEC_Knowledge_Agent > pSource, boost::shared_ptr< DEC_Knowledge_Agent > pTarget );
    static boost::shared_ptr< MT_Vector2D > GetPositionPtr( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static float GetOperationalState                    ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static float GetROE                                 ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  GetMajorOperationalState               ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  IsFlying                               ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static double DEC_ConnaissanceAgent_Altitude        ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  IsKnowledgeValid                       ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool IsSurrenderToMyArmy                     ( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  IsPrisoner                             ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  IsDead                                 ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  IsSurrendered                          ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static int   Lock                                   ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void  Unlock                                 ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void  UnitDecisionalState                    ( const DEC_Knowledge_Agent& callerAgent, const std::string& key, const std::string& value );
    static void  DisableCrowdEffect                     ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, bool disable );

    static double            GetMaterialComposantesProtectionLevel( boost::shared_ptr< DEC_Knowledge_Agent > pTarget, MIL_UrbanObject_ABC* pUrbanBlock );
    static const std::string GetCriticalIntelligence              ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );

    static bool  IsIlluminated                          ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  IsDefinitivelyIlluminated              ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  CanBeIlluminate                        ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool  CanIlluminate                          ( const MIL_AgentPion& callerAgent );
    static unsigned int GetAgentIdFromKnowledge         ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool IsRefugee                               ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool IsTerrorist                             ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool IsTransported                           ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static std::string GetMilPionType                   ( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static boost::shared_ptr< MIL_Mission_ABC > GetMission( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void NeutralizeAgent( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void ForceRadioSilence( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, bool silence );
    static void  ShareKnowledgesWith ( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, float minutes );
    static DEC_Decision_ABC* GetAgent( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );	
    static void SwitchAutomateLogistic( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool IsInObject( const MIL_Entity_ABC& callerAgent, const std::string& objectType, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, int isFriend );
    static bool IsInObjectWithCapacity( const MIL_Entity_ABC& callerAgent, const std::string& capacity, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, int isFriend );
    //@}
};

#endif // __DEC_KnowledgeAgentFunctions_h_
