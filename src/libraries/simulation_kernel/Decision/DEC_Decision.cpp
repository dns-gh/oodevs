// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Decision.h"
#include "DEC_Tools.h"
#include "Entities/Orders/MIL_MissionParameter_ABC.h"
#include "Entities/Orders/MIL_ParameterType_ABC.h"
#include "Decision/DEC_ActionFunctions.h"
#include "Decision/DEC_AgentFunctions.h"
#include "Decision/DEC_AutomateFunctions.h"
#include "Decision/DEC_CommunicationFunctions.h"
#include "Decision/DEC_DIAFunctions.h"
#include "Decision/DEC_EntityFunctions.h"
#include "Decision/DEC_FireFunctions.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Decision/DEC_KnowledgeAgentFunctions.h"
#include "Decision/DEC_KnowledgeFunctions.h"
#include "Decision/DEC_KnowledgeObjectFunctions.h"
#include "Decision/DEC_KnowledgePopulationFunctions.h"
#include "Decision/DEC_LogisticFunctions.h"
#include "Decision/DEC_MiscFunctions.h"
#include "Decision/DEC_ObjectFunctions.h"
#include "Decision/DEC_Objective.h"
#include "Decision/DEC_OrdersFunctions.h"
#include "Decision/DEC_PathFunctions.h"
#include "Decision/DEC_PerceptionFunctions.h"
#include "Decision/DEC_PopulationFunctions.h"
#include "Decision/DEC_ResourceNetworkFunctions.h"
#include "Decision/DEC_TelepathyFunctions.h"
#include "Decision/DEC_Gen_Object.h"
#include "Decision/DEC_UrbanObjectFunctions.h"
#include "Decision/DEC_TerrainFunctions.h"
#include "Decision/DEC_Logger.h"
#include "DEC_ResourceNetwork.h"
#include "Entities/Actions/PHY_ActionInterrogate.h"
#include "Entities/Agents/Actions/ComposanteLending/PHY_ActionLendCollectionComposantes.h"
#include "Entities/Agents/Actions/ComposanteLending/PHY_ActionLendHaulerComposantes.h"
#include "Entities/Agents/Actions/ComposanteLending/PHY_ActionLendSpecificComposantes.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionConstructObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionConsumeResources.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionPrepareObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionDestroyObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionMineObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionDemineObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionExtinguishObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionBypassObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionOccupyObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionAnimateObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionDeteriorateUrbanBlock.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionResumeWorkObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionResumeWorkUrbanBlock.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionDistributeObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionSupplyObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionExtractFromStockObject.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Position.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Knowledge.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionOnMajorComposantes.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionControlZone.h"
#include "Entities/Agents/Actions/Firing/Illumination/PHY_ActionIllumination.h"
#include "Entities/Agents/Actions/CrowdTransport/PHY_ActionLoadCrowd.h"
#include "Entities/Agents/Actions/CrowdTransport/PHY_ActionUnloadCrowd.h"
#include "Entities/Agents/Actions/Transport/PHY_ActionTransportLoad.h"
#include "Entities/Agents/Actions/Transport/PHY_ActionTransportUnload.h"
#include "Entities/Agents/Actions/Loading/PHY_ActionLoad.h"
#include "Entities/Agents/Actions/Loading/PHY_ActionUnload.h"
#include "Entities/Agents/Actions/Emergency/PHY_ActionInfluence.h"
#include "Entities/Agents/Actions/Emergency/PHY_ActionInfluenceInArea.h"
#include "Entities/Agents/Actions/Emergency/PHY_ActionTriggerActivity.h"
#include "Entities/Agents/Actions/Emergency/PHY_ActionTriggerActivityInArea.h"
#include "Entities/Agents/Actions/Emergency/PHY_ActionUnloadActivity.h"
#include "Entities/Agents/Actions/Underground/PHY_ActionMoveUnderground.h"
#include "Entities/Agents/Actions/Moving/PHY_ActionMove.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePion.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionUsingOnlyLoadable.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionUsingOnlyCarrier.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionOnMajorComposantes.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePopulation.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_FragOrder.h"
#include "Entities/Orders/MIL_MissionParameterFactory.h"
#include "Entities/Orders/MIL_MissionParameterVisitor_ABC.h"
#include "Entities/Orders/MIL_OrderTypeParameter.h"
#include "Entities/Populations/Actions/PHY_Population_ActionMove.h"
#include "Entities/Populations/Actions/PHY_Population_ActionMoveAlong.h"
#include "Entities/Populations/Actions/PHY_Population_ActionFireOnPion.h"
#include "Entities/Populations/Actions/PHY_Population_ActionFireOnPions.h"
#include "Entities/Populations/Actions/PHY_Population_ActionBattle.h"
#include "Entities/Populations/Actions/PHY_Population_ActionUrbanDestruction.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "simulation_terrain/TER_PathPoint.h"
#include <directia/tools/binders/ScriptRef.h>
#include <geometry/Point2.h>

// -----------------------------------------------------------------------------
// Name: ScriptRefs
// Created: LDC 2009-04-08
// -----------------------------------------------------------------------------
ScriptRefs::ScriptRefs( sword::Brain& brain  )
    : startEvent_             ( brain.GetScriptRef( "StartEvent" ) )
    , stopEvents_             ( brain.GetScriptRef( "StopEvents" ) )
    , setStateVariable_       ( brain.GetScriptRef( "SetStateVariable" ) )
    , collectgarbage_         ( brain.GetScriptRef( "collectgarbage" ) )
    , step_                   ( brain.GetScriptRef( "step" ) )
    , callbackPerception_     ( brain.GetScriptRef( "CallbackPerception" ) )
    , knowledgeCallbackAction_( brain.GetScriptRef( "KnowledgeCallbackAction" ) )
    , removeAction_           ( brain.GetScriptRef( "RemoveAction" ) )
    , initTaskParameter_      ( brain.GetScriptRef( "InitTaskParameter" ) )
    , activateBrainDebug_     ( brain.GetScriptRef( "ActivateBrainDebug" ) )
{
    // NOTHING
}

namespace DEC_DecisionImpl
{

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterUnitFunctions
// Created: LMT 2010-12-02
// -----------------------------------------------------------------------------
void RegisterUnitFunctions( sword::Brain& brain )
{
    brain.RegisterMethod( "DEC_Agent_EtatOps", &DEC_Decision_ABC::GetOperationalState );
    brain.RegisterMethod( "DEC_Agent_EtatOpsMajeur", &DEC_Decision_ABC::GetMajorOperationalState );
    brain.RegisterMethod( "DEC_Agent_EstPC", &DEC_Decision_ABC::IsPC );
    brain.RegisterMethod( "DEC_Agent_EstTransporte", &DEC_Decision_ABC::IsTransported );
    brain.RegisterMethod( "DEC_Agent_EstEnVol", &DEC_Decision_ABC::IsFlying );
    brain.RegisterMethod( "DEC_Agent_EstEnMouvement", &DEC_Decision_ABC::IsMoving );
    brain.RegisterMethod( "DEC_Agent_EstNeutralise", &DEC_Decision_ABC::IsNeutralized );
    brain.RegisterMethod( "DEC_Agent_AutomateEstEmbraye", &DEC_Decision_ABC::IsAutomateEngaged );
    brain.RegisterMethod( "DEC_Agent_Position", &DEC_Decision_ABC::GetPosition );
    brain.RegisterMethod( "DEC_Agent_EstMort", &DEC_Decision_ABC::IsDead );
    brain.RegisterMethod( "DEC_Agent_EstContamine", &DEC_Decision_ABC::IsContaminated );
    brain.RegisterMethod( "DEC_Agent_IsDecontaminationFinished", &DEC_Decision_ABC::IsDecontaminationFinished );
    brain.RegisterMethod( "DEC_Agent_EstBrouille", &DEC_Decision_ABC::IsJammed );
    brain.RegisterMethod( "DEC_Agent_EstEnSilenceRadioEmission", &DEC_Decision_ABC::IsInEmissionBlackout );
    brain.RegisterMethod( "DEC_Agent_EstEnSilenceRadioReception", &DEC_Decision_ABC::IsInReceptionBlackout );
    brain.RegisterMethod( "DEC_Pion_PionsAvecPC", &DEC_Decision_ABC::GetPionsWithPC );
    brain.RegisterMethod( "DEC_Pion_PionsAvecPCCommunication", &DEC_Decision_ABC::GetCommunicationPionsWithPC );
    brain.RegisterMethod( "DEC_Automate_PionsAvecPC", &DEC_Decision_ABC::GetPionsWithPC );
    brain.RegisterMethod( "DEC_Automate_PionsAvecPCCommunication", &DEC_Decision_ABC::GetCommunicationPionsWithPC );
    brain.RegisterFunction( "DEC_Automate_PionPCDeAutomate", &DEC_AutomateFunctions::GetPionPCOfAutomate );
    brain.RegisterFunction( "DEC_Automate_PionsDeAutomateSansPC", &DEC_AutomateFunctions::GetPionsOfAutomateWithoutPC );
    brain.RegisterFunction( "DEC_Automate_PionsDeAutomateAvecPC", &DEC_AutomateFunctions::GetAutomatPionsWithPC );
    brain.RegisterFunction( "DEC_Automate_PionsDeAutomateSansPCCommunication", &DEC_AutomateFunctions::GetCommunicationPionsOfAutomateWithoutPC );
    brain.RegisterFunction( "DEC_Automate_PionsDeAutomateAvecPCCommunication", &DEC_AutomateFunctions::GetCommunicationAutomatPionsWithPC );
    brain.RegisterMethod( "DEC_Automate_EstEmbraye", &DEC_Decision_ABC::IsAutomateEngaged );
    brain.RegisterFunction( "DEC_Connaissances_UnitesDansZone", &DEC_KnowledgeFunctions::GetAllAgentsInZone );
    brain.RegisterFunction( "DEC_Connaissances_UnitesPrenantAPartieUnAmi", &DEC_KnowledgeFunctions::GetEnemiesAttacking );
    brain.RegisterFunction( "DEC_Connaissances_UniteLaPlusProcheDunAmi", &DEC_KnowledgeFunctions::GetNearestToFriend );
    brain.RegisterFunction( "DEC_Connaissances_UnitesEnnemiesDansZone", &DEC_KnowledgeFunctions::GetEnemyAgentsInZone );

    brain.RegisterFunction( "DEC_Agent_Renforts", &DEC_MiscFunctions::GetAgentReinforcements );
    brain.RegisterFunction( "DEC_Agent_NombreRenforts", &DEC_MiscFunctions::GetAgentReinforcementsNumber );
    brain.RegisterFunction( "DEC_Agent_AgentEstEnVille", &DEC_AgentFunctions::IsInCity );
    brain.RegisterFunction( "DEC_Pion_PionPCDeAutomate", &DEC_AgentFunctions::GetPionPCOfAutomate );
    brain.RegisterFunction( "DEC_Pion_PionsDeAutomateSansPC", &DEC_AgentFunctions::GetPionsWithoutPCOfAutomate );
    brain.RegisterFunction( "DEC_GetAutomate", &DEC_MiscFunctions::GetAutomate );
    brain.RegisterFunction( "DEC_Pion_ChangeAutomate", &DEC_AutomateFunctions::PionChangeAutomate );
    brain.RegisterFunction( "DEC_Automate_PionPosition", &DEC_AutomateFunctions::GetPionPosition );
    brain.RegisterFunction( "DEC_Automate_PerceptionPourPion", &DEC_AutomateFunctions::GetPerceptionForPion );
    brain.RegisterFunction( "DEC_Automate_CalculerPositionParRapportALimaPourPion", &DEC_AutomateFunctions::ComputePointBeforeLimaForPion );
    brain.RegisterFunction( "DEC_Automate_PionTempsPourParcourirDistanceEnLigneDroite", &DEC_AutomateFunctions::PionTimeToMoveDistance );
    brain.RegisterFunction( "DEC_PionTempsTheoriquePourParcourirDistanceEnLigneDroite", &DEC_AgentFunctions::TheoricTimeToMoveDistance );
    brain.RegisterFunction( "DEC_AutonomieEnDeplacement", &DEC_AgentFunctions::TimeLeftForMoving );
    brain.RegisterFunction( "DEC_GetRawMission", &DEC_AutomateFunctions::GetMission );
    brain.RegisterFunction( "DEC_SetMission", &DEC_AutomateFunctions::SetMission );
    brain.RegisterFunction( "DEC_GetDirectionDanger", &DEC_AgentFunctions::GetDirectionDanger );
    brain.RegisterFunction( "DEC_IsMissionPionAvailable", &DEC_OrdersFunctions::IsPionMissionAvailable );
    brain.RegisterFunction( "DEC_IsAutomateMissionPionAvailable", &DEC_OrdersFunctions::IsAutomateMissionAvailable );
    brain.RegisterFunction( "DEC_Pion_GetMilPionType", &DEC_AgentFunctions::GetMilPionType );
    brain.RegisterFunction( "DEC_Pion_GetMilPionName", &DEC_AgentFunctions::GetMilPionName );
    brain.RegisterFunction( "DEC_Agent_EstImmobilise", &DEC_AgentFunctions::IsImmobilized );
    brain.RegisterFunction( "DEC_Agent_EstAutonome", &DEC_AgentFunctions::IsAutonomous );
    brain.RegisterFunction( "DEC_Agent_CanMoveOn", &DEC_TerrainFunctions::CanMoveOn );

    brain.RegisterFunction( "DEC_Agent_ForcerImmunisationNbc", &DEC_AgentFunctions::TemporaryImmunizeAgent );
    brain.RegisterFunction( "DEC_Agent_ARadar", &DEC_AgentFunctions::AgentHasRadar );
    brain.RegisterFunction( "DEC_Agent_RapportDeForceLocal", &DEC_AgentFunctions::GetRapForLocalAgent );
    brain.RegisterFunction( "DEC_HasMission", &DEC_AgentFunctions::HasMission );

    brain.RegisterFunction( "DEC_Agent_GetTempsDeploiement", &DEC_AgentFunctions::GetInstallationTime );     // $$$$ ABR 2011-12-15: Old method, should be removed soon
    brain.RegisterFunction( "DEC_Agent_GetTempsDedeploiement", &DEC_AgentFunctions::GetUninstallationTime ); // $$$$ ABR 2011-12-15: Old method, should be removed soon

    brain.RegisterFunction( "DEC_GenObject_CreateInstantaneously", &DEC_AgentFunctions::CreateInstantaneously );
    brain.RegisterFunction( "DEC_Agent_CanPerformHealthEvacuation", &DEC_AgentFunctions::CanPerformHealthEvacuation );
    brain.RegisterFunction( "DEC_Agent_EstBrouille", &DEC_AgentFunctions::IsJammed );
    brain.RegisterFunction( "DEC_Agent_EstEnSilenceRadioEmission", &DEC_AgentFunctions::IsInEmissionBlackout );
    brain.RegisterFunction( "DEC_Agent_EstEnSilenceRadioReception", &DEC_AgentFunctions::IsInReceptionBlackout );
    brain.RegisterFunction( "DEC_Agent_IsInSmoke", &DEC_AgentFunctions::IsInSmoke );
    brain.RegisterFunction( "DEC_Agent_GetCurrentSpeed", &DEC_AgentFunctions::GetCurrentSpeed );
    brain.RegisterFunction( "DEC_Agent_DisableCrowdEffect", &DEC_AgentFunctions::DisableCrowdEffect );
    brain.RegisterFunction( "DEC_Agent_PionCanFly", std::function< bool( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::PionCanFly, _1 ) ) );
    brain.RegisterFunction( "DEC_Agent_GetStandardFlyingHeight", &DEC_AgentFunctions::GetStandardFlyingHeight );
    brain.RegisterFunction( "DEC_Agent_GetTacticalFlyingHeight", &DEC_AgentFunctions::GetTacticalFlyingHeight );
    brain.RegisterFunction( "DEC_Agent_MaxSpeed", &DEC_AgentFunctions::GetMaxSpeed );
    brain.RegisterFunction( "DEC_GetUnitById", &DEC_AgentFunctions::GetUnitById );
    brain.RegisterFunction( "DEC_Agent_AgentPeutTransporterPion", &DEC_ActionFunctions::AgentCanTransportPion );
    brain.RegisterFunction( "DEC_GetEquipmentFromID", &DEC_AgentFunctions::GetEquipmentFromID );
    brain.RegisterFunction( "DEC_Agent_IsUnderground", &DEC_AgentFunctions::IsUnderground );
    brain.RegisterFunction( "DEC_Agent_GetTransportedUnits", &DEC_ActionFunctions::GetTransportedUnits );
    brain.RegisterFunction( "DEC_Agent_EstCompletementEmbarquable", &DEC_AgentFunctions::IsLoadable );
    brain.RegisterFunction( "DEC_Agent_EstValide", &DEC_AgentFunctions::IsValid );
    
