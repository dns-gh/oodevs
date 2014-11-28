//*****************************************************************************
//
// $Created: JVT 04-05-18 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Functions/DEC_ActionFunctions.cpp $
// $Author: Nld $
// $Modtime: 12/05/05 17:40 $
// $Revision: 13 $
// $Workfile: DEC_ActionFunctions.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_ActionFunctions.h"
#include "Decision/Brain.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Entities/Actions/PHY_Action_ABC.h"
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
#include "Entities/Agents/Actions/CrowdTransport/PHY_RoleAction_CrowdTransport.h"
#include "Entities/Agents/Actions/Transport/PHY_ActionTransportLoad.h"
#include "Entities/Agents/Actions/Transport/PHY_ActionTransportUnload.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
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
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Refugee/PHY_RoleInterface_Refugee.h"
#include "Entities/Agents/Roles/Logistic/PHY_RoleInterface_Supply.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationStock.h"
#include "Entities/Populations/Actions/PHY_Population_ActionMove.h"
#include "Entities/Populations/Actions/PHY_Population_ActionMoveAlong.h"
#include "Entities/Populations/Actions/PHY_Population_ActionFireOnPion.h"
#include "Entities/Populations/Actions/PHY_Population_ActionFireOnPions.h"
#include "Entities/Populations/Actions/PHY_Population_ActionBattle.h"
#include "Entities/Populations/Actions/PHY_Population_ActionUrbanDestruction.h"
#include "Entities/MIL_Entity_ABC.h"
#include "Entities/Objects/StockAttribute.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "MIL_Time_ABC.h"
#include "simulation_kernel/RefugeeActionsNotificationHandler_ABC.h"
#include <boost/foreach.hpp>

