// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Decision/DEC_RolePion_Decision.cpp $
// $Author: Nld $
// $Modtime: 23/06/05 10:28 $
// $Revision: 19 $
// $Workfile: DEC_RolePion_Decision.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_RolePion_Decision.h"
#include "Decision/DEC_AgentFunctions.h"
#include "Decision/DEC_KnowledgeAgentFunctions.h"
#include "Decision/DEC_KnowledgeObjectFunctions.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Decision/DEC_KnowledgeFunctions.h"
#include "Decision/DEC_PerceptionFunctions.h"
#include "Decision/DEC_MiscFunctions.h"
#include "Decision/DEC_ActionFunctions.h"
#include "Decision/DEC_LogisticFunctions.h"
#include "Decision/DEC_ObjectFunctions.h"

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::RegisterUserArchetypeFunctions
// Created: ADY 2010-08-02
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::RegisterUserArchetypeFunctions ( directia::brain::Brain& brain )
{
    brain[ "DEC_ActiverObjet" ] = &DEC_ObjectFunctions::ActivateObject;
    brain[ "DEC_DetruireObjetSansDelais" ] = &DEC_ObjectFunctions::MagicDestroyObject;
    brain[ "DEC_DetruireObjetIdSansDelais" ] = &DEC_ObjectFunctions::MagicDestroyObjectId;
    brain[ "DEC_CreateDynamicGenObject" ] = &DEC_ObjectFunctions::CreateDynamicGenObject;
    brain[ "DEC_CreateDynamicGenObjectFromSharedLocalisation" ] = &DEC_ObjectFunctions::CreateDynamicGenObjectFromSharedLocalisation;

    // Tests / Debug
    brain[ "DEC_DebugPointXY" ] = &DEC_MiscFunctions::GetPointXY;

    // Pion accessors
    brain[ "DEC_HasDotationForFiring" ] = &DEC_AgentFunctions::HasDotationForFiring;
    brain[ "DEC_Agent_PeutActiverObjet" ] = &DEC_AgentFunctions::CanActivateObject;
    brain[ "DEC_Agent_PeutIllumine" ] = &DEC_AgentFunctions::CanIlluminate;

    //agent knowledge accessors
    brain[ "DEC_ConnaissanceAgent_EstIllumine" ] = &DEC_KnowledgeAgentFunctions::IsIlluminated;
    brain[ "DEC_ConnaissanceAgent_EstDefinitivementIllumine" ] = &DEC_KnowledgeAgentFunctions::IsDefinitivelyIlluminated;
    brain[ "DEC_ConnaissanceAgent_EstDansFoule" ] = &DEC_KnowledgeAgentFunctions::IsInCrowd;
    brain[ "DEC_ConnaissanceAgent_Neutraliser" ]= DEC_KnowledgeAgentFunctions::NeutralizeAgent;

    // Object knowledges accessors
    brain[ "DEC_IsValidKnowledgeObject" ] = &DEC_KnowledgeObjectFunctions::IsKnowledgeValid;
    brain[ "DEC_ConnaissanceObjet_EstObstacleDeManoeuvreActif" ] = &DEC_KnowledgeObjectFunctions::IsReservedObstacleActivated;
    brain[ "DEC_ConnaissanceObjet_EstObstacleDeManoeuvre" ] = &DEC_KnowledgeObjectFunctions::IsReservedObstacle;
    brain[ "DEC_ConnaissanceObjet_DateActivationObstacle" ] = &DEC_KnowledgeObjectFunctions::GetActivationTime;
    brain[ "DEC_ConnaissanceObjet_EstContourne" ] = &DEC_KnowledgeObjectFunctions::IsBypassed;
    brain[ "DEC_ConnaissanceObjet_EstValorise" ] = &DEC_KnowledgeObjectFunctions::IsMined;
    brain[ "DEC_ConnaissanceObjet_EstBreche" ] = &DEC_KnowledgeObjectFunctions::IsBreached;
    brain[ "DEC_ConnaissanceObjet_NiveauConstruction" ] = &DEC_KnowledgeObjectFunctions::GetConstructionLevel;
    brain[ "DEC_ConnaissanceObjet_NiveauValorisation" ] = &DEC_KnowledgeObjectFunctions::GetValorizationLevel;
    brain[ "DEC_ConnaissanceObjet_Localisation" ] = &DEC_KnowledgeObjectFunctions::GetLocalisation;
    brain[ "DEC_ConnaissanceObjet_Type" ] = &DEC_KnowledgeObjectFunctions::GetType;
    brain[ "DEC_ConnaissanceObjet_Name" ] = &DEC_KnowledgeObjectFunctions::GetName;
    brain[ "DEC_ConnaissanceObjet_NiveauAnimation" ] = &DEC_KnowledgeObjectFunctions::GetAnimationLevel;
    brain[ "DEC_ConnaissanceObjet_LargeurSiteFranchissement" ] = &DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth;
    brain[ "DEC_ConnaissanceObjet_EstReconnu" ] = &DEC_KnowledgeObjectFunctions::IsRecon;
    brain[ "DEC_ConnaissanceObjet_PeutEtreContourne" ] = &DEC_KnowledgeObjectFunctions::CanBeBypassed;
    brain[ "DEC_ConnaissanceObjet_ChangeDensitePopulationSortante" ] = &DEC_KnowledgeObjectFunctions::SetExitingPopulationDensity;
    brain[ "DEC_ConnaissanceObjet_ChangeDensitePopulationSortanteEnPourcentage" ] = &DEC_KnowledgeObjectFunctions::SetExitingPopulationDensityInPercentage;
    brain[ "DEC_ConnaissanceObjet_ResetDensitePopulationSortante" ] = &DEC_KnowledgeObjectFunctions::ResetExitingPopulationDensity;
    brain[ "DEC_ConnaissanceObjet_PointEstDansZoneEvitement"] = &DEC_KnowledgeObjectFunctions::IsInAvoidanceArea;
    brain[ "DEC_ConnaissanceObjet_PointEstProcheZoneEffet"] = &DEC_KnowledgeObjectFunctions::IsNearEffectArea;
    brain[ "DEC_ConnaissanceObjet_BurningLevel" ] = &DEC_KnowledgeObjectFunctions::GetBurningLevel;

    // Urban knowledges accessors
    brain[ "DEC_NiveauDeProtectionMaterielComposantes" ] = &DEC_KnowledgeAgentFunctions::GetMaterialComposantesProtectionLevel;
    brain[ "DEC_GetAgentIdFromKnowledge" ] = &DEC_KnowledgeAgentFunctions::GetAgentIdFromKnowledge;
    brain[ "DEC_Connaissances_BlocUrbainDansCercle" ] = &DEC_KnowledgeFunctions::GetUrbanBlockInCircle;

    //limas / Missions
    brain[ "DEC_GetRawMission" ] = &DEC_AgentFunctions::GetMission;
    brain[ "DEC_Connaissance_GetRawMission" ] = &DEC_KnowledgeAgentFunctions::GetMission;
    brain[ "DEC_HasMission" ] = &DEC_AgentFunctions::HasMission;
    brain[ "DEC_GetDirectionDanger" ] = &DEC_AgentFunctions::GetDirectionDanger;
    brain[ "DEC_GetDirectionEnnemi" ] = &DEC_AgentFunctions::GetDirectionDanger;

    //Calculs de positions
    brain[ "DEC_Geometrie_PionDevant" ] = &DEC_GeometryFunctions::GetFrontestPion;
    brain[ "DEC_Geometrie_PionDerriere" ] = &DEC_GeometryFunctions::ComputeBackestAgent;
    brain[ "DEC_Perception_AjouterPointReconnuAutomateParPion" ] = &DEC_PerceptionFunctions::AddReconnoiteredPointBy;
    brain[ "DEC_Perception_EstPointReconnuParPionOuPersonne" ] = &DEC_PerceptionFunctions::PointIsReconnoiteredByMeOrNoOne;

    // Logistique
    brain[ "DEC_DemandeDeRavitaillement" ] = &DEC_LogisticFunctions::PionRequestSupply;
    brain[ "DEC_ChangerSuperieurLogistiqueConnaissance" ] = &DEC_KnowledgeAgentFunctions::SwitchAutomateLogistic;

    // Prisonniers
    brain[ "DEC_Prisonniers_EstDebarqueDansCamp" ] = &DEC_ActionFunctions::Prisoners_IsUnloadedInCamp;

    // Refugies
    brain[ "DEC_Refugies_EstDebarqueDansCamp" ] = &DEC_ActionFunctions::Refugees_IsUnloadedInCamp;

    // Critical Intelligence
    brain[ "DEC_ObtenirRenseignementCritiqueSurPion" ] = &DEC_KnowledgeAgentFunctions::GetCriticalIntelligence;

    // Mount/dismount
    brain[ "DEC_CanMount" ] = &DEC_AgentFunctions::CanMount;

    // Réseaux souterrains
    brain[ "DEC_Agent_TempsPourTraverserReseauSouterrain" ] = &DEC_KnowledgeObjectFunctions::EstimatedUndergroundTime;
    brain[ "DEC_Agent_SeDissimulerDansReseauSouterrain" ] = &DEC_KnowledgeObjectFunctions::HideInUndergroundNetwork;
    brain[ "DEC_Agent_SortirDuReseauSouterrain" ] = &DEC_KnowledgeObjectFunctions::GetOutFromUndergroundNetwork;
}