    brain.RegisterFunction( "DEC_Tir_LancerFumigeneSurPosition",
        std::function< double( const DEC_Decision_ABC&, boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_FireFunctions::ThrowSmokeOnPosition, _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Tir_PorteeMaxPourTirer", &DEC_FireFunctions::GetMaxRangeToFireDecision );
    brain.RegisterFunction( "DEC_Tir_PorteeMaxTirIndirectSansChoisirMunition", &DEC_FireFunctions::GetMaxRangeToIndirectFireDecision );
    
    brain.RegisterFunction( "DEC_Agent_PeutIllumine", &DEC_AgentFunctions::CanIlluminate );
    brain.RegisterFunction( "DEC_CanMount", &DEC_AgentFunctions::CanMount );
    brain.RegisterFunction( "DEC_GetDirectionEnnemi", &DEC_AgentFunctions::GetDirectionDanger );
    brain.RegisterFunction( "DEC_GetRawMission", &DEC_AgentFunctions::GetMission );
    brain.RegisterFunction( "DEC_HasDotationForFiring", &DEC_AgentFunctions::HasDotationForFiring );    

    brain.RegisterFunction( "DEC_Automate_CalculerPointProcheLocalisationDansFuseauPourPion", &DEC_AutomateFunctions::ComputePionNearestLocalisationPointInFuseau );
    brain.RegisterFunction( "DEC_Automate_GetEfficacite", &DEC_AutomateFunctions::GetPionEfficiency );

    brain.RegisterFunction( "DEC_DonnerMissionADAAutomate", &DEC_OrdersFunctions::GiveMissionToAutomat );

    brain.RegisterMethod( "DEC_Automate_PionEstNeutralise", &DEC_AutomateDecision::IsPionNeutralized );

    // Elongation factor concept was removed but we keep the DEC function around
    // to avoid breaking old models. This should be removed after their migration.
    brain.RegisterFunction( "DEC_Elongation"     , std::function< void(double) >( []( double ) {} ));

    brain.RegisterFunction( "DEC_EnableSharedPerceptionWithKnowledge", &DEC_AgentFunctions::EnableSharedPerceptionWithKnowledge );
    brain.RegisterFunction( "DEC_DisabledSharedPerceptionWithKnowledge", &DEC_AgentFunctions::DisabledSharedPerceptionWithKnowledge );
    brain.RegisterFunction( "DEC_Knowledge_EnableSharedPerceptionWithKnowledge", &DEC_AgentFunctions::KnowledgeEnableSharedPerceptionWithKnowledge );
    brain.RegisterFunction( "DEC_Knowledge_DisabledSharedPerceptionWithKnowledge", &DEC_AgentFunctions::KnowledgeDisabledSharedPerceptionWithKnowledge );
    brain.RegisterFunction( "DEC_EnableSharingKnowledgesWithKnowledge", &DEC_AgentFunctions::EnableSharingKnowledgesWithKnowledge );
    brain.RegisterFunction( "DEC_DisabledSharingKnowledgesWithKnowledge", &DEC_AgentFunctions::DisabledSharingKnowledgesWithKnowledge );
    brain.RegisterFunction( "DEC_Knowledge_CommunicateWithKnowledgeGroup", &DEC_AgentFunctions::KnowledgeCommunicate );

    brain.RegisterFunction( "_DEC_Agent_NiveauInstallation", &DEC_AgentFunctions::GetPosture );
    brain.RegisterFunction( "_DEC_Agent_EstDansFoule", &DEC_AgentFunctions::IsInCrowd );
}

void RegisterAutomatonFunctions( sword::Brain& brain )
{
    // Accessors
    brain.RegisterFunction( "_DEC_Automate_PionsSansPC", &DEC_AutomateFunctions::GetPionsWithoutPC );
    brain.RegisterFunction( "_DEC_Automate_PionPC", &DEC_AutomateFunctions::GetPionPC );
    brain.RegisterFunction( "_DEC_Automate_AutomatesSubordonnes", &DEC_AutomateFunctions::GetAutomates );
    brain.RegisterFunction( "_DEC_Automate_PionsMelee", &DEC_AutomateFunctions::GetPionsMelee );
    brain.RegisterFunction( "_DEC_Automate_PionsGenie", &DEC_AutomateFunctions::GetPionsGenie );
    brain.RegisterFunction( "_DEC_AutomateSuperieur_EstEmbraye", &DEC_AutomateFunctions::IsParentAutomateEngaged );
    brain.RegisterFunction( "_DEC_Automate_PionsAvecPCCommunication", &DEC_AutomateFunctions::GetCommunicationPionsWithPC );
    brain.RegisterFunction( "_DEC_Automate_PionsSansPCCommunication", &DEC_AutomateFunctions::GetCommunicationPionsWithoutPC );

    // State
    brain.RegisterFunction( "_DEC_Automate_ROE", &DEC_AutomateFunctions::GetRulesOfEngagementState );
    brain.RegisterFunction( "_DEC_Automate_ChangeEtatROE", &DEC_AutomateFunctions::NotifyRulesOfEngagementStateChanged );
    brain.RegisterFunction( "_DEC_Automate_ChangeEtatROEPopulation", &DEC_AutomateFunctions::NotifyRulesOfEngagementPopulationStateChanged );
    brain.RegisterFunction( "_DEC_Automate_ROEPopulation", &DEC_AutomateFunctions::GetRoePopulation );
    brain.RegisterFunction( "_DEC_Automate_isLogistic", &DEC_AutomateFunctions::IsLogistic );

    // Agent accessors
    brain.RegisterFunction( "_DEC_Automate_PionEstContamine", &DEC_AutomateFunctions::IsPionContaminated );
    brain.RegisterFunction( "_DEC_Automate_PionEstEmpoisonne", &DEC_AutomateFunctions::IsPionIntoxicated );
    brain.RegisterFunction( "_DEC_Automate_PionEstNeutralise", &DEC_AutomateFunctions::IsPionNeutralized );
    brain.RegisterFunction( "_DEC_Automate_PionEstTransporte", &DEC_AutomateFunctions::IsPionTransported );
    brain.RegisterFunction( "_DEC_Automate_PionRelevePion",  &DEC_AutomateFunctions::MakePionRelievePion );
    brain.RegisterFunction( "_DEC_Automate_PionPeutReleverPion",  &DEC_AutomateFunctions::CanPionRelievePion );
    brain.RegisterFunction( "_DEC_Automate_EstPointDansFuseauPion", &DEC_AutomateFunctions::IsPointInPionFuseau );
    brain.RegisterFunction( "_DEC_Automate_PionPeutConstruireObjet", &DEC_AutomateFunctions::CanPionConstructObject );
    brain.RegisterFunction( "_DEC_Automate_PionPeutConstruireObjetAvecLocalisation", &DEC_AutomateFunctions::CanPionConstructObjectWithLocalisation );
    brain.RegisterFunction( "_DEC_Automate_PionPeutConstruireContournementObjet", &DEC_AutomateFunctions::CanPionBypassObject );
    brain.RegisterFunction( "_DEC_Automate_PionPeutDetruireObjet", &DEC_AutomateFunctions::CanPionDestroyObject );
    brain.RegisterFunction( "_DEC_Automate_PionPeutDevaloriserObjet", &DEC_AutomateFunctions::CanPionDemineObject );

    // Logistics
    brain.RegisterFunction( "_DEC_Automate_PcDeTC2", &DEC_LogisticFunctions::AutomateGetPcTC2 );
    brain.RegisterFunction( "_DEC_DemandeDeRavitaillement", &DEC_LogisticFunctions::AutomateRequestSupply );
}

// -----------------------------------------------------------------------------
// Name: RegisterPopulationFunctions
// Created: LDC 2011-05-05
// -----------------------------------------------------------------------------
void RegisterPopulationFunctions( sword::Brain& brain )
{
    // Population
    brain.RegisterFunction( "DEC_Agent_CanaliserPopulation", &DEC_AgentFunctions::ChannelPopulations );
    brain.RegisterFunction( "DEC_GetConcentrationLaPlusProche", &DEC_KnowledgePopulationFunctions::GetClosestConcentration );
    brain.RegisterFunction( "DEC_CrowdKnowledge_IsValid", &DEC_KnowledgePopulationFunctions::IsValid );
    brain.RegisterFunction( "DEC_GetPositionConcentration", &DEC_KnowledgePopulationFunctions::GetConcentrationPosition  );
    brain.RegisterFunction( "DEC_GetNombrePersonnesDansConcentration", &DEC_KnowledgePopulationFunctions::GetAllHumansInConcentration );
    brain.RegisterFunction( "DEC_GetNombrePersonnesDansFoule", &DEC_KnowledgePopulationFunctions::GetAllHumans );
    brain.RegisterFunction( "DEC_GetKnowledgeBrain", &DEC_KnowledgePopulationFunctions::GetBrain );
    brain.RegisterFunction( "DEC_ConnaissancePopulation_HasFlow", &DEC_KnowledgePopulationFunctions::IsFlowKnown );
    brain.RegisterFunction( "DEC_CrowdKnowledge_CanLoadCrowdConcentration", &DEC_KnowledgePopulationFunctions::CanLoadCrowdConcentration );
    brain.RegisterFunction( "DEC_GetDomination", &DEC_PopulationFunctions::GetDominationState );
    brain.RegisterFunction( "DEC_ConnaissancePopulation_PointEnTeteDeFlux",
        std::function< boost::shared_ptr<MT_Vector2D> (const DEC_Decision_ABC&, int) >(boost::bind( &DEC_KnowledgePopulationFunctions::GetFlowHead, _1, _2 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_RoePopulation",
        std::function< int ( int ) > ( boost::bind(&DEC_PopulationFunctions::GetKnowledgeAgentRoePopulation, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Localisation", &DEC_PopulationFunctions::GetKnowledgeObjectLocalisation );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Degrader", &DEC_PopulationFunctions::DamageObject );

    // Inhabitants => je ne sais pas trop comment nommer ou classer cette m�thode : le terme "population" fait parfois r�f�rence aux foules, parfois aux populations...
    brain.RegisterFunction( "DEC_Agent_Alert", &DEC_AgentFunctions::AlertInhabitants );
    brain.RegisterFunction( "DEC_Agent_UndoAlert", &DEC_AgentFunctions::UndoAlertInhabitants );
    brain.RegisterFunction( "DEC_Agent_IsAlerted", &DEC_AgentFunctions::IsInhabitantsAlerted );
    brain.RegisterFunction( "DEC_Agent_Confine", &DEC_AgentFunctions::ConfineInhabitants );
    brain.RegisterFunction( "DEC_Agent_UndoConfine", &DEC_AgentFunctions::UndoConfineInhabitants );
    brain.RegisterFunction( "DEC_Agent_IsConfined", &DEC_AgentFunctions::IsInhabitantsConfined );
    brain.RegisterFunction( "DEC_Agent_UrbanBlockIsPopulated", &DEC_AgentFunctions::UrbanBlockIsPopulated );
    brain.RegisterFunction( "DEC_Agent_Evacuate", &DEC_AgentFunctions::EvacuateInhabitants );
    brain.RegisterFunction( "DEC_Agent_UndoEvacuate", &DEC_AgentFunctions::UndoEvacuateInhabitants );
    brain.RegisterFunction( "DEC_Agent_IsEvacuated", &DEC_AgentFunctions::IsInhabitantsEvacuated );

    // Self
    brain.RegisterFunction( "_DEC_GetPosition", &DEC_PopulationFunctions::GetBarycenter );
    brain.RegisterFunction( "_DEC_GetNombrePersonne",  &DEC_PopulationFunctions::GetHealthyHumans );
    brain.RegisterFunction( "_DEC_GetNombrePersonneContaminee", &DEC_PopulationFunctions::GetContaminatedHumans );

    // Security
    brain.RegisterFunction( "_DEC_GetUrbanBlockAngriness", &DEC_PopulationFunctions::GetUrbanBlockAngriness );
    brain.RegisterFunction( "_DEC_ReintegrateUrbanBlock", &DEC_PopulationFunctions::ReintegrateUrbanBlock );
    brain.RegisterFunction( "_DEC_Population_HealWounded", &DEC_PopulationFunctions::HealWounded );

    // Move
    brain.RegisterFunction( "_DEC_HasFlow", &DEC_PopulationFunctions::HasFlow );
    brain.RegisterFunction( "_DEC_Population_HasReachedBlockBorder", &DEC_PopulationFunctions::HasReachedBlockBorder );
    brain.RegisterFunction( "_DEC_Population_HasReachedDestination", &DEC_PopulationFunctions::HasReachedDestination );
    brain.RegisterFunction( "_DEC_Population_HasReachedDestinationCompletely", &DEC_PopulationFunctions::HasReachedDestinationCompletely );

    // Etats decisionnel
    brain.RegisterFunction( "_DEC_Population_ChangeEtatDomination", &DEC_PopulationFunctions::NotifyDominationStateChanged );
    brain.RegisterFunction( "_DEC_Population_Morts", &DEC_PopulationFunctions::GetDeadHumans );

    // Effects
    brain.RegisterFunction( "_DEC_Population_RalentissementPion_ChangeVitesse", &DEC_PopulationFunctions::SetPionMaxSpeed );
    brain.RegisterFunction( "_DEC_Population_RalentissementPion_VitesseParDefaut", &DEC_PopulationFunctions::ResetPionMaxSpeed );
    brain.RegisterFunction( "_DEC_Population_ChangerAttitude", &DEC_PopulationFunctions::SetAttitude );
    brain.RegisterFunction( "_DEC_Population_Attitude", &DEC_PopulationFunctions::GetAttitude );
    brain.RegisterFunction( "_DEC_Population_Positions", &DEC_PopulationFunctions::GetCurrentLocations );
    brain.RegisterFunction( "_DEC_Population_ChangeUrbanDestructionState", &DEC_PopulationFunctions::SetUrbanDestructionState );
    brain.RegisterFunction( "_DEC_Population_UrbanDestructionState", &DEC_PopulationFunctions::GetUrbanDestructionState );
    brain.RegisterFunction( "_DEC_Population_ChangeDemonstrationState", &DEC_PopulationFunctions::SetDemonstrationState );
    brain.RegisterFunction( "_DEC_Population_DemonstrationState", &DEC_PopulationFunctions::GetDemonstrationState );

    // Knowledge objects
    brain.RegisterFunction( "_DEC_ObjectKnowledgesInCircle", &DEC_PopulationFunctions::GetObjectsInCircle );
    brain.RegisterFunction( "_DEC_ConnaissanceObjet_Distance", &DEC_PopulationFunctions::GetKnowledgeObjectDistance );
    brain.RegisterFunction( "_DEC_ConnaissanceObjet_PointPlusProche", &DEC_PopulationFunctions::GetKnowledgeObjectClosestPoint );
    brain.RegisterFunction( "_DEC_ConnaissanceObjet_EstEnnemi", &DEC_PopulationFunctions::IsEnemy );

    // Knowledge agents
    brain.RegisterFunction( "DEC_Connaissance_EnAgent", &DEC_KnowledgeAgentFunctions::GetAgent );
    brain.RegisterFunction< std::vector< unsigned int >, const DEC_Decision_ABC* >( "_DEC_Connaissances_PionsPrenantAPartie", &DEC_PopulationFunctions::GetPionsAttacking );
    brain.RegisterFunction< std::vector< unsigned int >, const DEC_Decision_ABC* >( "_DEC_Connaissances_PionsSecurisant", &DEC_PopulationFunctions::GetPionsSecuring );

    // Orders
    brain.RegisterFunction( "_DEC_AssignMissionCrowdParameter", &MIL_MissionParameterFactory::SetCrowdKnowledgeParameter );
    brain.RegisterFunction<boost::shared_ptr<MIL_MissionParameter_ABC>, DEC_Decision_ABC*, int>( "_DEC_AssignMissionCrowdListParameter", &MIL_MissionParameterFactory::CreatePopulationKnowledge );

    // Actions
    brain.RegisterFunction( "_DEC__StartDeplacement", &DEC_ActionFunctions::StartAction< PHY_Population_ActionMove, MT_Vector2D* > );
    brain.RegisterFunction( "_DEC_StartDeplacementItineraire", &DEC_ActionFunctions::StartAction< PHY_Population_ActionMoveAlong, std::vector< boost::shared_ptr< MT_Vector2D > > > );
    brain.RegisterFunction( "_DEC__StartTirSurPions", &DEC_ActionFunctions::StartAction< PHY_Population_ActionFireOnPions, float > );
    brain.RegisterFunction( "_DEC__StartTirSurPion", &DEC_ActionFunctions::StartAction< PHY_Population_ActionFireOnPion, float, unsigned int > );
    brain.RegisterFunction( "_DEC_StartTirSurPion", &DEC_ActionFunctions::StartAction< PHY_Population_ActionFireOnPion, float, DEC_Decision_ABC* > );
    brain.RegisterFunction( "_DEC_DetruireBlocUrbain",  &DEC_ActionFunctions::StartAction< PHY_Population_ActionUrbanDestruction, MIL_UrbanObject_ABC* > );
    brain.RegisterFunction( "_DEC_StartAgresserFoule", &DEC_ActionFunctions::StartAction< PHY_Population_ActionBattle > );
}

// -----------------------------------------------------------------------------
// Name: RegisterAgentKnowledgeFunctions
// Created: LDC 2011-05-05
// -----------------------------------------------------------------------------
void RegisterAgentKnowledgeFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EtatOps", &DEC_KnowledgeAgentFunctions::GetOperationalState );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstDetruitTactique", &DEC_KnowledgeAgentFunctions::GetMajorOperationalState );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_Position", &DEC_KnowledgeAgentFunctions::GetPositionPtr );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstEnVol", &DEC_KnowledgeAgentFunctions::IsFlying );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_Altitude", &DEC_KnowledgeAgentFunctions::DEC_ConnaissanceAgent_Altitude );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_NatureAtlas", &DEC_KnowledgeAgentFunctions::GetNatureAtlas );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_NiveauPerceptionMax", &DEC_KnowledgeAgentFunctions::GetMaxPerceptionLevelForKnowledgeGroup );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstValide", &DEC_KnowledgeAgentFunctions::IsKnowledgeValid );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstEnMouvement", &DEC_KnowledgeAgentFunctions::IsMoving );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_SEstRendu", &DEC_KnowledgeAgentFunctions::IsSurrendered );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstPrisonnier", &DEC_KnowledgeAgentFunctions::IsPrisoner );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstRenduAMonCamp", &DEC_KnowledgeAgentFunctions::IsSurrenderToMyArmy );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstRefugie", &DEC_KnowledgeAgentFunctions::IsRefugee );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstMort", &DEC_KnowledgeAgentFunctions::IsDead );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_Verrouiller", &DEC_KnowledgeAgentFunctions::Lock );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_Deverrouiller", &DEC_KnowledgeAgentFunctions::Unlock );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_TuerOfficiers", &DEC_KnowledgeAgentFunctions::KillOfficers );
    brain.RegisterFunction( "DEC_Connaissances_UnitesPrenantAPartieSurAmi" , &DEC_KnowledgeFunctions::GetAgentsAttackingAlly );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_GetMilPionType", &DEC_KnowledgeAgentFunctions::GetMilPionType );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_DangerositeSurPion", &DEC_KnowledgeAgentFunctions::GetDangerosityOnPion );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_DangerositeSurConnaissance", &DEC_KnowledgeAgentFunctions::GetDangerosityOnKnowledge );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EnAgent", &DEC_KnowledgeAgentFunctions::GetAgent );
    brain.RegisterFunction( "DEC_Connaissances_PartageConnaissancesAvecConnaissanceAgent", &DEC_KnowledgeAgentFunctions::ShareKnowledgesWith );
    brain.RegisterFunction( "DEC_ObjectKnowledge_GetObjectsInZone", &DEC_KnowledgeFunctions::GetObjectsWithCapacityInZone );
    brain.RegisterFunction( "DEC_ObjectKnowledge_IsPositionInside", &DEC_KnowledgeFunctions::IsPositionInsideObjectOfType );
    brain.RegisterFunction( "DEC_GetModulationVitesseMax", &DEC_MiscFunctions::GetMaxSpeedModificator );
    brain.RegisterMethod( "DEC_AgentKnowledge_Relevance", &DEC_Knowledge_Agent::GetRelevance );
    
    // Returns agent knownledge known by callerId and identified by knowledgeId,
    // throws an exception if the knowledge cannot be resolved.
    brain.RegisterFunction( "DEC_GetAgentKnowledge", &DEC_KnowledgeFunctions::GetAgentKnowledge );

    brain.RegisterFunction( "DEC_Agent_ForcerSilenceRadio", &DEC_KnowledgeAgentFunctions::ForceRadioSilence );
    brain.RegisterFunction( "DEC_AgentKnowledge_DisableCrowdEffect", &DEC_KnowledgeAgentFunctions::DisableCrowdEffect );
    brain.RegisterFunction( "DEC_ChangerSuperieurLogistiqueConnaissance", &DEC_KnowledgeAgentFunctions::SwitchAutomateLogistic );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstDansFoule", &DEC_KnowledgeAgentFunctions::IsInCrowd );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstDefinitivementIllumine", &DEC_KnowledgeAgentFunctions::IsDefinitivelyIlluminated );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstIllumine", &DEC_KnowledgeAgentFunctions::IsIlluminated );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstTerroriste", &DEC_KnowledgeAgentFunctions::IsTerrorist );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_Neutraliser", &DEC_KnowledgeAgentFunctions::NeutralizeAgent );
    brain.RegisterFunction( "DEC_Connaissance_GetRawMission", &DEC_KnowledgeAgentFunctions::GetMission );
    brain.RegisterFunction( "DEC_GetAgentIdFromKnowledge", &DEC_KnowledgeAgentFunctions::GetAgentIdFromKnowledge );
    brain.RegisterFunction( "DEC_ObtenirRenseignementCritiqueSurPion", &DEC_KnowledgeAgentFunctions::GetCriticalIntelligence );

    brain.RegisterFunction( "_DEC_KnowledgeAgent_IsPerceptionLevelMax", &DEC_KnowledgeAgentFunctions::IsPerceptionLevelMax );
}

void RegisterPopulationKnowledgeFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "_DEC_KnowledgePopulation_Domination",  &DEC_KnowledgePopulationFunctions::GetDominationState );
    brain.RegisterFunction( "_DEC_ConnaissancePopulation_EstDansZone", &DEC_KnowledgePopulationFunctions::IsInZone );
    brain.RegisterFunction( "_DEC_KnowledgePopulation_GetBarycenter", &DEC_KnowledgePopulationFunctions::GetBarycenter );
    brain.RegisterFunction( "_DEC_Crowd_GetHumansFromAllTypes", &DEC_KnowledgePopulationFunctions::GetHumansFromAllTypes );   
}

void RegisterKnowledgeFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "_DEC_Connaissances_PartageConnaissancesAvec", &DEC_KnowledgeFunctions::ShareKnowledgesWith );
    brain.RegisterFunction( "_DEC_Connaissances_PartageConnaissancesDansZoneAvec", &DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith );
    brain.RegisterFunction( "_DEC_Knowledges_ObjectsInCircle", &DEC_KnowledgeFunctions::GetObjectsInCircle );
    brain.RegisterFunction( "_DEC_Connaissances_ObjetsDansFuseau", &DEC_KnowledgeFunctions::GetObjectsInFuseau );
    brain.RegisterFunction( "_DEC_Connaissances_UnitesEnnemiesVivantesPercuesParPion", &DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion );
    brain.RegisterFunction( "_DEC_Connaissances_Populations", &DEC_KnowledgeFunctions::GetPopulations );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterRepresentationFunctions
// Created: LDC 2013-06-14
// -----------------------------------------------------------------------------
void RegisterRepresentationFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_AddEnemyRepresentation", &DEC_MiscFunctions::AddEnemyRepresentation );
    brain.RegisterFunction( "DEC_RemoveEnemyRepresentation", &DEC_MiscFunctions::RemoveEnemyRepresentation );
    brain.RegisterFunction( "DEC_GetEnemyRepresentations", &DEC_MiscFunctions::GetEnemyRepresentation );

    brain.RegisterFunction( "_DEC_GetOrdersCategory", &DEC_MiscFunctions::GetOrdersCategory );
    brain.RegisterFunction( "_DEC_GetPointsCategory", &DEC_MiscFunctions::GetPointsCategory );
    brain.RegisterFunction( "_DEC_RemoveFromOrdersCategory", &DEC_MiscFunctions::RemoveFromOrdersCategory );
    brain.RegisterFunction( "_DEC_DeleteRepresentation", &DEC_MiscFunctions::DeleteOrderRepresentation );
    brain.RegisterFunction( "_DEC_RemoveFromPointsCategory", &DEC_MiscFunctions::RemoveFromPointsCategory );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterActionFunctions
// Created: LDC 2013-06-14
// -----------------------------------------------------------------------------
void RegisterActionFunctions( sword::Brain& brain )
{
    brain.RegisterFunction< unsigned int, const DEC_Decision_ABC*, unsigned int >( "_DEC__StopAction", &DEC_ActionFunctions::StopAction );
    brain.RegisterFunction( "_DEC_PauseAction", &DEC_ActionFunctions::SuspendAction );
    brain.RegisterFunction( "_DEC_ReprendAction", &DEC_ActionFunctions::ResumeAction );
    brain.RegisterFunction( "_DEC_EtatAction", &DEC_ActionFunctions::GetActionState );

    brain.RegisterFunction( "DEC_Connaissance_PeutTransporterPion", &DEC_ActionFunctions::CanTransportKnowledge );
    brain.RegisterFunction( "DEC_Agent_PeutTransporterFoule", &DEC_ActionFunctions::CanTransportCrowd );
    brain.RegisterFunction( "DEC_Agent_GetCapacityToTransportCrowd", &DEC_ActionFunctions::GetCapacityToTransportCrowd );
    brain.RegisterFunction( "DEC_Agent_TransporteFoule", &DEC_ActionFunctions::IsTransportingCrowd );
    brain.RegisterFunction( "DEC_Connaissance_Transporter", &DEC_ActionFunctions::Knowledge_Load );
    brain.RegisterFunction( "DEC_TransportConnaissance_AjouterPion", &DEC_ActionFunctions::TransportKnowledge_AddPion );
    brain.RegisterFunction( "DEC_Agent_OrienterEtEmbarquer" , &DEC_ActionFunctions::LoadAgentInCamp );
    brain.RegisterFunction( "DEC_Agent_RefugieEstEmbarque", &DEC_ActionFunctions::IsAgentLoaded );
    brain.RegisterFunction( "DEC_Agent_DebarquerRefugiesDansCamp", &DEC_ActionFunctions::UnLoadAgentInCamp );
    brain.RegisterFunction( "DEC_Agent_EstRendu", &DEC_ActionFunctions::IsSurrendered );
    brain.RegisterFunction( "DEC_Agent_EstRefugie", &DEC_ActionFunctions::IsRefugee );
    brain.RegisterFunction( "DEC_Prisonniers_EstDebarqueDansCamp", &DEC_ActionFunctions::Prisoners_IsUnloadedInCamp );
    brain.RegisterFunction( "DEC_Refugies_EstDebarqueDansCamp", &DEC_ActionFunctions::Refugees_IsUnloadedInCamp );
    brain.RegisterFunction( "DEC_Connaissance_TransportNombreAllerRetour", &DEC_ActionFunctions::GetNumberOfRoundTripToTransportKnowledge );

    brain.RegisterFunction( "_DEC_StartDeplacement", &DEC_ActionFunctions::StartAction< PHY_ActionMove, boost::shared_ptr< TER_Path_ABC >, bool > );
    brain.RegisterFunction( "_DEC_StartTirDirect", &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePion, boost::shared_ptr< DEC_Knowledge_Agent >, double, int, int > );
    brain.RegisterFunction( "_DEC_StartTirDirectDebarques", &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePionUsingOnlyLoadable, boost::shared_ptr< DEC_Knowledge_Agent >, double, int > );
    brain.RegisterFunction( "_DEC_StartTirDirectTransporteurs", &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePionUsingOnlyCarrier, boost::shared_ptr< DEC_Knowledge_Agent >, double, int > );
    brain.RegisterFunction( "_DEC__StartTirDirectSurComposantesMajeures", &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePionOnMajorComposantes, int, boost::shared_ptr< DEC_Knowledge_Agent >, double, int > );
    brain.RegisterFunction( "_DEC__StartTirSurPopulation", &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePopulation, unsigned int, const std::string& > );
    brain.RegisterFunction( "_DEC_StartTirIndirectSurPosition",
        std::function< unsigned int ( DEC_Decision_ABC*, const PHY_DotationCategory*, float, MT_Vector2D* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionIndirectFire_Position, const PHY_DotationCategory*, float, MT_Vector2D*, DEC_Decision_ABC* >, _1, _2, _3, _4, nullptr ) ) );
    brain.RegisterFunction( "_DEC_StartTirIndirectSurPositionAvecDemandeur", &DEC_ActionFunctions::StartAction< PHY_ActionIndirectFire_Position, const PHY_DotationCategory*, float, MT_Vector2D*, DEC_Decision_ABC* > );
    brain.RegisterFunction( "_DEC_StartTirIndirectSurConnaissance", &DEC_ActionFunctions::StartAction< PHY_ActionIndirectFire_Knowledge, const PHY_DotationCategory*, float, unsigned int > );
    brain.RegisterFunction( "_DEC_StartTirIndirectSurConnaissancePtr", &DEC_ActionFunctions::StartAction< PHY_ActionIndirectFire_Knowledge, const PHY_DotationCategory*, float, boost::shared_ptr< DEC_Knowledge_Agent > > );
    brain.RegisterFunction( "_DEC_StartCreateObject", &DEC_ActionFunctions::StartAction< PHY_ActionConstructObject, boost::shared_ptr< DEC_Gen_Object >, bool > );
    brain.RegisterFunction( "_DEC_StartPrepareObject", &DEC_ActionFunctions::StartAction< PHY_ActionPrepareObject, boost::shared_ptr< DEC_Gen_Object > > );
    brain.RegisterFunction( "_DEC_StartDevaloriserObjet", &DEC_ActionFunctions::StartAction< PHY_ActionDemineObject, boost::shared_ptr< DEC_Knowledge_Object > > );
    brain.RegisterFunction( "_DEC_StartDetruireObjet", &DEC_ActionFunctions::StartAction< PHY_ActionDestroyObject, boost::shared_ptr< DEC_Knowledge_Object > > );
    brain.RegisterFunction( "_DEC_StartValoriserObjet", &DEC_ActionFunctions::StartAction< PHY_ActionMineObject, boost::shared_ptr< DEC_Knowledge_Object > > );
    brain.RegisterFunction( "_DEC_StartCreerContournementObjet", &DEC_ActionFunctions::StartAction< PHY_ActionBypassObject, boost::shared_ptr< DEC_Knowledge_Object > > );
    brain.RegisterFunction( "_DEC__StartOccuperObjet", &DEC_ActionFunctions::StartAction< PHY_ActionOccupyObject, boost::shared_ptr< DEC_Knowledge_Object > > );
    brain.RegisterFunction( "_DEC_StartReprendreTravauxObjet", &DEC_ActionFunctions::StartAction< PHY_ActionResumeWorkObject, boost::shared_ptr< DEC_Knowledge_Object>, bool > );
    brain.RegisterFunction( "_DEC_ReparerBlocUrbain", &DEC_ActionFunctions::StartAction< PHY_ActionResumeWorkUrbanBlock, MIL_UrbanObject_ABC* > );
    brain.RegisterFunction( "_DEC_DeteriorateUrbanBlock", &DEC_ActionFunctions::StartAction< PHY_ActionDeteriorateUrbanBlock, MIL_UrbanObject_ABC*, double > );
    brain.RegisterFunction( "_DEC__StartAnimerObjet", &DEC_ActionFunctions::StartAction< PHY_ActionAnimateObject, boost::shared_ptr< DEC_Knowledge_Object > > );
    brain.RegisterFunction( "_DEC_StartDistributionObjet", &DEC_ActionFunctions::StartAction< PHY_ActionDistributeObject, boost::shared_ptr< DEC_Knowledge_Object >, double > );
    brain.RegisterFunction( "_DEC_StartSupplyObject", &DEC_ActionFunctions::StartAction< PHY_ActionSupplyObject, boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >&, double > );
    brain.RegisterFunction( "_DEC_StartExtractFromStockObject", &DEC_ActionFunctions::StartAction< PHY_ActionExtractFromStockObject, boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >&, double > );
    brain.RegisterFunction( "_DEC__StartControlerZone", &DEC_ActionFunctions::StartAction< PHY_ActionControlZone, const TER_Localisation*, double, bool > );
    brain.RegisterFunction( "_DEC_StartEmbarquement", &DEC_ActionFunctions::StartAction< transport::PHY_ActionLoad > );
    brain.RegisterFunction( "_DEC_StartDebarquement", &DEC_ActionFunctions::StartAction< transport::PHY_ActionUnload > );
    brain.RegisterFunction( "_DEC_StartIlluminer", &DEC_ActionFunctions::StartAction< PHY_ActionIllumination, boost::shared_ptr< DEC_Knowledge_Agent >, DEC_Decision_ABC* > );
    brain.RegisterFunction( "_DEC_StartExtinguishObject", &DEC_ActionFunctions::StartAction< PHY_ActionExtinguishObject, boost::shared_ptr< DEC_Knowledge_Object > > );
    brain.RegisterFunction( "_DEC_StartInterrogateCrowd", &DEC_ActionFunctions::StartAction< PHY_ActionInterrogate, int > );
    brain.RegisterFunction( "_DEC_StartInterrogateUnit", &DEC_ActionFunctions::StartAction< PHY_ActionInterrogate, boost::shared_ptr< DEC_Knowledge_Agent > > );
    brain.RegisterFunction( "_DEC_StartTraverserReseauSouterrain", &DEC_ActionFunctions::StartAction< PHY_ActionMoveUnderground, boost::shared_ptr< DEC_Knowledge_Object > > );

    // Logistique
    brain.RegisterFunction( "_DEC_StartPreterVSRAM", &DEC_ActionFunctions::StartAction< PHY_ActionLendCollectionComposantes, DEC_Decision_ABC*, DEC_Decision_ABC*, unsigned int > );
    brain.RegisterFunction( "_DEC_StartPreterRemorqueurs", &DEC_ActionFunctions::StartAction< PHY_ActionLendHaulerComposantes, DEC_Decision_ABC*, DEC_Decision_ABC*, unsigned int > );
    brain.RegisterFunction( "_DEC_StartPreterComposantes", &DEC_ActionFunctions::StartAction< PHY_ActionLendSpecificComposantes, DEC_Decision_ABC*, DEC_Decision_ABC*, PHY_ComposanteTypePion*, unsigned int > );
    brain.RegisterFunction( "_DEC_StartConsumingResources",
        std::function< unsigned int( DEC_Decision_ABC*, unsigned int, double, double ) >(
            [&]( DEC_Decision_ABC* agent, unsigned int category, double value, double duration ) {
                return DEC_ActionFunctions::StartAction< PHY_ActionConsumeResources >( agent, category, value, duration, MIL_Time_ABC::GetTime().GetTickDuration() );
    }));

    // Transport / Heliportage
    brain.RegisterFunction( "_DEC_Start_TransportEmbarquer", &DEC_ActionFunctions::StartAction< PHY_ActionTransportLoad > );
    brain.RegisterFunction( "_DEC_Start_TransportDebarquer", &DEC_ActionFunctions::StartAction< PHY_ActionTransportUnload, MT_Vector2D* > );
    brain.RegisterFunction( "_DEC_StartEmbarquerFouleDUneConcentration", &DEC_ActionFunctions::StartAction< crowdtransport::PHY_ActionLoadCrowd, int, unsigned int > );
    brain.RegisterFunction( "_DEC_StartDebarquerFouleSurPosition", &DEC_ActionFunctions::StartAction< crowdtransport::PHY_ActionUnloadCrowd, int, MT_Vector2D* > );

    // Emergency functions
    brain.RegisterFunction( "_DEC_Start_EmergencyInfluence", &DEC_ActionFunctions::StartAction< PHY_ActionInfluence, const std::string&, double > );
    brain.RegisterFunction( "_DEC_Start_EmergencyTriggerActivity", &DEC_ActionFunctions::StartAction< PHY_ActionTriggerActivity, const std::string&, double > );
    brain.RegisterFunction( "_DEC_Start_EmergencyInfluenceInArea", &DEC_ActionFunctions::StartAction< PHY_ActionInfluenceInArea, const std::string&, double, const TER_Localisation* > );
    brain.RegisterFunction( "_DEC_Start_EmergencyTriggerActivityInArea", &DEC_ActionFunctions::StartAction< PHY_ActionTriggerActivityInArea, const std::string&, double, const TER_Localisation* > );
    brain.RegisterFunction( "_DEC_Start_EmergencyUnloadActivity", &DEC_ActionFunctions::StartAction< PHY_ActionUnloadActivity, const std::string&, double > );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterGeometryFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterGeometryFunctions( sword::Brain& brain)
{
    brain.RegisterFunction( "DEC_Automate_Position", &DEC_AutomateFunctions::GetBarycenter );
    brain.RegisterFunction( "DEC_Agent_PositionPtr", &DEC_AgentFunctions::GetAgentPositionPtr );
    brain.RegisterFunction( "DEC_Agent_Height", &DEC_AgentFunctions::GetHeight );
    brain.RegisterFunction( "DEC_Geometrie_DecouperListePoints", &DEC_GeometryFunctions::SplitListPoints );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionCouverture", &DEC_GeometryFunctions::ComputeCoverPosition );
    brain.RegisterFunction( "DEC_Geometrie_CalculerBarycentreAgents", &DEC_GeometryFunctions::ComputeAgentsBarycenter );
    brain.RegisterFunction( "DEC_Geometrie_CalculerBarycentreListePoints", &DEC_GeometryFunctions::ComputeListPointsBarycenter );
    brain.RegisterFunction( "DEC_Geometrie_PositionTranslate", &DEC_GeometryFunctions::TranslatePosition );
    brain.RegisterFunction( "DEC_Geometrie_PositionTranslateDir", &DEC_GeometryFunctions::TranslatePositionInDirection );
    brain.RegisterFunction( "DEC_Geometrie_PositionTranslateVector", &DEC_GeometryFunctions::TranslatePositionInVector );
    brain.RegisterFunction( "DEC_Geometrie_PositionsEgales", &DEC_GeometryFunctions::ComparePositions );
    brain.RegisterFunction( "DEC_Geometrie_Distance", &DEC_GeometryFunctions::Distance );
    brain.RegisterFunction( "DEC_Geometrie_Distance3D", &DEC_GeometryFunctions::Distance3D );
    brain.RegisterFunction( "DEC_Geometrie_ConvertirPointEnLocalisation", &DEC_GeometryFunctions::ConvertPointToLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_EstPointDansLocalisation", &DEC_GeometryFunctions::IsPointInsideLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_CreerLocalisation", &DEC_GeometryFunctions::CreateLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_CreerListePoints", &DEC_GeometryFunctions::CreateListPoint );
    brain.RegisterFunction( "DEC_Geometrie_CreerPoint", &DEC_GeometryFunctions::CreatePoint );
    brain.RegisterFunction( "DEC_Geometrie_CreerPointLatLong", &DEC_GeometryFunctions::CreatePointFromLatLong );
    brain.RegisterFunction( "DEC_Geometrie_CreerPointXY", &DEC_GeometryFunctions::CreatePointFromXY );
    brain.RegisterFunction( "DEC_Geometrie_CopiePoint", &DEC_GeometryFunctions::CopyPoint );
    brain.RegisterFunction( "DEC_Geometrie_CreerDirection", &DEC_GeometryFunctions::CreateDirection );
    brain.RegisterFunction( "DEC_Geometrie_CreerVecteur", &DEC_GeometryFunctions::CreateVector );
    brain.RegisterFunction( "DEC_Geometrie_CreerDirectionPerpendiculaire", &DEC_GeometryFunctions::CreateOrthoDirection );
    brain.RegisterFunction( "DEC_Geometrie_InverseDirection", &DEC_GeometryFunctions::ReverseDirection );
    brain.RegisterFunction( "DEC_Geometrie_CopieEtInverseDirection", &DEC_GeometryFunctions::CopyAndReverseDirection );
    brain.RegisterFunction( "DEC_Geometrie_CopieEtRotateDirection", &DEC_GeometryFunctions::CopyAndRotateDirection );
    brain.RegisterFunction( "DEC_Geometrie_CalculerDistanceLigneMoyenne", &DEC_GeometryFunctions::ComputeDistanceFromMiddleLine );
    brain.RegisterFunction( "DEC_Geometrie_CalculerBarycentreLocalisation", &DEC_GeometryFunctions::ComputeLocalisationBarycenter );
    brain.RegisterFunction( "DEC_Geometrie_ListePointsLocalisation", &DEC_GeometryFunctions::ListLocalisationPoints );
    brain.RegisterFunction( "DEC_Geometrie_OffsetItinerary", &DEC_GeometryFunctions::ListLocalisationPoints );
    brain.RegisterFunction( "DEC_Geometrie_DirectionMoyenne", &DEC_GeometryFunctions::ComputeMeanDirection );
    brain.RegisterFunction( "DEC_Geometrie_PositionAleatoireDansFuseauSurCercle", &DEC_GeometryFunctions::ComputeRandomPointOnCircleInFuseau );
    brain.RegisterFunction( "DEC_Geometrie_PositionAleatoireDansFuseauDansCercle", &DEC_GeometryFunctions::ComputeRandomPointInCircleInFuseau );
    brain.RegisterFunction( "DEC_Geometrie_PositionAleatoireSurCercle", &DEC_GeometryFunctions::ComputeRandomPointOnCircle );
    brain.RegisterFunction( "DEC_Geometrie_PositionAleatoireDansCercle", &DEC_GeometryFunctions::ComputeRandomPointInCircle );
    brain.RegisterFunction( "DEC_Geometrie_PositionAleatoireDansZone", &DEC_GeometryFunctions::ComputeRandomPointInZone );
    brain.RegisterFunction( "DEC_Geometrie_CreerLocalisationCercle", &DEC_GeometryFunctions::CreateCircleLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_CreerLocalisationLigne", &DEC_GeometryFunctions::CreateLineLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_CreerLocalisationPolyligne", &DEC_GeometryFunctions::CreatePolylineLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_CreerLocalisationPolygone", &DEC_GeometryFunctions::CreatePolygonLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_AgrandirLocalisation", &DEC_GeometryFunctions::CreateScaledLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_PionEstCoordonne", &DEC_GeometryFunctions::ListUncoordinatedPawns );
    brain.RegisterFunction( "DEC_Geometrie_PourcentageTerrainCompartimente", &DEC_GeometryFunctions::ComputeClosedTerrainRatioInFuseau );
    brain.RegisterFunction( "DEC_Geometrie_PourcentageTerrainOuvert", &DEC_GeometryFunctions::ComputeOpenTerrainRatioInFuseau );
    brain.RegisterFunction( "DEC_Geometrie_TrierZonesSelonOuvertureTerrain", &DEC_GeometryFunctions::SortZonesAccordingToTerrainOpening );
    brain.RegisterFunction( "DEC_Geometrie_PourcentageZoneTerrainCompartimente", &DEC_GeometryFunctions::ComputeClosedTerrainRatioInZone );
    brain.RegisterFunction( "DEC_Geometrie_PourcentageZoneTerrainOuvert", &DEC_GeometryFunctions::ComputeOpenTerrainRatioInZone );
    brain.RegisterFunction( "DEC_Geometrie_TrierFuseauxSelonOuvertureTerrain", &DEC_GeometryFunctions::SortFuseauxAccordingToTerrainOpening );
    brain.RegisterFunction( "DEC_Geometrie_ConvertirFuseauEnLocalisation", &DEC_GeometryFunctions::ConvertFuseauToLocalisation );
    brain.RegisterFunction( "DEC_Geometrie_ProchainObjectifDansFuseau", &DEC_GeometryFunctions::GetNextObjectiveInFuseau );
    brain.RegisterFunction( "DEC_Geometrie_DistanceBetweenPoints", &DEC_GeometryFunctions::ComputeDistance );
    brain.RegisterFunction( "DEC_Geometrie_AreaSize", &DEC_GeometryFunctions::ComputeAreaSize );
    brain.RegisterFunction( "DEC_Geometrie_IntersectionSize", &DEC_GeometryFunctions::ComputeIntersectionArea );
    brain.RegisterFunction( "DEC_Geometrie_AreaDiameter", &DEC_GeometryFunctions::ComputeAreaDiameter );
    brain.RegisterFunction( "DEC_Geometrie_DecouperLocalisation", std::function< std::vector< boost::shared_ptr< MT_Vector2D > >( const TER_Localisation*, unsigned int ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisation, _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_ClipperLocalisation", &DEC_GeometryFunctions::ClipLocalisation );
    brain.RegisterFunction( "DEC_IsPointInCity", &DEC_GeometryFunctions::IsPointInCity );
    brain.RegisterFunction( "DEC_IsPointInObject", &DEC_GeometryFunctions::IsPointInObject );
    brain.RegisterFunction( "DEC_Geometrie_ComputeNearestBorder", &DEC_GeometryFunctions::ComputeNearestBorder );
    brain.RegisterFunction( "DEC_Geometrie_CalculerTrafficablePointPourPoint",
        std::function< std::vector< boost::shared_ptr< MT_Vector2D > >( const MT_Vector2D& ) >( boost::bind( &DEC_GeometryFunctions::ComputeTrafficableLocalisation, _1 ) ) );
    brain.RegisterFunction( "DEC_IsPointInUrbanBlockTrafficableForPlatoon",
        std::function< bool( DEC_Decision_ABC*, MT_Vector2D& ) >( boost::bind( &DEC_GeometryFunctions::IsPointInUrbanBlockTrafficableForPlatoon, _1, _2 ) ) );
    brain.RegisterFunction( "DEC_IsPointInDestroyedUrbanBlock", &DEC_GeometryFunctions::IsPointInDestroyedUrbanBlock );
    brain.RegisterFunction( "DEC_Geometrie_PositionsParRapportALocalisation",
        std::function< std::vector< boost::shared_ptr< MT_Vector2D > >( const std::vector< DEC_Decision_ABC* >&, TER_Localisation*, MT_Vector2D*, double ) >( boost::bind( &DEC_GeometryFunctions ::ComputeLocalisationPointsForPionsInFuseau, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointArriveePourFuseau",
        std::function< boost::shared_ptr< MT_Vector2D >( MIL_Fuseau& ) >( boost::bind( &DEC_GeometryFunctions::ComputeDestPointForFuseau, _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerLocalisationsBU", &DEC_GeometryFunctions::ComputeUrbanBlockLocalisations );
    brain.RegisterFunction( "DEC_Geometrie_EstPointDansFuseau_AvecParamFuseau", &DEC_GeometryFunctions::IsPointInFuseau_ParamFuseau );
    brain.RegisterFunction( "DEC_Geometrie_LocalisationsEgales", &DEC_GeometryFunctions::CompareLocalisations );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointSurFuseau", &DEC_GeometryFunctions::GetPointAlongFuseau );
    brain.RegisterFunction( "DEC_Geometrie_StartCalculLignesAvantEtArrierePourPion", &DEC_GeometryFunctions::StartComputingFrontAndBackLinesForPlatoon );
    brain.RegisterMethod( "DEC_Geometrie_X", &MT_Vector2D::GetX );
    brain.RegisterMethod( "DEC_Geometrie_Y", &MT_Vector2D::GetY );
    brain.RegisterFunction( "DEC_Geometrie_CalculerDistanceLigneAvant", &DEC_GeometryFunctions::ComputeDistanceFromFrontLine );
    brain.RegisterFunction( "DEC_Geometrie_StopCalculLignesAvantEtArriere", &DEC_GeometryFunctions::StopComputingFrontAndBackLines );
    brain.RegisterFunction( "DEC_Geometrie_PositionAdvanceAlongDangerDirection", &DEC_GeometryFunctions::ComputePositionAdvanceAlongDangerDirection );
    brain.RegisterFunction( "DEC_Geometrie_ConvexHull", &DEC_GeometryFunctions::ComputeConvexHull );

    brain.RegisterFunction( "DEC_Geometrie_FindRoadIntersectionWithZone", &DEC_TerrainFunctions::GetRoadIntersectionsWithZone );
    brain.RegisterFunction( "DEC_Geometrie_IsLinearRiverInBetween", &DEC_TerrainFunctions::IsLinearRiverInBetween );
    brain.RegisterFunction( "DEC_Geometrie_IsWaterInBetween", &DEC_TerrainFunctions::IsWaterInBetween );
    brain.RegisterFunction( "DEC_Geometrie_GetTerrainData", &DEC_TerrainFunctions::GetTerrainData );
    brain.RegisterFunction( "DEC_Geometrie_PositionAdvanceAlongAOR", &DEC_GeometryFunctions::ComputePositionAdvanceAlongFuseau );
    brain.RegisterFunction( "DEC_Geometrie_CalculerEntreeFuseau", &DEC_GeometryFunctions::ComputeEntryPointForFuseau );

    brain.RegisterFunction( "DEC_GetOpenSearchSpeed", &DEC_GeometryFunctions::GetOpenSearchSpeed );
    brain.RegisterFunction( "DEC_GetOpenRecoSpeed", &DEC_GeometryFunctions::GetOpenRecoSpeed );
    brain.RegisterFunction( "DEC_GetUrbanSearchSpeed", &DEC_GeometryFunctions::GetUrbanSearchSpeed );
    brain.RegisterFunction( "DEC_GetUrbanRecoSpeed", &DEC_GeometryFunctions::GetUrbanRecoSpeed );
    brain.RegisterFunction( "DEC_Geometrie_UrbanRatio", &DEC_GeometryFunctions::GetUrbanRatio );
    brain.RegisterFunction( "DEC_Geometrie_SurfaceParametersUrbanRatio", &DEC_GeometryFunctions::GetSurfaceParametersUrbanRatio );
    brain.RegisterFunction( "DEC_Geometrie_PointParametersUrbanRatio", &DEC_GeometryFunctions::GetPointParametersUrbanRatio );
    brain.RegisterFunction( "DEC_Geometrie_UrbanParametersNumber", &DEC_GeometryFunctions::GetUrbanParametersNumber );
    
    brain.RegisterFunction( "DEC_Copie_DirectionDanger_Mission", &DEC_MiscFunctions::CopyDirectionDanger );
    brain.RegisterFunction( "DEC_DebugPointXY", &DEC_MiscFunctions::GetPointXY );
    brain.RegisterFunction( "DEC_Geometrie_CalculerAutomateDistanceLigneAvant", &DEC_GeometryFunctions::ComputeDistanceAutomatFromFrontLine );
    brain.RegisterFunction( "DEC_Geometrie_CalculerDistanceLigneArriere", &DEC_GeometryFunctions::ComputeDistanceFromBackLine );
    brain.RegisterFunction( "DEC_Geometrie_CalculerRetard", &DEC_GeometryFunctions::ComputeAutomatDelayFromSchedule );
    brain.RegisterFunction( "DEC_Geometrie_PionDerriere", &DEC_GeometryFunctions::ComputeBackestAgent );
    brain.RegisterFunction( "DEC_Geometrie_PionDevant", &DEC_GeometryFunctions::GetFrontestPion );
    
    brain.RegisterFunction( "DEC_Geometrie_PosDeploiementMistralNasse",
        std::function< std::vector< boost::shared_ptr< MT_Vector2D > >( int, const MT_Vector2D*, float, float, float, const MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::ComputePosDeploiementASANasse, _1, _2, _3, _4, _5, _6 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_PosDeploiementDoubleRideau",
        std::function< std::vector< boost::shared_ptr< MT_Vector2D > >( int, const MT_Vector2D*, float, float, float, const MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::ComputePosDeploiementASADoubleRideau, _1, _2, _3, _4, _5, _6 ) ) );

    brain.RegisterFunction( "_DEC_Geometrie_CalculerBarycentreLocalisationDansFuseau", &DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau );
    brain.RegisterFunction( "DEC_Geometry_Pion_SplitLocalisation", &DEC_GeometryFunctions::SplitLocalisationInParts );
    brain.RegisterFunction( "_DEC_Geometrie_DecoupeFuseauEnTroncons", &DEC_GeometryFunctions::SplitLocalisationInSections );
    brain.RegisterFunction( "_DEC_Geometrie_CalculerPositionObstacle", &DEC_GeometryFunctions::ComputeObstaclePosition );
    brain.RegisterFunction( "_DEC_Geometrie_CalculerPointArrivee", &DEC_GeometryFunctions::ComputeDestPoint );
    brain.RegisterFunction( "_DEC_Geometrie_CalculerPointDepart", &DEC_GeometryFunctions::ComputeStartPoint );
    brain.RegisterFunction( "_DEC_Geometrie_EstPointDansFuseau",  &DEC_GeometryFunctions::IsPointInFuseau );
    brain.RegisterFunction( "_DEC_Geometrie_CalculerPositionParRapportALima", &DEC_GeometryFunctions::ComputePointBeforeLima );
    brain.RegisterFunction( "_DEC_Geometrie_CalculerPositionParRapportALimaDansFuseau", &DEC_GeometryFunctions::ComputePointBeforeLimaInFuseau );
    brain.RegisterFunction( "_DEC_Geometrie_CalculerPositionsParRapportALima",  &DEC_GeometryFunctions::ComputePointsBeforeLima );
    brain.RegisterFunction( "_DEC_Geometrie_StartCalculLignesAvantEtArriere", &DEC_GeometryFunctions::StartComputingFrontAndBackLines );
    brain.RegisterFunction( "_DEC_Geometrie_StartCalculAutomateLignesAvantEtArriere", &DEC_GeometryFunctions::StartComputingAutomatFrontAndBackLines );
    brain.RegisterFunction( "DEC_Geometrie_CalculerAutomateDistanceLigneArriere", &DEC_GeometryFunctions::ComputeDistanceAutomatFromBackLine );
    brain.RegisterFunction( "_DEC_Geometrie_CalculerPointArriveePourPion", &DEC_GeometryFunctions::ComputeDestPointForPion );
    brain.RegisterFunction( "_DEC_Geometrie_CalculerPointProcheLocalisationDansFuseau", &DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau );
    brain.RegisterFunction( "_DEC_Geometrie_CalculerPointProcheLocalisationNonClippeeDansFuseau", &DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInFuseau );
    brain.RegisterFunction( "_DEC_Geometrie_CalculerRetardSurObjectifs", &DEC_GeometryFunctions::ComputeDelayFromScheduleAndObjectives );
    brain.RegisterFunction( "_DEC_Geometrie_AdvanceAlongFuseau", &DEC_GeometryFunctions::ComputeAdvanceAlongFuseau );
    brain.RegisterFunction( "_DEC_Geometrie_GetPointsLimas", &DEC_GeometryFunctions::GetPointsOnLimasInFuseau );

    // ASA
    brain.RegisterFunction( "_DEC_Geometrie_PosDeploiementASAOmni", &DEC_GeometryFunctions::ComputePosDeploiementASAOmni );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterUrbanBlockFunctions
// Created: LMT 2010-09-15
// -----------------------------------------------------------------------------
void RegisterUrbanBlockFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_ConnaissanceUrbanBlock_Barycentre", &DEC_UrbanObjectFunctions::GetCurrentBarycenter );
    brain.RegisterFunction( "DEC_ConnaissanceUrbanBlock_BarycentreDansBU", &DEC_UrbanObjectFunctions::GetBarycenter );
    brain.RegisterFunction( "DEC_ConnaissanceUrbanBlock_Lisiere", &DEC_UrbanObjectFunctions::GetBoundingBox );
    brain.RegisterFunction( "DEC_NiveauDeProtectionMaterielComposantes", &DEC_KnowledgeAgentFunctions::GetMaterialComposantesProtectionLevel );
    brain.RegisterFunction( "DEC_PolygoneBlocUrbain", &DEC_UrbanObjectFunctions::GetPolygonFromUrbanBlock );
    brain.RegisterFunction( "DEC_BlocUrbain_Type", &DEC_UrbanObjectFunctions::GetType );
    brain.RegisterFunction( "DEC_EtatBlocUrbain", &DEC_UrbanObjectFunctions::GetStateUrbanBlock );
    brain.RegisterFunction( "DEC_SetUrbanBlockState", &DEC_UrbanObjectFunctions::SetUrbanBlockState );
    brain.RegisterFunction( "DEC_IsPointInUrbanBlock", &DEC_GeometryFunctions::IsPointInUrbanBlock );
    brain.RegisterFunction( "DEC_UrbanBlocksInZone", &DEC_UrbanObjectFunctions::GetUrbanBlockInZone );
    brain.RegisterFunction( "DEC_Connaissances_BlocUrbainDansCercle", &DEC_KnowledgeFunctions::GetUrbanBlockInCircle );
    brain.RegisterFunction( "DEC_EtatBlocUrbain", 
        std::function< float( MIL_UrbanObject_ABC* )>( boost::bind( &DEC_UrbanObjectFunctions::GetStateUrbanBlock, _1 ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterResourceNetworkFunctions
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
void RegisterResourceNetworkFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_ReseauRessource_Position", &DEC_ResourceNetworkFunctions::GetResourceNetworkPosition );
    brain.RegisterFunction( "DEC_ReseauRessource_DesactiverElement", &DEC_ResourceNetworkFunctions::DeactivateResourceNetworkElement );
    brain.RegisterFunction( "DEC_ReseauRessource_ActiverElement", &DEC_ResourceNetworkFunctions::ActivateResourceNetworkElement );
    brain.RegisterFunction( "DEC_CreerLienObjetVersReseau", &DEC_ResourceNetworkFunctions::CreateResourceNetworkObjectAndLinkReturn ); // obsolete?
    brain.RegisterFunction( "DEC_DetruireObjetResourceSansDelais", &DEC_ResourceNetworkFunctions::DestroyResourceNetworkObjectLink ); // obsolete?
    brain.RegisterFunction( "DEC_ReseauRessourceAugmenteProduction", &DEC_ResourceNetworkFunctions::IncreaseResourceProduction );
    brain.RegisterFunction( "DEC_ReseauRessourceBaisseProduction", &DEC_ResourceNetworkFunctions::DecreaseResourceProduction );
    brain.RegisterFunction( "DEC_ResourceNetwork_NodesInZone", &DEC_ResourceNetworkFunctions::GetResourceNetworksInZone );
    brain.RegisterFunction( "DEC_ResourceNetwork_NodesInZoneByType", &DEC_ResourceNetworkFunctions::GetResourceNetworksInZoneByType );
    brain.RegisterFunction( "DEC_ResourceNetwork_CreateLinkFromObject", &DEC_ResourceNetworkFunctions::CreateResourceNetworkLinkFromObject );
    brain.RegisterFunction( "DEC_ResourceNetwork_CreateLinkFromUrbanBlock", &DEC_ResourceNetworkFunctions::CreateResourceNetworkLinkFromUrbanBlock );
    brain.RegisterFunction( "DEC_ResourceNetwork_CreateLinkFromNode", &DEC_ResourceNetworkFunctions::CreateResourceNetworkLinkFromResourceNetwork );
    brain.RegisterFunction( "DEC_ResourceNetwork_DestroyLink", &DEC_ResourceNetworkFunctions::DestroyResourceNetworkLink );
    brain.RegisterFunction( "DEC_ResourceNetwork_DestroyLinkWithObject", &DEC_ResourceNetworkFunctions::DestroyResourceNetworkLinkWithObject );
    brain.RegisterFunction( "DEC_ResourceNetwork_Create", &DEC_ResourceNetworkFunctions::CreateNode );
    brain.RegisterFunction( "DEC_ResourceNetwork_GetSupply", &DEC_ResourceNetworkFunctions::GetSupply );
    brain.RegisterFunction( "DEC_ResourceNetwork_Supply", &DEC_ResourceNetworkFunctions::Supply );
    brain.RegisterMethod( "GetTypeName", &DEC_ResourceNetwork::GetTypeName );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterAreaFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterAreaFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_BMArea_Barycenter", &DEC_GeometryFunctions::ComputeBarycenter );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterFuseauFunctions
// Created: DDA 2010-12-03
// -----------------------------------------------------------------------------
void RegisterFuseauFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_Fuseau_Width", &DEC_GeometryFunctions::GetWidth );
    brain.RegisterFunction( "DEC_Fuseau_IsNull", &DEC_GeometryFunctions::IsNull );
    brain.RegisterFunction( "DEC_Fuseau_ComputeMiddlePointsInAOR", &DEC_GeometryFunctions::ComputeMiddlePointsInAOR );
    brain.RegisterFunction( "DEC_GetFuseau", &DEC_OrdersFunctions::GetFuseau );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterTimeManagementFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterTimeManagementFunctions( sword::Brain& brain )
{
    // $$$$ AGE 2007-10-11: Un seul temps
    brain.RegisterFunction( "DEC_TempsSim", &DEC_DIAFunctions::GetSimTime );
    brain.RegisterFunction( "DEC_TempsReel", &DEC_DIAFunctions::GetRealTime );
    brain.RegisterFunction( "DEC_Nuit", &DEC_DIAFunctions::IsNight );
    brain.RegisterFunction( "DEC_GetTimeInSeconds", &DEC_MiscFunctions::GetTimeInSeconds );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterParametersCopyFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterParametersCopyFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_Copie_Point", &DEC_DIAFunctions::CopyPoint );
    brain.RegisterFunction( "DEC_Copie_Point_Mission", &DEC_DIAFunctions::CopyPointMission );
    brain.RegisterFunction( "DEC_Copie_ListePoints_Mission", &DEC_DIAFunctions::CopyPathMission );
    brain.RegisterFunction( "DEC_Copie_PointDansListePoints_Mission", &DEC_DIAFunctions::CopyPointToListPointMission );
    brain.RegisterFunction( "DEC_Copie_Localisation", &DEC_DIAFunctions::CopyLocalisation );
    brain.RegisterFunction( "DEC_Copie_Localisation_Mission", &DEC_DIAFunctions::CopyLocalisationMission );
    brain.RegisterFunction( "DEC_Copie_ListeLocalisations_Mission", &DEC_DIAFunctions::CopyLocalisationListMission );
    brain.RegisterFunction( "DEC_Copie_LocalisationDansListeLocalisations_Mission", &DEC_DIAFunctions::CopyLocalisationToLocationListMission );
    brain.RegisterFunction( "DEC_UserTypeList_PushBack_Mission", &DEC_DIAFunctions::CopyKnowledgeObjectToKnowledgeObjectListMission );
    brain.RegisterFunction( "DEC_GenObjectList_PushBack_Mission", &DEC_DIAFunctions::CopyGenObjectToGenObjectListMission );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterListsManipulationFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterListsManipulationFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_ListePoints_GetAt", &DEC_DIAFunctions::ListPoint_GetAt );
    brain.RegisterFunction( "DEC_ListePoints_Size", &DEC_DIAFunctions::ListPoint_Size );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterLogisticFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterLogisticFunctions( sword::Brain& brain )
{
    // Maintenance
    brain.RegisterFunction( "_DEC_Maintenance_ActiverChaine", &DEC_LogisticFunctions::MaintenanceEnableSystem );
    brain.RegisterFunction( "_DEC_Maintenance_DesactiverChaine", &DEC_LogisticFunctions::MaintenanceDisableSystem );
    brain.RegisterFunction( "_DEC_Maintenance_ChangerPriorites", &DEC_LogisticFunctions::MaintenanceChangePriorities );
    brain.RegisterFunction( "_DEC_Maintenance_Priorites", &DEC_LogisticFunctions::GetAutomateMaintenancePriorities );
    brain.RegisterFunction( "_DEC_Maintenance_ChangerPrioritesTactiques", &DEC_LogisticFunctions::MaintenanceChangeTacticalPriorities );
    brain.RegisterFunction( "_DEC_Maintenance_PrioritesTactiques", &DEC_LogisticFunctions::GetAutomateMaintenanceTacticalPriorities );
    brain.RegisterFunction( "_DEC_Maintenance_ChangerRegimeTravail", &DEC_LogisticFunctions::MaintenanceChangeWorkRate );
    brain.RegisterFunction( "_DEC_Maintenance_RegimeTravail", &DEC_LogisticFunctions::GetAutomateMaintenanceWorkRate );

    // Medical
    brain.RegisterFunction( "DEC_NecessiteEvacuationBlesses", &DEC_LogisticFunctions::HasWoundedHumansToEvacuate );
    brain.RegisterFunction( "DEC_EvacuerBlessesVersTC2", &DEC_LogisticFunctions::EvacuateWoundedHumansToTC2 );
    brain.RegisterFunction( "DEC_InterdireEvacuationAutomatiqueBlesses", &DEC_LogisticFunctions::ForbidWoundedHumansAutoEvacuation );
    brain.RegisterFunction( "DEC_AutoriserEvacuationAutomatiqueBlesses", &DEC_LogisticFunctions::AllowWoundedHumansAutoEvacuation );
    brain.RegisterFunction( "_DEC_Sante_ActiverChaine", &DEC_LogisticFunctions::MedicalEnableSystem );
    brain.RegisterFunction( "_DEC_Sante_DesactiverChaine", &DEC_LogisticFunctions::MedicalDisableSystem );
    brain.RegisterFunction( "_DEC_Sante_ActiverFonctionTri", &DEC_LogisticFunctions::MedicalEnableSortingFunction );
    brain.RegisterFunction( "_DEC_Sante_DesactiverFonctionTri", &DEC_LogisticFunctions::MedicalDisableSortingFunction );
    brain.RegisterFunction( "_DEC_Sante_ActiverFonctionSoin", &DEC_LogisticFunctions::MedicalEnableHealingFunction );
    brain.RegisterFunction( "_DEC_Sante_DesactiverFonctionSoin", &DEC_LogisticFunctions::MedicalDisableHealingFunction );
    brain.RegisterFunction( "_DEC_Sante_ChangerPriorites", &DEC_LogisticFunctions::MedicalChangePriorities );
    brain.RegisterFunction( "_DEC_Sante_Priorites", &DEC_LogisticFunctions::GetAutomateMedicalPriorities );
    brain.RegisterFunction( "_DEC_Sante_ChangerPrioritesTactiques", &DEC_LogisticFunctions::MedicalChangeTacticalPriorities );
    brain.RegisterFunction( "_DEC_Sante_PrioritesTactiques", &DEC_LogisticFunctions::GetAutomateMedicalTacticalPriorities );

    // Supply
    brain.RegisterFunction( "DEC_DemandeDeRavitaillement", &DEC_LogisticFunctions::PionRequestSupply );
    brain.RegisterFunction( "DEC_EnforceAviationResources", &DEC_LogisticFunctions::EnforceAviationResources );
    brain.RegisterFunction( "DEC_Stock_ConnectToResourceNode", &DEC_LogisticFunctions::ConnectToResourceNode );
    brain.RegisterFunction( "DEC_Stock_DisconnectFromResourceNode", &DEC_LogisticFunctions::DisconnectFromResourceNode );
    brain.RegisterFunction( "_DEC_Ravitaillement_ActiverChaine", &DEC_LogisticFunctions::SupplyEnableSystem );
    brain.RegisterFunction( "_DEC_Ravitaillement_DesactiverChaine", &DEC_LogisticFunctions::SupplyDisableSystem );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterEngineerObjectsFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterEngineerObjectsFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_GenObject_Type", &DEC_ObjectFunctions::GetGenObjectType );
    brain.RegisterFunction( "DEC_GenObject_ExternalIdentifier", &DEC_ObjectFunctions::GetGenObjectExternalIdentifier );
    brain.RegisterFunction( "DEC_GenObject_Localisation", &DEC_ObjectFunctions::GetGenObjectLocalisation );
    brain.RegisterFunction( "DEC_GenObjectKnowledge_Localisation", &DEC_ObjectFunctions::GetObjectKnowledgeLocalisation );
    brain.RegisterMethod( "DEC_GenObject_Densite", &DEC_Gen_Object::GetDensity );
    brain.RegisterFunction( "DEC_GenObject_TypeObstacleManoeuvre", &DEC_ObjectFunctions::GetGenObjectReservedObstacle );
    brain.RegisterFunction( "DEC_GenObject_TC2", &DEC_ObjectFunctions::GetGenObjectTC2 );
    brain.RegisterMethod( "DEC_GenObject_DelaiActiviteMines", &DEC_Gen_Object::GetMinesActivityTime );
    brain.RegisterMethod( "DEC_GenObject_DateActivationMines", &DEC_Gen_Object::GetActivationTime );
    brain.RegisterMethod( "DEC_GenObject_Name", &DEC_Gen_Object::GetName );
    brain.RegisterMethod( "DEC_GenObject_HasCapacity", &DEC_Gen_Object::HasCapacity );
    brain.RegisterFunction( "DEC_GenObject_Mining", &DEC_ObjectFunctions::GetGenObjectMining );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterObjectivesFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterObjectivesFunctions( sword::Brain& brain )
{
    brain.RegisterMethod( "DEC_Objectif_Localisation", &DEC_Objective::GetLocalisation );
    brain.RegisterMethod( "DEC_Objectif_Flag",         &DEC_Objective::Flag );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterSpecificPointsFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterSpecificPointsFunctions( sword::Brain& brain )
{
    //Rep_Points
    brain.RegisterFunction( "DEC_GetRepPoint", &DEC_PathFunctions::GetRepPoint );
    brain.RegisterFunction( "DEC_IsAvantPoint", &DEC_PathFunctions::IsAvantPoint );
    brain.RegisterFunction( "DEC_IsPoint", &DEC_PathFunctions::IsPoint );
    brain.RegisterFunction( "DEC_GetTypePoint", &DEC_PathFunctions::GetTypePoint );
    brain.RegisterFunction( "DEC_GetDestPoint", &DEC_PathFunctions::GetDestPoint );
    brain.RegisterFunction( "DEC_GetTypeLimaPoint", &DEC_PathFunctions::GetTypeLimaPoint );
    brain.RegisterFunction( "DEC_GetLimaPoint", &DEC_PathFunctions::GetLimaPoint );
    brain.RegisterFunction( "DEC_GetNextRemovableObjectOnPath", &DEC_PathFunctions::GetNextRemovableObjectOnPath );
    brain.RegisterFunction( "DEC_GetClosestPath", &DEC_PathFunctions::GetClosestPath );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterTypeFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterTypeFunctions( sword::Brain& brain )
{
    brain.RegisterMethod( "GetType", &MIL_Mission_ABC::GetDIAType );
    brain.RegisterMethod( "GetType", &DEC_Decision_ABC::GetDIAType );
    brain.RegisterMethod( "GetType", &DEC_RolePion_Decision::GetDIAType );
    brain.RegisterMethod( "GetType", &DEC_AutomateDecision::GetDIAType );
    brain.RegisterMethod( "GetType", &TER_PathPoint::GetDIAType );
}

void RegisterOrderFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "_DEC_FinMission", &DEC_OrdersFunctions::FinishMission );
    brain.RegisterFunction( "_DEC_NouvelleMission", &DEC_OrdersFunctions::IsNewMissionStarted );
    brain.RegisterFunction( "_DEC_GetLima", &DEC_OrdersFunctions::GetLima );
    brain.RegisterFunction( "_DEC_GetLimasFromType", &DEC_OrdersFunctions::GetLimasFromType );
    brain.RegisterFunction( "_DEC_ProchaineLimaHoraireNonFlagee", &DEC_OrdersFunctions::GetNextScheduledLima );
    brain.RegisterFunction( "_DEC_Fuseau", &DEC_OrdersFunctions::GetFuseau );
    brain.RegisterFunction( "_DEC_SetMissionLimaFlag", &DEC_OrdersFunctions::AutomateSetMissionLimaFlag );
    brain.RegisterFunction( "_DEC_GetMissionLimaFlag", &DEC_OrdersFunctions::GetMissionLimaFlag );
    brain.RegisterFunction( "_DEC_SetMissionLimaFlagHoraire", &DEC_OrdersFunctions::AutomateSetMissionLimaScheduleFlag );

    // MRT / conduite
    brain.RegisterFunction( "_DEC_MRT_CreerMissionPion", &DEC_OrdersFunctions::MRT_CreatePionMission );
    brain.RegisterFunction( "_DEC_MRT_Valide", &DEC_OrdersFunctions::MRT_Validate );
    brain.RegisterFunction( "_DEC_MRT_AffecteFuseaux", &DEC_OrdersFunctions::MRT_AffectFuseaux );
    brain.RegisterFunction( "_DEC_CDT_CreerMissionPion", &DEC_OrdersFunctions::CDT_CreatePionMission );
    brain.RegisterFunction( "_DEC_CDT_DonnerMissionPion_Mission", &DEC_OrdersFunctions::CDT_GivePionMission );
    brain.RegisterFunction( "_DEC_CreerMissionAutomate", &DEC_OrdersFunctions::CreateAutomateMission );
    brain.RegisterFunction( "_DEC_DonnerMissionAutomate_Mission", &DEC_OrdersFunctions::GiveAutomateMission );
    brain.RegisterFunction( "_DEC_DonnerMissionAutomate", &DEC_OrdersFunctions::GiveAutomateMissionToAutomat );
    brain.RegisterFunction( "_DEC_DecouperFuseau", &DEC_OrdersFunctions::SplitFuseau );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterMissionParametersFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterMissionParametersFunctions( sword::Brain& brain, bool isMasalife )
{
    brain.RegisterFunction( "DEC_AssignMissionPionParameter", &MIL_MissionParameterFactory::SetPawnParameter );
    brain.RegisterFunction( "DEC_AssignMissionAutomatParameter", &MIL_MissionParameterFactory::SetAutomatParameter );
    brain.RegisterFunction( "DEC_AssignMissionBoolParameter", &MIL_MissionParameterFactory::SetBoolParameter );
    brain.RegisterFunction( "DEC_AssignMissionObjectKnowledgeParameter", &MIL_MissionParameterFactory::SetObjectKnowledgeParameter );
    brain.RegisterFunction( "DEC_AssignMissionNatureAtlasTypeParameter", &MIL_MissionParameterFactory::SetNatureAtlasTypeParameter );
    brain.RegisterFunction( "DEC_AssignMissionAutomatListParameter", &MIL_MissionParameterFactory::CreateAutomat );
    brain.RegisterFunction( "DEC_AssignMissionAgentKnowledgeParameter", &MIL_MissionParameterFactory::SetAgentKnowledgeParameter );
    brain.RegisterFunction( "DEC_AssignMissionPathListParameter", &MIL_MissionParameterFactory::CreatePathConst );
    brain.RegisterFunction( "DEC_AssignMissionEnumereParameter", &MIL_MissionParameterFactory::SetEnumereParameter );
    brain.RegisterFunction( "DEC_AssignMissionLocationParameter", &MIL_MissionParameterFactory::SetLocationParameter );
    brain.RegisterFunction( "DEC_AssignMissionObjectKnowledgeListParameter", &MIL_MissionParameterFactory::CreateObjectKnowledge );
    brain.RegisterFunction( "DEC_AssignMissionPointParameter", &MIL_MissionParameterFactory::SetPointParameter );
    brain.RegisterFunction( "DEC_AssignMissionAgentKnowledgeListParameter", &MIL_MissionParameterFactory::CreateAgentKnowledge );
    brain.RegisterFunction( "DEC_AssignMissionGenObjectParameter", &MIL_MissionParameterFactory::SetGenObjectParameter );
    brain.RegisterFunction( "DEC_AssignMissionGenObjectListParameter", &MIL_MissionParameterFactory::CreateGenObject );
    brain.RegisterFunction( "DEC_AssignMissionPionListParameter", &MIL_MissionParameterFactory::CreatePion );
    brain.RegisterFunction( "DEC_AssignMissionLocationListParameter", &MIL_MissionParameterFactory::CreateLocation );
    brain.RegisterFunction( "DEC_AssignMissionPointListParameter", &MIL_MissionParameterFactory::CreatePoint );
    brain.RegisterFunction( "DEC_AssignMissionUrbanBlockParameter", &MIL_MissionParameterFactory::SetUrbanBlockParameter );
    brain.RegisterFunction( "DEC_AssignMissionUrbanBlockListParameter", &MIL_MissionParameterFactory::CreateUrbanBlock );
    brain.RegisterFunction( "DEC_AssignMissionDirectionParameter", &MIL_MissionParameterFactory::SetDirectionParameter );
    brain.RegisterFunction( "DEC_AssignMissionDotationTypeParameter", &MIL_MissionParameterFactory::SetDotationTypeParameter );
    brain.RegisterFunction( "DEC_AssignMissionNumericTypeParameter", &MIL_MissionParameterFactory::SetNumericTypeParameter );
    brain.RegisterFunction( "DEC_AssignMissionResourceNetworkParameter", &MIL_MissionParameterFactory::SetResourceNetworkNodeParameter );
    brain.RegisterFunction( "DEC_AssignMissionResourceNetworkListParameter", &MIL_MissionParameterFactory::CreateResourceNetworkNode );
    brain.RegisterFunction( "DEC_AssignMissionResourceNetworkTypeParameter", &MIL_MissionParameterFactory::SetResourceNetworkTypeParameter );
    brain.RegisterFunction( "DEC_AssignMissionResourceNetworkTypeListParameter", &MIL_MissionParameterFactory::CreateResourceNetworkType );
    brain.RegisterFunction( "DEC_AssignMissionPathConstParameter", &MIL_MissionParameterFactory::AssignPathConst );
    brain.RegisterFunction( "DEC_IsFragOrderAvailable", &DEC_OrdersFunctions::IsFragOrderAvailable );
    brain.RegisterFunction( "DEC_IsMissionAvailable", &DEC_OrdersFunctions::IsPionMissionAvailable );
    brain.RegisterFunction( "DEC_Mission_IsPath", &DEC_OrdersFunctions::DEC_Mission_IsPath );
    brain.RegisterFunction( "DEC_IsAutomateMissionAvailable", &DEC_OrdersFunctions::IsAutomateMissionAvailable );
    directia::tools::binders::ScriptRef initParameterFunction = brain.GetScriptRef( "InitTaskParameter" );
    brain.RegisterFunction( "DEC_FillMissionParameters",
        std::function< void( const directia::tools::binders::ScriptRef&, boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_MiscFunctions::FillMissionParameters, boost::ref(brain), initParameterFunction, _1 , _2, isMasalife ) ) );
    brain.RegisterFunction( "DEC_AssignerFuseauAMissionPion_Mission",
        std::function< void( MIL_Fuseau* ,  boost::shared_ptr< MIL_Mission_ABC > )>( boost::bind( &DEC_OrdersFunctions::AssignFuseauToPionMission , _1, _2 ) ) );
    brain.RegisterFunction( "DEC_AssignerFuseauAMissionAutomate_Mission",
            std::function< void( MIL_Fuseau* ,  boost::shared_ptr< MIL_Mission_ABC > )>( boost::bind( &DEC_OrdersFunctions::AssignFuseauToAutomateMission , _1, _2 ) ) );
    brain.RegisterFunction( "DEC_AssignerDirectionAMissionAutomate_Mission",
            std::function< void (MT_Vector2D* ,  boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_OrdersFunctions::AssignDirectionToAutomateMission , _1, _2 ) ) );
    brain.RegisterFunction( "DEC_AssignMissionListParameter", &MIL_MissionParameterFactory::AssignMissionListParameter );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterObjectFunctions
// Created: LDC 2012-09-06
// -----------------------------------------------------------------------------
void RegisterObjectFunctions( sword::Brain& brain )
{
    // Objet
    brain.RegisterFunction( "DEC_Agent_AgentPeutConstruireObjetEmbarque", &DEC_AgentFunctions::AgentCanConstructObjectWithLoaded );
    brain.RegisterFunction( "DEC_Agent_AgentPeutConstruireObjetEmbarqueAvecLocalisation", &DEC_AgentFunctions::AgentCanConstructObjectWithLoadedAndLocalisation );
    brain.RegisterFunction( "DEC_Agent_AgentADotationPourConstruireObjet", &DEC_AgentFunctions::AgentHasDotationForBuilding );
    brain.RegisterFunction( "DEC_Agent_AgentADotationPourConstruireObjetSansRenfort", &DEC_AgentFunctions::AgentHasDotationForBuildingWithOutLoaded );
    brain.RegisterFunction( "DEC_Agent_AgentPeutDetruireObjet", &DEC_AgentFunctions::AgentCanDestroyObject );
    brain.RegisterFunction( "DEC_Agent_AgentPeutConstruireContournementObjet", &DEC_AgentFunctions::AgentCanBypassObject );
    brain.RegisterFunction( "DEC_Agent_AgentPeutDetruireTypeObjet", &DEC_AgentFunctions::AgentCanDestroyObjectType );
    brain.RegisterFunction( "DEC_Agent_AgentPeutDetruireTypeObjetAvecLocalisation", &DEC_AgentFunctions::AgentCanDestroyObjectTypeWithLocalisation );

    brain.RegisterFunction( "DEC_Agent_PeutActiverObjet", &DEC_AgentFunctions::CanActivateObject );

    brain.RegisterFunction( "DEC_Agent_GetAgentDotation", &DEC_AgentFunctions::GetAgentDotationNumber );
    brain.RegisterFunction( "DEC_GetAgentDotationManquantePourConstruireObjet", &DEC_AgentFunctions::GetAgentMissingDotationForBuildingObject );
    brain.RegisterFunction( "DEC_GetAgentDotationManquantePourConstruireObjetExistant", &DEC_AgentFunctions::GetAgentMissingDotationForBuildingExistingObject );
    brain.RegisterFunction( "DEC_GetAgentDotationManquantePourValoriserObjet", &DEC_AgentFunctions::GetAgentMissingDotationForMining );
    brain.RegisterFunction( "DEC_GetAgentsPouvantConstruire", &DEC_AgentFunctions::RetrieveUnitsAbleToBuild );
    brain.RegisterFunction( "DEC_GetAgentsPouvantConstruireAvecLocalisation", &DEC_AgentFunctions::RetrieveUnitsAbleToBuildWithLocalisation );
    brain.RegisterFunction( "DEC_GetAgentsPouvantValoriser", &DEC_AgentFunctions::RetrieveUnitsAbleToMine );
    brain.RegisterFunction( "DEC_GetAgentsPouvantValoriserAvecLocalisation", &DEC_AgentFunctions::RetrieveUnitsAbleToMineWithLocalisation );
    brain.RegisterFunction( "DEC_GetAgentsPouvantContourner", &DEC_AgentFunctions::RetrieveUnitsAbleToByPass );
    brain.RegisterFunction( "DEC_GetAgentsPouvantContournerAvecLocalisation", &DEC_AgentFunctions::RetrieveUnitsAbleToByPassWithLocalisation );
    brain.RegisterFunction( "DEC_GetAgentsPouvantDetruire", &DEC_AgentFunctions::RetrieveUnitsAbleToDestroy );
    brain.RegisterFunction( "DEC_GetAgentsPouvantDetruireAvecLocalisation", &DEC_AgentFunctions::RetrieveUnitsAbleToDestroyWithLocalisation );
    brain.RegisterFunction( "DEC_GetAgentsPouvantDevaloriserAvecLocalisation", &DEC_AgentFunctions::RetrieveUnitsAbleToDemineWithLocalisation );
    brain.RegisterFunction( "DEC_GetAgentsPouvantDevaloriserEmbarqueAvecLocalisation", &DEC_AgentFunctions::RetrieveUnitsAbleToDemineWithOutLoadedWithLocalisation );

    brain.RegisterFunction( "S_TypeObject_ToString", &DEC_ObjectFunctions::ConvertTypeObjectToString );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_PeutEtreValorise", &DEC_KnowledgeObjectFunctions::CanBeValorized );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstUneIssueDeReseauSouterrain", &DEC_KnowledgeObjectFunctions::IsUndergroundNetworkExit );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_ActiverIssueDeReseauSouterrain", &DEC_KnowledgeObjectFunctions::ActivateUndergroundNetworkExit );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_DesactiverIssueDeReseauSouterrain", &DEC_KnowledgeObjectFunctions::DeactivateUndergroundNetworkExit );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_IssuesDuMemeReseauSouterrain", DEC_KnowledgeObjectFunctions::EnterAndExitInSameUndergroundNetwork );
    brain.RegisterFunction( "DEC_ObjectKnowledge_HasCapacity", &DEC_KnowledgeObjectFunctions::HasCapacity );
    brain.RegisterFunction( "DEC_ObjectKnowledge_BuildInstantaneously", &DEC_KnowledgeObjectFunctions::BuildInstantaneously );
    brain.RegisterFunction( "DEC_ObjectKnowledge_IsFullMined", &DEC_KnowledgeObjectFunctions::IsFullMined );
    brain.RegisterFunction( "DEC_ObjectKnowledge_MustBeMined", &DEC_KnowledgeObjectFunctions::MustBeMined );
    brain.RegisterFunction( "DEC_ObjectKnowledge_IsTrafficable", &DEC_KnowledgeObjectFunctions::IsObjectTrafficable );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstConstuit", &DEC_KnowledgeObjectFunctions::IsConstructed );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstObstacleDeManoeuvreActif", &DEC_KnowledgeObjectFunctions::IsObstacleActivated );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_DateActivationObstacle", &DEC_KnowledgeObjectFunctions::GetActivationTime );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstObstacleDeManoeuvre", &DEC_KnowledgeObjectFunctions::IsActivableObstacle );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstContourne", &DEC_KnowledgeObjectFunctions::IsBypassed );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstValorise", &DEC_KnowledgeObjectFunctions::IsMined );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstBreche", &DEC_KnowledgeObjectFunctions::IsBreached );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_NiveauConstruction", &DEC_KnowledgeObjectFunctions::GetConstructionLevel );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_NiveauValorisation", &DEC_KnowledgeObjectFunctions::GetValorizationLevel );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Localisation", &DEC_KnowledgeObjectFunctions::GetLocalisation );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Type", &DEC_KnowledgeObjectFunctions::GetType );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Name", &DEC_KnowledgeObjectFunctions::GetName );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_NiveauAnimation", &DEC_KnowledgeObjectFunctions::GetAnimationLevel );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_LargeurSiteFranchissement", &DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstReconnu", &DEC_KnowledgeObjectFunctions::IsRecon );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_PeutEtreContourne", &DEC_KnowledgeObjectFunctions::CanBeBypassed );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_ChangeDensitePopulationSortante", &DEC_KnowledgeObjectFunctions::SetExitingPopulationDensity );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_ResetDensitePopulationSortante", &DEC_KnowledgeObjectFunctions::ResetExitingPopulationDensity );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_PointEstDansZoneEvitement", &DEC_KnowledgeObjectFunctions::IsInAvoidanceArea );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_PointEstProcheZoneEffet", &DEC_KnowledgeObjectFunctions::IsNearEffectArea );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_BurningLevel", &DEC_KnowledgeObjectFunctions::GetBurningLevel );
    brain.RegisterFunction( "DEC_IsValidKnowledgeObject", &DEC_KnowledgeObjectFunctions::IsKnowledgeValid );    
    brain.RegisterFunction( "DEC_ActiverObjet", &DEC_ObjectFunctions::ActivateObject );
    brain.RegisterFunction( "DEC_CreateDynamicGenObject", &DEC_ObjectFunctions::CreateDynamicGenObject );
    brain.RegisterFunction( "DEC_CreateDynamicGenObjectFromSharedLocalisation", &DEC_ObjectFunctions::CreateDynamicGenObjectFromSharedLocalisation );
    brain.RegisterFunction( "DEC_DetruireObjetIdSansDelais", &DEC_ObjectFunctions::MagicDestroyObjectId );
    brain.RegisterFunction( "DEC_DetruireObjetSansDelais", &DEC_ObjectFunctions::MagicDestroyObject );
    brain.RegisterFunction( "DEC_Agent_SeDissimulerDansReseauSouterrain", &DEC_KnowledgeObjectFunctions::HideInUndergroundNetwork );
    brain.RegisterFunction( "DEC_Agent_SortirDuReseauSouterrain", &DEC_KnowledgeObjectFunctions::GetOutFromUndergroundNetwork );
    brain.RegisterFunction( "DEC_Agent_TempsPourTraverserReseauSouterrain", &DEC_KnowledgeObjectFunctions::EstimatedUndergroundTime );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_DemandeDeDecontaminationSurPion", &DEC_KnowledgeObjectFunctions::QueueUnitForDecontamination );
    brain.RegisterFunction( "DEC_Agent_DecontamineConnaissance",
        std::function< int( boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::QueueKnowledgeForDecontamination, _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Circulation_EquiperItineraireLogistique", &DEC_KnowledgeObjectFunctions::EquipLogisticRoute );

    brain.RegisterFunction( "_DEC_CreerObjetSansDelais", &DEC_ObjectFunctions::MagicCreateObject );
    brain.RegisterFunction( "_DEC_MagicGetOrCreateObject", &DEC_ObjectFunctions::MagicGetOrCreateObject );
    brain.RegisterFunction( "DEC_ObjectKnowledge_IsImpassable", &DEC_KnowledgeObjectFunctions::IsImpassable );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterReportFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterReportFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_PointToString", &DEC_DIAFunctions::PointToString );
    brain.RegisterFunction( "DEC_DirectionToString",  &DEC_DIAFunctions::DirectionToString  );
    brain.RegisterFunction( "DEC_ItineraireToString", &DEC_DIAFunctions::PathToString  );
    brain.RegisterFunction( "DEC_UnitDecisionalState", &DEC_KnowledgeAgentFunctions::UnitDecisionalState );
    brain.RegisterFunction( "DEC_RC1",
        std::function< void ( DEC_Decision_ABC&, int, const std::string& ) >( boost::bind( &DEC_MiscFunctions::Report, _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_AgentKnowledge",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_MiscFunctions::ReportAgentKnowledge, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_DotationType",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, const PHY_DotationCategory* ) >( boost::bind( &DEC_MiscFunctions::ReportDotationType, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_EquipmentType",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, const PHY_ComposanteTypePion* ) >( boost::bind( &DEC_MiscFunctions::ReportEquipmentType, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_Float",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, float ) >( boost::bind( &DEC_MiscFunctions::ReportFloat, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_Int_Int",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, int, int ) >( boost::bind( &DEC_MiscFunctions::ReportIntInt, _1, _2, _3, _4, _5 ) ) );
    brain.RegisterFunction( "DEC_RC_AgentKnowledge_Int",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, boost::shared_ptr< DEC_Knowledge_Agent >, int ) >( boost::bind( &DEC_MiscFunctions::ReportAgentKnowledgeInt, _1, _2, _3, _4, _5 ) ) );
    brain.RegisterFunction( "DEC_RC_Float_Float",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, float, float ) >( boost::bind( &DEC_MiscFunctions::ReportFloatFloat, _1, _2, _3, _4, _5 ) ) );
    brain.RegisterFunction( "DEC_RC_Id",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, int ) >( boost::bind( &DEC_MiscFunctions::ReportId, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_ObjectKnowledge",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_MiscFunctions::ReportObjectKnoweldge, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_Pion",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::ReportPion, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_Pion_Automate",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, DEC_Decision_ABC*, DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::ReportPionAutomate, _1, _2, _3, _4, _5 ) ) );
    brain.RegisterFunction( "DEC_RC_Pion_Pion",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, DEC_Decision_ABC*, DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::ReportPionPion, _1, _2, _3, _4, _5 ) ) );
    brain.RegisterFunction( "DEC_RC_Pion_Int",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, DEC_Decision_ABC*, int ) >( boost::bind( &DEC_MiscFunctions::ReportPionInt, _1, _2, _3, _4, _5 ) ) );
    brain.RegisterFunction( "DEC_RC_PopulationKnowledge",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, int ) >( boost::bind( &DEC_MiscFunctions::ReportPopulationKnowledge, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_TirPion",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, int ) >( boost::bind( &DEC_MiscFunctions::ReportTirPion, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_String",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, const std::string& ) >( boost::bind( &DEC_MiscFunctions::ReportString, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_Stage",
        std::function< void ( DEC_Decision_ABC&, int, const std::string&, const std::string& ) >( boost::bind( &DEC_MiscFunctions::ReportStage, _1, _2, _3, _4 ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterTelepathyFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterTelepathyFunctions( sword::Brain& brain )
{
    brain.RegisterMethod( "GetBoolVariable", &DEC_Decision_ABC::GetVariable< bool > );
    brain.RegisterMethod( "SetStateVariable", &DEC_Decision_ABC::SetStateVariable );
    brain.RegisterMethod( "SetambianceMission_", &DEC_Decision_ABC::SetAmbianceMission );
    brain.RegisterMethod( "SetbAppuieFreinage_", &DEC_Decision_ABC::SetAppuieFreinage );
    brain.RegisterMethod( "GetbDemandeOrdreConduitePoursuivre_", &DEC_Decision_ABC::GetDemandeOrdreConduitePoursuivre );
    brain.RegisterMethod( "GetbEnCoursExtractionPersonnel_", &DEC_Decision_ABC::GetEnCoursExtractionPersonnel );
    brain.RegisterMethod( "GetbEnExploitation_", &DEC_Decision_ABC::GetEnExploitation );
    brain.RegisterMethod( "SetbEnExploitation_", &DEC_Decision_ABC::SetbEnExploitation );
    brain.RegisterMethod( "GetbEnPhaseRavitaillement_", &DEC_Decision_ABC::GetEnPhaseRavitaillement );
    brain.RegisterMethod( "SetbEnPhaseRavitaillement_", &DEC_Decision_ABC::SetEnPhaseRavitaillement );
    brain.RegisterMethod( "GetbMiseEnOeuvre_", &DEC_Decision_ABC::GetMiseEnOeuvre );
    brain.RegisterMethod( "SetbMiseEnOeuvre_", &DEC_Decision_ABC::SetMiseEnOeuvre );
    brain.RegisterMethod( "GeteEtatFeu_", &DEC_Decision_ABC::GetEtatFeu );
    brain.RegisterMethod( "GetlisteEnisTirAutorise_", &DEC_Decision_ABC::GetListeEnisTirAutorise );
    brain.RegisterMethod( "SetlisteEnisTirAutorise_", &DEC_Decision_ABC::SetListeEnisTirAutorise );
    brain.RegisterMethod( "ClearlisteEnisTirAutorise_", &DEC_Decision_ABC::ClearListeEnisTirAutorise );
    brain.RegisterMethod( "GetlistePionsCoordination_", &DEC_Decision_ABC::GetListePionsCoordination );
    brain.RegisterMethod( "ClearlistePionsCoordination_", &DEC_Decision_ABC::ClearListePionsCoordination );
    brain.RegisterMethod( "GetptSauvegarde_", &DEC_Decision_ABC::GetPtSauvegarde );
    brain.RegisterMethod( "GetlisteZonesTir_", &DEC_Decision_ABC::GetListeZonesTir );
    brain.RegisterMethod( "ClearlisteZonesTir_", &DEC_Decision_ABC::ClearListeZonesTir );
    brain.RegisterMethod( "GetobjMisEnCours_", &DEC_Decision_ABC::GetObjMisEnCours );
    brain.RegisterMethod( "SetobjMisEnCours_", &DEC_Decision_ABC::SetObjMisEnCours );
    brain.RegisterMethod( "GetsuppliesLoaded_", &DEC_Decision_ABC::GetSuppliesLoaded );
    brain.RegisterMethod( "SetsuppliesLoaded_", &DEC_Decision_ABC::SetSuppliesLoaded );
    brain.RegisterMethod( "GetobjectifCourant_", &DEC_Decision_ABC::GetObjectifCourant );
    brain.RegisterMethod( "GetplotRavitaillementAssigne_", &DEC_Decision_ABC::GetPlotRavitaillementAssigne );
    brain.RegisterMethod( "SetplotRavitaillementAssigne_", &DEC_Decision_ABC::SetPlotRavitaillementAssigne );
    brain.RegisterMethod( "ClearplotsRavitaillement_", &DEC_Decision_ABC::ClearPlotsRavitaillement );
    brain.RegisterMethod( "GetporteeAction_", &DEC_Decision_ABC::GetPorteeAction );
    brain.RegisterMethod( "SetporteeAction_", &DEC_Decision_ABC::SetPorteeAction );
    brain.RegisterMethod( "GetrNiveauAlerteRavitaillement_", &DEC_Decision_ABC::GetNiveauAlerteRavitaillement );
    brain.RegisterMethod( "Getpoint_", &TER_PathPoint::GetPos );
    brain.RegisterMethod( "Setmunitions_", &DEC_Decision_ABC::SetMunition );
    brain.RegisterMethod( "Getmunitions_", &DEC_Decision_ABC::GetMunition );
    brain.RegisterMethod( "SetnbIT_", &DEC_Decision_ABC::SetNbIt );
    brain.RegisterMethod( "GetnbIT_", &DEC_Decision_ABC::GetNbIt );
    brain.RegisterMethod( "Setcible_", &DEC_Decision_ABC::SetTarget );
    brain.RegisterMethod( "Getcible_", &DEC_Decision_ABC::GetTarget );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterItineraryFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterItineraryFunctions( sword::Brain& brain )
{
    brain.RegisterMethod( "DEC_Itineraire_DernierPoint", &DEC_Decision_ABC::GetLastPointOfPath );
    brain.RegisterMethod( "DEC_Itineraire_ExtrapolerPosition", &DEC_Decision_ABC::ExtrapolatePosition );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterToolsFunctions
// Created: LGY 2012-11-21
// -----------------------------------------------------------------------------
void RegisterToolsFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_RandomValue", &DEC_DIAFunctions::GetRandomValue );
    brain.RegisterFunction( "DEC_Perception_AjouterPointReconnuAutomateParPion", &DEC_PerceptionFunctions::AddReconnoiteredPointBy );
    brain.RegisterFunction( "DEC_Perception_EstPointReconnuParPionOuPersonne", &DEC_PerceptionFunctions::PointIsReconnoiteredByMeOrNoOne );
}

void RegisterTransportFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_CanLoad", &DEC_ActionFunctions::CanLoad );
    brain.RegisterFunction( "DEC_RoundTripsLeftToTransportAgent", &DEC_ActionFunctions::GetNumberOfRoundTripsLeftToTransportPion );
}

void RegisterMiscellaneousFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_GetSzName", &DEC_MiscFunctions::GetName );

    // Knowledge objects
    brain.RegisterFunction( "_DEC_ObjectKnowledgesInZone", &DEC_KnowledgeFunctions::GetObjectsInZone );
}

void RegisterDebugFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "_DEC_DebugAffichePoint", &DEC_MiscFunctions::DebugDrawPoint );
    brain.RegisterFunction( "_DEC_DebugAffichePoints", &DEC_MiscFunctions::DebugDrawPoints );
    brain.RegisterFunction( "_DEC_Debug", &DEC_MiscFunctions::Debug );
    brain.RegisterFunction( "_DEC_Trace", &DEC_MiscFunctions::Trace );
}

void RegisterEntityFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "_DEC_DecisionalState", &DEC_EntityFunctions::DecisionalState );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterCommonUserFunctions
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
void RegisterCommonUserFunctions( sword::Brain& brain, bool isMasalife )
{
    RegisterUnitFunctions( brain );
    RegisterAutomatonFunctions( brain );
    RegisterPopulationFunctions( brain );
    RegisterActionFunctions( brain );
    RegisterAgentKnowledgeFunctions( brain );
    RegisterPopulationKnowledgeFunctions( brain );
    RegisterKnowledgeFunctions( brain );
    RegisterRepresentationFunctions( brain );
    RegisterGeometryFunctions( brain );
    RegisterUrbanBlockFunctions( brain );
    RegisterResourceNetworkFunctions( brain );
    RegisterAreaFunctions( brain );
    RegisterFuseauFunctions( brain );
    RegisterTimeManagementFunctions( brain );
    RegisterParametersCopyFunctions( brain );
    RegisterListsManipulationFunctions( brain );
    RegisterLogisticFunctions( brain );
    RegisterEngineerObjectsFunctions( brain );
    RegisterObjectFunctions( brain );
    RegisterObjectivesFunctions( brain );
    RegisterSpecificPointsFunctions( brain );
    RegisterTypeFunctions( brain );
    RegisterMissionParametersFunctions( brain, isMasalife );
    RegisterOrderFunctions( brain );
    RegisterReportFunctions( brain );
    RegisterTelepathyFunctions( brain );
    RegisterItineraryFunctions( brain );
    RegisterToolsFunctions( brain );
    RegisterTransportFunctions( brain );
    RegisterMiscellaneousFunctions( brain );
    RegisterDebugFunctions( brain );
    RegisterEntityFunctions( brain );
    DEC_CommunicationFunctions::Register( brain );
    DEC_TelepathyFunctions::Register( brain );
    MIL_FragOrder::Register( brain );
}

typedef bool (*T_Function)( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element );
typedef bool (*T_FunctionBM)( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element );

bool BoolFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    bool value;
    if( element.ToBool( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool BoolFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    BoolFunction( refMission, name, element );
    return true;
}
bool EnumerationFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    int value;
    if( element.ToId( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool EnumerationFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    EnumerationFunction( refMission, name, element );
    return true;
}
bool PointFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< MT_Vector2D > value;
    if( element.ToPoint( value ) && value.get() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool PointListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPointList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool PointFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< MT_Vector2D > value;
    if( element.ToPoint( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.point" ), name, value, false );
        return true;
    }
    return false;
}

bool PointListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPointList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.point" ), name, value, true );
        return true;
    }
    return false;
}
bool PolygonFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToPolygon( value ) && value.get() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool PolygonListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToPolygonList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool AreaFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToPolygon( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.area" ), name, value, false );
        return true;
    }
    return false;
}
bool AreaListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToPolygonList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.area" ), name, value, true );
        return true;
    }
    return false;
}
bool LocationFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToLocation( value ) && value.get() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool LocationListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToLocationList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool PathFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPath( value ) && !value.empty() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool PathListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< std::vector< boost::shared_ptr< MT_Vector2D > > > value;
    if( element.ToPathList( value ) && !value.empty() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool PathFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPath( value ) && !value.empty() )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.point" ), name, value, true );
        return true;
    }
    return false;
}
bool PathListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > list;
    if( element.ToList( list ) )
    {
        knowledgeCreateFunction( refMission, "beginlist", name );
        for( auto it = list.begin(); it != list.end(); ++it )
            PathFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it );
        knowledgeCreateFunction( refMission, "endlist", name );
        return true;
    }
    return false;
}
bool DirectionFunction( const directia::tools::binders::ScriptRef& /*refMission*/, const std::string& /*name*/, MIL_MissionParameter_ABC& /*element*/ )
{
    // $$$$ LDC: FIXME The only existing Direction argument is dangerDirection_ which is never used by the brains.
    return false;
}
bool DirectionFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< MT_Vector2D > value;
    if( element.ToDirection( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission,  brain.GetScriptRef( "integration.ontology.types.direction" ), name, value, false );
        return true;
    }
    return false;
}
bool NatureAtlasFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    int value = 0;
    if( element.ToNatureAtlas( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool NatureAtlasFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    return NatureAtlasFunction( refMission, name, element );
}
bool AutomatFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0;
    if( element.ToAutomat( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool AutomatFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0;
    if( element.ToAutomat( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.automat" ), name, value, false );
        return true;
    }
    return false;
}
bool AutomatListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAutomatList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool AutomatListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAutomatList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.automat" ), name, value, true );
        return true;
    }
    return false;
}
bool AgentFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const DEC_Decision_ABC* value = 0; // $$$$ LDC: Parfois on se sert de champs dessus comme eniEnCours_...
    if( element.ToAgent( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool AgentFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const DEC_Decision_ABC* value = 0; // $$$$ LDC: Parfois on se sert de champs dessus comme eniEnCours_...
    if( element.ToAgent( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.agent" ), name, value, false );
        return true;
    }
    return false;
}
bool AgentListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const DEC_Decision_ABC* > value;
    if( element.ToAgentList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool AgentListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const DEC_Decision_ABC* > value;
    if( element.ToAgentList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.agent" ), name, value, true );
        return true;
    }
    return false;
}
bool AgentKnowledgeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Agent > value;
    if( element.ToAgentKnowledge( value ) && value.get() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool AgentKnowledgeFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Agent > value;//@TODO SEE how to bind agent and knowledge agent with the same BM knowledge
    if( element.ToAgentKnowledge( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.agentKnowledge" ), name, value, false );
        return true;
    }
    return false;
}
bool AgentKnowledgeListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > value;
    if( element.ToAgentKnowledgeList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool AgentKnowledgeListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > value;
    if( element.ToAgentKnowledgeList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.agentKnowledge" ), name, value, true );
        return true;
    }
    return false;
}
bool ObjectKnowledgeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Object > value;
    if( element.ToObjectKnowledge( value ) && value && value->IsValid() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool ObjectKnowledgeFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Object > value;
    if( element.ToObjectKnowledge( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.object" ), name, value, false );
        return true;
    }
    return false;
}
bool ObjectKnowledgeListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > value;
    if( element.ToObjectKnowledgeList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool ObjectKnowledgeListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > value;
    if( element.ToObjectKnowledgeList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.object" ), name, value, true );
        return true;
    }
    return false;
}
bool PopulationKnowledgeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Population > value;
    if( element.ToPopulationKnowledge( value ) && value )
    {
        refMission[ name ] = value->GetID();
        return true;
    }
    return false;
}
bool PopulationKnowledgeFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Population > value;
    if( element.ToPopulationKnowledge( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.population" ), name, value->GetID(), false );//@TODO MGD Add CompositeReachable for Population?
         return true;
    }
    return false;
}

bool UrbanBlockFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    MIL_UrbanObject_ABC* value = 0;
    if( element.ToUrbanBlock( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.urbanBlock" ), name, value, false );
        return true;
    }
    return false;
}

bool UrbanBlockListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< MIL_UrbanObject_ABC* > value;
    if( element.ToUrbanBlockList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.urbanBlock" ), name, value, true );
        return true;
    }
    return false;
}

bool DotationTypeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const PHY_DotationCategory* value = 0;
    if( element.ToDotationType( value ) && value )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}

bool DotationTypeFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DotationTypeFunction( refMission, name, element );
    return true;
}

bool DotationTypeListFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_DotationCategory* > value;
    if( element.ToDotationTypeList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}

bool ResourceNetworkNodeFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_ResourceNetwork > value;
    if( element.ToResourceNetworkNode( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.resourceNetwork" ), name, value, false );
        return true;
    }
    return false;
}

bool ResourceNetworkNodeListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_ResourceNetwork > > value;
    if( element.ToResourceNetworkNodeList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.resourceNetwork" ), name, value, true );
        return true;
    }
    return false;
}

bool ResourceNetworkTypeFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const PHY_ResourceNetworkType* value = 0;
    if( element.ToResourceNetworkType( value ) && value )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}

bool ResourceNetworkTypeListFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_ResourceNetworkType* > value;
    if( element.ToResourceNetworkTypeList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}

bool EquipmentTypeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const PHY_ComposanteTypePion* value = 0;
    if( element.ToEquipmentType( value ) && value )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}

bool EquipmentTypeFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const PHY_ComposanteTypePion* value = 0;
    if( element.ToEquipmentType( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.equipmentType" ), name, value, false );
        return true;
    }
    return false;
}

bool EquipmentTypeListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_ComposanteTypePion* > value;
    if( element.ToEquipmentTypeList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.equipmentType" ), name, value, true );
        return true;
    }
    return false;
}

