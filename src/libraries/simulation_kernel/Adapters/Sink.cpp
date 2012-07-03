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
#include "FireHooks.h"
#include "PerceptionHooks.h"
#include "RoleAdapter.h"
#include "CoreLogger.h"
#include "MIL_AgentServer.h"
#include "RolePion_Decision.h"
#include "RolePion_Location.h"
#include "RoleAction_Moving.h"
#include "RolePion_Perceiver.h"
#include "RolePion_Composantes.h"
#include "ReportEventListener.h"
#include "MovementReportNameEventListener.h"
#include "DirectFirePionEventListener.h"
#include "DirectFirePionAttackEventListener.h"
#include "MovementReportEventListener.h"
#include "MovementCallbackEventListener.h"
#include "MovementCallbackEventListener.h"
#include "MovementEventListener.h"
#include "FloodModelFactory.h"
#include "Entities/Agents/Roles/Deployment/PHY_RoleInterface_Deployment.h"
#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"
#include "Entities/Agents/Units/Categories/PHY_Volume.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Entities/Objects/MIL_ObjectFactory.h"
#include "Entities/Objects/MIL_ObjectFilter.h"
#include "Entities/MIL_Army_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Orders/MIL_Report.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_AgentComposante.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "Knowledge/DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Knowledge/DEC_KS_Perception.h"
#include "Tools/MIL_Tools.h"
#include "MT_Tools/MT_ScipioException.h"
#include "tools/Loader_ABC.h"
#include <core/Facade.h>
#include <core/Model.h>
#include <core/UserData.h>
#include <core/EventListener_ABC.h>
#include <core/MergeVisitor.h>
#include <boost/foreach.hpp>

using namespace sword;

BOOST_CLASS_EXPORT_IMPLEMENT( sword::Sink )

