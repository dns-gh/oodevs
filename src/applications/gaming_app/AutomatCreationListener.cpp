// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "AutomatCreationListener.h"
#include "actions/Action_ABC.h"
#include "actions/ActionsModel.h"
#include "actions/ActionTasker.h"
#include "actions/ActionTiming.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatComposition.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/AgentType.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: AutomatCreationListener constructor
// Created: LDC 2010-10-11
// -----------------------------------------------------------------------------
AutomatCreationListener::AutomatCreationListener( const geometry::Point2f& point, const kernel::AutomatType& type, int context,
                 tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, kernel::Controller& controller, kernel::AgentTypes& agentTypes,
                 kernel::CoordinateConverter_ABC& coordinateConverter, actions::ActionsModel& actionsModel, const kernel::Time_ABC& time )
    : point_( point)
    , type_( type )
    , context_ ( context )
    , automatResolver_( automatResolver )
    , controller_( controller )
    , agentTypes_( agentTypes )
    , coordinateConverter_( coordinateConverter )
    , actionsModel_( actionsModel )
    , time_( time )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatCreationListener destructor
// Created: LDC 2010-10-11
// -----------------------------------------------------------------------------
AutomatCreationListener::~AutomatCreationListener()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatCreationListener::OnMessageReceived
// Created: LDC 2010-10-11
// -----------------------------------------------------------------------------
bool AutomatCreationListener::OnMessageReceived( const sword::SimToClient& message )
{
    if( message.context() != context_ || !message.message().has_automat_creation() )
        return false;
    const sword::AutomatCreation& creation = message.message().automat_creation();
    kernel::Automat_ABC* automat = automatResolver_.Find( creation.automat().id() );
    if( !automat )
        return false;
    tools::Iterator< const kernel::AutomatComposition& > it = type_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::AutomatComposition& composition = it.NextElement();
        const kernel::AgentType& agentType = composition.GetType();
        unsigned int number = composition.GetSensibleNumber();
        for( unsigned int i = 0; i < number; ++i )
        {
            actions::Action_ABC* action = actionsModel_.CreateAgentCreationAction( agentType, point_, *automat, controller_, agentTypes_, coordinateConverter_ );
            action->Attach( *new actions::ActionTiming( controller_, time_ ) );
            action->Attach( *new actions::ActionTasker( automat, false ) );
            actionsModel_.Publish( *action );
        }
    }
    return true;
}
