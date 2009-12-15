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

#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Agents/Units/Categories/PHY_RoePopulation.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"
#include "Entities/Automates/DEC_AutomateDecision.h"
#include "Entities/Orders/MIL_FragOrder.h"
#include "Entities/Orders/MIL_MissionType_ABC.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Decision/DEC_Model_ABC.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Path/Agent/DEC_Agent_Path.h"
#include "Network/NET_ASN_Messages.h"
#include "MT_Tools/MT_CrashHandler.h"

#include "Decision/Functions/DEC_AgentFunctions.h"
#include "Decision/Functions/DEC_KnowledgeAgentFunctions.h"
#include "Decision/Functions/DEC_KnowledgeObjectFunctions.h"
#include "Decision/Functions/DEC_KnowledgePopulationFunctions.h"
#include "Decision/Functions/DEC_GeometryFunctions.h"
#include "Decision/Functions/DEC_FireFunctions.h"
#include "Decision/Functions/DEC_DynamicFireFunctions.h"
#include "Decision/Functions/DEC_KnowledgeFunctions.h"
#include "Decision/Functions/DEC_PathFunctions.h"
#include "Decision/Functions/DEC_PerceptionFunctions.h"
#include "Decision/Functions/DEC_MiscFunctions.h"
#include "Decision/Functions/DEC_OrdersFunctions.h"
#include "Decision/Functions/DEC_ActionFunctions.h"
#include "Decision/Functions/DEC_PathFunctions.h"
#include "Decision/Functions/DEC_LogisticFunctions.h"
#include "Decision/Functions/DEC_ObjectFunctions.h"
#include "Decision/Functions/DEC_CommunicationFunctions.h"

#include "Entities/Agents/Actions/ComposanteLending/PHY_ActionLendCollectionComposantes.h"
#include "Entities/Agents/Actions/ComposanteLending/PHY_ActionLendHaulerComposantes.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionConstructObject.h" 
#include "Entities/Agents/Actions/Objects/PHY_ActionPrepareObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionDestroyObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionMineObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionBypassObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionOccupyObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionAnimateObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionResumeWorkObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionDistributeObject.h"
#include "Entities/Agents/Actions/Moving/PHY_ActionMove.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Position.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Knowledge.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePopulation.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePion.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionUsingOnlyLoadable.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionUsingOnlyCarrier.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionOnMajorComposantes.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionControlZone.h"
#include "Entities/Agents/Actions/Transport/PHY_ActionTransportLoad.h"
#include "Entities/Agents/Actions/Transport/PHY_ActionTransportUnload.h"
#include "Entities/Agents/Actions/Loading/PHY_ActionLoad.h"
#include "Entities/Agents/Actions/Loading/PHY_ActionUnload.h"
#include "Entities/Agents/Actions/Emergency/PHY_ActionInfluence.h"
#include "Entities/Agents/Actions/Emergency/PHY_ActionInfluenceInArea.h"
#include "Entities/Agents/Actions/Emergency/PHY_ActionTriggerActivity.h"
#include "Entities/Agents/Actions/Emergency/PHY_ActionTriggerActivityInArea.h"
#include "Entities/Agents/Actions/Emergency/PHY_ActionUnloadActivity.h"

#include "Knowledge/DEC_Knowledge_Def.h"

#include "simulation_kernel/NetworkNotificationHandler_ABC.h"
#include "simulation_kernel/NetworkUnitMessageNotificationHandler_ABC.h"

BOOST_CLASS_EXPORT_GUID( DEC_RolePion_Decision, "DEC_RolePion_Decision" )

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision constructor
// Created: NLD 2004-08-13
// -----------------------------------------------------------------------------
DEC_RolePion_Decision::DEC_RolePion_Decision( MIL_AgentPion& pion, DEC_DataBase& database )
    : DEC_Decision              ( pion, database )
    , nForceRatioState_         ( eForceRatioStateNone         )
    , nRulesOfEngagementState_  ( eRoeStateNone                )
    , nCloseCombatState_        ( eCloseCombatStateNone        )
    , nOperationalState_        ( eOpStateOperational          )
    , nIndirectFireAvailability_( eFireAvailabilityNone        )
    , pRoePopulation_           ( &PHY_RoePopulation::none_    )
    , bStateHasChanged_         ( true                         )
    , eEtatDec_                 ( 0                            )
    , eEtatDecPrudence_         ( 0                            )
    , eEtatLima_                ( 0                            )
    , eEtatNbc_                 ( 0                            )
    , eEtatDestruction_         ( 0                            )
    , eEtatFeu_                 ( 0                            )
    , eEtatAmbiance_            ( 0                            )
    , eEtatRadio_               ( 0                            )
    , eEtatRadar_               ( 0                            )
    , eEtatDeplacement_         ( 0                            )
    , eEtatOrdreCoordination_   ( 0                            )
    , eConsigneTir_             ( 0                            )
    , eConsigneTirPopulation_   ( 0                            )
    , eEtatEchelon_             ( 0                            )
    , eEtatSoutien_             ( 0                            )
    , eEtatSituationEnnemi_     ( 0                            )
    , eEtatPhaseMission_        ( 0                            )
    , bOrdreInterrompreMission_ ( false                        )
    , bOrdreDecrocher_          ( false                        )
    , bOrdreTenirSurLR_         ( false                        )
    , bOrdreTenir_              ( false                        )
    , bPasserSurLC_             ( false                        )
    , pionEnEscorte_            ( 0                            )
    , rTenir_                   ( 0.f                          )
    , eTypeContact_             ( 0                            )
    , eNiveauAction_            ( 0                            )
    , bDefenseStatique_Mobile_  ( false                        )
    , pAutomate_                ( 0                            )
{    
    const DEC_Model_ABC& model = pion.GetType().GetModel();
    try
    {
        name_ = pion.GetName();
        pAutomate_ = &pion.GetAutomate().GetDecision();
        SetModel( model );
    }
    catch( std::runtime_error& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, e.what() );
    }

    // Parameters list for calling default behavior
    StartDefaultBehavior();    
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision destructor
// Created: NLD 2004-08-16
// -----------------------------------------------------------------------------
DEC_RolePion_Decision::~DEC_RolePion_Decision()
{
}


// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::load
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< DEC_Decision< MIL_AgentPion > >( *this )
         >> pEntity_ 
         >> nForceRatioState_
         >> nRulesOfEngagementState_
         >> nCloseCombatState_
         >> nOperationalState_
         >> nIndirectFireAvailability_;
    assert( pEntity_ );

    uint nRoePopulationID;
    file >> nRoePopulationID;
    pRoePopulation_ = PHY_RoePopulation::Find( nRoePopulationID );
    assert( pRoePopulation_ );
       
    uint nPionTypeID;
    file >> nPionTypeID;
          
    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( nPionTypeID );
    assert( pType );
 
    const DEC_Model_ABC& model = pType->GetModel();
    try
    {
        SetModel( model );
        file >> name_;
        
        DEC_AutomateDecision* pDecision;
        file >> pDecision;
        assert( pDecision );
        pAutomate_ = pDecision;
    }
    catch( std::runtime_error& e )
    {
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, e.what() );
    }

    // Parameters list for calling default behavior
    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::save
