// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "Sink.h"

#include "AlatMonitoringEventListener.h"
#include "CallbackEventListener.h"
#include "CheckPoints/SerializationTools.h"
#include "CoreLogger.h"
#include "DirectFirePerceptionEventListener.h"
#include "DirectFirePionAttackEventListener.h"
#include "DirectFirePionEventListener.h"
#include "DirectFirePopulationAttackEventListener.h"
#include "DirectFirePopulationEventListener.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"
#include "Entities/Agents/Roles/Deployment/PHY_RoleInterface_Deployment.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/Objects/MIL_ObjectManager.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "ExternalPerceptionEventListener.h"
#include "FireHooks.h"
#include "FloodModel.h"
#include "FlyingShellPerceptionEventListener.h"
#include "Hook.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeObject.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_AgentComposante.h"
#include "Knowledge/DEC_Knowledge_Object.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "MIL_AgentServer.h"
#include "ModelCounter.h"
#include "MovementEventListener.h"
#include "MovementHooks.h"
#include "MovementReportEventListener.h"
#include "MovementReportNameEventListener.h"
#include "MT_Tools/MT_FormatString.h"
#include "PerceptionCallbackEventListener.h"
#include "PerceptionHooks.h"
#include "PopulationFactory_ABC.h"
#include "ReportEventListener.h"
#include "RoleAction_Moving.h"
#include "RoleAdapter.h"
#include "RolePion_Composantes.h"
#include "RolePion_Decision.h"
#include "RolePion_Location.h"
#include "RolePion_Perceiver.h"
#include "SinkRoleExtender.h"
#include "tools/Loader_ABC.h"
#include "Tools/MIL_Tools.h"
#include "Urban/MIL_UrbanCache.h"
#include "Urban/MIL_UrbanObject_ABC.h"

#include <core/Convert.h>
#include <boost/foreach.hpp>
#include <boost/functional/factory.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>

using namespace sword;

BOOST_CLASS_EXPORT_IMPLEMENT( sword::Sink )

namespace sword
{
template< typename Archive >
void save_construct_data( Archive& archive, const Sink* sink, const unsigned int /*version*/ )
{
    const AgentFactory_ABC* agents = &sink->agents_;
    const PopulationFactory_ABC* populations = &sink->populations_;
    const core::Model* model = sink->model_.get();
    archive << agents
            << populations
            << model
            << sink->gcPause_
            << sink->gcMult_
            << sink->decLogger_
            << sink->dangerousObjects_
            << sink->elements_;
}

template< typename Archive >
void load_construct_data( Archive& archive, Sink* sink, const unsigned int /*version*/ )
{
    AgentFactory_ABC* agents;
    PopulationFactory_ABC* populations;
    core::Model* model;
    unsigned int gcPause;
    unsigned int gcMult;
    std::auto_ptr< sword::DEC_Logger > decLogger;
    std::vector< unsigned int > dangerousObjects;
    archive >> agents
            >> populations
            >> model
            >> gcPause
            >> gcMult
            >> decLogger
            >> dangerousObjects;
    ::new( sink )Sink( *agents, *populations, std::auto_ptr< core::Model >( model ), gcPause, gcMult, decLogger, dangerousObjects );
    archive >> sink->elements_;
}
}

namespace
{
    DECLARE_HOOK( InitializePathClass, void, ( const char* xml, const unsigned int* first, size_t size ) )
    DECLARE_HOOK( InitializePerceptionTypes, void, ( const char* xml ) )
    DECLARE_HOOK( InitializeEquipments, void, ( const char* xml, SWORD_Model* model ) )
    DECLARE_HOOK( InitializeLaunchers, void, ( const char* xml ) )
    DECLARE_HOOK( InitializeDotations, void, ( const char* xml ) )
    DECLARE_HOOK( InitializeWeaponSystems, void, ( const char* xml, double tickDuration ) )
    DECLARE_HOOK( InitializeDecisional, void, ( const char* xml, double tickDuration ) )

