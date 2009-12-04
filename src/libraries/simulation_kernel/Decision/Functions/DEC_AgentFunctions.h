// *****************************************************************************
//
// $Created: NLD 2004-03-31 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_AgentFunctions.h $
// $Author: Jvt $
// $Modtime: 4/05/05 15:25 $
// $Revision: 28 $
// $Workfile: DEC_AgentFunctions.h $
//
// *****************************************************************************

#ifndef __DEC_AgentFunctions_h_
#define __DEC_AgentFunctions_h_

#include "MIL.h"

class DEC_AutomateDecision;
class DEC_Decision_ABC;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class MIL_AgentPion;
class MIL_Mission_ABC;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_AgentFunctions
{
public:
    //! @name Functions
    //@{
    // Accessors
    static bool IsPC                        ( const MIL_AgentPion& callerAgent );
    static bool IsDead                      ( const MIL_AgentPion& callerAgent );
    static bool IsAutomateEngaged           ( const MIL_AgentPion& callerAgent );
    static bool IsTransported               ( const MIL_AgentPion& callerAgent );
    static bool IsNeutralized               ( const MIL_AgentPion& callerAgent );            
    static MT_Float GetOperationalState     ( const MIL_AgentPion& callerAgent );
    static MT_Float GetMajorOperationalState( const MIL_AgentPion& callerAgent );
    static boost::shared_ptr< MT_Vector2D > GetPosition   ( const MIL_AgentPion& callerAgent );
    static const MT_Vector2D* GetDirection  ( const MIL_AgentPion& callerAgent );
    static void ActivateBlackout            ( MIL_AgentPion& callerAgent );
    static void DeactivateBlackout          ( MIL_AgentPion& callerAgent );
    static int  GetPosture                  ( const MIL_AgentPion& callerAgent );
        
    static bool CanConstructObject          ( const MIL_AgentPion& callerAgent, const std::string& type );
    static bool CanBypassObject             ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge );
    static bool CanDestroyObject            ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge );
    static bool CanMineObject               ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge );
    static bool CanActivateObject           ( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge );
    
    static void EnableDiscreteMode          ( MIL_AgentPion& callerAgent );
    static void DisableDiscreteMode         ( MIL_AgentPion& callerAgent );

    // Embarquement / débarquement
    static bool IsLoaded                          ( const MIL_AgentPion& callerAgent );          
    static MT_Float GetLoadingTime                ( const MIL_AgentPion& callerAgent );
    static MT_Float GetUnloadingTime              ( const MIL_AgentPion& callerAgent );
    static bool AreHumanTransportersReady         ( const MIL_AgentPion& callerAgent );
    static void DisableHumanTransportersNow       ( MIL_AgentPion& callerAgent );
    static void RecoverHumanTransportersNow       ( MIL_AgentPion& callerAgent );

    // Etat décisionnel
    static void NotifyForceRatioStateChanged                 ( MIL_AgentPion& callerAgent, int state );
    static void NotifyRulesOfEngagementStateChanged          ( MIL_AgentPion& callerAgent, int state );
    static void NotifyRulesOfEngagementPopulationStateChanged( MIL_AgentPion& callerAgent, int population );
    static void NotifyCloseCombatStateChanged                ( MIL_AgentPion& callerAgent, int state );
    static void NotifyOperationalStateChanged                ( MIL_AgentPion& callerAgent, int state );
    static void NotifyIndirectFireAvailabilityChanged        ( MIL_AgentPion& callerAgent, int state );

    // Etat des facteurs humains
    static unsigned int GetHumanFactorTiredness ( const MIL_AgentPion& callerAgent );
    static unsigned int GetHumanFactorMorale    ( const MIL_AgentPion& callerAgent );

    // Etats du pion
    static bool IsFlying           ( const MIL_AgentPion& callerAgent );
    static void SetFlyingHeight    ( MIL_AgentPion& callerAgent, MT_Float height );
    static void SetElongationFactor( MIL_AgentPion& callerAgent, MT_Float factor );
    static float TimeLeftForMoving ( const MIL_AgentPion& callerAgent );
    static float TimeToMoveDistance( const MIL_AgentPion& callerAgent, float distance );

    // Hiérarchie
    static std::vector< DEC_Decision_ABC* > GetPionsWithPC   ( const MIL_AgentPion& callerAgent );
    static std::vector< DEC_Decision_ABC* > GetPionsWithoutPC( const MIL_AgentPion& callerAgent );
    static DEC_Decision_ABC* GetPionPC          ( const MIL_AgentPion& callerAgent );
    static DEC_Decision_ABC* GetPionPCOfAutomate( const DEC_Decision_ABC* automat );
    static std::vector< DEC_Decision_ABC* > GetPionsWithoutPCOfAutomate( const DEC_Decision_ABC* automat );
    static bool ChangeAutomate( MIL_AgentPion& callerAgent, DEC_Decision_ABC* automat );
       
    // Interception
    static boost::shared_ptr< MT_Vector2D > GetInterceptionPoint( const MIL_AgentPion& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );

    // NBC
    static bool IsContaminated         ( const MIL_AgentPion& callerAgent );
    static void WearNbcProtectionSuit  ( MIL_AgentPion& callerAgent );
    static void RemoveNbcProtectionSuit( MIL_AgentPion& callerAgent );
    static void SelfDecontaminate      ( MIL_AgentPion& callerAgent );

    // Population
    static int GetRoePopulation( const MIL_AgentPion& callerAgent );
    static void ChannelPopulations( const TER_Localisation* location );

    // Installation
    static bool IsInstalled  ( const MIL_AgentPion& callerAgent );
    static bool IsUninstalled( const MIL_AgentPion& callerAgent );
    static void Install      ( MIL_AgentPion& callerAgent );

    // Missions
    static boost::shared_ptr< MT_Vector2D > GetDirectionDanger( MIL_Mission_ABC* pMission );

    static MIL_Mission_ABC* GetMission( DEC_Decision_ABC* pAgent );
    static void SetMission( DEC_Decision_ABC* pAgent, MIL_Mission_ABC* pMission );

    // Test / debug
    static void DecisionalState( const MIL_AgentPion& callerAgent, const std::string& key, const std::string& value );
    
    static bool RelievePion   ( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* agentToRelieve );
    static bool CanRelievePion( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* agentToRelieve );
    //@}
};

#endif // __DEC_AgentFunctions_h_
