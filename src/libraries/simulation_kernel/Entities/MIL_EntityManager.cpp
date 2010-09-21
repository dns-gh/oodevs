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
#include "Agents/MIL_AgentTypePion.h"
#include "Agents/MIL_AgentPion.h"
#include "Automates/MIL_AutomateType.h"
#include "Automates/MIL_Automate.h"
#include "Effects/MIL_EffectManager.h"
#include "hla/HLA_Federate.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/MIL_KnowledgeGroupType.h"
#include "MIL_Army.h"
#include "MIL_Formation.h"
#include "MIL_Singletons.h"
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
#include "simulation_kernel/Decision/DEC_Workspace.h"
#include "simulation_kernel/AgentFactory.h"
#include "simulation_kernel/ArmyFactory.h"
#include "simulation_kernel/AutomateFactory.h"
#include "simulation_kernel/FormationFactory.h"
#include "simulation_kernel/PopulationFactory.h"
#include "simulation_kernel/Knowledge/KnowledgeGroupFactory.h"
#include "simulation_kernel/Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "simulation_kernel/PHY_ResourceNetworkType.h"
#include "simulation_kernel/UrbanModel.h"
#include "simulation_kernel/UrbanType.h"
#include "tools/MIL_IDManager.h"
#include "tools/MIL_ProfilerMgr.h"
#include "tools/MIL_Tools.h"
#include "MT_Tools/MT_FormatString.h"
#include <urban/ObjectVisitor_ABC.h>
#include <xeumeuleu/xml.hpp>
#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#pragma warning( pop )

#include "protocol/ClientSenders.h"
#include "protocol/protocol.h"

namespace bfs = boost::filesystem;

using namespace Common;
using namespace MsgsSimToClient;
using namespace MsgsClientToSim;

BOOST_CLASS_EXPORT_IMPLEMENT( MIL_EntityManager )

namespace
{

long TaskerToId( const Tasker& tasker )
{
    if( tasker.has_unit() )
        return tasker.unit().id();
    if( tasker.has_automat() )
        return tasker.automat().id();
    if( tasker.has_population() )
        return tasker.population().id();
    if( tasker.has_formation() )
        return tasker.formation().id();
    throw( std::exception( "Misformed tasker in protocol message" ) );
}

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
                                    MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().GetDatabase(),
                                    MIL_AgentServer::GetWorkspace().GetConfig().ReadGCParameter_setPause(), 
                                    MIL_AgentServer::GetWorkspace().GetConfig().ReadGCParameter_setStepMul() );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager constructor
// Created: NLD 2004-08-10
// -----------------------------------------------------------------------------
MIL_EntityManager::MIL_EntityManager( const MIL_Time_ABC& time, MIL_EffectManager& effects, MIL_ProfilerMgr& profiler, HLA_Federate* hla, DEC_DataBase& database, unsigned int gcPause, unsigned int gcMult )
    : time_                         ( time )
    , profilerManager_              ( profiler )
    , hla_                          ( hla )
    , effectManager_                ( effects )
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
    , populationFactory_            ( new PopulationFactory( database, gcPause, gcMult ) )
    , agentFactory_                 ( new AgentFactory( *idManager_, database, gcPause, gcMult ) )
    , automateFactory_              ( new AutomateFactory( *idManager_, database, gcPause, gcMult ) )
    , formationFactory_             ( new FormationFactory( *automateFactory_ ) )
    , knowledgeGroupFactory_        ( new KnowledgeGroupFactory() )
    , armyFactory_                  ( new ArmyFactory( *automateFactory_, *agentFactory_, *formationFactory_, *pObjectManager_, *populationFactory_, *knowledgeGroupFactory_ ) )
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
    PHY_DotationLogisticType      ::Terminate();
    PHY_SupplyResourcesAlarms     ::Terminate();
    PHY_MaintenanceResourcesAlarms::Terminate();
    PHY_MedicalResourcesAlarms    ::Terminate();
    MIL_LimaFunction              ::Terminate();
    UrbanType                     ::Terminate();
    PHY_ResourceNetworkType       ::Terminate();

    delete pObjectManager_;
}

// =============================================================================
// ODB INIT
// =============================================================================

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
    MT_LOG_INFO_MSG( MT_FormatString( "ODB file name : '%s'", strOrbat.c_str() ) );

    xml::xifstream xis( strOrbat );
    xis >> xml::start( "orbat" );

    InitializeDotations( xis );
    InitializeArmies   ( xis );
    InitializeDiplomacy( xis );

    MT_LOG_INFO_MSG( MT_FormatString( " => %d automates"  , automateFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d pions"      , agentFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d populations", populationFactory_->Count() ) );

    xis >> xml::end;

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

