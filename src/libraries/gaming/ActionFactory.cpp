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
#include "ActionMission.h"
#include "ActionFragOrder.h"
#include "Model.h"
#include "AgentsModel.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/MissionType.h"
#include "clients_kernel/OrderParameter.h"
#include "ActionParameterFactory_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: ActionFactory constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionFactory::ActionFactory( Controllers& controllers, const ActionParameterFactory_ABC& factory, const Model& model
                            , const Resolver_ABC< MissionType >& missions )
    : controllers_( controllers )
    , factory_( factory )
    , model_( model )
    , missions_( missions )
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
    return new ActionMission( target, mission, controllers_.controller_, true );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const Entity_ABC& target, const FragOrderType& fragOrder ) const
{
    return new ActionFragOrder( target, fragOrder, controllers_.controller_ );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const ASN1T_MsgPionOrder& message ) const
{
    const MissionType& mission = missions_.Get( message.mission );
    std::auto_ptr< Action_ABC > action( new ActionMission( model_.agents_.GetAgent( message.oid_unite_executante ), mission, controllers_.controller_, false ) );
    AddParameters( *action, mission, message.parametres );
    AddOrderContext( *action, message.order_context );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const ASN1T_MsgAutomateOrder& message ) const
{
    const MissionType& mission = missions_.Get( message.mission );
    std::auto_ptr< Action_ABC > action( new ActionMission( model_.agents_.GetAutomat( message.oid_unite_executante ), mission, controllers_.controller_, false ) );
    AddParameters( *action, mission, message.parametres );
    AddOrderContext( *action, message.order_context );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AddParameters
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
void ActionFactory::AddParameters( Action_ABC& action, const OrderType& mission, const ASN1T_MissionParameters& asn ) const
{
    Iterator< const OrderParameter& > params = mission.CreateIterator();
    for( unsigned int i = 0; i < asn.n && params.HasMoreElements(); ++i )
        if( ActionParameter_ABC* param = factory_.CreateParameter( params.NextElement().GetName(), asn.elem[i], action.GetEntity() ) )
            action.AddParameter( *param );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::AddOrderContext
// Created: SBO 2007-04-19
// -----------------------------------------------------------------------------
void ActionFactory::AddOrderContext( Action_ABC& action, const ASN1T_OrderContext& asn ) const
{
    if( asn.m.limite_gauchePresent )
        action.AddParameter( *factory_.CreateParameter( tools::translate( "ActionFactory", "Limit 1" ), asn.limite_gauche ) );
    if( asn.m.limite_droitePresent )
        action.AddParameter( *factory_.CreateParameter( tools::translate( "ActionFactory", "Limit 2" ), asn.limite_droite ) );
    action.AddParameter( *factory_.CreateParameter( tools::translate( "ActionFactory", "Dangerous direction" ), asn.direction_dangereuse ) );
    action.AddParameter( *factory_.CreateParameter( tools::translate( "ActionFactory", "Limas" ), asn.limas ) );
}
