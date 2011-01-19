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
#include "AgentFactory.h"
#include "ArmyFactory.h"
#include "AutomateFactory.h"
#include "FormationFactory.h"
#include "MIL_Army.h"
#include "MIL_Formation.h"
#include "MIL_Singletons.h"
#include "PopulationFactory.h"
#include "InhabitantFactory.h"
#include "PHY_ResourceNetworkType.h"
#include "UrbanType.h"
#include "Agents/MIL_AgentTypePion.h"
#include "Agents/MIL_AgentPion.h"
#include "Agents/Actions/Firing/PHY_FireResults_Pion.h"
#include "Agents/Units/Categories/PHY_NatureLevel.h"
#include "Agents/Units/Categories/PHY_NatureAtlas.h"
#include "Agents/Units/Categories/PHY_RoePopulation.h"
#include "Agents/Units/Categories/PHY_Volume.h"
#include "Agents/Units/Categories/PHY_Protection.h"
#include "Agents/Units/HumanFactors/PHY_Experience.h"
#include "Agents/Units/HumanFactors/PHY_Tiredness.h"
#include "Agents/Units/HumanFactors/PHY_Morale.h"
#include "Agents/Units/Postures/PHY_Posture.h"
#include "Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Agents/Units/Dotations/PHY_DotationCategory_IndirectFire_ABC.h"
#include "Agents/Units/Dotations/PHY_DotationType.h"
#include "Agents/Units/Dotations/PHY_DotationLogisticType.h"
#include "Agents/Units/Dotations/PHY_ConsumptionType.h"
#include "Agents/Units/Dotations/PHY_IndirectFireDotationClass.h"
#include "Agents/Units/Dotations/PHY_AmmoDotationClass.h"
#include "Agents/Units/Dotations/PHY_DotationNature.h"
#include "Agents/Units/Weapons/PHY_LauncherType.h"
#include "Agents/Units/Weapons/PHY_WeaponType.h"
#include "Agents/Units/Sensors/PHY_SensorType.h"
#include "Agents/Units/Radars/PHY_RadarType.h"
#include "Agents/Units/Radars/PHY_RadarClass.h"
#include "Agents/Units/Humans/PHY_HumanRank.h"
#include "Agents/Units/Humans/PHY_HumanWound.h"
#include "Agents/Units/Composantes/PHY_ActiveProtection.h"
#include "Agents/Units/Composantes/PHY_ComposanteTypePion.h"
#include "Agents/Units/Composantes/PHY_ComposanteState.h"
#include "Agents/Units/Logistic/PHY_MaintenanceWorkRate.h"
#include "Agents/Units/Logistic/PHY_MaintenanceLevel.h"
#include "Agents/Units/Logistic/PHY_BreakdownType.h"
#include "Agents/Roles/Illumination/PHY_RoleInterface_Illumination.h"
#include "Agents/Roles/Logistic/PHY_Convoy_ABC.h"
#include "Agents/Roles/Logistic/PHY_MaintenanceResourcesAlarms.h"
#include "Agents/Roles/Logistic/PHY_MedicalResourcesAlarms.h"
#include "Agents/Roles/Logistic/PHY_SupplyResourcesAlarms.h"
#include "Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Agents/Perceptions/PHY_PerceptionRecoSurveillance.h"
#include "Agents/Perceptions/PHY_PerceptionFlyingShell.h"
#include "Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Automates/MIL_AutomateType.h"
#include "Automates/MIL_Automate.h"
#include "clients_kernel/ExerciseFileLoader.h"
#include "Decision/DEC_Workspace.h"
#include "Effects/MIL_EffectManager.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Specialisations/LOG/MIL_AutomateLOG.h"
#include "hla/HLA_Federate.h"
#include "Inhabitants/MIL_InhabitantType.h"
#include "Inhabitants/MIL_Inhabitant.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/KnowledgeGroupFactory.h"
#include "Knowledge/MIL_KnowledgeGroupType.h"
#include "Network/NET_AsnException.h"
#include "Network/NET_Publisher_ABC.h"
#include "Objects/MIL_FireClass.h"
#include "Objects/MIL_MedicalTreatmentType.h"
#include "Objects/MIL_NbcAgentType.h"
#include "Objects/MIL_ObjectManager.h"
#include "Objects/MIL_ObjectFactory.h"
#include "Objects/MIL_Object_ABC.h"
#include "Orders/MIL_LimaFunction.h"
#include "Orders/MIL_Report.h"
#include "Populations/MIL_PopulationType.h"
#include "Populations/MIL_PopulationAttitude.h"
#include "Populations/MIL_Population.h"
#include "Tools/MIL_IDManager.h"
#include "Tools/MIL_Config.h"
#include "Tools/MIL_ProfilerMgr.h"
#include "Tools/MIL_Tools.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_FormatString.h"
#include "protocol/ClientSenders.h"
#include "protocol/Protocol.h"
#include "resource_network/ResourceNetworkModel.h"
#include <urban/Model.h>
#include <urban/PhysicalAttribute.h>
#include <urban/ObjectVisitor_ABC.h>
#include <urban/StaticModel.h>
#include <urban/TerrainObject_ABC.h>
#include <xeumeuleu/xml.hpp>
#pragma warning( push, 0 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

namespace bfs = boost::filesystem;

using namespace sword;

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_EntityManager )

namespace
{
    long TaskerToId( const Tasker& tasker )
    {
        if( tasker.has_unit() )
            return tasker.unit().id();
        if( tasker.has_automat() )
            return tasker.automat().id();
        if( tasker.has_crowd() )
            return tasker.crowd().id();
        if( tasker.has_formation() )
            return tasker.formation().id();
        if( tasker.has_party() )
            return tasker.party().id();
        throw std::exception( "Misformed tasker in protocol message" );
    }
}
MIL_Automate* TaskerToAutomat( MIL_EntityManager_ABC& manager, const Tasker& tasker )
{
    return tasker.has_automat() && tasker.automat().has_id() ? manager.FindAutomate( tasker.automat().id() ) : 0;
}

MIL_Formation* TaskerToFormation( MIL_EntityManager& manager, const Tasker& tasker )
{
    return tasker.has_formation() && tasker.formation().has_id() ? manager.FindFormation( tasker.formation().id() ) : 0;
}

template< typename Archive >
void save_construct_data( Archive& /*archive*/, const MIL_EntityManager* /*entities*/, const unsigned int /*version*/ )
{
    //@TODO MGD work on serialization to avoid singleton and add test for all entities
    //const AutomateFactory_ABC* const automateFactory = &factory->automateFactory_;
    //archive << armyFactory_
    //        << formationFactory_
    //        <<
}