class MIL_EntityManager::UrbanWrapperVisitor : public urban::ObjectVisitor_ABC
{
public:
    UrbanWrapperVisitor( MIL_EntityManager& manager ) : manager_( manager )
    {}
    ~UrbanWrapperVisitor(){}
    virtual void Visit( const urban::TerrainObject_ABC& object )
    {
        manager_.CreateUrbanObject( object );
    }
private:
    MIL_EntityManager& manager_;
};

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateUrbanObjects
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateUrbanObjects( UrbanModel& urbanModel, const MIL_Config& config )
{
    UrbanWrapperVisitor visitor( *this );
    urbanModel.Accept( visitor );
    LoadUrbanStates( config );
    NotifyPionsInsideUrbanObject();
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateUrbanObjectWrapper
// Created: SLG 2010-06-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateUrbanObject( const urban::TerrainObject_ABC& object )
{
    assert( pObjectManager_ );
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

    xis >> xml::start( "diplomacies" )
            >> xml::list( "side", *this, &MIL_EntityManager::ReadDiplomacy )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::InitializeDotations
// Created: JSR 2010-05-04
// -----------------------------------------------------------------------------
void MIL_EntityManager::InitializeDotations( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing dotations" );

    xis >> xml::optional >> xml::start( "dotations" )
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

    xis >> xml::start( "sides" )
        >> xml::list( "side", boost::bind( &ArmyFactory_ABC::Create, boost::ref( *armyFactory_ ), _1 ) )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateAutomat
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateAutomat( xml::xistream& xis, MIL_Formation& formation )
{
    automateFactory_->Create( xis, formation );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateAutomat
// Created: NLD 2006-10-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateAutomat( xml::xistream& xis, MIL_Automate& parent )
{
    automateFactory_->Create( xis, parent );
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

    if( hla_ )
        hla_->Register( *pPion );

    pPion->SendCreation ();
    pPion->SendFullState();
    pPion->SendKnowledge();
    return *pPion;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
void MIL_EntityManager::CreateObject( xml::xistream& xis, MIL_Army_ABC& army )
{
    //@TODO MGD REMOVE
    assert( pObjectManager_ );
    pObjectManager_->CreateObject( xis, army );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: NLD 2006-10-23
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::CreateObject( MIL_Army_ABC& army, const std::string& type, const TER_Localisation* pLocalisation, ObstacleType_DemolitionTargetType obstacleType )
{
    assert( pObjectManager_ );
    return pObjectManager_->CreateObject( army, type, pLocalisation, obstacleType );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::CreateObject
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
MIL_Object_ABC* MIL_EntityManager::CreateObject( const std::string& type, MIL_Army_ABC& army, const TER_Localisation& localisation )
{
    assert( pObjectManager_ );
    return pObjectManager_->CreateObject( type, army, localisation );
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
    assert( pObjectManager_ );
    return pObjectManager_->Find( nID );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::FindObjectType
// Created: JCR 2008-06-03
// -----------------------------------------------------------------------------
const MIL_ObjectType_ABC& MIL_EntityManager::FindObjectType( const std::string& type ) const
{
    assert( pObjectManager_ );
    return pObjectManager_->FindType( type );
}

// =============================================================================
// UPDATE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::UpdateKnowledges
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_EntityManager::UpdateKnowledges()
{
    profiler_.Start();

    int currentTimeStep =  MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();
    armyFactory_->Apply( boost::bind( &MIL_Army_ABC::UpdateKnowledges,  _1, boost::ref(currentTimeStep) ) );
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
    float duration = (float)MIL_AgentServer::GetWorkspace().GetTimeStepDuration();
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
    assert( pObjectManager_ );

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

    automateFactory_->Apply( boost::bind( &MIL_Automate::UpdateNetwork, _1 ) );
    agentFactory_->Apply( boost::bind( &MIL_AgentPion::UpdateNetwork, _1 ) );
    populationFactory_->Apply( boost::bind( &MIL_Population::UpdateNetwork, _1 ) );

    assert( pObjectManager_ );
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
    UpdateActions   ();
    UpdateEffects   ();
    UpdateStates    ();
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

// =============================================================================
// NETWORK
// =============================================================================

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
// Name: MIL_EntityManager::OnReceiveMsgUnitOrder
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgUnitOrder( const Common::MsgUnitOrder& message, unsigned int nCtx )
{
    client::UnitOrderAck ack;
    ack().mutable_tasker()->set_id( message.tasker().id() );
    ack().set_error_code( MsgsSimToClient::OrderAck_ErrorCode_no_error );
    try
    {
        MIL_AgentPion* pPion = FindAgentPion( message.tasker().id() );
        if( !pPion )
            throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_unit );
        pPion->OnReceiveMsgOrder( message );
    }
    catch( NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgAutomatOrder
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgAutomatOrder( const Common::MsgAutomatOrder& message, unsigned int nCtx )
{
    client::AutomatOrderAck ack;
    ack().mutable_tasker()->set_id( message.tasker().id() );
    ack().set_error_code( MsgsSimToClient::OrderAck_ErrorCode_no_error );
    try
    {
        MIL_Automate* pAutomate = FindAutomate( message.tasker().id() );
        if( !pAutomate )
            throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_unit );
        pAutomate->OnReceiveMsgOrder( message );
    }
    catch( NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgUnitMagicAction
// Created: JSR 2010-04-13
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgUnitMagicAction( const MsgsClientToSim::MsgUnitMagicAction& message, unsigned int nCtx )
{
    client::UnitMagicActionAck ack;
    ack().mutable_id()->set_id( message.id().id() );
    ack().set_error_code( MsgsSimToClient::UnitActionAck_ErrorCode_no_error );
    try
    {
        switch( message.type() )
        {
        case MsgsClientToSim::MsgUnitMagicAction_Type_move_to :
            ProcessMsgMagicActionMoveTo( message, nCtx );
            break;
        case MsgsClientToSim::MsgUnitMagicAction_Type_unit_creation :
            if( MIL_Automate*  pAutomate = FindAutomate ( message.id().id() ) )
                pAutomate->OnReceiveMsgUnitCreationRequest( message );
            else
                throw NET_AsnException< MsgsSimToClient::UnitActionAck_ErrorCode >( MsgsSimToClient::UnitActionAck_ErrorCode_error_invalid_unit );
            break;
        case MsgsClientToSim::MsgUnitMagicAction_Type_population_total_destruction:
        case MsgsClientToSim::MsgUnitMagicAction_Type_population_kill:
        case MsgsClientToSim::MsgUnitMagicAction_Type_population_resurrect:
        case MsgsClientToSim::MsgUnitMagicAction_Type_population_change_attitude:
            if( MIL_Population* pPopulation = pPopulation = populationFactory_->Find ( message.id().id() ) )
                pPopulation->OnReceiveMsgPopulationMagicAction( message );
            else
                throw NET_AsnException< MsgsSimToClient::MsgPopulationMagicActionAck_ErrorCode >( MsgsSimToClient::MsgPopulationMagicActionAck_ErrorCode_error_invalid_unit );
            break;
        case MsgsClientToSim::MsgUnitMagicAction_Type_create_fire_order:
            ProcessMsgMagicActionCreateFireOrder( message, nCtx );
            break;
        case MsgsClientToSim::MsgUnitMagicAction_Type_change_knowledge_group:
            ProcessMsgAutomateChangeKnowledgeGroup( message, nCtx );
            break;
        case MsgsClientToSim::MsgUnitMagicAction_Type_change_logistic_links:
            ProcessMsgAutomateChangeLogisticLinks( message, nCtx );
            break;
        case MsgsClientToSim::MsgUnitMagicAction_Type_unit_change_superior:
            ProcessMsgUnitChangeSuperior( message, nCtx );
            break;
        case MsgsClientToSim::MsgUnitMagicAction_Type_change_automat_superior:
        case MsgsClientToSim::MsgUnitMagicAction_Type_change_formation_superior:
            ProcessMsgAutomateChangeSuperior( message, nCtx );
            break;
        case MsgsClientToSim::MsgUnitMagicAction_Type_log_supply_push_flow:
            ProcessMsgLogSupplyPushFlow( message, nCtx );
            break;
        case MsgsClientToSim::MsgUnitMagicAction_Type_log_supply_change_quotas:
            ProcessMsgLogSupplyChangeQuotas( message, nCtx );
            break;
        default:
            if( MIL_Automate*  pAutomate = FindAutomate ( message.id().id() ) )
                pAutomate->OnReceiveMsgUnitMagicAction( message, *armyFactory_ );
            else if( MIL_AgentPion* pPion = FindAgentPion( message.id().id() ) )
                pPion->OnReceiveMsgUnitMagicAction( message, *armyFactory_ );
            else
                throw NET_AsnException< MsgsSimToClient::UnitActionAck_ErrorCode >( MsgsSimToClient::UnitActionAck_ErrorCode_error_invalid_unit );
            break;
        }
    }
    catch( NET_AsnException< MsgsSimToClient::UnitActionAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgKnowledgeMagicAction
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgKnowledgeMagicAction( const MsgsClientToSim::MsgKnowledgeMagicAction& message, unsigned int nCtx )
{
    client::KnowledgeGroupMagicActionAck ack;
    ack().mutable_id()->set_id( message.id().id() );
    ack().set_error_code( MsgsSimToClient::KnowledgeGroupAck_ErrorCode_no_error );
    try
    {
        switch( message.type() )
        {
        case MsgsClientToSim::MsgKnowledgeMagicAction_Type_enable :
        case MsgsClientToSim::MsgKnowledgeMagicAction_Type_update_side :
        case MsgsClientToSim::MsgKnowledgeMagicAction_Type_update_side_parent :
        case MsgsClientToSim::MsgKnowledgeMagicAction_Type_update_type :
            ProcessMsgKnowledgeGroupUpdate( message, nCtx );
            break;
        default:
            throw NET_AsnException< MsgsSimToClient::KnowledgeGroupAck_ErrorCode >( MsgsSimToClient::KnowledgeGroupAck_ErrorCode_error_invalid_type );
            break;
        }
    }
    catch( NET_AsnException< MsgsSimToClient::KnowledgeGroupAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgPopulationOrder
// Created: NLD 2005-09-29
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgPopulationOrder( const Common::MsgPopulationOrder& message, unsigned int nCtx )
{
    client::PopulationOrderAck ack;
    ack().mutable_tasker()->set_id( message.tasker().id() );
    ack().set_error_code( MsgsSimToClient::OrderAck_ErrorCode_no_error );
    try
    {
        MIL_Population* pPopulation = populationFactory_->Find( message.tasker().id() );
        if( !pPopulation )
            throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_unit );
        pPopulation->OnReceiveMsgOrder( message );
    }
    catch( NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgFragOrder
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgFragOrder( const MsgsClientToSim::MsgFragOrder& message, unsigned int nCtx )
{
    client::FragOrderAck ack;
    unsigned int taskerId ( TaskerToId( message.tasker() ) );

    ack().set_error_code( MsgsSimToClient::OrderAck_ErrorCode_no_error );

    try
    {
        if( MIL_Automate* pAutomate = FindAutomate  ( taskerId ) )
        {
            pAutomate->OnReceiveMsgFragOrder( message );
            ack().mutable_tasker()->mutable_automat()->set_id( taskerId );
        }
        else if( MIL_Population* pPopulation = populationFactory_->Find( taskerId ) )
        {
            pPopulation->OnReceiveMsgFragOrder( message );
            ack().mutable_tasker()->mutable_population()->set_id( taskerId );
        }
        else if( MIL_AgentPion* pPion = FindAgentPion ( taskerId ) )
        {
            pPion->OnReceiveMsgFragOrder( message );
            ack().mutable_tasker()->mutable_unit()->set_id( taskerId );
        }
        else
            throw NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >( MsgsSimToClient::OrderAck_ErrorCode_error_invalid_unit );
    }
    catch( NET_AsnException< MsgsSimToClient::OrderAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgSetAutomateMode
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgSetAutomateMode( const MsgsClientToSim::MsgSetAutomatMode& message, unsigned int nCtx )
{
    client::SetAutomatModeAck ack;
    ack().mutable_id()->set_id( message.automate().id() );
    ack().set_error_code( MsgsSimToClient::MsgSetAutomatModeAck_ErrorCode_no_error );
    try
    {
        MIL_Automate* pAutomate = FindAutomate( message.automate().id() );
        if( !pAutomate )
            throw NET_AsnException< MsgsSimToClient::MsgSetAutomatModeAck_ErrorCode >( MsgsSimToClient::MsgSetAutomatModeAck_ErrorCode_error_invalid_unit );
        pAutomate->OnReceiveMsgSetAutomateMode( message );
    }
    catch( NET_AsnException< MsgsSimToClient::MsgSetAutomatModeAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgUnitCreationRequest
// Created: AGE 2007-06-18
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgUnitCreationRequest( const MsgsClientToSim::MsgUnitCreationRequest& message, unsigned int nCtx )
{
    client::UnitCreationRequestAck ack;
    ack().set_error( MsgsSimToClient::UnitActionAck_ErrorCode_no_error );
    try
    {
        MIL_Automate* pAutomate = FindAutomate( message.superior().id() );
        if( !pAutomate )
            throw NET_AsnException< MsgsSimToClient::UnitActionAck_ErrorCode >( MsgsSimToClient::UnitActionAck_ErrorCode_error_invalid_unit );
        pAutomate->OnReceiveMsgUnitCreationRequest( message );
    }
    catch( NET_AsnException< MsgsSimToClient::UnitActionAck_ErrorCode >& e )
    {
        ack().set_error( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgObjectMagicAction
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgObjectMagicAction( const MsgsClientToSim::MsgObjectMagicAction& message, unsigned int nCtx )
{
    assert( pObjectManager_ );
    pObjectManager_->OnReceiveMsgObjectMagicAction( message, nCtx, *armyFactory_ );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgChangeDiplomacy
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgChangeDiplomacy( const MsgMagicAction& message, unsigned int nCtx )
{
    client::ChangeDiplomacyAck ack;
    ack().mutable_party1()->set_id( message.parametres().elem( 0 ).value().identifier() );
    ack().mutable_party2()->set_id( message.parametres().elem( 1 ).value().identifier() );
    ack().set_diplomatie( ( Common::EnumDiplomacy ) message.parametres().elem( 2 ).value().enumeration() );
    ack().set_error_code( MsgsSimToClient::MsgChangeDiplomacyAck_EnumChangeDiplomacyErrorCode_no_error_diplomacy );
    try
    {
        MIL_Army_ABC* pArmy1 = armyFactory_->Find( message.parametres().elem( 0 ).value().identifier() );
        if( !pArmy1 )
            throw NET_AsnException< MsgsSimToClient::MsgChangeDiplomacyAck_EnumChangeDiplomacyErrorCode >( MsgsSimToClient::MsgChangeDiplomacyAck_EnumChangeDiplomacyErrorCode_error_invalid_camp_diplomacy );
        pArmy1->OnReceiveMsgChangeDiplomacy( message.parametres() );
    }
    catch( NET_AsnException< MsgsSimToClient::MsgChangeDiplomacyAck_EnumChangeDiplomacyErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgChangeResourceLinks
// Created: JSR 2010-08-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgChangeResourceLinks( const MsgsClientToSim::MsgMagicAction& message, unsigned int nCtx )
{
    assert( pObjectManager_ );
    pObjectManager_->OnReceiveMsgChangeResourceLinks( message, nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessMsgAutomateChangeKnowledgeGroup
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessMsgAutomateChangeKnowledgeGroup( const MsgsClientToSim::MsgUnitMagicAction& message, unsigned int nCtx )
{
    client::AutomatChangeKnowledgeGroupAck ack;
    ack().set_error_code( MsgsSimToClient::HierarchyModificationAck_ErrorCode_no_error_hierarchy );
    try
    {
        MIL_Automate* pAutomate = FindAutomate( message.id().id() );
        if( !pAutomate )
            throw NET_AsnException< MsgsSimToClient::HierarchyModificationAck_ErrorCode >( MsgsSimToClient::HierarchyModificationAck_ErrorCode_error_invalid_automate );
        pAutomate->OnReceiveMsgChangeKnowledgeGroup( message, *armyFactory_ );
    }
    catch( NET_AsnException< MsgsSimToClient::HierarchyModificationAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );

    if( ack().error_code() == MsgsSimToClient::HierarchyModificationAck_ErrorCode_no_error_hierarchy )
    {
        if( message.has_parametres() &&
            message.parametres().elem_size() == 2 )
        {
            client::AutomatChangeKnowledgeGroup resendMessage;
            resendMessage().mutable_automat()->set_id( message.id().id() );
            if( message.parametres().elem( 0 ).has_value() && message.parametres().elem( 0 ).value().has_knowledgegroup() )
                resendMessage().mutable_knowledge_group()->set_id( message.parametres().elem( 0 ).value().knowledgegroup().id() );
            if( message.parametres().elem( 1 ).has_value() && message.parametres().elem( 1 ).value().has_party() )
                resendMessage().mutable_party()->set_id( message.parametres().elem( 1 ).value().party().id() );
            resendMessage.Send( NET_Publisher_ABC::Publisher() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessMsgAutomateChangeLogisticLinks
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessMsgAutomateChangeLogisticLinks( const MsgsClientToSim::MsgUnitMagicAction& message, unsigned int nCtx )
{
    client::AutomatChangeLogisticLinksAck ack;
    ack().set_error_code( MsgsSimToClient::HierarchyModificationAck_ErrorCode_no_error_hierarchy );
    try
    {
        MIL_Automate* pAutomate = FindAutomate( message.id().id() );
        if( !pAutomate )
            throw NET_AsnException< MsgsSimToClient::HierarchyModificationAck_ErrorCode >( MsgsSimToClient::HierarchyModificationAck_ErrorCode_error_invalid_automate );
        pAutomate->OnReceiveMsgChangeLogisticLinks( message );
    }
    catch( NET_AsnException< MsgsSimToClient::HierarchyModificationAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );

    if( ack().error_code() == MsgsSimToClient::HierarchyModificationAck_ErrorCode_no_error_hierarchy )
    {
        if( message.has_parametres() &&
            message.parametres().elem_size() == 4 )
        {
            client::AutomatChangeLogisticLinks resendMessage;
            resendMessage().mutable_automat()->set_id( message.id().id() );
            const Common::MsgMissionParameter& tc2 = message.parametres().elem( 0 );
            const Common::MsgMissionParameter& maintenance = message.parametres().elem( 1 );
            const Common::MsgMissionParameter& sante = message.parametres().elem( 2 );
            const Common::MsgMissionParameter& supply = message.parametres().elem( 3 );

            if( tc2.has_value() && tc2.value().has_identifier() && tc2.value().identifier() != 0 && tc2.value().identifier() != (unsigned int) -1 )
                resendMessage().mutable_tc2()->set_id( tc2.value().identifier() );
            if( maintenance.has_value() && maintenance.value().has_identifier() && maintenance.value().identifier() != 0 && maintenance.value().identifier() != (unsigned int) -1 )
                resendMessage().mutable_maintenance()->set_id( maintenance.value().identifier() );
            if( sante.has_value() && sante.value().has_identifier() && sante.value().identifier() != 0 && sante.value().identifier() != (unsigned int) -1 )
                resendMessage().mutable_health()->set_id( sante.value().identifier() );
            if( supply.has_value() && supply.value().has_identifier() && supply.value().identifier() != 0 && supply.value().identifier() != (unsigned int) -1 )
                resendMessage().mutable_supply()->set_id( supply.value().identifier() );
            resendMessage.Send( NET_Publisher_ABC::Publisher() );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessMsgAutomateChangeSuperior
// Created: NLD 2007-04-11
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessMsgAutomateChangeSuperior( const MsgsClientToSim::MsgUnitMagicAction& message, unsigned int nCtx )
{
    client::AutomatChangeSuperiorAck ack;
    ack().set_error_code( MsgsSimToClient::HierarchyModificationAck_ErrorCode_no_error_hierarchy );
    try
    {
        MIL_Automate* pAutomate = FindAutomate( message.id().id() );
        if( !pAutomate )
            throw NET_AsnException< MsgsSimToClient::HierarchyModificationAck_ErrorCode >( MsgsSimToClient::HierarchyModificationAck_ErrorCode_error_invalid_automate );
        pAutomate->OnReceiveMsgChangeSuperior( message, *formationFactory_ );
    }
    catch( NET_AsnException< MsgsSimToClient::HierarchyModificationAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );

    if( ack().error_code() == MsgsSimToClient::HierarchyModificationAck_ErrorCode_no_error_hierarchy )
    {
        client::AutomatChangeSuperior resendMessage;
        resendMessage().mutable_automat()->set_id( message.id().id() );
        if( message.type() == MsgsClientToSim::MsgUnitMagicAction_Type_change_formation_superior )
            resendMessage().mutable_superior()->mutable_formation()->set_id( message.parametres().elem( 0 ).value().formation().id() );
        else if( message.type() == MsgsClientToSim::MsgUnitMagicAction_Type_change_automat_superior )
            resendMessage().mutable_superior()->mutable_automat()->set_id( message.parametres().elem( 0 ).value().automat().id() );
        resendMessage.Send( NET_Publisher_ABC::Publisher() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessMsgUnitChangeSuperior
// Created: NLD 2004-10-25
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessMsgUnitChangeSuperior( const MsgsClientToSim::MsgUnitMagicAction& message, unsigned int nCtx )
{
    client::UnitChangeSuperiorAck ack;
    ack().set_error_code( MsgsSimToClient::HierarchyModificationAck_ErrorCode_no_error_hierarchy );
    try
    {
        MIL_AgentPion* pPion = FindAgentPion( message.id().id() );
        if( !pPion )
            throw NET_AsnException< MsgsSimToClient::HierarchyModificationAck_ErrorCode >( MsgsSimToClient::HierarchyModificationAck_ErrorCode_error_invalid_pion );
        pPion->OnReceiveMsgChangeSuperior( *this, message );
    }
    catch( NET_AsnException< MsgsSimToClient::HierarchyModificationAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
    if( ack().error_code() == MsgsSimToClient::HierarchyModificationAck_ErrorCode_no_error_hierarchy )
    {
        client::UnitChangeSuperior resendMessage;
        resendMessage().mutable_unit()->set_id ( message.id().id() );
        resendMessage().mutable_parent()->set_id ( message.parametres().elem( 0 ).value().automat().id() );
        resendMessage.Send( NET_Publisher_ABC::Publisher() );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessMsgLogSupplyChangeQuotas
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessMsgLogSupplyChangeQuotas( const MsgsClientToSim::MsgUnitMagicAction& message, unsigned int nCtx )
{
    client::LogSupplyChangeQuotasAck ack;
    ack().set_ack( MsgsSimToClient::MsgLogSupplyChangeQuotasAck_LogSupplyChangeQuotas_no_error_quotas );
    try
    {
        MIL_Automate* pReceiver = FindAutomate( message.id().id() );
        if( !pReceiver )
            throw NET_AsnException< MsgsSimToClient::MsgLogSupplyChangeQuotasAck_LogSupplyChangeQuotas >( MsgsSimToClient::MsgLogSupplyChangeQuotasAck_LogSupplyChangeQuotas_error_invalid_receveur_quotas );
        pReceiver->OnReceiveMsgLogSupplyChangeQuotas( message.parametres() );
    }
    catch( NET_AsnException< MsgsSimToClient::MsgLogSupplyChangeQuotasAck_LogSupplyChangeQuotas >& e )
    {
        ack().set_ack( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessMsgLogSupplyPushFlow
// Created: NLD 2005-02-03
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessMsgLogSupplyPushFlow( const MsgsClientToSim::MsgUnitMagicAction& message, unsigned int nCtx )
{
    client::LogSupplyPushFlowAck ack;
    ack().set_ack( MsgsSimToClient::MsgLogSupplyPushFlowAck_EnumLogSupplyPushFlow_no_error_pushflow );
    try
    {
        MIL_Automate* pReceiver = FindAutomate( message.id().id() );
        if( !pReceiver )
            throw NET_AsnException< MsgsSimToClient::MsgLogSupplyPushFlowAck_EnumLogSupplyPushFlow >( MsgsSimToClient::MsgLogSupplyPushFlowAck_EnumLogSupplyPushFlow_error_invalid_receveur_pushflow );
        pReceiver->OnReceiveMsgLogSupplyPushFlow( message.parametres() );
    }
    catch( NET_AsnException< MsgsSimToClient::MsgLogSupplyPushFlowAck_EnumLogSupplyPushFlow >& e )
    {
        ack().set_ack( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessMsgMagicActionMoveTo
// Created: JSR 2010-04-07
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessMsgMagicActionMoveTo( const MsgsClientToSim::MsgUnitMagicAction& message, unsigned int )
{
    if( MIL_Automate*  pAutomate = FindAutomate ( message.id().id() ) )
        pAutomate->OnReceiveMsgMagicActionMoveTo( message );
    else if( MIL_AgentPion* pPion = FindAgentPion( message.id().id() ) )
        pPion->OnReceiveMsgMagicActionMoveTo( message );
    else if( MIL_Population* pPopulation = populationFactory_->Find( message.id().id() ) )
        pPopulation->OnReceiveMsgPopulationMagicActionMoveTo( message );
    else
        throw NET_AsnException< MsgsSimToClient::UnitActionAck_ErrorCode >( MsgsSimToClient::UnitActionAck_ErrorCode_error_invalid_unit );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::OnReceiveMsgKnowledgeGroupCreation
// Created: FHD 2009-12-15:
// LTO
// -----------------------------------------------------------------------------
void MIL_EntityManager::OnReceiveMsgKnowledgeGroupCreation( const MsgsClientToSim::MsgMagicAction& /*message*/, unsigned int nCtx )
{
    client::KnowledgeGroupCreationAck ack;
    ack().mutable_id()->set_id( 0 );
    ack().set_error_code( MsgsSimToClient::KnowledgeGroupAck_ErrorCode_no_error );
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}


// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessMsgKnowledgeGroupUpdate
// Created: FDS 2010-01-13
// LTO
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessMsgKnowledgeGroupUpdate( const MsgsClientToSim::MsgKnowledgeMagicAction& message, unsigned int nCtx )
{
    client::KnowledgeGroupUpdateAck ack;
    ack().mutable_id()->set_id( message.id().id() );
    ack().set_error_code( MsgsSimToClient::KnowledgeGroupAck_ErrorCode_no_error );

    try
    {
        MIL_KnowledgeGroup* pReceiver = FindKnowledgeGroup( message.id().id() );
        if( !pReceiver || pReceiver->IsJammed() )
            throw NET_AsnException< MsgsSimToClient::KnowledgeGroupAck_ErrorCode >( MsgsSimToClient::KnowledgeGroupAck_ErrorCode_error_invalid_type );

        pReceiver->OnReceiveMsgKnowledgeGroupUpdate( message, *armyFactory_  );
    }
    catch( NET_AsnException< MsgsSimToClient::KnowledgeGroupAck_ErrorCode >& e )
    {
        ack().set_error_code( e.GetErrorID() );
    }
    ack.Send( NET_Publisher_ABC::Publisher(), nCtx );
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::ProcessMsgMagicActionCreateFireOrder
// Created: MGD 2010-02-24
// LTO
// -----------------------------------------------------------------------------
void MIL_EntityManager::ProcessMsgMagicActionCreateFireOrder( const MsgsClientToSim::MsgUnitMagicAction& msg, unsigned int nCtx )
{
    client::ActionCreateFireOrderAck  ack;
    ack().set_error_code( MsgsSimToClient::MsgActionCreateFireOrderAck_EnumActionCreateFireOrderErrorCode_no_error );
    try
    {
        if( !msg.has_parametres() || msg.parametres().elem_size() != 3)
            throw NET_AsnException< MsgsSimToClient::MsgActionCreateFireOrderAck_EnumActionCreateFireOrderErrorCode >( MsgsSimToClient::MsgActionCreateFireOrderAck_EnumActionCreateFireOrderErrorCode_error_invalid_target );

        MIL_Agent_ABC* reporter = FindAgentPion( msg.id().id() );
        if( !reporter )
            throw NET_AsnException< MsgsSimToClient::MsgActionCreateFireOrderAck_EnumActionCreateFireOrderErrorCode >( MsgsSimToClient::MsgActionCreateFireOrderAck_EnumActionCreateFireOrderErrorCode_error_invalid_reporter );

        // Target
        const Common::MsgMissionParameter& target = msg.parametres().elem( 0 );
        if( !target.has_value() || !target.value().has_identifier() )
            throw NET_AsnException< MsgsSimToClient::MsgPopulationMagicActionAck_ErrorCode >( MsgsSimToClient::MsgPopulationMagicActionAck_ErrorCode_error_invalid_attribute );

        boost::shared_ptr< DEC_Knowledge_Agent > targetKn = reporter->GetKnowledge().ResolveKnowledgeAgent( target.value().identifier() );
        if( !targetKn )
            throw NET_AsnException< MsgsSimToClient::MsgActionCreateFireOrderAck_EnumActionCreateFireOrderErrorCode >( MsgsSimToClient::MsgActionCreateFireOrderAck_EnumActionCreateFireOrderErrorCode_error_invalid_target );

        // Ammo
        const Common::MsgMissionParameter& ammo = msg.parametres().elem( 1 );
        if( !ammo.has_value() || !ammo.value().has_resourcetype() )
            throw NET_AsnException< MsgsSimToClient::MsgPopulationMagicActionAck_ErrorCode >( MsgsSimToClient::MsgPopulationMagicActionAck_ErrorCode_error_invalid_attribute );

        const PHY_DotationCategory* pDotationCategory = PHY_DotationType::FindDotationCategory( ammo.value().resourcetype().id() );
        if( !pDotationCategory )
            throw NET_AsnException< MsgsSimToClient::MsgActionCreateFireOrderAck_EnumActionCreateFireOrderErrorCode >( MsgsSimToClient::MsgActionCreateFireOrderAck_EnumActionCreateFireOrderErrorCode_error_invalid_munition );

        if( pDotationCategory->IsGuided() && !targetKn->GetAgentKnown().GetRole< PHY_RoleInterface_Illumination >().IsIlluminated() )
            throw NET_AsnException< MsgsSimToClient::MsgActionCreateFireOrderAck_EnumActionCreateFireOrderErrorCode >( MsgsSimToClient::MsgActionCreateFireOrderAck_EnumActionCreateFireOrderErrorCode_error_target_no_illuminated );

        // Iterations
        const Common::MsgMissionParameter& iterations = msg.parametres().elem( 2 );
        if( !iterations.has_value() || !iterations.value().has_areal() )
            throw NET_AsnException< MsgsSimToClient::MsgPopulationMagicActionAck_ErrorCode >( MsgsSimToClient::MsgPopulationMagicActionAck_ErrorCode_error_invalid_attribute );

        PHY_FireResults_Pion fireResult( *reporter , targetKn->GetPosition(), *pDotationCategory );
        unsigned int ammos = (unsigned int) pDotationCategory->GetIndirectFireData()->ConvertToNbrAmmo( iterations.value().areal() );

        MIL_Report::PostEvent( *reporter, MIL_Report::eReport_IndirectFireOnTarget, targetKn );

        pDotationCategory->ApplyIndirectFireEffect( *reporter, targetKn->GetAgentKnown(), ammos , fireResult );
    }
    catch( NET_AsnException< MsgsSimToClient::MsgActionCreateFireOrderAck_EnumActionCreateFireOrderErrorCode >& e )
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
    // $$$$ NLD 2007-01-11: A voir
    delete pObjectManager_;
    pObjectManager_ = 0;

    ArmyFactory_ABC * armyFactory;
    FormationFactory_ABC * formationFactory;
    AutomateFactory_ABC * automateFactory;
    AgentFactory_ABC * agentFactory;
    PopulationFactory_ABC * populationFactory;
    KnowledgeGroupFactory_ABC * knowledgeGroupFactory; // LTO
    file //>> effectManager_  // Effets liés aux actions qui ne sont pas sauvegardés
         >> knowledgeGroupFactory; // LTO
    knowledgeGroupFactory_.reset( knowledgeGroupFactory );
    file >> armyFactory
         >> formationFactory//@TODO MGD serialize
         >> agentFactory
         >> automateFactory
         >> populationFactory
         >> pObjectManager_
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

    MT_LOG_INFO_MSG( MT_FormatString( " => %d automates"  , automateFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d pions"      , agentFactory_->Count() ) );
    MT_LOG_INFO_MSG( MT_FormatString( " => %d populations", populationFactory_->Count() ) );
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
    const KnowledgeGroupFactory_ABC* const knowledgeGroupFactory = knowledgeGroupFactory_.get();

    file //<< effectManager_  // Effets liés aux actions qui ne sont pas sauvegardés
         << knowledgeGroupFactory; // LTO
    file << tempArmy
         << tempFormationFactory
         << tempAgentFactory
         << tempAutomateFactory
         << populationFactory
         << pObjectManager_
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
    xos << xml::start( "orbat" )
            << xml::start( "dotations" )
                << xml::attribute( "infinite", infiniteDotations_ )
            << xml::end
            << xml::start( "sides" );
                armyFactory_->Apply( boost::bind( &MIL_Army_ABC::WriteODB, _1, boost::ref( xos ) ) );
    xos     << xml::end
            << xml::start( "diplomacies" );
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
// Name: MIL_EntityManager::GetKnowledgesTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetKnowledgesTime() const
{
    return rKnowledgesTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetDecisionsTime
// Created: NLD 2006-05-03
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetDecisionsTime() const
{
    return rAutomatesDecisionTime_ + rPionsDecisionTime_ + rPopulationsDecisionTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetAutomatesDecisionTime
// Created: NLD 2006-05-03
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetAutomatesDecisionTime() const
{
    return rAutomatesDecisionTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetPionsDecisionTime
// Created: NLD 2006-05-03
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetPionsDecisionTime() const
{
    return rPionsDecisionTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetPopulationsDecisionTime
// Created: NLD 2006-05-03
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetPopulationsDecisionTime() const
{
    return rPopulationsDecisionTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetActionsTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetActionsTime() const
{
    return rActionsTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetEffectsTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetEffectsTime() const
{
    return rEffectsTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_EntityManager::GetStatesTime
// Created: NLD 2004-10-08
// -----------------------------------------------------------------------------
MT_Float MIL_EntityManager::GetStatesTime() const
{
    return rStatesTime_;
}

// -----------------------------------------------------------------------------
// Name: Model::setToTasker
// Created: PHC 2010-07-07
// -----------------------------------------------------------------------------
void MIL_EntityManager::SetToTasker( Common::Tasker& tasker, unsigned int id ) const
{
    if( FindAutomate( id ) )
        tasker.mutable_automat()->set_id( id );
    else if( FindFormation( id ) )
        tasker.mutable_formation()->set_id( id );
    else if( FindPopulation( id ) )
        tasker.mutable_population()->set_id( id );
    else if( FindAgentPion( id ) )
        tasker.mutable_unit()->set_id( id );
    else throw( std::exception( "Misformed tasker in protocol message" ) );
}