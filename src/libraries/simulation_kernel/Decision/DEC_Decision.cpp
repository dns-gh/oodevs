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
#include "Decision/DEC_AutomateFunctions.h"
#include "Decision/DEC_AgentFunctions.h"
#include "Decision/DEC_CommunicationFunctions.h"
#include "Decision/DEC_DIAFunctions.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Decision/DEC_KnowledgeAgentFunctions.h"
#include "Decision/DEC_KnowledgeFunctions.h"
#include "Decision/DEC_KnowledgeObjectFunctions.h"
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
#include "DEC_ResourceNetwork.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_FragOrder.h"
#include "Entities/Orders/MIL_MissionParameterFactory.h"
#include "Entities/Orders/MIL_MissionParameterVisitor_ABC.h"
#include "Entities/Orders/MIL_OrderTypeParameter.h"
#include "Knowledge/DEC_Knowledge_Population.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include <geometry/Point2.h>
#include <boost/bind.hpp>
#include <boost/filesystem.hpp>
#include <directia/tools/binders/ScriptRef.h>

namespace bfs = boost::filesystem;

// -----------------------------------------------------------------------------
// Name: ScriptRefs
// Created: LDC 2009-04-08
// -----------------------------------------------------------------------------
ScriptRefs::ScriptRefs( directia::brain::Brain& brain  )
    : sendEvent_               ( brain[ "SendEvent" ] )
    , startEvent_              ( brain[ "StartEvent" ] )
    , stopEvents_              ( brain[ "StopEvents" ] )
    , setStateVariable_        ( brain[ "SetStateVariable" ] )
    , collectgarbage_          ( brain[ "collectgarbage" ] )
    , step_                    ( brain[ "step" ] )
    , callbackPerception_      ( brain[ "CallbackPerception" ] )
    , knowledgeCallbackAction_ ( brain[ "KnowledgeCallbackAction" ] )
    , removeAction_            ( brain[ "RemoveAction" ] )
    , initTaskParameter_       ( brain[ "InitTaskParameter" ] )
    , cleanBrainBeforeDeletion_( brain[ "CleanBrainBeforeDeletion" ] )
{
    // NOTHING
}