    void InitializePathfinder( xml::xistream& xis, const std::vector< unsigned int >& dangerousObjects )
    {
        xml::xostringstream xos;
        xos << xis;
        GET_HOOK( InitializePathClass )( xos.str().c_str(), dangerousObjects.empty() ? 0 : &dangerousObjects[0], dangerousObjects.size() );
    }
    void InitializeEquipments( xml::xistream& xis, core::Model& model )
    {
        xml::xostringstream xos;
        xos << xis;
        GET_HOOK( InitializeEquipments )( xos.str().c_str(), core::Convert( &model ) ) ;
    }
    void InitializeLaunchers( xml::xistream& xis )
    {
        xml::xostringstream xos;
        xos << xis;
        GET_HOOK( InitializeLaunchers )( xos.str().c_str() ) ;
    }
    void InitializePerception( xml::xistream& xis )
    {
        xml::xostringstream xos;
        xos << xis;
        GET_HOOK( InitializePerceptionTypes )( xos.str().c_str() );
    }
    void InitializeDotations( xml::xistream& xis )
    {
        xml::xostringstream xos;
        xos << xis;
        GET_HOOK( InitializeDotations )( xos.str().c_str() ) ;
    }
    void InitializeWeaponSystems( xml::xistream& xis )
    {
        xml::xostringstream xos;
        xos << xis;
        GET_HOOK( InitializeWeaponSystems )( xos.str().c_str(), MIL_Time_ABC::GetTime().GetTickDuration() ) ;
    }
    void InitializeDecisional( xml::xistream& xis )
    {
        xml::xostringstream xos;
        xos << xis;
        GET_HOOK( InitializeDecisional )( xos.str().c_str(), MIL_Time_ABC::GetTime().GetTickDuration() ) ;
    }
    // $$$$ _RC_ SLI 2012-09-11: hard coded
    const std::string configuration = "<core>"
                                      "  <modules>"
                                      "    <module>movement_module</module>"
                                      "    <module>perception_module</module>"
                                      "    <module>fire_module</module>"
                                      "    <module>propagation_module</module>"
                                      "  </modules>"
                                      "  <phases default='actions'>"
                                      "    <phase name='perceptions'>"
                                      "      <command name='perception'/>"
                                      "    </phase>"
                                      "    <phase name='actions'/>"
                                      "  </phases>"
                                      "</core>";
    core::Model& LoadConfig( core::Model& model )
    {
        model[ "profiling/hook" ] = MIL_AgentServer::GetWorkspace().GetConfig().IsHookProfilingEnabled();
        model[ "profiling/command" ] = MIL_AgentServer::GetWorkspace().GetConfig().IsCommandProfilingEnabled();
        return model;
    }
}

// -----------------------------------------------------------------------------
// Name: Sink constructor
// Created: SBO 2011-06-06
// -----------------------------------------------------------------------------
Sink::Sink( AgentFactory_ABC& agents, const PopulationFactory_ABC& populations,
            unsigned int gcPause, unsigned int gcMult, bool logEnabled,
            const std::vector< unsigned int >& dangerousObjects )
    : agents_          ( agents )
    , populations_     ( populations )
    , gcPause_         ( gcPause )
    , gcMult_          ( gcMult )
    , decLogger_       ( logEnabled ? new DEC_Logger( "Pion" ) : 0 )
    , dangerousObjects_( dangerousObjects )
    , modules_         ( configuration )
    , logger_          ( new CoreLogger() )
    , model_           ( new core::Model() )
    , facade_          ( new core::Facade( modules_, *logger_, LoadConfig( *model_ ) ) )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: Sink constructor
// Created: MCO 2012-09-12
// -----------------------------------------------------------------------------
Sink::Sink( AgentFactory_ABC& agents, const PopulationFactory_ABC& populations,
            std::auto_ptr< core::Model > model, unsigned int gcPause, unsigned int gcMult,
            std::auto_ptr< sword::DEC_Logger > decLogger, const std::vector< unsigned int >& dangerousObjects )
    : agents_          ( agents )
    , populations_     ( populations )
    , gcPause_         ( gcPause )
    , gcMult_          ( gcMult )
    , decLogger_       ( decLogger )
    , dangerousObjects_( dangerousObjects )
    , modules_         ( configuration )
    , logger_          ( new CoreLogger() )
    , model_           ( model )
    , facade_          ( new core::Facade( modules_, *logger_, LoadConfig( *model_ ) ) )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: Sink destructor