bool GDHFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    float value = 0;
    if( element.ToGDH( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool GDHFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    GDHFunction( refMission, name, element );
    return true;
}
bool NumericFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    float value = 0;
    if( element.ToNumeric( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool NumericFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    NumericFunction( refMission, name, element );
    return true;
}
bool NumericListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
     std::vector< float > value;
     if( element.ToNumericList( value ) )
     {
         refMission[ name ] = value;
         return true;
     }
     return false;
}
bool NumericListFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    NumericListFunction( refMission, name, element );
    return true;
}
bool GenObjectFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Gen_Object > value;
    if( element.ToGenObject( value ) && value.get() )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool GenObjectFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Gen_Object > value;
    if( element.ToGenObject( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.genObject" ), name, value, false );
        return true;
    }
    return false;
}
bool GenObjectListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Gen_Object > > value;
    if( element.ToGenObjectList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool GenObjectListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Gen_Object > > value;
    if( element.ToGenObjectList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.genObject" ), name, value, true );
        return true;
    }
    return false;
}
bool MaintenancePrioritiesFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_ComposanteTypePion* > value;
    if( element.ToMaintenancePriorities( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.equipmentType" ), name, value, true );
        return true;
    }
    return false;
}
bool MaintenancePrioritiesFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_ComposanteTypePion* > value;
    if( element.ToMaintenancePriorities( value ) )
    {
        refMission[ name ] = value ;
        return true;
    }
    return false;
}
bool MedicalPrioritiesFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_HumanWound* > value;
    if( element.ToMedicalPriorities( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.humanWound" ), name, value, true );
        return true;
    }
    return false;
}
bool MedicalPrioritiesFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_HumanWound* > value;
    if( element.ToMedicalPriorities( value ) )
    {
        refMission[ name ] = value ;
        return true;
    }
    return false;
}
bool IndirectFireFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    int idIndirectFire;
    if( element.ToId( idIndirectFire ) )
    {
        refMission[ name ] = idIndirectFire;
        return true;
    }
    return false;
}
bool StringFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::string value;
    if( element.ToString( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool StringFunctionBM( sword::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    StringFunction( refMission, name, element );
    return true;
}
bool ObjectiveListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Objective > > value;
    if( element.ToObjectiveList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}
bool LocationCompositeFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    return PointFunction( refMission, name, element )
        || PathFunction( refMission, name, element )
        || PolygonFunction( refMission, name, element )
        || AutomatFunction( refMission, name, element )
        || AgentFunction( refMission, name, element )
        || AgentKnowledgeFunction( refMission, name, element )
        || ObjectKnowledgeFunction( refMission, name, element )
        || PopulationKnowledgeFunction( refMission, name, element );
}

bool LocationCompositeListFunction( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > list;
    if( element.ToList( list ) )
    {
        std::vector< const DEC_Decision_ABC* > result;
        for( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >::const_iterator it = list.begin(); it != list.end(); ++it )
        {
            const DEC_Decision_ABC* value = 0;
            if( (*it)->ToAgent( value ) )
                result.push_back( value );
            else
            {
                boost::shared_ptr< DEC_Knowledge_Agent > knowledge;
                if( (*it)->ToAgentKnowledge( knowledge ) && knowledge.get() )
                    result.push_back( &knowledge->GetAgentKnown().GetDecision() );
            }
        }
        refMission[ name ] = result;
        return true;
    }
    return false;
}

bool LocationCompositeFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    return PointFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || PathFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || AreaFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || AutomatFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || AgentFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || AgentKnowledgeFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || ObjectKnowledgeFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || PopulationKnowledgeFunctionBM( brain, knowledgeCreateFunction, refMission, name, element )
        || UrbanBlockFunctionBM( brain, knowledgeCreateFunction, refMission, name, element );
}