namespace DEC_DecisionImpl
{
// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterUnitFunctions
// Created: LMT 2010-12-02
// -----------------------------------------------------------------------------
void RegisterUnitFunctions( directia::brain::Brain& brain)
{
    brain.Register( "DEC_Agent_EtatOpsMajeur", &DEC_Decision_ABC::GetMajorOperationalState );
    brain.Register( "DEC_Agent_EstPC", &DEC_Decision_ABC::IsPC );
    brain.Register( "DEC_Agent_EstTransporte", &DEC_Decision_ABC::IsTransported );
    brain.Register( "DEC_Agent_EstEnVol", &DEC_Decision_ABC::IsFlying );
    brain.Register( "DEC_Agent_EstEnMouvement", &DEC_Decision_ABC::IsMoving );
    brain.Register( "DEC_Agent_EstNeutralise", &DEC_Decision_ABC::IsNeutralized );
    brain.Register( "DEC_Agent_AutomateEstEmbraye", &DEC_Decision_ABC::IsAutomateEngaged );
    brain.Register( "DEC_Agent_Position", &DEC_Decision_ABC::GetPosition );
    brain.Register( "DEC_Agent_EstMort", &DEC_Decision_ABC::IsDead );
    brain.Register( "DEC_Agent_EstContamine", &DEC_Decision_ABC::IsContaminated );
    brain.Register( "DEC_Agent_EstBrouille", &DEC_Decision_ABC::IsJammed );
    brain.Register( "DEC_Agent_EstEnSilenceRadioEmission", &DEC_Decision_ABC::IsInEmissionBlackout );
    brain.Register( "DEC_Agent_EstEnSilenceRadioReception", &DEC_Decision_ABC::IsInReceptionBlackout );
    brain.Register( "DEC_Pion_PionsAvecPC", &DEC_Decision_ABC::GetPionsWithPC );
    brain.Register( "DEC_Pion_PionsAvecPCCommunication", &DEC_Decision_ABC::GetCommunicationPionsWithPC );
    brain.Register( "DEC_Automate_PionsAvecPC", &DEC_Decision_ABC::GetPionsWithPC );
    brain.Register( "DEC_Automate_PionsAvecPCCommunication", &DEC_Decision_ABC::GetCommunicationPionsWithPC );
    brain[ "DEC_Automate_PionsDeAutomateSansPC" ] = &DEC_AutomateFunctions::GetPionsOfAutomateWithoutPC;
    brain[ "DEC_Automate_PionsDeAutomateAvecPC" ] = &DEC_AutomateFunctions::GetAutomatPionsWithPC;
    brain[ "DEC_Automate_PionsDeAutomateSansPCCommunication" ] = &DEC_AutomateFunctions::GetCommunicationPionsOfAutomateWithoutPC;
    brain[ "DEC_Automate_PionsDeAutomateAvecPCCommunication" ] = &DEC_AutomateFunctions::GetCommunicationAutomatPionsWithPC;
    brain.Register( "DEC_Automate_EstEmbraye", &DEC_Decision_ABC::IsAutomateEngaged );
    brain[ "DEC_Connaissances_UnitesPrenantAPartieUnAmi" ] = &DEC_KnowledgeFunctions::GetEnemiesAttacking;
    brain[ "DEC_Connaissances_UniteLaPlusProcheDunAmi" ] = &DEC_KnowledgeFunctions::GetNearestToFriend;
    brain[ "DEC_Connaissances_UnitesEnnemiesDansZone" ] = &DEC_KnowledgeFunctions::GetEnemyAgentsInZone;
    
    brain[ "DEC_Agent_Renforts" ] = &DEC_MiscFunctions::GetAgentReinforcements;
    brain[ "DEC_Agent_NombreRenforts" ] = &DEC_MiscFunctions::GetAgentReinforcementsNumber;
    brain[ "DEC_Pion_PionPCDeAutomate" ] = &DEC_AgentFunctions::GetPionPCOfAutomate;
    brain[ "DEC_Pion_PionsDeAutomateSansPC" ] = &DEC_AgentFunctions::GetPionsWithoutPCOfAutomate;
    brain[ "DEC_GetAutomate" ] = &DEC_MiscFunctions::GetAutomate;
    brain[ "DEC_Pion_ChangeAutomate" ] = &DEC_AutomateFunctions::PionChangeAutomate;
    brain[ "DEC_Automate_PionPosition" ] = &DEC_AutomateFunctions::GetPionPosition;
    brain[ "DEC_Automate_PerceptionPourPion" ] = &DEC_AutomateFunctions::GetPerceptionForPion;
    brain[ "DEC_Automate_CalculerPositionParRapportALimaPourPion" ] = &DEC_AutomateFunctions::ComputePointBeforeLimaForPion;
    brain[ "DEC_Automate_PionTempsPourParcourirDistanceEnLigneDroite" ] = &DEC_AutomateFunctions::PionTimeToMoveDistance;
    brain[ "DEC_GetRawMission" ] = &DEC_AutomateFunctions::GetMission;
    brain[ "DEC_SetMission" ] = &DEC_AutomateFunctions::SetMission;
    brain[ "DEC_IsMissionPionAvailable" ] = &DEC_OrdersFunctions::IsPionMissionAvailable;
    brain[ "DEC_IsAutomateMissionPionAvailable" ] = &DEC_OrdersFunctions::IsAutomateMissionAvailable;
    brain[ "DEC_Pion_GetMilPionType" ] = &DEC_AgentFunctions::GetMilPionType;
    brain[ "DEC_Pion_GetMilPionName" ] = &DEC_AgentFunctions::GetMilPionName;
    brain[ "DEC_Agent_EstImmobilise" ] = &DEC_AgentFunctions::IsImmobilized;
    brain[ "DEC_Agent_CanMoveOn" ] =
        boost::function< bool( const DEC_Decision_ABC*, boost::shared_ptr< MT_Vector2D > )>( boost::bind( &DEC_TerrainFunctions::CanMoveOn, _1, _2 ) );
    
    brain[ "DEC_Agent_ForcerImmunisationNbc" ] = &DEC_AgentFunctions::TemporaryImmunizeAgent;
}

// -----------------------------------------------------------------------------
// Name: RegisterPopulationFunctions
// Created: LDC 2011-05-05
// -----------------------------------------------------------------------------
void RegisterPopulationFunctions( directia::brain::Brain& brain )
{
    // Population
    brain[ "DEC_Agent_CanaliserPopulation" ] = &DEC_AgentFunctions::ChannelPopulations;

    // Inhabitants => je ne sais pas trop comment nommer ou classer cette méthode : le terme "population" fait parfois référence aux foules, parfois aux populations...
    brain[ "DEC_Agent_Alert" ] = &DEC_AgentFunctions::AlertInhabitants;
    brain[ "DEC_Agent_IsAlerted" ] = &DEC_AgentFunctions::IsInhabitantsAlerted;
    brain[ "DEC_Agent_Confine" ] = &DEC_AgentFunctions::ConfineInhabitants;
}

// -----------------------------------------------------------------------------
// Name: RegisterAgentKnowledgeFunctions
// Created: LDC 2011-05-05
// -----------------------------------------------------------------------------
void RegisterAgentKnowledgeFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_ConnaissanceAgent_EtatOps" ] = &DEC_KnowledgeAgentFunctions::GetOperationalState;
    brain[ "DEC_ConnaissanceAgent_EstDetruitTactique" ] = &DEC_KnowledgeAgentFunctions::GetMajorOperationalState;
    brain[ "DEC_ConnaissanceAgent_Position" ] = &DEC_KnowledgeAgentFunctions::GetPositionPtr;
    brain[ "DEC_ConnaissanceAgent_EstEnVol" ] = &DEC_KnowledgeAgentFunctions::IsFlying;
    brain[ "DEC_ConnaissanceAgent_Altitude" ] = &DEC_KnowledgeAgentFunctions::DEC_ConnaissanceAgent_Altitude;
    brain[ "DEC_ConnaissanceAgent_NatureAtlas" ] = &DEC_KnowledgeAgentFunctions::GetNatureAtlas;
    brain[ "DEC_ConnaissanceAgent_NiveauPerceptionMax" ] = &DEC_KnowledgeAgentFunctions::GetMaxPerceptionLevelForKnowledgeGroup;
    brain[ "DEC_ConnaissanceAgent_DangerositeSurPion" ] = &DEC_KnowledgeAgentFunctions::GetDangerosityOnPion;
    brain[ "DEC_ConnaissanceAgent_DangerositeSurConnaissance" ] = &DEC_KnowledgeAgentFunctions::GetDangerosityOnKnowledge;
    brain[ "DEC_ConnaissanceAgent_EstValide" ] = &DEC_KnowledgeAgentFunctions::IsKnowledgeValid;
    brain[ "DEC_ConnaissanceAgent_EstEnMouvement" ] = &DEC_KnowledgeAgentFunctions::IsMoving;
    brain[ "DEC_ConnaissanceAgent_SEstRendu" ] = &DEC_KnowledgeAgentFunctions::IsSurrendered;
    brain[ "DEC_ConnaissanceAgent_EstPrisonnier" ] = &DEC_KnowledgeAgentFunctions::IsPrisoner;
    brain[ "DEC_ConnaissanceAgent_EstRenduAMonCamp" ] = &DEC_KnowledgeAgentFunctions::IsSurrenderToMyArmy;
    brain[ "DEC_ConnaissanceAgent_EstRefugie" ] = &DEC_KnowledgeAgentFunctions::IsRefugee;
    brain[ "DEC_ConnaissanceAgent_EstMort" ] = &DEC_KnowledgeAgentFunctions::IsDead;
    brain[ "DEC_ConnaissanceAgent_Verrouiller" ] = &DEC_KnowledgeAgentFunctions::Lock;
    brain[ "DEC_ConnaissanceAgent_Deverrouiller" ] = &DEC_KnowledgeAgentFunctions::Unlock;
    brain[ "DEC_ConnaissanceAgent_TuerOfficiers" ] = &DEC_KnowledgeAgentFunctions::KillOfficers;
    brain[ "DEC_Connaissances_UnitesPrenantAPartieSurAmi"  ] = &DEC_KnowledgeFunctions::GetAgentsAttackingAlly;
    brain[ "DEC_ConnaissanceAgent_Verrouiller" ] = &DEC_KnowledgeAgentFunctions::Lock;
    brain[ "DEC_ConnaissanceAgent_Deverrouiller" ] = &DEC_KnowledgeAgentFunctions::Unlock;
    brain[ "DEC_ConnaissanceAgent_GetMilPionType" ] = &DEC_KnowledgeAgentFunctions::GetMilPionType;
    brain[ "DEC_Agent_RapportDeForceLocal" ] = &DEC_AgentFunctions::GetRapForLocalAgent;
    brain[ "DEC_HasMission" ] = &DEC_AgentFunctions::HasMission;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterGeometryFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterGeometryFunctions( directia::brain::Brain& brain)
{
    brain[ "DEC_Automate_Position" ] = &DEC_AutomateFunctions::GetBarycenter;
    brain[ "DEC_Agent_PositionPtr" ] = &DEC_AgentFunctions::GetAgentPositionPtr;
    brain[ "DEC_Geometrie_DecouperListePoints" ] = &DEC_GeometryFunctions::SplitListPoints;
    brain[ "DEC_Geometrie_CalculerPositionCouverture" ] = &DEC_GeometryFunctions::ComputeCoverPosition;
    brain[ "DEC_Geometrie_CalculerBarycentreAgents" ] = &DEC_GeometryFunctions::ComputeAgentsBarycenter;
    brain[ "DEC_Geometrie_PositionTranslate" ] = &DEC_GeometryFunctions::TranslatePosition;
    brain[ "DEC_Geometrie_PositionTranslateDir" ] = &DEC_GeometryFunctions::TranslatePositionInDirection;
    brain[ "DEC_Geometrie_PositionTranslateVector" ] = &DEC_GeometryFunctions::TranslatePositionInVector;
    brain[ "DEC_Geometrie_PositionsEgales" ] = &DEC_GeometryFunctions::ComparePositions;
    brain[ "DEC_Geometrie_Distance" ] = &DEC_GeometryFunctions::Distance;
    brain[ "DEC_Geometrie_Distance3D" ] = &DEC_GeometryFunctions::Distance3D;
    brain[ "DEC_Geometrie_ConvertirPointEnLocalisation" ] = &DEC_GeometryFunctions::ConvertPointToLocalisation;
    brain[ "DEC_Geometrie_EstPointDansLocalisation" ] = &DEC_GeometryFunctions::IsPointInsideLocalisation;
    brain[ "DEC_Geometrie_CreerLocalisation" ] = &DEC_GeometryFunctions::CreateLocalisation;
    brain[ "DEC_Geometrie_CreerListePoints" ] = &DEC_GeometryFunctions::CreateListPoint;
    brain[ "DEC_Geometrie_CreerPoint" ] = &DEC_GeometryFunctions::CreatePoint;
    brain[ "DEC_Geometrie_CopiePoint" ] = &DEC_GeometryFunctions::CopyPoint;
    brain[ "DEC_Geometrie_CreerDirection" ] = &DEC_GeometryFunctions::CreateDirection;
    brain[ "DEC_Geometrie_CreerVecteur" ] = &DEC_GeometryFunctions::CreateVector;
    brain[ "DEC_Geometrie_CreerDirectionPerpendiculaire" ] = &DEC_GeometryFunctions::CreateOrthoDirection;
    brain[ "DEC_Geometrie_InverseDirection" ] = &DEC_GeometryFunctions::ReverseDirection;
    brain[ "DEC_Geometrie_CopieEtInverseDirection" ] = &DEC_GeometryFunctions::CopyAndReverseDirection;
    brain[ "DEC_Geometrie_CopieEtRotateDirection" ] = &DEC_GeometryFunctions::CopyAndRotateDirection;
    brain[ "DEC_Geometrie_CalculerDistanceLigneMoyenne" ] = &DEC_GeometryFunctions::ComputeDistanceFromMiddleLine;
    brain[ "DEC_Geometrie_CalculerBarycentreLocalisation" ] = &DEC_GeometryFunctions::ComputeLocalisationBarycenter;
    brain[ "DEC_Geometrie_ListePointsLocalisation" ] = &DEC_GeometryFunctions::ListLocalisationPoints;
    brain[ "DEC_Geometrie_OffsetItinerary" ] = &DEC_GeometryFunctions::ListLocalisationPoints;
    brain[ "DEC_Geometrie_DirectionMoyenne" ] = &DEC_GeometryFunctions::ComputeMeanDirection;
    brain[ "DEC_Geometrie_PositionAleatoireSurCercle" ] = &DEC_GeometryFunctions::ComputeRandomPointOnCircle;
    brain[ "DEC_Geometrie_PositionAleatoireDansCercle" ] = &DEC_GeometryFunctions::ComputeRandomPointInCircle;
    brain[ "DEC_Geometrie_PositionAleatoireDansZone" ] = &DEC_GeometryFunctions::ComputeRandomPointInZone;
    brain[ "DEC_Geometrie_CreerLocalisationCercle" ] = &DEC_GeometryFunctions::CreateCircleLocalisation;
    brain[ "DEC_Geometrie_CreerLocalisationLigne" ] = &DEC_GeometryFunctions::CreateLineLocalisation;
    brain[ "DEC_Geometrie_CreerLocalisationPolyligne" ] = &DEC_GeometryFunctions::CreatePolylineLocalisation;
    brain[ "DEC_Geometrie_CreerLocalisationPolygone" ] = &DEC_GeometryFunctions::CreatePolygonLocalisation;
    brain[ "DEC_Geometrie_AgrandirLocalisation" ] = &DEC_GeometryFunctions::CreateScaledLocalisation;
    brain[ "DEC_Geometrie_PionEstCoordonne" ] = &DEC_GeometryFunctions::ListUncoordinatedPawns;
    brain[ "DEC_Geometrie_PourcentageTerrainCompartimente" ] = &DEC_GeometryFunctions::ComputeClosedTerrainRatioInFuseau;
    brain[ "DEC_Geometrie_PourcentageTerrainOuvert" ] = &DEC_GeometryFunctions::ComputeOpenTerrainRatioInFuseau;
    brain[ "DEC_Geometrie_TrierZonesSelonOuvertureTerrain" ] = &DEC_GeometryFunctions::SortZonesAccordingToTerrainOpening;
    brain[ "DEC_Geometrie_PourcentageZoneTerrainCompartimente" ] = &DEC_GeometryFunctions::ComputeClosedTerrainRatioInZone;
    brain[ "DEC_Geometrie_PourcentageZoneTerrainOuvert" ] = &DEC_GeometryFunctions::ComputeOpenTerrainRatioInZone;
    brain[ "DEC_Geometrie_TrierFuseauxSelonOuvertureTerrain" ] = &DEC_GeometryFunctions::SortFuseauxAccordingToTerrainOpening;
    brain[ "DEC_Geometrie_ConvertirFuseauEnLocalisation" ] = &DEC_GeometryFunctions::ConvertFuseauToLocalisation;
    brain[ "DEC_Geometrie_ProchainObjectifDansFuseau" ] = &DEC_GeometryFunctions::GetNextObjectiveInFuseau;
    brain[ "DEC_Geometrie_DistanceBetweenPoints" ] = &DEC_GeometryFunctions::ComputeDistance;
    brain[ "DEC_Geometrie_AreaSize" ] = &DEC_GeometryFunctions::ComputeAreaSize;
    brain[ "DEC_Geometrie_AreaDiameter" ] = &DEC_GeometryFunctions::ComputeAreaDiameter;
    brain[ "DEC_Geometrie_DecouperLocalisation" ] = boost::function< std::vector< boost::shared_ptr< MT_Vector2D > >( const TER_Localisation*, unsigned int ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisation, _1, _2 ) );
    brain[ "DEC_Geometrie_ClipperLocalisation" ] = &DEC_GeometryFunctions::ClipLocalisation;
    brain[ "DEC_IsPointInCity" ] = &DEC_GeometryFunctions::IsPointInCity;
    brain[ "DEC_Geometrie_ComputeNearestBorder" ] = &DEC_GeometryFunctions::ComputeNearestBorder;
    brain[ "DEC_Geometrie_FindRoadIntersectionWithZone" ] = &DEC_TerrainFunctions::GetRoadIntersectionsWithZone;
    brain[ "DEC_Geometrie_CalculerTrafficablePointPourPoint" ] =
        boost::function< std::vector< boost::shared_ptr< MT_Vector2D > >( const MT_Vector2D& ) >( boost::bind( &DEC_GeometryFunctions::ComputeTrafficableLocalisation, _1 ) );
    brain[ "DEC_IsPointInUrbanBlockTrafficableForPlatoon" ] =
        boost::function< bool( DEC_Decision_ABC*, MT_Vector2D& ) >( boost::bind( &DEC_GeometryFunctions::IsPointInUrbanBlockTrafficableForPlatoon, _1, _2 ) );
    brain[ "DEC_Geometrie_PositionsParRapportALocalisation" ] =
        boost::function< std::vector< boost::shared_ptr< MT_Vector2D > >( const std::vector< DEC_Decision_ABC* >&, TER_Localisation*, MT_Vector2D*, double ) >( boost::bind( &DEC_GeometryFunctions ::ComputeLocalisationPointsForPionsInFuseau, _1, _2, _3, _4 ) );
    brain[ "DEC_Geometrie_CalculerPointArriveePourFuseau" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( MIL_Fuseau& ) >( boost::bind( &DEC_GeometryFunctions::ComputeDestPointForFuseau, _1 ) );
    brain[ "DEC_Geometrie_CalculerLocalisationsBU" ] = &DEC_GeometryFunctions::ComputeUrbanBlockLocalisations;
    brain[ "DEC_Geometrie_EstPointDansFuseau_AvecParamFuseau" ] = &DEC_GeometryFunctions::IsPointInFuseau_ParamFuseau;
    brain[ "DEC_Geometrie_LocalisationsEgales" ] = &DEC_GeometryFunctions::CompareLocalisations;
    brain[ "DEC_Geometrie_CalculerPointSurFuseau" ] = &DEC_GeometryFunctions::GetPointAlongFuseau;
    brain[ "DEC_Geometrie_StartCalculLignesAvantEtArrierePourPion" ] = &DEC_GeometryFunctions::StartComputingFrontAndBackLinesForPlatoon;
    brain.Register( "DEC_Geometrie_X", &MT_Vector2D::GetX );
    brain.Register( "DEC_Geometrie_Y", &MT_Vector2D::GetY );    
    brain[ "DEC_Geometrie_PositionAdvanceAlongDangerDirection" ] = &DEC_GeometryFunctions::ComputePositionAdvanceAlongDangerDirection;
    
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterUrbanBlockFunctions
// Created: LMT 2010-09-15
// -----------------------------------------------------------------------------
void RegisterUrbanBlockFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_ConnaissanceUrbanBlock_Barycentre" ] = &DEC_UrbanObjectFunctions::GetCurrentBarycenter;
    brain[ "DEC_ConnaissanceUrbanBlock_BarycentreDansBU" ] = &DEC_UrbanObjectFunctions::GetBarycenter;
    brain[ "DEC_ConnaissanceUrbanBlock_Lisiere" ] = &DEC_UrbanObjectFunctions::GetBoundingBox;
    brain[ "DEC_NiveauDeProtectionMaterielComposantes" ] = &DEC_KnowledgeAgentFunctions::GetMaterialComposantesProtectionLevel;
    brain[ "DEC_PolygoneBlocUrbain" ] = &DEC_UrbanObjectFunctions::GetPolygonFromUrbanBlock;
    brain[ "DEC_BlocUrbain_Type" ] = &DEC_UrbanObjectFunctions::GetType;
    brain[ "DEC_EtatBlocUrbain" ] = &DEC_UrbanObjectFunctions::GetStateUrbanBlock;
    brain[ "DEC_IsPointInUrbanBlock" ] = &DEC_GeometryFunctions::IsPointInUrbanBlock;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterResourceNetworkFunctions
// Created: JSR 2011-05-03
// -----------------------------------------------------------------------------
void RegisterResourceNetworkFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_ReseauRessource_Position" ] = &DEC_ResourceNetworkFunctions::GetResourceNetworkPosition;
    brain[ "DEC_ReseauRessource_DesactiverElement" ] = &DEC_ResourceNetworkFunctions::DeactivateResourceNetworkElement;
    brain[ "DEC_ReseauRessource_ActiverElement" ] = &DEC_ResourceNetworkFunctions::ActivateResourceNetworkElement;
    brain[ "DEC_CreerLienObjetVersReseau" ] = &DEC_ResourceNetworkFunctions::CreateResourceNetworkLinkReturn;
    brain[ "DEC_DetruireObjetResourceSansDelais" ] = &DEC_ResourceNetworkFunctions::DestroyResourceNetworkLink;
    brain[ "DEC_ReseauRessourceAugmenteProduction" ] = &DEC_ResourceNetworkFunctions::IncreaseResourceProduction;
    brain[ "DEC_ReseauRessourceBaisseProduction" ] = &DEC_ResourceNetworkFunctions::DecreaseResourceProduction;
    brain.Register( "GetTypeName", &DEC_ResourceNetwork::GetTypeName );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterAreaFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterAreaFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_BMArea_Barycenter" ] = &DEC_GeometryFunctions::ComputeBarycenter;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterFuseauFunctions
// Created: DDA 2010-12-03
// -----------------------------------------------------------------------------
void RegisterFuseauFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_Fuseau_Width" ] = &DEC_GeometryFunctions::GetWidth;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterTimeManagementFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterTimeManagementFunctions( directia::brain::Brain& brain )
{
    // $$$$ AGE 2007-10-11: Un seul temps
    brain[ "DEC_TempsSim" ] = &DEC_DIAFunctions::GetSimTime;
    brain[ "DEC_TempsReel" ] = &DEC_DIAFunctions::GetRealTime;
    brain[ "DEC_Nuit" ] = &DEC_DIAFunctions::IsNight;
    brain[ "DEC_GetTimeInSeconds" ] = &DEC_MiscFunctions::GetTimeInSeconds;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterParametersCopyFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterParametersCopyFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_Copie_Point" ] = &DEC_DIAFunctions::CopyPoint;
    brain[ "DEC_Copie_Point_Mission" ] = &DEC_DIAFunctions::CopyPointMission;
    brain[ "DEC_Copie_ListePoints_Mission" ] = &DEC_DIAFunctions::CopyPathMission;
    brain[ "DEC_Copie_PointDansListePoints_Mission" ] = &DEC_DIAFunctions::CopyPointToListPointMission;
    brain[ "DEC_Copie_Localisation" ] = &DEC_DIAFunctions::CopyLocalisation;
    brain[ "DEC_Copie_Localisation_Mission" ] = &DEC_DIAFunctions::CopyLocalisationMission;
    brain[ "DEC_Copie_ListeLocalisations_Mission" ] = &DEC_DIAFunctions::CopyLocalisationListMission;
    brain[ "DEC_Copie_LocalisationDansListeLocalisations_Mission" ] = &DEC_DIAFunctions::CopyLocalisationToLocationListMission;
    brain[ "DEC_UserTypeList_PushBack_Mission" ] = &DEC_DIAFunctions::CopyKnowledgeObjectToKnowledgeObjectListMission;
    brain[ "DEC_GenObjectList_PushBack_Mission" ] = &DEC_DIAFunctions::CopyGenObjectToGenObjectListMission;
}


// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterListsManipulationFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterListsManipulationFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_ListePoints_GetAt" ] = &DEC_DIAFunctions::ListPoint_GetAt;
    brain[ "DEC_ListePoints_Size" ] = &DEC_DIAFunctions::ListPoint_Size;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterLogisticFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterLogisticFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_NecessiteEvacuationBlesses" ] = &DEC_LogisticFunctions::HasWoundedHumansToEvacuate;
    brain[ "DEC_EvacuerBlessesVersTC2" ] = &DEC_LogisticFunctions::EvacuateWoundedHumansToTC2;
    brain[ "DEC_InterdireEvacuationAutomatiqueBlesses" ] = &DEC_LogisticFunctions::ForbidWoundedHumansAutoEvacuation;
    brain[ "DEC_AutoriserEvacuationAutomatiqueBlesses" ] = &DEC_LogisticFunctions::AllowWoundedHumansAutoEvacuation;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterEngineerObjectsFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterEngineerObjectsFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_GenObject_Type" ] = &DEC_ObjectFunctions::GetGenObjectType;
    brain[ "DEC_GenObject_ExternalIdentifier" ] = &DEC_ObjectFunctions::GetGenObjectExternalIdentifier;
    brain[ "DEC_GenObject_Localisation" ] = &DEC_ObjectFunctions::GetGenObjectLocalisation;
    brain[ "DEC_GenObjectKnowledge_Localisation" ] = &DEC_ObjectFunctions::GetObjectKnowledgeLocalisation;
    brain.Register( "DEC_GenObject_Densite", &DEC_Gen_Object::GetDensity );
    brain[ "DEC_GenObject_TypeObstacleManoeuvre" ] = &DEC_ObjectFunctions::GetGenObjectReservedObstacle;
    brain[ "DEC_GenObject_TC2" ] = &DEC_ObjectFunctions::GetGenObjectTC2;
    brain.Register( "DEC_GenObject_DelaiActiviteMines", &DEC_Gen_Object::GetMinesActivityTime );
    brain.Register( "DEC_GenObject_DateActivationMines", &DEC_Gen_Object::GetActivationTime );
    brain.Register( "DEC_GenObject_Name", &DEC_Gen_Object::GetName );
    brain.Register( "DEC_GenObject_HasCapacity", &DEC_Gen_Object::HasCapacity );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterObjectsFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterObjectsFunctions( directia::brain::Brain& brain )
{
    brain[ "S_TypeObject_ToString" ] = &DEC_ObjectFunctions::ConvertTypeObjectToString;
    brain[ "DEC_ConnaissanceObjet_PeutEtreValorise" ] = &DEC_KnowledgeObjectFunctions::CanBeValorized;
    brain[ "DEC_ConnaissanceObjet_EstUneIssueDeReseauSouterrain" ] = &DEC_KnowledgeObjectFunctions::IsUndergroundNetworkExit;
    brain[ "DEC_ConnaissanceObjet_ActiverIssueDeReseauSouterrain" ] = &DEC_KnowledgeObjectFunctions::ActivateUndergroundNetworkExit;
    brain[ "DEC_ConnaissanceObjet_DesactiverIssueDeReseauSouterrain" ] = &DEC_KnowledgeObjectFunctions::DeactivateUndergroundNetworkExit;
    brain[ "DEC_ConnaissanceObjet_IssuesDuMemeReseauSouterrain" ] = DEC_KnowledgeObjectFunctions::EnterAndExitInSameUndergroundNetwork;
    brain[ "DEC_ObjectKnowledge_HasCapacity" ] = &DEC_KnowledgeObjectFunctions::HasCapacity;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterObjectivesFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterObjectivesFunctions( directia::brain::Brain& brain )
{
    brain.Register( "DEC_Objectif_Localisation", &DEC_Objective::GetLocalisation );
    brain.Register( "DEC_Objectif_Flag",         &DEC_Objective::Flag );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterSpecificPointsFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterSpecificPointsFunctions( directia::brain::Brain& brain )
{
    //Rep_Points
    brain[ "DEC_GetRepPoint" ] = &DEC_PathFunctions::GetRepPoint;
    brain[ "DEC_IsAvantPoint" ] = &DEC_PathFunctions::IsAvantPoint;
    brain[ "DEC_IsPoint" ] = &DEC_PathFunctions::IsPoint;
    brain[ "DEC_GetTypePoint" ] = &DEC_PathFunctions::GetTypePoint;
    brain[ "DEC_GetDestPoint" ] = &DEC_PathFunctions::GetDestPoint;
    brain[ "DEC_GetTypeLimaPoint" ] = &DEC_PathFunctions::GetTypeLimaPoint;
    brain[ "DEC_GetLimaPoint" ] = &DEC_PathFunctions::GetLimaPoint;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterTypeFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterTypeFunctions( directia::brain::Brain& brain )
{
    brain.Register( "GetType", &MIL_Mission_ABC::GetDIAType );
    brain.Register( "GetType", &DEC_Decision_ABC::GetDIAType );
    brain.Register( "GetType", &DEC_RolePion_Decision::GetDIAType );
    brain.Register( "GetType", &DEC_AutomateDecision::GetDIAType );
    brain.Register( "GetType", &DEC_PathPoint::GetDIAType );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterMissionParametersFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterMissionParametersFunctions( directia::brain::Brain& brain, bool isMasalife )
{
    brain[ "DEC_AssignMissionPionParameter" ] = &MIL_MissionParameterFactory::SetPawnParameter;
    brain[ "DEC_AssignMissionAutomatParameter" ] = &MIL_MissionParameterFactory::SetAutomatParameter;
    brain[ "DEC_AssignMissionBoolParameter" ] = &MIL_MissionParameterFactory::SetBoolParameter;
    brain[ "DEC_AssignMissionObjectKnowledgeParameter" ] = &MIL_MissionParameterFactory::SetObjectKnowledgeParameter;
    brain[ "DEC_AssignMissionNatureAtlasTypeParameter" ] = &MIL_MissionParameterFactory::SetNatureAtlasTypeParameter;
    brain[ "DEC_AssignMissionAutomatListParameter" ] = &MIL_MissionParameterFactory::CreateAutomat;
    brain[ "DEC_AssignMissionAgentKnowledgeParameter" ] = &MIL_MissionParameterFactory::SetAgentKnowledgeParameter;
    brain[ "DEC_AssignMissionPathListParameter" ] = &MIL_MissionParameterFactory::CreatePathConst;
    brain[ "DEC_AssignMissionEnumereParameter" ] = &MIL_MissionParameterFactory::SetEnumereParameter;
    brain[ "DEC_AssignMissionLocationParameter" ] = &MIL_MissionParameterFactory::SetLocationParameter;
    brain[ "DEC_AssignMissionObjectKnowledgeListParameter" ] = &MIL_MissionParameterFactory::CreateObjectKnowledge;
    brain[ "DEC_AssignMissionPointParameter" ] = &MIL_MissionParameterFactory::SetPointParameter;
    brain[ "DEC_AssignMissionAgentKnowledgeListParameter" ] = &MIL_MissionParameterFactory::CreateAgentKnowledge;
    brain[ "DEC_AssignMissionGenObjectParameter" ] = &MIL_MissionParameterFactory::SetGenObjectParameter;
    brain[ "DEC_AssignMissionGenObjectListParameter" ] = &MIL_MissionParameterFactory::CreateGenObject;
    brain[ "DEC_AssignMissionPionListParameter" ] = &MIL_MissionParameterFactory::CreatePion;
    brain[ "DEC_AssignMissionLocationListParameter" ] = &MIL_MissionParameterFactory::CreateLocation;
    brain[ "DEC_AssignMissionPointListParameter" ] = &MIL_MissionParameterFactory::CreatePoint;
    brain[ "DEC_AssignMissionUrbanBlockParameter" ] = &MIL_MissionParameterFactory::SetUrbanBlockParameter;
    brain[ "DEC_AssignMissionUrbanBlockListParameter" ] = &MIL_MissionParameterFactory::CreateUrbanBlock;
    brain[ "DEC_AssignMissionDirectionParameter" ] = &MIL_MissionParameterFactory::SetDirectionParameter;
    brain[ "DEC_AssignMissionDotationTypeParameter" ] = &MIL_MissionParameterFactory::SetDotationTypeParameter;
    brain[ "DEC_AssignMissionNumericTypeParameter" ] = &MIL_MissionParameterFactory::SetNumericTypeParameter;
    brain[ "DEC_AssignMissionResourceNetworkParameter" ] = &MIL_MissionParameterFactory::SetResourceNetworkParameter;
    brain[ "DEC_AssignMissionResourceNetworkListParameter" ] = &MIL_MissionParameterFactory::CreateResourceNetwork;
    brain[ "DEC_AssignMissionPathConstParameter" ] = &MIL_MissionParameterFactory::AssignPathConst;
    brain[ "DEC_IsFragOrderAvailable" ] = &DEC_OrdersFunctions::IsFragOrderAvailable;
    brain[ "DEC_IsMissionAvailable" ] = &DEC_OrdersFunctions::IsPionMissionAvailable;
    brain[ "DEC_Mission_IsPath" ] = &DEC_OrdersFunctions::DEC_Mission_IsPath;
    brain[ "DEC_IsAutomateMissionAvailable" ] = &DEC_OrdersFunctions::IsAutomateMissionAvailable;
    directia::tools::binders::ScriptRef initParameterFunction = brain[ "InitTaskParameter" ];
    brain[ "DEC_FillMissionParameters" ] =
        boost::function< void( const directia::tools::binders::ScriptRef&, boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_MiscFunctions::FillMissionParameters, boost::ref(brain), initParameterFunction, _1 , _2, isMasalife ) );
    brain[ "DEC_AssignerFuseauAMissionPion_Mission" ] =
        boost::function< void( MIL_Fuseau* ,  boost::shared_ptr< MIL_Mission_ABC > )>( boost::bind( &DEC_OrdersFunctions::AssignFuseauToPionMission , _1, _2 ) );
    brain[ "DEC_AssignerFuseauAMissionAutomate_Mission" ] =
            boost::function< void( MIL_Fuseau* ,  boost::shared_ptr< MIL_Mission_ABC > )>( boost::bind( &DEC_OrdersFunctions::AssignFuseauToAutomateMission , _1, _2 ) );
    brain[ "DEC_AssignerDirectionAMissionAutomate_Mission" ] =
            boost::function< void (MT_Vector2D* ,  boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_OrdersFunctions::AssignDirectionToAutomateMission , _1, _2 ) );
    brain[ "DEC_AssignMissionListParameter" ] = &MIL_MissionParameterFactory::AssignMissionListParameter;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterObjectFunctions
// Created: LDC 2012-09-06
// -----------------------------------------------------------------------------
void RegisterObjectFunctions( directia::brain::Brain& brain )
{
    // Objet
    brain[ "DEC_Agent_AgentPeutConstruireObjetEmbarque" ] = &DEC_AgentFunctions::AgentCanConstructObjectWithLoaded;
    brain[ "DEC_Agent_AgentPeutConstruireObjetEmbarqueAvecLocalisation" ] = &DEC_AgentFunctions::AgentCanConstructObjectWithLoadedAndLocalisation;
    brain[ "DEC_Agent_AgentADotationPourConstruireObjet" ] = &DEC_AgentFunctions::AgentHasDotationForBuilding;
    brain[ "DEC_Agent_AgentADotationPourConstruireObjetSansRenfort" ] = &DEC_AgentFunctions::AgentHasDotationForBuildingWithOutLoaded;
    brain[ "DEC_Agent_AgentPeutDetruireObjet" ] = &DEC_AgentFunctions::AgentCanDestroyObject;
    brain[ "DEC_Agent_AgentPeutConstruireContournementObjet" ] = &DEC_AgentFunctions::AgentCanBypassObject;
    brain[ "DEC_Agent_AgentPeutDetruireTypeObjet" ] = &DEC_AgentFunctions::AgentCanDestroyObjectType;
    brain[ "DEC_Agent_AgentPeutDetruireTypeObjetAvecLocalisation" ] = &DEC_AgentFunctions::AgentCanDestroyObjectTypeWithLocalisation;
    brain[ "DEC_Agent_AgentPeutTransporterPion" ] = &DEC_AgentFunctions::AgentCanTransportPion;

    brain[ "DEC_Agent_PeutActiverObjet" ] = &DEC_AgentFunctions::CanActivateObject;
    
    brain[ "DEC_Agent_GetAgentDotation" ] = &DEC_AgentFunctions::GetAgentDotationNumber;
    brain[ "DEC_GetAgentDotationManquantePourConstruireObjet" ] = &DEC_AgentFunctions::GetAgentMissingDotationForBuildingObject;
    brain[ "DEC_GetAgentDotationManquantePourConstruireObjetExistant" ] = &DEC_AgentFunctions::GetAgentMissingDotationForBuildingExistingObject;
    brain[ "DEC_GetAgentDotationManquantePourValoriserObjet" ] = &DEC_AgentFunctions::GetAgentMissingDotationForMining;
    brain[ "DEC_GetAgentsPouvantConstruire" ] = &DEC_AgentFunctions::RetrieveUnitsAbleToBuild;
    brain[ "DEC_GetAgentsPouvantConstruireAvecLocalisation" ] = &DEC_AgentFunctions::RetrieveUnitsAbleToBuildWithLocalisation;
    brain[ "DEC_GetAgentsPouvantValoriser" ] = &DEC_AgentFunctions::RetrieveUnitsAbleToMine;
    brain[ "DEC_GetAgentsPouvantValoriserAvecLocalisation" ] = &DEC_AgentFunctions::RetrieveUnitsAbleToMineWithLocalisation;
    brain[ "DEC_GetAgentsPouvantContourner" ] = &DEC_AgentFunctions::RetrieveUnitsAbleToByPass;
    brain[ "DEC_GetAgentsPouvantContournerAvecLocalisation" ] = &DEC_AgentFunctions::RetrieveUnitsAbleToByPassWithLocalisation;
    brain[ "DEC_GetAgentsPouvantDetruire" ] = &DEC_AgentFunctions::RetrieveUnitsAbleToDestroy;
    brain[ "DEC_GetAgentsPouvantDetruireAvecLocalisation" ] = &DEC_AgentFunctions::RetrieveUnitsAbleToDestroyWithLocalisation;
    brain[ "DEC_GetAgentsPouvantDevaloriserAvecLocalisation" ] = &DEC_AgentFunctions::RetrieveUnitsAbleToDemineWithLocalisation;
    brain[ "DEC_GetAgentsPouvantDevaloriserEmbarqueAvecLocalisation" ] = &DEC_AgentFunctions::RetrieveUnitsAbleToDemineWithOutLoadedWithLocalisation;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterReportFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterReportFunctions( directia::brain::Brain& brain )
{
    brain[ "DEC_PointToString" ] = &DEC_DIAFunctions::PointToString;
    brain[ "DEC_DirectionToString" ] =  &DEC_DIAFunctions::DirectionToString ;
    brain[ "DEC_ItineraireToString" ] = &DEC_DIAFunctions::PathToString ;
    brain[ "DEC_UnitDecisionalState" ] = &DEC_KnowledgeAgentFunctions::UnitDecisionalState;
    brain[ "DEC_RC1" ] =
        boost::function< void ( DEC_Decision_ABC&, int, int ) >( boost::bind( &DEC_MiscFunctions::Report, _1, _2, _3 ) );
    brain[ "DEC_RC_AgentKnowledge" ] =
        boost::function< void ( DEC_Decision_ABC&, int, int, boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_MiscFunctions::ReportAgentKnowledge, _1, _2, _3, _4 ) );
    brain[ "DEC_RC_DotationType" ] =
        boost::function< void ( DEC_Decision_ABC&, int, int, const PHY_DotationCategory* ) >( boost::bind( &DEC_MiscFunctions::ReportDotationType, _1, _2, _3, _4 ) );
    brain[ "DEC_RC_EquipmentType" ] =
        boost::function< void ( DEC_Decision_ABC&, int, int, const PHY_ComposanteTypePion* ) >( boost::bind( &DEC_MiscFunctions::ReportEquipmentType, _1, _2, _3, _4 ) );
    brain[ "DEC_RC_Float" ] =
        boost::function< void ( DEC_Decision_ABC&, int, int, float ) >( boost::bind( &DEC_MiscFunctions::ReportFloat, _1, _2, _3, _4 ) );
    brain[ "DEC_RC_Float_Float" ] =
        boost::function< void ( DEC_Decision_ABC&, int, int, float, float ) >( boost::bind( &DEC_MiscFunctions::ReportFloatFloat, _1, _2, _3, _4, _5 ) );
    brain[ "DEC_RC_Id" ] =
        boost::function< void ( DEC_Decision_ABC&, int, int, int ) >( boost::bind( &DEC_MiscFunctions::ReportId, _1, _2, _3, _4 ) );
    brain[ "DEC_RC_ObjectKnowledge" ] =
        boost::function< void ( DEC_Decision_ABC&, int, int, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_MiscFunctions::ReportObjectKnoweldge, _1, _2, _3, _4 ) );
    brain[ "DEC_RC_Pion" ] =
        boost::function< void ( DEC_Decision_ABC&, int, int, DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::ReportPion, _1, _2, _3, _4 ) );
    brain[ "DEC_RC_Pion_Automate" ] =
        boost::function< void ( DEC_Decision_ABC&, int, int, DEC_Decision_ABC*, DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::ReportPionAutomate, _1, _2, _3, _4, _5 ) );
    brain[ "DEC_RC_Pion_Pion" ] =
        boost::function< void ( DEC_Decision_ABC&, int, int, DEC_Decision_ABC*, DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::ReportPionPion, _1, _2, _3, _4, _5 ) );
    brain[ "DEC_RC_PopulationKnowledge" ] =
        boost::function< void ( DEC_Decision_ABC&, int, int, int ) >( boost::bind( &DEC_MiscFunctions::ReportPopulationKnowledge, _1, _2, _3, _4 ) );
    brain[ "DEC_RC_TirPion" ] =
        boost::function< void ( DEC_Decision_ABC&, int, int, int ) >( boost::bind( &DEC_MiscFunctions::ReportTirPion, _1, _2, _3, _4 ) );
    brain[ "DEC_RC_String" ] =
        boost::function< void ( DEC_Decision_ABC&, int, int, const std::string& ) >( boost::bind( &DEC_MiscFunctions::ReportString, _1, _2, _3, _4 ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterTelepathyFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterTelepathyFunctions( directia::brain::Brain& brain )
{
    brain.Register( "SetStateVariable", &DEC_Decision_ABC::SetStateVariable );
    brain.Register( "SetambianceMission_", &DEC_Decision_ABC::SetAmbianceMission );
    brain.Register( "SetbAppuieFreinage_", &DEC_Decision_ABC::SetAppuieFreinage );
    brain.Register( "GetbDemandeOrdreConduitePoursuivre_", &DEC_Decision_ABC::GetDemandeOrdreConduitePoursuivre );
    brain.Register( "GetbEnCoursExtractionPersonnel_", &DEC_Decision_ABC::GetEnCoursExtractionPersonnel );
    brain.Register( "GetbEnExploitation_", &DEC_Decision_ABC::GetEnExploitation );
    brain.Register( "SetbEnExploitation_", &DEC_Decision_ABC::SetbEnExploitation );
    brain.Register( "GetbEnPhaseRavitaillement_", &DEC_Decision_ABC::GetEnPhaseRavitaillement );
    brain.Register( "SetbEnPhaseRavitaillement_", &DEC_Decision_ABC::SetEnPhaseRavitaillement );
    brain.Register( "GetbMiseEnOeuvre_", &DEC_Decision_ABC::GetMiseEnOeuvre );
    brain.Register( "SetbMiseEnOeuvre_", &DEC_Decision_ABC::SetMiseEnOeuvre );
    brain.Register( "GeteEtatFeu_", &DEC_Decision_ABC::GetEtatFeu );
    brain.Register( "GetlisteEnisTirAutorise_", &DEC_Decision_ABC::GetListeEnisTirAutorise );
    brain.Register( "SetlisteEnisTirAutorise_", &DEC_Decision_ABC::SetListeEnisTirAutorise );
    brain.Register( "ClearlisteEnisTirAutorise_", &DEC_Decision_ABC::ClearListeEnisTirAutorise );
    brain.Register( "GetlistePionsCoordination_", &DEC_Decision_ABC::GetListePionsCoordination );
    brain.Register( "ClearlistePionsCoordination_", &DEC_Decision_ABC::ClearListePionsCoordination );
    brain.Register( "GetptSauvegarde_", &DEC_Decision_ABC::GetPtSauvegarde );
    brain.Register( "GetlisteZonesTir_", &DEC_Decision_ABC::GetListeZonesTir );
    brain.Register( "ClearlisteZonesTir_", &DEC_Decision_ABC::ClearListeZonesTir );
    brain.Register( "GetobjMisEnCours_", &DEC_Decision_ABC::GetObjMisEnCours );
    brain.Register( "SetobjMisEnCours_", &DEC_Decision_ABC::SetObjMisEnCours );
    brain.Register( "GetobjectifCourant_", &DEC_Decision_ABC::GetObjectifCourant );
    brain.Register( "GetplotRavitaillementAssigne_", &DEC_Decision_ABC::GetPlotRavitaillementAssigne );
    brain.Register( "SetplotRavitaillementAssigne_", &DEC_Decision_ABC::SetPlotRavitaillementAssigne );
    brain.Register( "ClearplotsRavitaillement_", &DEC_Decision_ABC::ClearPlotsRavitaillement );
    brain.Register( "GetporteeAction_", &DEC_Decision_ABC::GetPorteeAction );
    brain.Register( "SetporteeAction_", &DEC_Decision_ABC::SetPorteeAction );
    brain.Register( "GetrNiveauAlerteRavitaillement_", &DEC_Decision_ABC::GetNiveauAlerteRavitaillement );
    brain.Register( "Getpoint_", &DEC_PathPoint::GetPos );
    // brain.Register( "Getmunitions_", &DEC_Decision_ABC::GetMunition );
    brain.Register( "Setmunitions_", &DEC_Decision_ABC::SetMunition );
    brain.Register( "Getmunitions_", &DEC_Decision_ABC::GetMunition );
    brain.Register( "SetnbIT_", &DEC_Decision_ABC::SetNbIt );
    brain.Register( "GetnbIT_", &DEC_Decision_ABC::GetNbIt );
    brain.Register( "Setcible_", &DEC_Decision_ABC::SetTarget );
    brain.Register( "Getcible_", &DEC_Decision_ABC::GetTarget );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterItineraryFunctions
// Created: SLI 2010-07-09
// -----------------------------------------------------------------------------
void RegisterItineraryFunctions( directia::brain::Brain& brain )
{
    brain.Register( "DEC_Itineraire_DernierPoint", &DEC_Decision_ABC::GetLastPointOfPath );
    brain.Register( "DEC_Itineraire_ExtrapolerPosition", &DEC_Decision_ABC::ExtrapolatePosition );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RegisterCommonUserFunctions
// Created: LDC 2009-04-22
// -----------------------------------------------------------------------------
void RegisterCommonUserFunctions( directia::brain::Brain& brain, bool isMasalife )
{
    RegisterUnitFunctions( brain );
    RegisterPopulationFunctions( brain );
    RegisterAgentKnowledgeFunctions( brain );
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
    DEC_CommunicationFunctions::Register( brain );
    DEC_TelepathyFunctions::Register( brain );
    MIL_FragOrder::Register( brain );
}

typedef bool (*T_Function)( const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element );
typedef bool (*T_FunctionBM)( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element );

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
bool BoolFunctionBM( directia::brain::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
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
bool EnumerationFunctionBM( directia::brain::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
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
bool PointFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< MT_Vector2D > value;
    if( element.ToPoint( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.point" ] , name, value, false );
        return true;
    }
    return false;
}

bool PointListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPointList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.point" ], name, value, true );
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
bool AreaFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToPolygon( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.area" ], name, value, false );
        return true;
    }
    return false;
}
bool AreaListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToPolygonList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.area" ], name, value, true );
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
bool PathFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< MT_Vector2D > > value;
    if( element.ToPath( value ) && !value.empty() )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.point" ], name, value, true );
        return true;
    }
    return false;
}
bool DirectionFunction( const directia::tools::binders::ScriptRef& /*refMission*/, const std::string& /*name*/, MIL_MissionParameter_ABC& /*element*/ )
{
    // $$$$ LDC: FIXME The only existing Direction argument is dangerDirection_ which is never used by the brains.
    return false;
}
bool DirectionFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< MT_Vector2D > value;
    if( element.ToDirection( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission,  brain[ "integration.ontology.types.direction" ] , name, value, false );
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
bool NatureAtlasFunctionBM( directia::brain::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
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
bool AutomatFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DEC_Decision_ABC* value = 0;
    if( element.ToAutomat( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.automat" ], name, value, false );
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
bool AutomatListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< DEC_Decision_ABC* > value;
    if( element.ToAutomatList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.automat" ], name, value, true );
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
bool AgentFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    const DEC_Decision_ABC* value = 0; // $$$$ LDC: Parfois on se sert de champs dessus comme eniEnCours_...
    if( element.ToAgent( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.agent" ], name, value, false );
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
bool AgentListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const DEC_Decision_ABC* > value;
    if( element.ToAgentList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.agent" ], name, value, true );
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
bool AgentKnowledgeFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Agent > value;//@TODO SEE how to bind agent and knowledge agent with the same BM knowledge
    if( element.ToAgentKnowledge( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.agentKnowledge" ], name, value, false );
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
bool AgentKnowledgeListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > value;
    if( element.ToAgentKnowledgeList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.agentKnowledge" ], name, value, true );
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
bool ObjectKnowledgeFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Object > value;
    if( element.ToObjectKnowledge( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.object" ], name, value, false );
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
bool ObjectKnowledgeListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Knowledge_Object > > value;
    if( element.ToObjectKnowledgeList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.object" ], name, value, true );
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
bool PopulationKnowledgeFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Knowledge_Population > value;
    if( element.ToPopulationKnowledge( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.population" ], name, value->GetID(), false );//@TODO MGD Add CompositeReachable for Population?
         return true;
    }
    return false;
}

bool UrbanBlockFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    UrbanObjectWrapper* value = 0;
    if( element.ToUrbanBlock( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.urbanBlock" ], name, value, false );
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

bool DotationTypeFunctionBM( directia::brain::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    DotationTypeFunction( refMission, name, element );
    return true;
}

bool DotationTypeListFunctionBM( directia::brain::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_DotationCategory* > value;
    if( element.ToDotationTypeList( value ) )
    {
        refMission[ name ] = value;
        return true;
    }
    return false;
}

bool ResourceNetworkFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_ResourceNetwork > value;
    if( element.ToResourceNetwork( value ) && value )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.resourceNetwork" ], name, value, false );
        return true;
    }
    return false;
}

bool ResourceNetworkListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_ResourceNetwork > > value;
    if( element.ToResourceNetworkList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.resourceNetwork" ], name, value, true );
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
bool GDHFunctionBM( directia::brain::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
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
bool NumericFunctionBM( directia::brain::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
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
bool NumericListFunctionBM( directia::brain::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
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
bool GenObjectFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< DEC_Gen_Object > value;
    if( element.ToGenObject( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.genObject" ], name, value, false );
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
bool GenObjectListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< DEC_Gen_Object > > value;
    if( element.ToGenObjectList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.genObject" ], name, value, true );
        return true;
    }
    return false;
}
bool MaintenancePrioritiesFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_ComposanteTypePion* > value;
    if( element.ToMaintenancePriorities( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.equipmentType" ], name, value, true );
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
bool MedicalPrioritiesFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< const PHY_HumanWound* > value;
    if( element.ToMedicalPriorities( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.humanWound" ], name, value, true );
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
bool StringFunctionBM( directia::brain::Brain& /*brain*/, directia::tools::binders::ScriptRef& /*knowledgeCreateFunction*/, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
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

bool LocationCompositeFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
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

bool LocationCompositeListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> > list;
    if( element.ToList( list ) )
    {
        knowledgeCreateFunction( refMission, "beginlist", name );
        for( std::vector< boost::shared_ptr<MIL_MissionParameter_ABC> >::const_iterator it = list.begin(); it != list.end(); ++it )
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

bool PhaseLineFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    boost::shared_ptr< TER_Localisation > value;
    if( element.ToLima( value ) && value.get() )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.limaStatique" ], name, value, false );
        return true;
    }
    return false;
}

bool PhaseLineListFunctionBM( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const std::string& name, MIL_MissionParameter_ABC& element )
{
    std::vector< boost::shared_ptr< TER_Localisation > > value;
    if( element.ToLimaList( value ) )
    {
        knowledgeCreateFunction( refMission, brain[ "integration.ontology.types.limaStatique" ], name, value, true );
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
        functorsBM[ "PathList" ] = PathFunctionBM;
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
        functorsBM[ "LocationComposite" ] = LocationCompositeFunctionBM;
        functorsBM[ "LocationCompositeList" ] = LocationCompositeListFunctionBM;
        functorsBM[ "PhaseLine" ] = PhaseLineFunctionBM;
        functorsBM[ "PhaseLineList" ] = PhaseLineListFunctionBM;
        functorsBM[ "ResourceType" ] = DotationTypeFunctionBM;
        functorsBM[ "ResourceTypeList" ] = DotationTypeListFunctionBM;
        functorsBM[ "ResourceNetwork" ] = ResourceNetworkFunctionBM;
        functorsBM[ "ResourceNetworkList" ] = ResourceNetworkListFunctionBM;
    }
}


class RegisterMissionParameterVisitor : public MIL_MissionParameterVisitor_ABC
{
public:
    RegisterMissionParameterVisitor( directia::brain::Brain& brain, const directia::tools::binders::ScriptRef& refMission, directia::tools::binders::ScriptRef& knowledgeCreateFunction )
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
     directia::brain::Brain& brain_;
     const directia::tools::binders::ScriptRef& refMission_;
     directia::tools::binders::ScriptRef& knowledgeCreateFunction_;
};

class RegisterMissionParameterBMVisitor : public MIL_MissionParameterVisitor_ABC
{
public:
    RegisterMissionParameterBMVisitor( directia::brain::Brain& brain, const directia::tools::binders::ScriptRef& refMission, directia::tools::binders::ScriptRef& knowledgeCreateFunction )
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
                refMission_[ dianame ] = brain_[ "NIL" ];
        }
    }

private:
     directia::brain::Brain& brain_;
     const directia::tools::binders::ScriptRef& refMission_;
     directia::tools::binders::ScriptRef& knowledgeCreateFunction_;
};

// -----------------------------------------------------------------------------
// Name: RegisterMissionParameters
// Created: LDC 2009-05-04
// -----------------------------------------------------------------------------
void RegisterMissionParameters( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const boost::shared_ptr< MIL_Mission_ABC > mission, bool isMasalife )
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
    std::map< std::string, boost::shared_ptr< directia::brain::Brain > > brainTable;

    void LoadResourcesFile( const std::string& file, boost::shared_ptr< directia::brain::Brain >& pArchetypeBrain )
    {
        return (*pArchetypeBrain)[ "include" ]( file );
    }
}

bool CreateBrain( boost::shared_ptr< directia::brain::Brain >& pArchetypeBrain, boost::shared_ptr< directia::brain::Brain >& pBrain,
                  const std::string& includePath, const std::string& brainFile, bool isMasalife, const std::string& type, bool reload )
{
    pArchetypeBrain = isMasalife ? brainTable[type] : brainTable[brainFile];
    if( reload )
        pArchetypeBrain.reset();
    if( !pArchetypeBrain )
    {
        if( isMasalife )
        {
            pArchetypeBrain.reset( new directia::brain::Brain(
                "plugins={"
                + PLUGIN( "masalife_brain" )
                + PLUGIN( "knowledge" )
                + PLUGIN( "communication" )
                + PLUGIN46( "errorhandler" )
                + "} cwd='" + includePath + "'" ) );
            (*pArchetypeBrain)[ "LoadResourcesFile" ] = boost::function< void( const std::string& ) >(
                boost::bind( &LoadResourcesFile, _1, boost::ref( pArchetypeBrain ) ) );
            (*pArchetypeBrain)["include"]( brainFile ,includePath, type );
            if( !reload )
                brainTable[type] = pArchetypeBrain;
        }
        else
        {
            pArchetypeBrain.reset( new directia::brain::Brain(
                "plugins={"
                + PLUGIN46( "eventmanager" )
                + PLUGIN46( "motivation" )
                + PLUGIN46( "errorhandler" )
                + "} cwd='" + includePath + "'" ) );
            (*pArchetypeBrain)[ "LoadResourcesFile" ] = boost::function< void( const std::string& ) >(
                boost::bind( &LoadResourcesFile, _1, boost::ref( pArchetypeBrain ) ) );
            (*pArchetypeBrain)["include"]( brainFile ,includePath, type );
            if( !reload )
                brainTable[brainFile] = pArchetypeBrain;
        }
        if( reload )
            pBrain = pArchetypeBrain; 
        else
            pBrain.reset( new directia::brain::Brain( *pArchetypeBrain ) );
        return true;
    }
    else
        pBrain.reset( new directia::brain::Brain( *pArchetypeBrain ) );
    return false;
}

}
