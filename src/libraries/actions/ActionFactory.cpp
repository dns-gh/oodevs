// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "actions_pch.h"
#include "ActionFactory.h"
#include "ActionTasker.h"
#include "ActionTiming.h"
#include "AgentMission.h"
#include "AutomatMission.h"
#include "AutomatCreationMagicAction.h"
#include "CreationListener_ABC.h"
#include "EngageMagicAction.h"
#include "FragOrder.h"
#include "Identifier.h"
#include "KnowledgeGroupMagicAction.h"
#include "MagicAction.h"
#include "Numeric.h"
#include "ObjectMagicAction.h"
#include "Parameter_ABC.h"
#include "ParameterFactory_ABC.h"
#include "ParameterList.h"
#include "Point.h"
#include "PopulationMission.h"
#include "Quantity.h"
#include "String.h"
#include "Bool.h"
#include "UnitMagicAction.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/MagicActionType.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Point.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/PopulationType.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionFactory constructor
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
ActionFactory::ActionFactory( kernel::Controller& controller, const actions::ParameterFactory_ABC& factory, const kernel::EntityResolver_ABC& entities
                            , const kernel::StaticModel& staticModel, const kernel::Time_ABC& simulation )
    : controller_         ( controller )
    , factory_            ( factory )
    , entities_           ( entities )
    , staticModel_        ( staticModel )
    , missions_           ( staticModel.types_ )
    , fragOrders_         ( staticModel.types_ )
    , magicActions_       ( staticModel.types_ )
    , coordinateConverter_( staticModel.coordinateConverter_ )
    , simulation_         ( simulation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionFactory destructor
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
ActionFactory::~ActionFactory()
{
    // NOTHING
}

namespace
{
    std::exception MissingParameter( const actions::Action_ABC& action, const kernel::OrderParameter& param )
    {
        return std::exception( tools::translate( "ActionFactory", "Parameter mismatch in action '%1' (id: %2): missing parameter '%3'." )
                                .arg( action.GetName() ).arg( action.GetId() ).arg( param.GetName().c_str() ) );
    }
    std::exception TargetNotFound( unsigned long id = 0 )
    {
        if( id )
            return std::exception( tools::translate( "ActionFactory", "Unable to find executing entity '%1'." ).arg( id ) );
        return std::exception( tools::translate( "ActionFactory", "Executing target not set" ) );
    }
    std::exception MagicIdNotFound( const std::string& id )
    {
        return std::exception( tools::translate( "ActionFactory", "Magic action type '%1' unknown." ).arg( id.c_str() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( const kernel::Entity_ABC& target, const kernel::MissionType& mission ) const
{
    std::auto_ptr< actions::Action_ABC > action;
    if( target.GetTypeName() == kernel::Agent_ABC::typeName_ )
        action.reset( new actions::AgentMission( target, mission, controller_, true ) );
    else if( target.GetTypeName() == kernel::Automat_ABC::typeName_ )
        action.reset( new actions::AutomatMission( target, mission, controller_, true ) );
    else if( target.GetTypeName() == kernel::Population_ABC::typeName_ )
        action.reset( new actions::PopulationMission( target, mission, controller_, true ) );
    else
        throw std::runtime_error( __FUNCTION__ " Cannot resolve executing entity" );

    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &target, false ) );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrderType& fragOrder ) const
{
    std::auto_ptr< actions::Action_ABC > action( new actions::FragOrder( target, fragOrder, controller_, true ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &target, false ) );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( xml::xistream& xis, bool readonly /* = false*/ ) const
{
    const std::string type = xis.attribute< std::string >( "type" );
    if( type == "mission" )
        return CreateMission( xis, readonly );
    if( type == "fragorder" )
        return CreateFragOrder( xis, readonly );
    if( type == "magic" )
        return CreateMagicAction( xis, readonly );
    if( type == "magicunit" )
        return CreateUnitMagicAction( xis, readonly );
    if( type == "magicobject" )
        return CreateObjectMagicAction( xis, readonly );
    if( type == "magicknowledge" )
        return CreateKnowledgeGroupMagicAction( xis, readonly );
    if( type == "change_mode" )
        return AutomateChangeModeMagicAction( xis, readonly );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateStubAction
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateStubAction( xml::xistream& xis ) const
{
    const std::string type = xis.attribute< std::string >( "type" );
    if( type == "mission" )
        return CreateStubMission( xis );
    if( type == "fragorder" )
        return CreateStubFragOrder( xis );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AddTiming
// Created: SBO 2010-09-21
// -----------------------------------------------------------------------------
template< typename Message >
void ActionFactory::AddTiming( actions::Action_ABC& action, const Message& message ) const
{
    if( message.has_start_time() )
        action.Attach( *new ActionTiming( controller_, simulation_, message.start_time().data() ) );
    else
        action.Attach( *new ActionTiming( controller_, simulation_ ) );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( const sword::UnitOrder& message ) const
{
    const kernel::MissionType& mission = missions_.Get( message.type().id() );
    const kernel::Entity_ABC& tasker = entities_.GetAgent( message.tasker().id() );
    std::auto_ptr< actions::Action_ABC > action( new actions::AgentMission( tasker, mission, controller_, true ) );
    AddTiming( *action, message );
    action->Attach( *new ActionTasker( &tasker ) );
    action->Polish();
    AddParameters( *action, mission, message.parameters() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( const sword::AutomatOrder& message ) const
{
    const kernel::MissionType& mission = missions_.Get( message.type().id() );
    const kernel::Entity_ABC& tasker = entities_.GetAutomat( message.tasker().id() );
    std::auto_ptr< actions::Action_ABC > action( new actions::AutomatMission( tasker, mission, controller_, true ) );
    AddTiming( *action, message );
    action->Attach( *new ActionTasker( &tasker ) );
    action->Polish();
    AddParameters( *action, mission, message.parameters() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( const sword::CrowdOrder& message ) const
{
    const kernel::MissionType& mission = missions_.Get( message.type().id() );
    const kernel::Entity_ABC& tasker = entities_.GetPopulation( message.tasker().id() );
    std::auto_ptr< actions::Action_ABC > action( new actions::PopulationMission( tasker, mission, controller_, true ) );
    AddTiming( *action, message );
    action->Attach( *new ActionTasker( &tasker ) );
    action->Polish();
    AddParameters( *action, mission, message.parameters() );
    return action.release();
}

namespace
{
    const kernel::Entity_ABC& FindTasker( sword::FragOrder message, const kernel::EntityResolver_ABC& entities )
    {
        if( message.tasker().has_automat() )
            try
            {
                return entities.GetAutomat( message.tasker().automat().id() );
            }
            catch( ... )
            {
                throw TargetNotFound( message.tasker().automat().id() );
            }
        if( message.tasker().has_crowd() )
            return entities.GetPopulation( message.tasker().crowd().id() );
        if( message.tasker().has_unit() )
            return entities.GetAgent( message.tasker().unit().id() );
        throw TargetNotFound();
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( const sword::FragOrder& message ) const
{
    const kernel::Entity_ABC& tasker = FindTasker( message, entities_ );
    const kernel::FragOrderType& order = fragOrders_.Get( message.type().id() );
    std::auto_ptr< actions::Action_ABC > action( new actions::FragOrder( tasker, order, controller_, true ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( &tasker ) );
    action->Polish();
    AddParameters( *action, order, message.parameters() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateMission
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateMission( xml::xistream& xis, bool readonly, bool stub ) const
{
    const unsigned long id = xis.attribute< unsigned long >( "target" );
    std::auto_ptr< actions::Mission > action;
    const kernel::Entity_ABC* target = entities_.FindAgent( id );
    if( target )
        action.reset( new actions::AgentMission( xis, controller_, missions_, *target, stub ) );
    else
    {
        target = entities_.FindAutomat( id );
        if( target )
            action.reset( new actions::AutomatMission( xis, controller_, missions_, *target, stub ) );
        else
        {
            target = entities_.FindPopulation( id );
            if( target )
                action.reset( new actions::PopulationMission( xis, controller_, missions_, *target, stub ) );
            else
                throw TargetNotFound( id );
        }
    }
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target, readonly ) );
    action->Polish();

    tools::Iterator< const kernel::OrderParameter& > it = action->GetType().CreateIterator();
    if( stub )
    {
        xis >> xml::list( "parameter", *this, &ActionFactory::ReadStubParameter, *action, it, *target );
        action->Invalidate();
    }
    else
    {
        xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
        if( it.HasMoreElements() )
            throw MissingParameter( *action, it.NextElement() );
    }
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateMission
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateMission( xml::xistream& xis, bool readonly ) const
{
    return CreateMission( xis, readonly, false );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateStubMission
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateStubMission( xml::xistream& xis ) const
{
    return CreateMission( xis, false, true );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateFragOrder
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateFragOrder( xml::xistream& xis, bool readonly, bool stub ) const
{
    const unsigned long id = xis.attribute< unsigned long >( "target" );

    std::auto_ptr< actions::FragOrder > action;
    const kernel::Entity_ABC* target = entities_.FindAgent( id );
    if( !target )
        target = entities_.FindAutomat( id );
    if( !target )
        target = entities_.FindPopulation( id );
    if( !target )
        throw TargetNotFound( id );
    action.reset( new actions::FragOrder( xis, controller_, fragOrders_, *target ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target, readonly ) );
    action->Polish();

    tools::Iterator< const kernel::OrderParameter& > it = action->GetType().CreateIterator();
    if( stub )
    {
        xis >> xml::list( "parameter", *this, &ActionFactory::ReadStubParameter, *action, it, *target );
        action->Invalidate();
    }
    else
    {
        xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
        if( it.HasMoreElements() )
            throw MissingParameter( *action, it.NextElement() );
    }
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateFragOrder
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateFragOrder( xml::xistream& xis, bool readonly ) const
{
    return CreateFragOrder( xis, readonly, false );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateStubFragOrder
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateStubFragOrder( xml::xistream& xis ) const
{
    return CreateFragOrder( xis, false, true );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateMagicAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateMagicAction( xml::xistream& xis, bool readonly ) const
{
    const std::string id = xis.attribute< std::string >( "id" );
    std::string name;
    xis >> xml::optional >> xml::attribute( "name", name );
    std::auto_ptr< actions::MagicAction > action;
    action.reset( new actions::MagicAction( xis, controller_, magicActions_.Get( id ), name.c_str() ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    action->Attach( *new ActionTasker( 0, readonly ) );
    action->Polish();

    tools::Iterator< const kernel::OrderParameter& > it = action->GetType().CreateIterator();
    xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, *action, it );
    if( it.HasMoreElements() )
        throw MissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateUnitMagicAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateUnitMagicAction( xml::xistream& xis, bool readonly ) const
{
    const unsigned long targetid = xis.attribute< unsigned long >( "target" );
    const std::string id = xis.attribute< std::string >( "id" );
    std::string name;
    xis >> xml::optional >> xml::attribute( "name", name );

    std::auto_ptr< actions::UnitMagicAction > action;
    const kernel::Entity_ABC* target = entities_.FindAgent( targetid );
    if( !target )
        target = entities_.FindAutomat( targetid );
    if( !target )
        target = entities_.FindPopulation( targetid );
    if( !target )
        target = entities_.FindFormation( targetid );
    if( !target )
        target = entities_.FindTeam( targetid );
    if( !target )
        throw TargetNotFound( targetid );

    action.reset( new actions::UnitMagicAction( xis, controller_, magicActions_.Get( id ), *target, name.c_str() ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target, readonly ) );
    action->Polish();

    tools::Iterator< const kernel::OrderParameter& > it = action->GetType().CreateIterator();
    xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
    if( it.HasMoreElements() )
        throw MissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AutomateChangeModeMagicAction
// Created: FDS 2010-11-23
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::AutomateChangeModeMagicAction( xml::xistream& xis, bool readonly ) const
{
    const unsigned long targetid = xis.attribute< unsigned long >( "target" );
    const std::string type = xis.attribute< std::string >( "type" );
    std::string name;
    xis >> xml::optional >> xml::attribute( "name", name );
    bool engaged;
    xis >> xml::attribute( "engaged", engaged );

    std::auto_ptr< actions::EngageMagicAction > action;
    const kernel::Entity_ABC* target = entities_.FindAgent( targetid );
    if( !target )
        target = entities_.FindAutomat( targetid );
    if( !target )
        target = entities_.FindPopulation( targetid );
    if( !target )
        target = entities_.FindFormation( targetid );
    if( !target )
        target = entities_.FindTeam( targetid );
    if( !target )
        throw TargetNotFound( targetid );

    action.reset( new actions::EngageMagicAction( xis, controller_, magicActions_.Get( type ), *target, name.c_str(), engaged ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target, readonly ) );
    action->Polish();

    return action.release();
}
// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateObjectMagicAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateObjectMagicAction( xml::xistream& xis, bool readonly ) const
{
    const std::string id = xis.attribute< std::string >( "id" );

    std::auto_ptr< actions::ObjectMagicAction > action;

    kernel::Entity_ABC* target = 0;
    if( id != "create_object" )
    {
        const unsigned long targetid = xis.attribute< unsigned long >( "target" );
        target = entities_.FindObject( targetid );
        if( !target )
            throw TargetNotFound( targetid );
    }

    action.reset( new actions::ObjectMagicAction( xis, controller_, magicActions_.Get( id ), target ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    action->Attach( *new ActionTasker( 0, readonly ) );
    action->Polish();

    tools::Iterator< const kernel::OrderParameter& > it = action->GetType().CreateIterator();
    if( target )
        xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
    else
        xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, *action, it );
    if( it.HasMoreElements() )
        throw MissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateObjectMagicAction
// Created: JCR 2011-01-04
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateObjectMagicAction( const std::string& magicAction, unsigned long targetId  ) const
{
    std::auto_ptr< actions::ObjectMagicAction > action;
    kernel::Entity_ABC* target = 0;

    if( magicAction != "create_object" )
    {
        target = entities_.FindObject( targetId );
        if( !target )
            throw TargetNotFound( targetId );
    }
    action.reset( new actions::ObjectMagicAction( target, magicActions_.Get( magicAction ), controller_, true ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( 0, false ) );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateObjectUpdateMagicAction
// Created: JSR 2011-03-01
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateObjectUpdateMagicAction( const kernel::Object_ABC& object, parameters::ParameterList& attribute ) const
{
    std::auto_ptr< actions::ObjectMagicAction > action;
    action.reset( new actions::ObjectMagicAction( &object, magicActions_.Get( "update_object" ), controller_, true ) );
    action->Rename( tools::translate( "gaming_app::Action", "Object Update" ) );
    tools::Iterator< const OrderParameter& > it = action->GetType().CreateIterator();
    parameters::ParameterList* attributesList = new parameters::ParameterList( it.NextElement() );
    action->AddParameter( *attributesList );
    attributesList->AddParameter( attribute );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( 0, false ) );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateObjectDestroyMagicAction
// Created: JSR 2011-03-01
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateObjectDestroyMagicAction( const kernel::Object_ABC& object ) const
{
    std::auto_ptr< actions::ObjectMagicAction > action;
    action.reset( new actions::ObjectMagicAction( &object, magicActions_.Get( "destroy_object" ), controller_, true ) );
    action->Rename( tools::translate( "gaming_app::Action", "Object Destruction" ) );
    action->Attach( *new ActionTiming( controller_, simulation_ ) );
    action->Attach( *new ActionTasker( 0, false ) );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateKnowledgeGroupMagicAction
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateKnowledgeGroupMagicAction( xml::xistream& xis, bool readonly ) const
{
    const unsigned long targetid = xis.attribute< unsigned long >( "target" );
    const std::string id = xis.attribute< std::string >( "id" );

    std::auto_ptr< actions::KnowledgeGroupMagicAction > action;
    const kernel::KnowledgeGroup_ABC* target = entities_.FindKnowledgeGroup( targetid );
    if( !target )
        throw TargetNotFound( targetid );

    action.reset( new actions::KnowledgeGroupMagicAction( xis, controller_, magicActions_.Get( id ), *target ) );
    action->Attach( *new ActionTiming( xis, controller_, simulation_ ) );
    action->Attach( *new ActionTasker( target, readonly ) );
    action->Polish();

    tools::Iterator< const kernel::OrderParameter& > it = action->GetType().CreateIterator();
    xis >> xml::list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
    if( it.HasMoreElements() )
        throw MissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AddParameters
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
void ActionFactory::AddParameters( actions::Action_ABC& action, const kernel::OrderType& order, const sword::MissionParameters& message ) const
{
    tools::Iterator< const kernel::OrderParameter& > it = order.CreateIterator();
    int i = 0;
    while( it.HasMoreElements() )
    {
        if( i >= message.elem_size() )
            throw std::runtime_error( __FUNCTION__ " Mission parameter count does not match mission definition" );
        if( const ActionTasker* tasker = action.Retrieve< ActionTasker >() )
            if( const kernel::Entity_ABC* entity = tasker->GetTasker() )
            if( actions::Parameter_ABC* newParam = factory_.CreateParameter( it.NextElement(), message.elem( i++ ), *entity ) )
                action.AddParameter( *newParam );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::ReadParameter
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
void ActionFactory::ReadParameter( xml::xistream& xis, actions::Action_ABC& action, tools::Iterator< const kernel::OrderParameter& >& it, const kernel::Entity_ABC& entity ) const
{
    try
    {
        if( !it.HasMoreElements() )
            throw std::exception( tools::translate( "ActionFactory", "too many parameters provided" ) );
        // $$$$ LDC AddParameter can throw after having taken ownership of the parameter thus do not use auto_ptr here
        actions::Parameter_ABC* param = factory_.CreateParameter( it.NextElement(), xis, entity );
        action.AddParameter( *param );
    }
    catch( std::exception& e )
    {
        throw std::exception( tools::translate( "ActionFactory", "Parameter mismatch in action '%1' (id: %2): %3." )
                                .arg( action.GetName() ).arg( action.GetId() ).arg( e.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::ReadStubParameter
// Created: LDC 2010-07-28
// -----------------------------------------------------------------------------
void ActionFactory::ReadStubParameter( xml::xistream& xis, actions::Action_ABC& action, tools::Iterator< const kernel::OrderParameter& >& it, const kernel::Entity_ABC& entity ) const
{
    try
    {
        ReadParameter( xis, action, it, entity );
    }
    catch( std::exception& /*e*/ ) {}
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::ReadParameter
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
void ActionFactory::ReadParameter( xml::xistream& xis, actions::Action_ABC& action, tools::Iterator< const kernel::OrderParameter& >& it ) const
{
    try
    {
        if( !it.HasMoreElements() )
            throw std::exception( tools::translate( "ActionFactory", "too many parameters provided" ) );
        std::auto_ptr< actions::Parameter_ABC > param( factory_.CreateParameter( it.NextElement(), xis ) );
        action.AddParameter( *param );
        param.release();
    }
    catch( std::exception& e )
    {
        throw std::exception( tools::translate( "ActionFactory", "Parameter mismatch in action '%1' (id: %2): %3." )
                                .arg( action.GetName() ).arg( action.GetId() ).arg( e.what() ) );
    }
}

namespace
{
    unsigned int FindKnowledgeGroupForArmy( const kernel::Entity_ABC& entity )
    {
        if( dynamic_cast< const kernel::KnowledgeGroup_ABC* >( &entity ) )
            return entity.GetId();
        const kernel::CommunicationHierarchies& hierarchy = entity.Get< kernel::CommunicationHierarchies >();
        tools::Iterator< const kernel::Entity_ABC& > it = hierarchy.CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            unsigned int group = FindKnowledgeGroupForArmy( it.NextElement() );
            if( group )
                return group;
        }
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAutomatCreationAction
// Created: LDC 2010-10-06
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAutomatCreationAction( const AutomatType& type, const Entity_ABC& selected, const geometry::Point2f& point,
                                                                tools::Resolver_ABC< Automat_ABC >& agentsModel, CreationListener_ABC& agentMessenger,
                                                                ActionsModel& actionsModel, const Time_ABC& simulation ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "automat_creation" );

    AutomatCreationMagicAction* action = new AutomatCreationMagicAction( selected, actionType, controller_, tools::translate( "ActionFactory", "Automat Creation" ),
          staticModel_, type, point, agentsModel, agentMessenger, actionsModel, simulation, true );

    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( it.NextElement(), type.GetId() ) );
    int knowledgeGroup = 0;
    const kernel::CommunicationHierarchies* hierarchy = selected.Retrieve< kernel::CommunicationHierarchies >();
    if( !hierarchy )
    {
        const kernel::TacticalHierarchies& tacticalHierarchy = selected.Get< kernel::TacticalHierarchies >();
        const kernel::Entity_ABC& top = tacticalHierarchy.GetTop();
        knowledgeGroup = FindKnowledgeGroupForArmy( top );
    }
    else
    {
        const kernel::Entity_ABC* superior = hierarchy->GetSuperior();
        while( superior )
        {
            if( dynamic_cast< const kernel::KnowledgeGroup_ABC* >( superior ) )
            {
                knowledgeGroup = superior->GetId();
                break;
            }
            superior = superior->Get< kernel::CommunicationHierarchies >().GetSuperior();
        }
    }
    action->AddParameter( *new parameters::Identifier( it.NextElement(), knowledgeGroup ) );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAgentCreationAction
// Created: LDC 2010-10-11
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAgentCreationAction( const kernel::AgentType& type, const geometry::Point2f& point, const kernel::Entity_ABC& selected ) const
{
    kernel::Point location;
    location.AddPoint( point );
    kernel::MagicActionType& actionType = magicActions_.Get( "unit_creation" );
    UnitMagicAction* action = new UnitMagicAction( selected, actionType, controller_, tools::translate( "ActionFactory", "Unit Creation" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Identifier( it.NextElement(), type.GetId() ) );
    action->AddParameter( *new parameters::Point( it.NextElement(), coordinateConverter_, location ) );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateFormationCreationAction
// Created: LDC 2010-10-20
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateFormationCreationAction( int level, const kernel::Entity_ABC& selected ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "formation_creation" );
    UnitMagicAction* action = new UnitMagicAction( selected, actionType, controller_, tools::translate( "ActionFactory", "Formation Creation" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Numeric( it.NextElement(), static_cast<float>( level ) ) );
    action->AddParameter( *new parameters::String( it.NextElement(), std::string() ) );
    action->AddParameter( *new parameters::String( it.NextElement(), std::string() ) );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateCrowdCreationAction
// Created: LDC 2010-10-22
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateCrowdCreationAction( const kernel::PopulationType& type, int number, const geometry::Point2f& point, const kernel::Entity_ABC& selected ) const
{
    kernel::Point location;
    location.AddPoint( point );
    kernel::MagicActionType& actionType = magicActions_.Get( "crowd_creation" );
    UnitMagicAction* action = new UnitMagicAction( selected, actionType, controller_, tools::translate( "ActionFactory", "Crowd Creation" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::String( it.NextElement(), type.GetName() ) );
    action->AddParameter( *new parameters::Point( it.NextElement(), coordinateConverter_, location ) );
    action->AddParameter( *new parameters::Numeric( it.NextElement(), static_cast< float >( number ) ) );
    action->AddParameter( *new parameters::String( it.NextElement(), std::string() ) );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateCrowdChangeHealthStateAction
// Created: JSR 2011-03-15
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateCrowdChangeHealthStateAction( int healthy, int wounded, int contaminated, int dead, const kernel::Entity_ABC& selected ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "crowd_change_health_state" );
    UnitMagicAction* action = new UnitMagicAction( selected, actionType, controller_, tools::translate( "ActionFactory", "Population Change Health State" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Quantity( it.NextElement(), healthy ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), wounded ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), contaminated ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), dead ) );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateInhabitantChangeHealthStateAction
// Created: ABR 2011-01-26
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateInhabitantChangeHealthStateAction( int healthy, int wounded, int dead, const kernel::Entity_ABC& selected ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "inhabitant_change_health_state" );
    UnitMagicAction* action = new UnitMagicAction( selected, actionType, controller_, tools::translate( "ActionFactory", "Population Change Health State" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Quantity( it.NextElement(), healthy ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), wounded ) );
    action->AddParameter( *new parameters::Quantity( it.NextElement(), dead ) );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateInhabitantChangeAlertedStateAction
// Created: BCI 2011-02-03
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateInhabitantChangeAlertedStateAction( bool alerted, const kernel::Entity_ABC& selected ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "inhabitant_change_alerted_state" );
    UnitMagicAction* action = new UnitMagicAction( selected, actionType, controller_, tools::translate( "ActionFactory", "Population Change Alerted State" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Bool( it.NextElement(), alerted ) );
    return action;
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateInhabitantChangeConfinedStateAction
// Created: BCI 2011-02-03
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateInhabitantChangeConfinedStateAction( bool confined, const kernel::Entity_ABC& selected ) const
{
    kernel::MagicActionType& actionType = magicActions_.Get( "inhabitant_change_confined_state" );
    UnitMagicAction* action = new UnitMagicAction( selected, actionType, controller_, tools::translate( "ActionFactory", "Population Change Confined State" ), true );
    tools::Iterator< const kernel::OrderParameter& > it = actionType.CreateIterator();
    action->AddParameter( *new parameters::Bool( it.NextElement(), confined ) );
    return action;
}
