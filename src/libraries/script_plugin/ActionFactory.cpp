// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ActionFactory.h"
#include "dispatcher/Model.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Population.h"
#include "clients_kernel/OrderType.h"
#include "actions/AgentMission.h"
#include "actions/AutomatMission.h"
#include "actions/PopulationMission.h"
#include "actions/FragOrder.h"
#include "actions/ParameterFactory_ABC.h"
#include <xeumeuleu/xml.h>

using namespace plugins::script;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionFactory constructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
ActionFactory::ActionFactory( kernel::Controller& controller, const dispatcher::Model& model, const actions::ParameterFactory_ABC& factory )
    : controller_( controller )
    , model_     ( model )
    , missions_  ( model.GetMissionTypes() )
    , fragOrders_( model.GetFragOrderTypes() )
    , factory_   ( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionFactory destructor
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
ActionFactory::~ActionFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( const kernel::Entity_ABC& target, const kernel::MissionType& mission ) const
{
    if( target.GetTypeName() == Agent_ABC::typeName_ )
        return new actions::AgentMission( target, mission, controller_, true );
    else if( target.GetTypeName() == Automat_ABC::typeName_ )
        return new actions::AutomatMission( target, mission, controller_, true );
    else if( target.GetTypeName() == Population_ABC::typeName_ )
        return new actions::PopulationMission( target, mission, controller_, true );
    else
        throw std::runtime_error( __FUNCTION__ );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrderType& fragOrder ) const
{
    return new actions::FragOrder( target, fragOrder, controller_, true );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
actions::Action_ABC* ActionFactory::CreateAction( xml::xistream& xis ) const
{
    std::auto_ptr< actions::Action_ABC > result;
    const kernel::Entity_ABC& target = ResolveEntity( xml::attribute< unsigned long >( xis, "target" ) );
    const std::string type = xml::attribute< std::string >( xis, "type" );

    if( type == "fragorder" )
        result.reset( new actions::FragOrder( xis, controller_, fragOrders_, target ) );
    else if( type == "mission" && target.GetTypeName() == Agent_ABC::typeName_ )
        result.reset( new actions::AgentMission( xis, controller_, missions_, target ) );
    else if( type == "mission" && target.GetTypeName() == Automat_ABC::typeName_ )
        result.reset( new actions::AutomatMission( xis, controller_, missions_, target ) );
    else if( type == "mission" && target.GetTypeName() == Population_ABC::typeName_ )
        result.reset( new actions::PopulationMission( xis, controller_, missions_, target ) );
    else 
        throw std::runtime_error( __FUNCTION__ );

    Iterator< const OrderParameter& > it = result->GetType().CreateIterator();
    xis >> list( "parameter", *this, &ActionFactory::ReadParameter, *result, it, target );
    if( it.HasMoreElements() )
        throw std::runtime_error( __FUNCTION__ );
    return result.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::ReadParameter
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
void ActionFactory::ReadParameter( xml::xistream& xis, actions::Action_ABC& action, kernel::Iterator< const kernel::OrderParameter& >& it, const kernel::Entity_ABC& entity ) const
{
    if( !it.HasMoreElements() )
        throw std::exception( __FUNCTION__ );
    action.AddParameter( *factory_.CreateParameter( it.NextElement(), xis, entity ) );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::ResolveEntity
// Created: AGE 2008-07-16
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& ActionFactory::ResolveEntity( const unsigned long id ) const
{
    const kernel::Entity_ABC* result = model_.agents_.Find( id );
    if( !result ) result = model_.automats_.Find( id );
    if( !result ) result = model_.populations_.Find( id );
    if( !result )
        throw std::runtime_error( __FUNCTION__ );
    return *result;
}
