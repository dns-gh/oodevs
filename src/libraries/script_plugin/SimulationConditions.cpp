// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "SimulationConditions.h"
#include "EventCondition.h"
#include "MiscEvents.h"
#include "tools/ElementObserver_ABC.h"
#include "directia/Brain.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: SimulationConditions constructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
SimulationConditions::SimulationConditions( kernel::Controller& controller )
    : controller_( controller )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationConditions destructor
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
SimulationConditions::~SimulationConditions()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SimulationConditions::RegisterIn
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void SimulationConditions::RegisterIn( directia::Brain& brain )
{
    brain.RegisterObject( "events.sim", this );
    brain.RegisterFunction( "TickEnded",       &SimulationConditions::TickEnded );
    brain.RegisterFunction( "ClientConnected", &SimulationConditions::ClientConnected );
    brain.RegisterFunction( "ClientLeft",      &SimulationConditions::ClientLeft );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Condition_ABC > SimulationConditions::TickEnded
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > SimulationConditions::TickEnded()
{
    struct ClientConnected : public SimpleEventCondition< events::TickEnded >
    {
        ClientConnected( kernel::Controller& controller )
            : SimpleEventCondition( controller ) {}

        virtual void NotifyUpdated( const events::TickEnded& )
        {
            Trigger();
        };
    };

    return boost::shared_ptr< Condition_ABC >( new ClientConnected( controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Condition_ABC > SimulationConditions::ClientConnected
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > SimulationConditions::ClientConnected()
{
    struct ClientConnected : public SimpleEventCondition< events::ClientJoined >
    {
        ClientConnected( kernel::Controller& controller )
            : SimpleEventCondition( controller ) {}

        virtual void NotifyUpdated( const events::ClientJoined& client )
        {
            Trigger( client.client, client.profile );
        };
    };

    return boost::shared_ptr< Condition_ABC >( new ClientConnected( controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< Condition_ABC > SimulationConditions::ClientLeft
// Created: AGE 2008-07-09
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > SimulationConditions::ClientLeft()
{
    struct ClientLeft : public SimpleEventCondition< events::ClientLeft >
    {
        ClientLeft( kernel::Controller& controller )
            : SimpleEventCondition( controller ) {}

        virtual void NotifyUpdated( const events::ClientLeft& client )
        {
            Trigger( client.client );
        };
    };

    return boost::shared_ptr< Condition_ABC >( new ClientLeft( controller_ ) );
}
