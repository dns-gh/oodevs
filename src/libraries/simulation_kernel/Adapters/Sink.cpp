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
#include "Hook.h"
#include "MovementHooks.h"
#include "FloodModel.h"
#include "FireHooks.h"
#include "PerceptionHooks.h"
#include "RoleAdapter.h"
#include "CoreLogger.h"
#include "ModelCounter.h"
#include "MIL_AgentServer.h"
#include "RolePion_Decision.h"
#include "RolePion_Location.h"
#include "RoleAction_Moving.h"
#include "RolePion_Perceiver.h"
#include "RolePion_Decision.h"
#include "RolePion_Composantes.h"
#include "ReportEventListener.h"
#include "MovementReportNameEventListener.h"
#include "DirectFirePionEventListener.h"
#include "DirectFirePerceptionEventListener.h"
#include "DirectFirePionAttackEventListener.h"
#include "DirectFirePopulationEventListener.h"
#include "DirectFirePopulationAttackEventListener.h"
#include "ExternalPerceptionEventListener.h"
#include "MovementReportEventListener.h"
#include "CallbackEventListener.h"
#include "PerceptionCallbackEventListener.h"
#include "MovementEventListener.h"
#include "AlatMonitoringEventListener.h"
#include "FlyingShellPerceptionEventListener.h"
#include "PopulationFactory_ABC.h"
#include "Entities/Agents/Roles/Deployment/PHY_RoleInterface_Deployment.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Entities/Agents/Roles/Communications/PHY_RoleInterface_Communications.h"
#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/Objects/MIL_ObjectManager.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Objects/MIL_ObjectType_ABC.h"
#include "Entities/Objects/MIL_ObjectManipulator_ABC.h"
#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Populations/MIL_PopulationElement_ABC.h"
#include "Urban/MIL_UrbanCache.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_AgentComposante.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KS_Perception.h"
#include "Meteo/PHY_MeteoDataManager.h"
#include "Tools/MIL_Tools.h"
#include "MT_Tools/MT_ScipioException.h"
#include "tools/Loader_ABC.h"
#include <core/Facade.h>
#include <core/Model.h>
#include <core/UserData.h>
#include <core/EventListener_ABC.h>
#include <boost/foreach.hpp>

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
    std::vector< unsigned int > dangerousObjects;
    archive >> agents
            >> populations
            >> model
            >> gcPause
            >> gcMult
            >> dangerousObjects;
    ::new( sink )Sink( *agents, *populations, std::auto_ptr< core::Model >( model ), gcPause, gcMult, dangerousObjects );
    archive >> sink->elements_;
}
}

namespace
{
    DECLARE_HOOK( InitializePathClass, void, ( const char* xml, const unsigned int* first, size_t size ) )
    DECLARE_HOOK( InitializePerceptionTypes, void, ( const char* xml ) )
    DECLARE_HOOK( InitializeLaunchers, void, ( const char* xml ) )
    DECLARE_HOOK( InitializeDotations, void, ( const char* xml ) )
    DECLARE_HOOK( InitializeWeaponSystems, void, ( const char* xml, double tickDuration ) )
    DECLARE_HOOK( InitializeDecisional, void, ( const char* xml, double tickDuration ) )
    DECLARE_HOOK( InitializeRadarType, void, ( const char* xml ) )