template< typename Archive >
void load_construct_data( Archive& /*archive*/, MIL_EntityManager* role, const unsigned int /*version*/ )
{
    ::new( role )MIL_EntityManager( MIL_Singletons::GetTime(), MIL_EffectManager::GetEffectManager(),
                                    MIL_Singletons::GetProfiler(), MIL_Singletons::GetHla(),
                                    MIL_AgentServer::GetWorkspace().GetConfig().ReadGCParameter_setPause(), 
                                    MIL_AgentServer::GetWorkspace().GetConfig().ReadGCParameter_setStepMul() );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager constructor
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
MIL_EntityManager::MIL_EntityManager( const MIL_Time_ABC& time, MIL_EffectManager& effects, MIL_ProfilerMgr& profiler, HLA_Federate* hla, unsigned int gcPause, unsigned int gcMult )
    : time_                         ( time )
    , hla_                          ( hla )
    , effectManager_                ( effects )
    , profilerManager_              ( profiler )
    , nRandomBreakdownsNextTimeStep_( 0  )
    , rKnowledgesTime_              ( 0. )
    , rAutomatesDecisionTime_       ( 0. )
    , rPionsDecisionTime_           ( 0. )
    , rPopulationsDecisionTime_     ( 0. )
    , rActionsTime_                 ( 0. )
    , rEffectsTime_                 ( 0. )
    , rStatesTime_                  ( 0. )
    , idManager_                    ( new MIL_IDManager() )
    , pObjectManager_               ( new MIL_ObjectManager() )
    , populationFactory_            ( new PopulationFactory( gcPause, gcMult ) )
    , inhabitantFactory_            ( new InhabitantFactory() )
    , agentFactory_                 ( new AgentFactory( *idManager_, gcPause, gcMult ) )
    , automateFactory_              ( new AutomateFactory( *idManager_, gcPause, gcMult ) )
    , formationFactory_             ( new FormationFactory( *automateFactory_ ) )
    , knowledgeGroupFactory_        ( new KnowledgeGroupFactory() )
    , armyFactory_                  ( new ArmyFactory( *automateFactory_, *agentFactory_, *formationFactory_, *pObjectManager_, *populationFactory_, *inhabitantFactory_, *knowledgeGroupFactory_ ) )
    , gcPause_                      ( gcPause )
    , gcMult_                       ( gcMult )
    , infiniteDotations_            ( false )
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
    MIL_PopulationAttitude        ::Terminate();
    MIL_AutomateType              ::Terminate();
    MIL_AgentTypePion             ::Terminate();
    PHY_SensorType                ::Terminate();
    PHY_ComposanteTypePion        ::Terminate();
    PHY_WeaponType                ::Terminate();
    PHY_LauncherType              ::Terminate();
    PHY_DotationType              ::Terminate();
    PHY_DotationNature            ::Terminate();
    MIL_NbcAgentType              ::Terminate();
    MIL_FireClass                 ::Terminate();
    MIL_MedicalTreatmentType      ::Terminate();
    PHY_Protection                ::Terminate();
    PHY_Volume                    ::Terminate();
    PHY_HumanRank                 ::Terminate();
    PHY_HumanWound                ::Terminate();
    PHY_ComposanteState           ::Terminate();
    PHY_IndirectFireDotationClass ::Terminate();
    PHY_AmmoDotationClass         ::Terminate();
    MIL_KnowledgeGroupType        ::Terminate();
    PHY_Experience                ::Terminate();
    PHY_Tiredness                 ::Terminate();
    PHY_Morale                    ::Terminate();
    PHY_MaintenanceLevel          ::Terminate();
    PHY_PerceptionLevel           ::Terminate();
    PHY_RadarClass                ::Terminate();
    PHY_NatureLevel               ::Terminate();
    PHY_NatureAtlas               ::Terminate();
    PHY_RoePopulation             ::Terminate();
    PHY_MaintenanceWorkRate       ::Terminate();
    PHY_Convoy_ABC                ::Terminate();
    PHY_RadarType                 ::Terminate();
    MIL_PopulationType            ::Terminate();
    MIL_InhabitantType            ::Terminate();
    PHY_DotationLogisticType      ::Terminate();
    PHY_SupplyResourcesAlarms     ::Terminate();
    PHY_MaintenanceResourcesAlarms::Terminate();
    PHY_MedicalResourcesAlarms    ::Terminate();
    MIL_LimaFunction              ::Terminate();
    UrbanType                     ::Terminate();
    PHY_ResourceNetworkType       ::Terminate();
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

    const std::string strOrbat = config.GetOrbatFile();
    MIL_Tools::CheckXmlCrc32Signature( strOrbat );
    MT_LOG_INFO_MSG( MT_FormatString( "ODB file name : '%s'", strOrbat.c_str() ) );

    kernel::ExerciseFileLoader loader ( config );
    loader.LoadAndUpdate( "orbat", boost::bind( &MIL_EntityManager::ReadOrbat, this, _1 ), "resources/orbat0-4.2.xsl" );

    MIL_AgentServer::GetWorkspace().GetResourceNetworkModel().Finalize();

    MT_LOG_INFO_MSG( MT_FormatString( " => %d automates"  , automateFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d pions"      , agentFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d populations", populationFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d inhabitants", inhabitantFactory_->Count() ) );

    // Check automate composition
    if( config.CheckAutomateComposition() )
    {
        for( tools::Iterator< const MIL_Automate& > it = automateFactory_->CreateIterator(); it.HasMoreElements(); )
        {
            const MIL_Automate& automate = it.NextElement();
            if( !automate.CheckComposition() )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, MT_FormatString( "The effective composition of the automate '%d' ('%s') is not consistent with the composition described in the type '%s'", automate.GetID(), automate.GetName().c_str(), automate.GetType().GetName().c_str() ) );
        }
    }

    // Disengage automata for frozen mode
    if( config.IsFrozenMode() )
    {
        automateFactory_->Apply( boost::bind( &MIL_Automate::Disengage, _1 ) );
    }
    UpdateStates();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ReadOrbat
// Created: LDC 2010-11-24
// -----------------------------------------------------------------------------
void MIL_EntityManager::ReadOrbat( xml::xistream& xis )
{
    xis >> xml::start( "orbat" );
    InitializeDotations( xis );
    InitializeArmies   ( xis );
    InitializeDiplomacy( xis );
}

namespace 
{
    class UrbanWrapperVisitor : public urban::ObjectVisitor_ABC
    {
public:
    UrbanWrapperVisitor( MIL_EntityManager& manager ) : manager_( manager )
    {}
    ~UrbanWrapperVisitor(){}
    virtual void Visit( const urban::TerrainObject_ABC& object )
    {
        const urban::PhysicalAttribute* pPhysical = object.Retrieve< urban::PhysicalAttribute >();
        if( pPhysical && pPhysical->GetArchitecture() && ( !UrbanType::GetUrbanType().GetStaticModel().FindType< urban::MaterialCompositionType >( pPhysical->GetArchitecture()->GetMaterial() ) || !UrbanType::GetUrbanType().GetStaticModel().FindType< urban::RoofShapeType >( pPhysical->GetArchitecture()->GetRoofShape() ) ) )
        {
            MT_LOG_INFO_MSG( MT_FormatString( "The architecture of the urban bloc '%d' ('%s') is not consistent with the architecture described in the urban file", object.GetId(), object.GetName().c_str() ) );
            return;
        }
        manager_.CreateUrbanObject( object );
    }
private:
    MIL_EntityManager& manager_;
};
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateUrbanObjects
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateUrbanObjects( urban::Model& urbanModel, const MIL_Config& config )
{
    UrbanWrapperVisitor visitor( *this );
    urbanModel.Accept( visitor );
    LoadUrbanStates( config );
    NotifyPionsInsideUrbanObject();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateUrbanObject
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateUrbanObject( const urban::TerrainObject_ABC& object )
{
    pObjectManager_->CreateUrbanObject( object );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::LoadUrbanStates
// Created: JSR 2010-06-28
// -----------------------------------------------------------------------------
void MIL_EntityManager::LoadUrbanStates( const MIL_Config& config )
{
    const std::string strUrbanState = config.GetUrbanStateFile();
    if( strUrbanState.empty() || !bfs::exists( bfs::path( strUrbanState, bfs::native ) ) )
        return;

    MT_LOG_INFO_MSG( MT_FormatString( "UrbanState file name : '%s'", strUrbanState.c_str() ) );

    MIL_Tools::CheckXmlCrc32Signature( strUrbanState );
    xml::xifstream xis( strUrbanState );
    xis >> xml::start( "urban-state" )
            >> xml::start( "urban-objects" )
                >> xml::list( "urban-object", boost::bind( &MIL_ObjectManager::ReadUrbanState, boost::ref( *pObjectManager_ ), _1 ) )
            >> xml::end
        >> xml::end;
}

namespace
{
    void UrbanNotificationFunctor( MIL_Agent_ABC& agent )
    {
        MT_Vector2D position = agent.GetRole< PHY_RoleInterface_Location >().GetPosition();
        agent.GetRole< PHY_RoleInterface_UrbanLocation >().MagicMove( position );
    }
}
// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::NotifyPionsInsideUrbanObject
// Created: SBO 2010-07-07
// -----------------------------------------------------------------------------
void MIL_EntityManager::NotifyPionsInsideUrbanObject()
{
    agentFactory_->Apply( boost::bind( &UrbanNotificationFunctor, _1 )  );
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
// Name: MIL_EntityManager::InitializeDotations
// Created: JSR 2010-05-04
// -----------------------------------------------------------------------------
void MIL_EntityManager::InitializeDotations( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing resources" );

    xis >> xml::optional >> xml::start( "resources" )
            >> xml::attribute( "infinite", infiniteDotations_ )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ReadDiplomacy
// Created: ABL 2007-07-10
// -----------------------------------------------------------------------------
void MIL_EntityManager::ReadDiplomacy( xml::xistream& xis )
{
    unsigned int id;
    xis >> xml::attribute( "id", id );

    MIL_Army_ABC* pArmy = armyFactory_->Find( id );
    if( !pArmy )
        xis.error( "Unknown side" );
    pArmy->InitializeDiplomacy( xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeArmies
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::InitializeArmies( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing armies" );

    assert( armyFactory_->Count() == 0 );

    xis >> xml::start( "parties" )
        >> xml::list( "party", boost::bind( &ArmyFactory_ABC::Create, boost::ref( *armyFactory_ ), _1 ) )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateAutomat
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateAutomat( xml::xistream& xis, MIL_Entity_ABC& parent )
{
    automateFactory_->Create( xis, parent );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateAutomat
// Created: LDC 2010-10-05
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateAutomat( const MIL_AutomateType& type, unsigned int knowledgeGroup, const std::string& name, MIL_Entity_ABC& parent, unsigned int nCtx )
{
    automateFactory_->Create( type, knowledgeGroup, name, parent, nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreatePion
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_EntityManager::CreatePion( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis )
{
    MIL_AgentPion* pPion = agentFactory_->Create( type, automate, xis );
    if( hla_ )
        hla_->Register( *pPion );
    return *pPion;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_EntityManager::CreatePion( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition )
{
    MIL_AgentPion* pPion = agentFactory_->Create( type, automate, vPosition );
    if( !pPion )
        throw std::runtime_error( "Pion couldn't be created." );
    if( hla_ )
        hla_->Register( *pPion );
    pPion->SendCreation ();
    pPion->SendFullState();
    pPion->SendKnowledge();
    return *pPion;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateObject( xml::xistream& xis, MIL_Army_ABC& army )
{
    pObjectManager_->CreateObject( xis, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::CreateObject( MIL_Army_ABC& army, const std::string& type, const TER_Localisation* pLocalisation, ObstacleType_DemolitionTargetType obstacleType )
{
    return pObjectManager_->CreateObject( army, type, pLocalisation, obstacleType );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation )
{
    return pObjectManager_->CreateObject( type, army, localisation );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateDistantObject
// Created: SLI 2010-10-04
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::CreateDistantObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation, const std::string& name )
{
    return pObjectManager_->CreateDistantObject( type, army, localisation, name );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: JCR 2008-06-06
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::CreateObject( MIL_Army_ABC& army, const MIL_ObjectBuilder_ABC& builder )
{
    return pObjectManager_->CreateObject( army, builder );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::CreateObject( const std::string& /*type*/, MIL_Army_ABC& /*army*/, const TER_Localisation& /*localisation*/, const std::string& /*strOption*/, const std::string& /*strExtra*/, double /*rCompletion*/, double /*rMining*/, double /*rBypass*/ )
{
    throw std::exception( __FUNCTION__ " not implemented" );
    // return pObjectManager_->CreateObject( type, army, localisation, strOption, strExtra, rCompletion, rMining, rBypass );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::FindObject( unsigned int nID ) const
{
    return pObjectManager_->Find( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindObjectType
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
const MIL_ObjectType_ABC& MIL_EntityManager::FindObjectType( const std::string& type ) const
{
    return pObjectManager_->FindType( type );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateKnowledges()
{
    profiler_.Start();
    int currentTimeStep = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    armyFactory_->Apply( boost::bind( &MIL_Army_ABC::UpdateKnowledges, _1, boost::ref( currentTimeStep ) ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::UpdateKnowledges, _1 ) );
    armyFactory_->Apply( boost::bind( &MIL_Army_ABC::CleanKnowledges, _1 ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::CleanKnowledges, _1 ) );
    rKnowledgesTime_ = profiler_.Stop();
}

namespace
{
    void UpdateAutomate( MT_Profiler& profiler, float duration, MIL_Automate& automate )
    {
        profiler.Start();
        automate.UpdateDecision( duration );
        MIL_AgentServer::GetWorkspace().GetProfilerManager().NotifyDecisionUpdated( automate, profiler.Stop() );
    }
    void UpdatePion( MT_Profiler& profiler, float duration, MIL_AgentPion& pion )
    {
        profiler.Start();
        pion.UpdateDecision( duration );
        MIL_AgentServer::GetWorkspace().GetProfilerManager().NotifyDecisionUpdated( pion, profiler.Stop() );
    }
    void UpdatePopulation( MT_Profiler& profiler, float duration, MIL_Population& population )
    {
        profiler.Start();
        population.UpdateDecision( duration );
        MIL_AgentServer::GetWorkspace().GetProfilerManager().NotifyDecisionUpdated( population, profiler.Stop() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateDecisions
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateDecisions()
{
    float duration = static_cast< float >( MIL_AgentServer::GetWorkspace().GetTimeStepDuration() );
    if( profilerManager_.IsProfilingEnabled() )
    {
        MT_Profiler decisionUpdateProfiler;

        profiler_.Start();
        automateFactory_->Apply( boost::bind( &UpdateAutomate, boost::ref(decisionUpdateProfiler), duration, _1 ) );
        rAutomatesDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        agentFactory_->Apply( boost::bind( &UpdatePion, boost::ref(decisionUpdateProfiler), duration, _1 ) );
        rPionsDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        populationFactory_->Apply( boost::bind( &UpdatePopulation, boost::ref(decisionUpdateProfiler), duration, _1 ) );
        rPopulationsDecisionTime_ = profiler_.Stop();
    }
    else
    {
        profiler_.Start();
        automateFactory_->Apply( boost::bind( &MIL_Automate::UpdateDecision, _1, duration ) );
        rAutomatesDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        agentFactory_->Apply( boost::bind( &MIL_AgentPion::UpdateDecision, _1, duration ) );
        rPionsDecisionTime_ = profiler_.Stop();

        profiler_.Start();
        populationFactory_->Apply( boost::bind( &MIL_Population::UpdateDecision, _1, duration ) );
        rPopulationsDecisionTime_ = profiler_.Stop();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateActions
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateActions()
{
    profiler_.Start();
    formationFactory_->Apply( boost::bind( &MIL_Formation::UpdateActions, _1 ) );
    automateFactory_->Apply( boost::bind( &MIL_Automate::UpdateActions, _1 ) );
    agentFactory_->Apply( boost::bind( &MIL_AgentPion::UpdateActions, _1 ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::UpdateActions, _1 ) );
    rActionsTime_ = profiler_.Stop();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateEffects
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateEffects()
{
    profiler_.Start();
    pObjectManager_->ProcessEvents();
    effectManager_.Update();
    rEffectsTime_ = profiler_.Stop();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateStates
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateStates()
{
    profiler_.Start();
    // !! Automate avant Pions (?? => LOG ??)
    automateFactory_->Apply( boost::bind( &MIL_Automate::UpdateState, _1 ) );
    agentFactory_->Apply( boost::bind( &MIL_AgentPion::UpdateState, _1 ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::UpdateState, _1 ) );
    inhabitantFactory_->Apply( boost::bind( &MIL_Inhabitant::UpdateState, _1 ) );
    automateFactory_->Apply( boost::bind( &MIL_Automate::UpdateNetwork, _1 ) );
    agentFactory_->Apply( boost::bind( &MIL_AgentPion::UpdateNetwork, _1 ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::UpdateNetwork, _1 ) );
    inhabitantFactory_->Apply( boost::bind( &MIL_Inhabitant::UpdateNetwork, _1 ) );
    pObjectManager_->UpdateStates();
    rStatesTime_ = profiler_.Stop();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::PreprocessRandomBreakdowns
// Created: NLD 2005-01-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::PreprocessRandomBreakdowns()
{
    const unsigned int nCurrentTimeStep = time_.GetCurrentTick();
    if( nRandomBreakdownsNextTimeStep_ > nCurrentTimeStep )
        return;
    while( nRandomBreakdownsNextTimeStep_ <= nCurrentTimeStep )
        nRandomBreakdownsNextTimeStep_ += ( 3600 * 24 / time_.GetTickDuration() );
    agentFactory_->Apply( boost::bind( &MIL_AgentPion::PreprocessRandomBreakdowns, _1, nRandomBreakdownsNextTimeStep_ ) );
    MT_LOG_INFO_MSG( "Breakdowns preprocessed" );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateKnowledgeGroups
// Created:  FHD 2009-12-18:
// LTO
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateKnowledgeGroups()
{
    profiler_.Start();
    knowledgeGroupFactory_->Apply( boost::bind( &MIL_KnowledgeGroup::UpdateKnowledgeGroup, _1 ) );
    rStatesTime_ = profiler_.Stop();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::Update
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::Update()
{
    PreprocessRandomBreakdowns();
    if( !MIL_AgentServer::GetWorkspace().GetConfig().IsFrozenMode() )
    {
        UpdateKnowledges();
        UpdateDecisions ();
    }
    UpdateActions();
    UpdateEffects();
    UpdateStates();
    UpdateKnowledgeGroups(); // LTO
};

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::Clean
// Created: AGE 2004-11-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::Clean()
{
    agentFactory_->Apply( boost::bind( &MIL_AgentPion::Clean, _1 ) );
    automateFactory_->Apply( boost::bind( &MIL_Automate::Clean, _1 ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::Clean, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void MIL_EntityManager::SendStateToNewClient() const
{
    armyFactory_->Apply( boost::bind( &MIL_Army_ABC::SendCreation, _1 ) );
    armyFactory_->Apply( boost::bind( &MIL_Army_ABC::SendFullState, _1 ) );
    pObjectManager_->SendCreation();
    pObjectManager_->SendFullState();
    armyFactory_->Apply( boost::bind( &MIL_Army_ABC::SendKnowledge, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveUnitOrder
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveUnitOrder( const sword::UnitOrder& message, unsigned int nCtx )
{
    client::TaskCreationRequestAck ack;
    ack().mutable_tasker()->mutable_unit()->set_id( message.tasker().id() );
    ack().set_error_code( sword::OrderAck::no_error );
    try
    {
        MIL_AgentPion* pPion = FindAgentPion( message.tasker().id() );
        if( !pPion )
            throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_unit );
        pPion->OnReceiveOrder( message );
    }
    catch( NET_AsnException< sword::OrderAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveAutomatOrder
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveAutomatOrder( const sword::AutomatOrder& message, unsigned int nCtx )
{
    client::TaskCreationRequestAck ack;
    ack().mutable_tasker()->mutable_automat()->set_id( message.tasker().id() );
    ack().set_error_code( sword::OrderAck::no_error );
    try
    {
        MIL_Automate* pAutomate = FindAutomate( message.tasker().id() );
        if( !pAutomate )
            throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_unit );
        pAutomate->OnReceiveOrder( message );
    }
    catch( NET_AsnException< sword::OrderAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveUnitMagicAction
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveUnitMagicAction( const sword::UnitMagicAction& message, unsigned int nCtx )
{
    client::UnitMagicActionAck ack;
    unsigned int id = 0;
    try
    {
        const sword::Tasker& tasker = message.tasker();
        id = TaskerToId( tasker );
    }
    catch( std::exception& )
    {
        ack().mutable_unit()->set_id( 0 );
        ack().set_error_code( sword::UnitActionAck::error_invalid_unit );
        return;
    }

    ack().mutable_unit()->set_id( id );
    ack().set_error_code( sword::UnitActionAck::no_error );
    try
    {
        switch( message.type() )
        {
        case sword::UnitMagicAction::move_to :
            ProcessMagicActionMoveTo( message, nCtx );
            break;
        case sword::UnitMagicAction::unit_creation :
            if( MIL_Automate*  pAutomate = FindAutomate( id ) )
                pAutomate->OnReceiveUnitCreationRequest( message );
            else
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_unit );
            break;
        case sword::UnitMagicAction::crowd_total_destruction:
        case sword::UnitMagicAction::crowd_kill:
        case sword::UnitMagicAction::crowd_resurrect:
        case sword::UnitMagicAction::crowd_change_attitude:
            if( MIL_Population* pPopulation = pPopulation = populationFactory_->Find ( id ) )
                pPopulation->OnReceiveCrowdMagicAction( message );
            else
                throw NET_AsnException< sword::CrowdMagicActionAck_ErrorCode >( sword::CrowdMagicActionAck::error_invalid_unit );
            break;
        case sword::UnitMagicAction::create_fire_order:
            ProcessMagicActionCreateFireOrder( message, nCtx );
            break;
        case sword::UnitMagicAction::change_knowledge_group:
            ProcessAutomateChangeKnowledgeGroup( message, nCtx );
            break;
        case sword::UnitMagicAction::change_logistic_links:
            ProcessAutomateChangeLogisticLinks( message, nCtx );
            break;
        case sword::UnitMagicAction::unit_change_superior:
            ProcessUnitChangeSuperior( message, nCtx );
            break;
        case sword::UnitMagicAction::change_automat_superior:
        case sword::UnitMagicAction::change_formation_superior:
            ProcessAutomateChangeSuperior( message, nCtx );
            break;
        case sword::UnitMagicAction::log_supply_push_flow:
            ProcessLogSupplyPushFlow( message, nCtx );
            break;
        case sword::UnitMagicAction_Type_log_supply_pull_flow:
            ProcessLogSupplyPullFlow( message, nCtx );
            break;
        case sword::UnitMagicAction_Type_log_supply_change_quotas:
            ProcessLogSupplyChangeQuotas( message, nCtx );
            break;
        case sword::UnitMagicAction::automat_creation:
            if( MIL_Automate*  pAutomate = FindAutomate( id ) )
                ProcessAutomatCreationRequest( message, *pAutomate, nCtx );
            else if( MIL_Formation* pFormation = FindFormation( id ) )
                ProcessAutomatCreationRequest( message, *pFormation, nCtx );
            else
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_unit );
            break;
        case sword::UnitMagicAction::formation_creation :
            if( MIL_Army_ABC*  pArmy = armyFactory_->Find( id ) )
                ProcessFormationCreationRequest( message, pArmy, 0 );
            else if( MIL_Formation* pFormation = FindFormation( id ) )
                ProcessFormationCreationRequest( message, 0, pFormation );
            else
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_unit );
            break;
        case sword::UnitMagicAction::crowd_creation:
            if( MIL_Formation* pFormation = FindFormation( id ) )
                ProcessCrowdCreationRequest( message, pFormation->GetArmy() );
            else if( MIL_Army_ABC*  pArmy = armyFactory_->Find( id ) )
                ProcessCrowdCreationRequest( message, *pArmy );
            else
                throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_unit );
            break;
        default:
            if( MIL_Automate* pAutomate = FindAutomate( id ) )
                pAutomate->OnReceiveUnitMagicAction( message, *armyFactory_ );
            else if( MIL_AgentPion* pPion = FindAgentPion( id ) )
                pPion->OnReceiveUnitMagicAction( message, *armyFactory_ );
            else
                throw NET_AsnException< sword::UnitActionAck::ErrorCode >( sword::UnitActionAck::error_invalid_unit );
            break;
        }
    }
    catch( NET_AsnException< sword::UnitActionAck::ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveUrbanMagicAction
// Created: SLG 2010-12-21
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveUrbanMagicAction( const sword::UrbanMagicAction& message, unsigned int nCtx )
{
    pObjectManager_->OnReceiveUrbanMagicAction( message, nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessAutomatCreationRequest
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessAutomatCreationRequest( const sword::UnitMagicAction& msg, MIL_Entity_ABC& entity, unsigned int nCtx )
{
    try
    {
        if( msg.type() != sword::UnitMagicAction_Type_automat_creation )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

        if( !msg.has_parameters() || msg.parameters().elem_size() != 3 )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

        const sword::MissionParameter& id = msg.parameters().elem( 0 );
        if( id.value_size() != 1 || !id.value().Get(0).has_identifier() )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

        const MIL_AutomateType* pType = MIL_AutomateType::FindAutomateType( id.value().Get(0).identifier() );
        if( !pType )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_unit );

        const sword::MissionParameter& groupId = msg.parameters().elem( 1 );
        if( groupId.value_size() != 1  || !groupId.value().Get(0).has_identifier() )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );

        const sword::MissionParameter& nameParam = msg.parameters().elem( 2 );
        if( nameParam.value_size() != 1 || !nameParam.value().Get(0).has_acharstr() )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_parameter );
        const std::string name = nameParam.value().Get(0).acharstr();
        MIL_AgentServer::GetWorkspace().GetEntityManager().CreateAutomat( *pType, groupId.value().Get(0).identifier(), name, entity, nCtx ); // auto-registration
    }
    catch( std::runtime_error& )
    {
        throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_unit );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessFormationCreationRequest
// Created: LDC 2010-10-20
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessFormationCreationRequest( const sword::UnitMagicAction& message, MIL_Army_ABC* army, MIL_Formation* formation )
{
    client::MagicActionAck ack;
    ack().set_error_code( sword::MagicActionAck::no_error );
    if( !army )
    {
        if( !formation )
        {
            ack().set_error_code( sword::MagicActionAck::error_invalid_parameter );
            return;
        }
        army = &(formation->GetArmy());
    }
    if( !message.has_parameters() || message.parameters().elem_size() != 3 || !(message.parameters().elem( 0 ).value_size() == 1) || !message.parameters().elem( 0 ).value().Get(0).has_areal() )
    {
        ack().set_error_code( sword::MagicActionAck::error_invalid_parameter );
        return;
    }
    const ::sword::MissionParameters& parameters = message.parameters();
    int level = static_cast< int >( parameters.elem( 0 ).value().Get(0).areal() );
    std::string name = ( parameters.elem( 1 ).value_size() == 1 && parameters.elem( 1 ).value().Get(0).has_acharstr() ) ? parameters.elem( 1 ).value().Get(0).acharstr() : std::string();
    std::string logLevel = ( parameters.elem( 2 ).value_size() == 1 && parameters.elem( 2 ).value().Get(0).has_acharstr() ) ? parameters.elem( 2 ).value().Get(0).acharstr() : std::string();;
    MIL_Formation& newFormation = formationFactory_->Create( level, name, logLevel, *army, formation );
    ack.Send( NET_Publisher_ABC::Publisher() );
    newFormation.SendCreation();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessCrowdCreationRequest
// Created: LDC 2010-10-22
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessCrowdCreationRequest( const sword::UnitMagicAction& message, MIL_Army_ABC& army )
{
    client::MagicActionAck ack;
    ack().set_error_code( sword::MagicActionAck::no_error );
    if( !message.has_parameters() || message.parameters().elem_size() != 4
        || message.parameters().elem( 0 ).value_size() != 1 || !message.parameters().elem( 0 ).value().Get(0).has_acharstr() 
        || message.parameters().elem( 1 ).value_size() != 1 || !message.parameters().elem( 1 ).value().Get(0).has_point() 
        || message.parameters().elem( 2 ).value_size() != 1 || !message.parameters().elem( 2 ).value().Get(0).has_areal() )
    {
        ack().set_error_code( sword::MagicActionAck::error_invalid_parameter );
        return;
    }
    const ::sword::MissionParameters& parameters = message.parameters();
    std::string type = parameters.elem( 0 ).value().Get(0).acharstr();
    ::sword::Location location = parameters.elem( 1 ).value().Get(0).point().location();
    if( !location.has_coordinates() )
    {
        ack().set_error_code( sword::MagicActionAck::error_invalid_parameter );
        return;
    }
    ack.Send( NET_Publisher_ABC::Publisher() );
    MT_Vector2D point;
    MIL_Tools::ConvertCoordMosToSim( location.coordinates().elem( 0 ), point );
    int number = static_cast< int >( parameters.elem( 2 ).value().Get(0).areal() );
    std::string name = ( parameters.elem( 3 ).value_size() == 1 && parameters.elem( 3 ).value().Get(0).has_acharstr() ) ? parameters.elem( 3 ).value().Get(0).acharstr() : std::string();
    populationFactory_->Create( type, point, number, name, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveKnowledgeMagicAction
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveKnowledgeMagicAction( const sword::KnowledgeMagicAction& message, unsigned int nCtx )
{
    client::KnowledgeGroupMagicActionAck ack;
    ack().mutable_knowledge_group()->set_id( message.knowledge_group().id() );
    ack().set_error_code( sword::KnowledgeGroupAck::no_error );
    try
    {
        switch( message.type() )
        {
        case sword::KnowledgeMagicAction::enable :
        case sword::KnowledgeMagicAction::update_party :
        case sword::KnowledgeMagicAction::update_party_parent :
        case sword::KnowledgeMagicAction::update_type :
            ProcessKnowledgeGroupUpdate( message, nCtx );
            break;
        default:
            throw NET_AsnException< sword::KnowledgeGroupAck::ErrorCode >( sword::KnowledgeGroupAck::error_invalid_type );
            break;
        }
    }
    catch( NET_AsnException< sword::KnowledgeGroupAck::ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveCrowdOrder
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveCrowdOrder( const sword::CrowdOrder& message, unsigned int nCtx )
{
    client::TaskCreationRequestAck ack;
    ack().mutable_tasker()->mutable_crowd()->set_id( message.tasker().id() );
    ack().set_error_code( sword::OrderAck::no_error );
    try
    {
        MIL_Population* pPopulation = populationFactory_->Find( message.tasker().id() );
        if( !pPopulation )
            throw NET_AsnException< sword::OrderAck_ErrorCode >( sword::OrderAck::error_invalid_unit );
        pPopulation->OnReceiveOrder( message );
    }
    catch( NET_AsnException< sword::OrderAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveFragOrder
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveFragOrder( const sword::FragOrder& message, unsigned int nCtx )
{
    client::FragOrderAck ack;
    unsigned int taskerId = TaskerToId( message.tasker() );
    ack().set_error_code( sword::OrderAck::no_error );
    try
    {
        if( MIL_Automate* pAutomate = FindAutomate( taskerId ) )
        {
            ack().mutable_tasker()->mutable_automat()->set_id( taskerId );
            pAutomate->OnReceiveFragOrder( message );
        }
        else if( MIL_Population* pPopulation = populationFactory_->Find( taskerId ) )
        {
            ack().mutable_tasker()->mutable_crowd()->set_id( taskerId );
            pPopulation->OnReceiveFragOrder( message );
        }
        else if( MIL_AgentPion* pPion = FindAgentPion ( taskerId ) )
        {
            ack().mutable_tasker()->mutable_unit()->set_id( taskerId );
            pPion->OnReceiveFragOrder( message );
        }
        else
        {
            ack().mutable_tasker()->mutable_unit()->set_id( 0 );
            throw NET_AsnException< sword::OrderAck::ErrorCode >( sword::OrderAck::error_invalid_unit );
        }
    }
    catch( NET_AsnException< sword::OrderAck::ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveSetAutomateMode
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveSetAutomateMode( const sword::SetAutomatMode& message, unsigned int nCtx )
{
    client::SetAutomatModeAck ack;
    ack().mutable_automate()->set_id( message.automate().id() );
    ack().set_error_code( sword::SetAutomatModeAck::no_error );
    try
    {
        MIL_Automate* pAutomate = FindAutomate( message.automate().id() );
        if( !pAutomate )
            throw NET_AsnException< sword::SetAutomatModeAck::ErrorCode >( sword::SetAutomatModeAck::error_invalid_unit );
        pAutomate->OnReceiveSetAutomateMode( message );
    }
    catch( NET_AsnException< sword::SetAutomatModeAck::ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveUnitCreationRequest
// Created: AGE 2007-06-18
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveUnitCreationRequest( const sword::UnitCreationRequest& message, unsigned int nCtx )
{
    client::UnitCreationRequestAck ack;
    ack().set_error( sword::UnitActionAck::no_error );
    try
    {
        MIL_Automate* pAutomate = FindAutomate( message.superior().id() );
        if( !pAutomate )
            throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_unit );
        pAutomate->OnReceiveUnitCreationRequest( message );
    }
    catch( NET_AsnException< sword::UnitActionAck_ErrorCode >& e )
    {
        ack().set_error( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveObjectMagicAction
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveObjectMagicAction( const sword::ObjectMagicAction& message, unsigned int nCtx )
{
    pObjectManager_->OnReceiveObjectMagicAction( message, nCtx, *armyFactory_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveChangeDiplomacy
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveChangeDiplomacy( const MagicAction& message, unsigned int nCtx )
{
    client::ChangeDiplomacyAck ack;
    ack().mutable_party1()->set_id( message.parameters().elem( 0 ).value().Get(0).identifier() );
    ack().mutable_party2()->set_id( message.parameters().elem( 1 ).value().Get(0).identifier() );
    ack().set_diplomacy( ( sword::EnumDiplomacy ) message.parameters().elem( 2 ).value().Get(0).enumeration() );
    ack().set_error_code( sword::ChangeDiplomacyAck::no_error_diplomacy );
    try
    {
        MIL_Army_ABC* pArmy1 = armyFactory_->Find( message.parameters().elem( 0 ).value().Get(0).identifier() );
        if( !pArmy1 )
            throw NET_AsnException< sword::ChangeDiplomacyAck_ErrorCode >( sword::ChangeDiplomacyAck::error_invalid_party_diplomacy );
        pArmy1->OnReceiveChangeDiplomacy( message.parameters() );
    }
    catch( NET_AsnException< sword::ChangeDiplomacyAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveChangeResourceLinks
// Created: JSR 2010-08-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveChangeResourceLinks( const sword::MagicAction& message, unsigned int nCtx )
{
    pObjectManager_->OnReceiveChangeResourceLinks( message, nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessAutomateChangeKnowledgeGroup
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessAutomateChangeKnowledgeGroup( const sword::UnitMagicAction& message, unsigned int nCtx )
{
    client::AutomatChangeKnowledgeGroupAck ack;
    ack().set_error_code( sword::HierarchyModificationAck::no_error_hierarchy );
    try
    {
        MIL_Automate* pAutomate = TaskerToAutomat( *this, message.tasker() );
        if( !pAutomate )
            throw NET_AsnException< sword::HierarchyModificationAck_ErrorCode >( sword::HierarchyModificationAck::error_invalid_automate );
        pAutomate->OnReceiveChangeKnowledgeGroup( message, *armyFactory_ );
    }
    catch( NET_AsnException< sword::HierarchyModificationAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );

    if( ack().error_code() == sword::HierarchyModificationAck::no_error_hierarchy )
    {
        if( message.has_parameters() &&
            message.parameters().elem_size() == 2 )
        {
            client::AutomatChangeKnowledgeGroup resendMessage;
            resendMessage().mutable_automat()->set_id( message.tasker().automat().id() );
            if( message.parameters().elem( 0 ).value_size() == 1 && message.parameters().elem( 0 ).value().Get(0).has_knowledgegroup() )
                resendMessage().mutable_knowledge_group()->set_id( message.parameters().elem( 0 ).value().Get(0).knowledgegroup().id() );
            if( message.parameters().elem( 1 ).value_size() == 1 && message.parameters().elem( 1 ).value().Get(0).has_party() )
                resendMessage().mutable_party()->set_id( message.parameters().elem( 1 ).value().Get(0).party().id() );
            resendMessage.Send( NET_Publisher_ABC::Publisher() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessAutomateChangeLogisticLinks
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessAutomateChangeLogisticLinks( const sword::UnitMagicAction& message, unsigned int nCtx )
{
    client::ChangeLogisticLinksAck ack;
    ack().set_error_code( sword::HierarchyModificationAck::no_error_hierarchy );
    try
    {        
        MIL_Automate* pAutomate = TaskerToAutomat( *this, message.tasker() );
        MIL_Formation* pFormation = TaskerToFormation( *this, message.tasker() );
        if( pAutomate )
            pAutomate->OnReceiveChangeLogisticLinks( message );

        if( pAutomate == 0 && pFormation == 0)
            throw NET_AsnException< sword::HierarchyModificationAck_ErrorCode >( sword::HierarchyModificationAck::error_invalid_automate );
        MIL_AutomateLOG* brainLog = pAutomate!=0 ? pAutomate->GetBrainLogistic() :  pFormation->GetBrainLogistic();
        if( brainLog )
            brainLog->OnReceiveChangeLogisticLinks( message );
    }
    catch( NET_AsnException< sword::HierarchyModificationAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );

    if( ack().error_code() == sword::HierarchyModificationAck::no_error_hierarchy )
    {
        MIL_Automate* pAutomate = TaskerToAutomat( *this, message.tasker() );
        MIL_Formation* pFormation = TaskerToFormation( *this, message.tasker() );
        if( pAutomate )
            pAutomate->SendLogisticLinks();
        else
            pFormation->SendLogisticLinks();
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessAutomateChangeSuperior
// Created: NLD 2007-04-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessAutomateChangeSuperior( const sword::UnitMagicAction& message, unsigned int nCtx )
{
    client::AutomatChangeSuperiorAck ack;
    ack().set_error_code( sword::HierarchyModificationAck::no_error_hierarchy );
    try
    {
        MIL_Automate* pAutomate = TaskerToAutomat( *this, message.tasker() );
        if( !pAutomate )
            throw NET_AsnException< sword::HierarchyModificationAck_ErrorCode >( sword::HierarchyModificationAck::error_invalid_automate );
        pAutomate->OnReceiveChangeSuperior( message, *formationFactory_ );
    }
    catch( NET_AsnException< sword::HierarchyModificationAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );

    if( ack().error_code() == sword::HierarchyModificationAck::no_error_hierarchy )
    {
        client::AutomatChangeSuperior resendMessage;
        resendMessage().mutable_automat()->set_id( message.tasker().automat().id() );
        if( message.type() == sword::UnitMagicAction_Type_change_formation_superior )
            resendMessage().mutable_superior()->mutable_formation()->set_id( message.parameters().elem( 0 ).value().Get(0).formation().id() );
        else if( message.type() == sword::UnitMagicAction_Type_change_automat_superior )
            resendMessage().mutable_superior()->mutable_automat()->set_id( message.parameters().elem( 0 ).value().Get(0).automat().id() );
        resendMessage.Send( NET_Publisher_ABC::Publisher() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessUnitChangeSuperior
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessUnitChangeSuperior( const sword::UnitMagicAction& message, unsigned int nCtx )
{
    client::UnitChangeSuperiorAck ack;
    ack().set_error_code( sword::HierarchyModificationAck::no_error_hierarchy );
    try
    {
        MIL_AgentPion* pPion = ( message.tasker().has_unit() && message.tasker().unit().has_id() ) ? FindAgentPion( message.tasker().unit().id() ) : 0;
        if( !pPion )
            throw NET_AsnException< sword::HierarchyModificationAck_ErrorCode >( sword::HierarchyModificationAck::error_invalid_agent );
        pPion->OnReceiveChangeSuperior( *this, message );
    }
    catch( NET_AsnException< sword::HierarchyModificationAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
    if( ack().error_code() == sword::HierarchyModificationAck::no_error_hierarchy )
    {
        client::UnitChangeSuperior resendMessage;
        resendMessage().mutable_unit()->set_id ( message.tasker().unit().id() );
        resendMessage().mutable_parent()->set_id ( message.parameters().elem( 0 ).value().Get(0).automat().id() );
        resendMessage.Send( NET_Publisher_ABC::Publisher() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessLogSupplyChangeQuotas
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessLogSupplyChangeQuotas( const sword::UnitMagicAction& message, unsigned int nCtx )
{
    client::LogSupplyChangeQuotasAck ack;
    ack().set_ack( sword::LogSupplyChangeQuotasAck::no_error_quotas );
    try
    {
        MIL_Automate* pAutomat = TaskerToAutomat( *this, message.tasker() );
        MIL_Formation* pFormation = TaskerToFormation( *this, message.tasker() );
        if( !pAutomat && !pFormation )
            throw NET_AsnException< sword::LogSupplyChangeQuotasAck::ErrorCode >( sword::LogSupplyChangeQuotasAck::error_invalid_receiver );
        MIL_AutomateLOG* pReceiver = pAutomat ? pAutomat->GetBrainLogistic() : pFormation->GetBrainLogistic();
        if( !pReceiver )
            throw NET_AsnException< sword::LogSupplyChangeQuotasAck::ErrorCode >( sword::LogSupplyChangeQuotasAck::error_invalid_receiver );
        pReceiver->OnReceiveLogSupplyChangeQuotas( message.parameters() );
    }
    catch( NET_AsnException< sword::LogSupplyChangeQuotasAck::ErrorCode >& e )
    {
        ack().set_ack( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessLogSupplyPushFlow
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessLogSupplyPushFlow( const sword::UnitMagicAction& message, unsigned int nCtx )
{
    client::LogSupplyPushFlowAck ack;
    ack().set_ack( sword::LogSupplyPushFlowAck::no_error_pushflow );
    try
    {
        MIL_Automate* pAutomate = TaskerToAutomat( *this, message.tasker() );
        if( pAutomate )
            pAutomate->OnReceiveLogSupplyPushFlow( message.parameters() );
        else
            throw NET_AsnException< sword::LogSupplyPushFlowAck::ErrorCode >( sword::LogSupplyPushFlowAck::error_invalid_receiver );
    }
    catch( NET_AsnException< sword::LogSupplyPushFlowAck::ErrorCode >& e )
    {
        ack().set_ack( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessLogSupplyPushFlow
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessLogSupplyPullFlow( const sword::UnitMagicAction& message, unsigned int nCtx )
{
    client::LogSupplyPullFlowAck ack;
    ack().set_ack( sword::LogSupplyPullFlowAck::no_error_pullflow );
    try
    {
        MIL_Automate* pAutomate = TaskerToAutomat( *this, message.tasker() );
        if( pAutomate )
            pAutomate->OnReceiveLogSupplyPullFlow( message.parameters() );
        else
            throw NET_AsnException< sword::LogSupplyPullFlowAck::ErrorCode >( sword::LogSupplyPullFlowAck::error_invalid_receiver );
    }
    catch( NET_AsnException< sword::LogSupplyPullFlowAck::ErrorCode >& e )
    {
        ack().set_ack( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessMagicActionMoveTo
// Created: JSR 2010-04-07
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessMagicActionMoveTo( const sword::UnitMagicAction& message, unsigned int )
{
    if( message.tasker().has_automat() && message.tasker().automat().has_id() )
    {
        if( MIL_Automate*  pAutomate = FindAutomate ( message.tasker().automat().id() ) )
            return pAutomate->OnReceiveMagicActionMoveTo( message );
    }
    else if( message.tasker().has_unit() && message.tasker().unit().has_id() )
    {
        if( MIL_AgentPion* pPion = FindAgentPion( message.tasker().unit().id() ) )
            return pPion->OnReceiveMagicActionMoveTo( message );
    }
    else if( message.tasker().has_crowd() && message.tasker().crowd().has_id() )
    { 
        if( MIL_Population* pPopulation = populationFactory_->Find( message.tasker().crowd().id() ) )
            return pPopulation->OnReceiveCrowdMagicActionMoveTo( message );
    }
    throw NET_AsnException< sword::UnitActionAck_ErrorCode >( sword::UnitActionAck::error_invalid_unit );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveKnowledgeGroupCreation
// Created: FHD 2009-12-15:
// LTO
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveKnowledgeGroupCreation( const sword::MagicAction& /*message*/, unsigned int nCtx )
{
    client::KnowledgeGroupCreationAck ack;
    ack().mutable_knowledge_group()->set_id( 0 );
    ack().set_error_code( sword::KnowledgeGroupAck::no_error );
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}


// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessKnowledgeGroupUpdate
// Created: FDS 2010-01-13
// LTO
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessKnowledgeGroupUpdate( const sword::KnowledgeMagicAction& message, unsigned int nCtx )
{
    client::KnowledgeGroupUpdateAck ack;
    ack().mutable_knowledge_group()->set_id( message.knowledge_group().id() );
    ack().set_error_code( sword::KnowledgeGroupAck::no_error );
    try
    {
        MIL_KnowledgeGroup* pReceiver = FindKnowledgeGroup( message.knowledge_group().id() );
        if( !pReceiver || pReceiver->IsJammed() )
            throw NET_AsnException< sword::KnowledgeGroupAck::ErrorCode >( sword::KnowledgeGroupAck::error_invalid_type );
        pReceiver->OnReceiveKnowledgeGroupUpdate( message, *armyFactory_  );
    }
    catch( NET_AsnException< sword::KnowledgeGroupAck::ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessMagicActionCreateFireOrder
// Created: MGD 2010-02-24
// LTO
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessMagicActionCreateFireOrder( const sword::UnitMagicAction& msg, unsigned int nCtx )
{
    client::ActionCreateFireOrderAck ack;
    ack().set_error_code( sword::ActionCreateFireOrderAck::no_error );
    try
    {
        if( !msg.has_parameters() || msg.parameters().elem_size() != 3)
            throw NET_AsnException< sword::ActionCreateFireOrderAck::ErrorCode >( sword::ActionCreateFireOrderAck::error_invalid_target );

        MIL_Agent_ABC* reporter = ( msg.tasker().has_unit() && msg.tasker().unit().has_id() ) ? FindAgentPion( msg.tasker().unit().id() ) : 0;
        if( !reporter )
            throw NET_AsnException< sword::ActionCreateFireOrderAck::ErrorCode >( sword::ActionCreateFireOrderAck::error_invalid_reporter );

        // Target
        const sword::MissionParameter& target = msg.parameters().elem( 0 );
        if( target.value_size() != 1 || !target.value().Get(0).has_identifier() )
            throw NET_AsnException< sword::CrowdMagicActionAck_ErrorCode >( sword::CrowdMagicActionAck::error_invalid_parameter );

        boost::shared_ptr< DEC_Knowledge_Agent > targetKn = reporter->GetKnowledge().ResolveKnowledgeAgent( target.value().Get(0).identifier() );
        if( !targetKn )
            throw NET_AsnException< sword::ActionCreateFireOrderAck::ErrorCode >( sword::ActionCreateFireOrderAck::error_invalid_target );

        // Ammo
        const sword::MissionParameter& ammo = msg.parameters().elem( 1 );
        if( ammo.value_size() != 1 || !ammo.value().Get(0).has_resourcetype() )
            throw NET_AsnException< sword::CrowdMagicActionAck_ErrorCode >( sword::CrowdMagicActionAck::error_invalid_parameter );

        const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( ammo.value().Get(0).resourcetype().id() );
        if( !pDotationCategory || !pDotationCategory->CanBeUsedForIndirectFire() )
            throw NET_AsnException< sword::ActionCreateFireOrderAck::ErrorCode >( sword::ActionCreateFireOrderAck::error_invalid_ammunition );

        if( pDotationCategory->IsGuided() && !targetKn->GetAgentKnown().GetRole< PHY_RoleInterface_Illumination >().IsIlluminated() )
            throw NET_AsnException< sword::ActionCreateFireOrderAck::ErrorCode >( sword::ActionCreateFireOrderAck::error_target_not_illuminated );

        // Iterations
        const sword::MissionParameter& iterations = msg.parameters().elem( 2 );
        if( iterations.value_size() != 1 || !iterations.value().Get(0).has_areal() )
            throw NET_AsnException< sword::CrowdMagicActionAck_ErrorCode >( sword::CrowdMagicActionAck::error_invalid_parameter );

        PHY_FireResults_Pion fireResult( *reporter , targetKn->GetPosition(), *pDotationCategory );
        unsigned int ammos = (unsigned int) pDotationCategory->GetIndirectFireData()->ConvertToNbrAmmo( iterations.value().Get(0).areal() );

        MIL_Report::PostEvent( *reporter, MIL_Report::eReport_IndirectFireOnTarget, targetKn );

        pDotationCategory->ApplyIndirectFireEffect( *reporter, targetKn->GetAgentKnown(), ammos , fireResult );
    }
    catch( NET_AsnException< sword::ActionCreateFireOrderAck::ErrorCode >& e )
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
// Name: MIL_EntityManager::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    ArmyFactory_ABC * armyFactory;
    FormationFactory_ABC * formationFactory;
    AutomateFactory_ABC * automateFactory;
    AgentFactory_ABC * agentFactory;
    PopulationFactory_ABC * populationFactory;
    InhabitantFactory_ABC * inhabitantFactory;
    KnowledgeGroupFactory_ABC * knowledgeGroupFactory; // LTO
    MIL_ObjectManager* objectManager;
    file //>> effectManager_  // Effets li�s aux actions qui ne sont pas sauvegard�s
         >> knowledgeGroupFactory; // LTO
    knowledgeGroupFactory_.reset( knowledgeGroupFactory );
    file >> armyFactory
         >> formationFactory//@TODO MGD serialize
         >> agentFactory
         >> automateFactory
         >> populationFactory
         >> inhabitantFactory
         >> objectManager
         >> rKnowledgesTime_
         >> rAutomatesDecisionTime_
         >> rPionsDecisionTime_
         >> rPopulationsDecisionTime_
         >> rActionsTime_
         >> rEffectsTime_
         >> rStatesTime_
         >> nRandomBreakdownsNextTimeStep_;

    armyFactory_.reset( armyFactory );
    formationFactory_.reset( formationFactory );
    agentFactory_.reset( agentFactory );
    automateFactory_.reset( automateFactory );
    populationFactory_.reset( populationFactory );
    inhabitantFactory_.reset( inhabitantFactory );
    pObjectManager_.reset( objectManager );

    MT_LOG_INFO_MSG( MT_FormatString( " => %d automates"  , automateFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d pions"      , agentFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d populations", populationFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d inhabitants", inhabitantFactory_->Count() ) );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    const ArmyFactory_ABC * const tempArmy = armyFactory_.get();
    const FormationFactory_ABC * const tempFormationFactory = formationFactory_.get();
    const AgentFactory_ABC * const tempAgentFactory = agentFactory_.get();
    const AutomateFactory_ABC * const tempAutomateFactory = automateFactory_.get();
    const PopulationFactory_ABC * const populationFactory = populationFactory_.get();
    const InhabitantFactory_ABC * const inhabitantFactory = inhabitantFactory_.get();
    const KnowledgeGroupFactory_ABC* const knowledgeGroupFactory = knowledgeGroupFactory_.get();
    const MIL_ObjectManager* const objectManager = pObjectManager_.get();

    file //<< effectManager_  // Effets li�s aux actions qui ne sont pas sauvegard�s
         << knowledgeGroupFactory; // LTO
    file << tempArmy
         << tempFormationFactory
         << tempAgentFactory
         << tempAutomateFactory
         << populationFactory
         << inhabitantFactory
         << objectManager
         << rKnowledgesTime_
         << rAutomatesDecisionTime_
         << rPionsDecisionTime_
         << rPopulationsDecisionTime_
         << rActionsTime_
         << rEffectsTime_
         << rStatesTime_
         << nRandomBreakdownsNextTimeStep_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::WriteODB
// Created: NLD 2006-05-29
// -----------------------------------------------------------------------------
void MIL_EntityManager::WriteODB( xml::xostream& xos ) const
{
    xos << xml::start( "orbat" );
    if ( infiniteDotations_ )
    {
        xos << xml::start( "resources" )
                << xml::attribute( "infinite", infiniteDotations_ )
            << xml::end;
    }
    xos     << xml::start( "parties" );
                armyFactory_->Apply( boost::bind( &MIL_Army_ABC::WriteODB, _1, boost::ref( xos ) ) );
    xos     << xml::end
            << xml::start( "diplomacy" );
                armyFactory_->Apply( boost::bind( &MIL_Army_ABC::WriteDiplomacyODB, _1, boost::ref( xos ) ) );
    xos     << xml::end
        << xml::end;
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
// LTO
// -----------------------------------------------------------------------------
MIL_KnowledgeGroup* MIL_EntityManager::FindKnowledgeGroup( unsigned int nID ) const
{
    return knowledgeGroupFactory_->Find( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindAgentPion
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_EntityManager::FindAgentPion( unsigned int nID ) const
{
    return agentFactory_->Find( nID );
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
// Name: MIL_EntityManager::HasInfiniteDotations
// Created: JSR 2010-05-04
// -----------------------------------------------------------------------------
const bool MIL_EntityManager::HasInfiniteDotations() const
{
    return infiniteDotations_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetUrbanObjectWrapper
// Created: JSR 2011-01-18
// -----------------------------------------------------------------------------
UrbanObjectWrapper& MIL_EntityManager::GetUrbanObjectWrapper( const urban::TerrainObject_ABC& object )
{
    return pObjectManager_->GetUrbanObjectWrapper( object );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ConvertUrbanIdToSimId
// Created: JSR 2011-01-18
// -----------------------------------------------------------------------------
unsigned int MIL_EntityManager::ConvertUrbanIdToSimId( unsigned int urbanId )
{
    return pObjectManager_->ConvertUrbanIdToSimId( urbanId );
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
void MIL_EntityManager::SetToTasker( sword::Tasker& tasker, unsigned int id ) const
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
    else throw( std::exception( "Misformed tasker in protocol message" ) );
}