// Created: JVT 2005-04-01
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    unsigned roe  = pRoePopulation_->GetID(),
             type = pEntity_->GetType().GetID();
    
    file << boost::serialization::base_object< DEC_Decision< MIL_AgentPion > >( *this )
         << pEntity_
         << nForceRatioState_
         << nRulesOfEngagementState_
         << nCloseCombatState_
         << nOperationalState_
         << nIndirectFireAvailability_
         << roe
         << type
         << name_ 
         << pAutomate_;         
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::EndCleanStateAfterCrash
// Created: LDC 2009-04-07
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::EndCleanStateAfterCrash()
{
    assert( pEntity_ );
    pEntity_->CancelAllActions();
    GetPion().GetRole< PHY_RoleInterface_Perceiver >().DisableAllPerceptions();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::RegisterUserFunctions
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::RegisterUserFunctions( directia::Brain& brain )
{
// Actions
    brain.RegisterFunction( "DEC_StopAction",
        boost::function< PHY_Action_ABC*( PHY_Action_ABC* ) >( boost::bind( &DEC_ActionFunctions::StopAction< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_PauseAction",
        boost::function< void( PHY_Action_ABC* ) >( boost::bind( &DEC_ActionFunctions::SuspendAction< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ReprendAction",
        boost::function< void( PHY_Action_ABC* ) >( boost::bind( &DEC_ActionFunctions::ResumeAction < MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    
    brain.RegisterFunction( "DEC_StartDeplacement",
        boost::function< PHY_Action_ABC*( boost::shared_ptr< DEC_Path_ABC > ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionMove, boost::shared_ptr< DEC_Path_ABC > >, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC__StartTirSurPopulation",
            boost::function< PHY_Action_ABC*( unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePopulation, unsigned int >, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_StartTirDirect",
            boost::function< PHY_Action_ABC*( boost::shared_ptr< DEC_Knowledge_Agent >, MT_Float, int, int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePion, boost::shared_ptr< DEC_Knowledge_Agent >, MT_Float, int, int >, boost::ref( GetPion() ), _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_StartTirDirectDebarques",
            boost::function< PHY_Action_ABC*( boost::shared_ptr< DEC_Knowledge_Agent >, MT_Float, int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionDirectFirePionUsingOnlyLoadable, boost::shared_ptr< DEC_Knowledge_Agent >, MT_Float, int >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_StartTirDirectTransporteurs",
            boost::function< PHY_Action_ABC*( boost::shared_ptr< DEC_Knowledge_Agent >, MT_Float, int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionDirectFirePionUsingOnlyCarrier, boost::shared_ptr< DEC_Knowledge_Agent >, MT_Float, int >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC__StartTirDirectSurComposantesMajeures",
            boost::function< PHY_Action_ABC*( int, boost::shared_ptr< DEC_Knowledge_Agent >, MT_Float, int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionDirectFirePionOnMajorComposantes, int, boost::shared_ptr< DEC_Knowledge_Agent >, MT_Float, int >, boost::ref( GetPion() ), _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_StartTirIndirectSurPosition",
            boost::function< PHY_Action_ABC* ( const PHY_DotationCategory*, float, MT_Vector2D* ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIndirectFire_Position, const PHY_DotationCategory*, float, MT_Vector2D* >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_StartTirIndirectSurConnaissance",
            boost::function< PHY_Action_ABC*( const PHY_DotationCategory*, float, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionIndirectFire_Knowledge, const PHY_DotationCategory*, float, unsigned int >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_StartCreateObject",
        boost::function< PHY_Action_ABC*( const std::string&, const TER_Localisation* ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionConstructObject, const std::string&, const TER_Localisation* >, boost::ref( GetPion() ), _1, _2) ) );
    brain.RegisterFunction( "DEC_StartPrepareObject",
            boost::function< PHY_Action_ABC*( const std::string&, const TER_Localisation* ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionPrepareObject, const std::string&, const TER_Localisation* >, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_StartDetruireObjet",
            boost::function< PHY_Action_ABC*( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionDestroyObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_StartValoriserObjet",
            boost::function< PHY_Action_ABC*( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionMineObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_StartCreerContournementObjet",
            boost::function< PHY_Action_ABC*( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionBypassObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC__StartOccuperObjet",
            boost::function< PHY_Action_ABC*( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionOccupyObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_StartReprendreTravauxObjet",
            boost::function< PHY_Action_ABC*( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionResumeWorkObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );  
    brain.RegisterFunction( "DEC__StartAnimerObjet",
            boost::function< PHY_Action_ABC*( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionAnimateObject, boost::shared_ptr< DEC_Knowledge_Object > >, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_StartDistributionObjet",
            boost::function< PHY_Action_ABC*( boost::shared_ptr< DEC_Knowledge_Object >, boost::shared_ptr< DEC_Knowledge_Population >, int ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionDistributeObject, boost::shared_ptr< DEC_Knowledge_Object >, boost::shared_ptr< DEC_Knowledge_Population >, int >, boost::ref( GetPion() ), _1, _2, _3 ) ) );  
    brain.RegisterFunction( "DEC__StartControlerZone",
            boost::function< PHY_Action_ABC*( const TER_Localisation*, MT_Float, bool ) >( boost::bind( &DEC_ActionFunctions::StartAction  < PHY_ActionControlZone, const TER_Localisation*, MT_Float, bool >, boost::ref( GetPion() ), _1, _2, _3 ) ) );    
    brain.RegisterFunction( "DEC_StartEmbarquement",
            boost::function< PHY_Action_ABC*() >( boost::bind( &DEC_ActionFunctions::StartAction  < transport::PHY_ActionLoad >, boost::ref( GetPion() ) ) ) );
    brain.RegisterFunction( "DEC_StartDebarquement",
            boost::function< PHY_Action_ABC*() >( boost::bind( &DEC_ActionFunctions::StartAction  < transport::PHY_ActionUnload >, boost::ref( GetPion() ) ) ) );

    // Embarquement / dbarquement
    brain.RegisterFunction( "DEC_Agent_EstEmbarque",
                            boost::bind( &DEC_AgentFunctions::IsLoaded, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_DureeEmbarquement",
                            boost::bind( &DEC_AgentFunctions::GetLoadingTime, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_DureeDebarquement",
                            boost::bind( &DEC_AgentFunctions::GetUnloadingTime, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_TransporteursPret",
                            boost::bind( &DEC_AgentFunctions::AreHumanTransportersReady, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_LaisserTransporteursSansDelai",
                            boost::bind( &DEC_AgentFunctions::DisableHumanTransportersNow, boost::ref( GetPion() ) ) );   
    brain.RegisterFunction( "DEC_RecupererTransporteursSansDelai",
                            boost::bind( &DEC_AgentFunctions::RecoverHumanTransportersNow, boost::ref( GetPion() ) ) );

    // Objects
    brain.RegisterFunction( "DEC_ActiverObjet",
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_ObjectFunctions::ActivateObject< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_CreerObjetSansDelais",
        boost::function< void( const std::string&, const TER_Localisation* ) > (boost::bind( &DEC_ObjectFunctions::MagicCreateObject < MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_DetruireObjetSansDelais" ,
    		boost::function< void( boost::shared_ptr< DEC_Knowledge_Object > ) > (boost::bind( &DEC_ObjectFunctions::MagicDestroyObject< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );

    // Path
    brain.RegisterFunction( "DEC_CreerItineraire",
        boost::function< boost::shared_ptr< DEC_Path_ABC >( MT_Vector2D*, int ) >( boost::bind( &DEC_PathFunctions::CreatePathToPoint, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_CreerItineraireBM",
        boost::function< boost::shared_ptr< DEC_Path_ABC >( float, float, float, int ) >( boost::bind( &DEC_PathFunctions::CreatePathToPointBM, boost::ref( GetPion() ), _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_CreerItineraireListe",
        boost::function< boost::shared_ptr< DEC_Path_ABC >( std::vector< boost::shared_ptr< MT_Vector2D > >, int ) >( boost::bind( &DEC_PathFunctions::CreatePathToPointList, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Itineraire_Etat", 
        boost::function< int ( DEC_Path_ABC* ) >( boost::bind( &DEC_PathFunctions::GetPathState, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_GetNextObjectOnPath",
        boost::function< std::pair< bool, std::pair< boost::shared_ptr< DEC_Knowledge_Object >, float > >( boost::shared_ptr< DEC_Knowledge_Object >, float, const std::vector< std::string >& ) >(
        boost::bind( &DEC_PathFunctions::GetNextObjectOnPath, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Itineraire_ExtrapolerPosition" ,
        boost::function< boost::shared_ptr< MT_Vector2D >( const MT_Float, bool ) >( boost::bind( &DEC_PathFunctions::ExtrapolatePosition, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Itineraire_DernierPoint",
        boost::function< boost::shared_ptr< MT_Vector2D >( const DEC_Path_ABC* ) >( boost::bind( &DEC_PathFunctions::GetLastPointOfPath, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Itineraire_EstEnMouvementSur",
        boost::function< bool ( const DEC_Path_ABC* ) >( boost::bind( &DEC_PathFunctions::IsMovingOnPath, boost::ref( GetPion() ), _1 ) ) );
  
    // Perception
    brain.RegisterFunction( "DEC_Perception_ActiverCoupsDeSonde",
                            boost::bind( &DEC_PerceptionFunctions::EnableCoupDeSonde, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Perception_DesactiverCoupsDeSonde",
                            boost::bind( &DEC_PerceptionFunctions::DisableCoupDeSonde, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Perception_PointEstVisible",
        boost::function< bool( MT_Vector2D* ) >( boost::bind( &DEC_PerceptionFunctions::IsPointVisible, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Perception_VisionVerrouilleeSurDirection",
        boost::function< void( boost::shared_ptr< MT_Vector2D > ) >( boost::bind( &DEC_PerceptionFunctions::SetVisionModeDirection, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Perception_VisionVerrouilleeSurPoint",
        boost::function< void( const MT_Vector2D* ) >( boost::bind( &DEC_PerceptionFunctions::SetVisionModePoint, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Perception_VisionNormale",
                            boost::bind( &DEC_PerceptionFunctions::SetVisionModeNormal, boost::ref( GetPion() ) ) );   
    brain.RegisterFunction( "DEC_Perception_ActiverReconnaissanceLocalisation",
        boost::function< int( const TER_Localisation* ) >( boost::bind( &DEC_PerceptionFunctions::EnableRecognitionLocalisation, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Perception_DesactiverReconnaissanceLocalisation",
         boost::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableRecognitionLocalisation, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Perception_ActiverReconnaissancePoint",
        boost::function< int( MT_Vector2D*, MT_Float, MT_Float ) >( boost::bind( &DEC_PerceptionFunctions::EnableRecognitionPoint, boost::ref( *(DEC_Decision_ABC*)this ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Perception_DesactiverReconnaissancePoint",
        boost::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableRecognitionPoint, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Perception_ActiverRadar",
                            boost::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::EnableRadar, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Perception_DesactiverRadar",
                            boost::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableRadar, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Perception_ActiverRadarSurLocalisation",
        boost::function< int( int, const TER_Localisation* ) >( boost::bind( &DEC_PerceptionFunctions::EnableRadarOnLocalisation, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Perception_DesactiverRadarSurLocalisation",
        boost::function< void( int, int ) >( boost::bind( &DEC_PerceptionFunctions::DisableRadarOnLocalisation, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Perception_ActiverDetectionObjetLocalisation",
        boost::function< int( const TER_Localisation*, const MT_Vector2D*, MT_Float ) >( boost::bind( &DEC_PerceptionFunctions::EnableObjectRecognitionLocalisation, boost::ref( *(DEC_Decision_ABC*)this ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Perception_DesactiverDetectionObjetLocalisation",
        boost::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableObjectRecognitionLocalisation, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Perception_ActiverPerceptionTirsIndirect",
        boost::function< int( const TER_Localisation* ) >( boost::bind( &DEC_PerceptionFunctions::EnableFlyingShellDetection, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Perception_DesactiverPerceptionTirsIndirect",
        boost::function< void( int ) >( boost::bind( &DEC_PerceptionFunctions::DisableFlyingShellDetection, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Perception_Furtivite", 
        boost::function< void( float ) >( boost::bind( &DEC_PerceptionFunctions::SetStealthFactor, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Perception_DesactiverSenseurs",
                            boost::bind( &DEC_PerceptionFunctions::DisableSensors, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Perception_ActiverSenseurs",
                            boost::bind( &DEC_PerceptionFunctions::EnableSensors, boost::ref( GetPion() ) ) );

    // Gestion des renforts
    brain.RegisterFunction( "DEC_Renforts" ,
        boost::function< std::vector<DEC_Decision_ABC*> () >( boost::bind( &DEC_MiscFunctions::GetReinforcements, boost::ref( GetPion() ) ) ) );
    brain.RegisterFunction( "DEC_Renforce",
        boost::function< bool ( const DEC_Decision_ABC* ) >(boost::bind( &DEC_MiscFunctions::Reinforce, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_AnnuleRenforcement" ,
        boost::bind( &DEC_MiscFunctions::CancelReinforcement, boost::ref( GetPion() ) ) );

    // Tests / Debug
    brain.RegisterFunction( "DEC_DebugAffichePoint"  ,
    		boost::function< void ( const MT_Vector2D* ) > (boost::bind( &DEC_MiscFunctions::DebugDrawPoint< MIL_AgentPion >, boost::ref( GetPion()), _1  ) ) );
    brain.RegisterFunction( "DEC_DebugAffichePoints" ,
    		boost::function< void ( std::vector< boost::shared_ptr< MT_Vector2D > > ) > (boost::bind( &DEC_MiscFunctions::DebugDrawPoints< MIL_AgentPion >, boost::ref( GetPion()), _1  ) ) );
    brain.RegisterFunction( "DEC_Debug",
    		boost::function < void ( const std::string& ) > ( boost::bind( &DEC_MiscFunctions::Debug< MIL_AgentPion > , boost::ref( GetPion()) , "Agent" , _1  ) ) );
    brain.RegisterFunction( "DEC_Trace",
        boost::function< void ( const std::string& ) >( boost::bind( &DEC_MiscFunctions::Trace< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );

    brain.RegisterFunction( "DEC_DecisionalState",
        boost::function< void ( const std::string&, const std::string& ) >( boost::bind( &DEC_AgentFunctions::DecisionalState, boost::ref( GetPion() ), _1, _2 ) ) );

    // Installation
    brain.RegisterFunction( "DEC_Agent_EstInstalle"   , boost::bind( &DEC_AgentFunctions::IsInstalled, boost::cref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_EstDesinstalle", boost::bind( &DEC_AgentFunctions::IsUninstalled, boost::cref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_SInstaller"    , boost::bind( &DEC_AgentFunctions::Install, boost::ref( GetPion() ) ) );

    // Pion accessors    
    brain.RegisterFunction( "DEC_Agent_EstPC"        , boost::bind( &DEC_AgentFunctions::IsPC, boost::cref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_EstPC"        , &DEC_Decision_ABC::IsPC );
    brain.RegisterFunction( "DEC_Agent_EstTransporte", boost::bind( &DEC_AgentFunctions::IsTransported, boost::cref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_EstTransporte", &DEC_Decision_ABC::IsTransported );
    brain.RegisterFunction( "DEC_Agent_EstEnVol"     , boost::bind( &DEC_AgentFunctions::IsFlying, boost::cref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_EstEnVol"     , &DEC_Decision_ABC::IsFlying );
    brain.RegisterFunction( "DEC_Agent_HauteurDeVol" , 
        boost::function< void ( MT_Float ) >( boost::bind( &DEC_AgentFunctions::SetFlyingHeight, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Agent_EstEnMouvement", &DEC_Decision_ABC::IsMoving );

    brain.RegisterFunction( "DEC_Agent_EtatOps", boost::bind( &DEC_AgentFunctions::GetOperationalState, boost::cref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_EtatOpsMajeur", boost::bind( &DEC_AgentFunctions::GetMajorOperationalState, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_EtatOpsMajeur", &DEC_Decision_ABC::GetMajorOperationalState );
    brain.RegisterFunction( "DEC_Agent_PeutConstruireObjet", 
        boost::function< bool ( const std::string& ) >( boost::bind( &DEC_AgentFunctions::CanConstructObject, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Agent_PeutConstruireContournementObjet", 
        boost::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::CanBypassObject, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Agent_PeutDetruireObjet",
        boost::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::CanDestroyObject, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Agent_PeutValoriserObjet",
        boost::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::CanMineObject, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Agent_PeutActiverObjet", 
        boost::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_AgentFunctions::CanActivateObject, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Agent_ActiverModeDiscret", boost::bind( &DEC_AgentFunctions::EnableDiscreteMode, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_DesactiverModeDiscret", boost::bind( &DEC_AgentFunctions::DisableDiscreteMode, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_EstNeutralise", boost::bind( &DEC_AgentFunctions::IsNeutralized, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_EstNeutralise", &DEC_Decision_ABC::IsNeutralized );
    brain.RegisterFunction( "DEC_Agent_AutomateEstEmbraye", boost::bind( &DEC_AgentFunctions::IsAutomateEngaged, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_AutomateEstEmbraye", &DEC_Decision_ABC::IsAutomateEngaged );
    brain.RegisterFunction( "DEC_Agent_Position", boost::bind( &DEC_AgentFunctions::GetPosition, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_Position", &DEC_Decision_ABC::GetPosition );
    brain.RegisterFunction( "DEC_Agent_Direction", boost::bind( &DEC_AgentFunctions::GetDirection, boost::cref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_EstMort", boost::bind( &DEC_AgentFunctions::IsDead, boost::cref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_EstMort", &DEC_Decision_ABC::IsDead );
    brain.RegisterFunction( "DEC_Agent_NiveauInstallation", boost::bind( &DEC_AgentFunctions::GetPosture, boost::cref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_RoePopulation", boost::bind( &DEC_AgentFunctions::GetRoePopulation, boost::cref( GetPion() ) ) );
 
    brain.RegisterFunction( "DEC_Agent_AutonomieEnDeplacement", boost::bind( &DEC_AgentFunctions::TimeLeftForMoving, boost::cref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_TempsPourParcourirDistanceEnLigneDroite", 
        boost::function< float ( float ) >( boost::bind( &DEC_AgentFunctions::TimeToMoveDistance, boost::cref( GetPion() ), _1 ) ) );

    // NBC
    brain.RegisterFunction( "DEC_Agent_EstContamine", boost::bind( &DEC_AgentFunctions::IsContaminated , boost::cref( GetPion() ) ) );  
    brain.RegisterFunction( "DEC_Agent_EstContamine", &DEC_Decision_ABC::IsContaminated );
    brain.RegisterFunction( "DEC_Agent_MettreTenueProtectionNBC"            , &DEC_Decision_ABC::WearNbcProtectionSuit             );
    brain.RegisterFunction( "DEC_Agent_EnleverTenueProtectionNBC"           , &DEC_Decision_ABC::RemoveNbcProtectionSuit           );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_DemandeDeDecontamination", 
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::QueueForDecontamination, boost::ref( GetPion() ), _1 ) ) );

    // Blackout
    brain.RegisterFunction( "DEC_Agent_PasserEnSilenceRadio", boost::bind( &DEC_AgentFunctions::ActivateBlackout, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Agent_ArreterSilenceRadio", boost::bind( &DEC_AgentFunctions::DeactivateBlackout, boost::ref( GetPion() ) ) );

    // Misc
    brain.RegisterFunction( "DEC_ModulationVitesseCourante",
        boost::function< void( MT_Float ) >( boost::bind( &DEC_MiscFunctions::SetCurrentSpeedModificator, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ModulationVitesseMax",
        boost::function< void( MT_Float ) >( boost::bind( &DEC_MiscFunctions::SetMaxSpeedModificator, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Elongation"     ,
        boost::function< void( MT_Float ) >( boost::bind( &DEC_AgentFunctions::SetElongationFactor, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ReleverPion"    ,
        boost::function< bool( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::RelievePion, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_PeutReleverPion",
        boost::function< bool( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::CanRelievePion, boost::ref( GetPion() ), _1 ) ) );

    // Population
    brain.RegisterFunction( "DEC_Agent_CanaliserPopulation",
        boost::function< void( const TER_Localisation* ) >( boost::bind( &DEC_AgentFunctions::ChannelPopulations, _1 ) ) );

    // Agent knowledges accessors
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EtatOps",
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetOperationalState, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstDetruitTactique",
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetMajorOperationalState, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_Position",
        boost::function< const MT_Vector2D*( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetPosition, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstEnVol", 
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsFlying, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_NatureAtlas", 
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetNatureAtlas, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_NiveauPerceptionMax",
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetMaxPerceptionLevelForKnowledgeGroup, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_Dangerosite",
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetDangerosity, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_DangerositeSurPion", 
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, const DEC_Decision_ABC* ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetDangerosityOnPion, _1, _2 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_DangerositeSurConnaissance", 
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::GetDangerosityOnKnowledge, _1, _2 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstPercuParUnite", 
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsPerceivedByAgent, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstUnEnnemi",
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsAnEnemy, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstValide",
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsKnowledgeValid, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstEnMouvement", 
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsMoving, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_PercoitUnite",
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsPerceivingAgent, boost::cref( GetPion() ), _1 ) ));
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstPrisonnier", 
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsPrisoner, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstMort",
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsDead, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_Verrouiller",
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::Lock, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_Deverrouiller", 
        boost::function< void( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::Unlock, _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_EstAPorteDeCapteurDansCone", 
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent >, const MT_Vector2D*, MT_Float ) >( boost::bind( &DEC_KnowledgeAgentFunctions::IsInDetectionCone, boost::cref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceAgent_TuerOfficiers", 
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_KnowledgeAgentFunctions::KillOfficers, _1 ) ) );

    // Object knowledges accessors
    brain.RegisterFunction( "DEC_IsValidKnowledgeObject",
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsKnowledgeValid< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstObstacleDeManoeuvreActif", 
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsReservedObstacleActivated< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstObstacleDeManoeuvre", 
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsReservedObstacle< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstContourne", 
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsBypassed< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Localisation", 
        boost::function< const TER_Localisation* ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::GetLocalisation< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Type", 
        boost::function< std::string( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::GetType< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_LargeurSiteFranchissement", 
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Reconnaitre",
        boost::function< void( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::Recon, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_EstReconnu", 
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::IsRecon< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );    
    brain.RegisterFunction( "DEC_ConnaissanceObjet_PeutEtreOccupe", 
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::CanBeOccupied, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_PeutEtreAnime",
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::CanBeAnimated, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_PeutEtreContourne", 
        boost::function< bool( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::CanBeBypassed, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissanceObjet_Degrader", 
        boost::function< int( boost::shared_ptr< DEC_Knowledge_Object >, float ) >( boost::bind( &DEC_KnowledgeObjectFunctions::DamageObject, boost::cref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_ConnaisssanceObjet_ChangeDensitePopulationSortante", 
        boost::function< void( boost::shared_ptr< DEC_Knowledge_Object >, float ) >( boost::bind( &DEC_KnowledgeObjectFunctions::SetExitingPopulationDensity, boost::cref( GetPion() ), _1, _2 ) ) ); 
    brain.RegisterFunction( "DEC_ConnaisssanceObjet_ResetDensitePopulationSortante", 
        boost::function< void( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_KnowledgeObjectFunctions::ResetExitingPopulationDensity, boost::cref( GetPion() ), _1 ) ) );

    // Population knowledges accessors
    brain.RegisterFunction( "DEC_KnowledgePopulation_Domination"                   ,
    		boost::function< std::pair< float, int > (int) > (boost::bind( &DEC_KnowledgePopulationFunctions::GetDominationState< MIL_AgentPion >, boost::ref( GetPion()), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissancePopulation_Attitude"                     ,
    		boost::function< unsigned int (int) > ( boost::bind( &DEC_KnowledgePopulationFunctions::GetAttitude< MIL_AgentPion >, boost::ref( GetPion()), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissancePopulation_Reconnaitre"                  ,
    		boost::function< void (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::Recon , boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissancePopulation_EstReconnu"                   ,
    		boost::function< bool(int)> ( boost::bind ( &DEC_KnowledgePopulationFunctions::IsRecon< MIL_AgentPion > , boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissancePopulation_EstPercueParUnite"            ,
    		boost::function< bool (int) > (boost::bind ( &DEC_KnowledgePopulationFunctions::IsPerceivedByAgent, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissancePopulation_Dangerosite"                  ,
    		boost::function< float(int)>(boost::bind ( &DEC_KnowledgePopulationFunctions::GetDangerosity , boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissancePopulation_EstDansZone"                  ,
    		boost::function< bool (int, TER_Localisation*) >(boost::bind( &DEC_KnowledgePopulationFunctions::IsInZone < MIL_AgentPion >, boost::cref( GetPion() ), _1 , _2 ) ) );
    brain.RegisterFunction( "DEC_ConnaissancePopulation_PointPlusProche"              ,
    		boost::function< boost::shared_ptr<MT_Vector2D> (int) >(boost::bind( &DEC_KnowledgePopulationFunctions::ClosestPoint, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissancePopulation_Securiser"                    ,
    		boost::function< void (int) >(boost::bind( &DEC_KnowledgePopulationFunctions::Secure, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissancePopulation_PointSecurisation"            ,
    		boost::function< boost::shared_ptr<MT_Vector2D> (int) > (boost::bind( &DEC_KnowledgePopulationFunctions::SecuringPoint, boost::cref( GetPion() ), _1  ) ) );
    brain.RegisterFunction( "DEC_ConnaissancePopulation_EstEnnemi"                    ,
    		boost::function< bool (int) >(boost::bind( &DEC_KnowledgePopulationFunctions::IsEnemy, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_ConnaissancePopulation_Exterminer"                   ,
    		boost::function< int (int, float) >(boost::bind( &DEC_KnowledgePopulationFunctions::Exterminate, boost::cref( GetPion() ), _1, _2 ) ) );
    
    // Global knowledge
    brain.RegisterFunction( "DEC_RapportDeForceLocal", boost::bind( &DEC_KnowledgeFunctions::GetRapForLocal, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Connaissances_UnitesDetecteesDansFuseau", boost::bind( &DEC_KnowledgeFunctions::GetDetectedAgentsInFuseau, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Connaissances_UnitesDetecteesDansZone",
        boost::function< T_ConstKnowledgeAgentVector( const TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetDetectedAgentsInZone, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_UnitesPrenantAPartie", boost::bind( &DEC_KnowledgeFunctions::GetAgentsAttacking, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Connaissances_UnitesEnnemiesDangereuses", boost::bind( &DEC_KnowledgeFunctions::GetDangerousEnemies, boost::ref( GetPion() ) ) );

    brain.RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesPercues", boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesPerceived, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesPercuesParPion", 
        boost::function< T_ConstKnowledgeAgentVector( const DEC_Decision_ABC* ) >( boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedByPion< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesPercuesDansFuseau", boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesPerceivedInFuseau, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesDansFuseau", boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesInFuseau, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesDansZone",
        boost::function< T_ConstKnowledgeAgentVector( TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesInZone, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_UnitesEnnemiesVivantesDansCercle",
        boost::function< T_ConstKnowledgeAgentVector( const MT_Vector2D*, float ) >( boost::bind( &DEC_KnowledgeFunctions::GetLivingEnemiesInCircle, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_RefugiesAProximite",
        boost::function< T_ConstKnowledgeAgentVector( MT_Float ) > ( boost::bind( &DEC_KnowledgeFunctions::GetNearbyRefugees, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_UnitesRenduesAProximite",
        boost::function< T_ConstKnowledgeAgentVector ( MT_Float ) > ( boost::bind( &DEC_KnowledgeFunctions::GetNearbySurrenderedAgents, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_PresenceEnnemieDansCercle", 
        boost::function< bool( const MT_Vector2D*, MT_Float ) >( boost::bind( &DEC_KnowledgeFunctions::EnemyPresenceInCircle, boost::cref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_UnitesAmiesDansZone", 
        boost::function< T_ConstKnowledgeAgentVector( const TER_Localisation* ) >( boost::bind( &DEC_KnowledgeFunctions::GetFriendsInZone< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_PartageConnaissancesAvec", 
        boost::function< void( DEC_Decision_ABC*, float ) >( boost::bind( &DEC_KnowledgeFunctions::ShareKnowledgesWith< MIL_AgentPion >, boost::cref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_PartageConnaissancesDansZoneAvec", 
        boost::function< void( DEC_Decision_ABC*, const MT_Vector2D*, float ) >( boost::bind( &DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith< MIL_AgentPion >, boost::cref( GetPion() ), _1, _2, _3 ) ) );    
    brain.RegisterFunction( "DEC_Knowledges_ObjectsInCircle",
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const MT_Vector2D*, MT_Float, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInCircle< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_ObjectKnowledgesInZone",
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const TER_Localisation*, const std::vector< std::string >& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInZone< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_ObjetsDansFuseau", 
        boost::function< std::vector< boost::shared_ptr< DEC_Knowledge_Object > >( const std::string& ) >( boost::bind( &DEC_KnowledgeFunctions::GetObjectsInFuseau< MIL_AgentPion >, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_CollisionsObjets", boost::bind( &DEC_KnowledgeFunctions::GetObjectsColliding, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Connaissances_ObjetLePlusProche", 
        boost::function< boost::shared_ptr< DEC_Knowledge_Object >( const std::string& ) >( boost::bind( &DEC_KnowledgeFunctions::GetClosestObject, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_ObjetAmiLePlusProche", 
        boost::function< boost::shared_ptr< DEC_Knowledge_Object >( const std::string& ) >( boost::bind( &DEC_KnowledgeFunctions::GetClosestFriendObject, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Connaissances_CollisionsPopulations", boost::bind( &DEC_KnowledgeFunctions::GetPopulationsColliding, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Connaissances_PopulationsPrenantAPartie", boost::bind( &DEC_KnowledgeFunctions::GetPopulationsAttacking, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Connaissances_Populations", boost::bind( &DEC_KnowledgeFunctions::GetPopulations< MIL_AgentPion >, boost::ref( GetPion() ) ) );

    // Ordres de conduite
    brain.RegisterFunction( "DEC_ReleverUnite", 
        boost::function< bool( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::RelievePion, boost::ref( GetPion() ), _1 ) ) );

    // Limas / Mission    
    brain.RegisterFunction( "DEC_NouvelleMission", boost::bind( &DEC_OrdersFunctions::IsNewMissionStarted< MIL_AgentPion >, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_FinMission", boost::bind( &DEC_OrdersFunctions::FinishMission< MIL_AgentPion >, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_GetLima",
        boost::function< unsigned int( unsigned int ) >( boost::bind( &DEC_OrdersFunctions::GetLima< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_SetMissionLimaFlag"            ,
    		boost::function< void (unsigned int, bool)> ( boost::bind( &DEC_OrdersFunctions::PionSetMissionLimaFlag, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_GetMissionLimaFlag"            ,
    		boost::function< bool (unsigned int)> ( boost::bind( &DEC_OrdersFunctions::GetMissionLimaFlag < MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_SetMissionLimaFlagHoraire"     ,
    		boost::function< void (unsigned int, bool)> ( boost::bind( &DEC_OrdersFunctions::PionSetMissionLimaScheduleFlag, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_GetRawMission", &DEC_AgentFunctions::GetMission );
    brain.RegisterFunction( "DEC_SetMission",
        boost::function< void( DEC_Decision_ABC*, MIL_Mission_ABC* )>( boost::bind( &DEC_AgentFunctions::SetMission, _1, _2 ) ) );

    // Etat dcisionnel
    brain.RegisterFunction( "DEC_Agent_ChangeEtatRapportDeForce",
        boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyForceRatioStateChanged, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Agent_ChangeEtatROE",
        boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyRulesOfEngagementStateChanged, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Agent_ChangeEtatROEPopulation",
        boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyRulesOfEngagementPopulationStateChanged, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Agent_ChangeEtatCombatDeRencontre",
         boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyCloseCombatStateChanged, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Agent_ChangeEtatOperationnel",
         boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyOperationalStateChanged, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Agent_ChangeDisponibiliteAuTirIndirect",
         boost::function< void( int ) >( boost::bind( &DEC_AgentFunctions::NotifyIndirectFireAvailabilityChanged, boost::ref( GetPion() ), _1 ) ) );

    // Facteurs humains
    brain.RegisterFunction( "DEC_FacteurHumain_Fatigue", boost::bind( &DEC_AgentFunctions::GetHumanFactorTiredness, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_FacteurHumain_Moral", boost::bind( &DEC_AgentFunctions::GetHumanFactorMorale, boost::ref( GetPion() ) ) );

    // Calculs de position
    brain.RegisterFunction( "DEC_Geometrie_PositionInterception",
        boost::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent >, MT_Vector2D*, MT_Float )>( boost::bind( &DEC_GeometryFunctions::GetInterceptionPosition, boost::cref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Agent_PositionInterception", 
        boost::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_AgentFunctions::GetInterceptionPoint, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionAppui",
        boost::function< boost::shared_ptr< MT_Vector2D >( DEC_Decision_ABC*, MT_Float ) >( boost::bind( &DEC_GeometryFunctions::ComputeSupportPosition, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionEmbuscade",
        boost::function< boost::shared_ptr< MT_Vector2D >( MT_Vector2D*, MT_Vector2D*, MT_Float ) >( boost::bind( &DEC_GeometryFunctions::ComputeAmbushPosition, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionSurete",
        boost::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent >, MT_Float ) >( boost::bind( &DEC_GeometryFunctions::ComputeSafetyPosition, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionSureteAvecPopulation",
        boost::function< boost::shared_ptr< MT_Vector2D >( uint, MT_Float ) >( boost::bind( &DEC_GeometryFunctions::ComputeSafetyPositionWithPopulation, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionSureteAvecObjectif",
        boost::function< boost::shared_ptr< MT_Vector2D >( boost::shared_ptr< DEC_Knowledge_Agent >, MT_Float, MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::ComputeSafetyPositionWithObjective, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointProcheLocalisationDansFuseau",
        boost::function< boost::shared_ptr< MT_Vector2D >( const TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau< MIL_AgentPion>, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointProcheLocalisationNonClippeeDansFuseau" ,
        boost::function< boost::shared_ptr< MT_Vector2D >( const TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInFuseau< MIL_AgentPion>, boost::cref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerBarycentreLocalisationDansFuseau",
        boost::function< boost::shared_ptr< MT_Vector2D >( TER_Localisation* ) >( boost::bind( &DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerEntreeFuseauLaPlusProche",
                            boost::bind( &DEC_GeometryFunctions::ComputeNearestFuseauEntryPoint, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Geometry_SplitLocalisation",
        boost::function< std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( TER_Localisation*, unsigned int ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisationInParts< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Geometry_SplitLocalisationSurface",
        boost::function< std::pair< std::vector< boost::shared_ptr< TER_Localisation > >, unsigned int >( TER_Localisation*, unsigned int ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisationInSurfaces< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_DecoupeFuseauEnTroncons",
        boost::function< std::vector< boost::shared_ptr< TER_Localisation > >( const MT_Float ) >( boost::bind( &DEC_GeometryFunctions::SplitLocalisationInSections< MIL_AgentPion >, boost::ref( GetPion() ), _1  ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionObstacle",
        boost::function< boost::shared_ptr< MT_Vector2D >( MT_Vector2D*, const std::string&, MT_Float) >( boost::bind( &DEC_GeometryFunctions::ComputeObstaclePosition< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointArrivee",
                            boost::bind( &DEC_GeometryFunctions::ComputeDestPoint< MIL_AgentPion >, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerPointDepart",
                            boost::bind( &DEC_GeometryFunctions::ComputeStartPoint< MIL_AgentPion >, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Geometrie_EstPointDansFuseau",
        boost::function< bool( MT_Vector2D* ) >( boost::bind( &DEC_GeometryFunctions::IsPointInFuseau< MIL_AgentPion >, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_CalculerBarycentreListeConnaissancesAgents",
        boost::function< boost::shared_ptr< MT_Vector2D >( const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > ) >( boost::bind( &DEC_GeometryFunctions::ComputeKnowledgeAgentBarycenter, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Geometrie_PionDevant"                                            , &DEC_GeometryFunctions::GetFrontestPion );
    brain.RegisterFunction( "DEC_Geometrie_PionDerriere"                                          , &DEC_GeometryFunctions::ComputeBackestAgent );    
    brain.RegisterFunction( "DEC_Geometrie_CalculerPositionParRapportALima",
        boost::function< boost::shared_ptr< MT_Vector2D >( int, float ) >( boost::bind( &DEC_GeometryFunctions::ComputePointBeforeLima< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) ) );

    // Fire 
    brain.RegisterFunction( "DEC_Tir_PorteeMaxPourTirerSurUnite",
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToFireOnEnemy, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Tir_PorteeMinPourTirerSurUnite",
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMinRangeToFireOnEnemy, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Tir_PorteeMaxPourTirerSurUnitePosturesReelles",
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToFireOnEnemyActualPosture, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Tir_PorteeMinPourTirerSurUnitePosturesReelles",
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMinRangeToFireOnEnemyActualPosture, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Tir_PorteeMaxPourTirerDebarqueSurUnite",
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToFireOnEnemyWhenUnloaded, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Tir_PorteeMaxPourEtreTireParUnite",
        boost::function< float( boost::shared_ptr< DEC_Knowledge_Agent >, float ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToBeFiredByEnemy, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Tir_PorteeMaxTirIndirect",
        boost::function< float( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToIndirectFire, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Tir_PorteeMinTirIndirect",
        boost::function< float( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetMinRangeToIndirectFire, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Tir_PorteeMaxTirIndirect_SansTesterEtatMunitions",
        boost::function< float( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetMaxRangeToIndirectFireWithoutAmmoCheck, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Tir_PorteeMinTirIndirect_SansTesterEtatMunitions",
        boost::function< float( const PHY_DotationCategory* ) >( boost::bind( &DEC_FireFunctions::GetMinRangeToIndirectFireWithoutAmmoCheck, boost::ref( GetPion() ), _1 ) ) );   
    brain.RegisterFunction( "DEC_Tir_LancerFumigeneSurConnaissance",
        boost::function< void( boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_FireFunctions::ThrowSmokeOnKnowledgeAgent, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_Tir_MunitionPourTirIndirect",
        boost::function< const PHY_DotationCategory* ( int, const MT_Vector2D* ) >( boost::bind( &DEC_FireFunctions::GetMunitionForIndirectFire, boost::ref( GetPion() ), _1, _2 ) ) );
    //
    brain.RegisterFunction( "DEC_Agent_PeutConstruireContournementObjet", 
        boost::function< bool ( boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_DynamicFireFunctions::GetRangeToExtinguish, boost::cref( GetPion() ), _1 ) ) );

    // RC
    brain.RegisterFunction( "DEC_RC1",
        boost::function< void ( int, int ) >( boost::bind( &DEC_MiscFunctions::Report< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_RC_AgentKnowledge",
        boost::function< void ( int, int, boost::shared_ptr< DEC_Knowledge_Agent > ) >( boost::bind( &DEC_MiscFunctions::ReportAgentKnowledge< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_DotationType",
        boost::function< void ( int, int, const PHY_DotationCategory* ) >( boost::bind( &DEC_MiscFunctions::ReportDotationType< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_EquipmentType",
        boost::function< void ( int, int, const PHY_ComposanteTypePion* ) >( boost::bind( &DEC_MiscFunctions::ReportEquipmentType< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_Float",
        boost::function< void ( int, int, float ) >( boost::bind( &DEC_MiscFunctions::ReportFloat< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_Float_Float",
        boost::function< void ( int, int, float, float ) >( boost::bind( &DEC_MiscFunctions::ReportFloatFloat< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_Id",
        boost::function< void ( int, int, int ) >( boost::bind( &DEC_MiscFunctions::ReportId< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_ObjectKnowledge",
        boost::function< void ( int, int, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind( &DEC_MiscFunctions::ReportObjectKnoweldge< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_Pion",
        boost::function< void ( int, int, DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::ReportPion< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_Pion_Automate",
        boost::function< void ( int, int, DEC_Decision_ABC*, DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::ReportPionAutomate< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3, _4 ) ) );
    brain.RegisterFunction( "DEC_RC_PopulationKnowledge",
        boost::function< void ( int, int, int ) >( boost::bind( &DEC_MiscFunctions::ReportPopulationKnowledge< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_RC_TirPion",
        boost::function< void ( int, int, int ) >( boost::bind( &DEC_MiscFunctions::ReportTirPion< MIL_AgentPion >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    
    // Hirarchie
    brain.RegisterFunction( "DEC_Pion_PionsAvecPC", boost::bind( &DEC_AgentFunctions::GetPionsWithPC, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Pion_PionsSansPC", boost::bind( &DEC_AgentFunctions::GetPionsWithoutPC, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Pion_PionPC", boost::bind( &DEC_AgentFunctions::GetPionPC, boost::cref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Pion_PionPCDeAutomate", 
        boost::function< DEC_Decision_ABC*( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::GetPionPCOfAutomate, _1 ) ) );
    brain.RegisterFunction( "DEC_Pion_PionsDeAutomateSansPC",
        boost::function< std::vector< DEC_Decision_ABC* >( const DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::GetPionsWithoutPCOfAutomate, _1 ) ) );
    brain.RegisterFunction( "DEC_Pion_ChangeAutomate", 
        boost::function< bool( DEC_Decision_ABC* ) >( boost::bind( &DEC_AgentFunctions::ChangeAutomate, boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_GetAutomate", 
        boost::function< DEC_Decision_ABC*( DEC_Decision_ABC* ) >( boost::bind( &DEC_MiscFunctions::GetAutomate, _1 ) ) );

    // Logistique
    brain.RegisterFunction( "DEC_StartPreterVSRAM",        
        boost::function< PHY_Action_ABC*( DEC_RolePion_Decision*, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionLendCollectionComposantes, DEC_RolePion_Decision*, unsigned int >, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_StartPreterRemorqueurs",
        boost::function< PHY_Action_ABC*( DEC_RolePion_Decision*, unsigned int ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionLendHaulerComposantes, DEC_RolePion_Decision*, unsigned int >, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_RecupererVSRAM",
        boost::function< void( const DEC_Decision_ABC*, const uint ) >( boost::bind( &DEC_LogisticFunctions::UndoLendCollectionComposantes, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_RecupererRemorqueurs",
        boost::function< void( const DEC_Decision_ABC*, const uint ) >( boost::bind( &DEC_LogisticFunctions::UndoLendHaulerComposantes, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Pion_TC2",
        boost::bind( &DEC_LogisticFunctions::PionGetTC2, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_DemandeDeRavitaillement" , &DEC_LogisticFunctions::PionRequestSupply                                  );
    brain.RegisterFunction( "DEC_ChangeValeurDotations2", 
        boost::function< void( int, const MT_Float ) >( boost::bind( &DEC_LogisticFunctions::ChangeDotationsValueUsingTC2, boost::ref( GetPion() ), _1, _2, -1 ) ) );
    brain.RegisterFunction( "DEC_ChangeValeurDotations3",
        boost::function< void( int, const MT_Float, int ) >( boost::bind( &DEC_LogisticFunctions::ChangeDotationsValueUsingTC2, boost::ref( GetPion() ), _1, _2, _3 ) ) );

    // Transport / Hliportage
    brain.RegisterFunction( "DEC_Transport_AjouterPion",
        boost::function< void( DEC_Decision_ABC*, bool ) >( boost::bind( &DEC_ActionFunctions::Transport_AddPion, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Transport_AjouterPions",
        boost::function< void( const std::vector< DEC_Decision_ABC* >&, bool ) >( boost::bind( &DEC_ActionFunctions::Transport_AddPions, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Start_TransportEmbarquer"           , boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTransportLoad >, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Start_TransportDebarquer"           , boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTransportUnload >, boost::ref( GetPion() ) ) );
    brain.RegisterFunction( "DEC_Transport_EmbarquerPionSansDelais"  ,
    		boost::function< void( const DEC_Decision_ABC*, bool ) >( boost::bind( &DEC_ActionFunctions::Transport_MagicLoadPion, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Transport_EmbarquerPionsSansDelais" ,
    		boost::function< void (const std::vector< DEC_Decision_ABC* >&, bool) >( boost::bind( &DEC_ActionFunctions::Transport_MagicLoadPions, boost::ref(GetPion()) , _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Transport_DebarquerPionSansDelais"  ,
    		boost::function<void(const DEC_Decision_ABC*)>( boost::bind( &DEC_ActionFunctions::Transport_MagicUnloadPion , boost::ref( GetPion()) , _1 ) ) );
    brain.RegisterFunction( "DEC_Transport_EstTermine"               ,
    		boost::bind( &DEC_ActionFunctions::Transport_IsFinished , boost::ref(GetPion())                    ) );
    brain.RegisterFunction( "DEC_Transport_Annuler"                  ,
    		boost::bind( &DEC_ActionFunctions::Transport_Cancel , boost::ref(GetPion())                    ) );
    brain.RegisterFunction( "DEC_Transport_EnCoursDeTransport",
                            boost::bind( &DEC_ActionFunctions::Transport_IsTransporting, boost::ref( GetPion() ) ) );

    brain.RegisterFunction( "DEC_Agent_PeutTransporterPion",
            boost::function< bool( const DEC_Decision_ABC*, bool ) >( boost::bind( &DEC_ActionFunctions::CanTransportPion, boost::ref( GetPion() ), _1, _2 ) ) );

    // Prisonniers
    brain.RegisterFunction( "DEC_Prisonniers_CapturerEtEmbarquer" ,
    		boost::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > )>(boost::bind(&DEC_ActionFunctions::Prisoners_CaptureAndLoad , boost::ref( GetPion()) ,_1 ) ) );
    brain.RegisterFunction( "DEC_Prisonniers_Debarquer"           ,
    		boost::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > )>(boost::bind(&DEC_ActionFunctions::Prisoners_Unload , boost::ref(GetPion()), _1 ) ) );
    brain.RegisterFunction( "DEC_Prisonniers_DebarquerDansCamp"   ,
    		boost::function< void (boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< DEC_Knowledge_Object > )>(boost::bind(&DEC_ActionFunctions::Prisoners_UnloadInCamp , boost::ref( GetPion() ), _1, _2  ) ) );
    brain.RegisterFunction( "DEC_Prisonniers_EstEmbarque"         ,
    		boost::function< bool (boost::shared_ptr< DEC_Knowledge_Agent >) >(boost::bind(&DEC_ActionFunctions::PrisonnersRefugees_IsLoaded , boost::ref( GetPion() ), _1  ) ) );
    brain.RegisterFunction( "DEC_Prisonniers_EstDebarqueDansCamp" ,
    		boost::function< bool (boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< DEC_Knowledge_Object > )>( boost::bind(&DEC_ActionFunctions::Prisoners_IsUnloadedInCamp , boost::ref( GetPion()), _1, _2 ) ) );
    
    // Refugies
    brain.RegisterFunction( "DEC_Refugies_OrienterEtEmbarquer" ,
    		boost::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > ) > ( boost::bind(&DEC_ActionFunctions::Refugees_OrientateAndLoad , boost::ref(GetPion()), _1 ) ) );
    brain.RegisterFunction( "DEC_Refugies_Debarquer"           ,
		boost::function< void ( boost::shared_ptr< DEC_Knowledge_Agent > ) > ( boost::bind(&DEC_ActionFunctions::Refugees_Unload , boost::ref(GetPion()), _1 ) ) );
    brain.RegisterFunction( "DEC_Refugies_DebarquerDansCamp"   ,
    		boost::function< void ( boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< DEC_Knowledge_Object > )> ( boost::bind(&DEC_ActionFunctions::Refugees_UnloadInCamp , boost::ref( GetPion() ), _1, _2  ) ) );
    brain.RegisterFunction( "DEC_Refugies_EstEmbarque"         ,
    		boost::function< bool ( boost::shared_ptr< DEC_Knowledge_Agent > ) > ( boost::bind ( &DEC_ActionFunctions::PrisonnersRefugees_IsLoaded , boost::ref( GetPion() ), _1  ) ) );
    brain.RegisterFunction( "DEC_Refugies_EstDebarqueDansCamp" ,
       		boost::function< bool (boost::shared_ptr< DEC_Knowledge_Agent >, boost::shared_ptr< DEC_Knowledge_Object > ) >( boost::bind(&DEC_ActionFunctions::Refugees_IsUnloadedInCamp , boost::ref( GetPion() ), _1, _2 ) ) );

    // Emergency functions
    brain.RegisterFunction( "DEC_Start_EmergencyInfluence",
        boost::function< PHY_Action_ABC*( const std::string&, double ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionInfluence, const std::string&, double >, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Start_EmergencyTriggerActivity",
        boost::function< PHY_Action_ABC*( const std::string&, double ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTriggerActivity, const std::string&, double >, boost::ref( GetPion() ), _1, _2 ) ) );
    brain.RegisterFunction( "DEC_Start_EmergencyInfluenceInArea",
        boost::function< PHY_Action_ABC*( const std::string&, double, const TER_Localisation* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionInfluenceInArea, const std::string&, double, const TER_Localisation* >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Start_EmergencyTriggerActivityInArea",
        boost::function< PHY_Action_ABC*( const std::string&, double, const TER_Localisation* ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionTriggerActivityInArea, const std::string&, double, const TER_Localisation* >, boost::ref( GetPion() ), _1, _2, _3 ) ) );
    brain.RegisterFunction( "DEC_Start_EmergencyUnloadActivity",
        boost::function< PHY_Action_ABC*( const std::string&, double ) >( boost::bind( &DEC_ActionFunctions::StartAction< PHY_ActionUnloadActivity, const std::string&, double >, boost::ref( GetPion() ), _1, _2 ) ) );    

    // Representations
    brain.RegisterFunction( "DEC_GetOrdersCategory",
                            boost::bind( &DEC_MiscFunctions::GetOrdersCategory , boost::ref( *pEntity_ ) ) );
    brain.RegisterFunction( "DEC_GetPointsCategory",
                            boost::bind( &DEC_MiscFunctions::GetPointsCategory , boost::ref( *pEntity_ ) ) );
    brain.RegisterFunction( "DEC_RemoveFromOrdersCategory",
        boost::function< void ( MIL_FragOrder* ) > ( boost::bind( &DEC_MiscFunctions::RemoveFromOrdersCategory , boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_DeleteRepresentation",
        boost::function< void ( MIL_FragOrder* ) > ( boost::bind( &DEC_MiscFunctions::DeleteOrderRepresentation , boost::ref( GetPion() ), _1 ) ) );
    brain.RegisterFunction( "DEC_RemoveFromPointsCategory" ,
        boost::function< void( DEC_PathPoint* )>( boost::bind( &DEC_MiscFunctions::RemoveFromPointsCategory, boost::ref( *pEntity_ ), _1 ) ) );
    brain.RegisterFunction( "DEC_GetSzName", &DEC_MiscFunctions::GetName );
        
    // Missions
    brain.RegisterFunction( "DEC_GetDirectionDanger", &DEC_AgentFunctions::GetDirectionDanger );
    
    DEC_CommunicationFunctions::Register( brain );

    pEntity_->GetType().RegisterFunctions( brain, GetPion() );
}

// =============================================================================
// DEFAULT BEHAVIOR MANAGEMENT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::StartMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::StartMissionBehavior( MIL_Mission_ABC& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior();
    
    ActivateOrder( strBehavior, mission );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::StopMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::StopMissionBehavior( MIL_Mission_ABC& mission )
{
    const std::string& strBehavior = mission.GetType().GetDIABehavior();
    StopMission( strBehavior );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::StopMissionBehavior
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyHasChanged()
{
    bStateHasChanged_ = true;
    GetPion().Apply( &network::NetworkNotificationHandler_ABC::NotifyDataHasChanged );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyRoePopulationChanged
// Created: NLD 2005-11-15
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyRoePopulationChanged( const PHY_RoePopulation& roe )
{
    assert( pRoePopulation_ );
    if( roe != *pRoePopulation_ )
    {
        pRoePopulation_   = &roe;
        NotifyHasChanged();
    }
}

// =============================================================================
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SendFullState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SendFullState( NET_ASN_MsgUnitAttributes& msg ) const
{
    assert( pRoePopulation_ );

    msg().m.rapport_de_forcePresent              = 1;    
    msg().m.combat_de_rencontrePresent           = 1;
    msg().m.etat_operationnelPresent             = 1;
    msg().m.disponibilite_au_tir_indirectPresent = 1;
    msg().m.roePresent                           = 1;
    msg().m.roe_populationPresent                = 1;

    msg().rapport_de_force              = (ASN1T_EnumForceRatioStatus)nForceRatioState_;
    msg().combat_de_rencontre           = (ASN1T_EnumMeetingEngagementStatus)nCloseCombatState_;
    msg().etat_operationnel             = (ASN1T_EnumOperationalStatus)nOperationalState_;
    msg().disponibilite_au_tir_indirect = (ASN1T_EnumFireAvailability)nIndirectFireAvailability_;
    msg().roe                           = (ASN1T_EnumRoe)nRulesOfEngagementState_;
    msg().roe_population                = pRoePopulation_->GetAsnID();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SendChangedState
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const
{
    if( bStateHasChanged_ )
        SendFullState( msg );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::RegisterSelf
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::RegisterSelf( directia::Brain& brain )
{
    brain.RegisterObject( "myself", (DEC_Decision_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::UpdateMeKnowledge
// Created: MGD 2009-11-01
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::UpdateMeKnowledge( directia::Brain& brain )
{
    const MT_Vector2D*  pos = GetPosition();
    brain.GetScriptFunction( "InitializeMe" )( std::string("net.masagroup.sword.military.world.Agent"), (DEC_Decision_ABC*)this, pos->rX_, pos->rY_, 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatDec
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatDec() const
{
    return eEtatDec_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatDec
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatDec( int value )
{
    eEtatDec_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatDecPrudence
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatDecPrudence() const
{
    return eEtatDecPrudence_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatDecPrudence
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatDecPrudence( int value )
{
    eEtatDecPrudence_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatLima
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatLima() const
{
    return eEtatLima_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatLima
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatLima( int value )
{
    eEtatLima_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatNbc
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatNbc() const
{
    return eEtatNbc_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatNbc
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatNbc( int value )
{
    eEtatNbc_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatDestruction
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatDestruction() const
{
    return eEtatDestruction_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatDestruction
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatDestruction( int value )
{
    eEtatDestruction_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatFeu
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatFeu() const
{
    return eEtatFeu_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatFeu
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatFeu( int value )
{
    eEtatFeu_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatAmbiance
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatAmbiance() const
{
    return eEtatAmbiance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatAmbiance
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatAmbiance( int value )
{
    eEtatAmbiance_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatRadio
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatRadio() const
{
    return eEtatRadio_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatRadio
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatRadio( int value )
{
    eEtatRadio_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatRadar
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatRadar() const
{
    return eEtatRadar_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatRadar
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatRadar( int value )
{
    eEtatRadar_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatDeplacement
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatDeplacement() const
{
    return eEtatDeplacement_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatDeplacement
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatDeplacement( int value )
{
    eEtatDeplacement_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatOrdreCoordination
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatOrdreCoordination() const
{
    return eEtatOrdreCoordination_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatOrdreCoordination
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatOrdreCoordination( int value )
{
    eEtatOrdreCoordination_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteConsigneTir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteConsigneTir() const
{
    return eConsigneTir_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteConsigneTir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteConsigneTir( int value )
{
    eConsigneTir_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteConsigneTirPopulation
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteConsigneTirPopulation() const
{
    return eConsigneTirPopulation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteConsigneTirPopulation
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteConsigneTirPopulation( int value )
{
    eConsigneTirPopulation_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatEchelon
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatEchelon() const
{
    return eEtatEchelon_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatEchelon
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatEchelon( int value )
{
    eEtatEchelon_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatSoutien
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatSoutien() const
{
    return eEtatSoutien_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatSoutien
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatSoutien( int value )
{
    eEtatSoutien_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatSituationEnnemi
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatSituationEnnemi() const
{
    return eEtatSituationEnnemi_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatSituationEnnemi
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatSituationEnnemi( int value )
{
    eEtatSituationEnnemi_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteEtatPhaseMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteEtatPhaseMission() const
{
    return eEtatPhaseMission_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteEtatPhaseMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteEtatPhaseMission( int value )
{
    eEtatPhaseMission_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetbOrdreInterrompreMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::GetbOrdreInterrompreMission() const
{
    return bOrdreInterrompreMission_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetbOrdreInterrompreMission
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetbOrdreInterrompreMission( bool value )
{
    bOrdreInterrompreMission_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetbOrdreDecrocher
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::GetbOrdreDecrocher() const
{
    return bOrdreDecrocher_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetbOrdreDecrocher
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetbOrdreDecrocher( bool value )
{
    bOrdreDecrocher_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetbOrdreTenirSurLR
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::GetbOrdreTenirSurLR() const
{
    return bOrdreTenirSurLR_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetbOrdreTenirSurLR
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetbOrdreTenirSurLR( bool value )
{
    bOrdreTenirSurLR_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetbOrdreTenir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::GetbOrdreTenir() const
{
    return bOrdreTenir_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetbOrdreTenir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetbOrdreTenir( bool value )
{
    bOrdreTenir_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetbPasserSurLC
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::GetbPasserSurLC() const
{
    return bPasserSurLC_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetbPasserSurLC
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetbPasserSurLC( bool value )
{
    bPasserSurLC_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetpionEnEscorte
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
DEC_Decision_ABC* DEC_RolePion_Decision::GetpionEnEscorte() const
{
    return pionEnEscorte_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetpionEnEscorte
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetpionEnEscorte( DEC_Decision_ABC* value )
{
    pionEnEscorte_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetitMvt
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Path_ABC > DEC_RolePion_Decision::GetitMvt() const
{
    return itMvt_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetitMvt
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetitMvt( const boost::shared_ptr< DEC_Path_ABC >& value )
{
    itMvt_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetobjectifEsquive
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_RolePion_Decision::GetobjectifEsquive() const
{
    return objectifEsquive_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetobjectifEsquive
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetobjectifEsquive( MT_Vector2D* value )
{
    if( value )
        objectifEsquive_.reset( new MT_Vector2D( *value ) );
    else
        objectifEsquive_.reset();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteniEnCours
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
boost::shared_ptr< DEC_Knowledge_Agent > DEC_RolePion_Decision::GeteniEnCours() const
{
    return eniEnCours_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteniEnCours
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteniEnCours( boost::shared_ptr< DEC_Knowledge_Agent > id )
{
    eniEnCours_ = id;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetmissionPrecedente
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
std::string DEC_RolePion_Decision::GetmissionPrecedente() const
{
    return missionPrecedente_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetmissionPrecedente
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetmissionPrecedente( const std::string& value )
{
    missionPrecedente_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetrTenir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
float DEC_RolePion_Decision::GetrTenir() const
{
    return rTenir_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetrTenir
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetrTenir( float value )
{
    rTenir_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteTypeContact
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteTypeContact() const
{
    return eTypeContact_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteTypeContact
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteTypeContact( int value )
{
    eTypeContact_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GeteNiveauAction
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
int DEC_RolePion_Decision::GeteNiveauAction() const
{
    return eNiveauAction_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SeteNiveauAction
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SeteNiveauAction( int value )
{
    eNiveauAction_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetbDefenseStatique_Mobile
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::GetbDefenseStatique_Mobile() const
{
    return bDefenseStatique_Mobile_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::SetbDefenseStatique_Mobile
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::SetbDefenseStatique_Mobile( bool value )
{
    bDefenseStatique_Mobile_ = value;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetselUnitesEnAppui
// Created: LDC 2009-05-19
// -----------------------------------------------------------------------------
const std::vector< DEC_Decision_ABC* >* DEC_RolePion_Decision::GetselUnitesEnAppui() const
{
    return &pionsEnAppui_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::AddToselUnitesEnAppui
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::AddToselUnitesEnAppui( DEC_Decision_ABC* pPion )
{
    pionsEnAppui_.push_back( pPion );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::RemoveFromselUnitesEnAppui
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::RemoveFromselUnitesEnAppui( DEC_Decision_ABC* pPion )
{
    for ( std::vector< DEC_Decision_ABC* >::iterator it = pionsEnAppui_.begin(); it != pionsEnAppui_.end(); ++it )
        if( *it == pPion )
        {
            pionsEnAppui_.erase( it );
            return;
        }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsNeutralized
// Created: LDC 2009-06-18
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsNeutralized() const
{
    return GetPion().IsNeutralized();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsMoving
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsMoving() const
{
    return( GetPion().GetRole< PHY_RoleInterface_Location >().GetCurrentSpeed() != 0. );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsContaminated
// Created: LDC 2009-06-18
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsContaminated() const
{
    return GetPion().GetRole< nbc::PHY_RoleInterface_NBC >().IsContaminated();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetPosition
// Created: LDC 2009-06-19
// -----------------------------------------------------------------------------
const MT_Vector2D* DEC_RolePion_Decision::GetPosition() const
{
    return DEC_AgentFunctions::GetPosition( GetPion() ).get();
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetPion
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
MIL_AgentPion& DEC_RolePion_Decision::GetPion() const
{
    assert( pEntity_ );
    return *pEntity_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::Clean
// Created: NLD 2004-09-08
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::Clean()
{
    bStateHasChanged_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetRoePopulation
// Created: NLD 2005-11-16
// -----------------------------------------------------------------------------
const PHY_RoePopulation& DEC_RolePion_Decision::GetRoePopulation() const
{
    assert( pRoePopulation_ );
    return *pRoePopulation_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsPC
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsPC() const
{
    return DEC_AgentFunctions::IsPC( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsTransported
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsTransported() const
{
    return DEC_AgentFunctions::IsTransported( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsFlying
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsFlying() const
{
    return DEC_AgentFunctions::IsFlying( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetMajorOperationalState
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
MT_Float DEC_RolePion_Decision::GetMajorOperationalState() const
{
    return DEC_AgentFunctions::GetMajorOperationalState( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsAutomateEngaged
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsAutomateEngaged() const
{
    return DEC_AgentFunctions::IsAutomateEngaged( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::IsDead
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
bool DEC_RolePion_Decision::IsDead() const
{
    return DEC_AgentFunctions::IsDead( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::WearNbcProtectionSuit
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::WearNbcProtectionSuit() const
{
    DEC_AgentFunctions::WearNbcProtectionSuit( GetPion() );
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::RemoveNbcProtectionSuit
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::RemoveNbcProtectionSuit() const
{
    DEC_AgentFunctions::RemoveNbcProtectionSuit( GetPion() );
}

// =============================================================================
// NOTIFICATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyIndirectFireAvailabilityChanged
// Created: NLD 2005-10-19
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyIndirectFireAvailabilityChanged( E_FireAvailability nState )
{
    if( nIndirectFireAvailability_ != nState )
    {
        nIndirectFireAvailability_ = nState;
        NotifyHasChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyForceRatioStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyForceRatioStateChanged( E_ForceRatioState nState )
{
    if( nForceRatioState_ != nState )
    {
        nForceRatioState_ = nState;
        NotifyHasChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyRulesOfEngagementStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyRulesOfEngagementStateChanged( E_RulesOfEngagementState nState )
{
    if( nRulesOfEngagementState_ != nState )
    {
        nRulesOfEngagementState_ = nState;
        NotifyHasChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyCloseCombatStateChanged
// Created: NLD 2004-10-15
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyCloseCombatStateChanged( E_CloseCombatState nState )
{
    if( nCloseCombatState_ != nState )
    {
        nCloseCombatState_ = nState;
        NotifyHasChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::NotifyOperationalStateChanged
// Created: NLD 2005-07-26
// -----------------------------------------------------------------------------
void DEC_RolePion_Decision::NotifyOperationalStateChanged( E_OperationalState nState )
{
    if( nOperationalState_ != nState )
    {
        nOperationalState_ = nState;
        NotifyHasChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetName
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
std::string DEC_RolePion_Decision::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetDecAutomate
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
DEC_AutomateDecision* DEC_RolePion_Decision::GetDecAutomate() const
{
    return pAutomate_;
}

// -----------------------------------------------------------------------------
// Name: DEC_RolePion_Decision::GetDIAType
// Created: LDC 2009-07-09
// -----------------------------------------------------------------------------
const std::string& DEC_RolePion_Decision::GetDIAType() const
{    
    return DEC_Decision< MIL_AgentPion >::GetDIAType();
}