void DEC_ActionFunctions::Register( sword::Brain& brain )
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
            []( DEC_Decision_ABC* agent, unsigned int category, double value, double duration ) {
                return DEC_ActionFunctions::StartAction< PHY_ActionConsumeResources >( agent, category, value, duration, MIL_Time_ABC::GetTime().GetTickDuration() );
    }));

    // Emergency functions
    brain.RegisterFunction( "_DEC_Start_EmergencyInfluence", &DEC_ActionFunctions::StartAction< PHY_ActionInfluence, const std::string&, double > );
    brain.RegisterFunction( "_DEC_Start_EmergencyTriggerActivity", &DEC_ActionFunctions::StartAction< PHY_ActionTriggerActivity, const std::string&, double > );
    brain.RegisterFunction( "_DEC_Start_EmergencyInfluenceInArea", &DEC_ActionFunctions::StartAction< PHY_ActionInfluenceInArea, const std::string&, double, const TER_Localisation* > );
    brain.RegisterFunction( "_DEC_Start_EmergencyTriggerActivityInArea", &DEC_ActionFunctions::StartAction< PHY_ActionTriggerActivityInArea, const std::string&, double, const TER_Localisation* > );
    brain.RegisterFunction( "_DEC_Start_EmergencyUnloadActivity", &DEC_ActionFunctions::StartAction< PHY_ActionUnloadActivity, const std::string&, double > );

    // Transport / Heliportage
    brain.RegisterFunction( "_DEC_Start_TransportEmbarquer", &DEC_ActionFunctions::StartAction< PHY_ActionTransportLoad > );
    brain.RegisterFunction( "_DEC_Start_TransportDebarquer", &DEC_ActionFunctions::StartAction< PHY_ActionTransportUnload, MT_Vector2D* > );
    brain.RegisterFunction( "_DEC_StartEmbarquerFouleDUneConcentration", &DEC_ActionFunctions::StartAction< crowdtransport::PHY_ActionLoadCrowd, int, unsigned int > );
    brain.RegisterFunction( "_DEC_StartDebarquerFouleSurPosition", &DEC_ActionFunctions::StartAction< crowdtransport::PHY_ActionUnloadCrowd, int, MT_Vector2D* > );
    brain.RegisterFunction( "_DEC_Transport_AjouterPion", &DEC_ActionFunctions::Transport_AddPion );
    brain.RegisterFunction( "_DEC_Transport_AjouterPions", &DEC_ActionFunctions::Transport_AddPions );
    brain.RegisterFunction( "_DEC_Transport_EmbarquerDansTransporteurSansDelais", &DEC_ActionFunctions::Transport_MagicLoadPionInCarrier );
    brain.RegisterFunction( "_DEC_Transport_DebarquerDeTransporteurSansDelais", &DEC_ActionFunctions::Transport_MagicUnloadPionFromCarrier );
    brain.RegisterFunction( "_DEC_Transport_Transporteur", &DEC_ActionFunctions::Transport_GetCarrier );
    brain.RegisterFunction( "_DEC_Transport_EmbarquerPionSansDelais", &DEC_ActionFunctions::Transport_MagicLoadPion );
    brain.RegisterFunction( "_DEC_Transport_EmbarquerPionsSansDelais", &DEC_ActionFunctions::Transport_MagicLoadPions );
    brain.RegisterFunction( "_DEC_Transport_DebarquerPionSansDelais", &DEC_ActionFunctions::Transport_MagicUnloadPion );
    brain.RegisterFunction( "_DEC_Transport_DebarquerPionsSansDelais", &DEC_ActionFunctions::Transport_MagicUnloadPions );
    brain.RegisterFunction( "_DEC_Transport_EstTermine", &DEC_ActionFunctions::Transport_IsFinished );
    brain.RegisterFunction( "_DEC_Transport_Annuler", &DEC_ActionFunctions::Transport_Cancel );
    brain.RegisterFunction( "_DEC_Transport_EnCoursDeTransport", &DEC_ActionFunctions::Transport_IsTransporting );
    brain.RegisterFunction( "_DEC_Agent_PeutTransporterPion", &DEC_ActionFunctions::AgentCanTransportPion );
    brain.RegisterFunction( "_DEC_Agent_TransportNombreAllerRetour", &DEC_ActionFunctions::GetNumberOfRoundTripToTransportPion );

    // Prisonniers
    brain.RegisterFunction( "_DEC_Prisonniers_CapturerEtEmbarquer", &DEC_ActionFunctions::Prisoners_CaptureAndLoad );
    brain.RegisterFunction( "_DEC_Prisonniers_Debarquer", &DEC_ActionFunctions::Prisoners_Unload );
    brain.RegisterFunction( "_DEC_Prisonniers_DebarquerDansCamp", &DEC_ActionFunctions::Prisoners_UnloadInCamp );

    brain.RegisterFunction( "_DEC_PrisonniersRefugies_EstEmbarque", &DEC_ActionFunctions::PrisonnersRefugees_IsLoaded );

    // Refugies
    brain.RegisterFunction( "_DEC_Refugies_OrienterEtEmbarquer", &DEC_ActionFunctions::Refugees_OrientateAndLoad );
    brain.RegisterFunction( "_DEC_Refugies_Debarquer", &DEC_ActionFunctions::Refugees_Unload );
    brain.RegisterFunction( "_DEC_Refugies_DebarquerDansCamp", &DEC_ActionFunctions::Refugees_UnloadInCamp );

    brain.RegisterFunction( "_DEC_Orientate", &DEC_ActionFunctions::Orientate );

    brain.RegisterFunction( "_DEC_Stock_IsExtractPossible", &DEC_ActionFunctions::Stock_IsExtractPossible );
    brain.RegisterFunction( "_DEC_Stock_IsSupplyPossible",  &DEC_ActionFunctions::Stock_IsSupplyPossible );
    brain.RegisterFunction( "DEC_Stock_IsDistributePossible", &DEC_ActionFunctions::Stock_IsDistributePossible );

    brain.RegisterFunction( "DEC_Agent_GetTransportedUnits", &DEC_ActionFunctions::GetTransportedUnits );

    brain.RegisterFunction( "DEC_CanLoad", &DEC_ActionFunctions::CanLoad );
    brain.RegisterFunction( "DEC_RoundTripsLeftToTransportAgent", &DEC_ActionFunctions::GetNumberOfRoundTripsLeftToTransportPion );

    brain.RegisterFunction( "_DEC__StartDeplacement", &DEC_ActionFunctions::StartAction< PHY_Population_ActionMove, MT_Vector2D* > );
    brain.RegisterFunction( "_DEC_StartDeplacementItineraire", &DEC_ActionFunctions::StartAction< PHY_Population_ActionMoveAlong, std::vector< boost::shared_ptr< MT_Vector2D > > > );
    brain.RegisterFunction( "_DEC__StartTirSurPions", &DEC_ActionFunctions::StartAction< PHY_Population_ActionFireOnPions, float > );
    brain.RegisterFunction( "_DEC__StartTirSurPion", &DEC_ActionFunctions::StartAction< PHY_Population_ActionFireOnPion, float, unsigned int > );
    brain.RegisterFunction( "_DEC_StartTirSurPion", &DEC_ActionFunctions::StartAction< PHY_Population_ActionFireOnPion, float, DEC_Decision_ABC* > );
    brain.RegisterFunction( "_DEC_Population_DetruireBlocUrbain",  &DEC_ActionFunctions::StartAction< PHY_Population_ActionUrbanDestruction, MIL_UrbanObject_ABC* > );
    brain.RegisterFunction( "_DEC_StartAgresserFoule", &DEC_ActionFunctions::StartAction< PHY_Population_ActionBattle > );

    brain.RegisterFunction( "DEC_Agent_AgentPeutTransporterPion", &DEC_ActionFunctions::AgentCanTransportPion );
}