// Created: SBO 2011-06-06
// -----------------------------------------------------------------------------
Sink::~Sink()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Sink::Initialize
// Created: MCO 2012-09-12
// -----------------------------------------------------------------------------
void Sink::Initialize()
{
    listeners_.push_back( new MovementEventListener( *model_, *facade_ ) );
    listeners_.push_back( new MovementReportEventListener( *facade_ ) );
    listeners_.push_back( new MovementReportNameEventListener( *facade_ ) );
    listeners_.push_back( new CallbackEventListener( *model_, *facade_, "movement callback" ) );
    listeners_.push_back( new PerceptionCallbackEventListener( *facade_ ) );
    listeners_.push_back( new ExternalPerceptionEventListener( *facade_ ) );
    listeners_.push_back( new AlatMonitoringEventListener( *facade_ ) );
    listeners_.push_back( new ReportEventListener( *facade_ ) );
    listeners_.push_back( new DirectFirePionEventListener( *facade_ ) );
    listeners_.push_back( new DirectFirePerceptionEventListener( *facade_ ) );
    listeners_.push_back( new DirectFirePionAttackEventListener( *facade_ ) );
    listeners_.push_back( new DirectFirePopulationAttackEventListener( *facade_ ) );
    listeners_.push_back( new DirectFirePopulationEventListener( *facade_ ) );
    listeners_.push_back( new CallbackEventListener( *model_, *facade_, "direct fire pion callback" ) );
    listeners_.push_back( new CallbackEventListener( *model_, *facade_, "direct fire population callback" ) );
    listeners_.push_back( new FlyingShellPerceptionEventListener( *facade_ ) );
    FireHooks::Initialize();
    PerceptionHooks::Initialize();
    MovementHooks::Initialize();
    Hooks::Initialize( *facade_, (*model_)[ "profiling/hook" ] );
    facade_->Resolve();
    MIL_AgentServer::GetWorkspace().GetConfig().GetLoader().LoadPhysicalFile( "pathfinder", boost::bind( &::InitializePathfinder, _1, boost::cref( dangerousObjects_ ) ) );
    MIL_AgentServer::GetWorkspace().GetConfig().GetLoader().LoadPhysicalFile( "sensors", boost::bind( &::InitializePerception, _1 ) );
    MIL_AgentServer::GetWorkspace().GetConfig().GetLoader().LoadPhysicalFile( "components", boost::bind( &::InitializeEquipments, _1, boost::ref( *model_ ) ) );
    MIL_AgentServer::GetWorkspace().GetConfig().GetLoader().LoadPhysicalFile( "launchers", boost::bind( &::InitializeLaunchers, _1 ) );
    MIL_AgentServer::GetWorkspace().GetConfig().GetLoader().LoadPhysicalFile( "resources", boost::bind( &::InitializeDotations, _1 ) );
    MIL_AgentServer::GetWorkspace().GetConfig().GetLoader().LoadPhysicalFile( "weapon-systems", boost::bind( &::InitializeWeaponSystems, _1 ) );
    MIL_AgentServer::GetWorkspace().GetConfig().GetLoader().LoadPhysicalFile( "decisional", boost::bind( &::InitializeDecisional, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: Sink::ExecutePerceptions
// Created: SLI 2012-09-11
// -----------------------------------------------------------------------------
void Sink::ExecutePerceptions()
{
    facade_->ExecuteCommands( "perceptions" );
    ApplyEffects();
}

// -----------------------------------------------------------------------------
// Name: Sink::ExecuteCommands
// Created: SLI 2012-01-13
// -----------------------------------------------------------------------------
void Sink::ExecuteCommands()
{
    facade_->ExecuteCommands();
}

// -----------------------------------------------------------------------------
// Name: Sink::ApplyEffects
// Created: SLI 2012-01-13
// -----------------------------------------------------------------------------
void Sink::ApplyEffects()
{
    facade_->ApplyEffects();
}

// -----------------------------------------------------------------------------
// Name: Sink::NotifyEffects
// Created: MCO 2012-10-25
// -----------------------------------------------------------------------------
void Sink::NotifyEffects()
{
    facade_->NotifyEffects();
}

SWORD_USER_DATA_EXPORT( boost::shared_ptr< DEC_Knowledge_Agent > )
SWORD_USER_DATA_EXPORT( boost::shared_ptr< DEC_Knowledge_Object > )
SWORD_USER_DATA_EXPORT( const MIL_Population* )
SWORD_USER_DATA_EXPORT( const MIL_PopulationElement_ABC* )
SWORD_USER_DATA_EXPORT( MIL_Object_ABC* )

namespace
{
    void UpdateAgentKnowledge( const core::Model& model, core::Model& knowledge, boost::shared_ptr< DEC_Knowledge_Agent > agent )
    {
        knowledge[ "data" ].SetUserData( agent );
        knowledge[ "posture/data" ].SetUserData( &agent->GetAgentKnown().GetRole< PHY_RoleInterface_Posture >() );
        knowledge[ "identifier" ] = agent->GetAgentKnown().GetID();
        knowledge[ "is-dead" ] = agent->IsDead();
        core::Model& components = knowledge[ "components" ];
        const T_KnowledgeComposanteVector& composantes = agent->GetComposantes();
        unsigned int score = 0;
        knowledge[ "major" ] = -1;
        for( std::size_t i = 0; i < composantes.size(); ++i )
        {
            core::Model& component = components.AddElement();
            const DEC_Knowledge_AgentComposante& composante = composantes[ i ];
            component[ "volume" ] = composante.GetType().GetVolume().GetID();
            if( composante.GetMajorScore() >= score )
            {
                score = composante.GetMajorScore();
                knowledge[ "major" ] = i;
            }
            component[ "major" ] = composante.IsMajor();
            const core::Model& equipment = model[ "equipments" ][ composante.GetType().GetMosID().id() ]; // $$$$ MCO 2012-07-02: could be a link because that info is 'static'
            component[ "weapons" ] = equipment[ "weapons" ];
            component[ "protection" ] = equipment[ "protection" ];
        }
    }
    void UpdateObjectKnowledge( core::Model& knowledge, boost::shared_ptr< DEC_Knowledge_Object > object )
    {
        knowledge[ "data" ].SetUserData( object );
        knowledge[ "identifier" ] = object->GetObjectKnown() ? object->GetObjectKnown()->GetID() : 0;
        knowledge[ "real-name" ] = object->GetType().GetRealName();
    }
    void UpdateKnowledgeRelations(
        core::Model& enemies, core::Model& friends, const MIL_KnowledgeGroup& group, boost::shared_ptr< DEC_Knowledge_Agent > knowledge )
    {
        const MIL_Army_ABC* army = knowledge->GetArmy();
        if( army && ! knowledge->IsRefugee() && ! knowledge->IsSurrendered() )
        {
            if( knowledge->IsAnEnemy( group.GetArmy() ) == eTristate_True )
                enemies.AddElement() = knowledge->GetID();
            else if( knowledge->IsAFriend( group.GetArmy() ) == eTristate_True )
                friends.AddElement() = knowledge->GetID();
        }
    }
    void UpdateAgentKnowledgeGroupBlackBoard( const core::Model& model,
        core::Model& knowledges, core::Model& enemies, core::Model& friends, const MIL_KnowledgeGroup& group )
    {
        auto bbKg = group.GetKnowledge();
        if( !bbKg )
            return;
        BOOST_FOREACH( const auto& agent, bbKg->GetKnowledgeAgentContainer().GetKnowledgeAgents() )
        {
            boost::shared_ptr< DEC_Knowledge_Agent > knowledge = agent.second;
            UpdateAgentKnowledge( model, knowledges[ "agents" ][ knowledge->GetID() ], knowledge );
            UpdateKnowledgeRelations( enemies, friends, group, knowledge );
        }
    }
    void UpdateObjectKnowledgeGroupBlackBoard( core::Model& knowledges, const MIL_KnowledgeGroup& group )
    {
        if( DEC_BlackBoard_CanContainKnowledgeObject* container = group.GetKnowledgeObjectContainer() )
            BOOST_FOREACH( const auto& object, container->GetKnowledgeObjects() )
                UpdateObjectKnowledge( knowledges[ "objects" ][ object.second->GetID() ], object.second );
    }
    void UpdateKnowledgeGroup( const core::Model& model,
        core::Model& knowledges, core::Model& enemies, core::Model& friends, boost::shared_ptr< MIL_KnowledgeGroup > group )
    {
        const unsigned int id = group->GetId();
        UpdateAgentKnowledgeGroupBlackBoard( model, knowledges[ id ], enemies[ id ], friends[ id ], *group );
        UpdateObjectKnowledgeGroupBlackBoard( knowledges[ id ], *group );
        auto groups = group->GetKnowledgeGroups();
        for( auto it = groups.begin(); it != groups.end(); ++it )
            UpdateKnowledgeGroup( model, knowledges, enemies, friends, *it );
    }
    void UpdateKnowledges( const core::Model& model, core::Model& knowledges, core::Model& enemies, core::Model& friends )
    {
        knowledges.Clear();
        enemies.Clear();
        friends.Clear();
        const tools::Resolver< MIL_Army_ABC >& armies = MIL_AgentServer::GetWorkspace().GetEntityManager().GetArmies();
        for( tools::Iterator< const MIL_Army_ABC& > it = armies.CreateIterator(); it.HasMoreElements(); )
        {
            const auto& groups = it.NextElement().GetKnowledgeGroups();
            for( auto it2 = groups.begin(); it2 != groups.end(); ++it2 )
                UpdateKnowledgeGroup( model, knowledges, enemies, friends, it2->second );
        }
    }
    struct PopulationElementVisitor : public MIL_EntityVisitor_ABC< MIL_PopulationElement_ABC >
    {
        explicit PopulationElementVisitor( core::Model& elements )
            : elements_( elements )
        {}
        virtual void Visit( const MIL_PopulationElement_ABC& element )
        {
            elements_[ element.GetID() ][ "data" ].SetUserData( &element );
            elements_[ element.GetID() ][ "can-be-perceived" ] = element.CanBePerceived();
        }
        core::Model& elements_;
    };
    void UpdatePopulations( core::Model& populations, const tools::Resolver< MIL_Population >& factory )
    {
        for( tools::Iterator< const MIL_Population& > it = factory.CreateIterator(); it.HasMoreElements(); )
        {
            const MIL_Population& population = it.NextElement();
            core::Model& populationModel = populations[ population.GetID() ];
            populationModel.Clear();
            populationModel[ "data" ].SetUserData( &population );
            PopulationElementVisitor visitor( populationModel[ "elements" ] );
            population.Apply( visitor );
        }
    }
    template< typename T >
    void UpdateObjects( core::Model& objects, const T& factory )
    {
        objects.Clear();
        BOOST_FOREACH( const T::value_type& it, factory )
        {
            core::Model& object = objects[ it.first ];
            object[ "data" ].SetUserData( it.second );
            object[ "type/real-name" ] = it.second->GetType().GetRealName();
            object[ "type/identifier" ] = it.second->GetType().GetID();
            object[ "is-universal" ] = it.second->IsUniversal();
            object[ "can-be-perceived" ] = (*it.second)().CanBePerceived();
            object[ "marked-for-destruction" ] = it.second->IsMarkedForDestruction();
        }
    }
    void UpdatePerceptions( core::Model& notifications )
    {
        notifications[ "agents" ].Clear();
        notifications[ "agents-in-zone" ].Clear();
        notifications[ "objects" ].Clear();
        notifications[ "population-concentrations" ].Clear();
        notifications[ "population-flows" ].Clear();
        notifications[ "urban-blocks" ].Clear();
    }
    void UpdateAgent( MIL_AgentPion& pion, core::Model& entity )
    {
        entity[ "is-deployed" ] = ! pion.CallRole( &PHY_RoleInterface_Deployment::IsUndeployed, true );
        entity[ "is-prisoner" ] = pion.CallRole( &surrender::PHY_RoleInterface_Surrender::IsPrisoner, false );
        entity[ "is-surrendered" ] = pion.CallRole( &surrender::PHY_RoleInterface_Surrender::IsSurrendered, false );
        entity[ "is-transported" ] = pion.CallRole( &transport::PHY_RoleInterface_Transported::IsTransported, false );
        entity[ "is-dead" ] = pion.IsDead();
        entity[ "is-underground" ] = pion.CallRole( &PHY_RoleAction_MovingUnderground::IsUnderground, false );
        entity[ "can-emit" ] = pion.CallRole( &PHY_RoleInterface_Communications::CanEmit, true );
        entity[ "danger/x" ] = pion.GetOrderManager().GetDirDanger().rX_;
        entity[ "danger/y" ] = pion.GetOrderManager().GetDirDanger().rY_;
        entity[ "marked-for-destruction" ] = pion.IsMarkedForDestruction();
        core::Model& movement = entity[ "movement" ];
        movement[ "max-slope" ] = pion.CallRole( &moving::PHY_RoleAction_InterfaceMoving::GetMaxSlope, 1.0 );
        movement[ "has-resources" ] = pion.CallRole( &RoleAction_Moving::HasResources, false );
        movement[ "can-move" ] = pion.CallRole( &RoleAction_Moving::CanMove, false );
        movement[ "is-ready" ] = pion.CallRole( &RoleAction_Moving::IsReady, true );
        movement[ "can-be-traffic-impacted" ] = pion.CanBeImpactedByTraffic();
        movement[ "can-interact-with-traffic" ] = pion.CanInteractWithTraffic();
        movement[ "has-been-teleported" ] = pion.HasBeenTeleported();
        entity[ "knowledges" ] = pion.GetKnowledgeGroup()->GetId();
        UpdatePerceptions( entity[ "perceptions/notifications" ] );
        core::Model& components = entity[ "components" ];
        const transport::PHY_RoleAction_Loading* loading = pion.RetrieveRole< transport::PHY_RoleAction_Loading >();
        unsigned int score = 0;
        entity[ "major" ] = -1;
        for( std::size_t c = 0; c < components.GetSize(); ++c )
        {
            core::Model& component = components.GetElement( c );
            const PHY_ComposantePion& composante = component[ "data" ].GetUserData< PHY_ComposantePion >();
            if( composante.GetMajorScore() >= score )
            {
                score = composante.GetMajorScore();
                entity[ "major" ] = c;
            }
            component[ "can-perceive" ] = composante.CanPerceive( loading );
        }
    }
    template< typename T >
    void UpdateFlyingShells( core::Model& model, const T& flyingShells )
    {
        model.Clear();
        BOOST_FOREACH( const MIL_Effect_IndirectFire* fire, flyingShells )
        {
            core::Model& flyingShell = model.AddElement();
            flyingShell[ "identifier" ] = fire->GetFireID();
            flyingShell[ "data" ].SetUserData( fire );
        }
    }
}

DEFINE_HOOK( Tick, 0, void, () )
{
    if( GET_PREVIOUS_HOOK( Tick ) )
        GET_PREVIOUS_HOOK( Tick )();
}

// -----------------------------------------------------------------------------
// Name: Sink::UpdateModel
// Created: SLI 2012-01-13
// -----------------------------------------------------------------------------
void Sink::UpdateModel( unsigned int tick, int duration, const MIL_ObjectManager& objects, const MIL_EffectManager& effects )
{
    (*model_)[ "tick" ] = tick;
    (*model_)[ "step" ] = duration;
    core::Model& entities = (*model_)[ "entities" ];
    UpdatePopulations( (*model_)[ "populations" ], populations_ );
    UpdateObjects( (*model_)[ "objects" ], objects.GetObjects() );
    UpdateFlyingShells( (*model_)[ "flying-shells" ], effects.GetFlyingShells() );
    for( tools::Iterator< const MIL_AgentPion& > it = CreateIterator(); it.HasMoreElements(); )
    {
        MIL_AgentPion& pion = const_cast< MIL_AgentPion& >( it.NextElement() );
        UpdateAgent( pion, entities[ pion.GetID() ] );
    }
    GET_HOOK( Tick )();
}

// -----------------------------------------------------------------------------
// Name: Sink::UpdateKnowledges
// Created: SLI 2012-09-25
// -----------------------------------------------------------------------------
void Sink::UpdateKnowledges()
{
    ::UpdateKnowledges( *model_, (*model_)[ "knowledges" ], (*model_)[ "enemies" ], (*model_)[ "friends" ] );
}

// -----------------------------------------------------------------------------
// Name: Sink::UpdateUrbanModel
// Created: SLI 2012-09-24
// -----------------------------------------------------------------------------
void Sink::UpdateUrbanModel( const MIL_UrbanCache& cache )
{
    BOOST_FOREACH( const MIL_UrbanObject_ABC* object, cache.GetCities() )
        (*model_)[ "urban-objects" ][ object->GetID() ][ "data" ].SetUserData( object );
    BOOST_FOREACH( const MIL_UrbanObject_ABC* object, cache.GetUrbanBlocks() )
        (*model_)[ "urban-objects" ][ object->GetID() ][ "data" ].SetUserData( object );
}

DEFINE_HOOK( LogProfiling, 0, void, () )
{
    if( GET_PREVIOUS_HOOK( LogProfiling ) )
        GET_PREVIOUS_HOOK( LogProfiling )();
}

// -----------------------------------------------------------------------------
// Name: Sink::LogProfiling
// Created: MCO 2012-11-12
// -----------------------------------------------------------------------------
void Sink::LogProfiling()
{
    if( !(*model_)[ "profiling/command" ] && !(*model_)[ "profiling/hook" ] )
        return;
    Hooks::LogProfiling();
    GET_HOOK( LogProfiling )();
}

// -----------------------------------------------------------------------------
// Name: Sink::PostCommand
// Created: MCO 2012-09-11
// -----------------------------------------------------------------------------
void Sink::PostCommand( const std::string& type, const core::Model& parameters )
{
    facade_->PostCommand( type, parameters );
}

// -----------------------------------------------------------------------------
// Name: Sink::StartCommand
// Created: MCO 2012-09-11
// -----------------------------------------------------------------------------
std::size_t Sink::StartCommand( const std::string& type, const core::Model& parameters )
{
    return facade_->StartCommand( type, parameters );
}

// -----------------------------------------------------------------------------
// Name: Sink::PauseCommand
// Created: MCO 2012-09-11
// -----------------------------------------------------------------------------
void Sink::PauseCommand( std::size_t command )
{
    facade_->PauseCommand( command );
}

// -----------------------------------------------------------------------------
// Name: Sink::ResumeCommand
// Created: MCO 2012-09-11
// -----------------------------------------------------------------------------
void Sink::ResumeCommand( std::size_t command )
{
    facade_->ResumeCommand( command );
}

// -----------------------------------------------------------------------------
// Name: Sink::StopCommand
// Created: MCO 2012-09-11
// -----------------------------------------------------------------------------
void Sink::StopCommand( std::size_t command )
{
    facade_->StopCommand( command );
}

// -----------------------------------------------------------------------------
// Name: Sink::Configure
// Created: SLI 2012-01-13
// -----------------------------------------------------------------------------
MIL_AgentPion& Sink::Configure( MIL_AgentPion& pion, const MT_Vector2D& position )
{
    tools::Resolver< MIL_AgentPion >::Register( pion.GetID(), pion );
    pion.CallRole( &PHY_RoleInterface_UrbanLocation::MagicMove, position );
    StartContinuousCommands( pion );
    return pion;
}

namespace
{
    core::Model& GetEntity( MIL_AgentPion& pion, core::Model& m )
    {
        return m[ "entities" ][ pion.GetID() ];
    }
    sword::RolePion_Location* RoleLocationFactory( MIL_AgentPion& pion, Sink* sink, const MT_Vector2D& position, core::Model& m )
    {
        core::Model& entity = m[ "entities" ][ pion.GetID() ];
        entity[ "movement/position/x" ] = position.rX_;
        entity[ "movement/position/y" ] = position.rY_;
        return new sword::RolePion_Location( *sink, pion, entity, position );
    }
}

// -----------------------------------------------------------------------------
// Name: Sink::CreateRoles
// Created: AHC 2013-02-22
// -----------------------------------------------------------------------------
void Sink::CreateRoles( SinkRoleExtender& ext, const MT_Vector2D& position )
{
    ext.AddFactory< sword::RolePion_Location >( boost::bind( &RoleLocationFactory, _1, this, position, boost::ref( *model_ ) ) );
    ext.AddFactory< sword::RolePion_Decision >(
            boost::bind( boost::factory< sword::RolePion_Decision* >(), _1, boost::ref( *model_ ), gcPause_, gcMult_, decLogger_.get(), boost::ref( *this ) ) );
    ext.AddFactory< sword::RoleAction_Moving >(
            boost::bind( boost::factory< RoleAction_Moving* >(), _1, boost::bind( &GetEntity, _1, boost::ref( *model_ ) ) ) );
    ext.AddFactory< sword::RolePion_Perceiver >(
            boost::bind( boost::factory< sword::RolePion_Perceiver* >(), boost::ref( *this ), boost::ref( *model_ ), _1, boost::bind( &GetEntity, _1, boost::ref( *model_ ) ) ) );
    ext.AddFactory< sword::RolePion_Composantes >(
            boost::bind( boost::factory< sword::RolePion_Composantes* >(), _1, boost::ref( *model_) ) );
    ext.AddFactory< transport::PHY_RoleAction_Loading >(
            boost::bind( boost::factory< transport::PHY_RoleAction_Loading* >(), _1 ) ); // $$$$ _RC_ SLI 2012-11-09: must be created after RolePion_Composantes
    ext.AddFactory< sword::RoleAdapter >(
            boost::bind( boost::factory< sword::RoleAdapter* >(), boost::ref( *this ), _1, boost::bind( &GetEntity, _1, boost::ref( *model_ ) ) ) );
}

namespace
{
    MIL_AgentPion& Finalize( MIL_AgentPion& pion )
    {
        pion.CallRole( &sword::RolePion_Location::Finalize );
        pion.CallRole( &sword::RolePion_Perceiver::Finalize );
        pion.CallRole( &sword::RoleAdapter::Finalize );
        return pion;
    }
}

// -----------------------------------------------------------------------------
// Name: Sink::Finalize
// Created: BAX 2012-10-18
// -----------------------------------------------------------------------------
void Sink::Finalize()
{
    tools::Resolver< MIL_AgentPion >::Apply( boost::bind( &::Finalize, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: Sink::Clean
// Created: JSR 2013-01-30
// -----------------------------------------------------------------------------
void Sink::Clean()
{
    for( auto it = elements_.begin(); it != elements_.end(); )
    {
        MIL_AgentPion* pion = it->second;
        assert( pion );
        pion->Clean();
        if( pion->CanBeDeleted() )
        {
            StopContinuousCommands( *pion );
            it = elements_.erase( it );
            delete pion;
        }
        else
            ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis, sword::RoleExtender_ABC* ext )
{
    if( MIL_AgentPion* pPion = Find( xis.attribute< unsigned long >( "id" ) ) )
        throw MASA_EXCEPTION( MT_FormatString( "A unit with ID '%d' already exists.", pPion->GetID() ) );
    MT_Vector2D vPosTmp;
    MIL_Tools::ConvertCoordMosToSim( xis.attribute< std::string >( "position" ), vPosTmp );

    SinkRoleExtender chainExt( ext );
    CreateRoles( chainExt, vPosTmp );
    MIL_AgentPion& pion = Configure( *agents_.Create( type, automate, xis, &chainExt ), vPosTmp );
    pion.ReadOverloading( xis );
    return &pion;
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
 MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, sword::RoleExtender_ABC* ext )
{
    SinkRoleExtender chainExt( ext );
    CreateRoles( chainExt, vPosition );
    return &::Finalize( Configure( *agents_.Create( type, automate, vPosition, &chainExt ), vPosition ) );
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, const std::string& name, sword::RoleExtender_ABC* ext )
{
    SinkRoleExtender chainExt( ext );
    CreateRoles( chainExt, vPosition );
    return &::Finalize( Configure( *agents_.Create( type, automate, vPosition, name, &chainExt ), vPosition ) );
}

// -----------------------------------------------------------------------------
// Name: Sink::CreateFloodModel
// Created: LGY 2012-08-07
// -----------------------------------------------------------------------------
std::auto_ptr< propagation::FloodModel_ABC > Sink::CreateFloodModel() const
{
    assert( facade_.get() );
    return std::auto_ptr< propagation::FloodModel_ABC >( new sword::FloodModel( *facade_ ) );
}

// -----------------------------------------------------------------------------
// Name: Sink::GetModelCount
// Created: SLI 2012-09-12
// -----------------------------------------------------------------------------
unsigned long Sink::GetModelCount() const
{
    ModelCounter counter;
    model_->Accept( counter );
    return counter.count_;
}

// -----------------------------------------------------------------------------
// Name: Sink::Register
// Created: BAX 2012-10-16
// -----------------------------------------------------------------------------
void Sink::Register( const core::Model& model, core::ModelListener_ABC& listener )
{
    facade_->Register( model, listener );
}

// -----------------------------------------------------------------------------
// Name: Sink::Unregister
// Created: BAX 2012-10-16
// -----------------------------------------------------------------------------
void Sink::Unregister( const core::Model& model, core::ModelListener_ABC& listener )
{
    facade_->Unregister( model, listener );
}

// -----------------------------------------------------------------------------
// Name: Sink::StartContinuousCommands
// Created: MCO 2013-03-05
// -----------------------------------------------------------------------------
void Sink::StartContinuousCommands( const MIL_AgentPion& pion )
{
    const unsigned int id = pion.GetID();
    core::Model parameters;
    parameters[ "identifier" ] = id;
    commands_.insert( std::make_pair( id, StartCommand( "compute height", parameters ) ) );
    commands_.insert( std::make_pair( id, StartCommand( "perception", parameters ) ) );
    commands_.insert( std::make_pair( id, StartCommand( "interact with trafficability", parameters ) ) );
}

// -----------------------------------------------------------------------------
// Name: Sink::StopContinuousCommands
// Created: MCO 2013-03-05
// -----------------------------------------------------------------------------
void Sink::StopContinuousCommands( const MIL_AgentPion& pion )
{
    const unsigned int id = pion.GetID();
    boost::for_each(
        commands_.equal_range( id ) | boost::adaptors::map_values,
        boost::bind( &Sink::StopCommand, this, _1 ) );
    commands_.erase( id );
}
