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
#include "Decision/Brain.h"
#include "Decision/DEC_AgentFunctions.h"
#include "Decision/DEC_KnowledgeAgentFunctions.h"
#include "Decision/DEC_KnowledgeObjectFunctions.h"
#include "Decision/DEC_KnowledgePopulationFunctions.h"
#include "Decision/DEC_UrbanObjectFunctions.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Decision/DEC_KnowledgeFunctions.h"
#include "Decision/DEC_PerceptionFunctions.h"
#include "Decision/DEC_MiscFunctions.h"
#include "Decision/DEC_OrdersFunctions.h"
#include "Decision/DEC_ActionFunctions.h"
#include "Decision/DEC_LogisticFunctions.h"
#include "Decision/DEC_ObjectFunctions.h"
#include "Decision/DEC_TerrainFunctions.h"
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
#include "Tools/MIL_Tools.h"
#include "MIL_Time_ABC.h"

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::RegisterUserArchetypeFunctions
// Created: ADY 2010-08-02
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::RegisterUserArchetypeFunctions( sword::Brain& brain )
{
    brain.RegisterFunction( "DEC_ActiverObjet", &DEC_ObjectFunctions::ActivateObject );
    brain.RegisterFunction( "DEC_DetruireObjetSansDelais", &DEC_ObjectFunctions::MagicDestroyObject );
    brain.RegisterFunction( "DEC_DetruireObjetIdSansDelais", &DEC_ObjectFunctions::MagicDestroyObjectId );
    brain.RegisterFunction( "DEC_CreateDynamicGenObject", &DEC_ObjectFunctions::CreateDynamicGenObject );
    brain.RegisterFunction( "DEC_CreateDynamicGenObjectFromSharedLocalisation", &DEC_ObjectFunctions::CreateDynamicGenObjectFromSharedLocalisation );

    // Tests / Debug
    brain.RegisterFunction( "DEC_DebugPointXY", &DEC_MiscFunctions::GetPointXY );

    // Pion accessors
    brain.RegisterFunction( "DEC_HasDotationForFiring", &DEC_AgentFunctions::HasDotationForFiring );
    brain.RegisterFunction( "DEC_Agent_PeutActiverObjet", &DEC_AgentFunctions::CanActivateObject );
    brain.RegisterFunction( "DEC_Agent_PeutIllumine", &DEC_AgentFunctions::CanIlluminate );

    //agent knowledge accessors
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstIllumine", &DEC_KnowledgeAgentFunctions::IsIlluminated );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstDefinitivementIllumine", &DEC_KnowledgeAgentFunctions::IsDefinitivelyIlluminated );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstDansFoule", &DEC_KnowledgeAgentFunctions::IsInCrowd );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_Neutraliser", &DEC_KnowledgeAgentFunctions::NeutralizeAgent );

    // Urban knowledges accessors
    brain.RegisterFunction( "DEC_NiveauDeProtectionMaterielComposantes", &DEC_KnowledgeAgentFunctions::GetMaterialComposantesProtectionLevel );
    brain.RegisterFunction( "DEC_GetAgentIdFromKnowledge", &DEC_KnowledgeAgentFunctions::GetAgentIdFromKnowledge );
    brain.RegisterFunction( "DEC_Connaissances_BlocUrbainDansCercle", &DEC_KnowledgeFunctions::GetUrbanBlockInCircle );

    //limas / Missions
    brain.RegisterFunction( "DEC_GetRawMission", &DEC_AgentFunctions::GetMission );
    brain.RegisterFunction( "DEC_Connaissance_GetRawMission", &DEC_KnowledgeAgentFunctions::GetMission );
    brain.RegisterFunction( "DEC_GetDirectionEnnemi", &DEC_AgentFunctions::GetDirectionDanger );

    //Calculs de positions
    brain.RegisterFunction( "DEC_Geometrie_PionDevant", &DEC_GeometryFunctions::GetFrontestPion );
    brain.RegisterFunction( "DEC_Geometrie_PionDerriere", &DEC_GeometryFunctions::ComputeBackestAgent );
    brain.RegisterFunction( "DEC_Perception_AjouterPointReconnuAutomateParPion", &DEC_PerceptionFunctions::AddReconnoiteredPointBy );
    brain.RegisterFunction( "DEC_Perception_EstPointReconnuParPionOuPersonne", &DEC_PerceptionFunctions::PointIsReconnoiteredByMeOrNoOne );

    // Logistique
    brain.RegisterFunction( "DEC_DemandeDeRavitaillement", &DEC_LogisticFunctions::PionRequestSupply );
    brain.RegisterFunction( "DEC_ChangerSuperieurLogistiqueConnaissance", &DEC_KnowledgeAgentFunctions::SwitchAutomateLogistic );

    // Prisonniers
    brain.RegisterFunction( "DEC_Prisonniers_EstDebarqueDansCamp", &DEC_ActionFunctions::Prisoners_IsUnloadedInCamp );

    // Refugies
    brain.RegisterFunction( "DEC_Refugies_EstDebarqueDansCamp", &DEC_ActionFunctions::Refugees_IsUnloadedInCamp );

    // Critical Intelligence
    brain.RegisterFunction( "DEC_ObtenirRenseignementCritiqueSurPion", &DEC_KnowledgeAgentFunctions::GetCriticalIntelligence );

    // Mount/dismount
    brain.RegisterFunction( "DEC_CanMount", &DEC_AgentFunctions::CanMount );

    // Réseaux souterrains
    brain.RegisterFunction( "DEC_Agent_TempsPourTraverserReseauSouterrain", &DEC_KnowledgeObjectFunctions::EstimatedUndergroundTime );
    brain.RegisterFunction( "DEC_Agent_SeDissimulerDansReseauSouterrain", &DEC_KnowledgeObjectFunctions::HideInUndergroundNetwork );
    brain.RegisterFunction( "DEC_Agent_SortirDuReseauSouterrain", &DEC_KnowledgeObjectFunctions::GetOutFromUndergroundNetwork );

    brain.RegisterFunction( "DEC_Agent_ForcerSilenceRadio", &DEC_KnowledgeAgentFunctions::ForceRadioSilence );
    brain.RegisterFunction( "DEC_Agent_DisableCrowdEffect", &DEC_AgentFunctions::DisableCrowdEffect );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstTerroriste", &DEC_KnowledgeAgentFunctions::IsTerrorist );
    brain.RegisterFunction( "DEC_AgentKnowledge_DisableCrowdEffect", &DEC_KnowledgeAgentFunctions::DisableCrowdEffect );
    brain.RegisterFunction( "DEC_CrowdKnowledge_CanLoadCrowdConcentration", &DEC_KnowledgePopulationFunctions::CanLoadCrowdConcentration );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::RegisterUserFunctions
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::RegisterUserFunctions( sword::Brain& brain )
{
    directia::tools::binders::ScriptRef initQueryFunction = brain.GetScriptRef( "InitQueryReturn" );

    // Missions
    RegisterFunction( "DEC_CreerMissionPion",
        std::function< boost::shared_ptr< MIL_Mission_ABC >( const std::string& ) >( boost::bind( &DEC_OrdersFunctions::CreatePionMissionBM, this, _1 ) ) );
    RegisterFunction( "DEC_CreerMissionPionVersPion",
        std::function< boost::shared_ptr< MIL_Mission_ABC >( const std::string& ) >( boost::bind( &DEC_OrdersFunctions::CreatePionMissionVersPionBM, this, _1 ) ) );
    RegisterFunction( "DEC_DonnerMissionPion",
        std::function< void( boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_OrdersFunctions::CDT_GiveMission, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_DonnerMissionPionVersPion",
        std::function< void( boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_OrdersFunctions::CDT_GiveMissionVersPion, boost::ref( GetPion() ), _1 ) ) );

    // Actions
    RegisterFunction( "DEC_StartTirIndirectSurPosition",
        std::function< unsigned int ( const PHY_DotationCategory*, float, MT_Vector2D* ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIndirectFire_Position, const PHY_DotationCategory*, float, MT_Vector2D*, DEC_Decision_ABC* >, boost::ref( GetPion() ), _1, _2, _3, nullptr ) ) );
    RegisterFunction( "DEC_StartTirIndirectSurPositionAvecDemandeur",
        std::function< unsigned int ( const PHY_DotationCategory*, float, MT_Vector2D*, DEC_Decision_ABC* ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIndirectFire_Position, const PHY_DotationCategory*, float, MT_Vector2D*, DEC_Decision_ABC* >, boost::ref( GetPion() ), _1, _2, _3, _4 ) ) );
    RegisterFunction( "DEC_StartTirIndirectSurConnaissance",
        std::function< unsigned int( const PHY_DotationCategory*, float, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIndirectFire_Knowledge, const PHY_DotationCategory*, float, unsigned int >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_StartTirIndirectSurConnaissancePtr",
        std::function< unsigned int( const PHY_DotationCategory*, float, boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIndirectFire_Knowledge, const PHY_DotationCategory*, float, boost::shared_ptr< DEC_Knowledge_Agent > >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_StartCreateObject",
        std::function< unsigned int( boost::shared_ptr< DEC_Gen_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionConstructObject, boost::shared_ptr< DEC_Gen_Object >, bool >, boost::ref( GetPion() ), _1, false ) ) );
    RegisterFunction( "DEC_StartCreateObjectInstantaneously",
        std::function< unsigned int( boost::shared_ptr< DEC_Gen_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionConstructObject, boost::shared_ptr< DEC_Gen_Object >, bool >, boost::ref( GetPion() ), _1, true ) ) );
    RegisterFunction( "DEC_StartPrepareObject",
        std::function< unsigned int( boost::shared_ptr< DEC_Gen_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionPrepareObject, boost::shared_ptr< DEC_Gen_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartDevaloriserObjet",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionDemineObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartDetruireObjet",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionDestroyObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartValoriserObjet",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionMineObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ADotationPourConstruireObjet",
        std::function< bool ( const std::string& ) >( boost::bind( &DEC_AgentFunctions::HasDotationForBuilding, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ADotationPourConstruireObjetSansRenforts",
        std::function< bool ( const std::string& ) >( boost::bind( &DEC_AgentFunctions::HasDotationForBuildingWithoutReinforcement, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartCreerContournementObjet",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionBypassObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC__StartOccuperObjet",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionOccupyObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartReprendreTravauxObjet",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object >, bool ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionResumeWorkObject, boost::shared_ptr< DEC_Knowledge_Object>, bool >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_ReparerBlocUrbain",
        std::function< unsigned int( MIL_UrbanObject_ABC* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionResumeWorkUrbanBlock, MIL_UrbanObject_ABC* >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_DeteriorateUrbanBlock",
        std::function< unsigned int( MIL_UrbanObject_ABC*, double ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionDeteriorateUrbanBlock, MIL_UrbanObject_ABC*, double >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC__StartAnimerObjet",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionAnimateObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartDistributionObjet",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object >, double ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionDistributeObject, boost::shared_ptr< DEC_Knowledge_Object >, double >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_StartSupplyObject",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >&, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionSupplyObject, boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >&, double >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_StartExtractFromStockObject",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >&, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionExtractFromStockObject, boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >&, double >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Stock_IsExtractPossible", std::function< bool( boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >& ) >( boost::bind( &DEC_ActionFunctions::Stock_IsExtractPossible, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Stock_IsSupplyPossible", std::function< bool( boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >& ) >( boost::bind( &DEC_ActionFunctions::Stock_IsSupplyPossible, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Stock_IsDistributePossible", std::function< bool( boost::shared_ptr< DEC_Knowledge_Object >, boost::shared_ptr< DEC_Knowledge_Population > ) >( boost::bind( &DEC_ActionFunctions::Stock_IsDistributePossible, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Stock_ConnectToResourceNode", &DEC_LogisticFunctions::ConnectToResourceNode );
    RegisterFunction( "DEC_Stock_DisconnectFromResourceNode", &DEC_LogisticFunctions::DisconnectFromResourceNode );
    RegisterFunction( "DEC__StartControlerZone",
        std::function< unsigned int( const TER_Localisation*, double, bool ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionControlZone, const TER_Localisation*, double, bool >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_StartEmbarquement",
        std::function< unsigned int() >( boost::bind( &DEC_ActionFunctions::StartAction  < transport::PHY_ActionLoad >, boost::ref( GetPion() ) ) ) );
    RegisterFunction( "DEC_StartDebarquement",
        std::function< unsigned int() >( boost::bind( &DEC_ActionFunctions::StartAction  < transport::PHY_ActionUnload >, boost::ref( GetPion() ) ) ) );
    RegisterFunction( "DEC_StartIlluminer",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent >, DEC_Decision_ABC* ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIllumination, boost::shared_ptr< DEC_Knowledge_Agent >, DEC_Decision_ABC* >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_StartExtinguishObject",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionExtinguishObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartInterrogateCrowd",
        std::function< unsigned int( int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionInterrogate, int >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartInterrogateUnit",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionInterrogate, boost::shared_ptr< DEC_Knowledge_Agent > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartTraverserReseauSouterrain",
        std::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionMoveUnderground, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );

    // Embarquement / debarquement
    RegisterFunction( "DEC_Agent_EstEmbarquable",
        boost::bind( &DEC_AgentFunctions::HasLoadable, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EstEmbarque",
        boost::bind( &DEC_AgentFunctions::IsLoaded, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EstDebarque",
        boost::bind( &DEC_AgentFunctions::IsUnloaded, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_DureeEmbarquement",
        boost::bind( &DEC_AgentFunctions::GetLoadingTime, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_DureeDebarquement",
        boost::bind( &DEC_AgentFunctions::GetUnloadingTime, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_TransporteursPret",
        boost::bind( &DEC_AgentFunctions::AreHumanTransportersReady, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_LaisserTransporteursSansDelai",
        boost::bind( &DEC_AgentFunctions::DisableHumanTransportersNow, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_RecupererTransporteursSansDelai",
        boost::bind( &DEC_AgentFunctions::RecoverHumanTransportersNow, boost::ref( GetPion() ) ) );

    // Objects
    RegisterFunction( "DEC_CreerObjetSansDelais",
        std::function< int( const std::string&, const TER_Localisation* ) > (boost::bind( &DEC_ObjectFunctions::MagicCreateObject, boost::cref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_MagicGetOrCreateObject",
        std::function< int( const std::string&, const TER_Localisation* ) > (boost::bind( &DEC_ObjectFunctions::MagicGetOrCreateObject, boost::cref( GetPion() ), _1, _2 ) ) );

    // Perception
    RegisterFunction( "DEC_Identification_DistanceMaxCompMajeure",
        boost::bind( &DEC_AgentFunctions::GetIdentificationDistance, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Reconnoissance_MajorComponentMinDistance",
        boost::bind( &DEC_AgentFunctions::GetReconnoissanceDistance, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Perception_EstFurtif",
        std::function< bool( void ) >( boost::bind( &DEC_PerceptionFunctions::IsStealth, boost::ref( GetPion() ) ) ) );
    RegisterFunction( "DEC_Perception_Furtivite",
        std::function< void( float ) >( boost::bind( &DEC_PerceptionFunctions::SetStealthFactor, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Detection_Distance",
        boost::bind( &DEC_AgentFunctions::GetDetectionDistance, boost::ref( GetPion() ) ) );

    // Gestion des renforts
    RegisterFunction( "DEC_Renforts",
        std::function< std::vector<DEC_Decision_ABC*> () >( boost::bind( &DEC_MiscFunctions::GetReinforcements, boost::ref( GetPion() ) ) ) );
    RegisterFunction( "DEC_Renforce",
        std::function< bool ( const DEC_Decision_ABC* ) >(boost::bind( &DEC_MiscFunctions::Reinforce, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_AnnuleRenforcement",
        boost::bind( &DEC_MiscFunctions::CancelReinforcement, boost::ref( GetPion() ) ) );

    // Tests / Debug
    RegisterFunction( "DEC_DebugAffichePoint"  ,
        std::function< void ( const MT_Vector2D* ) > (boost::bind( &DEC_MiscFunctions::DebugDrawPoint, boost::cref( GetPion() ), _1  ) ) );
    RegisterFunction( "DEC_DebugAffichePoints" ,
        std::function< void ( std::vector< boost::shared_ptr< MT_Vector2D > > ) > (boost::bind( &DEC_MiscFunctions::DebugDrawPoints, boost::cref( GetPion() ), _1  ) ) );
    RegisterFunction( "DEC_Debug",
        std::function < void ( const std::string& ) > ( boost::bind( &DEC_MiscFunctions::Debug, boost::cref( GetPion() ) , "Agent" , _1  ) ) );
    RegisterFunction( "DEC_Trace",
        std::function< void ( const std::string& ) >( boost::bind( &DEC_MiscFunctions::Trace, boost::cref( GetPion() ), _1 ) ) );

    RegisterFunction( "DEC_DecisionalState",
        std::function< void ( const std::string&, const std::string& ) >( boost::bind( &DEC_AgentFunctions::DecisionalState, boost::ref( GetPion() ), _1, _2 ) ) );

    // Installation
    RegisterFunction( "DEC_Agent_EstInstalle", boost::bind( &DEC_AgentFunctions::IsInstalled, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EstDesinstalle", boost::bind( &DEC_AgentFunctions::IsUninstalled, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_SInstaller", boost::bind( &DEC_AgentFunctions::Install, boost::ref( GetPion() ) ) );

    // Deployment
    RegisterFunction( "DEC_Agent_IsDeployed", boost::bind( &DEC_AgentFunctions::IsDeployed, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_IsUndeployed", boost::bind( &DEC_AgentFunctions::IsUndeployed, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_Deploy", boost::bind( &DEC_AgentFunctions::Deploy, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_Undeploye", boost::bind( &DEC_AgentFunctions::Undeploy, boost::ref( GetPion() ) ) );

    // Pion accessors
    RegisterFunction( "DEC_Agent_EstPC", boost::bind( &DEC_AgentFunctions::IsPC, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EstTransporte", boost::bind( &DEC_AgentFunctions::IsTransported, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EstEnVol", boost::bind( &DEC_AgentFunctions::IsFlying, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_HauteurDeVol",
        std::function< void ( double ) >( boost::bind( &DEC_AgentFunctions::SetFlyingHeight, boost::ref( GetPion() ), _1 ) ) );

    RegisterFunction( "DEC_Agent_EnVille", boost::bind( &DEC_AgentFunctions::IsInCity, this ) );
    RegisterFunction( "DEC_Agent_EtatOps", boost::bind( &DEC_AgentFunctions::GetOperationalState, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EtatOpsMajeur", boost::bind( &DEC_AgentFunctions::GetMajorOperationalState, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_PeutConstruireObjet",
        std::function< bool ( const std::string& ) >( boost::bind( &DEC_AgentFunctions::CanConstructObject, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_PeutConstruireObjetAvecLocalisation",
        std::function< bool ( const std::string&, const TER_Localisation* ) >( boost::bind( &DEC_AgentFunctions::CanConstructObjectWithLocalisation, boost::cref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Agent_PeutConstruireObjetSansRenforts",
        std::function< bool ( const std::string& ) >( boost::bind( &DEC_AgentFunctions::CanConstructWithoutReinforcement, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_PeutConstruireObjetSansRenfortsAvecLocalisation",
        std::function< bool ( const std::string&, const TER_Localisation* ) >( boost::bind( &DEC_AgentFunctions::CanConstructWithoutReinforcementWithLocalisation, boost::cref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Agent_PeutConstruireObjetEmbarque",
        std::function< bool ( const std::string& ) >( boost::bind( &DEC_AgentFunctions::CanConstructObjectWithLoaded, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_PeutConstruireObjetEmbarqueAvecLocalisation",
        std::function< bool ( const std::string&, const TER_Localisation* ) >( boost::bind( &DEC_AgentFunctions::CanConstructObjectWithLoadedAndLocalisation, boost::cref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Agent_PeutConstruireContournementObjet",
        std::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::CanBypassObject, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_PeutDetruireObjet",
        std::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::CanDestroyObject, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_PeutValoriserObjet",
        std::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::CanMineObject, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_PeutDevaloriserObjet",
        std::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::CanDemineObject, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ActiverModeDiscret", boost::bind( &DEC_AgentFunctions::EnableDiscreteMode, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_DesactiverModeDiscret", boost::bind( &DEC_AgentFunctions::DisableDiscreteMode, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EstNeutralise", boost::bind( &DEC_AgentFunctions::IsNeutralized, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EstCibleTirIndirect", boost::bind( &DEC_AgentFunctions::UpdateUnderIndirectFire, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_AutomateEstEmbraye", boost::bind( &DEC_AgentFunctions::IsAutomateEngaged, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_Position", boost::bind( &DEC_AgentFunctions::GetPosition, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_Direction", boost::bind( &DEC_AgentFunctions::GetDirection, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EstMort", boost::bind( &DEC_AgentFunctions::IsDead, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_NiveauInstallation", boost::bind( &DEC_AgentFunctions::GetPosture, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_RoePopulation", boost::bind( &DEC_AgentFunctions::GetRoePopulation, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EstDansFoule", boost::bind( &DEC_AgentFunctions::IsInCrowd, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_HasDotation",
        std::function< bool ( const PHY_DotationCategory* ) >( boost::bind( &DEC_AgentFunctions::HasDotation, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_CanUseDotation",
        std::function< bool ( const PHY_DotationCategory* ) >( boost::bind( &DEC_AgentFunctions::CanUseDotation, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_GetDotation",
        std::function< const PHY_DotationCategory*( unsigned int ) >( [&]( unsigned int value ){
            return DEC_AgentFunctions::GetDotation( GetPion(), value );
    }));
    RegisterFunction( "DEC_Agent_IlluminateRange", boost::bind( &DEC_AgentFunctions::GetIlluminatingRange, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_CanExtinguish",
        std::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::AgentCanExtinguish, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_CanFly", boost::bind( &DEC_AgentFunctions::AgentCanFly, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_GetFuelDotationNumber", std::function< double ( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::GetFuelDotationNumber, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_GetFuelDotationCapacity", std::function< double ( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::GetFuelDotationCapacity, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ResupplyFuel", &DEC_AgentFunctions::ResupplyFuel );
    RegisterFunction( "DEC_Agent_AutonomieEnDeplacement", boost::bind( &DEC_AgentFunctions::TimeLeftForMoving, this ) );
    RegisterFunction( "DEC_Agent_TempsPourParcourirDistanceEnLigneDroite",
        std::function< float ( float ) >( boost::bind( &DEC_AgentFunctions::TimeToMoveDistance, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_TempsPourDegagerUnObjet",
        std::function< float ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::EstimatedWorkTime, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_AutomateForcerReddition",
        std::function< bool( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::SurrenderAutomat, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ChangerAmbianceEnSurete",
        std::function< void ( bool ) >( boost::bind( &DEC_AgentFunctions::SetToAmbianceSafety, boost::ref( GetPion() ), _1 ) ) );

    // NBC
    RegisterFunction( "DEC_Agent_EstAgentNBC", boost::bind( &DEC_AgentFunctions::IsAgentNBC , boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EstContamine", boost::bind( &DEC_AgentFunctions::IsContaminated , boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EstEmpoisonne", boost::bind( &DEC_AgentFunctions::IsIntoxicated , boost::cref( GetPion() ) ) );

    RegisterFunction( "DEC_Agent_MettreTenueProtectionNBC", boost::bind( &DEC_AgentFunctions::WearNbcProtectionSuit, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EnleverTenueProtectionNBC", boost::bind( &DEC_AgentFunctions::RemoveNbcProtectionSuit, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_NiveauProtectionNBC", boost::bind( &DEC_AgentFunctions::GetNbcSuitLevel, boost::ref( GetPion() ) ) );

    RegisterFunction( "DEC_ConnaissanceObjet_DemandeDeDecontamination",
        std::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::QueueForDecontamination, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceObjet_DemandeDeDecontaminationSurPion",
        std::function< int( DEC_Decision_ABC*, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::QueueUnitForDecontamination, _1, _2 ) ) );

    RegisterFunction( "DEC_Agent_DecontamineConnaissance",
        std::function< int( boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::QueueKnowledgeForDecontamination, _1, _2 ) ) );

    RegisterFunction( "DEC_Agent_ImmuniserNbc", boost::bind( &DEC_AgentFunctions::ImmunizeAgent, boost::ref( GetPion() ) ) ); // deprecated
    RegisterFunction( "DEC_Agent_StopImmuniserNbc", boost::bind( &DEC_AgentFunctions::StopImmunizeAgent, boost::ref( GetPion() ) ) ); // deprecated

    // Blackout
    RegisterFunction( "DEC_Agent_PasserEnSilenceRadio", boost::bind( &DEC_AgentFunctions::ActivateBlackout, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_PasserEnSilenceRadioPartiel", boost::bind( &DEC_AgentFunctions::ActivatePartialBlackout, boost::ref( GetPion() ), true ) );
    RegisterFunction( "DEC_Agent_ArreterSilenceRadio", boost::bind( &DEC_AgentFunctions::DeactivateBlackout, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_PasserEnEmissionRestreinte", boost::bind( &DEC_AgentFunctions::ActivatePartialBlackout, boost::ref( GetPion() ), false ) );

    // Knowledge sharing
    RegisterFunction( "DEC_EnableSharedPerception", std::function< void( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::EnableSharedPerception, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_DisabledSharedPerception", std::function< void( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::DisabledSharedPerception, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_EnableSharedPerceptionWithKnowledge", &DEC_AgentFunctions::EnableSharedPerceptionWithKnowledge );
    RegisterFunction( "DEC_DisabledSharedPerceptionWithKnowledge", &DEC_AgentFunctions::DisabledSharedPerceptionWithKnowledge );
    RegisterFunction( "DEC_Knowledge_EnableSharedPerceptionWithKnowledge", &DEC_AgentFunctions::KnowledgeEnableSharedPerceptionWithKnowledge );
    RegisterFunction( "DEC_Knowledge_DisabledSharedPerceptionWithKnowledge", &DEC_AgentFunctions::KnowledgeDisabledSharedPerceptionWithKnowledge );

    RegisterFunction( "DEC_EnableSharingKnowledges", std::function< void( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::EnableSharingKnowledges, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_DisabledSharingKnowledges", std::function< void( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::DisabledSharingKnowledges, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_EnableSharingKnowledgesWithKnowledge", &DEC_AgentFunctions::EnableSharingKnowledgesWithKnowledge );
    RegisterFunction( "DEC_DisabledSharingKnowledgesWithKnowledge", &DEC_AgentFunctions::DisabledSharingKnowledgesWithKnowledge );

    RegisterFunction( "DEC_Knowledge_CommunicateWithKnowledgeGroup", &DEC_AgentFunctions::KnowledgeCommunicate );

    // Misc
    RegisterFunction( "DEC_ModulationVitesseCourante",
        std::function< void( double ) >( boost::bind( &DEC_MiscFunctions::SetCurrentSpeedModificator, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ModulationVitesseMax",
        std::function< void( double ) >( boost::bind( &DEC_MiscFunctions::SetMaxSpeedModificator, boost::ref( GetPion() ), _1 ) ) );
    // Elongation factor concept was removed but we keep the DEC function around
    // to avoid breaking old models. This should be removed after their migration.
    RegisterFunction( "DEC_Elongation"     , std::function< void(double) >( []( double ) {} ));
    RegisterFunction( "DEC_ReleverPion"    ,
        std::function< bool( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::RelievePion, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_PeutReleverPion",
        std::function< bool( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::CanRelievePion, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Suicide", boost::bind( &DEC_AgentFunctions::Suicide, boost::ref( GetPion() ) ) );

    // Agent knowledge accessors
    RegisterFunction( "DEC_ConnaissanceAgent_NiveauDePerceptionCourant",
        std::function< int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetCurrentPerceptionLevel, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_KnowledgeAgent_IsPerceptionLevelMax",
        std::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsPerceptionLevelMax, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_EstPercuParUnite",
        std::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsPerceivedByAgent, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_EstUnEnnemi",
        std::function< int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsAnEnemy, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_EstUnAllie",
        std::function< int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsAFriend, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_PercoitUnite",
        std::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsPerceivingAgent, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_EstAPorteDeCapteurDansCone",
        std::function< bool( boost::shared_ptr< DEC_Knowledge_Agent >, const MT_Vector2D*, double ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsInDetectionCone, boost::cref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_PeutEtreIllumine",
        std::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::CanBeIlluminate, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_PeutIlluminer",
        std::function< bool() >( boost::bind( &DEC_KnowledgeAgentFunctions::CanIlluminate, boost::cref( GetPion() ) ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_EstTransporte",
        std::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsTransported, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_KnowledgeAgent_IsInObject",
        std::function< bool( const std::string&, boost::shared_ptr< DEC_Knowledge_Agent >, int ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsInObject, boost::cref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_KnowledgeAgent_IsInObjectWithCapacity",
        std::function< bool( const std::string&, boost::shared_ptr< DEC_Knowledge_Agent >, int ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsInObjectWithCapacity, boost::cref( GetPion() ), _1, _2, _3 ) ) );

    // Object knowledges accessors
    RegisterFunction( "DEC_ConnaissanceObjet_EstUnEnnemi",
        std::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsAnEnemy, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceObjet_EstUnAllie",
        std::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsAFriend, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceObjet_Reconnaitre",
        std::function< void( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::Recon, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceObjet_PeutEtreOccupe",
        std::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::CanBeOccupied, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceObjet_PeutEtreAnime",
        std::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::CanBeAnimated, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceObjet_Degrader",
        std::function< int( boost::shared_ptr< DEC_Knowledge_Object >, float, const PHY_DotationCategory*) >( boost::bind( &DEC_KnowledgeObjectFunctions::DamageObject, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_ConnaissanceObjet_NiveauDePerceptionCourant",
        std::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::GetCurrentPerceptionLevel, boost::cref( GetPion() ), _1 ) ) );

    // Population knowledges accessors
    RegisterFunction( "DEC_KnowledgePopulation_Domination",
        std::function< std::pair< float, int > (int) > (boost::bind( &DEC_KnowledgePopulationFunctions::GetDominationState< MIL_AgentPion >, boost::ref( GetPion()), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_Attitude",
        std::function< unsigned int (int) > ( boost::bind( &DEC_KnowledgePopulationFunctions::GetAttitude< MIL_AgentPion >, boost::ref( GetPion()), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_Reconnaitre",
        std::function< void (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::Recon , boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_EstReconnu",
        std::function< bool(int)> ( boost::bind ( &DEC_KnowledgePopulationFunctions::IsRecon< MIL_AgentPion > , boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_EstPercueParUnite",
        std::function< bool (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::IsPerceivedByAgent, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_Dangerosite",
        std::function< float(int)>(boost::bind ( &DEC_KnowledgePopulationFunctions::GetDangerosity , boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_EstDansZone",
        std::function< bool (int, TER_Localisation*) >(boost::bind( &DEC_KnowledgePopulationFunctions::IsInZone < MIL_AgentPion >, boost::cref( GetPion() ), _1 , _2 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_PointPlusProche",
        std::function< boost::shared_ptr<MT_Vector2D> (int) >(boost::bind( &DEC_KnowledgePopulationFunctions::ClosestPoint, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_PointEnTeteDeFlux",
        std::function< boost::shared_ptr<MT_Vector2D> (const DEC_Decision_ABC&, int) >(boost::bind( &DEC_KnowledgePopulationFunctions::GetFlowHead, _1, _2 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_Securiser",
        std::function< void (int) >(boost::bind( &DEC_KnowledgePopulationFunctions::Secure, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_PointSecurisation",
        std::function< boost::shared_ptr<MT_Vector2D> (int) > (boost::bind( &DEC_KnowledgePopulationFunctions::SecuringPoint, boost::cref( GetPion() ), _1  ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_EstEnnemi",
        std::function< bool (int) >(boost::bind( &DEC_KnowledgePopulationFunctions::IsEnemy, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_Exterminer",
        std::function< int (int, float, const PHY_DotationCategory*) >(boost::bind( &DEC_KnowledgePopulationFunctions::Exterminate, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_GetAdhesionPopulation",
        std::function< float (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::GetCrowdAffinity , boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_GetAttitudePopulation",
        std::function< int (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::GetCrowdAttitude , boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartHidingInCrowd",
        std::function< void (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::StartHidingInCrowd, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StopHidingInCrowd",
        std::function< void (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::StopHidingInCrowd, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_GetAgentsHiddenInCrowd",
        std::function< std::vector< DEC_Decision_ABC* > (int) >( boost::bind( &DEC_KnowledgePopulationFunctions::GetAgentsHiddenInCrowd, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_AssignMissionCrowdParameter",
        std::function< void( boost::shared_ptr< MIL_Mission_ABC >, const std::string&, int ) >( boost::bind( &MIL_MissionParameterFactory::SetCrowdKnowledgeParameter, this, _1, _2, _3 ) ) );
    RegisterFunction( "DEC_AssignMissionCrowdListParameter",
        std::function< boost::shared_ptr<MIL_MissionParameter_ABC>( int ) >( boost::bind( &MIL_MissionParameterFactory::CreatePopulationKnowledge, this, _1 ) ) );
    RegisterFunction( "DEC_Crowd_ExtractWoundedFromCrowd",
        std::function< unsigned int( int, const MT_Vector2D* ) >( boost::bind( &DEC_KnowledgePopulationFunctions::ExtractWoundedFromCrowd, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Crowd_ExtractDeadFromCrowd",
        std::function< unsigned int( int, const MT_Vector2D* ) >( boost::bind( &DEC_KnowledgePopulationFunctions::ExtractDeadFromCrowd, boost::cref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Crowd_GetHumansFromAllTypes",
        std::function< std::vector< unsigned int >( int ) >( boost::bind( &DEC_KnowledgePopulationFunctions::GetHumansFromAllTypes< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Crowd_HealWoundedHumans",
        std::function< bool( int ) >( boost::bind( &DEC_KnowledgePopulationFunctions::HealWoundedHumans, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_Verrouiller",
        std::function< void (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::Lock, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_Deverrouiller",
        std::function< void (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::Unlock, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_BlocUrbain",
        std::function< void( const directia::tools::binders::ScriptRef& ) >( boost::bind( &DEC_KnowledgeFunctions::GetUrbanBlock, boost::ref( brain ), boost::ref( GetPion() ), initQueryFunction, _1 ) ) );
    RegisterFunction( "DEC_Connaissances_BlocUrbainPourPosition",
        std::function< const MIL_UrbanObject_ABC*( boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_KnowledgeFunctions::GetUrbanBlockForPosition, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_BlocUrbainDansZone",
        std::function< T_UrbanObjectVector( TER_Localisation* )>( boost::bind( &DEC_KnowledgeFunctions::GetUrbanBlockInZone, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_BlocUrbainDansOuIntersectentZone",
        std::function< T_UrbanObjectVector( TER_Localisation* )>( boost::bind( &DEC_KnowledgeFunctions::GetUrbanBlockInOrIntersectZone, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceUrbanBlock_NiveauDeReconnaissanceCourant", std::function< float( MIL_UrbanObject_ABC* )>( boost::bind( &DEC_UrbanObjectFunctions::GetCurrentRecceProgress, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceBlocUrbain_Traficabilite",
        std::function< double( MIL_UrbanObject_ABC* ) >( boost::bind( &DEC_UrbanObjectFunctions::GetPathfindCost, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_DetruireBlocUrbain",
        std::function< void( MIL_UrbanObject_ABC*, const PHY_DotationCategory* )>( boost::bind( &DEC_UrbanObjectFunctions::DestroyUrbanBlock, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_KnowledgePopulation_GetBarycenter",
        std::function< boost::shared_ptr< MT_Vector2D >( int ) >( boost::bind( &DEC_KnowledgePopulationFunctions::GetBarycenter< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );

    // Global knowledge
    RegisterFunction( "DEC_Connaissances_UnitesDetecteesDansFuseau", boost::bind( &DEC_KnowledgeFunctions::GetDetectedAgentsInFuseau, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesDetecteesDansZone",
        std::function< T_ConstKnowledgeAgentVector( const TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetDetectedAgentsInZone, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesPrenantAPartie", boost::bind( &DEC_KnowledgeFunctions::GetAgentsAttacking, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesPercues", boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesPerceived, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_Observable",
        std::function< void( const directia::tools::binders::ScriptRef& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObservableKnowledge, boost::ref( brain ), boost::ref( GetPion() ), initQueryFunction, _1 ) ) );
    RegisterFunction( "DEC_Connaissances_Destroyable",
        std::function< void( const directia::tools::binders::ScriptRef& ) >( boost::bind( &DEC_KnowledgeFunctions::GetDestroyableKnowledge, boost::ref( brain ), boost::ref( GetPion() ), initQueryFunction, _1 ) ) );//@TODO MGD same function than Observable for the moment
    RegisterFunction( "DEC_Connaissances_Identifiable",
        std::function< void( const directia::tools::binders::ScriptRef& ) >( boost::bind( &DEC_KnowledgeFunctions::GetIdentifiableKnowledge, boost::ref( brain ), boost::ref( GetPion() ), initQueryFunction, _1 ) ) );//@TODO MGD same function than Observable for the moment
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesPercuesParPion",
        std::function< T_ConstKnowledgeAgentVector( const DEC_Decision_ABC* ) >( boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesPercuesDansFuseau", boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedInFuseau, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesDansFuseau", boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesInFuseau, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesDansZone",
        std::function< T_ConstKnowledgeAgentVector( TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesInZone, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesBlesseesDansZone",
        std::function< T_ConstKnowledgeAgentVector( TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetWoundedUnitsInZone, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesCivilesDansZone",
        std::function< T_ConstKnowledgeAgentVector( TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetCiviliansInZone, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesDansCercle",
        std::function< T_ConstKnowledgeAgentVector( const MT_Vector2D*, float ) >( boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesInCircle, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesBlesseesOuTueesDansCercle",
        std::function< T_ConstKnowledgeAgentVector( const MT_Vector2D*, float ) >( boost::bind( &DEC_KnowledgeFunctions::GetWoundedOrDeadUnitsInCircle, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesDansBlocUrbain",
        std::function< T_ConstKnowledgeAgentVector( MIL_UrbanObject_ABC* ) >( boost::bind( &DEC_UrbanObjectFunctions::GetLivingEnemiesInBU, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_RefugiesAProximite",
        std::function< T_ConstKnowledgeAgentVector( double ) > ( boost::bind( &DEC_KnowledgeFunctions::GetNearbyRefugees, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_TerroristsAProximite",
        std::function< T_ConstKnowledgeAgentVector( double ) > ( boost::bind( &DEC_KnowledgeFunctions::GetNearbyTerrorists, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesRenduesAProximite",
        std::function< T_ConstKnowledgeAgentVector ( double ) > ( boost::bind( &DEC_KnowledgeFunctions::GetNearbySurrenderedAgents, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_PresenceEnnemieDansCercle",
        std::function< bool( const MT_Vector2D*, double ) >( boost::bind( &DEC_KnowledgeFunctions::EnemyPresenceInCircle, boost::cref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesAmiesDansZone",
        std::function< T_ConstKnowledgeAgentVector( const TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetFriendsInZone< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_PartageConnaissancesAvec",
        std::function< void( DEC_Decision_ABC*, float ) >( boost::bind( &DEC_KnowledgeFunctions::ShareKnowledgesWith< MIL_AgentPion >, boost::cref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Connaissances_PartageConnaissancesDansZoneAvec",
        std::function< void( DEC_Decision_ABC*, const MT_Vector2D*, float ) >( boost::bind( &DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith< MIL_AgentPion >, boost::cref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Knowledges_ObjectsInCircle",
        std::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const MT_Vector2D*, double, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInCircle< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3, false ) ) );
    RegisterFunction( "DEC_Knowledges_AllObjectsInCircle",
        std::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const MT_Vector2D*, double, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInCircle< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3, true ) ) );
    RegisterFunction( "DEC_ObjectKnowledgesInZone",
        std::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const TER_Localisation*, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInZone< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_ObjectKnowledgesIntersectingInZone",
        std::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const TER_Localisation*, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsIntersectingInZone< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Connaissances_ObjetsDansFuseau",
        std::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const std::string& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInFuseau< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_CollisionsObjets", boost::bind( &DEC_KnowledgeFunctions::GetObjectsColliding, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_CollisionsDesastres", boost::bind( &DEC_KnowledgeFunctions::GetCollidingDisasters, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_Desastres", boost::bind( &DEC_KnowledgeFunctions::GetDisasters, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_CollisionsObjetsDeType",
        std::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const std::string& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsCollidingFromType, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_ObjetLePlusProche",
        std::function< boost::shared_ptr< DEC_Knowledge_Object >( const std::string& ) >( boost::bind( &DEC_KnowledgeFunctions::GetClosestObject, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_ObjetAmiLePlusProche",
        std::function< boost::shared_ptr< DEC_Knowledge_Object >( const std::string& ) >( boost::bind( &DEC_KnowledgeFunctions::GetClosestFriendObject, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_CollisionsPopulations", boost::bind( &DEC_KnowledgeFunctions::GetPopulationsColliding, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_PopulationsPrenantAPartie", boost::bind( &DEC_KnowledgeFunctions::GetPopulationsAttacking, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_EstPrisAPartieParPopulation",
        std::function< bool( const int ) >( boost::bind( &DEC_KnowledgeFunctions::IsPopulationAttacking, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_Populations", boost::bind( &DEC_KnowledgeFunctions::GetPopulations< MIL_AgentPion >, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_ChangerNombreIndividuArmeDansFoule",
        std::function< void( const int, const double ) >( boost::bind( &DEC_KnowledgePopulationFunctions::ChangeArmedIndividuals, boost::ref( GetPion() ), _1, _2 ) ) );

    // Ordres de conduite
    RegisterFunction( "DEC_ReleverUnite",
        std::function< bool( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::RelievePion, boost::ref( GetPion() ), _1 ) ) );

    // Limas / Mission
    RegisterFunction( "DEC_NouvelleMission", boost::bind( &DEC_OrdersFunctions::IsNewMissionStarted< MIL_AgentPion >, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_FinMission", boost::bind( &DEC_OrdersFunctions::FinishMission< MIL_AgentPion >, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_GetLima",
        std::function< unsigned int( unsigned int ) >( boost::bind( &DEC_OrdersFunctions::GetLima< MIL_Agent_ABC >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_GetLimasFromType",
        std::function< std::vector< unsigned int >( unsigned int ) >( boost::bind( &DEC_OrdersFunctions::GetLimasFromType< MIL_Agent_ABC >, boost::ref( GetPion() ), _1 ) ) );

    RegisterFunction( "DEC_SetMissionLimaFlag"            ,
        std::function< void (unsigned int, bool)> ( boost::bind( &DEC_OrdersFunctions::PionSetMissionLimaFlag, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_GetMissionLimaFlag"            ,
        std::function< bool (unsigned int)> ( boost::bind( &DEC_OrdersFunctions::GetMissionLimaFlag < MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_SetMissionLimaFlagHoraire"     ,
        std::function< void (unsigned int, bool)> ( boost::bind( &DEC_OrdersFunctions::PionSetMissionLimaScheduleFlag, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Fuseau"    ,
        std::function< const MIL_Fuseau& ()> ( boost::bind( &DEC_OrdersFunctions::GetFuseau, boost::cref( *this ) ) ) );

    // Etat décisionnel
    RegisterFunction( "DEC_Agent_ChangeEtatRapportDeForce",
        std::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyForceRatioStateChanged, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ChangeEtatROE",
        std::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyRulesOfEngagementStateChanged, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_GetEtatROE",
        boost::bind( &DEC_AgentFunctions::GetRulesOfEngagementState, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_ChangeEtatROEPopulation",
        std::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyRulesOfEngagementPopulationStateChanged, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ChangeEtatCombatDeRencontre",
        std::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyCloseCombatStateChanged, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ChangeEtatOperationnel",
        std::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyOperationalStateChanged, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ChangeDisponibiliteAuTirIndirect",
        std::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyIndirectFireAvailabilityChanged, boost::ref( GetPion() ), _1 ) ) );

    // Facteurs humains
    RegisterFunction( "DEC_FacteurHumain_Fatigue", boost::bind( &DEC_AgentFunctions::GetHumanFactorTiredness, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_FacteurHumain_Moral", boost::bind( &DEC_AgentFunctions::GetHumanFactorMorale, boost::ref( GetPion() ) ) );

    // Calculs de position
    RegisterFunction( "DEC_Geometrie_PositionInterception",
        std::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent >, MT_Vector2D*, double )>( boost::bind( &DEC_GeometryFunctions::GetInterceptionPosition, boost::cref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Agent_PositionInterception",
        std::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_AgentFunctions::GetInterceptionPoint, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPositionAppui",
        std::function< boost::shared_ptr< MT_Vector2D >( DEC_Decision_ABC*, double ) >( boost::bind( &DEC_GeometryFunctions::ComputeSupportPosition, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPositionEmbuscade",
        std::function< boost::shared_ptr< MT_Vector2D >( MT_Vector2D*, MT_Vector2D*, double ) >( boost::bind( &DEC_GeometryFunctions::ComputeAmbushPosition, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPositionSurete",
        std::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent >, double ) >( boost::bind( &DEC_GeometryFunctions::ComputeSafetyPosition, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPositionSureteAvecPopulation",
        std::function< boost::shared_ptr< MT_Vector2D >( unsigned int, double ) >( boost::bind( &DEC_GeometryFunctions::ComputeSafetyPositionWithPopulation, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPositionSureteAvecObjectif",
        std::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent >, double, MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::ComputeSafetyPositionWithObjective, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPointProcheLocalisationDansFuseau",
        std::function< boost::shared_ptr< MT_Vector2D >( const TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeNearestLocalisationPointInUnitFuseau, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPointProcheLocalisationNonClippeeDansFuseau" ,
        std::function< boost::shared_ptr< MT_Vector2D >( const TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInUnitFuseau, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerBarycentreLocalisationDansFuseau",
        std::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeUnitLocalisationBarycenterInFuseau, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerEntreeFuseauLaPlusProche",
        boost::bind( &DEC_GeometryFunctions::ComputeNearestFuseauEntryPoint, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Geometry_SplitLocalisation",
        std::function< std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( TER_Localisation*, unsigned int, MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::SplitUnitLocalisationInParts, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Geometry_SplitLocalisationSurface",
        std::function< std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( TER_Localisation*, unsigned int, MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::SplitUnitLocalisationInSurfaces, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Geometry_RecursiveSplitLocalisationSurface",
        std::function< std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( TER_Localisation*, unsigned int ) >( boost::bind( &DEC_GeometryFunctions::RecursiveSplitUnitLocalisationInSurfaces, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Geometrie_DecoupeFuseauEnTroncons",
        std::function< std::vector< boost::shared_ptr< TER_Localisation > >( const double ) >( boost::bind( &DEC_GeometryFunctions::SplitUnitLocalisationInSections, boost::ref( GetPion() ), _1  ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPositionObstacle",
        std::function< boost::shared_ptr< MT_Vector2D >( MT_Vector2D*, const std::string&, double) >( boost::bind( &DEC_GeometryFunctions::ComputeObstaclePositionForUnit, boost::cref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPointArrivee",
        boost::bind( &DEC_GeometryFunctions::ComputeUnitDestPoint, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPointDepart",
        boost::bind( &DEC_GeometryFunctions::ComputeUnitStartPoint, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Geometrie_EstPointDansFuseau",
        std::function< bool( MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::IsPointInUnitFuseau, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerBarycentreListeConnaissancesAgents",
        std::function< boost::shared_ptr< MT_Vector2D >( const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& ) >( boost::bind( &DEC_GeometryFunctions::ComputeKnowledgeAgentBarycenter, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPositionParRapportALima",
        std::function< boost::shared_ptr< MT_Vector2D >( int, float ) >( boost::bind( &DEC_GeometryFunctions::ComputePointBeforeUnitLima, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerTrafficableBarycentreLocalisation",
        std::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeTrafficableLocalisationBarycenter, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_IsPointInUrbanBlockTrafficable",
        std::function< bool( MT_Vector2D& ) >( boost::bind( &DEC_GeometryFunctions::IsPointInUrbanBlockTrafficable, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Geometrie_GetLeavingAreaPosition",
        std::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::GetLeavingAreaPosition, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_EstDansLeFuseau", boost::bind( &DEC_AgentFunctions::AgentHasFuseau , boost::cref( GetPion() ) ) );

    //Keypoint
    RegisterFunction( "DEC_Crossroads",
        std::function< void( const directia::tools::binders::ScriptRef& ) >( boost::bind( &DEC_TerrainFunctions::GetCrossroads, boost::ref( brain ), boost::ref( GetPion() ), initQueryFunction, _1 ) )  );
    RegisterFunction( "DEC_FindSafetyPositions",
        std::function< std::vector< boost::shared_ptr< MT_Vector2D > >( float, float ) >( boost::bind( &DEC_TerrainFunctions::FindSafetyPositionsWithinCircle, boost::ref( GetPion() ), _1, _2 ) )  );

    // Hierarchie
    RegisterFunction( "DEC_Pion_PionsAvecPC", boost::bind( &DEC_AgentFunctions::GetPionsWithPC, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Pion_PionsSansPC", boost::bind( &DEC_AgentFunctions::GetPionsWithoutPC, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Pion_PionsAvecPCCommunication", boost::bind( &DEC_AgentFunctions::GetCommunicationPionsWithPC, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Pion_PionsSansPCCommunication", boost::bind( &DEC_AgentFunctions::GetCommunicationPionsWithoutPC, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Pion_PionPC", boost::bind( &DEC_AgentFunctions::GetPionPC, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Pion_ChangeAutomate",
        std::function< bool( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::ChangeAutomate, boost::ref( GetPion() ), _1 ) ) );

    // Logistique
    RegisterFunction( "DEC_StartPreterVSRAM",
        std::function< unsigned int( DEC_Decision_ABC*, DEC_Decision_ABC*, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionLendCollectionComposantes, DEC_Decision_ABC*, DEC_Decision_ABC*, unsigned int >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_StartPreterRemorqueurs",
        std::function< unsigned int( DEC_Decision_ABC*, DEC_Decision_ABC*, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionLendHaulerComposantes, DEC_Decision_ABC*, DEC_Decision_ABC*, unsigned int >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_StartPreterComposantes",
        std::function< unsigned int( DEC_Decision_ABC*, DEC_Decision_ABC*, PHY_ComposanteTypePion*, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionLendSpecificComposantes, DEC_Decision_ABC*, DEC_Decision_ABC*, PHY_ComposanteTypePion*, unsigned int >, boost::ref( GetPion() ), _1, _2, _3, _4 ) ) );
    RegisterFunction( "DEC_RecupererComposantes",
        std::function< void( const DEC_Decision_ABC*, PHY_ComposanteTypePion*, const unsigned int ) >( boost::bind( &DEC_LogisticFunctions::UndoLendSpecificComposantes, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_RecupererVSRAM",
        std::function< void( const DEC_Decision_ABC*, const unsigned int ) >( boost::bind( &DEC_LogisticFunctions::UndoLendCollectionComposantes, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_RecupererRemorqueurs",
        std::function< void( const DEC_Decision_ABC*, const unsigned int ) >( boost::bind( &DEC_LogisticFunctions::UndoLendHaulerComposantes, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Pion_PcDeTC2",
        boost::bind( &DEC_LogisticFunctions::PionGetPcTC2, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_ChangeValeurDotations2",
        std::function< void( int, const double ) >( boost::bind( &DEC_LogisticFunctions::ChangeDotationsValueUsingTC2, boost::ref( GetPion() ), _1, _2, -1 ) ) );
    RegisterFunction( "DEC_ChangeValeurDotations3",
        std::function< void( int, const double, int ) >( boost::bind( &DEC_LogisticFunctions::ChangeDotationsValueUsingTC2, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_CreateBreakdown",
        std::function< bool( const PHY_ComposanteTypePion*, unsigned int ) >( boost::bind( &DEC_AgentFunctions::CreateBreakdown, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_StartConsumingResources",
        std::function< unsigned int( unsigned int, double, double ) >(
            [&]( unsigned int category, double value, double duration ) {
                return DEC_ActionFunctions::StartAction< PHY_ActionConsumeResources >( GetPion(), category, value, duration, MIL_Time_ABC::GetTime().GetTickDuration() );
    }));

    // Transport / Heliportage
    RegisterFunction( "DEC_Transport_AjouterPion",
        std::function< void( DEC_Decision_ABC*, bool ) >( boost::bind( &DEC_ActionFunctions::Transport_AddPion, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Transport_AjouterPions",
        std::function< void( const std::vector< DEC_Decision_ABC* >&, bool ) >( boost::bind( &DEC_ActionFunctions::Transport_AddPions, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Start_TransportEmbarquer", boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTransportLoad >, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Start_TransportDebarquer",
        std::function< unsigned int( MT_Vector2D* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTransportUnload, MT_Vector2D* >, boost::ref( GetPion() ), _1 ) ) );

    RegisterFunction( "DEC_Transport_EmbarquerDansTransporteurSansDelais", std::function< void ( const DEC_Decision_ABC* ) >( boost::bind( &DEC_ActionFunctions::Transport_MagicLoadPionInCarrier, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Transport_DebarquerDeTransporteurSansDelais", std::function< void () >( boost::bind( &DEC_ActionFunctions::Transport_MagicUnloadPionFromCarrier, boost::ref( GetPion() ) ) ) );
    RegisterFunction( "DEC_Transport_Transporteur", std::function< DEC_Decision_ABC* () >( boost::bind( &DEC_ActionFunctions::Transport_GetCarrier, boost::ref( GetPion() ) ) ) );

    RegisterFunction( "DEC_Transport_EmbarquerPionSansDelais",
        std::function< void( const DEC_Decision_ABC*, bool ) >( boost::bind( &DEC_ActionFunctions::Transport_MagicLoadPion, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Transport_EmbarquerPionsSansDelais",
        std::function< void (const std::vector< DEC_Decision_ABC* >&, bool) >( boost::bind( &DEC_ActionFunctions::Transport_MagicLoadPions, boost::ref( GetPion()) , _1, _2 ) ) );
    RegisterFunction( "DEC_Transport_DebarquerPionSansDelais",
        std::function<void(const DEC_Decision_ABC*)>( boost::bind( &DEC_ActionFunctions::Transport_MagicUnloadPion , boost::ref( GetPion()) , _1 ) ) );
    RegisterFunction( "DEC_Transport_DebarquerPionsSansDelais",
        std::function<void(const std::vector< DEC_Decision_ABC* >&)>( boost::bind( &DEC_ActionFunctions::Transport_MagicUnloadPions , boost::ref( GetPion()) , _1 ) ) );
    RegisterFunction( "DEC_Transport_EstTermine",
        boost::bind( &DEC_ActionFunctions::Transport_IsFinished , boost::ref( GetPion()) ) );
    RegisterFunction( "DEC_Transport_Annuler",
        boost::bind( &DEC_ActionFunctions::Transport_Cancel , boost::ref( GetPion()) ) );
    RegisterFunction( "DEC_Transport_EnCoursDeTransport",
        boost::bind( &DEC_ActionFunctions::Transport_IsTransporting, boost::ref( GetPion() ) ) );

    RegisterFunction( "DEC_Agent_PeutTransporterPion",
        std::function< bool( const DEC_Decision_ABC*, bool ) >( boost::bind( &DEC_ActionFunctions::AgentCanTransportPion, this, _1, _2 ) ) );
    RegisterFunction( "DEC_Agent_TransportNombreAllerRetour",
        std::function< double( const DEC_Decision_ABC*, bool ) >( boost::bind( &DEC_ActionFunctions::GetNumberOfRoundTripToTransportPion, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Connaissance_TransportNombreAllerRetour",
        &DEC_ActionFunctions::GetNumberOfRoundTripToTransportKnowledge );

    RegisterFunction( "DEC_StartEmbarquerFouleDUneConcentration",
        std::function< unsigned int( int, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< crowdtransport::PHY_ActionLoadCrowd, int, unsigned int >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_StartDebarquerFouleSurPosition",
        std::function< unsigned int( int, MT_Vector2D* ) >( boost::bind( &DEC_ActionFunctions::StartAction< crowdtransport::PHY_ActionUnloadCrowd, int, MT_Vector2D* >, boost::ref( GetPion() ), _1, _2 ) ) );

    // Prisonniers
    RegisterFunction( "DEC_Prisonniers_CapturerEtEmbarquer" ,
        std::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > )>(boost::bind(&DEC_ActionFunctions::Prisoners_CaptureAndLoad , boost::ref( GetPion()) ,_1 ) ) );
    RegisterFunction( "DEC_Prisonniers_Debarquer"           ,
        std::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > )>(boost::bind(&DEC_ActionFunctions::Prisoners_Unload , boost::ref(GetPion()), _1 ) ) );
    RegisterFunction( "DEC_Prisonniers_DebarquerDansCamp"   ,
        std::function< void (boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< DEC_Knowledge_Object > )>(boost::bind(&DEC_ActionFunctions::Prisoners_UnloadInCamp , boost::ref( GetPion() ), _1, _2  ) ) );
    RegisterFunction( "DEC_Prisonniers_EstEmbarque"         ,
        std::function< bool (boost::shared_ptr< DEC_Knowledge_Agent >) >(boost::bind(&DEC_ActionFunctions::PrisonnersRefugees_IsLoaded , boost::ref( GetPion() ), _1  ) ) );

    // Refugies
    RegisterFunction( "DEC_Refugies_OrienterEtEmbarquer" ,
        std::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > ) > ( boost::bind(&DEC_ActionFunctions::Refugees_OrientateAndLoad , boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Refugies_Debarquer"           ,
        std::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > ) > ( boost::bind(&DEC_ActionFunctions::Refugees_Unload , boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Refugies_DebarquerDansCamp"   ,
        std::function< void ( boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< DEC_Knowledge_Object > )> ( boost::bind(&DEC_ActionFunctions::Refugees_UnloadInCamp , boost::ref( GetPion() ), _1, _2  ) ) );
    RegisterFunction( "DEC_Refugies_EstEmbarque"         ,
        std::function< bool ( boost::shared_ptr< DEC_Knowledge_Agent > ) > ( boost::bind ( &DEC_ActionFunctions::PrisonnersRefugees_IsLoaded , boost::ref( GetPion() ), _1  ) ) );

    // Emergency functions
    RegisterFunction( "DEC_Start_EmergencyInfluence",
        std::function< unsigned int( const std::string&, double ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionInfluence, const std::string&, double >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Start_EmergencyTriggerActivity",
        std::function< unsigned int( const std::string&, double ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTriggerActivity, const std::string&, double >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Start_EmergencyInfluenceInArea",
        std::function< unsigned int( const std::string&, double, const TER_Localisation* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionInfluenceInArea, const std::string&, double, const TER_Localisation* >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Start_EmergencyTriggerActivityInArea",
        std::function< unsigned int( const std::string&, double, const TER_Localisation* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTriggerActivityInArea, const std::string&, double, const TER_Localisation* >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Start_EmergencyUnloadActivity",
        std::function< unsigned int( const std::string&, double ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionUnloadActivity, const std::string&, double >, boost::ref( GetPion() ), _1, _2 ) ) );

    // Representations
    RegisterFunction( "DEC_GetOrdersCategory",
        boost::bind( &DEC_MiscFunctions::GetOrdersCategory , boost::ref( *pEntity_ ) ) );
    RegisterFunction( "DEC_RemoveFromOrdersCategory",
        std::function< void ( boost::shared_ptr< MIL_FragOrder > ) > ( boost::bind( &DEC_MiscFunctions::RemoveFromOrdersCategory , boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_DeleteRepresentation",
        std::function< void ( boost::shared_ptr< MIL_FragOrder > ) > ( boost::bind( &DEC_MiscFunctions::DeleteOrderRepresentation , boost::ref( GetPion() ), _1 ) ) );

    pEntity_->GetType().RegisterFunctions( brain, GetPion() );
    //
    // Critical Intelligence
    RegisterFunction( "DEC_ObtenirRenseignementCritiqueSurFoule",
         std::function< std::string( int ) >(boost::bind( &DEC_KnowledgePopulationFunctions::GetCriticalIntelligence, boost::cref( GetPion() ), _1 ) ) );
}
