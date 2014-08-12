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

class DEC_Decision_ABC;
class DEC_Knowledge_Agent;
class DEC_Knowledge_Object;
class MIL_Agent_ABC;
class MIL_AgentPion;
class MIL_Mission_ABC;
class PHY_DotationCategory;
class TER_Localisation;
class MT_Vector2D;
class MIL_UrbanObject_ABC;
class DEC_Gen_Object;
class MIL_Object_ABC;
class PHY_ComposanteTypePion;

// =============================================================================
// Created: NLD 2004-03-31
// =============================================================================
class DEC_AgentFunctions
{
public:
    //! @name Functions
    //@{
    // Accessors
    static DEC_Decision_ABC* GetUnitById( uint32_t id );
    static bool IsPC( const MIL_Agent_ABC& callerAgent );
    static bool CanPerformHealthEvacuation( const DEC_Decision_ABC* agent );
    static bool IsDead( const MIL_Agent_ABC& callerAgent );
    static bool IsAutomateEngaged( const MIL_Agent_ABC& callerAgent );
    static bool IsTransported( const MIL_Agent_ABC& callerAgent );
    static bool IsNeutralized( const MIL_Agent_ABC& callerAgent );
    static bool UpdateUnderIndirectFire( MIL_Agent_ABC& callerAgent );
    static bool IsAutonomous( DEC_Decision_ABC* pAgent );
    static double GetOperationalState( const MIL_Agent_ABC& callerAgent );
    static double GetMajorOperationalState( const MIL_Agent_ABC& callerAgent );
    static boost::shared_ptr< MT_Vector2D > GetPosition( const MIL_Agent_ABC& callerAgent );
    static boost::shared_ptr< MT_Vector2D > GetAgentPositionPtr( DEC_Decision_ABC* brain );
    static const MT_Vector2D* GetDirection( const MIL_Agent_ABC& callerAgent );
    static float GetIdentificationDistance( MIL_Agent_ABC& callerAgent );
    static float GetReconnoissanceDistance( MIL_Agent_ABC& callerAgent );
    static float GetDetectionDistance( MIL_Agent_ABC& callerAgent );
    static void ActivateBlackout( MIL_Agent_ABC& callerAgent );
    static void ActivatePartialBlackout( MIL_Agent_ABC& callerAgent, bool report );
    static void DeactivateBlackout( MIL_Agent_ABC& callerAgent );
    static int GetPosture( const MIL_Agent_ABC& callerAgent );
    static int GetNbcSuitLevel( const MIL_Agent_ABC& callerAgent );
    static bool IsInCity( const DEC_Decision_ABC* pAgent );
    static bool IsInCrowd( const MIL_Agent_ABC& callerAgent );
    static std::string GetMilPionType( DEC_Decision_ABC* pion );
    static std::string GetMilPionName( DEC_Decision_ABC* pion );
    static void ImmunizeAgent( MIL_Agent_ABC& callerAgent );  // deprecated
    static void StopImmunizeAgent( MIL_Agent_ABC& callerAgent );  // deprecated
    static double GetMaxSpeed( const DEC_Decision_ABC& agent );
    static bool AgentHasFuseau( const MIL_Agent_ABC&  agent );
    static bool CanConstructObject( const MIL_Agent_ABC& callerAgent, const std::string& type );
    static bool CanConstructObjectWithLocalisation( const MIL_Agent_ABC& callerAgent, const std::string& type, const TER_Localisation* location );
    static bool CanConstructWithoutReinforcement( const MIL_Agent_ABC& callerAgent, const std::string& type );
    static bool CanConstructWithoutReinforcementWithLocalisation( const MIL_Agent_ABC& callerAgent, const std::string& type, const TER_Localisation* location );
    static bool CanConstructObjectWithLoaded( const MIL_Agent_ABC& callerAgent, const std::string& type );
    static bool CanConstructObjectWithLoadedAndLocalisation( const MIL_Agent_ABC& callerAgent, const std::string& type, const TER_Localisation* location );
    static bool HasDotationForBuilding( MIL_Agent_ABC& callerAgent, const std::string& type );
    static bool CanBypassObject( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge );
    static bool CanDestroyObject( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge );
    static bool CanMineObject( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge );
    static bool CanDemineObject( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge );
    static bool CanActivateObject( boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge );
    static void EnableDiscreteMode( MIL_Agent_ABC& callerAgent );
    static void DisableDiscreteMode( MIL_Agent_ABC& callerAgent );
    static bool AgentCanFly( const MIL_Agent_ABC& callerAgent );
    static unsigned int GetStandardFlyingHeight( const DEC_Decision_ABC* callerAgent );
    static unsigned int GetTacticalFlyingHeight( const DEC_Decision_ABC* callerAgent );
    static double GetCurrentSpeed( const DEC_Decision_ABC* agent );
    static void CreateInstantaneously( const DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Gen_Object > genObject );
    static bool PionCanFly( const DEC_Decision_ABC* callerAgent );
    static bool AgentCanConstructObjectWithLoaded( const DEC_Decision_ABC* agent, const std::string& type );
    static bool AgentCanConstructObjectWithLoadedAndLocalisation( const DEC_Decision_ABC* agent, const std::string& type, const TER_Localisation* localisation );
    static bool AgentHasDotationForBuilding(const DEC_Decision_ABC* agent, const std::string& type );
    static bool AgentCanConstructObjectWithOutLoadedWithLocalisation( const DEC_Decision_ABC* agent, const std::string& type, const TER_Localisation* localisation );
    static bool AgentHasDotationForBuildingWithOutLoaded(const DEC_Decision_ABC* agent, const std::string& type );
    static bool HasDotationForBuildingWithoutReinforcement( MIL_Agent_ABC& callerAgent, const std::string& type );
    static bool AgentCanDestroyObject( const DEC_Decision_ABC* agent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge );
    static bool AgentCanBypassObject( const DEC_Decision_ABC* agent, boost::shared_ptr< DEC_Knowledge_Object > objectKnowledge );
    static bool AgentCanDestroyObjectType( const DEC_Decision_ABC* agent, const std::string& type );
    static bool AgentCanDestroyObjectTypeWithLocalisation( const DEC_Decision_ABC* agent, const std::string& type, const TER_Localisation* localisation );
    static bool AgentCanMineObject( const DEC_Decision_ABC* agent, const std::string& type, const TER_Localisation* localisation );
    static bool AgentCanDemineObject( const DEC_Decision_ABC* agent, const std::string& type, const TER_Localisation* localisation );
    static bool AgentCanDemineObjecttWithOutLoaded( const DEC_Decision_ABC* agent, const std::string& type, const TER_Localisation* localisation );
    static bool AgentCanByPassObjectWithLocalisation( const DEC_Decision_ABC* agent, const std::string& type, const TER_Localisation* localisation );
    static bool AgentCanExtinguish( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

    static std::vector< DEC_Decision_ABC* > RetrieveUnitsAbleToBuild( const std::vector< DEC_Decision_ABC* >& units, const std::string& type );
    static std::vector< DEC_Decision_ABC* > RetrieveUnitsAbleToBuildWithLocalisation( const std::vector< DEC_Decision_ABC* >& units, const std::string& type, const TER_Localisation* localisation );
    static std::vector< DEC_Decision_ABC* > RetrieveUnitsAbleToMine( const std::vector< DEC_Decision_ABC* >& units, const std::string& type );
    static std::vector< DEC_Decision_ABC* > RetrieveUnitsAbleToMineWithLocalisation( const std::vector< DEC_Decision_ABC* >& units, const std::string& type, const TER_Localisation* localisation );
    static std::vector< DEC_Decision_ABC* > RetrieveUnitsAbleToDemineWithLocalisation( const std::vector< DEC_Decision_ABC* >& units, const std::string& type, const TER_Localisation* localisation );
    static std::vector< DEC_Decision_ABC* > RetrieveUnitsAbleToDemineWithOutLoadedWithLocalisation( const std::vector< DEC_Decision_ABC* >& units, const std::string& type, const TER_Localisation* localisation );
    static std::vector< DEC_Decision_ABC* > RetrieveUnitsAbleToDestroy( const std::vector< DEC_Decision_ABC* >& units, const std::string& type );
    static std::vector< DEC_Decision_ABC* > RetrieveUnitsAbleToDestroyWithLocalisation( const std::vector< DEC_Decision_ABC* >& units, const std::string& type, const TER_Localisation* localisation );
    static std::vector< DEC_Decision_ABC* > RetrieveUnitsAbleToByPass( const std::vector< DEC_Decision_ABC* >& units, const std::string& type );
    static std::vector< DEC_Decision_ABC* > RetrieveUnitsAbleToByPassWithLocalisation( const std::vector< DEC_Decision_ABC* >& units, const std::string& type, const TER_Localisation* localisation );

    // Getter
    static double GetAgentDotationNumber( const DEC_Decision_ABC* agent, const PHY_DotationCategory* pDotationCategory );
    static std::pair< const PHY_DotationCategory*, double > GetAgentMissingDotationForBuildingObject( const DEC_Decision_ABC* agent, const DEC_Gen_Object* object );
    static std::pair< const PHY_DotationCategory*, double > GetAgentMissingDotationForBuildingExistingObject( const DEC_Decision_ABC* agent, const boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );
    static std::pair< const PHY_DotationCategory*, double > GetAgentMissingDotationForMining( const DEC_Decision_ABC* agent, const boost::shared_ptr< DEC_Knowledge_Object > pKnowledge );

    // Embarquement / débarquement
    static bool IsLoaded( const MIL_Agent_ABC& callerAgent );
    static bool IsUnloaded( const MIL_Agent_ABC& callerAgent );
    static bool HasLoadable( const MIL_Agent_ABC& callerAgent );
    static bool IsLoadable( DEC_Decision_ABC& callerAgent );
    static double GetLoadingTime( const MIL_Agent_ABC& callerAgent );
    static double GetUnloadingTime( const MIL_Agent_ABC& callerAgent );
    static bool AreHumanTransportersReady( const MIL_Agent_ABC& callerAgent );
    static void DisableHumanTransportersNow( MIL_Agent_ABC& callerAgent );
    static void RecoverHumanTransportersNow( MIL_Agent_ABC& callerAgent );
    static bool CanMount( DEC_Decision_ABC& callerAgent );

    // Etat décisionnel
    static int  GetRulesOfEngagementState( const MIL_Agent_ABC& callerAgent );
    static void NotifyForceRatioStateChanged( MIL_Agent_ABC& callerAgent, int state );
    static void NotifyRulesOfEngagementStateChanged( MIL_Agent_ABC& callerAgent, int state );
    static void NotifyRulesOfEngagementPopulationStateChanged( MIL_Agent_ABC& callerAgent, int population );
    static void NotifyCloseCombatStateChanged( MIL_Agent_ABC& callerAgent, int state );
    static void NotifyOperationalStateChanged( MIL_Agent_ABC& callerAgent, int state );
    static void NotifyIndirectFireAvailabilityChanged( MIL_Agent_ABC& callerAgent, int state );

    // Etat des facteurs humains
    static unsigned int GetHumanFactorTiredness( const MIL_Agent_ABC& callerAgent );
    static unsigned int GetHumanFactorMorale( const MIL_Agent_ABC& callerAgent );

    // Etats du pion
    static bool IsFlying( const MIL_Agent_ABC& callerAgent );
    static void SetFlyingHeight( MIL_Agent_ABC& callerAgent, double height );
    static float TimeLeftForMoving( const DEC_Decision_ABC* callerAgent );
    static float TimeToMoveDistance( const MIL_Agent_ABC& callerAgent, float distance );
    static float TheoricTimeToMoveDistance( const DEC_Decision_ABC* pAgent, float distance, bool loaded );
    static void Suicide( MIL_Agent_ABC& callerAgent );
    static bool CanIlluminate( DEC_Decision_ABC* pAgent );
    static float GetIlluminatingRange( const MIL_Agent_ABC&  callerAgent );
    static bool IsImmobilized( DEC_Decision_ABC* pAgent );
    static void DisableCrowdEffect( const DEC_Decision_ABC* agent, bool disable );
    static bool IsJammed( DEC_Decision_ABC* pAgent );
    static bool IsInEmissionBlackout( DEC_Decision_ABC* pAgent );
    static bool IsInReceptionBlackout( DEC_Decision_ABC* pAgent );
    static void SetToAmbianceSafety( MIL_Agent_ABC& callerAgent, bool safety );
    static bool IsInSmoke( DEC_Decision_ABC* pAgent );
    static bool CreateBreakdown( MIL_Agent_ABC& callerAgent, const PHY_ComposanteTypePion* composanteType, unsigned int breakdown );
    static bool IsUnderground( DEC_Decision_ABC& callerAgent );
    static const PHY_ComposanteTypePion* GetEquipmentFromID( unsigned int equipmentID );

    // Etat d'un agent
    static double GetRapForLocalAgent( const DEC_Decision_ABC* agent );

    // Hiérarchie
    static std::vector< DEC_Decision_ABC* > GetPionsWithPC( const MIL_Agent_ABC& callerAgent );
    static std::vector< DEC_Decision_ABC* > GetPionsWithoutPC( const MIL_Agent_ABC& callerAgent );
    static std::vector< DEC_Decision_ABC* > GetCommunicationPionsWithPC( const MIL_Agent_ABC& callerAgent );
    static std::vector< DEC_Decision_ABC* > GetCommunicationPionsWithoutPC( const MIL_Agent_ABC& callerAgent );
    static DEC_Decision_ABC* GetPionPC( const MIL_Agent_ABC& callerAgent );
    static DEC_Decision_ABC* GetPionPCOfAutomate( const DEC_Decision_ABC* automat );
    static std::vector< DEC_Decision_ABC* > GetPionsWithoutPCOfAutomate( const DEC_Decision_ABC* automat );
    static bool ChangeAutomate( MIL_Agent_ABC& callerAgent, DEC_Decision_ABC* automat );

    // Interception
    static boost::shared_ptr< MT_Vector2D > GetInterceptionPoint( const MIL_Agent_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );

    // NBC
    static bool IsAgentNBC( const MIL_Agent_ABC& callerAgent );
    static bool IsContaminated( const MIL_Agent_ABC& callerAgent );
    static bool IsIntoxicated( const MIL_Agent_ABC& callerAgent );
    static void WearNbcProtectionSuit( MIL_Agent_ABC& callerAgent );
    static void RemoveNbcProtectionSuit( MIL_Agent_ABC& callerAgent );
    static void SelfDecontaminate( MIL_Agent_ABC& callerAgent );
    static void TemporaryImmunizeAgent( DEC_Decision_ABC* pAgent, bool bImmunize );

    // Population
    static int GetRoePopulation( const MIL_Agent_ABC& callerAgent );
    static void ChannelPopulations( const TER_Localisation* location );

    //Dotations
    static const PHY_DotationCategory* GetDotation( const MIL_Agent_ABC& caller, unsigned id );
    static bool HasDotation( const MIL_Agent_ABC& callerAgent, const PHY_DotationCategory* category );
    static bool HasDotationForFiring( DEC_Decision_ABC* agent, const PHY_DotationCategory* category, int iterations );
    static bool CanUseDotation( MIL_Agent_ABC& callerAgent, const PHY_DotationCategory* category );
    static double GetFuelDotationNumber( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion );
    static double GetFuelDotationCapacity( MIL_AgentPion& callerAgent, const DEC_Decision_ABC* pPion );
    static void ResupplyFuel( DEC_Decision_ABC* pAgent, float percentage );
    static bool AgentHasRadar( const DEC_Decision_ABC* agent, int typeRadar );

    // Old deployment
    static double GetInstallationTime( DEC_Decision_ABC& callerAgent );   // $$$$ ABR 2011-12-15: Old method, should be removed soon
    static double GetUninstallationTime( DEC_Decision_ABC& callerAgent ); // $$$$ ABR 2011-12-15: Old method, should be removed soon
    // Deployment
    static bool IsDeployed( const MIL_Agent_ABC& callerAgent );
    static bool IsUndeployed( const MIL_Agent_ABC& callerAgent );
    static void Deploy( MIL_Agent_ABC& callerAgent );
    static void Undeploy( MIL_Agent_ABC& callerAgent );

    // Installation
    static bool IsInstalled( const MIL_Agent_ABC& callerAgent );
    static bool IsUninstalled( const MIL_Agent_ABC& callerAgent );
    static void Install( MIL_Agent_ABC& callerAgent );

    // Missions
    static boost::shared_ptr< MT_Vector2D > GetDirectionDanger( const boost::shared_ptr< MIL_Mission_ABC > pMission );

    static boost::shared_ptr< MIL_Mission_ABC > GetMission( DEC_Decision_ABC* pAgent );
    static bool HasMission( DEC_Decision_ABC* pAgent );

    // Test / debug
    static void DecisionalState( const MIL_Agent_ABC& callerAgent, const std::string& key, const std::string& value );

    // Relève
    static bool RelievePion( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* agentToRelieve );
    static bool CanRelievePion( MIL_Agent_ABC& callerAgent, const DEC_Decision_ABC* agentToRelieve );

    // Surrender target
    static bool SurrenderAutomat( MIL_Agent_ABC& callerAgent, DEC_Decision_ABC* automat );

    // Populations couvrantes
    static void AlertInhabitants( const TER_Localisation* location ); // $$$$ BCI 2011-02-01: je ne sais pas trop comment nommer ou classer cette méthode : le terme "population" fait parfois référence aux foules, parfois aux populations...
    static void UndoAlertInhabitants( const TER_Localisation* location );
    static bool IsInhabitantsAlerted( const TER_Localisation* location ); // $$$$ BCI 2011-02-01: je ne sais pas trop comment nommer ou classer cette méthode : le terme "population" fait parfois référence aux foules, parfois aux populations...
    static void ConfineInhabitants( const TER_Localisation* location ); // $$$$ BCI 2011-02-01: je ne sais pas trop comment nommer ou classer cette méthode : le terme "population" fait parfois référence aux foules, parfois aux populations...
    static void UndoConfineInhabitants( const TER_Localisation* location );
    static bool IsInhabitantsConfined( const TER_Localisation* location );
    static bool UrbanBlockIsPopulated( const MIL_UrbanObject_ABC* pUrbanBlock );
    static void EvacuateInhabitants( const TER_Localisation* location );
    static void UndoEvacuateInhabitants( const TER_Localisation* location );
    static bool IsInhabitantsEvacuated( const TER_Localisation* location );

    static void EnableSharedPerception( const MIL_AgentPion& callerAgent, DEC_Decision_ABC* pAgent );
    static void DisabledSharedPerception( const MIL_AgentPion& callerAgent, DEC_Decision_ABC* pAgent );
    static void EnableSharedPerceptionWithKnowledge( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void DisabledSharedPerceptionWithKnowledge( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void EnableSharingKnowledges( const MIL_AgentPion& callerAgent, DEC_Decision_ABC* pAgent );
    static void DisabledSharingKnowledges( const MIL_AgentPion& callerAgent, DEC_Decision_ABC* pAgent );
    static void KnowledgeEnableSharedPerceptionWithKnowledge( DEC_Decision_ABC& callerAgent, DEC_Decision_ABC* pAgent );
    static void KnowledgeDisabledSharedPerceptionWithKnowledge( DEC_Decision_ABC& callerAgent, DEC_Decision_ABC* pAgent );
    static void EnableSharingKnowledgesWithKnowledge( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static void DisabledSharingKnowledgesWithKnowledge( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge );
    static bool KnowledgeCommunicate( DEC_Decision_ABC& callerAgent, DEC_Decision_ABC* pAgent );

    // Perception
    static void IdentifyAllAgentsInZone( MIL_Agent_ABC& callerAgent, const TER_Localisation* location );
    //@}
};

#endif // __DEC_AgentFunctions_h_
