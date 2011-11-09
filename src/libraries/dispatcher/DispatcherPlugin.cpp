// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "DispatcherPlugin.h"
#include "SimulationPublisher_ABC.h"
#include "LinkResolver_ABC.h"
#include "Profile_ABC.h"
#include "OrderResolver_ABC.h"
#include "Model.h"
#include "Services.h"
#include "Logger.h"
#include "tools/MessageDispatcher_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: DispatcherPlugin constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
DispatcherPlugin::DispatcherPlugin( Model& model, SimulationPublisher_ABC& simulation, tools::MessageDispatcher_ABC& clientCommands,
                                    LinkResolver_ABC& links, OrderResolver_ABC& order, RotatingLog& log )
    : model_     ( model )
    , simulation_( simulation )
    , links_     ( links )
    , order_     ( order )
{
    clientCommands.RegisterMessage( MakeLogger( log, "Dispatcher received : ", *this, &DispatcherPlugin::OnReceive ) );
}

// -----------------------------------------------------------------------------
// Name: DispatcherPlugin destructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
DispatcherPlugin::~DispatcherPlugin()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DispatcherPlugin::Register
// Created: AGE 2008-08-13
// -----------------------------------------------------------------------------
void DispatcherPlugin::Register( dispatcher::Services& services )
{
    services.Declare< simulation::Service >();
}

// -----------------------------------------------------------------------------
// Name: DispatcherPlugin::Receive
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void DispatcherPlugin::Receive( const sword::SimToClient& message )
{
    for( CIT_Clients it = clients_.begin(); it != clients_.end(); ++it )
        (*it)->Send( message );
}

// -----------------------------------------------------------------------------
// Name: DispatcherPlugin::NotifyClientAuthenticated
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void DispatcherPlugin::NotifyClientAuthenticated( ClientPublisher_ABC& client, Profile_ABC& /*profile*/ )
{
    clients_.insert( &client );
    model_.Send( client );
}

// -----------------------------------------------------------------------------
// Name: DispatcherPlugin::NotifyClientLeft
// Created: AGE 2007-08-27
// -----------------------------------------------------------------------------
void DispatcherPlugin::NotifyClientLeft( ClientPublisher_ABC& client )
{
    clients_.erase( &client );
}

// -----------------------------------------------------------------------------
// Name: DispatcherPlugin::OnReceive
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void DispatcherPlugin::OnReceive( const std::string& link, const sword::ClientToSim& asnMsg )
{
    if( links_.GetProfile( link ).CheckRights( asnMsg ) )
    {
        order_.Resolve( asnMsg );
        simulation_.Send( asnMsg );
    }
}
