// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/MIL_EntityManager.cpp $
// $Author: Nld $
// $Modtime: 10/06/05 10:19 $
// $Revision: 45 $
// $Workfile: MIL_EntityManager.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "MIL_EntityManager.h"

#include "ArmyFactory.h"
#include "AutomateFactory.h"
#include "FormationFactory.h"
#include "InhabitantFactory.h"
#include "KnowledgesVisitor_ABC.h"
#include "MagicOrderManager.h"
#include "MIL_AgentServer.h"
#include "MIL_Army.h"
#include "MIL_EntityManagerStaticMethods.h"
#include "MIL_Formation.h"
#include "MissionController.h"
#include "OnComponentComputer_ABC.h"
#include "PopulationFactory.h"

#include "Actions/PHY_FireResults_Default.h"
#include "Adapters/Legacy/Sink.h"
#include "Agents/Actions/Firing/PHY_FireResults_Pion.h"
#include "Agents/MIL_AgentPion.h"
#include "Agents/MIL_AgentTypePion.h"
#include "Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Agents/Roles/Illumination/PHY_RoleInterface_Illumination.h"
#include "Agents/Roles/Logistic/FuneralConfig.h"
#include "Agents/Roles/Logistic/PHY_MaintenanceResourcesAlarms.h"
#include "Agents/Roles/Logistic/PHY_MedicalResourcesAlarms.h"
#include "Agents/Roles/Logistic/PHY_SupplyResourcesAlarms.h"
#include "Agents/Roles/Logistic/SupplyConvoyConfig.h"
#include "Agents/Units/Categories/PHY_NatureAtlas.h"
#include "Agents/Units/Categories/PHY_NatureLevel.h"
#include "Agents/Units/Categories/PHY_Protection.h"
#include "Agents/Units/Categories/PHY_RoePopulation.h"
#include "Agents/Units/Categories/PHY_Volume.h"
#include "Agents/Units/Composantes/PHY_ComposanteState.h"
#include "Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Agents/Units/Dotations/PHY_DotationLogisticType.h"
#include "Agents/Units/Dotations/PHY_DotationNature.h"
#include "Agents/Units/Dotations/PHY_DotationType.h"
#include "Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Agents/Units/HumanFactors/PHY_Experience.h"
#include "Agents/Units/HumanFactors/PHY_Morale.h"
#include "Agents/Units/HumanFactors/PHY_Stress.h"
#include "Agents/Units/HumanFactors/PHY_Tiredness.h"
#include "Agents/Units/Humans/PHY_HumanRank.h"
#include "Agents/Units/Humans/PHY_HumanWound.h"
#include "Agents/Units/Logistic/PHY_MaintenanceLevel.h"
#include "Agents/Units/Logistic/PHY_MaintenanceWorkRate.h"
#include "Agents/Units/Radars/PHY_RadarClass.h"
#include "Agents/Units/Radars/PHY_RadarType.h"
#include "Agents/Units/Sensors/PHY_SensorType.h"
#include "Agents/Units/Weapons/PHY_LauncherType.h"
#include "Agents/Units/Weapons/PHY_WeaponType.h"
#include "Automates/MIL_Automate.h"
#include "Automates/MIL_AutomateType.h"
#include "CheckPoints/SerializationTools.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Effects/MIL_EffectManager.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Logistic/PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Roles/Logistic/SupplyDotationManualRequestBuilder.h"
#include "Entities/Agents/Roles/Logistic/SupplyStockPushFlowRequestBuilder.h"
#include "Entities/Agents/Roles/Perception/PHY_RoleInterface_Perceiver.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Objects/BurnSurfaceAttribute.h"
#include "Entities/Objects/MIL_DisasterType.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Entities/Objects/MIL_ObjectManager.h"
#include "Entities/Orders/MIL_PopulationOrderManager.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Entities/Populations/MIL_FlowCollisionManager.h"
#include "Entities/Specialisations/LOG/LogisticHierarchy_ABC.h"
#include "Entities/Specialisations/LOG/LogisticLink_ABC.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "Inhabitants/MIL_Inhabitant.h"
#include "Inhabitants/MIL_InhabitantType.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_KS_ObjectKnowledgeSynthetizer.h"
#include "Knowledge/KnowledgeGroupFactory.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroupType.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_Logger.h"
#include "Network/NET_Publisher_ABC.h"
#include "Objects/MIL_FireClass.h"
#include "Objects/MIL_NbcAgentType.h"
#include "Objects/MIL_Object_ABC.h"
#include "Orders/MIL_LimaFunction.h"
#include "Orders/MIL_Report.h"
#include "PathfindComputer.h"
#include "Populations/MIL_Population.h"
#include "Populations/MIL_PopulationAttitude.h"
#include "Populations/MIL_PopulationType.h"
#include "propagation/FloodModel.h"
#include "protocol/ClientSenders.h"
#include "protocol/Protocol.h"
#include "resource_network/ResourceNetworkModel.h"
#include "tools/Loader_ABC.h"
#include "Tools/MIL_Config.h"
#include "Tools/MIL_DictionaryExtensions.h"
#include "Tools/MIL_IDManager.h"
#include "Tools/MIL_MessageParameters.h"
#include "Tools/MIL_ProfilerManager.h"
#include "Tools/NET_AsnException.h"
#include "tools/SchemaWriter.h"
#include "Urban/MIL_UrbanCache.h"
#include "Urban/MIL_UrbanObject.h"
#include "Urban/PHY_AccomodationType.h"
#include "Urban/PHY_InfrastructureType.h"
#include "Urban/PHY_MaterialCompositionType.h"
#include "Urban/PHY_ResourceNetworkType.h"
#include "Urban/PHY_RoofShapeType.h"
#include "simulation_terrain/TER_World.h"

#include <boost/make_shared.hpp>
#include <boost/lexical_cast.hpp>
#include <tuple>

using namespace sword;

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_EntityManager )

template< typename Archive >
void save_construct_data( Archive& /*archive*/, const MIL_EntityManager* /*manager*/, const unsigned int /*version*/ )
{
    // NOTHING
}

template< typename Archive >
void load_construct_data( Archive& archive, MIL_EntityManager* manager, const unsigned int /*version*/ )
{
    ::new( manager )MIL_EntityManager( MIL_Time_ABC::GetTime(),
            MIL_EffectManager::GetEffectManager(),
            MIL_AgentServer::GetWorkspace().GetConfig(), archive.GetWorld(),
            MIL_AgentServer::GetWorkspace().GetPathFindManager() );
}

void TerminatePhysicalSingletons()
{
    PHY_SensorType                ::Terminate();
    PHY_ComposanteTypePion        ::Terminate();
    PHY_WeaponType                ::Terminate();
    PHY_LauncherType              ::Terminate();
    PHY_DotationType              ::Terminate();
    PHY_DotationNature            ::Terminate();
    PHY_Protection                ::Terminate();
    PHY_Volume                    ::Terminate();
    PHY_HumanRank                 ::Terminate();
    PHY_HumanWound                ::Terminate();
    PHY_ComposanteState           ::Terminate();
    PHY_IndirectFireDotationClass ::Terminate();
    PHY_AmmoDotationClass         ::Terminate();
    PHY_Experience                ::Terminate();
    PHY_Stress                    ::Terminate();
    PHY_Tiredness                 ::Terminate();
    PHY_Morale                    ::Terminate();
    PHY_MaintenanceLevel          ::Terminate();
    PHY_PerceptionLevel           ::Terminate();
    PHY_RadarClass                ::Terminate();
    PHY_NatureLevel               ::Terminate();
    PHY_NatureAtlas               ::Terminate();
    PHY_RoePopulation             ::Terminate();
    PHY_MaintenanceWorkRate       ::Terminate();
    PHY_RadarType                 ::Terminate();
    PHY_DotationLogisticType      ::Terminate();
    PHY_SupplyResourcesAlarms     ::Terminate();
    PHY_MaintenanceResourcesAlarms::Terminate();
    PHY_MedicalResourcesAlarms    ::Terminate();
    PHY_MaterialCompositionType   ::Terminate();
    PHY_AccomodationType          ::Terminate();
    PHY_InfrastructureType        ::Terminate();
    PHY_RoofShapeType             ::Terminate();
    PHY_ResourceNetworkType       ::Terminate();
}

void TerminateMilitarySingletons()
{
    MIL_PopulationAttitude        ::Terminate();
    MIL_AutomateType              ::Terminate();
    MIL_AgentTypePion             ::Terminate();
    MIL_NbcAgentType              ::Terminate();
    MIL_FireClass                 ::Terminate();
    MIL_DisasterType              ::Terminate();
    MIL_KnowledgeGroupType        ::Terminate();
    MIL_PopulationType            ::Terminate();
    MIL_InhabitantType            ::Terminate();
    MIL_LimaFunction              ::Terminate();
}

void TerminateLogisticsSingletons()
{
    logistic::FuneralConfig       ::Terminate();
    logistic::SupplyConvoyConfig  ::Terminate();
}

namespace
{
    std::string GetStringParam( const sword::MissionParameters& params, int index )
    {
        const auto& elem = params.elem( index );
        if( elem.value_size() != 1 )
            throw MASA_BADPARAM_UNIT( "parameters[" << index << "] value size must be 1,"
                    " got " << elem.value_size() );
        if( !elem.value( 0 ).has_acharstr() )
            throw MASA_BADPARAM_UNIT( "parameters[" << index << "] must be an ACharStr" );
        return elem.value( 0 ).acharstr();
    }

    MIL_Automate* TaskerToAutomat( MIL_EntityManager_ABC& manager, const Tasker& tasker )
    {
        return tasker.has_automat() && tasker.automat().has_id() ? manager.FindAutomate( tasker.automat().id() ) : 0;
    }

    MIL_Formation* TaskerToFormation( MIL_EntityManager& manager, const Tasker& tasker )
    {
        return tasker.has_formation() && tasker.formation().has_id() ? manager.FindFormation( tasker.formation().id() ) : 0;
    }

    logistic::LogisticHierarchy_ABC* TaskerToLogisticHierarchy( MIL_EntityManager& manager, const Tasker& tasker )
    {
        // Return the targeted logistic hierarchy
        // For an automat, which can have its own logistic hierarchy and the logistic hierarchy of its potential logistic brain
        //  the method returns the logistic BRAIN hierarchy: in this case, the automat's logistic hierarchy is always itself.
        MIL_Automate* pAutomateTmp = TaskerToAutomat( manager, tasker );
        if( pAutomateTmp )
        {
            if( pAutomateTmp->GetBrainLogistic() )
                return &pAutomateTmp->GetBrainLogistic()->GetLogisticHierarchy();
            else
                return &pAutomateTmp->GetLogisticHierarchy();
        }
        MIL_Formation* pFormationTmp = TaskerToFormation( manager, tasker );
        if( pFormationTmp && pFormationTmp->GetBrainLogistic() )
            return &pFormationTmp->GetBrainLogistic()->GetLogisticHierarchy();
        return 0;
    }
}