bool LocationCompositeListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > list;
    if( element.ToList( list ) )
    {
        knowledgeCreateFunction( refMission, "beginlist", name );
        for( auto it = list.begin(); it != list.end(); ++it )
        {
            PointFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || PathFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || AreaFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || AutomatFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || AgentFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || AgentKnowledgeFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || ObjectKnowledgeFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || PopulationKnowledgeFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it )
        || UrbanBlockFunctionBM( brain, knowledgeCreateFunction, refMission, name, **it );
        }
        knowledgeCreateFunction( refMission, "endlist", name );
        return true;
    }
    return false;
}

bool PhaseLineFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToLima( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.limaStatique" ), name, value, false );
        return true;
    }
    return false;
}

bool PhaseLineListFunctionBM( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToLimaList( value ) )
    {
        knowledgeCreateFunction( refMission, brain.GetScriptRef( "integration.ontology.types.limaStatique" ), name, value, true );
        return true;
    }
    return false;
}

std::map< std::string, T_Function > functors;
std::map< std::string, T_FunctionBM > functorsBM;

void InitFunctions()
{
    if( functors.empty() )
    {
        functors[ "Agent" ] = AgentFunction;
        functors[ "AgentKnowledge" ] = AgentKnowledgeFunction;
        functors[ "AgentKnowledgeList" ] = AgentKnowledgeListFunction;
        functors[ "AgentList" ] = AgentListFunction;
        functors[ "Automat" ] = AutomatFunction;
        functors[ "AutomatList" ] = AutomatListFunction;
        functors[ "Boolean" ] = BoolFunction;
        functors[ "CrowdKnowledge" ] = PopulationKnowledgeFunction;
        functors[ "DateTime" ] = GDHFunction;
        functors[ "datetime" ] = GDHFunction;
        functors[ "Enumeration" ] = EnumerationFunction;
        functors[ "enumeration" ] = EnumerationFunction;
        functors[ "EquipmentType" ] = EquipmentTypeFunction;
        functors[ "GDH" ] = GDHFunction;
        functors[ "Heading" ] = DirectionFunction;
        functors[ "IndirectFire" ] = IndirectFireFunction;
        functors[ "Integer" ] = NumericFunction;
        functors[ "Itinerary" ] = PathFunction;
        functors[ "Location" ] = LocationFunction;
        functors[ "LocationComposite" ] = LocationCompositeFunction;
        functors[ "LocationCompositeList" ] = LocationCompositeListFunction;
        functors[ "LocationList" ] = LocationListFunction;
        functors[ "MaintenancePriorities" ] = MaintenancePrioritiesFunction;
        functors[ "MedicalPriorities" ] = MedicalPrioritiesFunction;
        functors[ "NatureAtlas" ] = NatureAtlasFunction;
        functors[ "Numeric" ] = NumericFunction;
        functors[ "NumericList" ] = NumericListFunction;
        functors[ "ObjectiveList" ] = ObjectiveListFunction;
        functors[ "ObjectKnowledge" ] = ObjectKnowledgeFunction;
        functors[ "ObjectKnowledgeList" ] = ObjectKnowledgeListFunction;
        functors[ "Path" ] = PathFunction;
        functors[ "PathList" ] = PathListFunction;
        functors[ "PlannedWork" ] = GenObjectFunction;
        functors[ "PlannedWorkList" ] = GenObjectListFunction;
        functors[ "Point" ] = PointFunction;
        functors[ "PointList" ] = PointListFunction;
        functors[ "Polygon" ] = PolygonFunction;
        functors[ "PolygonList" ] = PolygonListFunction;
        functors[ "PopulationKnowledge" ] = PopulationKnowledgeFunction;
        functors[ "ResourceType" ] = DotationTypeFunction;
        functors[ "String" ] = StringFunction;
        functors[ "string" ] = StringFunction;

        functorsBM[ "Agent" ] = AgentFunctionBM;
        functorsBM[ "AgentKnowledge" ] = AgentKnowledgeFunctionBM;
        functorsBM[ "AgentKnowledgeList" ] = AgentKnowledgeListFunctionBM;
        functorsBM[ "AgentList" ] = AgentListFunctionBM;
        functorsBM[ "Automat" ] = AutomatFunctionBM;
        functorsBM[ "AutomatList" ] = AutomatListFunctionBM;
        functorsBM[ "Boolean" ] = BoolFunctionBM;
        functorsBM[ "CrowdKnowledge" ] = PopulationKnowledgeFunctionBM;
        functorsBM[ "DateTime" ] = GDHFunctionBM;
        functorsBM[ "datetime" ] = GDHFunctionBM;
        functorsBM[ "Enumeration" ] = EnumerationFunctionBM;
        functorsBM[ "enumeration" ] = EnumerationFunctionBM;
        functorsBM[ "EquipmentType" ] = EquipmentTypeFunctionBM;
        functorsBM[ "EquipmentTypeList" ] = EquipmentTypeListFunctionBM;
        functorsBM[ "GDH" ] = GDHFunctionBM;
        functorsBM[ "Heading" ] = DirectionFunctionBM;
        functorsBM[ "Integer" ] = NumericFunctionBM;
        functorsBM[ "IntegerList" ] = NumericListFunctionBM;
        functorsBM[ "Itinerary" ] = PathFunctionBM;
        functorsBM[ "LocationComposite" ] = LocationCompositeFunctionBM;
        functorsBM[ "LocationCompositeList" ] = LocationCompositeListFunctionBM;
        functorsBM[ "MaintenancePriorities" ] = MaintenancePrioritiesFunctionBM;
        functorsBM[ "MedicalPriorities" ] = MedicalPrioritiesFunctionBM;
        functorsBM[ "NatureAtlas" ] = NatureAtlasFunctionBM;
        functorsBM[ "Numeric" ] = NumericFunctionBM;
        functorsBM[ "NumericList" ] = NumericListFunctionBM;
        functorsBM[ "ObjectKnowledge" ] = ObjectKnowledgeFunctionBM;
        functorsBM[ "ObjectKnowledgeList" ] = ObjectKnowledgeListFunctionBM;
        functorsBM[ "Path" ] = PathFunctionBM;
        functorsBM[ "PathList" ] = PathListFunctionBM;
        functorsBM[ "PhaseLine" ] = PhaseLineFunctionBM;
        functorsBM[ "PhaseLineList" ] = PhaseLineListFunctionBM;
        functorsBM[ "PlannedWork" ] = GenObjectFunctionBM;
        functorsBM[ "PlannedWorkList" ] = GenObjectListFunctionBM;
        functorsBM[ "Point" ] = PointFunctionBM;
        functorsBM[ "PointList" ] = PointListFunctionBM;
        functorsBM[ "Polygon" ] = AreaFunctionBM;
        functorsBM[ "PolygonList" ] = AreaListFunctionBM;
        functorsBM[ "PopulationKnowledge" ] = PopulationKnowledgeFunctionBM;
        functorsBM[ "ResourceNetworkNode" ] = ResourceNetworkNodeFunctionBM;
        functorsBM[ "ResourceNetworkNodeList" ] = ResourceNetworkNodeListFunctionBM;
        functorsBM[ "ResourceNetworkType" ] = ResourceNetworkTypeFunctionBM;
        functorsBM[ "ResourceNetworkTypeList" ] = ResourceNetworkTypeListFunctionBM;
        functorsBM[ "ResourceType" ] = DotationTypeFunctionBM;
        functorsBM[ "ResourceTypeList" ] = DotationTypeListFunctionBM;
        functorsBM[ "String" ] = StringFunctionBM;
        functorsBM[ "string" ] = StringFunctionBM;
        functorsBM[ "UrbanKnowledge" ] = UrbanBlockFunctionBM;
        functorsBM[ "UrbanKnowledgeList" ] = UrbanBlockListFunctionBM;
    }
}

