// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "ActionFactory.h"
#include "actions/AgentMission.h"
#include "actions/AutomatMission.h"
#include "actions/PopulationMission.h"
#include "actions/FragOrder.h"
#include "actions/MagicAction.h"
#include "actions/KnowledgeGroupMagicAction.h"
#include "actions/UnitMagicAction.h"
#include "actions/ObjectMagicAction.h"
#include "actions/Parameter_ABC.h"
#include "Model.h"
#include "AgentsModel.h"
#include "ObjectsModel.h"
#include "KnowledgeGroupsModel.h"
#include "ActionTasker.h"
#include "ActionTiming.h"
#include "ParameterFactory_ABC.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/OrderParameter.h"
#include "Tools.h"
#include <xeumeuleu/xml.h>

#pragma warning( disable : 4706 )

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionFactory constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionFactory::ActionFactory( Controllers& controllers, const ParameterFactory_ABC& factory, const Model& model
                            , const tools::Resolver_ABC< MissionType >& missions
                            , const tools::Resolver_ABC< FragOrderType >& fragOrders
                            , const tools::Resolver_ABC< kernel::MagicActionType, std::string >& magicActions
                            , const Simulation& simulation )
    : controllers_( controllers )
    , factory_( factory )
    , model_( model )
    , missions_( missions )
    , fragOrders_( fragOrders )
    , magicActions_( magicActions )
    , simulation_( simulation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionFactory destructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionFactory::~ActionFactory()
{
    // NOTHING
}

namespace
{
    void ThrowMissingParameter( const actions::Action_ABC& action, const OrderParameter& param )
    {
        throw std::exception( tools::translate( "ActionFactory", "Parameter mismatch in action '%1' (id: %2): missing parameter '%3'." )
                                .arg( action.GetName() ).arg( action.GetId() ).arg( param.GetName().c_str() ) );
    }

    void ThrowTargetNotFound( unsigned long id )
    {
        throw std::exception( tools::translate( "ActionFactory", "Unable to find executing entity '%1'." ).arg( id ) );
    }

    void ThrowMagicIdNotFound( const std::string& id )
    {
        throw std::exception( tools::translate( "ActionFactory", "Magic action type '%1' unknown." ).arg( id.c_str() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( const Entity_ABC& target, const MissionType& mission ) const
{
    std::auto_ptr< actions::Action_ABC > action;
    if( target.GetTypeName() == Agent_ABC::typeName_ ) 
        action.reset( new actions::AgentMission( target, mission, controllers_.controller_, true ) );
    else if( target.GetTypeName() == Automat_ABC::typeName_ )
        action.reset( new actions::AutomatMission( target, mission, controllers_.controller_, true ) );
    else if( target.GetTypeName() == Population_ABC::typeName_ )
        action.reset( new actions::PopulationMission( target, mission, controllers_.controller_, true ) );
    else
        throw std::runtime_error( __FUNCTION__ " Cannot resolve executing entity" );

    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->Attach( *new ActionTasker( target, false ) );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( const Entity_ABC& target, const FragOrderType& fragOrder ) const
{
    std::auto_ptr< actions::Action_ABC > action( new actions::FragOrder( target, fragOrder, controllers_.controller_, true ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->Attach( *new ActionTasker( target, false ) );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( const Common::MsgUnitOrder& message ) const
{
    const MissionType& mission = missions_.Get( message.mission() );
    const kernel::Entity_ABC& tasker = model_.agents_.GetAgent( message.oid() );
    std::auto_ptr< actions::Action_ABC > action( new actions::AgentMission( tasker, mission, controllers_.controller_, true ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->Attach( *new ActionTasker( tasker ) );
    action->Polish();
    AddParameters( *action, mission, message.parametres() );
    model_.actions_.Register( action->GetId(), *action ); // $$$$ SBO 2010-05-06: to allow actions from decisional to be recorded
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( const Common::MsgAutomatOrder& message ) const
{
    const MissionType& mission = missions_.Get( message.mission() );
    const kernel::Entity_ABC& tasker = model_.agents_.GetAutomat( message.oid() );
    std::auto_ptr< actions::Action_ABC > action( new actions::AutomatMission( tasker, mission, controllers_.controller_, true ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->Attach( *new ActionTasker( tasker ) );
    action->Polish();
    AddParameters( *action, mission, message.parametres() );
    model_.actions_.Register( action->GetId(), *action ); // $$$$ SBO 2010-05-06: to allow actions from decisional to be recorded
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( const Common::MsgPopulationOrder& message ) const
{
    const MissionType& mission = missions_.Get( message.mission() );
    const kernel::Entity_ABC& tasker = model_.agents_.GetPopulation( message.oid() );
    std::auto_ptr< actions::Action_ABC > action( new actions::PopulationMission( tasker, mission, controllers_.controller_, true ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->Attach( *new ActionTasker( tasker ) );
    action->Polish();
    AddParameters( *action, mission, message.parametres() );
    model_.actions_.Register( action->GetId(), *action ); // $$$$ SBO 2010-05-06: to allow actions from decisional to be recorded
    return action.release();
}

namespace
{
    const kernel::Entity_ABC& FindTasker( unsigned int id, AgentsModel& model )
    {
        const kernel::Entity_ABC* tasker = 0;
        if( tasker = model.FindAgent( id ) )
            return *tasker;
        if( tasker = model.FindAutomat( id ) )
            return *tasker;
        if( tasker = model.FindPopulation( id ) )
            return *tasker;
        ThrowTargetNotFound( id );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( const MsgsClientToSim::MsgFragOrder& message ) const
{
    const kernel::Entity_ABC& tasker = FindTasker( message.oid(), model_.agents_ );
    const FragOrderType& order = fragOrders_.Get( message.frag_order() );
    std::auto_ptr< actions::Action_ABC > action( new actions::FragOrder( tasker, order, controllers_.controller_, true ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->Attach( *new ActionTasker( tasker ) );
    action->Polish();
    AddParameters( *action, order, message.parametres() );
    model_.actions_.Register( action->GetId(), *action ); // $$$$ SBO 2010-05-06: to allow actions from decisional to be recorded
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AddParameters
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
void ActionFactory::AddParameters( actions::Action_ABC& action, const OrderType& order, const Common::MsgMissionParameters& message ) const
{
    tools::Iterator< const OrderParameter& > it = order.CreateIterator();
    int i = 0;
    while( it.HasMoreElements() )
    {
        if( i >= message.elem_size() )
            throw std::runtime_error( __FUNCTION__ " Mission parameter count does not match mission definition" );

        actions::ActionWithTarget_ABC* actionWithTarget = dynamic_cast< actions::ActionWithTarget_ABC* >( &action );
        if( actionWithTarget )
            if( actions::Parameter_ABC* newParam = factory_.CreateParameter( it.NextElement(), message.elem( i++ ), actionWithTarget->GetEntity() ) )
                actionWithTarget->AddParameter( *newParam );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( xml::xistream& xis ) const
{
    std::string type;
    xis >> attribute( "type", type );
    if( type == "mission" )
        return CreateMission( xis );
    if( type == "fragorder" )
        return CreateFragOrder( xis );
    if( type == "magic" )
        return CreateMagicAction( xis );
    if( type == "magicunit" )
        return CreateUnitMagicAction( xis );
    if( type == "magicobject" )
        return CreateObjectMagicAction( xis );
    if( type == "magicknowledge" )
        return CreateKnowledgeGroupMagicAction( xis );
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateMission
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateMission( xml::xistream& xis ) const
{
    const unsigned long id = xml::attribute< unsigned long >( xis, "target" );

    std::auto_ptr< actions::Mission > action;
    const kernel::Entity_ABC* target = model_.agents_.FindAgent( id );
    if( target )
        action.reset( new actions::AgentMission( xis, controllers_.controller_, missions_, *target ) );
    else if( target = model_.agents_.FindAutomat( id ) )
        action.reset( new actions::AutomatMission( xis, controllers_.controller_, missions_, *target ) );
    else if( target = model_.agents_.FindPopulation( id ) )
        action.reset( new actions::PopulationMission( xis, controllers_.controller_, missions_, *target ) );
    else
        ThrowTargetNotFound( id );
    action->Attach( *new ActionTiming( xis, controllers_.controller_, simulation_, *action ) );
    action->Attach( *new ActionTasker( *target, false ) );
    action->Polish();

    tools::Iterator< const OrderParameter& > it = action->GetType().CreateIterator();
    xis >> list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
    if( it.HasMoreElements() )
        ThrowMissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateFragOrder
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateFragOrder( xml::xistream& xis ) const
{
    const unsigned long id = xml::attribute< unsigned long >( xis, "target" );

    std::auto_ptr< actions::FragOrder > action;
    const kernel::Entity_ABC* target = model_.agents_.FindAgent( id );
    if( !target )
        target = model_.agents_.FindAutomat( id );
    if( !target )
        target = model_.agents_.FindPopulation( id );
    if( !target )
        ThrowTargetNotFound( id );
    action.reset( new actions::FragOrder( xis, controllers_.controller_, fragOrders_, *target ) );
    action->Attach( *new ActionTiming( xis, controllers_.controller_, simulation_, *action ) );
    action->Attach( *new ActionTasker( *target, false ) );
    action->Polish();

    tools::Iterator< const OrderParameter& > it = action->GetType().CreateIterator();
    xis >> list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
    if( it.HasMoreElements() )
        ThrowMissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateMagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateMagicAction( xml::xistream& xis ) const
{
    const std::string id = xml::attribute< std::string >( xis, "id" );

    std::auto_ptr< actions::MagicAction > action;
    action.reset( new actions::MagicAction( xis, controllers_.controller_, magicActions_.Get( id ) ) );
    action->Attach( *new ActionTiming( xis, controllers_.controller_, simulation_, *action ) );
    action->Polish();

    tools::Iterator< const OrderParameter& > it = action->GetType().CreateIterator();
    xis >> list( "parameter", *this, &ActionFactory::ReadParameter, *action, it );
    if( it.HasMoreElements() )
        ThrowMissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateUnitMagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateUnitMagicAction( xml::xistream& xis ) const
{
    const unsigned long targetid = xml::attribute< unsigned long >( xis, "target" );
    const std::string id = xml::attribute< std::string >( xis, "id" );

    std::auto_ptr< actions::UnitMagicAction > action;
    const kernel::Entity_ABC* target = model_.agents_.FindAgent( targetid );
    if( !target )
        target = model_.agents_.FindAutomat( targetid );
    if( !target )
        target = model_.agents_.FindPopulation( targetid );
    if( !target )
        ThrowTargetNotFound( targetid );

    action.reset( new actions::UnitMagicAction( xis, controllers_.controller_, magicActions_.Get( id ), *target ) );
    action->Attach( *new ActionTiming( xis, controllers_.controller_, simulation_, *action ) );
    action->Attach( *new ActionTasker( *target, false ) );
    action->Polish();

    tools::Iterator< const OrderParameter& > it = action->GetType().CreateIterator();
    xis >> list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
    if( it.HasMoreElements() )
        ThrowMissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateObjectMagicAction
// Created: JSR 2010-04-02
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateObjectMagicAction( xml::xistream& xis ) const
{
    const std::string id = xml::attribute< std::string >( xis, "id" );

    std::auto_ptr< actions::ObjectMagicAction > action;

    kernel::Entity_ABC* target = 0;
    if( id != "create_object" )
    {
        const unsigned long targetid = xml::attribute< unsigned long >( xis, "target" );
        target = model_.objects_.FindObject( targetid );
        if( !target )
            ThrowTargetNotFound( targetid );
    }

    action.reset( new actions::ObjectMagicAction( xis, controllers_.controller_, magicActions_.Get( id ), target ) );

    action->Attach( *new ActionTiming( xis, controllers_.controller_, simulation_, *action ) );
    action->Polish();

    tools::Iterator< const OrderParameter& > it = action->GetType().CreateIterator();
    if( target )
        xis >> list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
    else
        xis >> list( "parameter", *this, &ActionFactory::ReadParameter, *action, it );
    if( it.HasMoreElements() )
        ThrowMissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateKnowledgeGroupMagicAction
// Created: JSR 2010-04-20
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateKnowledgeGroupMagicAction( xml::xistream& xis ) const
{
    const unsigned long targetid = xml::attribute< unsigned long >( xis, "target" );
    const std::string id = xml::attribute< std::string >( xis, "id" );

    std::auto_ptr< actions::KnowledgeGroupMagicAction > action;
    const kernel::KnowledgeGroup_ABC* target = model_.knowledgeGroups_.Find( targetid );
    if( !target )
        ThrowTargetNotFound( targetid );

    action.reset( new actions::KnowledgeGroupMagicAction( xis, controllers_.controller_, magicActions_.Get( id ), *target ) );

    action->Attach( *new ActionTiming( xis, controllers_.controller_, simulation_, *action ) );
    action->Polish();

    tools::Iterator< const OrderParameter& > it = action->GetType().CreateIterator();
    xis >> list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
    if( it.HasMoreElements() )
        ThrowMissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::ReadParameter
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionFactory::ReadParameter( xml::xistream& xis, actions::Action_ABC& action, tools::Iterator< const OrderParameter& >& it, const Entity_ABC& entity ) const
{
    try
    {
        if( !it.HasMoreElements() )
            throw std::exception( tools::translate( "ActionFactory", "too many parameters provided" ) );
        std::auto_ptr< actions::Parameter_ABC > param( factory_.CreateParameter( it.NextElement(), xis, entity ) );
        action.AddParameter( *param );
        param.release();
    }
    catch( std::exception& e )
    {
        throw std::exception( tools::translate( "ActionFactory", "Parameter mismatch in action '%1' (id: %2): %3." )
                                .arg( action.GetName() ).arg( action.GetId() ).arg( e.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::ReadParameter
// Created: JSR 2010-04-02
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