    void InitializePathfinder( xml::xistream& xis, const std::vector< unsigned int >& dangerousObjects )
    {
        xml::xostringstream xos;
        xos << xis;
        GET_HOOK( InitializePathClass )( xos.str().c_str(), dangerousObjects.empty() ? 0 : &dangerousObjects[0], dangerousObjects.size() );
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
        GET_HOOK( InitializeWeaponSystems )( xos.str().c_str(), MIL_AgentServer::GetWorkspace().GetTickDuration() ) ;
    }
    void InitializeDecisional( xml::xistream& xis )
    {
        xml::xostringstream xos;
        xos << xis;
        GET_HOOK( InitializeDecisional )( xos.str().c_str(), MIL_AgentServer::GetWorkspace().GetTickDuration() ) ;
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
}

// -----------------------------------------------------------------------------
// Name: Sink constructor
// Created: SBO 2011-06-06
// -----------------------------------------------------------------------------
Sink::Sink( AgentFactory_ABC& agents, const PopulationFactory_ABC& populations,
            unsigned int gcPause, unsigned int gcMult, const std::vector< unsigned int >& dangerousObjects )
    : agents_           ( agents )
    , populations_      ( populations )
    , gcPause_          ( gcPause )
    , gcMult_           ( gcMult )
    , dangerousObjects_ ( dangerousObjects )
    , modules_          ( configuration )
    , logger_           ( new CoreLogger() )
    , model_            ( new core::Model() )
    , facade_           ( new core::Facade( modules_, *logger_, *model_ ) )
{
    Initialize();
}

// -----------------------------------------------------------------------------
// Name: Sink constructor
// Created: MCO 2012-09-12
// -----------------------------------------------------------------------------
Sink::Sink( AgentFactory_ABC& agents, const PopulationFactory_ABC& populations,
            std::auto_ptr< core::Model > model, unsigned int gcPause, unsigned int gcMult, const std::vector< unsigned int >& dangerousObjects )
    : agents_           ( agents )
    , populations_      ( populations )
    , gcPause_          ( gcPause )
    , gcMult_           ( gcMult )
    , dangerousObjects_ ( dangerousObjects )
    , modules_          ( configuration )
    , logger_           ( new CoreLogger() )
    , model_            ( model )
    , facade_           ( new core::Facade( modules_, *logger_, *model_ ) )
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
    listeners_.push_back( new ReportEventListener( *facade_ ) );
    listeners_.push_back( new MovementReportNameEventListener( *facade_ ) );
    listeners_.push_back( new CallbackEventListener( *model_, *facade_, "movement callback" ) );
    listeners_.push_back( new PerceptionCallbackEventListener( *facade_ ) );
    listeners_.push_back( new MovementEventListener( *model_, *facade_ ) );
    listeners_.push_back( new MovementReportEventListener( *facade_ ) );
    listeners_.push_back( new DirectFirePionEventListener( *facade_ ) );
    listeners_.push_back( new DirectFirePerceptionEventListener( *facade_ ) );
    listeners_.push_back( new DirectFirePionAttackEventListener( *facade_ ) );
    listeners_.push_back( new CallbackEventListener( *model_, *facade_, "direct fire pion callback" ) );
    listeners_.push_back( new ExternalPerceptionEventListener( *facade_ ) );
    listeners_.push_back( new AlatMonitoringEventListener( *model_, *facade_ ) );
    listeners_.push_back( new DirectFirePopulationAttackEventListener( *model_, *facade_ ) );
    listeners_.push_back( new DirectFirePopulationEventListener( *model_, *facade_, agents_ ) );
    MovementHooks::Initialize( *facade_ );
    RolePion_Decision::Initialize( *facade_ );
    FireHooks::Initialize( *facade_ );
    PerceptionHooks::Initialize( *facade_ );
    USE_HOOK( InitializePathClass, *facade_ );
    USE_HOOK( InitializePerceptionTypes, *facade_ );
    USE_HOOK( InitializeLaunchers, *facade_ );
    USE_HOOK( InitializeDotations, *facade_ );
    USE_HOOK( InitializeWeaponSystems, *facade_ );
    USE_HOOK( InitializeDecisional, *facade_ );
    facade_->Resolve();
    MIL_AgentServer::GetWorkspace().GetConfig().GetLoader().LoadPhysicalFile( "pathfinder", boost::bind( &::InitializePathfinder, _1, boost::cref( dangerousObjects_ ) ) );
    MIL_AgentServer::GetWorkspace().GetConfig().GetLoader().LoadPhysicalFile( "sensors", boost::bind( &::InitializePerception, _1 ) );
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

SWORD_USER_DATA_EXPORT( boost::shared_ptr< DEC_Knowledge_Agent > )
SWORD_USER_DATA_EXPORT( const MIL_Population* )
SWORD_USER_DATA_EXPORT( const MIL_PopulationElement_ABC* )
SWORD_USER_DATA_EXPORT( MIL_Object_ABC* )

namespace
{
    void UpdateKnowledge( const core::Model& entity, core::Model& knowledge, boost::shared_ptr< DEC_Knowledge_Agent > agent )
    {
        knowledge[ "agent" ].SetUserData( agent );
        knowledge[ "identifier" ] = agent->GetAgentKnown().GetID();
        knowledge[ "dead" ] = agent->IsDead();
        core::Model& components = knowledge[ "components" ];
        components.Clear();
        const core::Model& components2 = entity[ "components" ];
        const T_KnowledgeComposanteVector& composantes = agent->GetComposantes();
        for( std::size_t i = 0; i < composantes.size(); ++i )
        {
            assert( components2.GetSize() == composantes.size() );
            core::Model& component = components.AddElement();
            const DEC_Knowledge_AgentComposante& composante = composantes[ i ];
            component[ "volume" ] = composante.GetType().GetVolume().GetID();
            component[ "score" ] = composante.GetMajorScore();
            component[ "major" ] = composante.IsMajor();
            const core::Model& component2 = components2.GetElement( i );
            component[ "component" ] = component2[ "component" ];
            component[ "weapons" ] = component2[ "weapons" ]; // $$$$ MCO 2012-07-02: could be a link because that info is 'static'
        }
    }
    void UpdateKnowledgeRelations( core::Model& enemies, core::Model& friends, const MIL_KnowledgeGroup& group, boost::shared_ptr< DEC_Knowledge_Agent > knowledge )
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
    void UpdateKnowledgeGroupBlackBoard( const core::Model& entities, core::Model& knowledges, core::Model& enemies, core::Model& friends, const MIL_KnowledgeGroup& group )
    {
        knowledges.Clear();
        enemies.Clear();
        friends.Clear();
        typedef DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap::value_type T_Agent;
        const DEC_BlackBoard_CanContainKnowledgeAgent& blackboard = group.GetKnowledge().GetKnowledgeAgentContainer();
        BOOST_FOREACH( const T_Agent& agent, blackboard.GetKnowledgeAgents() )
        {
            boost::shared_ptr< DEC_Knowledge_Agent > knowledge = agent.second;
            UpdateKnowledge( entities[ knowledge->GetAgentKnown().GetID() ], knowledges[ knowledge->GetID() ], knowledge );
            UpdateKnowledgeRelations( enemies, friends, group, knowledge );
        }
    }
    void UpdateKnowledgeGroup( const core::Model& entities, core::Model& knowledges, core::Model& enemies, core::Model& friends, boost::shared_ptr< MIL_KnowledgeGroup > group )
    {
        const unsigned int id = group->GetId();
        UpdateKnowledgeGroupBlackBoard( entities, knowledges[ id ], enemies[ id ], friends[ id ], *group );
        MIL_KnowledgeGroup::T_KnowledgeGroupVector groups = group->GetKnowledgeGroups();
        for( MIL_KnowledgeGroup::T_KnowledgeGroupVector::const_iterator it = groups.begin(); it != groups.end(); ++it )
            UpdateKnowledgeGroup( entities, knowledges, enemies, friends, *it );
    }
    void UpdateKnowledges( const core::Model& entities, core::Model& knowledges, core::Model& enemies, core::Model& friends )
    {
        const tools::Resolver< MIL_Army_ABC >& armies = MIL_AgentServer::GetWorkspace().GetEntityManager().GetArmies();
        for( tools::Iterator< const MIL_Army_ABC& > it = armies.CreateIterator(); it.HasMoreElements(); )
        {
            typedef std::map< unsigned int, boost::shared_ptr< MIL_KnowledgeGroup > > T_Groups;
            const T_Groups& groups = it.NextElement().GetKnowledgeGroups();
            for( T_Groups::const_iterator it = groups.begin(); it != groups.end(); ++it )
                UpdateKnowledgeGroup( entities, knowledges, enemies, friends, it->second );
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
        }
    }
    void UpdateAgent( MIL_AgentPion& pion, core::Model& entity )
    {
        entity[ "is-deployed" ] = ! pion.GetRole< PHY_RoleInterface_Deployment >().IsUndeployed();
        entity[ "is-prisoner" ] = pion.GetRole< surrender::PHY_RoleInterface_Surrender >().IsPrisoner();
        entity[ "is-transported" ] = pion.GetRole< transport::PHY_RoleInterface_Transported >().IsTransported();
        entity[ "is-dead" ] = pion.IsDead();
        entity[ "is-underground" ] = pion.GetRole< PHY_RoleAction_MovingUnderground >().IsUnderground();
        entity[ "can-emit" ] = pion.GetRole< PHY_RoleInterface_Communications >().CanEmit();
        entity[ "danger/x" ] = pion.GetOrderManager().GetDirDanger().rX_;
        entity[ "danger/y" ] = pion.GetOrderManager().GetDirDanger().rY_;
        core::Model& movement = entity[ "movement" ];
        movement[ "max-slope" ] = pion.GetRole< RoleAction_Moving >().GetMaxSlope();
        movement[ "has-resources" ] = pion.GetRole< RoleAction_Moving >().HasResources();
        movement[ "can-move" ] = pion.GetRole< RoleAction_Moving >().CanMove();
        entity[ "knowledges" ] = pion.GetKnowledgeGroup()->GetId();
        core::Model& components = entity[ "components" ];
        const transport::PHY_RoleAction_Loading* loading = pion.RetrieveRole< transport::PHY_RoleAction_Loading >();
        for( std::size_t c = 0; c < components.GetSize(); ++c )
        {
            core::Model& component = components.GetElement( c );
            const PHY_ComposantePion& composante = component[ "component" ].GetUserData< PHY_ComposantePion >();
            component[ "score" ] = composante.GetMajorScore();
            component[ "can-perceive" ] = composante.CanPerceive( loading );
        }
    }
    template< typename T >
    void UpdateFlyingShells( core::Model& model, T& flyingShells )
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
    UpdateObjects( (*model_ )[ "objects" ], objects.GetObjects() );
    UpdateFlyingShells( (*model_ )[ "flying-shells" ], effects.GetFlyingShells() );
    for( tools::Iterator< const MIL_AgentPion& > it = agents_.CreateIterator(); it.HasMoreElements(); )
    {
        MIL_AgentPion& pion = const_cast< MIL_AgentPion& >( it.NextElement() );
        UpdateAgent( pion, entities[ pion.GetID() ] );
    }
}

// -----------------------------------------------------------------------------
// Name: Sink::UpdateKnowledges
// Created: SLI 2012-09-25
// -----------------------------------------------------------------------------
void Sink::UpdateKnowledges()
{
    ::UpdateKnowledges( (*model_)[ "entities" ], (*model_)[ "knowledges" ], (*model_)[ "enemies" ], (*model_)[ "friends" ] );
}

// -----------------------------------------------------------------------------
// Name: Sink::UpdateUrbanModel
// Created: SLI 2012-09-24
// -----------------------------------------------------------------------------
void Sink::UpdateUrbanModel( const MIL_UrbanCache& cache )
{
    BOOST_FOREACH( const MIL_UrbanObject_ABC* object, cache.GetCities() )
        (*model_ )[ "urban-objects" ][ object->GetID() ][ "data" ].SetUserData( object );
    BOOST_FOREACH( const MIL_UrbanObject_ABC* object, cache.GetUrbanBlocks() )
        (*model_ )[ "urban-objects" ][ object->GetID() ][ "data" ].SetUserData( object );
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
    core::Model& entity = (*model_)[ "entities" ][ pion.GetID() ];
    entity[ "movement/position/x" ] = position.rX_;
    entity[ "movement/position/y" ] = position.rY_;
    pion.RegisterRole( *new sword::RolePion_Location( pion, entity ) );
    try
    {
        pion.RegisterRole( *new sword::RolePion_Decision( pion, *model_, gcPause_, gcMult_, *this ) );
    }
    catch( MT_ScipioException& e )
    {
        e.SendToLogger();
    }
    pion.RegisterRole( *new sword::RoleAction_Moving( pion ) );
    pion.RegisterRole( *new sword::RolePion_Perceiver( *this, *model_, pion, entity ) );
    pion.RegisterRole( *new sword::RolePion_Composantes( pion, entity ) );
    pion.RegisterRole( *new sword::RoleAdapter( pion, entity ) );
    tools::Resolver< MIL_AgentPion >::Register( pion.GetID(), pion );
    pion.GetRole< PHY_RoleInterface_UrbanLocation >().MagicMove( position );
    {
        core::Model parameters;
        parameters[ "identifier" ] = pion.GetID();
        facade_->StartCommand( "compute height", parameters );
        facade_->StartCommand( "perception", parameters );
    }
    return pion;
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, xml::xistream& xis )
{
    std::string strPosition;
    xis >> xml::attribute( "position", strPosition );
    MT_Vector2D vPosTmp;
    MIL_Tools::ConvertCoordMosToSim( strPosition, vPosTmp );
    MIL_AgentPion& pion = Configure( *agents_.Create( type, automate, xis ), vPosTmp );
    pion.GetRole< PHY_RolePion_Composantes >().ReadOverloading( xis ); // Equipments + Humans
    return &pion;
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition )
{
    return &Configure( *agents_.Create( type, automate, vPosition ), vPosition );
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, const std::string& name )
{
    return &Configure( *agents_.Create( type, automate, vPosition, name ), vPosition );
}

// -----------------------------------------------------------------------------
// Name: Sink::CreateFloodModel
// Created: LGY 2012-08-07
// -----------------------------------------------------------------------------
std::auto_ptr< flood::FloodModel_ABC > Sink::CreateFloodModel() const
{
    assert( facade_.get() );
    return std::auto_ptr< flood::FloodModel_ABC >( new sword::FloodModel( *facade_ ) );
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
