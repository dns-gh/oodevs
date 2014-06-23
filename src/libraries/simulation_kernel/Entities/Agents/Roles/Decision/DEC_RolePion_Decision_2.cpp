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
        boost::function< boost::shared_ptr< MIL_Mission_ABC >( const std::string& ) >( boost::bind( &DEC_OrdersFunctions::CreatePionMissionBM, this, _1 ) ) );
    RegisterFunction( "DEC_CreerMissionPionVersPion",
        boost::function< boost::shared_ptr< MIL_Mission_ABC >( const std::string& ) >( boost::bind( &DEC_OrdersFunctions::CreatePionMissionVersPionBM, this, _1 ) ) );
    RegisterFunction( "DEC_DonnerMissionPion",
        boost::function< void( boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_OrdersFunctions::CDT_GiveMission, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_DonnerMissionPionVersPion",
        boost::function< void( boost::shared_ptr< MIL_Mission_ABC > ) >( boost::bind( &DEC_OrdersFunctions::CDT_GiveMissionVersPion, boost::ref( GetPion() ), _1 ) ) );

    // Actions
    RegisterFunction( "DEC_StartTirIndirectSurPosition",
        boost::function< unsigned int ( const PHY_DotationCategory*, float, MT_Vector2D* ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIndirectFire_Position, const PHY_DotationCategory*, float, MT_Vector2D* >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_StartTirIndirectSurPositionAvecDemandeur",
        boost::function< unsigned int ( const PHY_DotationCategory*, float, MT_Vector2D*, DEC_Decision_ABC* ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIndirectFire_Position, const PHY_DotationCategory*, float, MT_Vector2D*, DEC_Decision_ABC* >, boost::ref( GetPion() ), _1, _2, _3, _4 ) ) );
    RegisterFunction( "DEC_StartTirIndirectSurConnaissance",
        boost::function< unsigned int( const PHY_DotationCategory*, float, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIndirectFire_Knowledge, const PHY_DotationCategory*, float, unsigned int >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_StartTirIndirectSurConnaissancePtr",
        boost::function< unsigned int( const PHY_DotationCategory*, float, boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIndirectFire_Knowledge, const PHY_DotationCategory*, float, boost::shared_ptr< DEC_Knowledge_Agent > >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_StartCreateObject",
        boost::function< unsigned int( boost::shared_ptr< DEC_Gen_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionConstructObject, boost::shared_ptr< DEC_Gen_Object >, bool >, boost::ref( GetPion() ), _1, false ) ) );
    RegisterFunction( "DEC_StartCreateObjectInstantaneously",
        boost::function< unsigned int( boost::shared_ptr< DEC_Gen_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionConstructObject, boost::shared_ptr< DEC_Gen_Object >, bool >, boost::ref( GetPion() ), _1, true ) ) );
    RegisterFunction( "DEC_StartPrepareObject",
        boost::function< unsigned int( boost::shared_ptr< DEC_Gen_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionPrepareObject, boost::shared_ptr< DEC_Gen_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartDevaloriserObjet",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionDemineObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartDetruireObjet",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionDestroyObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartValoriserObjet",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionMineObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ADotationPourConstruireObjet",
        boost::function< bool ( const std::string& ) >( boost::bind( &DEC_AgentFunctions::HasDotationForBuilding, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ADotationPourConstruireObjetSansRenforts",
        boost::function< bool ( const std::string& ) >( boost::bind( &DEC_AgentFunctions::HasDotationForBuildingWithoutReinforcement, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartCreerContournementObjet",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionBypassObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC__StartOccuperObjet",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionOccupyObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartReprendreTravauxObjet",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object >, bool ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionResumeWorkObject, boost::shared_ptr< DEC_Knowledge_Object>, bool >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_ReparerBlocUrbain",
        boost::function< unsigned int( MIL_UrbanObject_ABC* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionResumeWorkUrbanBlock, MIL_UrbanObject_ABC* >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_DeteriorateUrbanBlock",
        boost::function< unsigned int( MIL_UrbanObject_ABC*, double ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionDeteriorateUrbanBlock, MIL_UrbanObject_ABC*, double >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC__StartAnimerObjet",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionAnimateObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartDistributionObjet",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object >, double ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionDistributeObject, boost::shared_ptr< DEC_Knowledge_Object >, double >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_StartSupplyObject",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >&, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionSupplyObject, boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >&, double >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_StartExtractFromStockObject",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >&, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionExtractFromStockObject, boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >&, double >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Stock_IsExtractPossible", boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >& ) >( boost::bind( &DEC_ActionFunctions::Stock_IsExtractPossible, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Stock_IsSupplyPossible", boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object >, const std::vector< const PHY_DotationCategory* >& ) >( boost::bind( &DEC_ActionFunctions::Stock_IsSupplyPossible, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Stock_IsDistributePossible", boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object >, boost::shared_ptr< DEC_Knowledge_Population > ) >( boost::bind( &DEC_ActionFunctions::Stock_IsDistributePossible, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Stock_ConnectToResourceNode", &DEC_LogisticFunctions::ConnectToResourceNode );
    RegisterFunction( "DEC_Stock_DisconnectFromResourceNode", &DEC_LogisticFunctions::DisconnectFromResourceNode );
    RegisterFunction( "DEC__StartControlerZone",
        boost::function< unsigned int( const TER_Localisation*, double, bool ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionControlZone, const TER_Localisation*, double, bool >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_StartEmbarquement",
        boost::function< unsigned int() >( boost::bind( &DEC_ActionFunctions::StartAction  < transport::PHY_ActionLoad >, boost::ref( GetPion() ) ) ) );
    RegisterFunction( "DEC_StartDebarquement",
        boost::function< unsigned int() >( boost::bind( &DEC_ActionFunctions::StartAction  < transport::PHY_ActionUnload >, boost::ref( GetPion() ) ) ) );
    RegisterFunction( "DEC_StartIlluminer",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent >, DEC_Decision_ABC* ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIllumination, boost::shared_ptr< DEC_Knowledge_Agent >, DEC_Decision_ABC* >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_StartExtinguishObject",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionExtinguishObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartInterrogateCrowd",
        boost::function< unsigned int( int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionInterrogate, int >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartInterrogateUnit",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionInterrogate, boost::shared_ptr< DEC_Knowledge_Agent > >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartTraverserReseauSouterrain",
        boost::function< unsigned int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionMoveUnderground, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );

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
        boost::function< int( const std::string&, const TER_Localisation* ) > (boost::bind( &DEC_ObjectFunctions::MagicCreateObject, boost::cref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_MagicGetOrCreateObject",
        boost::function< int( const std::string&, const TER_Localisation* ) > (boost::bind( &DEC_ObjectFunctions::MagicGetOrCreateObject, boost::cref( GetPion() ), _1, _2 ) ) );

    // Perception
    RegisterFunction( "DEC_Identification_DistanceMaxCompMajeure",
        boost::bind( &DEC_AgentFunctions::GetIdentificationDistance, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Reconnoissance_MajorComponentMinDistance",
        boost::bind( &DEC_AgentFunctions::GetReconnoissanceDistance, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Perception_EstFurtif",
        boost::function< bool( void ) >( boost::bind( &DEC_PerceptionFunctions::IsStealth, boost::ref( GetPion() ) ) ) );
    RegisterFunction( "DEC_Perception_Furtivite",
        boost::function< void( float ) >( boost::bind( &DEC_PerceptionFunctions::SetStealthFactor, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Detection_Distance",
        boost::bind( &DEC_AgentFunctions::GetDetectionDistance, boost::ref( GetPion() ) ) );

    // Gestion des renforts
    RegisterFunction( "DEC_Renforts",
        boost::function< std::vector<DEC_Decision_ABC*> () >( boost::bind( &DEC_MiscFunctions::GetReinforcements, boost::ref( GetPion() ) ) ) );
    RegisterFunction( "DEC_Renforce",
        boost::function< bool ( const DEC_Decision_ABC* ) >(boost::bind( &DEC_MiscFunctions::Reinforce, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_AnnuleRenforcement",
        boost::bind( &DEC_MiscFunctions::CancelReinforcement, boost::ref( GetPion() ) ) );

    // Tests / Debug
    RegisterFunction( "DEC_DebugAffichePoint"  ,
        boost::function< void ( const MT_Vector2D* ) > (boost::bind( &DEC_MiscFunctions::DebugDrawPoint, boost::cref( GetPion() ), _1  ) ) );
    RegisterFunction( "DEC_DebugAffichePoints" ,
        boost::function< void ( std::vector< boost::shared_ptr< MT_Vector2D > > ) > (boost::bind( &DEC_MiscFunctions::DebugDrawPoints, boost::cref( GetPion() ), _1  ) ) );
    RegisterFunction( "DEC_Debug",
        boost::function < void ( const std::string& ) > ( boost::bind( &DEC_MiscFunctions::Debug, boost::cref( GetPion() ) , "Agent" , _1  ) ) );
    RegisterFunction( "DEC_Trace",
        boost::function< void ( const std::string& ) >( boost::bind( &DEC_MiscFunctions::Trace, boost::cref( GetPion() ), _1 ) ) );

    RegisterFunction( "DEC_DecisionalState",
        boost::function< void ( const std::string&, const std::string& ) >( boost::bind( &DEC_AgentFunctions::DecisionalState, boost::ref( GetPion() ), _1, _2 ) ) );

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
        boost::function< void ( double ) >( boost::bind( &DEC_AgentFunctions::SetFlyingHeight, boost::ref( GetPion() ), _1 ) ) );

    RegisterFunction( "DEC_Agent_EnVille", boost::bind( &DEC_AgentFunctions::IsInCity, this ) );
    RegisterFunction( "DEC_Agent_EtatOps", boost::bind( &DEC_AgentFunctions::GetOperationalState, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EtatOpsMajeur", boost::bind( &DEC_AgentFunctions::GetMajorOperationalState, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_PeutConstruireObjet",
        boost::function< bool ( const std::string& ) >( boost::bind( &DEC_AgentFunctions::CanConstructObject, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_PeutConstruireObjetAvecLocalisation",
        boost::function< bool ( const std::string&, const TER_Localisation* ) >( boost::bind( &DEC_AgentFunctions::CanConstructObjectWithLocalisation, boost::cref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Agent_PeutConstruireObjetSansRenforts",
        boost::function< bool ( const std::string& ) >( boost::bind( &DEC_AgentFunctions::CanConstructWithoutReinforcement, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_PeutConstruireObjetSansRenfortsAvecLocalisation",
        boost::function< bool ( const std::string&, const TER_Localisation* ) >( boost::bind( &DEC_AgentFunctions::CanConstructWithoutReinforcementWithLocalisation, boost::cref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Agent_PeutConstruireObjetEmbarque",
        boost::function< bool ( const std::string& ) >( boost::bind( &DEC_AgentFunctions::CanConstructObjectWithLoaded, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_PeutConstruireObjetEmbarqueAvecLocalisation",
        boost::function< bool ( const std::string&, const TER_Localisation* ) >( boost::bind( &DEC_AgentFunctions::CanConstructObjectWithLoadedAndLocalisation, boost::cref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Agent_PeutConstruireContournementObjet",
        boost::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::CanBypassObject, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_PeutDetruireObjet",
        boost::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::CanDestroyObject, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_PeutValoriserObjet",
        boost::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::CanMineObject, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_PeutDevaloriserObjet",
        boost::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::CanDemineObject, boost::cref( GetPion() ), _1 ) ) );
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
        boost::function< bool ( const PHY_DotationCategory* ) >( boost::bind( &DEC_AgentFunctions::HasDotation, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_CanUseDotation",
        boost::function< bool ( const PHY_DotationCategory* ) >( boost::bind( &DEC_AgentFunctions::CanUseDotation, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_GetDotation",
        boost::function< const PHY_DotationCategory*( unsigned int ) >( [&]( unsigned int value ){
            return DEC_AgentFunctions::GetDotation( GetPion(), value );
    }));
    RegisterFunction( "DEC_Agent_IlluminateRange", boost::bind( &DEC_AgentFunctions::GetIlluminatingRange, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_CanExtinguish",
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::AgentCanExtinguish, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_CanFly", boost::bind( &DEC_AgentFunctions::AgentCanFly, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_GetFuelDotationNumber",  boost::function< double ( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::GetFuelDotationNumber, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_AutonomieEnDeplacement", boost::bind( &DEC_AgentFunctions::TimeLeftForMoving, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_TempsPourParcourirDistanceEnLigneDroite",
        boost::function< float ( float ) >( boost::bind( &DEC_AgentFunctions::TimeToMoveDistance, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_TempsPourDegagerUnObjet",
        boost::function< float ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::EstimatedWorkTime, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_AutomateForcerReddition",
        boost::function< bool( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::SurrenderAutomat, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ChangerAmbianceEnSurete",
        boost::function< void ( bool ) >( boost::bind( &DEC_AgentFunctions::SetToAmbianceSafety, boost::ref( GetPion() ), _1 ) ) );

    // NBC
    RegisterFunction( "DEC_Agent_EstAgentNBC", boost::bind( &DEC_AgentFunctions::IsAgentNBC , boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EstContamine", boost::bind( &DEC_AgentFunctions::IsContaminated , boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EstEmpoisonne", boost::bind( &DEC_AgentFunctions::IsIntoxicated , boost::cref( GetPion() ) ) );

    RegisterFunction( "DEC_Agent_MettreTenueProtectionNBC", boost::bind( &DEC_AgentFunctions::WearNbcProtectionSuit, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_EnleverTenueProtectionNBC", boost::bind( &DEC_AgentFunctions::RemoveNbcProtectionSuit, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_NiveauProtectionNBC", boost::bind( &DEC_AgentFunctions::GetNbcSuitLevel, boost::ref( GetPion() ) ) );

    RegisterFunction( "DEC_ConnaissanceObjet_DemandeDeDecontamination",
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::QueueForDecontamination, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceObjet_DemandeDeDecontaminationSurPion",
        boost::function< int( DEC_Decision_ABC*, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::QueueUnitForDecontamination, _1, _2 ) ) );

    RegisterFunction( "DEC_Agent_DecontamineConnaissance",
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::QueueKnowledgeForDecontamination, _1, _2 ) ) );

    RegisterFunction( "DEC_Agent_ImmuniserNbc", boost::bind( &DEC_AgentFunctions::ImmunizeAgent, boost::ref( GetPion() ) ) ); // deprecated
    RegisterFunction( "DEC_Agent_StopImmuniserNbc", boost::bind( &DEC_AgentFunctions::StopImmunizeAgent, boost::ref( GetPion() ) ) ); // deprecated

    // Blackout
    RegisterFunction( "DEC_Agent_PasserEnSilenceRadio", boost::bind( &DEC_AgentFunctions::ActivateBlackout, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_PasserEnSilenceRadioPartiel", boost::bind( &DEC_AgentFunctions::ActivatePartialBlackout, boost::ref( GetPion() ), true ) );
    RegisterFunction( "DEC_Agent_ArreterSilenceRadio", boost::bind( &DEC_AgentFunctions::DeactivateBlackout, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_PasserEnEmissionRestreinte", boost::bind( &DEC_AgentFunctions::ActivatePartialBlackout, boost::ref( GetPion() ), false ) );

    // Knowledge sharing
    RegisterFunction( "DEC_EnableSharedPerception", boost::function< void( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::EnableSharedPerception, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_DisabledSharedPerception", boost::function< void( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::DisabledSharedPerception, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_EnableSharedPerceptionWithKnowledge", &DEC_AgentFunctions::EnableSharedPerceptionWithKnowledge );
    RegisterFunction( "DEC_DisabledSharedPerceptionWithKnowledge", &DEC_AgentFunctions::DisabledSharedPerceptionWithKnowledge );
    RegisterFunction( "DEC_Knowledge_EnableSharedPerceptionWithKnowledge", &DEC_AgentFunctions::KnowledgeEnableSharedPerceptionWithKnowledge );
    RegisterFunction( "DEC_Knowledge_DisabledSharedPerceptionWithKnowledge", &DEC_AgentFunctions::KnowledgeDisabledSharedPerceptionWithKnowledge );

    RegisterFunction( "DEC_EnableSharingKnowledges", boost::function< void( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::EnableSharingKnowledges, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_DisabledSharingKnowledges", boost::function< void( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::DisabledSharingKnowledges, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_EnableSharingKnowledgesWithKnowledge", &DEC_AgentFunctions::EnableSharingKnowledgesWithKnowledge );
    RegisterFunction( "DEC_DisabledSharingKnowledgesWithKnowledge", &DEC_AgentFunctions::DisabledSharingKnowledgesWithKnowledge );

    RegisterFunction( "DEC_Knowledge_CommunicateWithKnowledgeGroup", &DEC_AgentFunctions::KnowledgeCommunicate );

    // Misc
    RegisterFunction( "DEC_ModulationVitesseCourante",
        boost::function< void( double ) >( boost::bind( &DEC_MiscFunctions::SetCurrentSpeedModificator, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ModulationVitesseMax",
        boost::function< void( double ) >( boost::bind( &DEC_MiscFunctions::SetMaxSpeedModificator, boost::ref( GetPion() ), _1 ) ) );
    // Elongation factor concept was removed but we keep the DEC function around
    // to avoid breaking old models. This should be removed after their migration.
    RegisterFunction( "DEC_Elongation"     , boost::function< void(double) >( []( double ) {} ));
    RegisterFunction( "DEC_ReleverPion"    ,
        boost::function< bool( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::RelievePion, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_PeutReleverPion",
        boost::function< bool( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::CanRelievePion, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Suicide", boost::bind( &DEC_AgentFunctions::Suicide, boost::ref( GetPion() ) ) );

    // Agent knowledge accessors
    RegisterFunction( "DEC_ConnaissanceAgent_NiveauDePerceptionCourant",
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetCurrentPerceptionLevel, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_EstPercuParUnite",
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsPerceivedByAgent, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_EstUnEnnemi",
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsAnEnemy, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_EstUnAllie",
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsAFriend, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_PercoitUnite",
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsPerceivingAgent, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_EstAPorteDeCapteurDansCone",
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent >, const MT_Vector2D*, double ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsInDetectionCone, boost::cref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_PeutEtreIllumine",
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::CanBeIlluminate, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_PeutIlluminer",
        boost::function< bool() >( boost::bind( &DEC_KnowledgeAgentFunctions::CanIlluminate, boost::cref( GetPion() ) ) ) );
    RegisterFunction( "DEC_ConnaissanceAgent_EstTransporte",
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsTransported, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_KnowledgeAgent_IsInObject",
        boost::function< bool( const std::string&, boost::shared_ptr< DEC_Knowledge_Agent >, int ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsInObject, boost::cref( GetPion() ), _1, _2, _3 ) ) );

    // Object knowledges accessors
    RegisterFunction( "DEC_ConnaissanceObjet_EstUnEnnemi",
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsAnEnemy, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceObjet_EstUnAllie",
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsAFriend, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceObjet_Reconnaitre",
        boost::function< void( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::Recon, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceObjet_PeutEtreOccupe",
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::CanBeOccupied, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceObjet_PeutEtreAnime",
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::CanBeAnimated, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceObjet_Degrader",
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Object >, float, const PHY_DotationCategory*) >( boost::bind( &DEC_KnowledgeObjectFunctions::DamageObject, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_ConnaissanceObjet_NiveauDePerceptionCourant",
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::GetCurrentPerceptionLevel, boost::cref( GetPion() ), _1 ) ) );

    // Population knowledges accessors
    RegisterFunction( "DEC_KnowledgePopulation_Domination",
        boost::function< std::pair< float, int > (int) > (boost::bind( &DEC_KnowledgePopulationFunctions::GetDominationState< MIL_AgentPion >, boost::ref( GetPion()), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_Attitude",
        boost::function< unsigned int (int) > ( boost::bind( &DEC_KnowledgePopulationFunctions::GetAttitude< MIL_AgentPion >, boost::ref( GetPion()), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_Reconnaitre",
        boost::function< void (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::Recon , boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_EstReconnu",
        boost::function< bool(int)> ( boost::bind ( &DEC_KnowledgePopulationFunctions::IsRecon< MIL_AgentPion > , boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_EstPercueParUnite",
        boost::function< bool (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::IsPerceivedByAgent, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_Dangerosite",
        boost::function< float(int)>(boost::bind ( &DEC_KnowledgePopulationFunctions::GetDangerosity , boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_EstDansZone",
        boost::function< bool (int, TER_Localisation*) >(boost::bind( &DEC_KnowledgePopulationFunctions::IsInZone < MIL_AgentPion >, boost::cref( GetPion() ), _1 , _2 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_PointPlusProche",
        boost::function< boost::shared_ptr<MT_Vector2D> (int) >(boost::bind( &DEC_KnowledgePopulationFunctions::ClosestPoint, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_PointEnTeteDeFlux",
        boost::function< boost::shared_ptr<MT_Vector2D> (const DEC_Decision_ABC&, int) >(boost::bind( &DEC_KnowledgePopulationFunctions::GetFlowHead, _1, _2 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_Securiser",
        boost::function< void (int) >(boost::bind( &DEC_KnowledgePopulationFunctions::Secure, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_PointSecurisation",
        boost::function< boost::shared_ptr<MT_Vector2D> (int) > (boost::bind( &DEC_KnowledgePopulationFunctions::SecuringPoint, boost::cref( GetPion() ), _1  ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_EstEnnemi",
        boost::function< bool (int) >(boost::bind( &DEC_KnowledgePopulationFunctions::IsEnemy, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_Exterminer",
        boost::function< int (int, float, const PHY_DotationCategory*) >(boost::bind( &DEC_KnowledgePopulationFunctions::Exterminate, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_GetAdhesionPopulation",
        boost::function< float (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::GetCrowdAffinity , boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_GetAttitudePopulation",
        boost::function< int (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::GetCrowdAttitude , boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StartHidingInCrowd",
        boost::function< void (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::StartHidingInCrowd, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_StopHidingInCrowd",
        boost::function< void (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::StopHidingInCrowd, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_GetAgentsHiddenInCrowd",
        boost::function< std::vector< DEC_Decision_ABC* > (int) >( boost::bind( &DEC_KnowledgePopulationFunctions::GetAgentsHiddenInCrowd, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_AssignMissionCrowdParameter",
        boost::function< void( boost::shared_ptr< MIL_Mission_ABC >, const std::string&, int ) >( boost::bind( &MIL_MissionParameterFactory::SetCrowdKnowledgeParameter, this, _1, _2, _3 ) ) );
    RegisterFunction( "DEC_AssignMissionCrowdListParameter",
        boost::function< boost::shared_ptr<MIL_MissionParameter_ABC>( int ) >( boost::bind( &MIL_MissionParameterFactory::CreatePopulationKnowledge, this, _1 ) ) );
    RegisterFunction( "DEC_Crowd_ExtractWoundedFromCrowd",
        boost::function< unsigned int( int, const MT_Vector2D* ) >( boost::bind( &DEC_KnowledgePopulationFunctions::ExtractWoundedFromCrowd, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Crowd_ExtractDeadFromCrowd",
        boost::function< unsigned int( int, const MT_Vector2D* ) >( boost::bind( &DEC_KnowledgePopulationFunctions::ExtractDeadFromCrowd, boost::cref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Crowd_GetHumansFromAllTypes",
        boost::function< std::vector< unsigned int >( int ) >( boost::bind( &DEC_KnowledgePopulationFunctions::GetHumansFromAllTypes< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Crowd_HealWoundedHumans",
        boost::function< bool( int ) >( boost::bind( &DEC_KnowledgePopulationFunctions::HealWoundedHumans, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_Verrouiller",
        boost::function< void (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::Lock, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissancePopulation_Deverrouiller",
        boost::function< void (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::Unlock, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_BlocUrbain",
        boost::function< void( const directia::tools::binders::ScriptRef& ) >( boost::bind( &DEC_KnowledgeFunctions::GetUrbanBlock, boost::ref( brain ), boost::ref( GetPion() ), initQueryFunction, _1 ) ) );
    RegisterFunction( "DEC_Connaissances_BlocUrbainPourPosition",
        boost::function< const MIL_UrbanObject_ABC*( boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_KnowledgeFunctions::GetUrbanBlockForPosition, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_BlocUrbainDansZone",
        boost::function< T_UrbanObjectVector( TER_Localisation* )>( boost::bind( &DEC_KnowledgeFunctions::GetUrbanBlockInZone, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceUrbanBlock_NiveauDeReconnaissanceCourant", boost::function< float( MIL_UrbanObject_ABC* )>( boost::bind( &DEC_UrbanObjectFunctions::GetCurrentRecceProgress, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_ConnaissanceBlocUrbain_Traficabilite",
        boost::function< double( MIL_UrbanObject_ABC* ) >( boost::bind( &DEC_UrbanObjectFunctions::GetPathfindCost, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_DetruireBlocUrbain",
        boost::function< void( MIL_UrbanObject_ABC*, const PHY_DotationCategory* )>( boost::bind( &DEC_UrbanObjectFunctions::DestroyUrbanBlock, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_KnowledgePopulation_GetBarycenter",
        boost::function< boost::shared_ptr< MT_Vector2D >( int ) >( boost::bind( &DEC_KnowledgePopulationFunctions::GetBarycenter< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );

    // Global knowledge
    RegisterFunction( "DEC_Connaissances_UnitesDetecteesDansFuseau", boost::bind( &DEC_KnowledgeFunctions::GetDetectedAgentsInFuseau, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesDetecteesDansZone",
        boost::function< T_ConstKnowledgeAgentVector( const TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetDetectedAgentsInZone, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesPrenantAPartie", boost::bind( &DEC_KnowledgeFunctions::GetAgentsAttacking, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesPercues", boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesPerceived, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_Observable",
        boost::function< void( const directia::tools::binders::ScriptRef& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObservableKnowledge, boost::ref( brain ), boost::ref( GetPion() ), initQueryFunction, _1 ) ) );
    RegisterFunction( "DEC_Connaissances_Destroyable",
        boost::function< void( const directia::tools::binders::ScriptRef& ) >( boost::bind( &DEC_KnowledgeFunctions::GetDestroyableKnowledge, boost::ref( brain ), boost::ref( GetPion() ), initQueryFunction, _1 ) ) );//@TODO MGD same function than Observable for the moment
    RegisterFunction( "DEC_Connaissances_Identifiable",
        boost::function< void( const directia::tools::binders::ScriptRef& ) >( boost::bind( &DEC_KnowledgeFunctions::GetIdentifiableKnowledge, boost::ref( brain ), boost::ref( GetPion() ), initQueryFunction, _1 ) ) );//@TODO MGD same function than Observable for the moment
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesPercuesParPion",
        boost::function< T_ConstKnowledgeAgentVector( const DEC_Decision_ABC* ) >( boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesPercuesDansFuseau", boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedInFuseau, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesDansFuseau", boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesInFuseau, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesDansZone",
        boost::function< T_ConstKnowledgeAgentVector( TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesInZone, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesBlesseesDansZone",
        boost::function< T_ConstKnowledgeAgentVector( TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetWoundedUnitsInZone, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesCivilesDansZone",
        boost::function< T_ConstKnowledgeAgentVector( TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetCiviliansInZone, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesDansCercle",
        boost::function< T_ConstKnowledgeAgentVector( const MT_Vector2D*, float ) >( boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesInCircle, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesBlesseesOuTueesDansCercle",
        boost::function< T_ConstKnowledgeAgentVector( const MT_Vector2D*, float ) >( boost::bind( &DEC_KnowledgeFunctions::GetWoundedOrDeadUnitsInCircle, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesDansBlocUrbain",
        boost::function< T_ConstKnowledgeAgentVector( MIL_UrbanObject_ABC* ) >( boost::bind( &DEC_UrbanObjectFunctions::GetLivingEnemiesInBU, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_RefugiesAProximite",
        boost::function< T_ConstKnowledgeAgentVector( double ) > ( boost::bind( &DEC_KnowledgeFunctions::GetNearbyRefugees, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_TerroristsAProximite",
        boost::function< T_ConstKnowledgeAgentVector( double ) > ( boost::bind( &DEC_KnowledgeFunctions::GetNearbyTerrorists, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesRenduesAProximite",
        boost::function< T_ConstKnowledgeAgentVector ( double ) > ( boost::bind( &DEC_KnowledgeFunctions::GetNearbySurrenderedAgents, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_PresenceEnnemieDansCercle",
        boost::function< bool( const MT_Vector2D*, double ) >( boost::bind( &DEC_KnowledgeFunctions::EnemyPresenceInCircle, boost::cref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Connaissances_UnitesAmiesDansZone",
        boost::function< T_ConstKnowledgeAgentVector( const TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetFriendsInZone< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_PartageConnaissancesAvec",
        boost::function< void( DEC_Decision_ABC*, float ) >( boost::bind( &DEC_KnowledgeFunctions::ShareKnowledgesWith< MIL_AgentPion >, boost::cref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Connaissances_PartageConnaissancesDansZoneAvec",
        boost::function< void( DEC_Decision_ABC*, const MT_Vector2D*, float ) >( boost::bind( &DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith< MIL_AgentPion >, boost::cref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Knowledges_ObjectsInCircle",
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const MT_Vector2D*, double, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInCircle< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3, false ) ) );
    RegisterFunction( "DEC_Knowledges_AllObjectsInCircle",
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const MT_Vector2D*, double, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInCircle< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3, true ) ) );
    RegisterFunction( "DEC_ObjectKnowledgesInZone",
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const TER_Localisation*, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInZone< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_ObjectKnowledgesIntersectingInZone",
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const TER_Localisation*, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsIntersectingInZone< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Connaissances_ObjetsDansFuseau",
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const std::string& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInFuseau< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_CollisionsObjets", boost::bind( &DEC_KnowledgeFunctions::GetObjectsColliding, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_CollisionsDesastres", boost::bind( &DEC_KnowledgeFunctions::GetCollidingDisasters, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_Desastres", boost::bind( &DEC_KnowledgeFunctions::GetDisasters, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_CollisionsObjetsDeType",
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const std::string& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsCollidingFromType, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_ObjetLePlusProche",
        boost::function< boost::shared_ptr< DEC_Knowledge_Object >( const std::string& ) >( boost::bind( &DEC_KnowledgeFunctions::GetClosestObject, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_ObjetAmiLePlusProche",
        boost::function< boost::shared_ptr< DEC_Knowledge_Object >( const std::string& ) >( boost::bind( &DEC_KnowledgeFunctions::GetClosestFriendObject, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_CollisionsPopulations", boost::bind( &DEC_KnowledgeFunctions::GetPopulationsColliding, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_PopulationsPrenantAPartie", boost::bind( &DEC_KnowledgeFunctions::GetPopulationsAttacking, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Connaissances_EstPrisAPartieParPopulation",
        boost::function< bool( const int ) >( boost::bind( &DEC_KnowledgeFunctions::IsPopulationAttacking, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Connaissances_Populations", boost::bind( &DEC_KnowledgeFunctions::GetPopulations< MIL_AgentPion >, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_ChangerNombreIndividuArmeDansFoule",
        boost::function< void( const int, const double ) >( boost::bind( &DEC_KnowledgePopulationFunctions::ChangeArmedIndividuals, boost::ref( GetPion() ), _1, _2 ) ) );

    // Ordres de conduite
    RegisterFunction( "DEC_ReleverUnite",
        boost::function< bool( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::RelievePion, boost::ref( GetPion() ), _1 ) ) );

    // Limas / Mission
    RegisterFunction( "DEC_NouvelleMission", boost::bind( &DEC_OrdersFunctions::IsNewMissionStarted< MIL_AgentPion >, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_FinMission", boost::bind( &DEC_OrdersFunctions::FinishMission< MIL_AgentPion >, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_GetLima",
        boost::function< unsigned int( unsigned int ) >( boost::bind( &DEC_OrdersFunctions::GetLima< MIL_Agent_ABC >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_GetLimasFromType",
        boost::function< std::vector< unsigned int >( unsigned int ) >( boost::bind( &DEC_OrdersFunctions::GetLimasFromType< MIL_Agent_ABC >, boost::ref( GetPion() ), _1 ) ) );

    RegisterFunction( "DEC_SetMissionLimaFlag"            ,
        boost::function< void (unsigned int, bool)> ( boost::bind( &DEC_OrdersFunctions::PionSetMissionLimaFlag, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_GetMissionLimaFlag"            ,
        boost::function< bool (unsigned int)> ( boost::bind( &DEC_OrdersFunctions::GetMissionLimaFlag < MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_SetMissionLimaFlagHoraire"     ,
        boost::function< void (unsigned int, bool)> ( boost::bind( &DEC_OrdersFunctions::PionSetMissionLimaScheduleFlag, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Fuseau"    ,
        boost::function< const MIL_Fuseau& ()> ( boost::bind( &DEC_OrdersFunctions::GetFuseau, boost::cref( *this ) ) ) );

    // Etat décisionnel
    RegisterFunction( "DEC_Agent_ChangeEtatRapportDeForce",
        boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyForceRatioStateChanged, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ChangeEtatROE",
        boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyRulesOfEngagementStateChanged, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_GetEtatROE",
        boost::bind( &DEC_AgentFunctions::GetRulesOfEngagementState, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Agent_ChangeEtatROEPopulation",
        boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyRulesOfEngagementPopulationStateChanged, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ChangeEtatCombatDeRencontre",
        boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyCloseCombatStateChanged, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ChangeEtatOperationnel",
        boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyOperationalStateChanged, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_ChangeDisponibiliteAuTirIndirect",
        boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyIndirectFireAvailabilityChanged, boost::ref( GetPion() ), _1 ) ) );

    // Facteurs humains
    RegisterFunction( "DEC_FacteurHumain_Fatigue", boost::bind( &DEC_AgentFunctions::GetHumanFactorTiredness, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_FacteurHumain_Moral", boost::bind( &DEC_AgentFunctions::GetHumanFactorMorale, boost::ref( GetPion() ) ) );

    // Calculs de position
    RegisterFunction( "DEC_Geometrie_PositionInterception",
        boost::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent >, MT_Vector2D*, double )>( boost::bind( &DEC_GeometryFunctions::GetInterceptionPosition, boost::cref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Agent_PositionInterception",
        boost::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_AgentFunctions::GetInterceptionPoint, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPositionAppui",
        boost::function< boost::shared_ptr< MT_Vector2D >( DEC_Decision_ABC*, double ) >( boost::bind( &DEC_GeometryFunctions::ComputeSupportPosition, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPositionEmbuscade",
        boost::function< boost::shared_ptr< MT_Vector2D >( MT_Vector2D*, MT_Vector2D*, double ) >( boost::bind( &DEC_GeometryFunctions::ComputeAmbushPosition, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPositionSurete",
        boost::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent >, double ) >( boost::bind( &DEC_GeometryFunctions::ComputeSafetyPosition, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPositionSureteAvecPopulation",
        boost::function< boost::shared_ptr< MT_Vector2D >( unsigned int, double ) >( boost::bind( &DEC_GeometryFunctions::ComputeSafetyPositionWithPopulation, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPositionSureteAvecObjectif",
        boost::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent >, double, MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::ComputeSafetyPositionWithObjective, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPointProcheLocalisationDansFuseau",
        boost::function< boost::shared_ptr< MT_Vector2D >( const TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau< MIL_AgentPion>, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPointProcheLocalisationNonClippeeDansFuseau" ,
        boost::function< boost::shared_ptr< MT_Vector2D >( const TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInFuseau< MIL_AgentPion>, boost::cref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerBarycentreLocalisationDansFuseau",
        boost::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerEntreeFuseauLaPlusProche",
        boost::bind( &DEC_GeometryFunctions::ComputeNearestFuseauEntryPoint, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Geometry_SplitLocalisation",
        boost::function< std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( TER_Localisation*, unsigned int, MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisationInParts< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Geometry_SplitLocalisationSurface",
        boost::function< std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( TER_Localisation*, unsigned int, MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisationInSurfaces< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Geometry_RecursiveSplitLocalisationSurface",
        boost::function< std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( TER_Localisation*, unsigned int ) >( boost::bind( &DEC_GeometryFunctions::RecursiveSplitLocalisationInSurfaces< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Geometrie_DecoupeFuseauEnTroncons",
        boost::function< std::vector< boost::shared_ptr< TER_Localisation > >( const double ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisationInSections< MIL_AgentPion >, boost::ref( GetPion() ), _1  ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPositionObstacle",
        boost::function< boost::shared_ptr< MT_Vector2D >( MT_Vector2D*, const std::string&, double) >( boost::bind( &DEC_GeometryFunctions::ComputeObstaclePositionForUnit, boost::cref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPointArrivee",
        boost::bind( &DEC_GeometryFunctions::ComputeDestPoint< MIL_AgentPion >, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPointDepart",
        boost::bind( &DEC_GeometryFunctions::ComputeStartPoint< MIL_AgentPion >, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Geometrie_EstPointDansFuseau",
        boost::function< bool( MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::IsPointInFuseau< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerBarycentreListeConnaissancesAgents",
        boost::function< boost::shared_ptr< MT_Vector2D >( const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& ) >( boost::bind( &DEC_GeometryFunctions::ComputeKnowledgeAgentBarycenter, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerPositionParRapportALima",
        boost::function< boost::shared_ptr< MT_Vector2D >( int, float ) >( boost::bind( &DEC_GeometryFunctions::ComputePointBeforeLima< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Geometrie_CalculerTrafficableBarycentreLocalisation",
        boost::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeTrafficableLocalisationBarycenter, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_IsPointInUrbanBlockTrafficable",
        boost::function< bool( MT_Vector2D& ) >( boost::bind( &DEC_GeometryFunctions::IsPointInUrbanBlockTrafficable, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Geometrie_GetLeavingAreaPosition",
        boost::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::GetLeavingAreaPosition< MIL_AgentPion>, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Agent_EstDansLeFuseau", boost::bind( &DEC_AgentFunctions::AgentHasFuseau , boost::cref( GetPion() ) ) );

    //Keypoint
    RegisterFunction( "DEC_Crossroads",
        boost::function< void( const directia::tools::binders::ScriptRef& ) >( boost::bind( &DEC_TerrainFunctions::GetCrossroads, boost::ref( brain ), boost::ref( GetPion() ), initQueryFunction, _1 ) )  );
    RegisterFunction( "DEC_FindSafetyPositions",
        boost::function< std::vector< boost::shared_ptr< MT_Vector2D > >( float, float ) >( boost::bind( &DEC_TerrainFunctions::FindSafetyPositionsWithinCircle, boost::ref( GetPion() ), _1, _2 ) )  );

    // Hierarchie
    RegisterFunction( "DEC_Pion_PionsAvecPC", boost::bind( &DEC_AgentFunctions::GetPionsWithPC, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Pion_PionsSansPC", boost::bind( &DEC_AgentFunctions::GetPionsWithoutPC, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Pion_PionsAvecPCCommunication", boost::bind( &DEC_AgentFunctions::GetCommunicationPionsWithPC, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Pion_PionsSansPCCommunication", boost::bind( &DEC_AgentFunctions::GetCommunicationPionsWithoutPC, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Pion_PionPC", boost::bind( &DEC_AgentFunctions::GetPionPC, boost::cref( GetPion() ) ) );
    RegisterFunction( "DEC_Pion_ChangeAutomate",
        boost::function< bool( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::ChangeAutomate, boost::ref( GetPion() ), _1 ) ) );

    // Logistique
    RegisterFunction( "DEC_StartPreterVSRAM",
        boost::function< unsigned int( DEC_Decision_ABC*, DEC_Decision_ABC*, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionLendCollectionComposantes, DEC_Decision_ABC*, DEC_Decision_ABC*, unsigned int >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_StartPreterRemorqueurs",
        boost::function< unsigned int( DEC_Decision_ABC*, DEC_Decision_ABC*, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionLendHaulerComposantes, DEC_Decision_ABC*, DEC_Decision_ABC*, unsigned int >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_StartPreterComposantes",
        boost::function< unsigned int( DEC_Decision_ABC*, DEC_Decision_ABC*, PHY_ComposanteTypePion*, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionLendSpecificComposantes, DEC_Decision_ABC*, DEC_Decision_ABC*, PHY_ComposanteTypePion*, unsigned int >, boost::ref( GetPion() ), _1, _2, _3, _4 ) ) );
    RegisterFunction( "DEC_RecupererComposantes",
        boost::function< void( const DEC_Decision_ABC*, PHY_ComposanteTypePion*, const unsigned int ) >( boost::bind( &DEC_LogisticFunctions::UndoLendSpecificComposantes, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_RecupererVSRAM",
        boost::function< void( const DEC_Decision_ABC*, const unsigned int ) >( boost::bind( &DEC_LogisticFunctions::UndoLendCollectionComposantes, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_RecupererRemorqueurs",
        boost::function< void( const DEC_Decision_ABC*, const unsigned int ) >( boost::bind( &DEC_LogisticFunctions::UndoLendHaulerComposantes, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Pion_PcDeTC2",
        boost::bind( &DEC_LogisticFunctions::PionGetPcTC2, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_ChangeValeurDotations2",
        boost::function< void( int, const double ) >( boost::bind( &DEC_LogisticFunctions::ChangeDotationsValueUsingTC2, boost::ref( GetPion() ), _1, _2, -1 ) ) );
    RegisterFunction( "DEC_ChangeValeurDotations3",
        boost::function< void( int, const double, int ) >( boost::bind( &DEC_LogisticFunctions::ChangeDotationsValueUsingTC2, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_CreateBreakdown",
        boost::function< bool( const PHY_ComposanteTypePion*, unsigned int ) >( boost::bind( &DEC_AgentFunctions::CreateBreakdown, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_StartConsumingResources",
        boost::function< unsigned int( unsigned int, double, double ) >(
            [&]( unsigned int category, double value, double duration ) {
                return DEC_ActionFunctions::StartAction< PHY_ActionConsumeResources >( GetPion(), category, value, duration, MIL_Time_ABC::GetTime().GetTickDuration() );
    }));

    // Transport / Heliportage
    RegisterFunction( "DEC_Transport_AjouterPion",
        boost::function< void( DEC_Decision_ABC*, bool ) >( boost::bind( &DEC_ActionFunctions::Transport_AddPion, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Transport_AjouterPions",
        boost::function< void( const std::vector< DEC_Decision_ABC* >&, bool ) >( boost::bind( &DEC_ActionFunctions::Transport_AddPions, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Start_TransportEmbarquer", boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTransportLoad >, boost::ref( GetPion() ) ) );
    RegisterFunction( "DEC_Start_TransportDebarquer",
        boost::function< unsigned int( MT_Vector2D* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTransportUnload, MT_Vector2D* >, boost::ref( GetPion() ), _1 ) ) );

    RegisterFunction( "DEC_Transport_EmbarquerDansTransporteurSansDelais", boost::function< void ( const DEC_Decision_ABC* ) >( boost::bind( &DEC_ActionFunctions::Transport_MagicLoadPionInCarrier, boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Transport_DebarquerDeTransporteurSansDelais", boost::function< void () >( boost::bind( &DEC_ActionFunctions::Transport_MagicUnloadPionFromCarrier, boost::ref( GetPion() ) ) ) );
    RegisterFunction( "DEC_Transport_Transporteur", boost::function< DEC_Decision_ABC* () >( boost::bind( &DEC_ActionFunctions::Transport_GetCarrier, boost::ref( GetPion() ) ) ) );

    RegisterFunction( "DEC_Transport_EmbarquerPionSansDelais",
        boost::function< void( const DEC_Decision_ABC*, bool ) >( boost::bind( &DEC_ActionFunctions::Transport_MagicLoadPion, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Transport_EmbarquerPionsSansDelais",
        boost::function< void (const std::vector< DEC_Decision_ABC* >&, bool) >( boost::bind( &DEC_ActionFunctions::Transport_MagicLoadPions, boost::ref( GetPion()) , _1, _2 ) ) );
    RegisterFunction( "DEC_Transport_DebarquerPionSansDelais",
        boost::function<void(const DEC_Decision_ABC*)>( boost::bind( &DEC_ActionFunctions::Transport_MagicUnloadPion , boost::ref( GetPion()) , _1 ) ) );
    RegisterFunction( "DEC_Transport_DebarquerPionsSansDelais",
        boost::function<void(const std::vector< DEC_Decision_ABC* >&)>( boost::bind( &DEC_ActionFunctions::Transport_MagicUnloadPions , boost::ref( GetPion()) , _1 ) ) );
    RegisterFunction( "DEC_Transport_EstTermine",
        boost::bind( &DEC_ActionFunctions::Transport_IsFinished , boost::ref( GetPion()) ) );
    RegisterFunction( "DEC_Transport_Annuler",
        boost::bind( &DEC_ActionFunctions::Transport_Cancel , boost::ref( GetPion()) ) );
    RegisterFunction( "DEC_Transport_EnCoursDeTransport",
        boost::bind( &DEC_ActionFunctions::Transport_IsTransporting, boost::ref( GetPion() ) ) );

    RegisterFunction( "DEC_Agent_PeutTransporterPion",
        boost::function< bool( const DEC_Decision_ABC*, bool ) >( boost::bind( &DEC_ActionFunctions::AgentCanTransportPion, this, _1, _2 ) ) );
    RegisterFunction( "DEC_Agent_TransportNombreAllerRetour",
        boost::function< double( const DEC_Decision_ABC*, bool ) >( boost::bind( &DEC_ActionFunctions::GetNumberOfRoundTripToTransportPion, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Connaissance_TransportNombreAllerRetour",
        &DEC_ActionFunctions::GetNumberOfRoundTripToTransportKnowledge );

    RegisterFunction( "DEC_StartEmbarquerFouleDUneConcentration",
        boost::function< unsigned int( int, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< crowdtransport::PHY_ActionLoadCrowd, int, unsigned int >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_StartDebarquerFouleSurPosition",
        boost::function< unsigned int( int, MT_Vector2D* ) >( boost::bind( &DEC_ActionFunctions::StartAction< crowdtransport::PHY_ActionUnloadCrowd, int, MT_Vector2D* >, boost::ref( GetPion() ), _1, _2 ) ) );

    // Prisonniers
    RegisterFunction( "DEC_Prisonniers_CapturerEtEmbarquer" ,
        boost::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > )>(boost::bind(&DEC_ActionFunctions::Prisoners_CaptureAndLoad , boost::ref( GetPion()) ,_1 ) ) );
    RegisterFunction( "DEC_Prisonniers_Debarquer"           ,
        boost::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > )>(boost::bind(&DEC_ActionFunctions::Prisoners_Unload , boost::ref(GetPion()), _1 ) ) );
    RegisterFunction( "DEC_Prisonniers_DebarquerDansCamp"   ,
        boost::function< void (boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< DEC_Knowledge_Object > )>(boost::bind(&DEC_ActionFunctions::Prisoners_UnloadInCamp , boost::ref( GetPion() ), _1, _2  ) ) );
    RegisterFunction( "DEC_Prisonniers_EstEmbarque"         ,
        boost::function< bool (boost::shared_ptr< DEC_Knowledge_Agent >) >(boost::bind(&DEC_ActionFunctions::PrisonnersRefugees_IsLoaded , boost::ref( GetPion() ), _1  ) ) );

    // Refugies
    RegisterFunction( "DEC_Refugies_OrienterEtEmbarquer" ,
        boost::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > ) > ( boost::bind(&DEC_ActionFunctions::Refugees_OrientateAndLoad , boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Refugies_Debarquer"           ,
        boost::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > ) > ( boost::bind(&DEC_ActionFunctions::Refugees_Unload , boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_Refugies_DebarquerDansCamp"   ,
        boost::function< void ( boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< DEC_Knowledge_Object > )> ( boost::bind(&DEC_ActionFunctions::Refugees_UnloadInCamp , boost::ref( GetPion() ), _1, _2  ) ) );
    RegisterFunction( "DEC_Refugies_EstEmbarque"         ,
        boost::function< bool ( boost::shared_ptr< DEC_Knowledge_Agent > ) > ( boost::bind ( &DEC_ActionFunctions::PrisonnersRefugees_IsLoaded , boost::ref( GetPion() ), _1  ) ) );

    // Emergency functions
    RegisterFunction( "DEC_Start_EmergencyInfluence",
        boost::function< unsigned int( const std::string&, double ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionInfluence, const std::string&, double >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Start_EmergencyTriggerActivity",
        boost::function< unsigned int( const std::string&, double ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTriggerActivity, const std::string&, double >, boost::ref( GetPion() ), _1, _2 ) ) );
    RegisterFunction( "DEC_Start_EmergencyInfluenceInArea",
        boost::function< unsigned int( const std::string&, double, const TER_Localisation* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionInfluenceInArea, const std::string&, double, const TER_Localisation* >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Start_EmergencyTriggerActivityInArea",
        boost::function< unsigned int( const std::string&, double, const TER_Localisation* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTriggerActivityInArea, const std::string&, double, const TER_Localisation* >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    RegisterFunction( "DEC_Start_EmergencyUnloadActivity",
        boost::function< unsigned int( const std::string&, double ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionUnloadActivity, const std::string&, double >, boost::ref( GetPion() ), _1, _2 ) ) );

    // Representations
    RegisterFunction( "DEC_GetOrdersCategory",
        boost::bind( &DEC_MiscFunctions::GetOrdersCategory , boost::ref( *pEntity_ ) ) );
    RegisterFunction( "DEC_RemoveFromOrdersCategory",
        boost::function< void ( boost::shared_ptr< MIL_FragOrder > ) > ( boost::bind( &DEC_MiscFunctions::RemoveFromOrdersCategory , boost::ref( GetPion() ), _1 ) ) );
    RegisterFunction( "DEC_DeleteRepresentation",
        boost::function< void ( boost::shared_ptr< MIL_FragOrder > ) > ( boost::bind( &DEC_MiscFunctions::DeleteOrderRepresentation , boost::ref( GetPion() ), _1 ) ) );

    pEntity_->GetType().RegisterFunctions( brain, GetPion() );
    //
    // Critical Intelligence
    RegisterFunction( "DEC_ObtenirRenseignementCritiqueSurFoule",
         boost::function< std::string( int ) >(boost::bind( &DEC_KnowledgePopulationFunctions::GetCriticalIntelligence, boost::cref( GetPion() ), _1 ) ) );
}
