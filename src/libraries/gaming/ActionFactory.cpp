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
#include "ActionAgentMission.h"
#include "ActionAutomatMission.h"
#include "ActionPopulationMission.h"
#include "ActionFragOrder.h"
#include "Model.h"
#include "AgentsModel.h"
#include "ActionTiming.h"
#include "ActionParameter_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/FragOrderType.h"
#include "clients_kernel/OrderParameter.h"
#include "ActionParameterFactory_ABC.h"
#include "Tools.h"
#include "xeumeuleu/xml.h"

using namespace kernel;
using namespace xml;

// -----------------------------------------------------------------------------
// Name: ActionFactory constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionFactory::ActionFactory( Controllers& controllers, const ActionParameterFactory_ABC& factory, const Model& model
                            , const Resolver_ABC< MissionType >& missions, const Resolver_ABC< FragOrderType >& fragOrders, const Simulation& simulation )
    : controllers_( controllers )
    , factory_( factory )
    , model_( model )
    , missions_( missions )
    , fragOrders_( fragOrders )
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

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const Entity_ABC& target, const MissionType& mission ) const
{
    std::auto_ptr< Action_ABC > action;
    if( model_.agents_.FindAgent( target.GetId() ) ) 
        action.reset( new ActionAgentMission( target, mission, controllers_.controller_, true ) );
    else if( model_.agents_.FindAutomat( target.GetId() ) )
        action.reset( new ActionAutomatMission( target, mission, controllers_.controller_, true ) );
    else if( model_.agents_.FindPopulation( target.GetId() ) )
        action.reset( new ActionPopulationMission( target, mission, controllers_.controller_, true ) );
    else
        throw std::runtime_error( __FUNCTION__ " Cannot resolve executing entity" );

    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const Entity_ABC& target, const FragOrderType& fragOrder ) const
{
    std::auto_ptr< Action_ABC > action( new ActionFragOrder( target, fragOrder, controllers_.controller_, true ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->Polish();
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const ASN1T_MsgUnitOrder& message ) const
{
    const MissionType& mission = missions_.Get( message.mission );
    std::auto_ptr< Action_ABC > action( new ActionAgentMission( model_.agents_.GetAgent( message.oid ), mission, controllers_.controller_, false ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->Polish();

    AddParameters( *action, mission, message.parametres );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const ASN1T_MsgAutomatOrder& message ) const
{
    const MissionType& mission = missions_.Get( message.mission );
    std::auto_ptr< Action_ABC > action( new ActionAutomatMission( model_.agents_.GetAutomat( message.oid ), mission, controllers_.controller_, false ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->Polish();

    AddParameters( *action, mission, message.parametres );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: AGE 2007-07-11
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const ASN1T_MsgPopulationOrder& message ) const
{
    const MissionType& mission = missions_.Get( message.mission );
    std::auto_ptr< Action_ABC > action( new ActionPopulationMission( model_.agents_.GetPopulation( message.oid ), mission, controllers_.controller_, false ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->Polish();

    AddParameters( *action, mission, message.parametres );
    return action.release();
}


// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const ASN1T_MsgFragOrder& message ) const
{
    const FragOrderType& order = fragOrders_.Get( message.frag_order );
    std::auto_ptr< Action_ABC > action;
    if( const Agent_ABC* agent = model_.agents_.FindAgent( message.oid ) )
        action.reset( new ActionFragOrder( *agent, order, controllers_.controller_, false ) );
    else if( const Automat_ABC* automat = model_.agents_.FindAutomat( message.oid ) )
        action.reset( new ActionFragOrder( *automat, order, controllers_.controller_, false ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
    action->Polish();

    AddParameters( *action, order, message.parametres );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AddParameters
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
void ActionFactory::AddParameters( Action_ABC& action, const OrderType& order, const ASN1T_MissionParameters& asn ) const
{
    Iterator< const OrderParameter& > it = order.CreateIterator();
    unsigned int i = 0;
    while( it.HasMoreElements() )
    {
        if( i >= asn.n )
            throw std::runtime_error( __FUNCTION__ " Mission parameter count does not match mission definition" );
        if( ActionParameter_ABC* newParam = factory_.CreateParameter( it.NextElement(), asn.elem[i++], action.GetEntity() ) )
            action.AddParameter( *newParam );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( xml::xistream& xis ) const
{
    std::string type;
    xis >> attribute( "type", type );
    if( type == "mission" )
        return CreateMission( xis );
    if( type == "fragorder" )
        return CreateFragOrder( xis );
    return 0;
}

namespace
{
    void ThrowMissingParameter( const Action_ABC& action, const OrderParameter& param )
    {
        throw std::exception( tools::translate( "ActionFactory", "Parameter mismatch in action '%1' (id: %2): missing parameter '%3'." )
                                .arg( action.GetName() ).arg( action.GetId() ).arg( param.GetName().c_str() ) );
    }

    void ThrowTargetNotFound( unsigned long id )
    {
        throw std::exception( tools::translate( "ActionFactory", "Unable to find executing entity '%1'." ).arg( id ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateMission
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateMission( xml::xistream& xis ) const
{
    const unsigned long id = xml::attribute< unsigned long >( xis, "target" );

    std::auto_ptr< ActionMission > action;
    const kernel::Entity_ABC* target = model_.agents_.FindAgent( id );
    if( target )
        action.reset( new ActionAgentMission( xis, controllers_.controller_, missions_, *target ) );
    else if( target = model_.agents_.FindAutomat( id ) )
        action.reset( new ActionAutomatMission( xis, controllers_.controller_, missions_, *target ) );
    else
        ThrowTargetNotFound( id );

    action->Attach( *new ActionTiming( xis, controllers_.controller_, simulation_, *action ) );
    action->Polish();

    Iterator< const OrderParameter& > it = action->GetType().CreateIterator();
    xis >> list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
    if( it.HasMoreElements() )
        ThrowMissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateFragOrder
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateFragOrder( xml::xistream& xis ) const
{
    const unsigned long id = xml::attribute< unsigned long >( xis, "target" );

    std::auto_ptr< ActionFragOrder > action;
    const kernel::Entity_ABC* target = model_.agents_.FindAgent( id );
    if( !target )
        target = model_.agents_.FindAutomat( id );
    if( !target )
        ThrowTargetNotFound( id );
    action.reset( new ActionFragOrder( xis, controllers_.controller_, fragOrders_, *target ) );

    action->Attach( *new ActionTiming( xis, controllers_.controller_, simulation_, *action ) );
    action->Polish();

    Iterator< const OrderParameter& > it = action->GetType().CreateIterator();
    xis >> list( "parameter", *this, &ActionFactory::ReadParameter, *action, it, *target );
    if( it.HasMoreElements() )
        ThrowMissingParameter( *action, it.NextElement() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::ReadParameter
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionFactory::ReadParameter( xml::xistream& xis, Action_ABC& action, Iterator< const OrderParameter& >& it, const Entity_ABC& entity ) const
{
    try
    {
        if( !it.HasMoreElements() )
            throw std::exception( tools::translate( "ActionFactory", "too much parameters provided" ) );
        std::auto_ptr< ActionParameter_ABC > param( factory_.CreateParameter( it.NextElement(), xis, entity ) );
        action.AddParameter( *param );
        param.release();
    }
    catch( std::exception& e )
    {
        throw std::exception( tools::translate( "ActionFactory", "Parameter mismatch in action '%1' (id: %2): %3." )
                                .arg( action.GetName() ).arg( action.GetId() ).arg( e.what() ) );
    }
}
