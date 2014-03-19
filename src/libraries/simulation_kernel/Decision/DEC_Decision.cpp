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
#include "Decision/DEC_GeometryFunctions.h"
#include "Decision/DEC_KnowledgeAgentFunctions.h"
#include "Decision/DEC_KnowledgeFunctions.h"
#include "Decision/DEC_KnowledgeObjectFunctions.h"
#include "Decision/DEC_KnowledgePopulationFunctions.h"
#include "Decision/DEC_LogisticFunctions.h"
#include "Decision/DEC_MiscFunctions.h"
#include "Decision/DEC_ObjectFunctions.h"
#include "Decision/DEC_OrdersFunctions.h"
#include "Decision/DEC_PathFunctions.h"
#include "Decision/DEC_ResourceNetworkFunctions.h"
#include "Decision/DEC_TelepathyFunctions.h"
#include "Decision/DEC_Gen_Object.h"
#include "Decision/DEC_PathPoint.h"
#include "Decision/DEC_UrbanObjectFunctions.h"
#include "Decision/DEC_TerrainFunctions.h"
#include "Decision/DEC_Logger.h"
#include "DEC_ResourceNetwork.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_FragOrder.h"
#include "Entities/Orders/MIL_MissionParameterFactory.h"
#include "Entities/Orders/MIL_MissionParameterVisitor_ABC.h"
#include "Entities/Orders/MIL_OrderTypeParameter.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
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
void RegisterUnitFunctions( sword::Brain& brain)
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
    brain.RegisterFunction( "DEC_Connaissances_UnitesPrenantAPartieUnAmi", &DEC_KnowledgeFunctions::GetEnemiesAttacking );
    brain.RegisterFunction( "DEC_Connaissances_UniteLaPlusProcheDunAmi", &DEC_KnowledgeFunctions::GetNearestToFriend );
    brain.RegisterFunction( "DEC_Connaissances_UnitesEnnemiesDansZone", &DEC_KnowledgeFunctions::GetEnemyAgentsInZone );

    brain.RegisterFunction( "DEC_Agent_Renforts", &DEC_MiscFunctions::GetAgentReinforcements );
    brain.RegisterFunction( "DEC_Agent_NombreRenforts", &DEC_MiscFunctions::GetAgentReinforcementsNumber );
    brain.RegisterFunction( "DEC_Pion_PionPCDeAutomate", &DEC_AgentFunctions::GetPionPCOfAutomate );
    brain.RegisterFunction( "DEC_Pion_PionsDeAutomateSansPC", &DEC_AgentFunctions::GetPionsWithoutPCOfAutomate );
    brain.RegisterFunction( "DEC_GetAutomate", &DEC_MiscFunctions::GetAutomate );
    brain.RegisterFunction( "DEC_Pion_ChangeAutomate", &DEC_AutomateFunctions::PionChangeAutomate );
    brain.RegisterFunction( "DEC_Automate_PionPosition", &DEC_AutomateFunctions::GetPionPosition );
    brain.RegisterFunction( "DEC_Automate_PerceptionPourPion", &DEC_AutomateFunctions::GetPerceptionForPion );
    brain.RegisterFunction( "DEC_Automate_CalculerPositionParRapportALimaPourPion", &DEC_AutomateFunctions::ComputePointBeforeLimaForPion );
    brain.RegisterFunction( "DEC_Automate_PionTempsPourParcourirDistanceEnLigneDroite", &DEC_AutomateFunctions::PionTimeToMoveDistance );
    brain.RegisterFunction( "DEC_GetRawMission", &DEC_AutomateFunctions::GetMission );
    brain.RegisterFunction( "DEC_SetMission", &DEC_AutomateFunctions::SetMission );
    brain.RegisterFunction( "DEC_IsMissionPionAvailable", &DEC_OrdersFunctions::IsPionMissionAvailable );
    brain.RegisterFunction( "DEC_IsAutomateMissionPionAvailable", &DEC_OrdersFunctions::IsAutomateMissionAvailable );
    brain.RegisterFunction( "DEC_Pion_GetMilPionType", &DEC_AgentFunctions::GetMilPionType );
    brain.RegisterFunction( "DEC_Pion_GetMilPionName", &DEC_AgentFunctions::GetMilPionName );
    brain.RegisterFunction( "DEC_Agent_EstImmobilise", &DEC_AgentFunctions::IsImmobilized );
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
    brain.RegisterFunction( "DEC_Agent_PionCanFly", boost::function< bool( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::PionCanFly, _1 ) ) );
    brain.RegisterFunction( "DEC_Agent_GetStandardFlyingHeight", &DEC_AgentFunctions::GetStandardFlyingHeight );
    brain.RegisterFunction( "DEC_Agent_GetTacticalFlyingHeight", &DEC_AgentFunctions::GetTacticalFlyingHeight );
    brain.RegisterFunction( "DEC_Agent_MaxSpeed", &DEC_AgentFunctions::GetMaxSpeed );
    brain.RegisterFunction( "DEC_GetUnitById", &DEC_AgentFunctions::GetUnitById );
    brain.RegisterFunction( "DEC_Agent_AgentPeutTransporterPion", &DEC_ActionFunctions::AgentCanTransportPion );
    brain.RegisterFunction( "DEC_Agent_IsUnderground", &DEC_AgentFunctions::IsUnderground );
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

    // Inhabitants => je ne sais pas trop comment nommer ou classer cette méthode : le terme "population" fait parfois référence aux foules, parfois aux populations...
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
    brain.RegisterFunction( "DEC_GetSzName", &DEC_MiscFunctions::GetName );
    brain.RegisterMethod( "DEC_AgentKnowledge_Relevance", &DEC_Knowledge_Agent::GetRelevance );
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
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterActionFunctions
// Created: LDC 2013-06-14
// -----------------------------------------------------------------------------
void RegisterActionFunctions( sword::Brain& brain )
{
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
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterGeometryFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterGeometryFunctions( sword::Brain& brain)
{
    brain.RegisterFunction( "DEC_Automate_Position", &DEC_AutomateFunctions::GetBarycenter );
    brain.RegisterFunction( "DEC_Agent_PositionPtr", &DEC_AgentFunctions::GetAgentPositionPtr );
    brain.RegisterFunction( "DEC_Geometrie_DecouperListePoints", &DEC_GeometryFunctions::SplitListPoints );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionCouverture", &DEC_GeometryFunctions::ComputeCoverPosition );
    brain.RegisterFunction( "DEC_Geometrie_CalculerBarycentreAgents", &DEC_GeometryFunctions::ComputeAgentsBarycenter );
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
    brain.RegisterFunction( "DEC_Geometrie_AreaDiameter", &DEC_GeometryFunctions::ComputeAreaDiameter );
    brain.RegisterFunction( "DEC_Geometrie_DecouperLocalisation", boost::function< std::vector< boost::shared_ptr< MT_Vector2D > >( const TER_Localisation*, unsigned int ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisation, _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_ClipperLocalisation", &DEC_GeometryFunctions::ClipLocalisation );
    brain.RegisterFunction( "DEC_IsPointInCity", &DEC_GeometryFunctions::IsPointInCity );
    brain.RegisterFunction( "DEC_Geometrie_ComputeNearestBorder", &DEC_GeometryFunctions::ComputeNearestBorder );
    brain.RegisterFunction( "DEC_Geometrie_CalculerTrafficablePointPourPoint",
        boost::function< std::vector< boost::shared_ptr< MT_Vector2D > >( const MT_Vector2D& ) >( boost::bind( &DEC_GeometryFunctions::ComputeTrafficableLocalisation, _1 ) ) );
    brain.RegisterFunction( "DEC_IsPointInUrbanBlockTrafficableForPlatoon",
        boost::function< bool( DEC_Decision_ABC*, MT_Vector2D& ) >( boost::bind( &DEC_GeometryFunctions::IsPointInUrbanBlockTrafficableForPlatoon, _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_PositionsParRapportALocalisation",
        boost::function< std::vector< boost::shared_ptr< MT_Vector2D > >( const std::vector< DEC_Decision_ABC* >&, TER_Localisation*, MT_Vector2D*, double ) >( boost::bind( &DEC_GeometryFunctions ::ComputeLocalisationPointsForPionsInFuseau, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointArriveePourFuseau",
        boost::function< boost::shared_ptr< MT_Vector2D >( MIL_Fuseau& ) >( boost::bind( &DEC_GeometryFunctions::ComputeDestPointForFuseau, _1 ) ) );
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
    brain.RegisterFunction( "DEC_Geometrie_PositionAdvanceAlongAOR", &DEC_GeometryFunctions::ComputePositionAdvanceAlongFuseau );
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
    brain.RegisterFunction( "DEC_IsPointInUrbanBlock", &DEC_GeometryFunctions::IsPointInUrbanBlock );
    brain.RegisterFunction( "DEC_UrbanBlocksInZone", &DEC_UrbanObjectFunctions::GetUrbanBlockInZone );
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
    brain.RegisterFunction( "DEC_NecessiteEvacuationBlesses", &DEC_LogisticFunctions::HasWoundedHumansToEvacuate );
    brain.RegisterFunction( "DEC_EvacuerBlessesVersTC2", &DEC_LogisticFunctions::EvacuateWoundedHumansToTC2 );
    brain.RegisterFunction( "DEC_InterdireEvacuationAutomatiqueBlesses", &DEC_LogisticFunctions::ForbidWoundedHumansAutoEvacuation );
    brain.RegisterFunction( "DEC_AutoriserEvacuationAutomatiqueBlesses", &DEC_LogisticFunctions::AllowWoundedHumansAutoEvacuation );
    brain.RegisterFunction( "DEC_EnforceAviationResources", &DEC_LogisticFunctions::EnforceAviationResources );
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
// Name: DEC_Decision::RegisterObjectsFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterObjectsFunctions( sword::Brain& brain )
{
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
    brain.RegisterMethod( "GetType", &DEC_PathPoint::GetDIAType );
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
        boost::function< void( const directia::tools::binders::ScriptRef&, boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_MiscFunctions::FillMissionParameters, boost::ref(brain), initParameterFunction, _1 , _2, isMasalife ) ) );
    brain.RegisterFunction( "DEC_AssignerFuseauAMissionPion_Mission",
        boost::function< void( MIL_Fuseau* ,  boost::shared_ptr< MIL_Mission_ABC > )>( boost::bind( &DEC_OrdersFunctions::AssignFuseauToPionMission , _1, _2 ) ) );
    brain.RegisterFunction( "DEC_AssignerFuseauAMissionAutomate_Mission",
            boost::function< void( MIL_Fuseau* ,  boost::shared_ptr< MIL_Mission_ABC > )>( boost::bind( &DEC_OrdersFunctions::AssignFuseauToAutomateMission , _1, _2 ) ) );
    brain.RegisterFunction( "DEC_AssignerDirectionAMissionAutomate_Mission",
            boost::function< void (MT_Vector2D* ,  boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_OrdersFunctions::AssignDirectionToAutomateMission , _1, _2 ) ) );
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
        boost::function< void ( DEC_Decision_ABC&, int, const std::string& ) >( boost::bind( &DEC_MiscFunctions::Report, _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_AgentKnowledge",
        boost::function< void ( DEC_Decision_ABC&, int, const std::string&, boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_MiscFunctions::ReportAgentKnowledge, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_DotationType",
        boost::function< void ( DEC_Decision_ABC&, int, const std::string&, const PHY_DotationCategory* ) >( boost::bind( &DEC_MiscFunctions::ReportDotationType, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_EquipmentType",
        boost::function< void ( DEC_Decision_ABC&, int, const std::string&, const PHY_ComposanteTypePion* ) >( boost::bind( &DEC_MiscFunctions::ReportEquipmentType, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_Float",
        boost::function< void ( DEC_Decision_ABC&, int, const std::string&, float ) >( boost::bind( &DEC_MiscFunctions::ReportFloat, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_Int_Int",
        boost::function< void ( DEC_Decision_ABC&, int, const std::string&, int, int ) >( boost::bind( &DEC_MiscFunctions::ReportIntInt, _1, _2, _3, _4, _5 ) ) );
    brain.RegisterFunction( "DEC_RC_Float_Float",
        boost::function< void ( DEC_Decision_ABC&, int, const std::string&, float, float ) >( boost::bind( &DEC_MiscFunctions::ReportFloatFloat, _1, _2, _3, _4, _5 ) ) );
    brain.RegisterFunction( "DEC_RC_Id",
        boost::function< void ( DEC_Decision_ABC&, int, const std::string&, int ) >( boost::bind( &DEC_MiscFunctions::ReportId, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_ObjectKnowledge",
        boost::function< void ( DEC_Decision_ABC&, int, const std::string&, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_MiscFunctions::ReportObjectKnoweldge, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_Pion",
        boost::function< void ( DEC_Decision_ABC&, int, const std::string&, DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::ReportPion, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_Pion_Automate",
        boost::function< void ( DEC_Decision_ABC&, int, const std::string&, DEC_Decision_ABC*, DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::ReportPionAutomate, _1, _2, _3, _4, _5 ) ) );
    brain.RegisterFunction( "DEC_RC_Pion_Pion",
        boost::function< void ( DEC_Decision_ABC&, int, const std::string&, DEC_Decision_ABC*, DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::ReportPionPion, _1, _2, _3, _4, _5 ) ) );
    brain.RegisterFunction( "DEC_RC_PopulationKnowledge",
        boost::function< void ( DEC_Decision_ABC&, int, const std::string&, int ) >( boost::bind( &DEC_MiscFunctions::ReportPopulationKnowledge, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_TirPion",
        boost::function< void ( DEC_Decision_ABC&, int, const std::string&, int ) >( boost::bind( &DEC_MiscFunctions::ReportTirPion, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_String",
        boost::function< void ( DEC_Decision_ABC&, int, const std::string&, const std::string& ) >( boost::bind( &DEC_MiscFunctions::ReportString, _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_Stage",
        boost::function< void ( DEC_Decision_ABC&, int, const std::string&, const std::string& ) >( boost::bind( &DEC_MiscFunctions::ReportStage, _1, _2, _3, _4 ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterTelepathyFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterTelepathyFunctions( sword::Brain& brain )
{
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
    brain.RegisterMethod( "Getpoint_", &DEC_PathPoint::GetPos );
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
}

void RegisterTransportFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_CanLoad", &DEC_ActionFunctions::CanLoad );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterCommonUserFunctions
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
void RegisterCommonUserFunctions( sword::Brain& brain, bool isMasalife )
{
    RegisterUnitFunctions( brain );
    RegisterPopulationFunctions( brain );
    RegisterActionFunctions( brain );
    RegisterAgentKnowledgeFunctions( brain );
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
    RegisterObjectsFunctions( brain );
    RegisterObjectivesFunctions( brain );
    RegisterSpecificPointsFunctions( brain );
    RegisterTypeFunctions( brain );
    RegisterMissionParametersFunctions( brain, isMasalife );
    RegisterObjectFunctions( brain );
    RegisterReportFunctions( brain );
    RegisterTelepathyFunctions( brain );
    RegisterItineraryFunctions( brain );
    RegisterToolsFunctions( brain );
    RegisterTransportFunctions( brain );
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
        functors[ "Boolean" ] = BoolFunction;
        functors[ "String" ] = StringFunction;
        functors[ "string" ] = StringFunction;
        functors[ "Enumeration" ] = EnumerationFunction;
        functors[ "enumeration" ] = EnumerationFunction;
        functors[ "DateTime" ] = GDHFunction;
        functors[ "datetime" ] = GDHFunction;
        functors[ "GDH" ] = GDHFunction;
        functors[ "Point" ] = PointFunction;
        functors[ "PointList" ] = PointListFunction;
        functors[ "Polygon" ] = PolygonFunction;
        functors[ "PolygonList" ] = PolygonListFunction;
        functors[ "Location" ] = LocationFunction;
        functors[ "LocationList" ] = LocationListFunction;
        functors[ "Path" ] = PathFunction;
        functors[ "PathList" ] = PathListFunction;
        functors[ "Heading" ] = DirectionFunction;
        functors[ "NatureAtlas" ] = NatureAtlasFunction;
        functors[ "Automat" ] = AutomatFunction;
        functors[ "AutomatList" ] = AutomatListFunction;
        functors[ "Agent" ] = AgentFunction;
        functors[ "AgentList" ] = AgentListFunction;
        functors[ "AgentKnowledge" ] = AgentKnowledgeFunction;
        functors[ "AgentKnowledgeList" ] = AgentKnowledgeListFunction;
        functors[ "ObjectKnowledge" ] = ObjectKnowledgeFunction;
        functors[ "ObjectKnowledgeList" ] = ObjectKnowledgeListFunction;
        functors[ "PopulationKnowledge" ] = PopulationKnowledgeFunction;
        functors[ "CrowdKnowledge" ] = PopulationKnowledgeFunction;
        functors[ "ResourceType" ] = DotationTypeFunction;
        functors[ "EquipmentType" ] = EquipmentTypeFunction;
        functors[ "Numeric" ] = NumericFunction;
        functors[ "NumericList" ] = NumericListFunction;
        functors[ "Integer" ] = NumericFunction;
        functors[ "NumericList" ] = NumericListFunction;
        functors[ "PlannedWork" ] = GenObjectFunction;
        functors[ "PlannedWorkList" ] = GenObjectListFunction;
        functors[ "MaintenancePriorities" ] = MaintenancePrioritiesFunction;
        functors[ "MedicalPriorities" ] = MedicalPrioritiesFunction;
        functors[ "IndirectFire" ] = IndirectFireFunction;
        functors[ "ObjectiveList" ] = ObjectiveListFunction;
        functors[ "LocationComposite" ] = LocationCompositeFunction;
        functors[ "LocationCompositeList" ] = LocationCompositeListFunction;

        functorsBM[ "Boolean" ] = BoolFunctionBM;
        functorsBM[ "String" ] = StringFunctionBM;
        functorsBM[ "string" ] = StringFunctionBM;
        functorsBM[ "Enumeration" ] = EnumerationFunctionBM;
        functorsBM[ "enumeration" ] = EnumerationFunctionBM;
        functorsBM[ "DateTime" ] = GDHFunctionBM;
        functorsBM[ "datetime" ] = GDHFunctionBM;
        functorsBM[ "GDH" ] = GDHFunctionBM;
        functorsBM[ "NatureAtlas" ] = NatureAtlasFunctionBM;
        functorsBM[ "Numeric" ] = NumericFunctionBM;
        functorsBM[ "NumericList" ] = NumericListFunctionBM;
        functorsBM[ "Integer" ] = NumericFunctionBM;
        functorsBM[ "IntegerList" ] = NumericListFunctionBM;
        functorsBM[ "Point" ] = PointFunctionBM;
        functorsBM[ "PointList" ] = PointListFunctionBM;
        functorsBM[ "Polygon" ] = AreaFunctionBM;
        functorsBM[ "PolygonList" ] = AreaListFunctionBM;
        functorsBM[ "Path" ] = PathFunctionBM;
        functorsBM[ "PathList" ] = PathListFunctionBM;
        functorsBM[ "Heading" ] = DirectionFunctionBM;
        functorsBM[ "Automat" ] = AutomatFunctionBM;
        functorsBM[ "AutomatList" ] = AutomatListFunctionBM;
        functorsBM[ "Agent" ] = AgentFunctionBM;
        functorsBM[ "AgentList" ] = AgentListFunctionBM;
        functorsBM[ "AgentKnowledge" ] = AgentKnowledgeFunctionBM;
        functorsBM[ "AgentKnowledgeList" ] = AgentKnowledgeListFunctionBM;
        functorsBM[ "ObjectKnowledge" ] = ObjectKnowledgeFunctionBM;
        functorsBM[ "ObjectKnowledgeList" ] = ObjectKnowledgeListFunctionBM;
        functorsBM[ "PopulationKnowledge" ] = PopulationKnowledgeFunctionBM;
        functorsBM[ "CrowdKnowledge" ] = PopulationKnowledgeFunctionBM;
        functorsBM[ "PlannedWork" ] = GenObjectFunctionBM;
        functorsBM[ "MaintenancePriorities" ] = MaintenancePrioritiesFunctionBM;
        functorsBM[ "MedicalPriorities" ] = MedicalPrioritiesFunctionBM;
        functorsBM[ "PlannedWorkList" ] = GenObjectListFunctionBM;
        functorsBM[ "UrbanKnowledge" ] = UrbanBlockFunctionBM;
        functorsBM[ "UrbanKnowledgeList" ] = UrbanBlockListFunctionBM;
        functorsBM[ "LocationComposite" ] = LocationCompositeFunctionBM;
        functorsBM[ "LocationCompositeList" ] = LocationCompositeListFunctionBM;
        functorsBM[ "PhaseLine" ] = PhaseLineFunctionBM;
        functorsBM[ "PhaseLineList" ] = PhaseLineListFunctionBM;
        functorsBM[ "ResourceType" ] = DotationTypeFunctionBM;
        functorsBM[ "ResourceTypeList" ] = DotationTypeListFunctionBM;
        functorsBM[ "ResourceNetworkNode" ] = ResourceNetworkNodeFunctionBM;
        functorsBM[ "ResourceNetworkNodeList" ] = ResourceNetworkNodeListFunctionBM;
        functorsBM[ "ResourceNetworkType" ] = ResourceNetworkTypeFunctionBM;
        functorsBM[ "ResourceNetworkTypeList" ] = ResourceNetworkTypeListFunctionBM;
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
    const std::string& idx = isMasalife ? type : brainFile.ToUTF8();
    pArchetypeBrain = brainTable[idx];
    if( reload )
        pArchetypeBrain.reset();
    if( pArchetypeBrain )
    {
        pBrain.reset( new sword::Brain( *pArchetypeBrain, logger ) );
        return false;
    }
    if( isMasalife )
        pArchetypeBrain.reset( new sword::Brain(
            "plugins={"
            + PLUGIN( "masalife_brain" )
            + PLUGIN( "knowledge" )
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
    pArchetypeBrain->RegisterFunction( "LoadResourcesFile", boost::function< void( const std::string& ) >(
        boost::bind( &LoadResourcesFile, _1, integrationDir, boost::ref( *pArchetypeBrain ) ) ) );
    pArchetypeBrain->GetScriptRef( "include" )( brainFile, includePath, type );
    if( !reload )
        brainTable[idx] = pArchetypeBrain;
    if( reload )
        pBrain = pArchetypeBrain;
    else
        pBrain.reset( new sword::Brain( *pArchetypeBrain, logger ) );
    return true;
}

}