class RegisterMissionParameterVisitor : public MIL_MissionParameterVisitor_ABC
{
public:
    RegisterMissionParameterVisitor( sword::Brain& brain, const directia::tools::binders::ScriptRef& refMission, directia::tools::binders::ScriptRef& knowledgeCreateFunction )
        : brain_( brain )
        , refMission_( refMission )
        , knowledgeCreateFunction_( knowledgeCreateFunction )
    {}
    virtual ~RegisterMissionParameterVisitor()
    {}
    virtual void Accept( const std::string& dianame, const MIL_OrderTypeParameter& type, MIL_MissionParameter_ABC& element )
    {
        std::string typeName = type.GetType().GetName();
        if( type.IsList() )
            typeName += "List";
        std::map< std::string, T_Function >::iterator itFind = functors.find( typeName );
        if( itFind != functors.end() )
            functors[ typeName ]( refMission_, dianame, element );
    }

private:
     sword::Brain& brain_;
     const directia::tools::binders::ScriptRef& refMission_;
     directia::tools::binders::ScriptRef& knowledgeCreateFunction_;
};

class RegisterMissionParameterBMVisitor : public MIL_MissionParameterVisitor_ABC
{
public:
    RegisterMissionParameterBMVisitor( sword::Brain& brain, const directia::tools::binders::ScriptRef& refMission, directia::tools::binders::ScriptRef& knowledgeCreateFunction )
        : brain_( brain )
        , refMission_( refMission )
        , knowledgeCreateFunction_( knowledgeCreateFunction )
    {}
    virtual ~RegisterMissionParameterBMVisitor()
    {}
    virtual void Accept( const std::string& dianame, const MIL_OrderTypeParameter& type, MIL_MissionParameter_ABC& element )
    {
        std::string typeName = type.GetType().GetName();
        if( type.IsList() )
            typeName += "List";
        std::map< std::string, T_FunctionBM >::iterator itFindBM = functorsBM.find( typeName );
        if( !!knowledgeCreateFunction_ && itFindBM != functorsBM.end() )
        {
            bool isInitialized = functorsBM[ typeName ]( brain_, knowledgeCreateFunction_, refMission_, dianame, element );
            if ( !isInitialized )
                refMission_[ dianame ] = brain_.GetScriptRef( "NIL" );
        }
    }

private:
     sword::Brain& brain_;
     const directia::tools::binders::ScriptRef& refMission_;
     directia::tools::binders::ScriptRef& knowledgeCreateFunction_;
};

