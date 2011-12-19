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
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Decision/DEC_AgentFunctions.h"
#include "Decision/DEC_KnowledgeAgentFunctions.h"
#include "Decision/DEC_KnowledgeObjectFunctions.h"
#include "Decision/DEC_KnowledgePopulationFunctions.h"
#include "Decision/DEC_UrbanObjectFunctions.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Decision/DEC_FireFunctions.h"
#include "Decision/DEC_KnowledgeFunctions.h"
#include "Decision/DEC_PathFunctions.h"
#include "Decision/DEC_PerceptionFunctions.h"
#include "Decision/DEC_MiscFunctions.h"
#include "Decision/DEC_OrdersFunctions.h"
#include "Decision/DEC_ActionFunctions.h"
#include "Decision/DEC_PathFunctions.h"
#include "Decision/DEC_LogisticFunctions.h"
#include "Decision/DEC_ObjectFunctions.h"
#include "Decision/DEC_MedicalTreatmentFunctions.h"
#include "Decision/DEC_TerrainFunctions.h"
#include "Entities/Actions/PHY_ActionInterrogate.h"
#include "Entities/Agents/Actions/ComposanteLending/PHY_ActionLendCollectionComposantes.h"
#include "Entities/Agents/Actions/ComposanteLending/PHY_ActionLendHaulerComposantes.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionConstructObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionPrepareObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionDestroyObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionMineObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionExtinguishObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionBypassObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionOccupyObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionAnimateObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionResumeWorkObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionResumeWorkUrbanBlock.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionDistributeObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionSupplyObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionExtractFromStockObject.h"
#include "Entities/Agents/Actions/Moving/PHY_ActionMove.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Position.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Knowledge.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePopulation.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePion.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionUsingOnlyLoadable.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionUsingOnlyCarrier.h"
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

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::RegisterUserArchetypeFunctions
// Created: ADY 2010-08-02
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::RegisterUserArchetypeFunctions ( directia::brain::Brain& brain )
{
    brain[ "DEC_ActiverObjet" ] = &DEC_ObjectFunctions::ActivateObject;
    brain[ "DEC_DetruireObjetSansDelais" ] = &DEC_ObjectFunctions::MagicDestroyObject;
    brain[ "DEC_CreateDynamicGenObject" ] = &DEC_ObjectFunctions::CreateDynamicGenObject;

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
    brain[ "DEC_ConnaissanceObjet_EstContourne" ] = &DEC_KnowledgeObjectFunctions::IsBypassed;
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
    brain[ "DEC_ConnaissanceObjet_BurningLevel" ] = &DEC_KnowledgeObjectFunctions::GetBurningLevel;

    // Urban knowledges accessors
    brain[ "DEC_NiveauDeProtectionMaterielComposantes" ] = &DEC_KnowledgeAgentFunctions::GetMaterialComposantesProtectionLevel;
    brain[ "DEC_GetAgentIdFromKnowledge" ] = &DEC_KnowledgeAgentFunctions::GetAgentIdFromKnowledge;

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

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::RegisterUserFunctions
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::RegisterUserFunctions( directia::brain::Brain& brain )
{
    directia::tools::binders::ScriptRef initQueryFunction = brain[ "InitQueryReturn" ];

    // Missions
    brain[ "DEC_CreerMissionPion" ] =
        boost::function< boost::shared_ptr< MIL_Mission_ABC >( const std::string& ) >( boost::bind( &DEC_OrdersFunctions::CreatePionMissionBM, this, _1 ) );
    brain[ "DEC_CreerMissionPionVersPion" ]=
        boost::function< boost::shared_ptr< MIL_Mission_ABC >( const std::string& ) >( boost::bind( &DEC_OrdersFunctions::CreatePionMissionVersPionBM, this, _1 ) );
    brain[ "DEC_DonnerMissionPion" ] =
        boost::function< void( boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_OrdersFunctions::CDT_GiveMission, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_DonnerMissionPionVersPion" ] =
        boost::function< void( boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_OrdersFunctions::CDT_GiveMissionVersPion, boost::ref( GetPion() ), _1 ) );

    // Actions
    brain[ "DEC__StopAction" ] =
        boost::function< unsigned int( unsigned int) >( boost::bind( &DEC_ActionFunctions::StopAction< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_PauseAction" ] =
        boost::function< void( unsigned int ) >( boost::bind( &DEC_ActionFunctions::SuspendAction< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_ReprendAction" ] =
        boost::function< void( unsigned int ) >( boost::bind( &DEC_ActionFunctions::ResumeAction < MIL_AgentPion >, boost::ref( GetPion() ), _1 ) );

    brain[ "DEC_StartDeplacement" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Path_ABC > ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionMove, boost::shared_ptr< DEC_Path_ABC > >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC__StartTirSurPopulation" ] =
        boost::function< unsigned int( unsigned int, const std::string& ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePopulation, unsigned int, const std::string& >, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_StartTirDirect" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent >, double, int, int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePion, boost::shared_ptr< DEC_Knowledge_Agent >, double, int, int >, boost::ref( GetPion() ), _1, _2, _3, _4 ) );
    brain[ "DEC_StartTirDirectDebarques" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent >, double, int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePionUsingOnlyLoadable, boost::shared_ptr< DEC_Knowledge_Agent >, double, int >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_StartTirDirectTransporteurs" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent >, double, int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionDirectFirePionUsingOnlyCarrier, boost::shared_ptr< DEC_Knowledge_Agent >, double, int >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC__StartTirDirectSurComposantesMajeures" ] =
        boost::function< unsigned int( int, boost::shared_ptr< DEC_Knowledge_Agent >, double, int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionDirectFirePionOnMajorComposantes, int, boost::shared_ptr< DEC_Knowledge_Agent >, double, int >, boost::ref( GetPion() ), _1, _2, _3, _4 ) );
    brain[ "DEC_StartTirIndirectSurPosition" ] =
        boost::function< unsigned int ( const PHY_DotationCategory*, float, MT_Vector2D* ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIndirectFire_Position, const PHY_DotationCategory*, float, MT_Vector2D* >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_StartTirIndirectSurConnaissance" ] =
        boost::function< unsigned int( const PHY_DotationCategory*, float, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIndirectFire_Knowledge, const PHY_DotationCategory*, float, unsigned int >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_StartTirIndirectSurConnaissancePtr" ] =
        boost::function< unsigned int( const PHY_DotationCategory*, float, boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIndirectFire_Knowledge, const PHY_DotationCategory*, float, boost::shared_ptr< DEC_Knowledge_Agent > >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_StartCreateObject" ] =
        boost::function< unsigned int( const std::string&, const TER_Localisation*, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionConstructObject, const std::string&, const TER_Localisation*, unsigned int >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_StartPrepareObject" ] =
        boost::function< unsigned int( const std::string&, const TER_Localisation* ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionPrepareObject, const std::string&, const TER_Localisation* >, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_StartDetruireObjet" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionDestroyObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_StartValoriserObjet" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionMineObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Agent_ADotationPourConstruireObjet" ] =
        boost::function< bool ( const std::string& ) >( boost::bind( &DEC_AgentFunctions::HasDotationForBuilding, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_StartCreerContournementObjet" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionBypassObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC__StartOccuperObjet" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionOccupyObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_StartReprendreTravauxObjet" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionResumeWorkObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_ReparerBlocUrbain" ] =
        boost::function< unsigned int( UrbanObjectWrapper* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionResumeWorkUrbanBlock, UrbanObjectWrapper* >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC__StartAnimerObjet" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionAnimateObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_StartDistributionObjet" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object >, double ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionDistributeObject, boost::shared_ptr< DEC_Knowledge_Object >, double >, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_StartSupplyObject" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >&, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionSupplyObject, boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >&, double >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_StartExtractFromStockObject" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >&, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionExtractFromStockObject, boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >&, double >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_Stock_IsExtractPossible" ] = boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >& ) >( boost::bind( &DEC_ActionFunctions::Stock_IsExtractPossible, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Stock_IsSupplyPossible" ] = boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >& ) >( boost::bind( &DEC_ActionFunctions::Stock_IsSupplyPossible, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Stock_IsDistributePossible" ] = boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object >, boost::shared_ptr< DEC_Knowledge_Population > ) >( boost::bind( &DEC_ActionFunctions::Stock_IsDistributePossible, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC__StartControlerZone" ] =
        boost::function< unsigned int( const TER_Localisation*, double, bool ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionControlZone, const TER_Localisation*, double, bool >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_StartEmbarquement" ] =
        boost::function< unsigned int() >( boost::bind( &DEC_ActionFunctions::StartAction  < transport::PHY_ActionLoad >, boost::ref( GetPion() ) ) );
    brain[ "DEC_StartDebarquement" ] =
        boost::function< unsigned int() >( boost::bind( &DEC_ActionFunctions::StartAction  < transport::PHY_ActionUnload >, boost::ref( GetPion() ) ) );
    brain[ "DEC_StartIlluminer" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent >, DEC_Decision_ABC* ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIllumination, boost::shared_ptr< DEC_Knowledge_Agent >, DEC_Decision_ABC* >, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Orientate" ] =
        boost::function< void( boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_ActionFunctions::Orientate, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_StartExtinguishObject" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionExtinguishObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_StartInterrogateCrowd" ] =
        boost::function< unsigned int( int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionInterrogate, int >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_StartInterrogateUnit" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionInterrogate, boost::shared_ptr< DEC_Knowledge_Agent > >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_StartTraverserReseauSouterrain" ] =
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionMoveUnderground, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) );

    // Embarquement / debarquement
    brain[ "DEC_Agent_EstEmbarquable" ] =
        boost::bind( &DEC_AgentFunctions::HasLoadable, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_EstEmbarque" ] =
        boost::bind( &DEC_AgentFunctions::IsLoaded, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_EstDebarque" ] =
        boost::bind( &DEC_AgentFunctions::IsUnloaded, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_DureeEmbarquement" ] =
        boost::bind( &DEC_AgentFunctions::GetLoadingTime, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_DureeDebarquement" ] =
        boost::bind( &DEC_AgentFunctions::GetUnloadingTime, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_TransporteursPret" ] =
        boost::bind( &DEC_AgentFunctions::AreHumanTransportersReady, boost::ref( GetPion() ) );
    brain[ "DEC_LaisserTransporteursSansDelai" ] =
        boost::bind( &DEC_AgentFunctions::DisableHumanTransportersNow, boost::ref( GetPion() ) );
    brain[ "DEC_RecupererTransporteursSansDelai" ] =
        boost::bind( &DEC_AgentFunctions::RecoverHumanTransportersNow, boost::ref( GetPion() ) );

    // Objects
    brain[ "DEC_CreerObjetSansDelais" ] =
        boost::function< void( const std::string&, const TER_Localisation* ) > (boost::bind( &DEC_ObjectFunctions::MagicCreateObject < MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) );

    // Path
    brain[ "DEC_CreerItineraire" ] =
        boost::function< boost::shared_ptr< DEC_Path_ABC >( MT_Vector2D*, int ) >( boost::bind( &DEC_PathFunctions::CreatePathToPoint, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_CreerItineraireBM" ] =
        boost::function< boost::shared_ptr< DEC_Path_ABC >( boost::shared_ptr< MT_Vector2D >, int ) >( boost::bind( &DEC_PathFunctions::CreatePathToPointBM, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_ShouldEmbark" ] =
        boost::function< bool( boost::shared_ptr< DEC_Path_ABC > ) >( boost::bind( &DEC_PathFunctions::ShouldEmbark, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_CreerItineraireListe" ] =
        boost::function< boost::shared_ptr< DEC_Path_ABC >( std::vector< boost::shared_ptr< MT_Vector2D > >, int ) >( boost::bind( &DEC_PathFunctions::CreatePathToPointList, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Itineraire_Etat" ] =
        boost::function< int ( DEC_Path_ABC* ) >( boost::bind( &DEC_PathFunctions::GetPathState, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_GetNextObjectOnPath" ] =
        boost::function< std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > >( boost::shared_ptr< DEC_Knowledge_Object >, float, const std::vector< std::string >& ) >(
        boost::bind( &DEC_PathFunctions::GetNextObjectOnPath, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_GetNextRemovableObjectOnPath" ] = &DEC_PathFunctions::GetNextRemovableObjectOnPath;
    brain[ "DEC_Itineraire_ExtrapolerPosition"  ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( const double, bool ) >( boost::bind( &DEC_PathFunctions::ExtrapolatePosition, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Itineraire_DernierPoint" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( const DEC_Path_ABC* ) >( boost::bind( &DEC_PathFunctions::GetLastPointOfPath, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Itineraire_EstEnMouvementSur" ] =
        boost::function< bool ( const DEC_Path_ABC* ) >( boost::bind( &DEC_PathFunctions::IsMovingOnPath, boost::ref( GetPion() ), _1 ) );

    // Perception
    brain[ "DEC_Perception_ActiverCoupsDeSonde" ] =
        boost::bind( &DEC_PerceptionFunctions::EnableCoupDeSonde, boost::ref( GetPion() ) );
    brain[ "DEC_Perception_DesactiverCoupsDeSonde" ] =
        boost::bind( &DEC_PerceptionFunctions::DisableCoupDeSonde, boost::ref( GetPion() ) );
    brain[ "DEC_Perception_PointEstVisible" ] =
        boost::function< bool( MT_Vector2D* ) >( boost::bind( &DEC_PerceptionFunctions::IsPointVisible, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Perception_VisionVerrouilleeSurDirection" ] =
        boost::function< void( boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_PerceptionFunctions::SetVisionModeDirection, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Perception_VisionVerrouilleeSurPoint" ] =
        boost::function< void( const MT_Vector2D* ) >( boost::bind( &DEC_PerceptionFunctions::SetVisionModePoint, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Perception_VisionVerrouilleeSurPointPtr" ] =
        boost::function< void( boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_PerceptionFunctions::SetVisionModePointPtr, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Perception_VisionNormale" ] =
        boost::bind( &DEC_PerceptionFunctions::SetVisionModeNormal, boost::ref( GetPion() ) );
    brain[ "DEC_Perception_ActiverReconnaissanceLocalisation" ] =
        boost::function< int( const TER_Localisation* ) >( boost::bind( &DEC_PerceptionFunctions::EnableRecognitionLocalisation, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Perception_ActivateLocationProgressiveRecce" ] =
        boost::function< int( const TER_Localisation*, float ) >( boost::bind( &DEC_PerceptionFunctions::EnableRecognitionLocalisation, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Perception_DesactiverReconnaissanceLocalisation" ] =
        boost::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableRecognitionLocalisation, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Perception_ActiverReconnaissanceDansBlocUrbain" ] =
        boost::function< int( UrbanObjectWrapper* ) >( boost::bind( &DEC_PerceptionFunctions::EnableRecognitionUrbanBlock, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Perception_DesactiverReconnaissanceDansBlocUrbain" ] =
        boost::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableRecognitionUrbanBlock, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Perception_ActiverReconnaissancePoint" ] =
        boost::function< int( MT_Vector2D*, double, double ) >( boost::bind( &DEC_PerceptionFunctions::EnableRecognitionPoint, boost::ref( *(DEC_Decision_ABC*)this ), _1, _2, _3 ) );
    brain[ "DEC_Perception_DesactiverReconnaissancePoint" ] =
        boost::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableRecognitionPoint, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Perception_ActiverRadar" ] =
        boost::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::EnableRadar, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Perception_DesactiverRadar" ] =
        boost::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableRadar, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Perception_ActiverRadarSurPointPtr" ] =
        boost::function< int( int, boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_PerceptionFunctions::EnableRadarOnPointPtr, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Perception_ActiverRadarSurLocalisation" ] =
        boost::function< int( int, const TER_Localisation* ) >( boost::bind( &DEC_PerceptionFunctions::EnableRadarOnLocalisation, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Perception_DesactiverRadarSurLocalisation" ] =
        boost::function< void( int, int ) >( boost::bind( &DEC_PerceptionFunctions::DisableRadarOnLocalisation, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Perception_ActiverDetectionObjetLocalisation" ] =
        boost::function< int( const TER_Localisation*, const MT_Vector2D*, double ) >( boost::bind( &DEC_PerceptionFunctions::EnableObjectRecognitionLocalisation, boost::ref( *(DEC_Decision_ABC*)this ), _1, _2, _3 ) );
    brain[ "DEC_Perception_DesactiverDetectionObjetLocalisation" ] =
        boost::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableObjectRecognitionLocalisation, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Perception_ActiverPerceptionTirsIndirect" ] =
        boost::function< int( const TER_Localisation* ) >( boost::bind( &DEC_PerceptionFunctions::EnableFlyingShellDetection, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Perception_DesactiverPerceptionTirsIndirect" ] =
        boost::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableFlyingShellDetection, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Perception_EstFurtif" ] =
        boost::function< bool( void ) >( boost::bind( &DEC_PerceptionFunctions::IsStealth, boost::ref( GetPion() ) ) );
    brain[ "DEC_Perception_Furtivite" ] =
        boost::function< void( float ) >( boost::bind( &DEC_PerceptionFunctions::SetStealthFactor, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Perception_DesactiverSenseurs" ] =
        boost::bind( &DEC_PerceptionFunctions::DisableSensors, boost::ref( GetPion() ) );
    brain[ "DEC_Perception_ActiverSenseurs" ] =
        boost::bind( &DEC_PerceptionFunctions::EnableSensors, boost::ref( GetPion() ) );
    brain[ "DEC_Identification_DistanceMaxCompMajeure" ] =
        boost::bind( &DEC_AgentFunctions::GetIdentificationDistance, boost::ref( GetPion() ) );
    brain[ "DEC_Reconnoissance_MajorComponentMinDistance" ] =
        boost::bind( &DEC_AgentFunctions::GetReconnoissanceDistance, boost::ref( GetPion() ) );
    brain[ "DEC_Detection_Distance" ] =
        boost::bind( &DEC_AgentFunctions::GetDetectionDistance, boost::ref( GetPion() ) );
    brain[ "DEC_Connaissances_IdentifierToutesUnitesDansZone" ] =
        boost::function< void( const TER_Localisation* ) >( boost::bind( &DEC_AgentFunctions::IdentifyAllAgentsInZone, boost::ref( GetPion() ), _1 ) );

    // Gestion des renforts
    brain[ "DEC_Renforts" ] =
        boost::function< std::vector<DEC_Decision_ABC*> () >( boost::bind( &DEC_MiscFunctions::GetReinforcements, boost::ref( GetPion() ) ) );
    brain[ "DEC_Renforce" ] =
        boost::function< bool ( const DEC_Decision_ABC* ) >(boost::bind( &DEC_MiscFunctions::Reinforce, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_AnnuleRenforcement" ] =
        boost::bind( &DEC_MiscFunctions::CancelReinforcement, boost::ref( GetPion() ) );

    // Tests / Debug
    brain[ "DEC_DebugAffichePoint"   ] =
        boost::function< void ( const MT_Vector2D* ) > (boost::bind( &DEC_MiscFunctions::DebugDrawPoint< MIL_AgentPion >, boost::ref( GetPion()), _1  ) );
    brain[ "DEC_DebugAffichePoints"  ] =
        boost::function< void ( std::vector< boost::shared_ptr< MT_Vector2D > > ) > (boost::bind( &DEC_MiscFunctions::DebugDrawPoints< MIL_AgentPion >, boost::ref( GetPion()), _1  ) );
    brain[ "DEC_Debug" ] =
        boost::function < void ( const std::string& ) > ( boost::bind( &DEC_MiscFunctions::Debug< MIL_AgentPion > , boost::ref( GetPion()) , "Agent" , _1  ) );
    brain[ "DEC_Trace" ] =
        boost::function< void ( const std::string& ) >( boost::bind( &DEC_MiscFunctions::Trace< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) );

    brain[ "DEC_DecisionalState" ] =
        boost::function< void ( const std::string&, const std::string& ) >( boost::bind( &DEC_AgentFunctions::DecisionalState, boost::ref( GetPion() ), _1, _2 ) );

    // Installation
    brain[ "DEC_Agent_EstInstalle" ] = boost::bind( &DEC_AgentFunctions::IsInstalled, boost::cref( GetPion() ) );
    brain[ "DEC_Agent_EstDesinstalle" ] = boost::bind( &DEC_AgentFunctions::IsUninstalled, boost::cref( GetPion() ) );
    brain[ "DEC_Agent_SInstaller" ] = boost::bind( &DEC_AgentFunctions::Install, boost::ref( GetPion() ) );

    // Old deployment
    brain[ "DEC_Agent_GetTempsDeploiement" ] = &DEC_AgentFunctions::GetInstallationTime;     // $$$$ ABR 2011-12-15: Old method, should be removed soon
    brain[ "DEC_Agent_GetTempsDedeploiement" ] = &DEC_AgentFunctions::GetUninstallationTime; // $$$$ ABR 2011-12-15: Old method, should be removed soon
    // Deployment
    brain[ "DEC_Agent_IsDeployed" ] = boost::bind( &DEC_AgentFunctions::IsDeployed, boost::cref( GetPion() ) );
    brain[ "DEC_Agent_IsUndeployed" ] = boost::bind( &DEC_AgentFunctions::IsUndeployed, boost::cref( GetPion() ) );
    brain[ "DEC_Agent_Deploy" ] = boost::bind( &DEC_AgentFunctions::Deploy, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_Undeploye" ] = boost::bind( &DEC_AgentFunctions::Undeploy, boost::ref( GetPion() ) );

    // Pion accessors
    brain[ "DEC_Agent_EstPC" ] = boost::bind( &DEC_AgentFunctions::IsPC, boost::cref( GetPion() ) );
    brain[ "DEC_Agent_EstTransporte" ] = boost::bind( &DEC_AgentFunctions::IsTransported, boost::cref( GetPion() ) );
    brain[ "DEC_Agent_EstEnVol" ] = boost::bind( &DEC_AgentFunctions::IsFlying, boost::cref( GetPion() ) );
    brain[ "DEC_Agent_HauteurDeVol" ] =
        boost::function< void ( double ) >( boost::bind( &DEC_AgentFunctions::SetFlyingHeight, boost::ref( GetPion() ), _1 ) );

    brain[ "DEC_Agent_EnVille" ] = boost::bind( &DEC_AgentFunctions::IsInCity, boost::cref( GetPion() ) );
    brain[ "DEC_Agent_EtatOps" ] = boost::bind( &DEC_AgentFunctions::GetOperationalState, boost::cref( GetPion() ) );
    brain[ "DEC_Agent_EtatOpsMajeur" ] = boost::bind( &DEC_AgentFunctions::GetMajorOperationalState, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_PeutConstruireObjet" ] =
        boost::function< bool ( const std::string& ) >( boost::bind( &DEC_AgentFunctions::CanConstructObject, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_Agent_PeutConstruireObjetSansRenforts" ] =
        boost::function< bool ( const std::string& ) >( boost::bind( &DEC_AgentFunctions::CanConstructWithoutReinforcement, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_Agent_PeutConstruireObjetEmbarque" ] =
        boost::function< bool ( const std::string& ) >( boost::bind( &DEC_AgentFunctions::CanConstructObjectWithLoaded, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_Agent_PeutConstruireContournementObjet" ] =
        boost::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::CanBypassObject, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_Agent_PeutDetruireObjet" ] =
        boost::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::CanDestroyObject, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_Agent_PeutValoriserObjet" ] =
        boost::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::CanMineObject, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_Agent_ActiverModeDiscret" ] = boost::bind( &DEC_AgentFunctions::EnableDiscreteMode, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_DesactiverModeDiscret" ] = boost::bind( &DEC_AgentFunctions::DisableDiscreteMode, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_EstNeutralise" ] = boost::bind( &DEC_AgentFunctions::IsNeutralized, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_AutomateEstEmbraye" ] = boost::bind( &DEC_AgentFunctions::IsAutomateEngaged, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_Position" ] = boost::bind( &DEC_AgentFunctions::GetPosition, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_Direction" ] = boost::bind( &DEC_AgentFunctions::GetDirection, boost::cref( GetPion() ) );
    brain[ "DEC_Agent_EstMort" ] = boost::bind( &DEC_AgentFunctions::IsDead, boost::cref( GetPion() ) );
    brain[ "DEC_Agent_NiveauInstallation" ] = boost::bind( &DEC_AgentFunctions::GetPosture, boost::cref( GetPion() ) );
    brain[ "DEC_Agent_RoePopulation" ] = boost::bind( &DEC_AgentFunctions::GetRoePopulation, boost::cref( GetPion() ) );
    brain[ "DEC_Agent_EstDansFoule" ] = boost::bind( &DEC_AgentFunctions::IsInCrowd, boost::cref( GetPion() ) );
    brain[ "DEC_HasDotation" ] =
        boost::function< bool ( const PHY_DotationCategory* ) >( boost::bind( &DEC_AgentFunctions::HasDotation, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_CanUseDotation" ] =
        boost::function< bool ( const PHY_DotationCategory* ) >( boost::bind( &DEC_AgentFunctions::CanUseDotation, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Agent_IlluminateRange" ] = boost::bind( &DEC_AgentFunctions::GetIlluminatingRange, boost::cref( GetPion() ) );


    brain[ "DEC_Agent_AutonomieEnDeplacement" ] = boost::bind( &DEC_AgentFunctions::TimeLeftForMoving, boost::cref( GetPion() ) );
    brain[ "DEC_Agent_TempsPourParcourirDistanceEnLigneDroite" ] =
        boost::function< float ( float ) >( boost::bind( &DEC_AgentFunctions::TimeToMoveDistance, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_Agent_TempsPourDegagerUnObjet" ] =
        boost::function< float ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::EstimatedWorkTime, boost::ref( GetPion() ), _1 ) );


    // NBC
    brain[ "DEC_Agent_EstContamine" ] = boost::bind( &DEC_AgentFunctions::IsContaminated , boost::cref( GetPion() ) );
    brain[ "DEC_Agent_MettreTenueProtectionNBC" ] = boost::bind( &DEC_AgentFunctions::WearNbcProtectionSuit, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_EnleverTenueProtectionNBC" ] = boost::bind( &DEC_AgentFunctions::RemoveNbcProtectionSuit, boost::ref( GetPion() ) );
    brain[ "DEC_ConnaissanceObjet_DemandeDeDecontamination" ] =
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::QueueForDecontamination, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Agent_ImmuniserNbc" ] = boost::bind( &DEC_AgentFunctions::ImmunizeAgent, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_StopImmuniserNbc" ] = boost::bind( &DEC_AgentFunctions::StopImmunizeAgent, boost::ref( GetPion() ) );

    // Blackout
    brain[ "DEC_Agent_PasserEnSilenceRadio" ] = boost::bind( &DEC_AgentFunctions::ActivateBlackout, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_PasserEnSilenceRadioPartiel" ] = boost::bind( &DEC_AgentFunctions::ActivatePartialBlackout, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_ArreterSilenceRadio" ] = boost::bind( &DEC_AgentFunctions::DeactivateBlackout, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_ForcerSilenceRadio" ] = &DEC_KnowledgeAgentFunctions::ForceRadioSilence;

    // Misc
    brain[ "DEC_ModulationVitesseCourante" ] =
        boost::function< void( double ) >( boost::bind( &DEC_MiscFunctions::SetCurrentSpeedModificator, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_ModulationVitesseMax" ] =
        boost::function< void( double ) >( boost::bind( &DEC_MiscFunctions::SetMaxSpeedModificator, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_GetModulationVitesseMax" ] = &DEC_MiscFunctions::GetMaxSpeedModificator;
    brain[ "DEC_Agent_GetCurrentSpeed" ] = &DEC_AgentFunctions::GetCurrentSpeed;
    brain[ "DEC_Elongation"      ] =
        boost::function< void( double ) >( boost::bind( &DEC_AgentFunctions::SetElongationFactor, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_ReleverPion"     ] =
        boost::function< bool( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::RelievePion, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_PeutReleverPion" ] =
        boost::function< bool( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::CanRelievePion, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Suicide" ] = boost::bind( &DEC_AgentFunctions::Suicide, boost::ref( GetPion() ) );

    // Agent knowledge accessors
    brain[ "DEC_ConnaissanceAgent_NiveauDePerceptionCourant" ] =
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetCurrentPerceptionLevel, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissanceAgent_Dangerosite" ] =
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetDangerosity, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissanceAgent_AttritionPotentielle" ] =
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetPotentialAttrition, boost::cref( GetPion() ), _1, _2 ) );
    brain[ "DEC_ConnaissanceAgent_EstPercuParUnite" ] = // i.e. I perceive the agent
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsPerceivedByAgent, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissanceAgent_EstUnEnnemi" ] =
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsAnEnemy, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissanceAgent_EstUnAllie" ] =
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsAFriend, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissanceAgent_PercoitUnite" ] = // i.e. the agent perceives me
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsPerceivingAgent, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissanceAgent_EstAPorteDeCapteurDansCone" ] =
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent >, const MT_Vector2D*, double ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsInDetectionCone, boost::cref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_ConnaissanceAgent_PeutEtreIllumine" ] =
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::CanBeIlluminate, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissanceAgent_PeutIlluminer" ] =
        boost::function< bool() >( boost::bind( &DEC_KnowledgeAgentFunctions::CanIlluminate, boost::cref( GetPion() ) ) );
    brain[ "DEC_ConnaissanceAgent_EnAgent" ] =
        boost::function< DEC_Decision_ABC*( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetAgent, _1 ) );
  
    // Object knowledges accessors
    brain[ "DEC_ConnaissanceObjet_EstUnEnnemi" ] =
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsAnEnemy, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissanceObjet_EstUnAllie" ] =
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsAFriend, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissanceObjet_Reconnaitre" ] =
        boost::function< void( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::Recon, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissanceObjet_PeutEtreOccupe" ] =
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::CanBeOccupied, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissanceObjet_PeutEtreAnime" ] =
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::CanBeAnimated, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissanceObjet_Degrader" ] =
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Object >, float, const PHY_DotationCategory*) >( boost::bind( &DEC_KnowledgeObjectFunctions::DamageObject, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_ConnaissanceObjet_NiveauDePerceptionCourant" ] =
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::GetCurrentPerceptionLevel, boost::cref( GetPion() ), _1 ) );

    // Population knowledges accessors
    brain[ "DEC_KnowledgePopulation_Domination" ] =
        boost::function< std::pair< float, int > (int) > (boost::bind( &DEC_KnowledgePopulationFunctions::GetDominationState< MIL_AgentPion >, boost::ref( GetPion()), _1 ) );
    brain[ "DEC_ConnaissancePopulation_Attitude" ] =
        boost::function< unsigned int (int) > ( boost::bind( &DEC_KnowledgePopulationFunctions::GetAttitude< MIL_AgentPion >, boost::ref( GetPion()), _1 ) );
    brain[ "DEC_ConnaissancePopulation_Reconnaitre" ] =
        boost::function< void (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::Recon , boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissancePopulation_EstReconnu" ] =
        boost::function< bool(int)> ( boost::bind ( &DEC_KnowledgePopulationFunctions::IsRecon< MIL_AgentPion > , boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissancePopulation_EstPercueParUnite" ] = // i.e. I perceive the pop
        boost::function< bool (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::IsPerceivedByAgent, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissancePopulation_Dangerosite" ] =
        boost::function< float(int)>(boost::bind ( &DEC_KnowledgePopulationFunctions::GetDangerosity , boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissancePopulation_EstDansZone" ] =
        boost::function< bool (int, TER_Localisation*) >(boost::bind( &DEC_KnowledgePopulationFunctions::IsInZone < MIL_AgentPion >, boost::cref( GetPion() ), _1 , _2 ) );
    brain[ "DEC_ConnaissancePopulation_PointPlusProche" ] =
        boost::function< boost::shared_ptr<MT_Vector2D> (int) >(boost::bind( &DEC_KnowledgePopulationFunctions::ClosestPoint, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissancePopulation_PointEnTeteDeFlux" ] =
        boost::function< boost::shared_ptr<MT_Vector2D> (const DEC_Decision_ABC&, int) >(boost::bind( &DEC_KnowledgePopulationFunctions::GetFlowHead, _1, _2 ) );
    brain[ "DEC_ConnaissancePopulation_Securiser" ] =
        boost::function< void (int) >(boost::bind( &DEC_KnowledgePopulationFunctions::Secure, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissancePopulation_PointSecurisation" ] =
        boost::function< boost::shared_ptr<MT_Vector2D> (int) > (boost::bind( &DEC_KnowledgePopulationFunctions::SecuringPoint, boost::cref( GetPion() ), _1  ) );
    brain[ "DEC_ConnaissancePopulation_EstEnnemi" ] =
        boost::function< bool (int) >(boost::bind( &DEC_KnowledgePopulationFunctions::IsEnemy, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissancePopulation_Exterminer" ] =
        boost::function< int (int, float, const PHY_DotationCategory*) >(boost::bind( &DEC_KnowledgePopulationFunctions::Exterminate, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_ConnaissancePopulation_HasFlow" ] =
        boost::function< bool (int) >( boost::bind( &DEC_KnowledgePopulationFunctions::HasFlow, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_GetAdhesionPopulation" ] =
        boost::function< float (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::GetCrowdAffinity , boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_GetAttitudePopulation" ] =
        boost::function< int (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::GetCrowdAttitude , boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_GetConcentrationLaPlusProche" ] = &DEC_KnowledgePopulationFunctions::GetClosestConcentration;
    brain[ "DEC_GetPositionConcentration" ] = &DEC_KnowledgePopulationFunctions::GetConcentrationPosition;
    brain[ "DEC_GetNombrePersonnesDansConcentration" ] = &DEC_KnowledgePopulationFunctions::GetAllHumansInConcentration;
    brain[ "DEC_AssignMissionCrowdParameter" ] = 
        boost::function< void( boost::shared_ptr< MIL_Mission_ABC >, const std::string&, int ) >( boost::bind( &MIL_MissionParameterFactory::SetCrowdKnowledgeParameter, this, _1, _2, _3 ) );

    // Urban knowledges accessors
    brain[ "DEC_Connaissances_BlocUrbain" ] =
        boost::function< void( const directia::tools::binders::ScriptRef& ) >( boost::bind( &DEC_KnowledgeFunctions::GetUrbanBlock, boost::ref( brain ), boost::ref( GetPion() ), initQueryFunction, _1 ) );
    brain[ "DEC_Connaissances_BlocUrbainPourPosition" ] =
        boost::function< UrbanObjectWrapper*( boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_KnowledgeFunctions::GetUrbanBlockForPosition, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Connaissances_BlocUrbainDansCercle" ] =
        boost::function< T_UrbanObjectVector( boost::shared_ptr< MT_Vector2D>, float )>( boost::bind( &DEC_KnowledgeFunctions::GetUrbanBlockInCircle, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Connaissances_BlocUrbainDansZone" ] =
        boost::function< T_UrbanObjectVector( TER_Localisation* )>( boost::bind( &DEC_KnowledgeFunctions::GetUrbanBlockInZone, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissanceUrbanBlock_NiveauDeReconnaissanceCourant" ] = boost::function< float( UrbanObjectWrapper* )>( boost::bind( &DEC_UrbanObjectFunctions::GetCurrentRecceProgress, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissanceBlocUrbain_Traficabilite" ] =
        boost::function< float( UrbanObjectWrapper* ) >( boost::bind( &DEC_UrbanObjectFunctions::GetPathfindCost, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_ConnaissanceBlocUrbain_RapForLocal" ] =
        boost::function< float( UrbanObjectWrapper* ) >( boost::bind( &DEC_UrbanObjectFunctions::GetRapForLocal, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_GetPerception" ] =
        boost::function< double( boost::shared_ptr< MT_Vector2D >, boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_PerceptionFunctions::GetPerception, boost::cref( GetPion() ), _1, _2 ) );
    brain[ "DEC_DetruireBlocUrbain" ] =
        boost::function< void( UrbanObjectWrapper*, const PHY_DotationCategory* )>( boost::bind( &DEC_UrbanObjectFunctions::DestroyUrbanBlock, boost::ref( GetPion() ), _1, _2 ) );

    // Global knowledge
    brain[ "DEC_RapportDeForceLocal" ] = boost::bind( &DEC_KnowledgeFunctions::GetRapForLocal, boost::ref( GetPion() ) );
    brain[ "DEC_Connaissances_UnitesDetecteesDansFuseau" ] = boost::bind( &DEC_KnowledgeFunctions::GetDetectedAgentsInFuseau, boost::ref( GetPion() ) );
    brain[ "DEC_Connaissances_UnitesDetecteesDansZone" ] =
        boost::function< T_ConstKnowledgeAgentVector( const TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetDetectedAgentsInZone, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_Connaissances_UnitesPrenantAPartie" ] = boost::bind( &DEC_KnowledgeFunctions::GetAgentsAttacking, boost::ref( GetPion() ) );
    brain[ "DEC_Connaissances_UnitesEnnemiesDangereuses" ] = boost::bind( &DEC_KnowledgeFunctions::GetDangerousEnemies, boost::ref( GetPion() ) );

    brain[ "DEC_Connaissances_UnitesEnnemiesVivantesPercues" ] = boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesPerceived, boost::ref( GetPion() ) );
    brain[ "DEC_Connaissances_Observable" ] =
        boost::function< void( const directia::tools::binders::ScriptRef& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObservableKnowledge, boost::ref( brain ), boost::ref( GetPion() ), initQueryFunction, _1 ) );
    brain[ "DEC_Connaissances_Destroyable" ] =
        boost::function< void( const directia::tools::binders::ScriptRef& ) >( boost::bind( &DEC_KnowledgeFunctions::GetDestroyableKnowledge, boost::ref( brain ), boost::ref( GetPion() ), initQueryFunction, _1 ) );//@TODO MGD same function than Observable for the moment
    brain[ "DEC_Connaissances_Identifiable" ] =
        boost::function< void( const directia::tools::binders::ScriptRef& ) >( boost::bind( &DEC_KnowledgeFunctions::GetIdentifiableKnowledge, boost::ref( brain ), boost::ref( GetPion() ), initQueryFunction, _1 ) );//@TODO MGD same function than Observable for the moment
    brain[ "DEC_Connaissances_UnitesEnnemiesVivantesPercuesParPion" ] =
        boost::function< T_ConstKnowledgeAgentVector( const DEC_Decision_ABC* ) >( boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_Connaissances_UnitesEnnemiesVivantesPercuesDansFuseau" ] = boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedInFuseau, boost::ref( GetPion() ) );
    brain[ "DEC_Connaissances_UnitesEnnemiesVivantesDansFuseau" ] = boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesInFuseau, boost::ref( GetPion() ) );
    brain[ "DEC_Connaissances_UnitesEnnemiesVivantesDansZone" ] =
        boost::function< T_ConstKnowledgeAgentVector( TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesInZone, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Connaissances_UnitesEnnemiesVivantesDansCercle" ] =
        boost::function< T_ConstKnowledgeAgentVector( const MT_Vector2D*, float ) >( boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesInCircle, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Connaissances_UnitesEnnemiesVivantesDansBlocUrbain" ] =
        boost::function< T_ConstKnowledgeAgentVector( UrbanObjectWrapper* ) >( boost::bind( &DEC_UrbanObjectFunctions::GetLivingEnemiesInBU, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Connaissances_RefugiesAProximite" ] =
        boost::function< T_ConstKnowledgeAgentVector( double ) > ( boost::bind( &DEC_KnowledgeFunctions::GetNearbyRefugees, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Connaissances_TerroristsAProximite" ] =
        boost::function< T_ConstKnowledgeAgentVector( double ) > ( boost::bind( &DEC_KnowledgeFunctions::GetNearbyTerrorists, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Connaissances_UnitesRenduesAProximite" ] =
        boost::function< T_ConstKnowledgeAgentVector ( double ) > ( boost::bind( &DEC_KnowledgeFunctions::GetNearbySurrenderedAgents, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Connaissances_PresenceEnnemieDansCercle" ] =
        boost::function< bool( const MT_Vector2D*, double ) >( boost::bind( &DEC_KnowledgeFunctions::EnemyPresenceInCircle, boost::cref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Connaissances_UnitesAmiesDansZone" ] =
        boost::function< T_ConstKnowledgeAgentVector( const TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetFriendsInZone< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_Connaissances_PartageConnaissancesAvec" ] =
        boost::function< void( DEC_Decision_ABC*, float ) >( boost::bind( &DEC_KnowledgeFunctions::ShareKnowledgesWith< MIL_AgentPion >, boost::cref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Connaissances_PartageConnaissancesAvecConnaissanceAgent" ] = &DEC_KnowledgeAgentFunctions::ShareKnowledgesWith;
    brain[ "DEC_Connaissances_PartageConnaissancesDansZoneAvec" ] =
        boost::function< void( DEC_Decision_ABC*, const MT_Vector2D*, float ) >( boost::bind( &DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith< MIL_AgentPion >, boost::cref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_Knowledges_ObjectsInCircle" ] =
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const MT_Vector2D*, double, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInCircle< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_ObjectKnowledgesInZone" ] =
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const TER_Localisation*, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInZone< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Connaissances_ObjetsDansFuseau" ] =
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const std::string& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInFuseau< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_Connaissances_CollisionsObjets" ] = boost::bind( &DEC_KnowledgeFunctions::GetObjectsColliding, boost::ref( GetPion() ) );
    brain[ "DEC_Connaissances_ObjetLePlusProche" ] =
        boost::function< boost::shared_ptr< DEC_Knowledge_Object >( const std::string& ) >( boost::bind( &DEC_KnowledgeFunctions::GetClosestObject, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_Connaissances_ObjetAmiLePlusProche" ] =
        boost::function< boost::shared_ptr< DEC_Knowledge_Object >( const std::string& ) >( boost::bind( &DEC_KnowledgeFunctions::GetClosestFriendObject, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_Connaissances_CollisionsPopulations" ] = boost::bind( &DEC_KnowledgeFunctions::GetPopulationsColliding, boost::ref( GetPion() ) );
    brain[ "DEC_Connaissances_PopulationsPrenantAPartie" ] = boost::bind( &DEC_KnowledgeFunctions::GetPopulationsAttacking, boost::ref( GetPion() ) );
    brain[ "DEC_Connaissances_EstPrisAPartieParPopulation" ] =
        boost::function< bool( const int ) >( boost::bind( &DEC_KnowledgeFunctions::IsPopulationAttacking, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Connaissances_Populations" ] = boost::bind( &DEC_KnowledgeFunctions::GetPopulations< MIL_AgentPion >, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_ChangerNombreIndividuArmeDansFoule" ] =
        boost::function< void( const int, const double ) >( boost::bind( &DEC_KnowledgePopulationFunctions::ChangeArmedIndividuals, boost::ref( GetPion() ), _1, _2 ) );

    // Ordres de conduite
    brain[ "DEC_ReleverUnite" ] =
        boost::function< bool( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::RelievePion, boost::ref( GetPion() ), _1 ) );

    // Limas / Mission
    brain[ "DEC_NouvelleMission" ] = boost::bind( &DEC_OrdersFunctions::IsNewMissionStarted< MIL_AgentPion >, boost::ref( GetPion() ) );
    brain[ "DEC_FinMission" ] = boost::bind( &DEC_OrdersFunctions::FinishMission< MIL_AgentPion >, boost::ref( GetPion() ) );
    brain[ "DEC_GetLima" ] =
        boost::function< unsigned int( unsigned int ) >( boost::bind( &DEC_OrdersFunctions::GetLima< MIL_Agent_ABC >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_SetMissionLimaFlag"             ] =
        boost::function< void (unsigned int, bool)> ( boost::bind( &DEC_OrdersFunctions::PionSetMissionLimaFlag, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_GetMissionLimaFlag"             ] =
        boost::function< bool (unsigned int)> ( boost::bind( &DEC_OrdersFunctions::GetMissionLimaFlag < MIL_AgentPion >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_SetMissionLimaFlagHoraire"      ] =
        boost::function< void (unsigned int, bool)> ( boost::bind( &DEC_OrdersFunctions::PionSetMissionLimaScheduleFlag, boost::ref( GetPion() ), _1, _2 ) );

    // Etat décisionnel
    brain[ "DEC_Agent_ChangeEtatRapportDeForce" ] =
        boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyForceRatioStateChanged, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Agent_ChangeEtatROE" ] =
        boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyRulesOfEngagementStateChanged, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Agent_GetEtatROE" ] =
        boost::bind( &DEC_AgentFunctions::GetRulesOfEngagementState, boost::ref( GetPion() ) );
    brain[ "DEC_Agent_ChangeEtatROEPopulation" ] =
        boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyRulesOfEngagementPopulationStateChanged, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Agent_ChangeEtatCombatDeRencontre" ] =
        boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyCloseCombatStateChanged, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Agent_ChangeEtatOperationnel" ] =
        boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyOperationalStateChanged, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Agent_ChangeDisponibiliteAuTirIndirect" ] =
        boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyIndirectFireAvailabilityChanged, boost::ref( GetPion() ), _1 ) );

    // Facteurs humains
    brain[ "DEC_FacteurHumain_Fatigue" ] = boost::bind( &DEC_AgentFunctions::GetHumanFactorTiredness, boost::ref( GetPion() ) );
    brain[ "DEC_FacteurHumain_Moral" ] = boost::bind( &DEC_AgentFunctions::GetHumanFactorMorale, boost::ref( GetPion() ) );

    // Calculs de position
    brain[ "DEC_Geometrie_PositionInterception" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent >, MT_Vector2D*, double )>( boost::bind( &DEC_GeometryFunctions::GetInterceptionPosition, boost::cref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_Agent_PositionInterception" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_AgentFunctions::GetInterceptionPoint, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_Geometrie_CalculerPositionAppui" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( DEC_Decision_ABC*, double ) >( boost::bind( &DEC_GeometryFunctions::ComputeSupportPosition, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Geometrie_CalculerPositionEmbuscade" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( MT_Vector2D*, MT_Vector2D*, double ) >( boost::bind( &DEC_GeometryFunctions::ComputeAmbushPosition, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_Geometrie_CalculerPositionSurete" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent >, double ) >( boost::bind( &DEC_GeometryFunctions::ComputeSafetyPosition, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Geometrie_CalculerPositionSureteAvecPopulation" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( unsigned int, double ) >( boost::bind( &DEC_GeometryFunctions::ComputeSafetyPositionWithPopulation, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Geometrie_CalculerPositionSureteAvecObjectif" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent >, double, MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::ComputeSafetyPositionWithObjective, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_Geometrie_CalculerPointProcheLocalisationDansFuseau" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( const TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau< MIL_AgentPion>, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Geometrie_CalculerPointProcheLocalisationNonClippeeDansFuseau"  ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( const TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInFuseau< MIL_AgentPion>, boost::cref( GetPion() ), _1 ) );
    brain[ "DEC_Geometrie_CalculerBarycentreLocalisationDansFuseau" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Geometrie_CalculerEntreeFuseauLaPlusProche" ] =
        boost::bind( &DEC_GeometryFunctions::ComputeNearestFuseauEntryPoint, boost::ref( GetPion() ) );
    brain[ "DEC_Geometry_SplitLocalisation" ] =
        boost::function< std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( TER_Localisation*, unsigned int, MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisationInParts< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_Geometry_SplitLocalisationSurface" ] =
        boost::function< std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( TER_Localisation*, unsigned int, MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisationInSurfaces< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_Geometry_RecursiveSplitLocalisationSurface" ] =
        boost::function< std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( TER_Localisation*, unsigned int ) >( boost::bind( &DEC_GeometryFunctions::RecursiveSplitLocalisationInSurfaces< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Geometrie_DecoupeFuseauEnTroncons" ] =
        boost::function< std::vector< boost::shared_ptr< TER_Localisation > >( const double ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisationInSections< MIL_AgentPion >, boost::ref( GetPion() ), _1  ) );
    brain[ "DEC_Geometrie_CalculerPositionObstacle" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( MT_Vector2D*, const std::string&, double) >( boost::bind( &DEC_GeometryFunctions::ComputeObstaclePosition< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_Geometrie_CalculerPointArrivee" ] =
        boost::bind( &DEC_GeometryFunctions::ComputeDestPoint< MIL_AgentPion >, boost::ref( GetPion() ) );
    brain[ "DEC_Geometrie_CalculerPointDepart" ] =
        boost::bind( &DEC_GeometryFunctions::ComputeStartPoint< MIL_AgentPion >, boost::ref( GetPion() ) );
    brain[ "DEC_Geometrie_EstPointDansFuseau" ] =
        boost::function< bool( MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::IsPointInFuseau< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Geometrie_CalculerBarycentreListeConnaissancesAgents" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > ) >( boost::bind( &DEC_GeometryFunctions::ComputeKnowledgeAgentBarycenter, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Geometrie_CalculerPositionParRapportALima" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( int, float ) >( boost::bind( &DEC_GeometryFunctions::ComputePointBeforeLima< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Geometrie_CalculerTrafficableBarycentreLocalisation" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeTrafficableLocalisationBarycenter, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_IsPointInUrbanBlockTrafficable" ] =
        boost::function< bool( MT_Vector2D& ) >( boost::bind( &DEC_GeometryFunctions::IsPointInUrbanBlockTrafficable, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Geometrie_PositionAdvanceAlongFuseau" ] =
        boost::function< double( MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::ComputePositionAdvanceAlongFuseau, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Geometrie_GetLeavingAreaPosition" ] =
        boost::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::GetLeavingAreaPosition< MIL_AgentPion>, boost::ref( GetPion() ), _1 ) );

    //Keypoint
    brain[ "DEC_Crossroads" ] =
        boost::function< void( const directia::tools::binders::ScriptRef& ) >( boost::bind( &DEC_TerrainFunctions::GetCrossroads, boost::ref( brain ), boost::ref( GetPion() ), initQueryFunction, _1 ) ) ;
    brain[ "DEC_FindSafetyPositions" ] =
        boost::function< std::vector< boost::shared_ptr< MT_Vector2D > >( float, float ) >( boost::bind( &DEC_TerrainFunctions::FindSafetyPositionsWithinCircle, boost::ref( GetPion() ), _1, _2 ) ) ;

    // Fire
    brain[ "DEC_Tir_PorteeMaxPourTirerSurUnite" ] =
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToFireOnEnemy, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Tir_PorteeMaxPourTirerSurUniteAvecMunition" ] =
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float, const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToFireOnEnemyWithDotation, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_Tir_PorteeMinPourTirerSurUnite" ] =
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMinRangeToFireOnEnemy, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Tir_PorteeMaxPourTirerSurUnitePosturesReelles" ] =
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToFireOnEnemyActualPosture, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Tir_PorteeMinPourTirerSurUnitePosturesReelles" ] =
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMinRangeToFireOnEnemyActualPosture, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Tir_PorteeMaxPourTirerDebarqueSurUnite" ] =
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToFireOnEnemyWhenUnloaded, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Tir_PorteeMaxPourEtreTireParUnite" ] =
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToBeFiredByEnemy, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Tir_PorteeMaxPourTirer" ] =
        boost::function< float( float ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToFire, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Tir_PorteeMaxTirIndirect" ] =
        boost::function< float( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToIndirectFire, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Tir_PorteeMinTirIndirect" ] =
        boost::function< float( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetMinRangeToIndirectFire, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Tir_PorteeMaxTirIndirect_SansTesterEtatMunitions" ] =
        boost::function< float( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToIndirectFireWithoutAmmoCheck, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Tir_PorteeMinTirIndirect_SansTesterEtatMunitions" ] =
        boost::function< float( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetMinRangeToIndirectFireWithoutAmmoCheck, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Tir_LancerFumigeneSurConnaissance" ] =
        boost::function< void( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_FireFunctions::ThrowSmokeOnKnowledgeAgent, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Tir_MunitionPourTirIndirect" ] =
        boost::function< const PHY_DotationCategory* ( int, const MT_Vector2D* ) >( boost::bind( &DEC_FireFunctions::GetMunitionForIndirectFire, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Pion_InterdireMunition" ] =
        boost::function< void( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::ForbidAmmunition, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Pion_AutoriserMunition" ] =
        boost::function< void( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::AllowAmmunition, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Pion_AutoriserToutesMunitions" ] =
        boost::bind( &DEC_FireFunctions::AllowAllAmmunitions,        boost::ref( GetPion() ) );

    // Hospitals
    brain[ "DEC_DeterminerHopital" ] =
        boost::function< boost::shared_ptr< DEC_Knowledge_Object > ( const DEC_Decision_ABC*, T_KnowledgeObjectDiaIDVector ) >( boost::bind( &DEC_MedicalTreatmentFunctions::DetermineHospital, boost::cref( GetPion() ), _1, _2 ) );
    brain[ "DEC_CanHospitalTreatWound" ] =
        boost::function< bool ( const DEC_Decision_ABC*, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_MedicalTreatmentFunctions::CanHospitalTreatWound, boost::cref( GetPion() ), _1, _2 ) );
    brain[ "DEC_DebarqueBlesseDansHopital" ] =
        boost::function< void ( const DEC_Decision_ABC*, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_MedicalTreatmentFunctions::TakeCareOfThePatient, boost::cref( GetPion() ), _1, _2 ) );

    // Hierarchie
    brain[ "DEC_Pion_PionsAvecPC" ] = boost::bind( &DEC_AgentFunctions::GetPionsWithPC, boost::ref( GetPion() ) );
    brain[ "DEC_Pion_PionsSansPC" ] = boost::bind( &DEC_AgentFunctions::GetPionsWithoutPC, boost::ref( GetPion() ) );
    brain[ "DEC_Pion_PionPC" ] = boost::bind( &DEC_AgentFunctions::GetPionPC, boost::cref( GetPion() ) );
    brain[ "DEC_Pion_ChangeAutomate" ] =
        boost::function< bool( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::ChangeAutomate, boost::ref( GetPion() ), _1 ) );

    // Logistique
    brain[ "DEC_StartPreterVSRAM" ] =
        boost::function< unsigned int( DEC_Decision_ABC*, DEC_Decision_ABC*, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionLendCollectionComposantes, DEC_Decision_ABC*, DEC_Decision_ABC*, unsigned int >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_StartPreterRemorqueurs" ] =
        boost::function< unsigned int( DEC_Decision_ABC*, DEC_Decision_ABC*, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionLendHaulerComposantes, DEC_Decision_ABC*, DEC_Decision_ABC*, unsigned int >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_RecupererVSRAM" ] =
        boost::function< void( const DEC_Decision_ABC*, const unsigned int ) >( boost::bind( &DEC_LogisticFunctions::UndoLendCollectionComposantes, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_RecupererRemorqueurs" ] =
        boost::function< void( const DEC_Decision_ABC*, const unsigned int ) >( boost::bind( &DEC_LogisticFunctions::UndoLendHaulerComposantes, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Pion_PcDeTC2" ] =
        boost::bind( &DEC_LogisticFunctions::PionGetPcTC2, boost::ref( GetPion() ) );
    brain[ "DEC_ChangeValeurDotations2" ] =
        boost::function< void( int, const double ) >( boost::bind( &DEC_LogisticFunctions::ChangeDotationsValueUsingTC2, boost::ref( GetPion() ), _1, _2, -1 ) );
    brain[ "DEC_ChangeValeurDotations3" ] =
        boost::function< void( int, const double, int ) >( boost::bind( &DEC_LogisticFunctions::ChangeDotationsValueUsingTC2, boost::ref( GetPion() ), _1, _2, _3 ) );

    // Transport / Heliportage
    brain[ "DEC_Transport_AjouterPion" ] =
        boost::function< void( DEC_Decision_ABC*, bool ) >( boost::bind( &DEC_ActionFunctions::Transport_AddPion, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Transport_AjouterPions" ] =
        boost::function< void( const std::vector< DEC_Decision_ABC* >&, bool ) >( boost::bind( &DEC_ActionFunctions::Transport_AddPions, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Start_TransportEmbarquer" ] = boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTransportLoad >, boost::ref( GetPion() ) );
    brain[ "DEC_Start_TransportDebarquer" ] = 
        boost::function< unsigned int( MT_Vector2D* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTransportUnload, MT_Vector2D* >, boost::ref( GetPion() ), _1 ) );

    brain[ "DEC_Transport_EmbarquerDansTransporteurSansDelais" ] = boost::function< void ( const DEC_Decision_ABC* ) >( boost::bind( &DEC_ActionFunctions::Transport_MagicLoadPionInCarrier, boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Transport_DebarquerDeTransporteurSansDelais" ] = boost::function< void () >( boost::bind( &DEC_ActionFunctions::Transport_MagicUnloadPionFromCarrier, boost::ref( GetPion() ) ) );
    brain[ "DEC_Transport_Transporteur" ] = boost::function< DEC_Decision_ABC* () >( boost::bind( &DEC_ActionFunctions::Transport_GetCarrier, boost::ref( GetPion() ) ) );

    brain[ "DEC_Transport_EmbarquerPionSansDelais" ] =
        boost::function< void( const DEC_Decision_ABC*, bool ) >( boost::bind( &DEC_ActionFunctions::Transport_MagicLoadPion, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Transport_EmbarquerPionsSansDelais" ] =
        boost::function< void (const std::vector< DEC_Decision_ABC* >&, bool) >( boost::bind( &DEC_ActionFunctions::Transport_MagicLoadPions, boost::ref( GetPion()) , _1, _2 ) );
    brain[ "DEC_Transport_DebarquerPionSansDelais" ] =
        boost::function<void(const DEC_Decision_ABC*)>( boost::bind( &DEC_ActionFunctions::Transport_MagicUnloadPion , boost::ref( GetPion()) , _1 ) );
    brain[ "DEC_Transport_DebarquerPionsSansDelais" ] =
        boost::function<void(const std::vector< DEC_Decision_ABC* >&)>( boost::bind( &DEC_ActionFunctions::Transport_MagicUnloadPions , boost::ref( GetPion()) , _1 ) );
    brain[ "DEC_Transport_EstTermine" ] =
        boost::bind( &DEC_ActionFunctions::Transport_IsFinished , boost::ref( GetPion()) );
    brain[ "DEC_Transport_Annuler" ] =
        boost::bind( &DEC_ActionFunctions::Transport_Cancel , boost::ref( GetPion()) );
    brain[ "DEC_Transport_EnCoursDeTransport" ] =
        boost::bind( &DEC_ActionFunctions::Transport_IsTransporting, boost::ref( GetPion() ) );

    brain[ "DEC_Agent_PeutTransporterPion" ] =
        boost::function< bool( const DEC_Decision_ABC*, bool ) >( boost::bind( &DEC_ActionFunctions::CanTransportPion, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Connaissance_PeutTransporterPion" ] = &DEC_ActionFunctions::CanTransportKnowledge;

    brain[ "DEC_Agent_PeutTransporterFoule" ] = &DEC_ActionFunctions::CanTransportCrowd;
    brain[ "DEC_Agent_TransporteFoule" ] = &DEC_ActionFunctions::IsTransportingCrowd;
    brain[ "DEC_StartEmbarquerFouleDUneConcentration" ] =
        boost::function< unsigned int( int, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< crowdtransport::PHY_ActionLoadCrowd, int, unsigned int >, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_StartDebarquerFouleSurPosition" ] =
        boost::function< unsigned int( int, MT_Vector2D* ) >( boost::bind( &DEC_ActionFunctions::StartAction< crowdtransport::PHY_ActionUnloadCrowd, int, MT_Vector2D* >, boost::ref( GetPion() ), _1, _2 ) );

    // Transport connaissance
    brain[ "DEC_Connaissance_Transporter" ] = &DEC_ActionFunctions::Knowledge_Load;
    brain[ "DEC_TransportConnaissance_AjouterPion" ] = &DEC_ActionFunctions::TransportKnowledge_AddPion;

    // Prisonniers
    brain[ "DEC_Prisonniers_CapturerEtEmbarquer"  ] =
        boost::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > )>(boost::bind(&DEC_ActionFunctions::Prisoners_CaptureAndLoad , boost::ref( GetPion()) ,_1 ) );
    brain[ "DEC_Prisonniers_Debarquer"            ] =
        boost::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > )>(boost::bind(&DEC_ActionFunctions::Prisoners_Unload , boost::ref(GetPion()), _1 ) );
    brain[ "DEC_Prisonniers_DebarquerDansCamp"    ] =
        boost::function< void (boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< DEC_Knowledge_Object > )>(boost::bind(&DEC_ActionFunctions::Prisoners_UnloadInCamp , boost::ref( GetPion() ), _1, _2  ) );
    brain[ "DEC_Prisonniers_EstEmbarque"          ] =
        boost::function< bool (boost::shared_ptr< DEC_Knowledge_Agent >) >(boost::bind(&DEC_ActionFunctions::PrisonnersRefugees_IsLoaded , boost::ref( GetPion() ), _1  ) );

    // Refugies
    brain[ "DEC_Refugies_OrienterEtEmbarquer"  ] =
        boost::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > ) > ( boost::bind(&DEC_ActionFunctions::Refugees_OrientateAndLoad , boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Refugies_Debarquer"            ] =
        boost::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > ) > ( boost::bind(&DEC_ActionFunctions::Refugees_Unload , boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_Refugies_DebarquerDansCamp"    ] =
        boost::function< void ( boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< DEC_Knowledge_Object > )> ( boost::bind(&DEC_ActionFunctions::Refugees_UnloadInCamp , boost::ref( GetPion() ), _1, _2  ) );
    brain[ "DEC_Refugies_EstEmbarque"          ] =
        boost::function< bool ( boost::shared_ptr< DEC_Knowledge_Agent > ) > ( boost::bind ( &DEC_ActionFunctions::PrisonnersRefugees_IsLoaded , boost::ref( GetPion() ), _1  ) );

    // Emergency functions
    brain[ "DEC_Start_EmergencyInfluence" ] =
        boost::function< unsigned int( const std::string&, double ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionInfluence, const std::string&, double >, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Start_EmergencyTriggerActivity" ] =
        boost::function< unsigned int( const std::string&, double ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTriggerActivity, const std::string&, double >, boost::ref( GetPion() ), _1, _2 ) );
    brain[ "DEC_Start_EmergencyInfluenceInArea" ] =
        boost::function< unsigned int( const std::string&, double, const TER_Localisation* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionInfluenceInArea, const std::string&, double, const TER_Localisation* >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_Start_EmergencyTriggerActivityInArea" ] =
        boost::function< unsigned int( const std::string&, double, const TER_Localisation* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTriggerActivityInArea, const std::string&, double, const TER_Localisation* >, boost::ref( GetPion() ), _1, _2, _3 ) );
    brain[ "DEC_Start_EmergencyUnloadActivity" ] =
        boost::function< unsigned int( const std::string&, double ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionUnloadActivity, const std::string&, double >, boost::ref( GetPion() ), _1, _2 ) );

    // Representations
    brain[ "DEC_GetOrdersCategory" ] =
        boost::bind( &DEC_MiscFunctions::GetOrdersCategory , boost::ref( *pEntity_ ) );
    brain[ "DEC_GetPointsCategory" ] =
        boost::bind( &DEC_MiscFunctions::GetPointsCategory , boost::ref( *pEntity_ ) );
    brain[ "DEC_RemoveFromOrdersCategory" ] =
        boost::function< void ( boost::shared_ptr< MIL_FragOrder > ) > ( boost::bind( &DEC_MiscFunctions::RemoveFromOrdersCategory , boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_DeleteRepresentation" ] =
        boost::function< void ( boost::shared_ptr< MIL_FragOrder > ) > ( boost::bind( &DEC_MiscFunctions::DeleteOrderRepresentation , boost::ref( GetPion() ), _1 ) );
    brain[ "DEC_RemoveFromPointsCategory"  ] =
        boost::function< void( boost::shared_ptr< DEC_PathPoint > )>( boost::bind( &DEC_MiscFunctions::RemoveFromPointsCategory, boost::ref( *pEntity_ ), _1 ) );
    brain[ "DEC_GetSzName" ] = &DEC_MiscFunctions::GetName;
    brain[ "DEC_AddEnemyRepresentation" ] = &DEC_MiscFunctions::AddEnemyRepresentation;
    brain[ "DEC_RemoveEnemyRepresentation" ] = &DEC_MiscFunctions::RemoveEnemyRepresentation;
    brain[ "DEC_GetEnemyRepresentations" ] = &DEC_MiscFunctions::GetEnemyRepresentation;

    pEntity_->GetType().RegisterFunctions( brain, GetPion() );
    //
    // Critical Intelligence
    brain[ "DEC_ObtenirRenseignementCritiqueSurFoule" ] =
         boost::function< std::string( int ) >(boost::bind( &DEC_KnowledgePopulationFunctions::GetCriticalIntelligence, boost::cref( GetPion() ), _1 ) );
}
