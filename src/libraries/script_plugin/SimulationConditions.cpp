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
#include "directia/brain/Brain.h"

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

namespace directia
{
    void UsedByDIA( SimulationConditions* ) {}
    void ReleasedByDIA( SimulationConditions* ) {}
}

// -----------------------------------------------------------------------------
// Name: SimulationConditions::RegisterIn
// Created: AGE 2008-06-12
// -----------------------------------------------------------------------------
void SimulationConditions::RegisterIn( directia::brain::Brain& brain )
{
    brain[ "events.sim" ] = this;
    brain.Register( "TickEnded",       &SimulationConditions::TickEnded );
    brain.Register( "ClientConnected", &SimulationConditions::ClientConnected );
    brain.Register( "ClientLeft",      &SimulationConditions::ClientLeft );
}

// -----------------------------------------------------------------------------
// Name: SimulationConditions::TickEnded
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > SimulationConditions::TickEnded()
{
    struct ClientConnected : public SimpleEventCondition< events::TickEnded >
    {
        ClientConnected( kernel::Controller& controller )
            : SimpleEventCondition( controller )
        {
            // NOTHING
        }
        virtual void NotifyUpdated( const events::TickEnded& ev )
        {
            Trigger( ev.tick, ev.duration );
        }
    };
    return boost::shared_ptr< Condition_ABC >( new ClientConnected( controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationConditions::ClientConnected
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > SimulationConditions::ClientConnected()
{
    struct ClientConnected : public SimpleEventCondition< events::ClientJoined >
    {
        ClientConnected( kernel::Controller& controller )
            : SimpleEventCondition( controller )
        {
            // NOTHING
        }
        virtual void NotifyUpdated( const events::ClientJoined& client )
        {
            Trigger( client.client, client.profile );
        };
    };
    return boost::shared_ptr< Condition_ABC >( new ClientConnected( controller_ ) );
}

// -----------------------------------------------------------------------------
// Name: SimulationConditions::ClientLeft
// Created: AGE 2008-07-09
// -----------------------------------------------------------------------------
boost::shared_ptr< Condition_ABC > SimulationConditions::ClientLeft()
{
    struct ClientLeft : public SimpleEventCondition< events::ClientLeft >
    {
        ClientLeft( kernel::Controller& controller )
            : SimpleEventCondition( controller )
        {
            // NOTHING
        }
        virtual void NotifyUpdated( const events::ClientLeft& client )
        {
            Trigger( client.client );
        };
    };
    return boost::shared_ptr< Condition_ABC >( new ClientLeft( controller_ ) );
}
