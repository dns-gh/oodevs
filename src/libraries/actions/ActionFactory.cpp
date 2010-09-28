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
#include "ActionsModel.h"
#include "ActionTasker.h"
#include "ActionTiming.h"
#include "AgentMission.h"
#include "AutomatMission.h"
#include "FragOrder.h"
#include "KnowledgeGroupMagicAction.h"
#include "MagicAction.h"
#include "ObjectMagicAction.h"
#include "Parameter_ABC.h"
#include "ParameterFactory_ABC.h"
#include "PopulationMission.h"
#include "UnitMagicAction.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/EntityResolver_ABC.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/OrderParameter.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/StaticModel.h"
#include "clients_kernel/Tools.h"
#include "protocol/protocol.h"
#include <xeumeuleu/xml.hpp>

using namespace actions;

// -----------------------------------------------------------------------------
// Name: ActionFactory constructor
// Created: SBO 2010-05-07
// -----------------------------------------------------------------------------
ActionFactory::ActionFactory( kernel::Controller& controller, const actions::ParameterFactory_ABC& factory, const kernel::EntityResolver_ABC& entities
                             , const kernel::StaticModel& staticModel, const kernel::Time_ABC& simulation )
    : controller_  ( controller )
    , factory_     ( factory )
    , entities_    ( entities )
    , missions_    ( staticModel.types_ )
    , fragOrders_  ( staticModel.types_ )
    , magicActions_( staticModel.types_ )
    , simulation_  ( simulation )
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
actions::Action_ABC* ActionFactory::CreateAction( xml::xistream& xis, bool readonly /*= false*/ ) const
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
actions::Action_ABC* ActionFactory::CreateAction( const Common::MsgUnitOrder& message ) const
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
actions::Action_ABC* ActionFactory::CreateAction( const Common::MsgAutomatOrder& message ) const
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
actions::Action_ABC* ActionFactory::CreateAction( const Common::MsgCrowdOrder& message ) const
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
    const kernel::Entity_ABC& FindTasker( MsgsClientToSim::MsgFragOrder message, const kernel::EntityResolver_ABC& entities )
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
actions::Action_ABC* ActionFactory::CreateAction( const MsgsClientToSim::MsgFragOrder& message ) const
{
    const kernel::Entity_ABC& tasker = FindTasker( message, entities_ );
    const kernel::FragOrderType& order = fragOrders_.Get( message.frag_order().id() );
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
    else if( target = entities_.FindAutomat( id ) )
        action.reset( new actions::AutomatMission( xis, controller_, missions_, *target, stub ) );
    else if( target = entities_.FindPopulation( id ) )
        action.reset( new actions::PopulationMission( xis, controller_, missions_, *target, stub ) );
    else
        throw TargetNotFound( id );
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
    std::auto_ptr< actions::MagicAction > action;
    action.reset( new actions::MagicAction( xis, controller_, magicActions_.Get( id ), "" ) );
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

    std::auto_ptr< actions::UnitMagicAction > action;
    const kernel::Entity_ABC* target = entities_.FindAgent( targetid );
    if( !target )
        target = entities_.FindAutomat( targetid );
    if( !target )
        target = entities_.FindPopulation( targetid );
    if( !target )
        throw TargetNotFound( targetid );

    action.reset( new actions::UnitMagicAction( xis, controller_, magicActions_.Get( id ), *target, "" ) );
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
void ActionFactory::AddParameters( actions::Action_ABC& action, const kernel::OrderType& order, const Common::MsgMissionParameters& message ) const
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