// -----------------------------------------------------------------------------
// Name: RegisterMissionParameters
// Created: LDC 2009-05-04
// -----------------------------------------------------------------------------
void RegisterMissionParameters( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const boost::shared_ptr< MIL_Mission_ABC > mission, bool isMasalife )
{
    InitFunctions();
    if( isMasalife )
    {
        RegisterMissionParameterBMVisitor visitor( brain, refMission, knowledgeCreateFunction );
        mission->Visit( visitor );
    }
    else
    {
        RegisterMissionParameterVisitor visitor( brain, refMission, knowledgeCreateFunction );
        mission->Visit( visitor );
    }
}

#ifndef PLATFORM
#error PLATFORM must be defined (for instance vc80 or vc100_x64) for masalife and directia plugins
#endif

#ifndef PLUGIN
#ifdef NDEBUG
#define PLUGIN46( plugin ) std::string( "'plugin_" plugin "_" BOOST_PP_STRINGIZE( PLATFORM ) "-mt-4_6.plugin'," )
#define PLUGIN( plugin ) std::string( "'plugin_" plugin "_" BOOST_PP_STRINGIZE( PLATFORM ) "-mt.plugin'," )
#else
#define PLUGIN46( plugin ) std::string( "'plugin_" plugin "_" BOOST_PP_STRINGIZE( PLATFORM ) "-mt-gd-4_6.plugin'," )
#define PLUGIN( plugin ) std::string( "'plugin_" plugin "_" BOOST_PP_STRINGIZE( PLATFORM ) "-mt-gd.plugin'," )
#endif
#endif

namespace
{
    std::map< std::string, boost::shared_ptr< sword::Brain > > brainTable;

    void LoadResourcesFile( const std::string& file, const tools::Path& dir, sword::Brain& brain )
    {
        brain.GetScriptRef( "include" )( ( dir / tools::Path( file.c_str() ) ).ToUTF8() );
    }
}

bool CreateBrain( boost::shared_ptr< sword::Brain >& pArchetypeBrain, boost::shared_ptr< sword::Brain >& pBrain,
                  const tools::Path& includePath, const tools::Path& brainFile, bool isMasalife,
                  const std::string& type, bool reload, const tools::Path& integrationDir, sword::DEC_Logger* logger )
{
    // If this is not a reloading (i.e., 'reload' is false), we create a brain that shares an archetype brain.
    // The archetypes are saved in 'brainTable' for later reuse. We create an archetype brain if there is not
    // already an archetype in the table.
    // If this is a reloading, we do not reuse an existing archetype, but create a new archetype brain and return
    // this archetype as the brain to use.
    // Each time we create a new archetype, we return true, to tell the caller that the archetype must be initialized.
    const std::string& idx = brainFile.ToUTF8();
    if( !reload )
    {
        pArchetypeBrain = brainTable[idx];
        if( pArchetypeBrain )
        {
            pBrain.reset( new sword::Brain( *pArchetypeBrain, logger ) );
            if( isMasalife )
                pBrain->GetScriptRef( "masalife.brain.core.setRole" )( type );
            return false;
        }
    }
    if( isMasalife )
        pArchetypeBrain.reset( new sword::Brain(
            "plugins={"
            + PLUGIN( "masalife_brain" )
            + PLUGIN( "communication" )
            + PLUGIN46( "errorhandler" )
            + PLUGIN46( "devtools" )
            + "} cwd='" + includePath.ToUTF8() + "'", logger ) );
    else
        pArchetypeBrain.reset( new sword::Brain(
            "plugins={"
            + PLUGIN46( "eventmanager" )
            + PLUGIN46( "motivation" )
            + PLUGIN46( "errorhandler" )
            + PLUGIN46( "devtools" )
            + "} cwd='" + includePath.ToUTF8() + "'", logger ) );
    pArchetypeBrain->RegisterFunction( "LoadResourcesFile", std::function< void( const std::string& ) >(
        boost::bind( &LoadResourcesFile, _1, integrationDir, boost::ref( *pArchetypeBrain ) ) ) );
    pArchetypeBrain->GetScriptRef( "include" )( brainFile, includePath, type );
    if( !reload )
    {
        brainTable[idx] = pArchetypeBrain;
        pBrain.reset( new sword::Brain( *pArchetypeBrain, logger ) );
    }
    else
        pBrain = pArchetypeBrain;
    if( isMasalife )
        pBrain->GetScriptRef( "masalife.brain.core.setRole" )( type );
    return true;
}

}
