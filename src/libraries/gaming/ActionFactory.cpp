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
#include "ActionFragOrder.h"
#include "Model.h"
#include "AgentsModel.h"
#include "ActionTiming.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
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
    else
        return 0;

    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );
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
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const ASN1T_MsgUnitOrder& message ) const
{
    const MissionType& mission = missions_.Get( message.mission );
    std::auto_ptr< Action_ABC > action( new ActionAgentMission( model_.agents_.GetAgent( message.oid_unite_executante ), mission, controllers_.controller_, false ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );

    AddParameters( *action, mission, message.parametres );
    AddOrderContext( *action, mission, message.order_context );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const ASN1T_MsgAutomatOrder& message ) const
{
    const MissionType& mission = missions_.Get( message.mission );
    std::auto_ptr< Action_ABC > action( new ActionAutomatMission( model_.agents_.GetAutomat( message.oid_unite_executante ), mission, controllers_.controller_, false ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );

    AddParameters( *action, mission, message.parametres );
    AddOrderContext( *action, mission, message.order_context );
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
    if( const Agent_ABC* agent = model_.agents_.FindAgent( message.oid_unite_executante ) )
        action.reset( new ActionFragOrder( *agent, order, controllers_.controller_, false ) );
    else if( const Automat_ABC* automat = model_.agents_.FindAutomat( message.oid_unite_executante ) )
        action.reset( new ActionFragOrder( *automat, order, controllers_.controller_, false ) );
    action->Attach( *new ActionTiming( controllers_.controller_, simulation_, *action ) );

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
        const OrderParameter& param = it.NextElement();
        if( param.IsContext() )
            continue;
        if( i >= asn.n )
            throw std::runtime_error( "Mission parameter count does not match mission definition" );
        if( ActionParameter_ABC* newParam = factory_.CreateParameter( param, asn.elem[i++], action.GetEntity() ) )
            action.AddParameter( *newParam );
    }
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AddOrderContext
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
void ActionFactory::AddOrderContext( Action_ABC& action, const OrderType& order, const ASN1T_OrderContext& asn ) const
{
    Iterator< const OrderParameter& > it = order.CreateIterator();
    while( it.HasMoreElements() )
    {
        const OrderParameter& param = it.NextElement();
        if( !param.IsContext() )
            continue;
        if( param.GetType() == "limits" && asn.m.limite_droitePresent && asn.m.limite_gauchePresent )
            action.AddParameter( *factory_.CreateParameter( param, asn.limite_gauche, asn.limite_droite ) );
        else if( param.GetType() == "limalist" )
            action.AddParameter( *factory_.CreateParameter( param, asn.limas ) );
        else if( param.GetType() == "dangerousdirection" )
            action.AddParameter( *factory_.CreateParameter( param, asn.direction_dangereuse ) );
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

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateMission
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateMission( xml::xistream& xis ) const
{
    unsigned long id;
    xis >> attribute( "target", id );

    std::auto_ptr< ActionMission > action;
    const kernel::Entity_ABC* target = model_.agents_.FindAgent( id );
    if( target )
        action.reset( new ActionAgentMission( xis, controllers_.controller_, missions_, *target ) );
    else if( target = model_.agents_.FindAutomat( id ) )
        action.reset( new ActionAutomatMission( xis, controllers_.controller_, missions_, *target ) );
    else
        throw std::runtime_error( tools::translate( "ActionFactory", "Mission's entity '%1' not found." ).arg( id ).ascii() );

    action->Attach( *new ActionTiming( xis, controllers_.controller_, simulation_, *action ) );

    Iterator< const OrderParameter& > it = action->GetType().CreateIterator();
    xis >> start( "parameters" )
            >> list( "parameter", *this, ReadParameter, *action, it, *target )
        >> end()
        >> start( "context" )
            >> list( "parameter", *this, ReadParameter, *action, it, *target )
        >> end();
    if( it.HasMoreElements() )
        throw std::runtime_error( tools::translate( "ActionFactory", "Parameter mismatch: too few parameters provided." ).ascii() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateFragOrder
// Created: SBO 2007-06-26
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateFragOrder( xml::xistream& xis ) const
{
    unsigned long id;
    xis >> attribute( "target", id );

    std::auto_ptr< ActionFragOrder > action;
    const kernel::Entity_ABC* target = model_.agents_.FindAgent( id );
    if( !target )
        target = model_.agents_.FindAutomat( id );
    if( !target )
        throw std::runtime_error( tools::translate( "ActionFactory", "Frag order's entity '%1' not found." ).arg( id ).ascii() );
    action.reset( new ActionFragOrder( xis, controllers_.controller_, fragOrders_, *target ) );

    action->Attach( *new ActionTiming( xis, controllers_.controller_, simulation_, *action ) );

    Iterator< const OrderParameter& > it = action->GetType().CreateIterator();
    xis >> start( "parameters" )
            >> list( "parameter", *this, ReadParameter, *action, it, *target )
        >> end();
    if( it.HasMoreElements() )
        throw std::runtime_error( tools::translate( "ActionFactory", "Parameter mismatch: too few parameters provided." ).ascii() );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::ReadParameter
// Created: SBO 2007-05-16
// -----------------------------------------------------------------------------
void ActionFactory::ReadParameter( xml::xistream& xis, Action_ABC& action, Iterator< const OrderParameter& >& it, const Entity_ABC& entity ) const
{
    if( !it.HasMoreElements() )
        throw std::runtime_error( tools::translate( "ActionFactory", "Parameter mismatch: too much parameters provided." ).ascii() );
    if( ActionParameter_ABC* param = factory_.CreateParameter( it.NextElement(), xis, entity ) )
        action.AddParameter( *param );
}