namespace sword
{
template< typename Archive >
void save_construct_data( Archive& archive, const Sink* sink, const unsigned int /*version*/ )
{
    const AgentFactory_ABC* factory = &sink->factory_;
    archive << factory
            << sink->gcPause_
            << sink->gcMult_
            << sink->elements_
            << sink->model_;
}

template< typename Archive >
void load_construct_data( Archive& archive, Sink* sink, const unsigned int /*version*/ )
{
    AgentFactory_ABC* factory;
    unsigned int gcPause;
    unsigned int gcMult;
    archive >> factory
            >> gcPause
            >> gcMult;
    ::new( sink )Sink( *factory, gcPause, gcMult );
    archive >> sink->elements_
            >> sink->model_;
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

    void InitializePathfinder( xml::xistream& xis )
    {
        MIL_DangerousObjectFilter filter;
        std::vector< unsigned int > dangerousObjects;
        MIL_ObjectFactory::FindDangerousIDs( dangerousObjects, filter );
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
}

// -----------------------------------------------------------------------------
// Name: Sink constructor
// Created: SBO 2011-06-06
// -----------------------------------------------------------------------------
Sink::Sink( AgentFactory_ABC& factory, unsigned int gcPause, unsigned int gcMult )
    : factory_( factory )
    , gcPause_( gcPause )
    , gcMult_ ( gcMult )
    , modules_( "<modules>"
                "  <module>movement_module</module>"
                "  <module>perception_module</module>"
                "  <module>fire_module</module>"
                "  <module>propagation_module</module>"
                "</modules>" )
    , logger_ ( new CoreLogger() )
    , model_  ( new core::Model() )
    , facade_ ( new core::Facade( modules_, *logger_, *model_ ) )
{
    listeners_.push_back( new ReportEventListener( *model_, *facade_ ) );
    listeners_.push_back( new MovementReportNameEventListener( *model_, *facade_ ) );
    listeners_.push_back( new MovementCallbackEventListener( *model_, *facade_ ) );
    listeners_.push_back( new MovementEventListener( *model_, *facade_ ) );
    listeners_.push_back( new MovementReportEventListener( *model_, *facade_ ) );
    listeners_.push_back( new DirectFirePionEventListener( *model_, *facade_, factory ) );
    listeners_.push_back( new DirectFirePionAttackEventListener( *model_, *facade_, factory ) );
    MovementHooks::Initialize( *facade_ );
    FireHooks::Initialize( *facade_ );
    PerceptionHooks::Initialize( *facade_ );
    USE_HOOK( InitializePathClass, *facade_ );
    USE_HOOK( InitializePerceptionTypes, *facade_ );
    USE_HOOK( InitializeLaunchers, *facade_ );
    USE_HOOK( InitializeDotations, *facade_ );
    USE_HOOK( InitializeWeaponSystems, *facade_ );
    USE_HOOK( InitializeDecisional, *facade_ );
    facade_->Resolve();
    MIL_AgentServer::GetWorkspace().GetConfig().GetLoader().LoadPhysicalFile( "pathfinder", boost::bind( &::InitializePathfinder, _1 ) );
    MIL_AgentServer::GetWorkspace().GetConfig().GetLoader().LoadPhysicalFile( "sensors", boost::bind( &::InitializePerception, _1 ) );
    MIL_AgentServer::GetWorkspace().GetConfig().GetLoader().LoadPhysicalFile( "launchers", boost::bind( &::InitializeLaunchers, _1 ) );
    MIL_AgentServer::GetWorkspace().GetConfig().GetLoader().LoadPhysicalFile( "resources", boost::bind( &::InitializeDotations, _1 ) );
    MIL_AgentServer::GetWorkspace().GetConfig().GetLoader().LoadPhysicalFile( "weapon-systems", boost::bind( &::InitializeWeaponSystems, _1 ) );
    MIL_AgentServer::GetWorkspace().GetConfig().GetLoader().LoadPhysicalFile( "decisional", boost::bind( &::InitializeDecisional, _1 ) );
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

namespace
{
    void UpdateKnowledge( core::Model& knowledge, boost::shared_ptr< DEC_Knowledge_Agent > agent )
    {
        knowledge[ "agent" ].SetUserData( agent );
        knowledge[ "identifier" ] = agent->GetAgentKnown().GetID();
        knowledge[ "valid" ] = agent->IsValid();
        knowledge[ "dead" ] = agent->IsDead();
        core::Model& components = knowledge[ "components" ];
        components.ClearElements();
        BOOST_FOREACH( const DEC_Knowledge_AgentComposante& composante, agent->GetComposantes() )
        {
            core::Model& component = components.AddElement();
            component[ "volume" ] = composante.GetType().GetVolume().GetID();
            component[ "score" ] = composante.GetMajorScore();
        }
    }
    void UpdateKnowledgeRelations( core::Model& enemies, core::Model& friends, const MIL_KnowledgeGroup& group, boost::shared_ptr< DEC_Knowledge_Agent > knowledge )
    {
        const MIL_Army_ABC* army = knowledge->GetArmy();
        if( army && ! knowledge->IsRefugee() && ! knowledge->IsSurrendered() )
        {
            if( knowledge->IsAnEnemy( group.GetArmy() ) == eTristate_True )
                enemies.AddElement() = knowledge->GetAgentKnown().GetID();
            else if( knowledge->IsAFriend( group.GetArmy() ) == eTristate_True )
                friends.AddElement() = knowledge->GetAgentKnown().GetID();
        }
    }
    void UpdateKnowledgeGroupBlackBoard( core::Model& knowledges, core::Model& enemies, core::Model& friends, const MIL_KnowledgeGroup& group )
    {
        enemies.ClearElements();
        friends.ClearElements();
        typedef DEC_BlackBoard_CanContainKnowledgeAgent::T_KnowledgeAgentMap::value_type T_Agent;
        const DEC_BlackBoard_CanContainKnowledgeAgent& blackboard = group.GetKnowledge().GetKnowledgeAgentContainer();
        BOOST_FOREACH( const T_Agent& agent, blackboard.GetKnowledgeAgents() )
        {
            boost::shared_ptr< DEC_Knowledge_Agent > knowledge = agent.second;
            UpdateKnowledge( knowledges[ knowledge->GetAgentKnown().GetID() ], knowledge );
            UpdateKnowledgeRelations( enemies, friends, group, knowledge );
        }
    }
    void UpdateKnowledgeGroup( core::Model& knowledges, core::Model& enemies, core::Model& friends, const MIL_KnowledgeGroup& group )
    {
        const unsigned int id = group.GetId();
        UpdateKnowledgeGroupBlackBoard( knowledges[ id ], enemies[ id ], friends[ id ], group );
        BOOST_FOREACH( const MIL_KnowledgeGroup* subgroup, group.GetKnowledgeGroups() )
            UpdateKnowledgeGroup( knowledges, enemies, friends, *subgroup );
    }
    void UpdateKnowledges( core::Model& knowledges, core::Model& enemies, core::Model& friends )
    {
        const tools::Resolver< MIL_Army_ABC >& armies = MIL_AgentServer::GetWorkspace().GetEntityManager().GetArmies();
        for( tools::Iterator< const MIL_Army_ABC& > it = armies.CreateIterator(); it.HasMoreElements(); )
        {
            typedef std::map< unsigned int, MIL_KnowledgeGroup* > T_Groups;
            const T_Groups& groups = it.NextElement().GetKnowledgeGroups();
            for( T_Groups::const_iterator it = groups.begin(); it != groups.end(); ++it )
                UpdateKnowledgeGroup( knowledges, enemies, friends, *it->second );
        }
    }
    void UpdateAgent( MIL_AgentPion& pion, core::Model& entity )
    {
        entity[ "is-deployed" ] = ! pion.GetRole< PHY_RoleInterface_Deployment >().IsUndeployed();
        entity[ "is-prisoner" ] = pion.GetRole< surrender::PHY_RoleInterface_Surrender >().IsPrisoner();
        entity[ "is-transported" ] = pion.GetRole< transport::PHY_RoleInterface_Transported >().IsTransported();
        entity[ "is-dead" ] = pion.IsDead();
        entity[ "is-underground" ] = pion.GetRole< PHY_RoleAction_MovingUnderground >().IsUnderground();
        entity[ "danger/x" ] = pion.GetOrderManager().GetDirDanger().rX_;
        entity[ "danger/y" ] = pion.GetOrderManager().GetDirDanger().rY_;
        core::Model& movement = entity[ "movement" ];
        movement[ "max-slope" ] = pion.GetRole< RoleAction_Moving >().GetMaxSlope();
        movement[ "has-resources" ] = pion.GetRole< RoleAction_Moving >().HasResources();
        movement[ "can-move" ] = pion.GetRole< RoleAction_Moving >().CanMove();
        entity[ "knowledges" ] = pion.GetKnowledgeGroup().GetId();
        core::Model& components = entity[ "components" ];
        for( std::size_t c = 0; c < components.GetSize(); ++c )
        {
            core::Model& component = components.GetElement( c );
            const PHY_ComposantePion& composante = component[ "component" ].GetUserData< PHY_ComposantePion >();
            component[ "score" ] = composante.GetMajorScore();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Sink::UpdateModel
// Created: SLI 2012-01-13
// -----------------------------------------------------------------------------
void Sink::UpdateModel( unsigned int tick, int duration )
{
    (*model_)[ "tick" ] = tick;
    (*model_)[ "step" ] = duration;
    UpdateKnowledges( (*model_)[ "knowledges" ], (*model_)[ "enemies" ], (*model_)[ "friends" ] );
    core::Model& entities = (*model_)[ "entities" ];
    for( tools::Iterator< const MIL_AgentPion& > it = factory_.CreateIterator(); it.HasMoreElements(); )
    {
        MIL_AgentPion& pion = const_cast< MIL_AgentPion& >( it.NextElement() );
        UpdateAgent( pion, entities[ pion.GetID() ] );
    }
}

// -----------------------------------------------------------------------------
// Name: Sink::GetModel
// Created: SLI 2012-02-02
// -----------------------------------------------------------------------------
core::Model& Sink::GetModel() const
{
    return *model_;
}

// -----------------------------------------------------------------------------
// Name: Sink::GetFacade
// Created: SLI 2012-02-02
// -----------------------------------------------------------------------------
core::Facade& Sink::GetFacade() const
{
    return *facade_;
}

// -----------------------------------------------------------------------------
// Name: Sink::Configure
// Created: SLI 2012-01-13
// -----------------------------------------------------------------------------
MIL_AgentPion& Sink::Configure( MIL_AgentPion& pion, const MT_Vector2D& vPosition )
{
    core::Model& entity = (*model_)[ "entities" ][ pion.GetID() ];
    entity[ "movement/position/x" ] = vPosition.rX_;
    entity[ "movement/position/y" ] = vPosition.rY_;
    pion.RegisterRole( *new sword::RolePion_Location( pion, entity ) );
    try
    {
        pion.RegisterRole( *new sword::RolePion_Decision( pion, gcPause_, gcMult_, *this ) );
    }
    catch( MT_ScipioException& e )
    {
        e.SendToLogger();
    }
    pion.RegisterRole( *new sword::RoleAction_Moving( pion ) );
    pion.RegisterRole( *new sword::RolePion_Perceiver( pion, entity ) );
    pion.RegisterRole( *new sword::RolePion_Composantes( pion, entity ) );
    pion.RegisterRole( *new sword::RoleAdapter( pion, entity ) );
    tools::Resolver< MIL_AgentPion >::Register( pion.GetID(), pion );
    pion.GetRole< PHY_RoleInterface_UrbanLocation >().MagicMove( vPosition );
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
    MIL_AgentPion& pion = Configure( *factory_.Create( type, automate, xis ), vPosTmp );
    pion.GetRole< PHY_RolePion_Composantes >().ReadOverloading( xis ); // Equipments + Humans
    return &pion;
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition )
{
    return &Configure( *factory_.Create( type, automate, vPosition ), vPosition );
}

// -----------------------------------------------------------------------------
// Name: Sink::Create
// Created: SLI 2012-02-10
// -----------------------------------------------------------------------------
MIL_AgentPion* Sink::Create( const MIL_AgentTypePion& type, MIL_Automate& automate, const MT_Vector2D& vPosition, const std::string& name )
{
    return &Configure( *factory_.Create( type, automate, vPosition, name ), vPosition );
}

// -----------------------------------------------------------------------------
// Name: Sink::CreateFloodModelFactory
// Created: LGY 2012-06-20
// -----------------------------------------------------------------------------
std::auto_ptr< FloodModelFactory_ABC > Sink::CreateFloodModelFactory() const
{
    return std::auto_ptr< sword::FloodModelFactory_ABC >( new FloodModelFactory() );
}
