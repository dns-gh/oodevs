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
#include "ActionParameterFactory_ABC.h"

// -----------------------------------------------------------------------------
// Name: ActionFactory constructor
// Created: SBO 2007-03-12
// -----------------------------------------------------------------------------
ActionFactory::ActionFactory( kernel::Controllers& controllers, const ActionParameterFactory_ABC& factory
                            , const Model& model, const kernel::Resolver_ABC< kernel::Mission >& missions )
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
Action_ABC* ActionFactory::CreateAction( const kernel::Entity_ABC& target, const kernel::Mission& mission ) const
{
    return new ActionMission( target, mission, controllers_.controller_, true );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-03-19
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrder& fragOrder ) const
{
    return new ActionFragOrder( target, fragOrder, controllers_.controller_ );
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const ASN1T_MsgPionOrder& message ) const
{
    std::auto_ptr< Action_ABC > action( new ActionMission( model_.agents_.GetAgent( message.oid_unite_executante ), missions_.Get( message.mission ), controllers_.controller_, false ) );
    for( unsigned int i = 0; i < message.parametres.n; ++i )
        if( ActionParameter_ABC* param = factory_.CreateParameter( QString( "Parameter %1" ).arg( i ), message.parametres.elem[i] ) )
            action->AddParameter( *param );
    if( message.order_context.m.limite_gauchePresent )
        action->AddParameter( *factory_.CreateParameter( "Limit 1", message.order_context.limite_gauche ) );
    if( message.order_context.m.limite_droitePresent )
        action->AddParameter( *factory_.CreateParameter( "Limit 2", message.order_context.limite_droite ) );
    action->AddParameter( *factory_.CreateParameter( "Direction", message.order_context.direction_dangereuse ) );
    action->AddParameter( *factory_.CreateParameter( "Limas", message.order_context.limas ) );
    return action.release();
}

// -----------------------------------------------------------------------------
// Name: ActionFactory::CreateAction
// Created: SBO 2007-04-13
// -----------------------------------------------------------------------------
Action_ABC* ActionFactory::CreateAction( const ASN1T_MsgAutomateOrder& message ) const
{
    std::auto_ptr< Action_ABC > action( new ActionMission( model_.agents_.GetAutomat( message.oid_unite_executante ), missions_.Get( message.mission ), controllers_.controller_, false ) );
    for( unsigned int i = 0; i < message.parametres.n; ++i )
        if( ActionParameter_ABC* param = factory_.CreateParameter( QString( "Parameter %1" ).arg( i ), message.parametres.elem[i] ) )
            action->AddParameter( *param );
    if( message.order_context.m.limite_gauchePresent )
        action->AddParameter( *factory_.CreateParameter( "Limit 1", message.order_context.limite_gauche ) );
    if( message.order_context.m.limite_droitePresent )
        action->AddParameter( *factory_.CreateParameter( "Limit 2", message.order_context.limite_droite ) );
    action->AddParameter( *factory_.CreateParameter( "Direction", message.order_context.direction_dangereuse ) );
    action->AddParameter( *factory_.CreateParameter( "Limas", message.order_context.limas ) );
    return action.release();
}