void MIL_EntityManager::Initialize( const tools::PhyLoader& loader, const MIL_Time_ABC& time,
        MIL_ObjectFactory& objectFactory )
{
    MIL_EntityManagerStaticMethods::Initialize( loader, time, objectFactory );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager constructor
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
MIL_EntityManager::MIL_EntityManager( const MIL_Time_ABC& time,
        MIL_EffectManager& effects, MIL_ObjectFactory& objectFactory,
        const MIL_Config& config, const boost::shared_ptr< const TER_World >& world,
        DEC_PathFind_Manager& pathfindManager )
    : time_                         ( time )
    , gcPause_                      ( config.GetGarbageCollectorPause() )
    , gcMult_                       ( config.GetGarbageCollectorStepMul() )
    , effectManager_                ( effects )
    , bSendUnitVisionCones_         ( false )
    , bEnableRandomBreakdowns_      ( config.EnableRandomBreakdowns() )
    , profilerManager_              ( new MIL_ProfilerManager( config ) )
    , nRandomBreakdownsNextTimeStep_( 0 )
    , rKnowledgesTime_              ( 0 )
    , rAutomatesDecisionTime_       ( 0 )
    , rPionsDecisionTime_           ( 0 )
    , rPopulationsDecisionTime_     ( 0 )
    , rActionsTime_                 ( 0 )
    , rEffectsTime_                 ( 0 )
    , rStatesTime_                  ( 0 )
    , idManager_                    ( new MIL_IDManager() )
    , missionController_            ( new MissionController() )
    , inhabitantFactory_            ( new InhabitantFactory() )
    , populationFactory_            ( new PopulationFactory( *missionController_, gcPause_, gcMult_, config.IsDecisionalLoggerEnabled() ) )
    , sink_                         ( new sword::legacy::Sink( *idManager_, *missionController_, gcPause_, gcMult_, config.IsDecisionalLoggerEnabled(), world ) )
    , pObjectManager_               ( new MIL_ObjectManager( objectFactory ) )
    , pFloodModel_                  ( sink_->CreateFloodModel().release() )
    , automateFactory_              ( new AutomateFactory( *idManager_, *missionController_, gcPause_, gcMult_, config.IsDecisionalLoggerEnabled() ) )
    , formationFactory_             ( new FormationFactory( *automateFactory_ ) )
    , knowledgeGroupFactory_        ( new KnowledgeGroupFactory() )
    , armyFactory_                  ( new ArmyFactory( *automateFactory_, *formationFactory_, *pObjectManager_, *populationFactory_, *inhabitantFactory_, *knowledgeGroupFactory_ ) )
    , flowCollisionManager_         ( new MIL_FlowCollisionManager() )
    , world_                        ( world )
    , pathfindComputer_             ( new PathfindComputer( pathfindManager, *world_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager constructor
// Created: MCO 2012-09-12
// -----------------------------------------------------------------------------
MIL_EntityManager::MIL_EntityManager( const MIL_Time_ABC& time, MIL_EffectManager& effects,
        const MIL_Config& config, const boost::shared_ptr< const TER_World >& world,
        DEC_PathFind_Manager& pathfindManager )
    : time_                         ( time )
    , gcPause_                      ( config.GetGarbageCollectorPause() )
    , gcMult_                       ( config.GetGarbageCollectorStepMul() )
    , effectManager_                ( effects )
    , bSendUnitVisionCones_         ( false )
    , bEnableRandomBreakdowns_      ( config.EnableRandomBreakdowns() )
    , profilerManager_              ( new MIL_ProfilerManager( config ) )
    , nRandomBreakdownsNextTimeStep_( 0  )
    , rKnowledgesTime_              ( 0 )
    , rAutomatesDecisionTime_       ( 0 )
    , rPionsDecisionTime_           ( 0 )
    , rPopulationsDecisionTime_     ( 0 )
    , rActionsTime_                 ( 0 )
    , rEffectsTime_                 ( 0 )
    , rStatesTime_                  ( 0 )
    , idManager_                    ( new MIL_IDManager() )
    , flowCollisionManager_         ( new MIL_FlowCollisionManager() ) // todo : delete if saved in checkpoint
    , world_                        ( world )
    , pathfindComputer_             ( new PathfindComputer( pathfindManager, *world_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager destructor
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_EntityManager::~MIL_EntityManager()
{
    // Types
    TerminateLogisticsSingletons();
    TerminateMilitarySingletons();
    TerminatePhysicalSingletons();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ReadODB
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::ReadODB( const MIL_Config& config )
{
    MT_LOG_STARTUP_MESSAGE( "-------------------------" );
    MT_LOG_STARTUP_MESSAGE( "----  Loading ODB    ----" );
    MT_LOG_STARTUP_MESSAGE( "-------------------------" );
    if( !bEnableRandomBreakdowns_ )
        MT_LOG_INFO_MSG( "Disabling random breakdowns" );

    const tools::Path orbatFile = config.GetOrbatFile();
    MT_LOG_INFO_MSG( "ODB file name : '" << orbatFile.ToUTF8() << "'" );
    config.GetLoader().LoadFile( orbatFile, boost::bind( &MIL_EntityManager::ReadOrbat, this, _1, boost::cref( config ) ) );

    MT_LOG_INFO_MSG( MT_FormatString( " => %d automates"  , automateFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d pions"      , sink_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d populations", populationFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d inhabitants", inhabitantFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d objects"    , pObjectManager_->Count() ) );

    // Check automate composition
    if( config.CheckAutomateComposition() )
        for( tools::Iterator< const MIL_Automate& > it = automateFactory_->CreateIterator(); it.HasMoreElements(); )
        {
            const MIL_Automate& automate = it.NextElement();
            if( !automate.CheckComposition() )
                throw MASA_EXCEPTION(
                    "The effective composition of the automate '" + boost::lexical_cast< std::string >( automate.GetID() )
                    + "' ('" + automate.GetName() + "') is not consistent with the composition described in the type '"
                    + automate.GetType().GetName() + "'" );
        }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ReadOrbat
// Created: LDC 2010-11-24
// -----------------------------------------------------------------------------
void MIL_EntityManager::ReadOrbat( xml::xistream& xis, const MIL_Config& config )
{
    xis >> xml::start( "orbat" );
    InitializeArmies   ( xis, config );
    InitializeDiplomacy( xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::LoadUrbanModel
// Created: JSR 2012-08-03
// -----------------------------------------------------------------------------
void MIL_EntityManager::LoadUrbanModel( const MIL_Config& config )
{
    // TODO déplacer dans une factory? ou dans MIL_ObjectManager
    try
    {
        bool oldUrbanMode = false;
        tools::Path fullPath = config.GetUrbanFile();
        bool urbanFound = fullPath.Exists();
        if( !urbanFound )
        {
            fullPath = config.GetTerrainUrbanFile();
            urbanFound = fullPath.Exists();
            oldUrbanMode = urbanFound;
        }
        if( urbanFound ) // avoid exception
        {
            MT_LOG_STARTUP_MESSAGE( "--------------------------------" );
            MT_LOG_STARTUP_MESSAGE( "----  Loading UrbanModel    ----" );
            MT_LOG_STARTUP_MESSAGE( "--------------------------------" );
            MT_LOG_INFO_MSG( "Loading Urban Model from path '" << ( oldUrbanMode ? config.GetTerrainFile() : config.GetExerciseFile() ).Parent().ToUTF8() << "'" );

            config.GetLoader().LoadFile( fullPath, boost::bind( &MIL_EntityManager::ReadUrban, this, _1, boost::ref( cities_ ) ) );
            if( cities_.empty() )
                return;

            geometry::Rectangle2d extent( geometry::Point2d( 0, 0 ), geometry::Point2d( config.GetTerrainWidth(), config.GetTerrainHeight() ) );
            MIL_AgentServer::GetWorkspace().GetUrbanCache().CreateQuadTree( cities_, extent );

            MT_LOG_INFO_MSG( MT_FormatString( "%d Urban blocs", MIL_AgentServer::GetWorkspace().GetUrbanCache().GetUrbanBlocks().size() ) );

            if( oldUrbanMode )
            {
                const tools::Path strUrbanState = config.GetUrbanStateFile();
                if( !strUrbanState.IsEmpty() && strUrbanState.Exists() )
                {
                    MT_LOG_INFO_MSG( "UrbanState file name : '" << strUrbanState.ToUTF8() << "'" );
                    config.GetLoader().LoadFile( strUrbanState, boost::bind( &MIL_EntityManager::ReadUrbanStates, this, _1 ) );
                }
            }
            NotifyPionsInsideUrbanObject();
        }
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception in loading Urban Model caught : " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ReadUrban
// Created: JSR 2012-08-03
// -----------------------------------------------------------------------------
void MIL_EntityManager::ReadUrban( xml::xistream& xis, std::vector< const MIL_UrbanObject_ABC* >& cities )
{
    xis >> xml::start( "urban" )
            >> xml::start( "urban-objects" )
                >> xml::list( "urban-object", *this, &MIL_EntityManager::ReadCity, cities )
            >> xml::end
        >> xml::end;

    // TODO faire dans un finalize?
    for( std::vector< const MIL_UrbanObject_ABC* >::const_iterator city = cities.begin(); city != cities.end(); ++city )
    {
        tools::Iterator< const MIL_UrbanObject_ABC& > districts = ( *city )->CreateIterator();
        while( districts.HasMoreElements() )
        {
            MIL_UrbanObject_ABC& district = const_cast< MIL_UrbanObject_ABC& >( districts.NextElement() );
            district.ComputeConvexHull();
        }
        const_cast< MIL_UrbanObject_ABC* >( *city )->ComputeConvexHull();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::WriteUrban
// Created: NPT 2012-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::WriteUrban( xml::xostream& xos ) const
{
    tools::SchemaWriter schemaWriter;
    xos << xml::start( "urban" );
    schemaWriter.WriteSchema( xos, "exercise", "urban" );
    xos     << xml::start( "urban-objects" );
        for( auto it = cities_.begin(); it != cities_.end(); ++it )
            ( *it )->WriteUrban( xos );
    xos     << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ReadCity
// Created: JSR 2012-08-03
// -----------------------------------------------------------------------------
void MIL_EntityManager::ReadCity( xml::xistream& xis, std::vector< const MIL_UrbanObject_ABC* >& cities )
{
    MIL_UrbanObject_ABC* urbanObject = static_cast< MIL_UrbanObject_ABC* >( pObjectManager_->CreateUrbanObject( xis, 0 ) );
    if( !urbanObject )
        return;
    cities.push_back( urbanObject );
    xis >> xml::optional >> xml::start( "urban-objects" )
            >> xml::list( "urban-object", *this, &MIL_EntityManager::ReadUrbanObject, *urbanObject )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ReadUrbanObject
// Created: JSR 2012-08-03
// -----------------------------------------------------------------------------
void MIL_EntityManager::ReadUrbanObject( xml::xistream& xis, MIL_UrbanObject_ABC& parent )
{
    MIL_UrbanObject_ABC* urbanObject = static_cast< MIL_UrbanObject_ABC* >( pObjectManager_->CreateUrbanObject( xis, &parent ) );
    if( !urbanObject )
        return;
    static_cast< tools::Resolver< MIL_UrbanObject_ABC >& >( parent ).Register( urbanObject->GetUrbanId(), *urbanObject );
    xis >> xml::optional >> xml::start( "urban-objects" )
        >> xml::list( "urban-object", *this, &MIL_EntityManager::ReadUrbanObject, *urbanObject )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ReadUrbanStates
// Created: JSR 2010-06-28
// -----------------------------------------------------------------------------
void MIL_EntityManager::ReadUrbanStates( xml::xistream& xis )
{
    xis >> xml::start( "urban-state" )
            >> xml::start( "urban-objects" )
                >> xml::list( "urban-object", boost::bind( &MIL_ObjectManager::ReadUrbanState, boost::ref( *pObjectManager_ ), _1 ) )
            >> xml::end
        >> xml::end;
}

namespace
{
    void NotifyInsideUrbanObject( MIL_Agent_ABC& agent )
    {
        agent.GetRole< PHY_RoleInterface_UrbanLocation >().MagicMove(
            agent.GetRole< PHY_RoleInterface_Location >().GetPosition() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::NotifyPionsInsideUrbanObject
// Created: SBO 2010-07-07
// -----------------------------------------------------------------------------
void MIL_EntityManager::NotifyPionsInsideUrbanObject()
{
    sink_->Apply( &NotifyInsideUrbanObject );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeDiplomacy
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::InitializeDiplomacy( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing diplomacy" );

    xis >> xml::start( "diplomacy" )
            >> xml::list( "party", *this, &MIL_EntityManager::ReadDiplomacy )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ReadDiplomacy
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void MIL_EntityManager::ReadDiplomacy( xml::xistream& xis )
{
    MIL_Army_ABC* pArmy = armyFactory_->Find( xis.attribute< unsigned long >( "id" ) );
    if( pArmy )
        pArmy->InitializeDiplomacy( xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::Finalize
// Created: LMT 2011-07-21
// -----------------------------------------------------------------------------
void MIL_EntityManager::Finalize()
{
    armyFactory_->Finalize();
    MIL_AgentServer::GetWorkspace().GetResourceNetworkModel().Finalize();
    inhabitantFactory_->Finalize();
    UpdateStates();
}

namespace
{
    class KnowledgesVisitor : public KnowledgesVisitor_ABC
    {
    public:
        KnowledgesVisitor()
            : agents_     ( 0 )
            , objects_    ( 0 )
            , populations_( 0 )
            , groups_     ( 0 )
        {
            // NOTHING
        }
        virtual void VisitKnowledgesAgent( std::size_t knowledges )
        {
            agents_ += knowledges;
        }
        virtual void VisitKnowledgesObject( std::size_t knowledges )
        {
            objects_ += knowledges;
        }
        virtual void VisitKnowledgesPopulation( std::size_t knowledges )
        {
            populations_ += knowledges;
        }
        virtual void VisitKnowledgesGroup( std::size_t knowledges )
        {
            groups_ += knowledges;
        }
        std::size_t Count()
        {
            return agents_ + objects_ + populations_;
        }
    public:
        std::size_t agents_;
        std::size_t objects_;
        std::size_t populations_;
        std::size_t groups_;
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::LogInfo
// Created: MCO 2012-11-12
// -----------------------------------------------------------------------------
void MIL_EntityManager::LogInfo()
{
    KnowledgesVisitor visitor;
    Accept( visitor );
    MT_LOG_INFO_MSG(
        pObjectManager_->Count() << " Objects - " << visitor.Count() << " Knowledges (" << visitor.agents_ << " agents, "
            << visitor.objects_ << " objects, " << visitor.populations_ << " populations) - " << visitor.groups_ << " Knowledge groups" );
    MT_LOG_INFO_MSG( sink_->Count() << " Agents - " << automateFactory_->Count() << " Automats - " << populationFactory_->Count() << " Crowds" );
    {
        std::stringstream s;
        s.setf( std::ios::fixed, std::ios::floatfield );
        s.precision( 3 );
        for( auto it = profilers_.begin(); it != profilers_.end(); ++it )
            s << it->first << " " << it->second << " ms ";
        MT_LOG_INFO_MSG( s.str() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeArmies
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::InitializeArmies( xml::xistream& xis, const MIL_Config& config )
{
    MT_LOG_INFO_MSG( "Initializing armies" );

    assert( armyFactory_->Count() == 0 );

    xis >> xml::start( "parties" )
        >> xml::list( boost::bind( &ArmyFactory_ABC::Create, boost::ref( *armyFactory_ ), _2, _3, boost::cref( config ) ) )
        >> xml::end;

    pObjectManager_->FinalizeObjects( *pFloodModel_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateAutomat
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_Automate& MIL_EntityManager::CreateAutomat( xml::xistream& xis, MIL_Entity_ABC& parent )
{
    return automateFactory_->Create( xis, parent );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateAutomat
// Created: LDC 2010-10-05
// -----------------------------------------------------------------------------
MIL_Automate& MIL_EntityManager::CreateAutomat( const MIL_AutomateType& type,
        unsigned int knowledgeGroup, const std::string& name,
        MIL_Entity_ABC& parent, unsigned int nCtx,
        const MIL_DictionaryExtensions& extensions )
{
    return automateFactory_->Create( type, knowledgeGroup, name, parent, nCtx, extensions );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreatePion
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_EntityManager::CreatePion( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis )
{
    MIL_AgentPion* pPion = sink_->Create( type, automate, xis, 0 );
    return *pPion;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreatePion
// Created: MMC 2011-05-27
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_EntityManager::CreatePion( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, const boost::optional< std::string >& name, unsigned int nCtx,
    const MIL_DictionaryExtensions* extensions )
{
    const auto& unitName = name ? *name : type.GetName();
    MIL_AgentPion* pPion = sink_->Create( type, automate, vPosition, unitName, 0 );
    if( !pPion )
        throw MASA_EXCEPTION( "Pion couldn't be created." );
    if( extensions )
        pPion->SetExtensions( *extensions );
    pPion->SendCreation ( nCtx );
    pPion->SendFullState( nCtx );
    pPion->SendKnowledge( nCtx );
    return *pPion;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateObject( xml::xistream& xis, MIL_Army_ABC* army )
{
    pObjectManager_->CreateObject( xis, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::CreateObject( MIL_Army_ABC* army, const std::string& type, const TER_Localisation* pLocalisation,
                                                 bool activated, unsigned int externalIdentifier, const std::string& name, double density )
{
    return pObjectManager_->CreateObject( army, type, pLocalisation, activated, externalIdentifier, name, density );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::CreateObject( const std::string& type, MIL_Army_ABC* army, const TER_Localisation& localisation )
{
    return pObjectManager_->CreateObject( type, army, localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: JSR 2011-10-26
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::CreateObject( const std::string& type, MIL_Army_ABC* army, const TER_Localisation& localisation, unsigned int id )
{
    return pObjectManager_->CreateObject( type, army, localisation, id );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::CreateObject( MIL_Army_ABC* army, const MIL_ObjectBuilder_ABC& builder )
{
    return pObjectManager_->CreateObject( army, builder );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::FindObject( unsigned int nID ) const
{
    return pObjectManager_->Find( nID );
}

namespace
{
    struct Profiler : boost::noncopyable
    {
        Profiler( double& time )
            : time_( time )
        {
            profiler_.Start();
        }
        ~Profiler()
        {
            time_ = profiler_.Stop();
        }
        double& time_;
        MT_Profiler profiler_;
    };
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateKnowledges()
{
    Profiler profiler( rKnowledgesTime_ );
    try
    {
        int currentTimeStep = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
        {
            Profiler profiler( profilers_[ "update knowledges" ] );
            armyFactory_->Apply( boost::bind( &MIL_Army_ABC::UpdateKnowledges, _1, currentTimeStep ) );
            populationFactory_->Apply( boost::bind( &MIL_Population::UpdateKnowledges, _1 ) );
            armyFactory_->Apply( boost::bind( &MIL_Army_ABC::CleanKnowledges, _1 ) );
            populationFactory_->Apply( boost::bind( &MIL_Population::CleanKnowledges, _1 ) );
        }
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error updating knowledges: " << tools::GetExceptionMsg( e ) );
    }
}

namespace
{
    template< typename T >
    void UpdateDecision( float duration, T& entity, MT_Profiler& profiler, MIL_ProfilerManager& profilerManager )
    {
        profiler.Start();
        entity.UpdateDecision( duration );
        profilerManager.NotifyDecisionUpdated( entity, profiler.Stop() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateDecisions
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateDecisions()
{
    float duration = static_cast< float >( MIL_Time_ABC::GetTime().GetTickDuration() );
    MT_Profiler decisionUpdateProfiler;
    {
        Profiler profiler( rAutomatesDecisionTime_ );
        automateFactory_->Apply( boost::bind( &UpdateDecision< MIL_Automate >, duration, _1, boost::ref( decisionUpdateProfiler ), boost::ref( *profilerManager_ ) ) );
    }
    {
        Profiler profiler( rPionsDecisionTime_ );
        sink_->Apply( boost::bind( &UpdateDecision< MIL_AgentPion >, duration, _1, boost::ref( decisionUpdateProfiler ), boost::ref( *profilerManager_ ) ) );
    }
    {
        Profiler profiler( rPopulationsDecisionTime_ );
        populationFactory_->Apply( boost::bind( &UpdateDecision< MIL_Population >, duration, _1, boost::ref( decisionUpdateProfiler ), boost::ref( *profilerManager_ ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateActions
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateActions()
{
    Profiler profiler( rActionsTime_ );
    formationFactory_->Apply( boost::bind( &MIL_Formation::UpdateActions, _1 ) );
    automateFactory_->Apply( boost::bind( &MIL_Automate::UpdateActions, _1 ) );
    sink_->Apply( boost::bind( &MIL_AgentPion::UpdateActions, _1 ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::UpdateActions, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateEffects
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateEffects()
{
    Profiler profiler( rEffectsTime_ );
    pObjectManager_->ProcessEvents();
    effectManager_.Update();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateStates
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateStates()
{
    Profiler profiler( rStatesTime_ );
    // !! Automate avant Pions (?? => LOG ??)
    automateFactory_->Apply( boost::bind( &MIL_Automate::UpdateState, _1 ) );
    sink_->Apply( boost::bind( &MIL_AgentPion::UpdateState, _1 ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::UpdateState, _1 ) );
    formationFactory_->Apply( boost::bind( &MIL_Formation::UpdateNetwork, _1 ) );
    automateFactory_->Apply( boost::bind( &MIL_Automate::UpdateNetwork, _1 ) );
    sink_->Apply( boost::bind( &MIL_AgentPion::UpdateNetwork, _1 ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::UpdateNetwork, _1 ) );
    pObjectManager_->UpdateStates( *pFloodModel_ );
    inhabitantFactory_->Apply( boost::bind( &MIL_Inhabitant::UpdateState, _1 ) ); // $$$$ LDC: Must be done after pObjectManager_ because otherwise objects are destroyed too early
    inhabitantFactory_->Apply( boost::bind( &MIL_Inhabitant::UpdateNetwork, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::PreprocessRandomBreakdowns
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::PreprocessRandomBreakdowns()
{
    const unsigned int nCurrentTimeStep = time_.GetCurrentTimeStep();
    if( nRandomBreakdownsNextTimeStep_ > nCurrentTimeStep )
        return;
    while( nRandomBreakdownsNextTimeStep_ <= nCurrentTimeStep )
        nRandomBreakdownsNextTimeStep_ += ( 3600 * 24 / time_.GetTickDuration() );
    sink_->Apply( boost::bind( &MIL_AgentPion::PreprocessRandomBreakdowns, _1, nRandomBreakdownsNextTimeStep_ ) );
    MT_LOG_INFO_MSG( "Breakdowns preprocessed" );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateKnowledgeGroups
// Created:  FHD 2009-12-18:
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateKnowledgeGroups()
{
    Profiler profiler( rStatesTime_ ); // $$$$ MCO 2012-11-28: again ?
    try
    {
        for( auto it = armyFactory_->CreateIterator(); it.HasMoreElements(); )
        {
            const auto& groups = it.NextElement().GetKnowledgeGroups();
            for( auto it = groups.begin(); it != groups.end(); ++it )
                it->second->UpdateKnowledgeGroup();
        }
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Error updating knowledge groups: " << tools::GetExceptionMsg( e ) );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateCrowdCollisions
// Created: JSR 2014-01-09
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateCrowdCollisions()
{
    flowCollisionManager_->Update();
    populationFactory_->Apply( boost::bind( &MIL_Population::UpdateCrowdCollisions, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::Update
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::Update()
{
    pObjectManager_->Clean();
    if( bEnableRandomBreakdowns_ )
        PreprocessRandomBreakdowns();
    UpdateKnowledges();
    UpdateCrowdCollisions();
    UpdateDecisions();
    UpdateActions();
    UpdateEffects();
    UpdateStates();
    UpdateKnowledgeGroups(); // LTO
    pathfindComputer_->Update();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::Clean
// Created: AGE 2004-11-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::Clean()
{
    pObjectManager_->Clean();
    sink_->Clean();
    automateFactory_->Apply( boost::bind( &MIL_Automate::Clean, _1 ) );
    formationFactory_->Apply( boost::bind( &MIL_Formation::Clean, _1 ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::Clean, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CleanDeletedAgentKnowledges
// Created: JSR 2013-02-21
// -----------------------------------------------------------------------------
void MIL_EntityManager::CleanDeletedAgentKnowledges()
{
    armyFactory_->Apply( boost::bind( &MIL_Army_ABC::CleanDeletedAgentKnowledges, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MIL_EntityManager::SendStateToNewClient() const
{
    armyFactory_->Apply( boost::bind( &MIL_Army_ABC::SendCreation, _1 ) );
    missionController_->SendFullState();
    armyFactory_->Apply( boost::bind( &MIL_Army_ABC::SendFullState, _1 ) );
    pObjectManager_->SendCreation();
    pObjectManager_->SendFullState();
    armyFactory_->Apply( boost::bind( &MIL_Army_ABC::SendKnowledge, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveUnitOrder
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveUnitOrder( const UnitOrder& message, unsigned int nCtx, unsigned int clientId )
{
    client::TaskCreationRequestAck ack;
    ack().mutable_tasker()->mutable_unit()->set_id( message.tasker().id() );
    ack().set_error_code( OrderAck::no_error );
    try
    {
        MIL_AgentPion* pPion = FindAgentPion( message.tasker().id() );
        if( !pPion )
            throw MASA_BADUNIT_ORDER( "invalid unit: " << message.tasker().id() );
        ack().set_id( pPion->OnReceiveOrder( message ) );
    }
    catch( const NET_AsnException< OrderAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
        ack().set_error_msg( tools::GetExceptionMsg( e ) );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveAutomatOrder
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveAutomatOrder( const AutomatOrder& message, unsigned int nCtx, unsigned int clientId )
{
    client::TaskCreationRequestAck ack;
    ack().mutable_tasker()->mutable_automat()->set_id( message.tasker().id() );
    ack().set_error_code( OrderAck::no_error );
    try
    {
        MIL_Automate* pAutomate = FindAutomate( message.tasker().id() );
        if( !pAutomate )
            throw MASA_BADUNIT_ORDER( "invalid automat: " << message.tasker().id() );
        ack().set_id( pAutomate->OnReceiveOrder( message ) );
    }
    catch( const NET_AsnException< OrderAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
        ack().set_error_msg( tools::GetExceptionMsg( e ) );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveUnitMagicAction
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveUnitMagicAction( const UnitMagicAction& message, unsigned int nCtx, unsigned int clientId )
{
    client::UnitMagicActionAck ack;
    auto& magics = MIL_AgentServer::GetWorkspace().GetMagicOrderManager();
    const auto magicId = magics.Register( message );
    ack().set_id( magicId );
    const auto tasker = protocol::TryGetTasker( message.tasker() );
    if( !tasker )
    {
        ack().mutable_unit()->set_id( 0 );
        ack().set_error_code( UnitActionAck::error_invalid_unit );
        ack.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
        magics.Send( magicId, ack().error_code(), "missing tasker" );
        return;
    }
    const auto id = *tasker;

    ack().mutable_unit()->set_id( id );
    ack().set_error_code( UnitActionAck::no_error );
    ack().set_error_msg( "" );
    ack().mutable_parameters()->CopyFrom( message.parameters() );
    ack().mutable_tasker()->CopyFrom( message.tasker() );
    ack().set_type( message.type() );
    ack().set_name( message.name() );

    try
    {
        const auto& params = message.parameters();
        switch( message.type() )
        {
        case UnitMagicAction::move_to :
            ProcessMagicActionMoveTo( message, id );
            break;
        case UnitMagicAction::unit_creation :
            if( MIL_Automate* pAutomate = FindAutomate( id ) )
            {
                unsigned int unitId = pAutomate->OnReceiveUnitCreationRequest(
                        message, nCtx );
                ack().mutable_result()->add_elem()->add_value()->mutable_agent()
                    ->set_id( unitId );
            }
            else
                throw MASA_BADUNIT_UNIT( "invalid automat: " << id );
            break;
        case UnitMagicAction::create_fire_order:
            ProcessMagicActionCreateFireOrder( message );
            break;
        case UnitMagicAction::change_knowledge_group:
            ProcessAutomateChangeKnowledgeGroup( message, nCtx );
            break;
        case UnitMagicAction::change_logistic_links:
            ProcessChangeLogisticLinks( message );
            break;
        case UnitMagicAction::unit_change_superior:
            ProcessUnitChangeSuperior( message, id );
            break;
        case UnitMagicAction::change_automat_superior:
        case UnitMagicAction::change_formation_superior:
            if( message.tasker().has_automat() )
                ProcessAutomateChangeSuperior( message, nCtx );
            else if( message.tasker().has_formation() )
                ProcessFormationChangeSuperior( message, nCtx );
            else
                throw MASA_BADUNIT_UNIT( "invalid formation or automat: " << id );
            break;
        case UnitMagicAction::log_supply_push_flow:
            ProcessLogSupplyPushFlow( message, ack() );
            break;
        case UnitMagicAction::log_supply_pull_flow:
            ProcessLogSupplyPullFlow( message, ack() );
            break;
        case UnitMagicAction::log_supply_change_quotas:
            ProcessLogSupplyChangeQuotas( message );
            break;
        case UnitMagicAction::automat_creation:
            if( MIL_Automate* pAutomate = FindAutomate( id ) )
                ProcessAutomatCreationRequest( message, *pAutomate, nCtx, ack() );
            else if( MIL_Formation* pFormation = FindFormation( id ) )
                ProcessAutomatCreationRequest( message, *pFormation, nCtx, ack() );
            else
                throw MASA_BADUNIT_UNIT( "invalid formation or automat: " << id );
            break;
        case UnitMagicAction::automat_and_units_creation:
            if( MIL_Automate* pAutomate = FindAutomate( id ) )
                ProcessAutomatAndUnitsCreationRequest( message, *pAutomate, nCtx, ack() );
            else if( MIL_Formation* pFormation = FindFormation( id ) )
                ProcessAutomatAndUnitsCreationRequest( message, *pFormation, nCtx, ack() );
            else
                throw MASA_BADUNIT_UNIT( "invalid formation or automat: " << id );
            break;
        case UnitMagicAction::formation_creation :
            ProcessFormationCreationRequest( params, id, nCtx, ack() );
            break;
        case UnitMagicAction::crowd_creation:
            try
            {
                ProcessCrowdCreationRequest( message, id, nCtx, ack() );
            }
            catch( const NET_AsnException< UnitActionAck::ErrorCode >& )
            {
                // Old behaviour, some UnitMagicAction used to return
                // MagicActionAck, no idea why.
                client::MagicActionAck ack;
                ack().set_error_code( MagicActionAck::error_invalid_parameter );
                ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
                throw;
            }
            break;
        case UnitMagicAction::transfer_equipment:
            if( MIL_AgentPion* pPion = FindAgentPion( id ) )
                ProcessTransferEquipmentRequest( message, *pPion );
            else
                throw MASA_BADUNIT_UNIT( "invalid unit: " << id );
            break;
        case UnitMagicAction::exec_script:
            ProcessExecScript( message, id, ack() );
            break;
        default:
            if( MIL_Formation* pFormation = FindFormation( id ) )
                pFormation->OnReceiveUnitMagicAction( message );
            else if( MIL_Automate* pAutomate = FindAutomate( id ) )
                pAutomate->OnReceiveUnitMagicAction( message, *armyFactory_ );
            else if( MIL_AgentPion* pPion = FindAgentPion( id ) )
                pPion->OnReceiveUnitMagicAction( message, *armyFactory_, nCtx, clientId );
            else if( MIL_Population* pPopulation = FindPopulation( id ) )
                pPopulation->OnReceiveUnitMagicAction( message );
            else if( MIL_Inhabitant* pInhabitant = FindInhabitant( id ) )
                pInhabitant->OnReceiveUnitMagicAction( message );
            else
                throw MASA_BADUNIT_UNIT( "invalid entity: " << id );
            break;
        }
    }
    catch( const NET_AsnBadParam< UnitActionAck::ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
        ack().set_error_msg( e.what() );
    }
    catch( const NET_AsnException< UnitActionAck::ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
        ack().set_error_msg( e.what() );
    }
    catch( const std::exception& e )
    {
        ack().set_error_code( UnitActionAck::error_invalid_parameter );
        ack().set_error_msg( tools::GetExceptionMsg( e ) );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
    magics.Send( magicId, ack().error_code(), ack().error_msg() );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessAutomatCreationRequest
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessAutomatCreationRequest( const UnitMagicAction& msg, MIL_Entity_ABC& entity, unsigned int nCtx, sword::UnitMagicActionAck& ack )
{
    const auto& params = msg.parameters();
    const int count = protocol::GetCount( params );
    protocol::Check( count > 1, "at least 2 parameters expected" );
    const auto automatType = protocol::GetIdentifier( params, 0 );
    const MIL_AutomateType* pType = MIL_AutomateType::FindAutomateType( automatType );
    if( !pType )
        throw MASA_BADPARAM_UNIT( "invalid automat type: " << automatType );
    unsigned int groupId = 0;
    if( const auto group = protocol::TryGetKnowledgeGroup( params, 1 ) )
        groupId = *group;
    else
        groupId = protocol::GetIdentifier( params, 1 );

    auto group = entity.GetArmy().FindKnowledgeGroup( groupId );
    if( !group || group->IsJammed() )
        throw MASA_BADPARAM_UNIT( "knowledge group is invalid or jammed" );

    std::string name;
    if( count > 2 )
        name = protocol::GetString( params, 2 );

    MIL_DictionaryExtensions extensions;
    if( count > 3 )
        extensions.ReadExtensions( msg.parameters().elem( 3 ).value(0).extensionlist() );

    // auto-registration
    const MIL_Automate& a = MIL_AgentServer::GetWorkspace().GetEntityManager()
        .CreateAutomat( *pType, groupId, name, entity, nCtx, extensions );
    ack.mutable_result()->add_elem()->add_value()->mutable_automat()
        ->set_id( a.GetID() );
}

namespace
{
    unsigned int GetSensibleNumber( unsigned int nMin, unsigned int nMax )
    {
        if( nMax == nMin )
            return std::max( nMin, 1u );
        if( nMax != std::numeric_limits< unsigned int >::max() )
            return std::max( ( nMax + nMin ) / 2u, 1u );
        return std::max( nMin, 1u );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessAutomatAndUnitsCreationRequest
// Created: LGY 2014-01-20
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessAutomatAndUnitsCreationRequest( const UnitMagicAction& msg, MIL_Entity_ABC& entity,
                                                               unsigned int nCtx, sword::UnitMagicActionAck& ack )
{
    const auto& params = msg.parameters();
    protocol::CheckCount( params, 3 );
    const auto automatType = protocol::GetIdentifier( params, 0 );
    const MIL_AutomateType* pType = MIL_AutomateType::FindAutomateType( automatType );
    if( !pType )
        throw MASA_BADPARAM_UNIT( "invalid automat type: " << automatType );

    const auto& point = protocol::GetPoint( params, 1 );
    MT_Vector2D position;
    world_->MosToSimMgrsCoord( point.latitude(), point.longitude(), position );

    unsigned int groupId = 0;
    if( const auto group = protocol::TryGetKnowledgeGroup( params, 1 ) )
        groupId = *group;
    else
        groupId = protocol::GetIdentifier( params, 2 );

    auto group = entity.GetArmy().FindKnowledgeGroup( groupId );
    if( !group || group->IsJammed() )
        throw MASA_BADPARAM_UNIT( "knowledge group is invalid or jammed" );

    MIL_DictionaryExtensions extensions;
    MIL_Automate& automat = CreateAutomat( *pType, groupId, "", entity, nCtx, extensions );

    unsigned int numberOfAgents = 0;
    const auto& compositions = pType->GetComposition();
    for( auto it = compositions.begin(); it != compositions.end(); ++it )
    {
        const MIL_AutomateType::sCompositionBounds& bounds = it->second;
        numberOfAgents += GetSensibleNumber( bounds.nMin_, bounds.nMax_ );
    }

    ack.mutable_result()->add_elem()->add_value()->mutable_automat()
        ->set_id( automat.GetID() );

    static const float pi = std::acos( -1.f );
    float angle = numberOfAgents > 1 ? 2 * pi / ( numberOfAgents - 1 ) : 0;
    unsigned int current = 0;
    std::vector< unsigned int > unitIds;
    for( size_t index = 0; index < compositions.size(); ++index )
    {
        const auto content = compositions.at( index );
        if( const MIL_AgentTypePion* agentType = dynamic_cast< const MIL_AgentTypePion* >( content.first ) )
        {
            const MIL_AutomateType::sCompositionBounds& bounds = content.second;
            unsigned int number = GetSensibleNumber( bounds.nMin_, bounds.nMax_ );
            MT_Vector2D newPosition = position;
            for( unsigned int i = 0; i < number; ++i )
            {
                if( agentType->GetID() != pType->GetTypePionPC().GetID() )
                {
                    newPosition = MT_Vector2D( 100.f * std::sin( current * angle ), 100.f * std::cos( current * angle ) ) + position;
                    current++;
                }
                MIL_AgentPion& pion = CreatePion( *agentType, automat, newPosition, 0, nCtx, 0 );
                ack.mutable_result()->add_elem()->add_value()->mutable_agent()
                    ->set_id( pion.GetID() );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessFormationCreationRequest
// Created: LDC 2010-10-20
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessFormationCreationRequest( const sword::MissionParameters& params,
    unsigned int taskerId, unsigned int nCtx, sword::UnitMagicActionAck& ack )
{
    MIL_Army_ABC* army = armyFactory_->Find( taskerId );
    MIL_Formation* formation = 0;
    if( !army )
    {
        formation = FindFormation( taskerId );
        if( !formation )
            throw MASA_BADUNIT_UNIT( "invalid party or formation: " << taskerId );
        army = &formation->GetArmy();
    }
    const int count = protocol::CheckCount( params, 2, 4 );
    const int level = static_cast< int >( protocol::GetReal( params, 0 ));
    const std::string& name = protocol::GetString( params, 1 );
    std::string logLevel = "";
    if( count > 2 && !protocol::IsNull( params, 2 ) )
        logLevel = protocol::GetString( params, 2 );
    std::vector< protocol::Extension > extensions;
    if( count > 3 )
        extensions = protocol::GetExtensionList( params, 3 );

    auto& newFormation = formationFactory_->Create( level, name, logLevel, *army, formation );
    if( !extensions.empty() )
        newFormation.SetExtensions( extensions );

    // This MagicActionAck is a leftover of previous protocol iteration, we
    // should get rid of it in future versions.
    client::MagicActionAck oldAck;
    oldAck().set_error_code( MagicActionAck::no_error );
    oldAck.Send( NET_Publisher_ABC::Publisher(), nCtx );

    ack.mutable_result()->add_elem()->add_value()->mutable_formation()
        ->set_id( newFormation.GetID() );
    newFormation.SendCreation( nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessCrowdCreationRequest
// Created: LDC 2010-10-22
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessCrowdCreationRequest( const UnitMagicAction& message,
        unsigned int parentId, unsigned int context, sword::UnitMagicActionAck& ack )
{
    MIL_Army_ABC* parent = 0;
    if( MIL_Formation* formation = FindFormation( parentId ) )
        parent = &formation->GetArmy();
    else if( MIL_Army_ABC* army = armyFactory_->Find( parentId ) )
        parent = army;
    else
        throw MASA_BADUNIT_UNIT( "invalid formation or army: " << parentId );

    if( !message.has_parameters() || message.parameters().elem_size() != 6 )
        throw MASA_BADPARAM_UNIT( "invalid parameters count, 6 parameters expected" );
    if( message.parameters().elem( 0 ).value_size() != 1 ||
            !message.parameters().elem( 0 ).value( 0 ).has_acharstr() )
        throw MASA_BADPARAM_UNIT( "parameters[0] must be an ACharStr" );
    if( message.parameters().elem( 1 ).value_size() != 1 ||
            !message.parameters().elem( 1 ).value( 0 ).has_point() )
        throw MASA_BADPARAM_UNIT( "parameters[1] must be a Point" );
    if( message.parameters().elem( 2 ).value_size() != 1 ||
            !message.parameters().elem( 2 ).value( 0 ).has_quantity() )
        throw MASA_BADPARAM_UNIT( "parameters[2] must be a Quantity" );
    if( message.parameters().elem( 3 ).value_size() != 1
            || !message.parameters().elem( 3 ).value( 0 ).has_quantity() )
        throw MASA_BADPARAM_UNIT( "parameters[3] must be a Quantity" );
    if( message.parameters().elem( 4 ).value_size() != 1 ||
            !message.parameters().elem( 4 ).value( 0 ).has_quantity() )
        throw MASA_BADPARAM_UNIT( "parameters[4] must be a Quantity" );
    if( message.parameters().elem( 5 ).value_size() != 1 ||
            !message.parameters().elem( 5 ).value( 0 ).has_acharstr() )
        throw MASA_BADPARAM_UNIT( "parameters[5] must be an ACharStr" );

    const ::MissionParameters& parameters = message.parameters();
    std::string type = parameters.elem( 0 ).value( 0 ).acharstr();
    ::Location location = parameters.elem( 1 ).value( 0 ).point().location();
    if( !location.has_coordinates() )
        throw MASA_BADPARAM_UNIT( "invalid crowd location" );

    const unsigned int healthy = parameters.elem( 2 ).value( 0 ).quantity();
    const unsigned int wounded = parameters.elem( 3 ).value( 0 ).quantity();
    const unsigned int dead = parameters.elem( 4 ).value( 0 ).quantity();
    MT_Vector2D point;
    const auto& position = location.coordinates().elem( 0 );
    world_->MosToSimMgrsCoord( position.latitude(), position.longitude(), point );
    int number = healthy + wounded + dead;
    if( number == 0 )
        throw MASA_BADPARAM_UNIT( "crowd cannot be created empty" );

    std::string name = parameters.elem( 5 ).value( 0 ).acharstr();
    MIL_Population& popu = populationFactory_->Create( type, point, number, name, *parent, 0, context );
    popu.ChangeComposition( healthy, wounded, 0, dead );

    client::MagicActionAck oldAck;
    oldAck().set_error_code( MagicActionAck::no_error );
    oldAck.Send( NET_Publisher_ABC::Publisher(), context );

    ack.mutable_result()->add_elem()->add_value()->mutable_crowd()->set_id( popu.GetID() );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveKnowledgeMagicAction
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveKnowledgeMagicAction( const KnowledgeMagicAction& message, unsigned int nCtx, unsigned int clientId )
{
    auto& magics = MIL_AgentServer::GetWorkspace().GetMagicOrderManager();
    const auto magicId = magics.Register( message );
    client::KnowledgeGroupMagicActionAck ack;
    ack().set_id( magicId );
    ack().mutable_knowledge_group()->set_id( message.knowledge_group().id() );
    ack().set_error_code( KnowledgeGroupAck::no_error );
    try
    {
        auto pReceiver = FindKnowledgeGroup( message.knowledge_group().id() );
        if( !pReceiver )
            throw MASA_BADGROUP_KNOWLEDGE( "unknown knowledge group" );
        pReceiver->OnReceiveKnowledgeGroupUpdate( message, ack(), *armyFactory_ );
    }
    catch( const NET_AsnException< sword::KnowledgeGroupAck::ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
        ack().set_error_msg( e.what() );
    }
    catch( const std::exception& e )
    {
        ack().set_error_code( sword::KnowledgeGroupAck::error_invalid_parameter );
        ack().set_error_msg( e.what() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
    magics.Send( magicId, ack().error_code(), ack().error_msg() );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveCrowdOrder
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveCrowdOrder( const CrowdOrder& message, unsigned int nCtx, unsigned int clientId )
{
    client::TaskCreationRequestAck ack;
    ack().mutable_tasker()->mutable_crowd()->set_id( message.tasker().id() );
    ack().set_error_code( OrderAck::no_error );
    try
    {
        MIL_Population* pPopulation = populationFactory_->Find( message.tasker().id() );
        if( !pPopulation )
            throw MASA_BADUNIT_ORDER( "invalid crowd: " << message.tasker().id() );
        ack().set_id( pPopulation->OnReceiveOrder( message ) );
    }
    catch( const NET_AsnException< OrderAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
        ack().set_error_msg( tools::GetExceptionMsg( e ) );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveFragOrder
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveFragOrder( const FragOrder& message, unsigned int nCtx, unsigned int clientId )
{
    client::FragOrderAck ack;
    ack().set_error_code( OrderAck::no_error );
    try
    {
        const auto taskerId = protocol::TryGetTasker( message.tasker() );
        if( !taskerId )
            throw MASA_INVALIDTASKER;
        bool sentAck = false;
        auto sendAck = [&]( uint32_t id ) {
            ack().set_id( id );
            ack.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
            sentAck = true;
        };
        if( MIL_Automate* pAutomate = FindAutomate( *taskerId ) )
        {
            ack().mutable_tasker()->mutable_automat()->set_id( *taskerId );
            pAutomate->OnReceiveFragOrder( message, sendAck );
        }
        else if( MIL_Population* pPopulation = FindPopulation( *taskerId ) )
        {
            ack().mutable_tasker()->mutable_crowd()->set_id( *taskerId );
            pPopulation->OnReceiveFragOrder( message, sendAck );
        }
        else if( MIL_AgentPion* pPion = FindAgentPion( *taskerId ) )
        {
            ack().mutable_tasker()->mutable_unit()->set_id( *taskerId );
            pPion->OnReceiveFragOrder( message, sendAck );
        }
        else
            throw MASA_BADUNIT_ORDER( "invalid automat, crowd or unit: " << *taskerId );
        if( sentAck )
            return;
    }
    catch( const NET_AsnException< OrderAck::ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
        ack().set_error_msg( tools::GetExceptionMsg( e ) );
    }
    catch( const NET_InvalidTasker& )
    {
        ack().set_error_code( OrderAck::error_invalid_unit );
    }
    catch( const std::exception& e )
    {
        ack().set_error_code( OrderAck::error_invalid_parameter );
        ack().set_error_msg( tools::GetExceptionMsg( e ) );
    }
    if( !ack().has_tasker() )
        ack().mutable_tasker()->mutable_unit()->set_id( 0 );
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveSetAutomateMode
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveSetAutomateMode( const SetAutomatMode& message, unsigned int nCtx, unsigned int clientId )
{
    auto& magics = MIL_AgentServer::GetWorkspace().GetMagicOrderManager();
    const auto magicId = magics.Register( message );
    client::SetAutomatModeAck ack;
    ack().set_id( magicId );
    ack().mutable_automate()->set_id( message.automate().id() );
    ack().set_error_code( SetAutomatModeAck::no_error );
    try
    {
        MIL_Automate* pAutomate = FindAutomate( message.automate().id() );
        if( !pAutomate )
            throw MASA_BADPARAM_ASN( SetAutomatModeAck::ErrorCode, SetAutomatModeAck::error_invalid_unit, STR( "invalid automat: " << message.automate().id() ));
        pAutomate->OnReceiveSetAutomateMode( message );
    }
    catch( const NET_AsnException< SetAutomatModeAck::ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
    magics.Send( magicId, ack().error_code(), "" );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveUnitCreationRequest
// Created: AGE 2007-06-18
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveUnitCreationRequest( const UnitCreationRequest& message, unsigned int nCtx )
{
    client::UnitCreationRequestAck ack;
    ack().set_error_code( UnitActionAck::no_error );
    try
    {
        MIL_Automate* pAutomate = FindAutomate( message.superior().id() );
        if( !pAutomate )
            throw MASA_BADUNIT_UNIT( "invalid unit: " << message.superior().id() );
        pAutomate->OnReceiveUnitCreationRequest( message, nCtx );
    }
    catch( const NET_AsnException< UnitActionAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveObjectMagicAction
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveObjectMagicAction( const ObjectMagicAction& message, unsigned int nCtx, unsigned int clientId )
{
    auto& magics = MIL_AgentServer::GetWorkspace().GetMagicOrderManager();
    const auto magicId = magics.Register( message );
    client::ObjectMagicActionAck ack;
    ack().set_id( magicId );
    pObjectManager_->OnReceiveObjectMagicAction( message, ack(), *armyFactory_, *pFloodModel_ );
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
    magics.Send( magicId, ack().error_code(), ack().error_msg() );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveChangeDiplomacy
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
E_Diplomacy GetDiplomacy( sword::EnumDiplomacy e )
{
    switch( e )
    {
        case sword::friendly: return eDiplomacy_Ami;
        case sword::enemy: return eDiplomacy_Ennemi;
        case sword::neutral: return eDiplomacy_Neutre;
    }
    return eDiplomacy_Inconnu;
}

void MIL_EntityManager::OnReceiveChangeDiplomacy( const sword::MagicAction& message, unsigned int nCtx )
{
    client::ChangeDiplomacyAck ack;
    client::ChangeDiplomacy changeDiplomacyMes;
    const auto& params = message.parameters();
    protocol::CheckCount( params, 3 );
    const uint32_t party1 = parameters::GetPartyId( params, 0 );
    const uint32_t party2 = parameters::GetPartyId( params, 1 );
    const sword::EnumDiplomacy value = GET_ENUMERATION( sword::EnumDiplomacy, params, 2 );

    MIL_Army_ABC* pArmy1 = armyFactory_->Find( party1 );
    protocol::Check( pArmy1, STR( "invalid party indentifier " << party1 ) );
    MIL_Army_ABC* pArmy2 = armyFactory_->Find( party2 );
    protocol::Check( pArmy2, STR( "invalid party indentifier " << party2 ) );
    pArmy1->ChangeDiplomacy( *pArmy2, GetDiplomacy( value ) );

    changeDiplomacyMes().mutable_party1()->set_id( party1 );
    changeDiplomacyMes().mutable_party2()->set_id( party2 );
    changeDiplomacyMes().set_diplomacy( value );
    changeDiplomacyMes.Send( NET_Publisher_ABC::Publisher(), nCtx );
    ack().mutable_party1()->set_id( party1 );
    ack().mutable_party2()->set_id( party2 );
    ack().set_diplomacy( value );
    ack().set_error_code( ChangeDiplomacyAck::no_error_diplomacy );
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveChangeResourceLinks
// Created: JSR 2010-08-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveChangeResourceLinks( const MagicAction& message )
{
    pObjectManager_->OnReceiveChangeResourceLinks( message );
}

namespace
{
    bool IsOfType( const PHY_ComposantePion& composante, unsigned int id )
    {
        return composante.GetType().GetMosID().id() == id;
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessTransferEquipmentRequest
// Created: ABR 2011-06-15
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessTransferEquipmentRequest( const sword::UnitMagicAction& message, MIL_AgentPion& pion )
{
    const MissionParameters& params = message.parameters();
    protocol::CheckCount( params, 2 );
    MIL_AgentPion* target = FindAgentPion( protocol::GetIdentifier( params, 0 ));
    if( !target )
        throw MASA_BADPARAM_UNIT( "invalid target identifier" );
    const int count = protocol::GetCount( params, 1 );
    protocol::Check( count > 0, "invalid empty equipment list" );
    if( target == &pion )
        throw MASA_BADPARAM_UNIT( "source and target are identical" );
    PHY_RolePion_Composantes& source = pion.GetRole< PHY_RolePion_Composantes >();
    std::map< unsigned int, int > composantes;
    for( int i = 0; i < count; ++i )
    {
        const uint32_t id = protocol::GetIdentifier( params, 1, i, 0 );
        const int quantity = protocol::GetQuantity( params, 1, i, 1 );
        if( quantity < 0 )
            throw MASA_BADPARAM_UNIT( "invalid negative equipment count #" << i );
        if( quantity == 0 )
            continue;
        if( ! source.CanLendComposantes( boost::bind( &IsOfType, _1, id ) ) )
            throw MASA_BADPARAM_UNIT( "no equipment type of parameter #" << i
                    << " available to lend in source unit" );
        composantes[ id ] += quantity;
    }
    for( auto it = composantes.begin(); it != composantes.end(); ++it )
        source.LendComposantes( *target, it->second, boost::bind( &IsOfType, _1, it->first ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessAutomateChangeKnowledgeGroup
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessAutomateChangeKnowledgeGroup( const UnitMagicAction& message, unsigned int nCtx )
{
    MIL_Automate* pAutomate = TaskerToAutomat( *this, message.tasker() );
    if( !pAutomate )
        throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, "invalid automat" );
    pAutomate->OnReceiveChangeKnowledgeGroup( message );
    client::AutomatChangeKnowledgeGroup resendMessage;
    resendMessage().mutable_automat()->set_id( pAutomate->GetID() );
    resendMessage().mutable_party()->set_id( pAutomate->GetArmy().GetID() );
    resendMessage().mutable_knowledge_group()->set_id( pAutomate->GetKnowledgeGroup()->GetId() );
    resendMessage.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessChangeLogisticLinks
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessChangeLogisticLinks( const UnitMagicAction& message )
{
    const boost::optional< uint32_t > tasker = protocol::TryGetTasker( message.tasker() );
    logistic::LogisticHierarchy_ABC* pSubordinate = TaskerToLogisticHierarchy( *this, message.tasker() );
    if( !pSubordinate )
        throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, "invalid subordinate automate" );

    std::vector< MIL_AutomateLOG* > superiors;
    for( int i = 0; i < message.parameters().elem_size(); ++i )
    {
        const sword::MissionParameter& parameterSuperior = message.parameters().elem( i );
        if( !parameterSuperior.null_value() )
        {
            MIL_AutomateLOG* pSuperior = FindBrainLogistic( parameterSuperior.value( 0 ) );
            if( !pSuperior )
                throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, "invalid supply automat" );
            if( tasker && *tasker == pSuperior->GetLogisticId() )
                throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, "cannot set itself as its own logistic superior" );
            superiors.push_back( pSuperior );
        }
    }
    pSubordinate->ChangeLinks( superiors );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessAutomateChangeSuperior
// Created: NLD 2007-04-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessAutomateChangeSuperior( const UnitMagicAction& message, unsigned int nCtx )
{
    const auto& params = message.parameters();
    MIL_Automate* pAutomate = TaskerToAutomat( *this, message.tasker() );
    if( !pAutomate )
        throw MASA_BADPARAM_UNIT( "invalid automat" );

    client::AutomatChangeSuperior resendMessage;
    resendMessage().mutable_automat()->set_id( message.tasker().automat().id() );
    if( const auto formationId = protocol::TryGetFormationId( params, 0 ) )
    {
        MIL_Formation* pFormation = FindFormation( *formationId );
        if( !pFormation )
            throw MASA_BADPARAM_UNIT( "invalid new parent formation" );
        resendMessage().mutable_superior()->mutable_formation()->set_id( *formationId );
    }
    else
    {
        const uint32_t automatId = protocol::GetAutomatId( params, 0 );
        MIL_Automate* pAutomat = FindAutomate( automatId );
        if( !pAutomat )
            throw MASA_BADPARAM_UNIT( "invalid new parent automat" );
        resendMessage().mutable_superior()->mutable_automat()->set_id( automatId );
    }
    pAutomate->OnReceiveChangeSuperior( message, *formationFactory_ );
    resendMessage.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessUnitChangeSuperior
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessUnitChangeSuperior( const UnitMagicAction& message, unsigned int taskerId )
{
    MIL_AgentPion* pPion = FindAgentPion( taskerId );
    if( !pPion )
        throw MASA_BADUNIT_UNIT( "invalid unit: " << taskerId );
    const auto& params = message.parameters();
    protocol::CheckCount( params, 1 );
    pPion->OnReceiveChangeSuperior( *this, protocol::GetAutomatId( params, 0 ));
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessLogSupplyChangeQuotas
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessLogSupplyChangeQuotas( const UnitMagicAction& message )
{
    if( message.parameters().elem_size() != 2 )
        throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, "invalid receiver" );

    // Supplied
    logistic::LogisticHierarchy_ABC* pSupplied = TaskerToLogisticHierarchy( *this, message.tasker() );
    if( !pSupplied )
        throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, "invalid receiver" );

    // Param 0: supplier
    MIL_AutomateLOG* pSupplier = FindBrainLogistic( message.parameters().elem( 0 ).value( 0 ) );
    if( !pSupplier )
        throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, "invalid supplier" );

    // Param 1: quotas
    std::set< const PHY_DotationCategory* > quotasTypes;
    const sword::MissionParameter& quotas = message.parameters().elem( 1 );
    const boost::shared_ptr< logistic::LogisticLink_ABC > superiorLink = pSupplied->FindSuperiorLink( *pSupplier );
    if( !superiorLink )
        throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, "invalid receiver superior logistic link" );

    quotasTypes = superiorLink->OnReceiveChangeQuotas( quotas );
    if( quotasTypes.empty() )
        throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, "invalid dotation" );

    std::set< MIL_Automate* > unavailableSuppliers;
    for( std::set< const PHY_DotationCategory* >::iterator typeIt = quotasTypes.begin(); typeIt != quotasTypes.end(); ++typeIt )
    {
        bool bDeployed = false;
        MIL_Automate* pStockAutomat = pSupplier->GetStockAutomat( **typeIt, bDeployed );
        if( pStockAutomat && !bDeployed )
            unavailableSuppliers.insert( pStockAutomat );
    }
    for( std::set< MIL_Automate* >::iterator supplierIt = unavailableSuppliers.begin(); supplierIt != unavailableSuppliers.end(); ++supplierIt )
        MIL_Report::PostEvent( **supplierIt, report::eRC_SupplierUnavailable );
}

namespace
{
    template< typename Transporters >
    void CheckSuppliesCanBeLoaded( const Transporters& transporters,
        const tools::Map< const PHY_DotationCategory*, double >& supplies )
    {
        for( auto it = supplies.begin(); it != supplies.end(); ++it )
        {
            bool found = false;
            for( auto it2 = transporters.begin(); it2 != transporters.end() && !found; ++it2 )
            {
                const PHY_ComposanteTypePion* type = PHY_ComposanteTypePion::Find( it2->equipmenttype().id() );
                protocol::Check( type, "invalid transporter" );
                if( type->CanTransportStock( *it->first ) )
                    found = true;
            }
            protocol::Check( found, "not all supplies can be loaded by transporters" );
        }
    }

    double ComputeMaxMass( const PHY_ComposanteTypePion& type,
        const tools::Map< const PHY_ComposanteTypePion*, unsigned >& transporters )
    {
        double mass = 0;
        for( auto it = transporters.begin(); it != transporters.end(); ++it )
            if( type.GetStockTransporterNature() == it->first->GetStockTransporterNature() )
            {
                double maxMass, maxVolume;
                it->first->GetStockTransporterCapacity( maxMass, maxVolume );
                mass += it->second * maxMass;
            }
        return mass;
    }

    std::pair< double, double > ComputeMassVolume( const PHY_ComposanteTypePion& type,
        const tools::Map< const PHY_ComposanteTypePion*, unsigned >& transporters,
        const tools::Map< const PHY_DotationCategory*, double >& supplies )
    {
        double mass = 0;
        double volume = 0;
        for( auto it = supplies.begin(); it != supplies.end(); ++it )
            if( type.CanTransportStock( *it->first ) )
            {
                mass += it->second * it->first->GetWeight();
                volume += it->second * it->first->GetVolume();
            }
        const double total = ComputeMaxMass( type, transporters );
        double maxMass, maxVolume;
        type.GetStockTransporterCapacity( maxMass, maxVolume );
        return std::make_pair( mass / total, volume * maxMass / total / maxVolume );
    }

    template< typename Transporters >
    void CheckTransportersLoad( const Transporters& transporters,
        const tools::Map< const PHY_DotationCategory*, double >& supplies,
        sword::MissionParameter_Value& states )
    {
        tools::Map< const PHY_ComposanteTypePion*, unsigned > result;
        for( auto it = transporters.begin(); it != transporters.end(); ++it )
        {
            const PHY_ComposanteTypePion* type = PHY_ComposanteTypePion::Find( it->equipmenttype().id() );
            protocol::Check( type, "invalid transporter" );
            const unsigned quantity = it->quantity();
            protocol::Check( quantity > 0, "transporter quantity must be positive" );
            result[ type ] += quantity;
        }
        bool massOverloaded = false;
        bool volumeOverloaded = false;
        for( auto it = result.begin(); it != result.end(); ++it )
        {
            double mass, volume, minMass, maxMass, minVolume, maxVolume;
            it->first->GetStockTransporterCapacity( maxMass, maxVolume );
            std::tie( minMass, minVolume ) = it->first->GetStockTransporterCapacity();
            std::tie( mass, volume ) = ComputeMassVolume( *it->first, result, supplies );
            states.add_list()->set_booleanvalue( mass * maxMass < minMass );
            if( mass > 1 )
                massOverloaded = true;
            states.add_list()->set_booleanvalue( mass > 1 );
            states.add_list()->set_booleanvalue( volume * maxVolume < minVolume );
            if( volume > 1 )
                volumeOverloaded = true;
            states.add_list()->set_booleanvalue( volume > 1 );
        }
        protocol::Check( !massOverloaded, "transporter capacity mass overloaded" );
        protocol::Check( !volumeOverloaded, "transporter capacity volume overloaded" );
    }

    template< typename Resources >
    void ReadResources( const Resources& resources, tools::Map< const PHY_DotationCategory*, double >& supplies )
    {
        protocol::Check( resources.size() > 0, "at least one resource expected" );
        for( auto it2 = resources.begin(); it2 != resources.end(); ++it2 )
        {
            const PHY_DotationCategory* category = PHY_DotationType::FindDotationCategory( it2->resourcetype().id() );
            protocol::Check( category, "invalid resource" );
            const double quantity = it2->quantity();
            protocol::Check( quantity > 0, "resource quantity must be positive" );
            supplies[ category ] += quantity;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessLogSupplyPushFlow
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessLogSupplyPushFlow( const UnitMagicAction& message, sword::UnitMagicActionAck& ack )
{
    const auto tasker = protocol::TryGetTasker( message.tasker() );
    if( !tasker )
        throw MASA_INVALIDTASKER;
    MIL_AutomateLOG* pBrainLog = FindBrainLogistic( *tasker );
    protocol::Check( pBrainLog, "invalid supplier" );
    protocol::CheckCount( message.parameters(), 1 );
    const auto& value = message.parameters().elem( 0 ).value( 0 );
    protocol::Check( value.has_push_flow_parameters(), "invalid parameters" );
    const auto& parameters = value.push_flow_parameters();
    tools::Map< const PHY_DotationCategory*, double > supplies;
    protocol::Check( parameters.recipients().size() > 0, "at least one recipient expected" );
    for( auto it = parameters.recipients().begin(); it != parameters.recipients().end(); ++it )
        ReadResources( it->resources(), supplies );
    auto states = ack.mutable_result()->add_elem()->add_value();
    const auto& transporters = parameters.transporters();
    if( transporters.size() )
    {
        CheckSuppliesCanBeLoaded( transporters, supplies );
        CheckTransportersLoad( transporters, supplies, *states );
    }
    if( !pBrainLog->OnReceiveLogSupplyPushFlow( parameters, *automateFactory_ ) )
        throw MASA_EXCEPTION( "unable to create push flow request" );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessLogSupplyPullFlow
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessLogSupplyPullFlow( const UnitMagicAction& message, sword::UnitMagicActionAck& ack )
{
    MIL_Automate* pAutomate = TaskerToAutomat( *this, message.tasker() );
    protocol::Check( pAutomate, "invalid receiver" );
    protocol::CheckCount( message.parameters(), 1 );
    const auto& value = message.parameters().elem( 0 ).value( 0 );
    protocol::Check( value.has_pull_flow_parameters(), "invalid parameters" );
    const auto& parameters = value.pull_flow_parameters();
    MIL_AutomateLOG* supplier = FindBrainLogistic( parameters.supplier() );
    protocol::Check( supplier, "invalid supplier" );
    tools::Map< const PHY_DotationCategory*, double > supplies;
    ReadResources( parameters.resources(), supplies );
    auto states = ack.mutable_result()->add_elem()->add_value();
    const auto& transporters = parameters.transporters();
    if( transporters.size() )
    {
        CheckSuppliesCanBeLoaded( transporters, supplies );
        CheckTransportersLoad( transporters, supplies, *states );
    }
    if( !pAutomate->OnReceiveLogSupplyPullFlow( parameters, *supplier ) )
        throw MASA_EXCEPTION( "unable to create pull flow request" );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessMagicActionMoveTo
// Created: JSR 2010-04-07
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessMagicActionMoveTo( const UnitMagicAction& message, unsigned int taskerId )
{
    if( MIL_Automate* pAutomate = FindAutomate( taskerId) )
        return pAutomate->OnReceiveMagicActionMoveTo( message );
    if( MIL_AgentPion* pPion = FindAgentPion( taskerId ) )
        return pPion->OnReceiveMagicActionMoveTo( message );
    if( MIL_Population* pPopulation = populationFactory_->Find( taskerId ))
        return pPopulation->OnReceiveCrowdMagicActionMoveTo( message.parameters() );
    throw MASA_BADUNIT_UNIT( "invalid automa, crowd or unit: " << taskerId );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveKnowledgeGroupCreation
// Created: FHD 2009-12-15
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveKnowledgeGroupCreation( const MagicAction& message, sword::MagicActionAck& ack )
{
    const auto& params = message.parameters();
    protocol::CheckCount( params, 2 );
    const std::string& typeStr = protocol::GetString( params, 1 );
    const auto* type = MIL_KnowledgeGroupType::FindType( typeStr );
    if( !type )
        throw MASA_BADPARAM_MAGICACTION( "invalid knowledge group type: " << typeStr );
    const uint32_t parentId = protocol::GetIdentifier( params, 0 );
    boost::shared_ptr< MIL_KnowledgeGroup > parent = FindKnowledgeGroup( parentId );
    boost::shared_ptr< MIL_KnowledgeGroup > group;
    if( parent )
    {
        group.reset( new MIL_KnowledgeGroup( *type, *parent ) );
        parent->RegisterKnowledgeGroup( group );
    }
    else
    {
        MIL_Army_ABC* army = armyFactory_->Find( parentId );
        if( ! army )
            throw MASA_BADPARAM_MAGICACTION( "invalid party identifier: " << parentId );
        group.reset( new MIL_KnowledgeGroup( *type, *army, false ) );
        army->RegisterKnowledgeGroup( group );
    }
    if( const DEC_KnowledgeBlackBoard_KnowledgeGroup* blackboard = group->GetKnowledge() )
    {
        const MIL_Army_ABC::T_Objects& objects = group->GetArmy().GetObjects();
        for( auto it = objects.begin(); it != objects.end(); ++it )
            blackboard->GetKsObjectKnowledgeSynthetizer().AddEphemeralObjectKnowledge( *it->second );
    }
    ack.mutable_result()->add_elem()->add_value()->set_identifier( group->GetId() );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnPathfindRequest
// Created: LGY 2014-02-28
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnPathfindRequest( const sword::PathfindRequest& message, unsigned int nCtx, unsigned int clientId )
{
    const unsigned int id = message.unit().id();
    const auto& positions = message.positions();
    protocol::Check( positions.size() > 1, "must have at least two points" );
    if( MIL_AgentPion* pPion = FindAgentPion( id ) )
        pathfindComputer_->Compute( *pPion, message, nCtx, clientId, false );
    else if( MIL_Population* pPopulation = FindPopulation( id ) )
        pathfindComputer_->Compute( *pPopulation, message, nCtx, clientId, false );
    else
    {
        client::ComputePathfindAck ack;
        ack().set_error_code( ComputePathfindAck::error_invalid_parameter );
        ack().set_error_msg( "invalid crowd or unit identifier" );
        ack.Send( NET_Publisher_ABC::Publisher(), nCtx, clientId );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceivePathfindCreation
// Created: LGY 2014-04-15
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceivePathfindCreation( const sword::MagicAction& message, sword::MagicActionAck& ack,
                                                   unsigned int nCtx, unsigned int clientId )
{
    const auto& params = message.parameters();
    protocol::CheckCount( params, 1 );
    const auto& value = message.parameters().elem( 0 ).value( 0 );
    protocol::Check( value.has_pathfind_request(), "invalid parameters" );
    const auto& request = value.pathfind_request();
    const auto& positions = request.positions();
    protocol::Check( positions.size() > 1, "must have at least two points" );
    const unsigned int id = request.unit().id();
    unsigned long result = 0;
    if( MIL_AgentPion* pPion = FindAgentPion( id ) )
        result = pathfindComputer_->Compute( *pPion, request, nCtx, clientId, true );
    else if( MIL_Population* pPopulation = FindPopulation( id ) )
        result = pathfindComputer_->Compute( *pPopulation, request, nCtx, clientId, true );
    else
    {
        ack.set_error_code( MagicActionAck::error_invalid_parameter );
        ack.set_error_msg( "invalid crowd or unit identifier" );
    }
    if( ack.error_code() == sword::ControlAck::no_error )
        ack.mutable_result()->add_elem()->add_value()->mutable_pathfind()->set_id( result );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceivePathfindDestruction
// Created: LGY 2014-04-15
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceivePathfindDestruction( const sword::MagicAction& message, sword::MagicActionAck& ack )
{
    const auto& params = message.parameters();
    protocol::CheckCount( params, 1 );
    const auto& value = message.parameters().elem( 0 ).value( 0 );
    protocol::Check( value.has_pathfind(), "invalid parameter" );
    const auto id = value.pathfind().id();
    if( !pathfindComputer_->Destroy( id ) )
    {
        ack.set_error_code( MagicActionAck::error_invalid_parameter );
        ack.set_error_msg( "invalid pathfind identifier");
    }
    ack.mutable_result()->add_elem()->add_value()->mutable_pathfind()->set_id( id );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessMagicActionCreateFireOrder
// Created: MGD 2010-02-24
// LTO
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessMagicActionCreateFireOrder( const UnitMagicAction& msg )
{
    if( !msg.has_parameters() || msg.parameters().elem_size() != 3 )
        throw MASA_BADPARAM_UNIT( "invalid parameters count, 3 parameters expected" );

    MIL_Agent_ABC* reporter = msg.tasker().has_unit() && msg.tasker().unit().has_id() ? FindAgentPion( msg.tasker().unit().id() ) : 0;
    if( !reporter )
        throw MASA_BADPARAM_ASN( UnitActionAck_ErrorCode, UnitActionAck::error_invalid_unit, "invalid tasker, the receiver must be a valid unit" );

    const MissionParameter& target = msg.parameters().elem( 0 );
    if( target.value_size() != 1 || !target.value(0).has_identifier() )
        throw MASA_BADPARAM_UNIT( "parameters[0] must be an identifier" );

    boost::shared_ptr< DEC_Knowledge_Agent > targetKn = reporter->GetKnowledge().ResolveKnowledgeAgent( target.value(0).identifier() );
    if( !targetKn )
        throw MASA_BADPARAM_UNIT( "parameters[0] must be a valid knowledge identifier in reporter's knowledge group" );

    const MissionParameter& ammo = msg.parameters().elem( 1 );
    if( ammo.value_size() != 1 || !ammo.value(0).has_resourcetype() )
        throw MASA_BADPARAM_UNIT( "parameters[1] must be a resource type" );

    const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( ammo.value(0).resourcetype().id() );
    if( !pDotationCategory || !pDotationCategory->CanBeUsedForIndirectFire() )
        throw MASA_BADPARAM_UNIT( "parameters[1] must be a dotation category "
                                  "identifier that can be used for indirect fire" );

    if( pDotationCategory->IsGuided() && !targetKn->GetAgentKnown().GetRole< PHY_RoleInterface_Illumination >().IsIlluminated() )
        throw MASA_BADPARAM_UNIT( "parameters[1] is a guided dotation but the target is not illuminated" );

    const MissionParameter& iterations = msg.parameters().elem( 2 );
    if( iterations.value_size() != 1 || !iterations.value(0).has_areal() )
        throw MASA_BADPARAM_UNIT( "parameters[2] must be a real" );

    const float value = iterations.value( 0 ).areal();
    if( value <= 0.f )
        throw MASA_BADPARAM_UNIT( "parameters[2] must be a positive real number" );

    PHY_FireResults_Pion fireResult( *reporter , targetKn->GetPosition(), *pDotationCategory );
    const unsigned int ammos = static_cast< unsigned int >( pDotationCategory->ConvertToNbrAmmo( value ) );

    MIL_Report::PostEvent( *reporter, report::eRC_TirIndirectSurCible, targetKn );

    pDotationCategory->ApplyStrikeEffect( *reporter, targetKn->GetAgentKnown(), ammos, fireResult );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveCreateFireOrderOnLocation
// Created: ABR 2011-01-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveCreateFireOrderOnLocation( const MagicAction& msg )
{
    const auto& params = msg.parameters();
    protocol::CheckCount( params, 3 );
    sword::CoordLatLong point;
    try
    {
        const auto points = protocol::GetLocation( params, 0 );
        protocol::Check( points.size() == 1, "must be a point location", 0 );
        point = points[0];
    }
    catch( const protocol::Exception& )
    {
        point = protocol::GetPoint( params, 0 );
    }

    const uint32_t resourceType = protocol::GetResourceType( params, 1 );
    const auto* pDotationCategory = PHY_DotationType::FindDotationCategory( resourceType );
    protocol::Check( pDotationCategory,
        "must be a valid dotation category identifier", 1 );
    protocol::Check( pDotationCategory->CanBeUsedForIndirectFire(),
        "must be a dotation category identifier that can be used for indirect fire", 1 );

    const float iterations = protocol::GetReal( params, 2 );
    protocol::Check( iterations >= 0, "must be a positive real number", 2 );
    const uint32_t ammos = static_cast< uint32_t >(
            pDotationCategory->ConvertToNbrAmmo( iterations ) );

    MT_Vector2D targetPos;
    world_->MosToSimMgrsCoord( point.latitude(), point.longitude(), targetPos );
    PHY_FireResults_Default fireResult;
    pDotationCategory->ApplyIndirectFireEffect( targetPos, targetPos, ammos, fireResult );
}

namespace
{
    template< typename Agents, typename Func >
    void ApplyOnRequest( const Agents& agents, uint32_t id, const Func& f )
    {
        PHY_MaintenanceComposanteState* request = 0;
        agents.Apply( [&]( const MIL_AgentPion& p )
        {
            if( !request )
                request = p.GetRole< PHY_RoleInterface_Composantes >().FindRequest( id );
        } );
        if( !request )
            throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, "invalid log request identifier" );
        f( *request );
    }
}

void MIL_EntityManager::OnReceiveSelectNewLogisticState( const sword::MagicAction& msg )
{
    const auto& params = msg.parameters();
    protocol::CheckCount( params, 1 );
    const auto id = protocol::GetIdentifier( params, 0 );
    ApplyOnRequest( *sink_, id, []( PHY_MaintenanceComposanteState& request )
    {
        request.SelectNewState();
    } );
}

void MIL_EntityManager::OnReceiveTransferToLogisticSuperior( const sword::MagicAction& msg )
{
    const auto& params = msg.parameters();
    protocol::CheckCount( params, 1 );
    const auto id = protocol::GetIdentifier( params, 0 );
    ApplyOnRequest( *sink_, id, []( PHY_MaintenanceComposanteState& request )
    {
        request.TransferToLogisticSuperior();
    } );
}

void MIL_EntityManager::OnReceiveSelectDiagnosisTeam( const sword::MagicAction& message )
{
    const auto& params = message.parameters();
    protocol::CheckCount( params, 2 );
    const auto requestId = protocol::GetIdentifier( params, 0 );
    const auto equipment = PHY_ComposanteTypePion::Find( protocol::GetIdentifier( params, 1 ) );
    protocol::Check( equipment, "invalid equipment type identifier" );
    ApplyOnRequest( *sink_, requestId, [&]( PHY_MaintenanceComposanteState& request )
    {
        request.SelectDiagnosisTeam( *equipment );
    } );
}

void MIL_EntityManager::OnReceiveSelectRepairTeam( const sword::MagicAction& message )
{
    const auto& params = message.parameters();
    protocol::CheckCount( params, 2 );
    const auto requestId = protocol::GetIdentifier( params, 0 );
    const auto equipment = PHY_ComposanteTypePion::Find( protocol::GetIdentifier( params, 1 ) );
    protocol::Check( equipment, "invalid equipment type identifier" );
    ApplyOnRequest( *sink_, requestId, [&]( PHY_MaintenanceComposanteState& request )
    {
        request.SelectRepairTeam( *equipment );
    } );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveSelectMaintenanceTransporter
// Created: SLI 2014-01-30
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveSelectMaintenanceTransporter( const sword::MagicAction& message )
{
    const auto& params = message.parameters();
    const unsigned int count = protocol::CheckCount( params, 2, 3 );
    const auto requestId = protocol::GetIdentifier( params, 0 );
    const auto equipment = PHY_ComposanteTypePion::Find( protocol::GetIdentifier( params, 1 ) );
    protocol::Check( equipment, "invalid equipment type identifier" );
    boost::optional< const MIL_Agent_ABC& > destination;
    if( count > 2 )
    {
        const MIL_Agent_ABC* agent = FindAgentPion( protocol::GetAgentId( params, 2 ) );
        protocol::Check( agent, "invalid destination agent identifier" );
        destination = *agent;
    }
    ApplyOnRequest( *sink_, requestId, [&]( PHY_MaintenanceComposanteState& request )
    {
        request.SelectMaintenanceTransporter( *equipment, destination );
    } );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveBurningCellRequest
// Created: BCI 2011-03-01
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveBurningCellRequest( const BurningCellRequest& message, unsigned int nCtx )
{
    client::BurningCellRequestAck ack;
    ack().set_error_code( BurningCellRequestAck::no_error );
    try
    {
        MIL_Object_ABC* object = FindObject( message.object().id() );
        if( object )
        {
            BurnSurfaceAttribute* burnSurfaceAttribute = object->RetrieveAttribute< BurnSurfaceAttribute >();
            if( burnSurfaceAttribute )
                burnSurfaceAttribute->OnReceiveBurningCellRequest( message );
        }
    }
    catch( const NET_AsnException< BurningCellRequestAck::ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ChannelPopulations
// Created: SBO 2006-01-16
// -----------------------------------------------------------------------------
void MIL_EntityManager::ChannelPopulations( const TER_Localisation& localisation )
{
    populationFactory_->Apply( boost::bind( &MIL_Population::NotifyChanneled, _1, localisation ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::AlertInhabitants
// Created: BCI 2011-02-01
// -----------------------------------------------------------------------------
void MIL_EntityManager::AlertInhabitants( const TER_Localisation& localisation )
{
    inhabitantFactory_->Apply( boost::bind( &MIL_Inhabitant::NotifyAlerted, _1, localisation ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::AlertInhabitants
// Created: CCD 2012-01-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::UndoAlertInhabitants( const TER_Localisation& localisation )
{
    inhabitantFactory_->Apply( boost::bind( &MIL_Inhabitant::NotifyUndoAlerted, _1, localisation ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ConfineInhabitants
// Created: BCI 2011-02-18
// -----------------------------------------------------------------------------
void MIL_EntityManager::ConfineInhabitants( const TER_Localisation& localisation )
{
    inhabitantFactory_->Apply( boost::bind( &MIL_Inhabitant::NotifyConfined, _1, localisation ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::EvacuateInhabitants
// Created: CCD 2012-01-13
// -----------------------------------------------------------------------------
void MIL_EntityManager::EvacuateInhabitants( const TER_Localisation& localisation )
{
    inhabitantFactory_->Apply( boost::bind( &MIL_Inhabitant::NotifyEvacuated, _1, localisation ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UndoEvacuateInhabitants
// Created: CCD 2012-01-18
// -----------------------------------------------------------------------------
void MIL_EntityManager::UndoEvacuateInhabitants( const TER_Localisation& localisation )
{
    inhabitantFactory_->Apply( boost::bind( &MIL_Inhabitant::NotifyUndoEvacuated, _1, localisation ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UndoConfineInhabitants
// Created: CCD 2012-01-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::UndoConfineInhabitants( const TER_Localisation& localisation )
{
    inhabitantFactory_->Apply( boost::bind( &MIL_Inhabitant::NotifyUndoConfined, _1, localisation ) );
}

struct IsInhabitantsAlertedFunctor : boost::noncopyable
{
    IsInhabitantsAlertedFunctor( const TER_Localisation& localisation )
        : nbAlerted_   ( 0 )
        , total_       ( 0 )
        , localisation_( localisation )
    {
        // NOTHING
    }
    void operator()( const MIL_Inhabitant& inhabitant ) const
    {
        if( inhabitant.IsAlerted( localisation_ ) )
            ++nbAlerted_;
        ++total_;
    }
    mutable int nbAlerted_;
    mutable int total_;
    const TER_Localisation& localisation_;
};

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::IsInhabitantsAlerted
// Created: BCI 2011-02-18
// -----------------------------------------------------------------------------
bool MIL_EntityManager::IsInhabitantsAlerted( const TER_Localisation& localisation )
{
    IsInhabitantsAlertedFunctor functor( localisation );
    inhabitantFactory_->Apply( functor );
    return functor.nbAlerted_ > 0 && functor.total_ > 0;
}

struct IsInhabitantsConfinedFunctor : boost::noncopyable
{
    IsInhabitantsConfinedFunctor( const TER_Localisation& localisation )
        : nbConfined_   ( 0 )
        , total_       ( 0 )
        , localisation_( localisation )
    {
        // NOTHING
    }
    void operator()( const MIL_Inhabitant& inhabitant ) const
    {
        if( inhabitant.IsConfined( localisation_ ) )
            ++nbConfined_;
        ++total_;
    }
    mutable int nbConfined_;
    mutable int total_;
    const TER_Localisation& localisation_;
};

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::IsInhabitantsConfined
// Created: CCD 2012-01-10
// -----------------------------------------------------------------------------
bool MIL_EntityManager::IsInhabitantsConfined( const TER_Localisation& localisation )
{
    IsInhabitantsConfinedFunctor functor( localisation );
    inhabitantFactory_->Apply( functor );
    return functor.nbConfined_ > 0 && functor.total_ > 0;
}

struct IsInhabitantsEvacuatedFunctor : boost::noncopyable
{
    IsInhabitantsEvacuatedFunctor( const TER_Localisation& localisation )
        : nbEvacuated_ ( 0 )
        , total_       ( 0 )
        , localisation_( localisation )
    {
        // NOTHING
    }
    void operator()( const MIL_Inhabitant& inhabitant ) const
    {
        if( inhabitant.IsEvacuated( localisation_ ) )
            ++nbEvacuated_;
        ++total_;
    }
    mutable int nbEvacuated_;
    mutable int total_;
    const TER_Localisation& localisation_;
};

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::IsInhabitantsEvacuated
// Created: CCD 2012-01-13
// -----------------------------------------------------------------------------
bool MIL_EntityManager::IsInhabitantsEvacuated( const TER_Localisation& localisation )
{
    IsInhabitantsEvacuatedFunctor functor( localisation );
    inhabitantFactory_->Apply( functor );
    return functor.nbEvacuated_ > 0 && functor.total_ > 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file //>> effectManager_  // Effets liés aux actions qui ne sont pas sauvegardés
         >> knowledgeGroupFactory_;
    file >> armyFactory_
         >> formationFactory_//@TODO MGD serialize
         >> automateFactory_
         >> populationFactory_
         >> inhabitantFactory_
         >> pObjectManager_
         >> missionController_
         >> sink_
         >> rKnowledgesTime_
         >> rAutomatesDecisionTime_
         >> rPionsDecisionTime_
         >> rPopulationsDecisionTime_
         >> rActionsTime_
         >> rEffectsTime_
         >> rStatesTime_
         >> nRandomBreakdownsNextTimeStep_
         >> cities_
         >> MIL_Report::nextMessageId_;
    auto& wk = MIL_AgentServer::GetWorkspace();
    wk.GetUrbanCache().CreateQuadTree(
        cities_,
        geometry::Rectangle2d(
            geometry::Point2d( 0, 0 ),
            geometry::Point2d( wk.GetConfig().GetTerrainWidth(), wk.GetConfig().GetTerrainHeight() ) ) );
    pFloodModel_.reset( sink_->CreateFloodModel().release() );
    pObjectManager_->FinalizeObjects( *pFloodModel_ );
    missionController_->Initialize( *sink_, *populationFactory_ );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d automates"  , automateFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d pions"      , sink_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d populations", populationFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d inhabitants", inhabitantFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d objects"    , pObjectManager_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d urbans"     , wk.GetUrbanCache().GetUrbanBlocks().size() ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
         //<< effectManager_  // Effets liés aux actions qui ne sont pas sauvegardés
    file << knowledgeGroupFactory_; // LTO
    file << armyFactory_
         << formationFactory_
         << automateFactory_
         << populationFactory_
         << inhabitantFactory_
         << pObjectManager_
         << missionController_
         << sink_
         << rKnowledgesTime_
         << rAutomatesDecisionTime_
         << rPionsDecisionTime_
         << rPopulationsDecisionTime_
         << rActionsTime_
         << rEffectsTime_
         << rStatesTime_
         << nRandomBreakdownsNextTimeStep_
         << cities_
         << MIL_Report::nextMessageId_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_EntityManager::WriteODB( xml::xostream& xos ) const
{
    tools::SchemaWriter schemaWriter;
    xos << xml::start( "orbat" );
    schemaWriter.WriteSchema( xos, "exercise", "orbat" );
    xos     << xml::start( "parties" );
                armyFactory_->Apply( boost::bind( &MIL_Army_ABC::WriteODB, _1, boost::ref( xos ) ) );
                pObjectManager_->WriteODB( xos );
    xos     << xml::end
            << xml::start( "diplomacy" );
                armyFactory_->Apply( boost::bind( &MIL_Army_ABC::WriteDiplomacyODB, _1, boost::ref( xos ) ) );
    xos     << xml::end
        << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::WriteKnowledges
// Created: NPT 2012-08-08
// -----------------------------------------------------------------------------
void MIL_EntityManager::WriteKnowledges( xml::xostream& xos ) const
{
    tools::SchemaWriter schemaWriter;
    xos << xml::start( "knowledges" );
        schemaWriter.WriteSchema( xos, "exercise", "knowledges" );
        armyFactory_->Apply( boost::bind( &MIL_Army_ABC::WriteKnowledges, _1, boost::ref( xos ) ) );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindAutomate
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
MIL_Automate* MIL_EntityManager::FindAutomate( unsigned int nID ) const
{
    return automateFactory_->Find( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindBrainLogistic
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
MIL_AutomateLOG* MIL_EntityManager::FindBrainLogistic( unsigned int nID ) const
{
    if( MIL_Formation* pFormation = FindFormation( nID ) )
        return pFormation->GetBrainLogistic();
    if( MIL_Automate* pAutomate = FindAutomate( nID ) )
        return pAutomate->GetBrainLogistic();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindBrainLogistic
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
MIL_AutomateLOG* MIL_EntityManager::FindBrainLogistic( const sword::ParentEntity& parameter ) const
{
    if( parameter.has_automat() )
        return FindBrainLogistic( parameter.automat().id() );
    else if( parameter.has_formation() )
        return FindBrainLogistic( parameter.formation().id() );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindBrainLogistic
// Created: NLD 2011-01-10
// -----------------------------------------------------------------------------
MIL_AutomateLOG* MIL_EntityManager::FindBrainLogistic( const sword::MissionParameter_Value& parameter ) const
{
    if( parameter.has_identifier() )
        return FindBrainLogistic( parameter.identifier() );
    else if( parameter.has_automat() )
        return FindBrainLogistic( parameter.automat().id() );
    else if( parameter.has_formation() )
        return FindBrainLogistic( parameter.formation().id() );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindPopulation
// Created: RPD 2010-07-18
// -----------------------------------------------------------------------------
MIL_Population* MIL_EntityManager::FindPopulation( unsigned int nID ) const
{
    return populationFactory_->Find( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindInhabitant
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
MIL_Inhabitant* MIL_EntityManager::FindInhabitant( unsigned int nID ) const
{
    return inhabitantFactory_->Find( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindFormation
// Created: RPD 2010-07-18
// -----------------------------------------------------------------------------
MIL_Formation* MIL_EntityManager::FindFormation( unsigned int nID ) const
{
    return formationFactory_->Find( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindKnowledgeGroup
// Created: SLG 2009-12-17
// -----------------------------------------------------------------------------
boost::shared_ptr< MIL_KnowledgeGroup > MIL_EntityManager::FindKnowledgeGroup( unsigned int nID ) const
{
    for( auto it = armyFactory_->CreateIterator(); it.HasMoreElements(); )
        if( boost::shared_ptr< MIL_KnowledgeGroup > group = it.NextElement().FindKnowledgeGroup( nID ) )
            return group;
    return boost::shared_ptr< MIL_KnowledgeGroup >();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindAgentPion
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_EntityManager::FindAgentPion( unsigned int nID ) const
{
    return sink_->Find( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetArmies
// Created: NLD 2004-09-01 //@TODO MGD just use on time in object to destroy knowledge, find a way to remove this function
// -----------------------------------------------------------------------------
const tools::Resolver< MIL_Army_ABC >& MIL_EntityManager::GetArmies() const
{
    return *armyFactory_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ConvertUrbanIdToSimId
// Created: JSR 2011-01-18
// -----------------------------------------------------------------------------
unsigned int MIL_EntityManager::ConvertUrbanIdToSimId( unsigned int urbanId ) const
{
    for( auto it = cities_.begin(); it != cities_.end(); ++it )
    {
        const MIL_UrbanObject_ABC* city = *it;
        if( city->GetUrbanId() == urbanId )
            return city->GetID();
        tools::Iterator< const MIL_UrbanObject_ABC& > districts = city->CreateIterator();
        while( districts.HasMoreElements() )
        {
            const MIL_UrbanObject_ABC& district = districts.NextElement();
            if( district.GetUrbanId() == urbanId )
                return district.GetID();
            tools::Iterator< const MIL_UrbanObject_ABC& > blocks = district.CreateIterator();
            while( blocks.HasMoreElements() )
            {
                const MIL_UrbanObject_ABC& block = blocks.NextElement();
                if( block.GetUrbanId() == urbanId )
                    return block.GetID();
            }
        }
    }
    MT_LOG_WARNING_MSG( "Cannot find urban object with id " << urbanId );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ConvertIdToUrbanId
// Created: NPT 2012-09-12
// -----------------------------------------------------------------------------
bool MIL_EntityManager::ConvertIdToUrbanId( unsigned int& id ) const
{
    MIL_UrbanObject_ABC* urban = dynamic_cast< MIL_UrbanObject_ABC* >( FindObject( id ) );
    if( urban )
    {
        id = urban->GetUrbanId();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetFlowCollisionManager
// Created: JSR 2014-01-09
// -----------------------------------------------------------------------------
MIL_FlowCollisionManager& MIL_EntityManager::GetFlowCollisionManager() const
{
    return *flowCollisionManager_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetKnowledgesTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
double MIL_EntityManager::GetKnowledgesTime() const
{
    return rKnowledgesTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetDecisionsTime
// Created: NLD 2006-05-03
// -----------------------------------------------------------------------------
double MIL_EntityManager::GetDecisionsTime() const
{
    return rAutomatesDecisionTime_ + rPionsDecisionTime_ + rPopulationsDecisionTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetAutomatesDecisionTime
// Created: NLD 2006-05-03
// -----------------------------------------------------------------------------
double MIL_EntityManager::GetAutomatesDecisionTime() const
{
    return rAutomatesDecisionTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetPionsDecisionTime
// Created: NLD 2006-05-03
// -----------------------------------------------------------------------------
double MIL_EntityManager::GetPionsDecisionTime() const
{
    return rPionsDecisionTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetPopulationsDecisionTime
// Created: NLD 2006-05-03
// -----------------------------------------------------------------------------
double MIL_EntityManager::GetPopulationsDecisionTime() const
{
    return rPopulationsDecisionTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetActionsTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
double MIL_EntityManager::GetActionsTime() const
{
    return rActionsTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetEffectsTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
double MIL_EntityManager::GetEffectsTime() const
{
    return rEffectsTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetStatesTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
double MIL_EntityManager::GetStatesTime() const
{
    return rStatesTime_;
}

// -----------------------------------------------------------------------------
// Name: Model::setToTasker
// Created: PHC 2010-07-07
// -----------------------------------------------------------------------------
void MIL_EntityManager::SetToTasker( Tasker& tasker, unsigned int id ) const
{
    if( FindAutomate( id ) )
        tasker.mutable_automat()->set_id( id );
    else if( FindFormation( id ) )
        tasker.mutable_formation()->set_id( id );
    else if( FindPopulation( id ) )
        tasker.mutable_crowd()->set_id( id );
    else if( FindAgentPion( id ) )
        tasker.mutable_unit()->set_id( id );
    /*else if( FindInhabitant( id ) )
        tasker.mutable_population()->set_id( id );*/
    else throw MASA_EXCEPTION( "Misformed tasker in protocol message." );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateCrowd
// Created: BCI 2011-03-16
// -----------------------------------------------------------------------------
MIL_Population* MIL_EntityManager::CreateCrowd( const std::string& type, const MT_Vector2D& point, int number, const std::string& name, MIL_Army_ABC& army, MIL_UrbanObject_ABC* pUrbanObject /*= 0*/ )
{
    return &populationFactory_->Create( type, point, number, name, army, pUrbanObject, 0 );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindPopulation
// Created: BCI 2011-03-17
// -----------------------------------------------------------------------------
MIL_Population* MIL_EntityManager::FindPopulation( MIL_UrbanObject_ABC* urbanObject ) const
{
    return populationFactory_->FindByUrbanObject( urbanObject );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::Accept
// Created: LGY 2011-08-29
// -----------------------------------------------------------------------------
void MIL_EntityManager::Accept( KnowledgesVisitor_ABC& visitor ) const
{
    for( auto it = armyFactory_->CreateIterator(); it.HasMoreElements(); )
    {
        const auto& groups = it.NextElement().GetKnowledgeGroups();
        visitor.VisitKnowledgesGroup( groups.size() );
        for( auto it2 = groups.begin(); it2 != groups.end(); ++it2 )
            it2->second->Accept( visitor );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::VisitUniversalObjects
// Created: LDC 2012-01-26
// -----------------------------------------------------------------------------
void MIL_EntityManager::VisitUniversalObjects( const boost::function< void( MIL_Object_ABC& ) >& visitor ) const
{
    pObjectManager_->VisitUniversalObjects( visitor );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessFormationChangeSuperior
// Created: AHC 2013-01-14
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessFormationChangeSuperior( const UnitMagicAction& message, unsigned int nCtx )
{
    MIL_Formation* pFormation = TaskerToFormation( *this, message.tasker() );
    if( !pFormation )
        throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, "invalid formation" );

    if( !message.parameters().elem_size() || !message.parameters().elem( 0 ).value_size() )
        throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, "wrong parameters number" );

    client::FormationChangeSuperior resendMessage;
    resendMessage().mutable_formation()->set_id( message.tasker().formation().id() );
    if( message.parameters().elem( 0 ).value( 0 ).has_formation() )
    {
        MIL_Formation* pFormation = FindFormation( message.parameters().elem( 0 ).value( 0 ).formation().id() );
        if( !pFormation )
            throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, "invalid new parent formation" );
        resendMessage().mutable_superior()->mutable_formation()->set_id( message.parameters().elem( 0 ).value( 0 ).formation().id() );
    }
    else if( message.parameters().elem( 0 ).value( 0 ).has_party() )
    {
        MIL_Army_ABC* pArmy1 = armyFactory_->Find( message.parameters().elem( 0 ).value( 0 ).party().id() );
        if( !pArmy1 )
            throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode, sword::UnitActionAck::error_invalid_parameter, "invalid new parent army" );
        resendMessage().mutable_superior()->mutable_party()->set_id( message.parameters().elem( 0 ).value( 0 ).party().id() );
    }
    else
        throw MASA_BADPARAM_ASN( sword::UnitActionAck::ErrorCode,
            sword::UnitActionAck::error_invalid_parameter, "invalid new superior" );

    pFormation->OnReceiveChangeSuperior( message, *formationFactory_ );
    resendMessage.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

void MIL_EntityManager::ProcessExecScript( const sword::UnitMagicAction& msg,
        unsigned int brainOwnerId, sword::UnitMagicActionAck& ack )
{
    const auto& params = msg.parameters();
    if( params.elem_size() != 2 )
        throw MASA_BADPARAM_UNIT( "2 parameters expected, got " << params.elem_size() );
    const std::string function = GetStringParam( params, 0 );
    const std::string script = GetStringParam( params, 1 );

    std::string result;
    if( MIL_AgentPion* unit = FindAgentPion( brainOwnerId ) )
        result = unit->GetDecision().ExecuteScript( function, script );
    else
        throw MASA_BADUNIT_UNIT( "invalid unit: " << brainOwnerId );

    ack.mutable_result()->add_elem()->add_value()->set_acharstr( result );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::SetVisionCones
// Created: NLD 2003-10-24
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveControlToggleVisionCones( const sword::ControlEnableVisionCones& message )
{
    const bool enable = message.vision_cones();
    MT_LOG_INFO_MSG( (enable ? "Enabling" : "Disabling") << " vision cones" );
    bSendUnitVisionCones_ = enable;
    if( enable )
        sink_->Apply( []( MIL_Agent_ABC& agent )
        {
            agent.Apply( &PHY_RoleInterface_Perceiver::SendVisionCones );
        } );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::SendVisionCones
// Created: AGE 2007-09-06
// -----------------------------------------------------------------------------
bool MIL_EntityManager::SendVisionCones() const
{
    return bSendUnitVisionCones_;
}