namespace
{
    bool IsNotCampKnowledgeOrHasLogisticCapacity( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object >& pCampKnowledge )
    {
        return( !pKnowledge || !pKnowledge->IsValid() || !pCampKnowledge || !pCampKnowledge->IsValid() || !pCampKnowledge->GetObjectKnown() ); //|| pCampKnowledge->GetObjectKnown()->Retrieve< LogisticCapacity >() != 0 )
    }
}

unsigned int DEC_ActionFunctions::StopAction( const DEC_Decision_ABC* caller, unsigned int actionId )
{
    return StopAction( caller->GetEntity(), actionId );
}

unsigned int DEC_ActionFunctions::StopAction( MIL_Entity_ABC& caller, unsigned int actionId )
{
    if( actionId != 0 )
        caller.UnregisterAction( actionId );
    return 0;
}

void DEC_ActionFunctions::SuspendAction( const DEC_Decision_ABC* caller, unsigned int actionId )
{
    if( auto action = caller->GetEntity().GetAction( actionId ) )
        action->Suspend();
}

void DEC_ActionFunctions::ResumeAction( const DEC_Decision_ABC* caller, unsigned int actionId )
{
     if( auto action = caller->GetEntity().GetAction( actionId ) )
        action->Resume();
}

unsigned int DEC_ActionFunctions::GetActionState( const DEC_Decision_ABC* caller, unsigned int actionId )
{
    if( auto action = caller->GetEntity().GetAction( actionId ) )
        return action->GetState();
    return PHY_Action_ABC::eError;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_CaptureAndLoad
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Prisoners_CaptureAndLoad( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    MIL_AgentPion& agent = callerAgent->GetPion();
    if( pKnowledge && pKnowledge->IsValid() && pKnowledge->GetAgentKnown().GetRole< surrender::PHY_RoleInterface_Surrender >().Capture( agent ) )
    {
        bool transportOnlyLoadable = false;
        agent.Apply( &transport::TransportNotificationHandler_ABC::MagicLoadPion, pKnowledge->GetAgentKnown(), transportOnlyLoadable );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_Unload
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Prisoners_Unload( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent >  pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
    {
        pKnowledge->GetAgentKnown().GetRole< surrender::PHY_RoleInterface_Surrender >().Release();
        callerAgent->GetPion().GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( pKnowledge->GetAgentKnown() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_UnloadInCamp
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Prisoners_UnloadInCamp( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > pCampKnowledge  )
{
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        return;
    MIL_AgentPion& agent = callerAgent->GetPion();
    agent.GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( pKnowledge->GetAgentKnown() );
    pKnowledge->GetAgentKnown().GetRole< surrender::PHY_RoleInterface_Surrender >().Imprison( *pCampKnowledge->GetObjectKnown(), agent );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Prisoners_IsUnloadedInCamp
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Prisoners_IsUnloadedInCamp( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > pCampKnowledge )
{
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        return false;
    return pKnowledge->GetAgentKnown().GetRole< surrender::PHY_RoleInterface_Surrender >().IsImprisoned( *pCampKnowledge->GetObjectKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_OrientateAndLoad
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Refugees_OrientateAndLoad( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
        pKnowledge->GetAgentKnown().Apply( &refugee::RefugeeActionsNotificationHandler_ABC::Orientate, callerAgent->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_Unload
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Refugees_Unload( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return;
    pKnowledge->GetAgentKnown().Apply( &refugee::RefugeeActionsNotificationHandler_ABC::Release, callerAgent->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_UnloadInCamp
// Created: NLD 2007-02-14
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Refugees_UnloadInCamp( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > pCampKnowledge )
{
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        return;
    pKnowledge->GetAgentKnown().Apply( &refugee::RefugeeActionsNotificationHandler_ABC::ReleaseCamp, callerAgent->GetPion(), *pCampKnowledge->GetObjectKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::LoadAgentInCamp
// Created: DDA 2012-03-05
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::LoadAgentInCamp( DEC_Decision_ABC& callerAgent, DEC_Decision_ABC* pPion )
{
    if( !pPion )
        return;
    pPion->GetPion().Apply( &refugee::RefugeeActionsNotificationHandler_ABC::Orientate, callerAgent.GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::UnLoadAgentInCamp
// Created: DDA 2012-03-05
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::UnLoadAgentInCamp( DEC_Decision_ABC& callerAgent, DEC_Decision_ABC* pPion, boost::shared_ptr< DEC_Knowledge_Object > pCampKnowledge )
{
    if( !pPion || !pCampKnowledge || !pCampKnowledge->IsValid() || !pCampKnowledge->GetObjectKnown() )
        return;
    pPion->GetPion().Apply( &refugee::RefugeeActionsNotificationHandler_ABC::ReleaseCamp, callerAgent.GetPion(), *pCampKnowledge->GetObjectKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::IsAgentLoaded
// Created: DDA 2012-03-05
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::IsAgentLoaded( DEC_Decision_ABC& callerAgent, DEC_Decision_ABC* pPion )
{
    if( !pPion )
        return false;
    return callerAgent.GetPion().GetRole< transport::PHY_RoleAction_Transport >().IsLoaded( pPion->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::IsRefugee
// Created: DDA 2011-03-05
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::IsRefugee( DEC_Decision_ABC* pPion )
{
    if( !pPion )
        return false;
    return pPion->GetPion().GetType().IsRefugee();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_IsLoaded
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::PrisonnersRefugees_IsLoaded( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( !pKnowledge || !pKnowledge->IsValid() )
        return false;
    return callerAgent->GetPion().GetRole< transport::PHY_RoleAction_Transport >().IsLoaded( pKnowledge->GetAgentKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Refugees_IsUnloadedInCamp
// Created: NLD 2007-02-26
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Refugees_IsUnloadedInCamp( boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, boost::shared_ptr< DEC_Knowledge_Object > pCampKnowledge )
{
    if( IsNotCampKnowledgeOrHasLogisticCapacity( pKnowledge, pCampKnowledge ) )
        return false;
    return pKnowledge->GetAgentKnown().GetRole< PHY_RoleInterface_Refugee >().IsManaged( *pCampKnowledge->GetObjectKnown() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Knowledge_Load
// Created: EVH 2011-05-16
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Knowledge_Load( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge )
{
    if( pKnowledge && pKnowledge->IsValid() )
    {
        bool transportOnlyLoadable = false;
        callerAgent.GetPion().Apply( &transport::TransportNotificationHandler_ABC::MagicLoadPion, pKnowledge->GetAgentKnown(), transportOnlyLoadable );
        pKnowledge->GetAgentKnown().GetOrderManager().CancelMission();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_AddPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_AddPion( DEC_Decision_ABC* callerAgent, DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable )
{
    if( !pPion )
        throw MASA_EXCEPTION( "Null pion passed to DEC_ActionFunctions::Transport_AddPion" );
    callerAgent->GetPion().GetRole< transport::PHY_RoleAction_Transport >().AddPion( pPion->GetPion(), bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_AddPions
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_AddPions( DEC_Decision_ABC* callerAgent, const std::vector< DEC_Decision_ABC* >& pions, bool bTransportOnlyLoadable )
{
    for( auto it = pions.begin(); it != pions.end(); ++it )
        Transport_AddPion( callerAgent, *it, bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::TransportKnowledge_AddPion
// Created: EVH 2011-05-23
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::TransportKnowledge_AddPion( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, bool bTransportOnlyLoadable )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "Null knowledge passed to DEC_ActionFunctions::TransportKnowledge_AddPion" );
    MIL_Agent_ABC& pion = pKnowledge->GetAgentKnown();
    callerAgent.GetPion().GetRole< transport::PHY_RoleAction_Transport >().AddPion( pion, bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicLoadPionInCarrier
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicLoadPionInCarrier( DEC_Decision_ABC* caller, const DEC_Decision_ABC* pCarrier )
{
    if( !pCarrier )
        throw MASA_EXCEPTION( "Null carrier passed to DEC_ActionFunctions::Transport_MagicLoadPionInCarrier" );
    pCarrier->GetPion().Apply( &transport::TransportNotificationHandler_ABC::MagicLoadPion, caller->GetPion(), boost::cref( false ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicUnloadPionFromCarrier
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicUnloadPionFromCarrier( DEC_Decision_ABC* caller )
{
    //$$$ TMP
    MIL_AgentPion& agent = caller->GetPion();
    const MIL_Agent_ABC* transporter = agent.GetRole< transport::PHY_RolePion_Transported >().GetTransporter();
    if( transporter )
        const_cast< MIL_Agent_ABC* >( transporter )->GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( agent );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_GetCarrier
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_ActionFunctions::Transport_GetCarrier( const DEC_Decision_ABC* caller )
{
    const MIL_Agent_ABC* transporter = caller->GetPion().GetRole< transport::PHY_RolePion_Transported >().GetTransporter();
    if( transporter )
        return &const_cast< DEC_Decision_ABC& >( transporter->GetDecision() );
    return 0;
}
// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicLoadPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicLoadPion( DEC_Decision_ABC* callerAgent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable )
{
    if( !pPion )
        throw MASA_EXCEPTION( "Null pion passed to DEC_ActionFunctions::Transport_MagicLoadPion" );
    callerAgent->GetPion().Apply( &transport::TransportNotificationHandler_ABC::MagicLoadPion, pPion->GetPion(), bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicLoadPions
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicLoadPions( DEC_Decision_ABC* callerAgent, const std::vector< DEC_Decision_ABC* >& pions, bool bTransportOnlyLoadable )
{
    for( auto it = pions.begin(); it != pions.end(); ++it )
        Transport_MagicLoadPion( callerAgent, *it, bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicUnloadPion
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicUnloadPion( DEC_Decision_ABC* callerAgent, const DEC_Decision_ABC* pPion )
{
    if( !pPion )
        throw MASA_EXCEPTION( "Null pion passed to DEC_ActionFunctions::Transport_MagicUnloadPion" );
    callerAgent->GetPion().GetRole< transport::PHY_RoleAction_Transport >().MagicUnloadPion( pPion->GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_MagicUnloadPions
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_MagicUnloadPions( DEC_Decision_ABC* callerAgent, const std::vector< DEC_Decision_ABC* >& pions )
{
    for( auto it = pions.begin(); it != pions.end(); ++it )
        Transport_MagicUnloadPion( callerAgent, *it );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_IsFinished
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Transport_IsFinished( const DEC_Decision_ABC* callerAgent )
{
    return callerAgent->GetPion().GetRole< transport::PHY_RoleAction_Transport >().IsFinished();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_Cancel
// Created: NLD 2005-04-19
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Transport_Cancel( DEC_Decision_ABC* callerAgent )
{
    callerAgent->GetPion().GetRole< transport::PHY_RoleAction_Transport >().Cancel();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::AgentCanTransportPion
// Created: GGE 2013-07-04
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::AgentCanTransportPion( const DEC_Decision_ABC* agent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable )
{
    if( !agent || !pPion )
        return false;
    return agent->GetPion().GetRole< transport::PHY_RoleAction_Transport >().CanTransportPion( pPion->GetPion(), bTransportOnlyLoadable );
}

bool DEC_ActionFunctions::CanLoad( const DEC_Decision_ABC* pPion, const DEC_Decision_ABC* pTarget, bool bTransportOnlyLoadable )
{
    if( !pPion || !pTarget )
        return false;
    return pPion->GetPion().GetRole< transport::PHY_RoleAction_Transport >().CanLoad( pTarget->GetPion(), bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::GetNumberOfRoundTripsLeftToTransportPion
// Created: NMI 2014-03-24
// -----------------------------------------------------------------------------
double DEC_ActionFunctions::GetNumberOfRoundTripsLeftToTransportPion( const DEC_Decision_ABC* pPion, const DEC_Decision_ABC* pTarget, bool bTransportOnlyLoadable )
{
    assert( pPion );
    return pPion->GetPion().GetRole< transport::PHY_RoleAction_Transport >().GetNumberOfRoundTripsLeftToTransportPion( pTarget->GetPion(), bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::GetNumberOfRoundTripToTransportPion
// Created: GGE 2013-04-04
// -----------------------------------------------------------------------------
double DEC_ActionFunctions::GetNumberOfRoundTripToTransportPion( const DEC_Decision_ABC* callerAgent, const DEC_Decision_ABC* pPion, bool bTransportOnlyLoadable )
{
    assert( pPion );
    return callerAgent->GetPion().GetRole< transport::PHY_RoleAction_Transport >().GetNumberOfRoundTripToTransportPion( pPion->GetPion(), bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::CanTransportKnowledge
// Created: EVH 2011-05-30
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::CanTransportKnowledge( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, bool bTransportOnlyLoadable )
{
    if( !pKnowledge )
        throw MASA_EXCEPTION( "Null knowledge passed to DEC_ActionFunctions::CanTransportKnowledge" );
    MIL_Agent_ABC& pion = pKnowledge->GetAgentKnown();
    return callerAgent.GetPion().GetRole< transport::PHY_RoleAction_Transport >().CanTransportPion( pion, bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::GetNumberOfRoundTripToTransportKnowledge
// Created: GGE 2013-04-04
// -----------------------------------------------------------------------------
double DEC_ActionFunctions::GetNumberOfRoundTripToTransportKnowledge( DEC_Decision_ABC& callerAgent, boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge, bool bTransportOnlyLoadable )
{
    if( !pKnowledge )
        throw std::runtime_error( "Null knowledge passed to DEC_ActionFunctions::GetNumberOfRoundTripToTransportKnowledge" );
    MIL_Agent_ABC& pion = pKnowledge->GetAgentKnown();
    return callerAgent.GetPion().GetRole< transport::PHY_RoleAction_Transport >().GetNumberOfRoundTripToTransportPion( pion, bTransportOnlyLoadable );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::GetTransportedUnits
// Created: NMI 2014-03-20
// -----------------------------------------------------------------------------
std::vector< DEC_Decision_ABC* > DEC_ActionFunctions::GetTransportedUnits( DEC_Decision_ABC& callerAgent )
{
    return callerAgent.GetPion().Get< transport::PHY_RoleAction_Transport >().GetTransportedUnits();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::CanTransportCrowd
// Created: JSR 2011-08-09
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::CanTransportCrowd( DEC_Decision_ABC& callerAgent )
{
    return callerAgent.GetPion().GetRole< PHY_RolePion_Composantes >().CanTransportCrowd();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::GetCapacityToTransportCrowd
// Created: GGE 2013-04-04
// -----------------------------------------------------------------------------
int DEC_ActionFunctions::GetCapacityToTransportCrowd( DEC_Decision_ABC& callerAgent )
{
    return callerAgent.GetPion().GetRole< PHY_RolePion_Composantes >().GetCrowdTransporterCapacity();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::IsTransportingCrowd
// Created: DDA 2011-08-31
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::IsTransportingCrowd( DEC_Decision_ABC& callerAgent )
{
    return callerAgent.GetPion().GetRole< crowdtransport::PHY_RoleAction_CrowdTransport >().IsTransportingCrowd();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Transport_IsTransporting
// Created: NLD 2005-07-28
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Transport_IsTransporting( const DEC_Decision_ABC* callerAgent )
{
    return callerAgent->GetPion().GetRole< transport::PHY_RoleAction_Transport >().IsTransporting();
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Orientate
// Created: MGD 2010-07-22
// -----------------------------------------------------------------------------
void DEC_ActionFunctions::Orientate( DEC_Decision_ABC* callerAgent, boost::shared_ptr< MT_Vector2D > dir )
{
    MIL_AgentPion& agent = callerAgent->GetPion();
    agent.GetRole< PHY_RoleInterface_Location >().Move( agent.GetRole< PHY_RoleInterface_Location >().GetPosition(), *dir, 0. );
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Stock_IsExtractPossible
// Created: BCI 2011-02-08
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Stock_IsExtractPossible( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes )
{
    MIL_Agent_ABC& agent = callerAgent->GetPion();
    PHY_RoleInterface_Supply* supplyRole = agent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( !supplyRole )
        return false;

    PHY_RolePion_Composantes& composantes = agent.GetRole< PHY_RolePion_Composantes >();

    if( !pKnowledge || !pKnowledge->GetObjectKnown() )
        throw MASA_EXCEPTION( "Null knowledge object in DEC_ActionFunctions::Stock_IsExtractPossible" );
    if( StockAttribute* attribute = pKnowledge->GetObjectKnown()->RetrieveAttribute< StockAttribute >() )
    {
        BOOST_FOREACH( const PHY_DotationCategory* pDotation, dotationTypes )
        {
            if( pDotation && attribute->CanDistribute( *pDotation ) && composantes.CanStockMoreOf( *supplyRole, *pDotation ) )
                return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Stock_IsSupplyPossible
// Created: BCI 2011-02-08
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Stock_IsSupplyPossible( DEC_Decision_ABC* callerAgent, boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, const std::vector< const PHY_DotationCategory* >& dotationTypes )
{
    MIL_Agent_ABC& agent = callerAgent->GetPion();
    PHY_RoleInterface_Supply* supplyRole = agent.RetrieveRole< PHY_RoleInterface_Supply >();
    if( !supplyRole )
        return false;

    if( !pKnowledge || !pKnowledge->GetObjectKnown() )
        throw MASA_EXCEPTION( "Null knowledge object in DEC_ActionFunctions::Stock_IsSupplyPossible" );
    if( StockAttribute* attribute = pKnowledge->GetObjectKnown()->RetrieveAttribute< StockAttribute >() )
    {
        BOOST_FOREACH( const PHY_DotationCategory* pDotation, dotationTypes )
        {
            if( !pDotation )
                continue;
            PHY_DotationStock& stock = agent.GetRole< PHY_RolePion_Composantes >().GetOrAddStock( *supplyRole, *pDotation );
            if( !stock.IsEmpty() && attribute->CanBeSuppliedWith( *pDotation ) )
                return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::Stock_IsDistributePossible
// Created: BCI 2011-02-16
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::Stock_IsDistributePossible( boost::shared_ptr< DEC_Knowledge_Object > pKnowledge, boost::shared_ptr< DEC_Knowledge_Population > population )
{
    if( !pKnowledge || !pKnowledge->GetObjectKnown() )
        throw MASA_EXCEPTION( "Null knowledge object in DEC_ActionFunctions::Stock_IsDistributePossible" );
    if( StockAttribute* attribute = pKnowledge->GetObjectKnown()->RetrieveAttribute< StockAttribute >() )
        return !attribute->IsEmpty();
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_ActionFunctions::IsSurrendered()
// Created: GGE 2012-02-17
// -----------------------------------------------------------------------------
bool DEC_ActionFunctions::IsSurrendered( DEC_Decision_ABC& callerAgent )
{
   if( callerAgent.GetPion().GetRole< surrender::PHY_RoleInterface_Surrender >().IsSurrendered() )
       return true;
   return false;
}
