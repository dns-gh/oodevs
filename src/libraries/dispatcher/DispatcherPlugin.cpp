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
#include "Model.h"
#include "ClientPublisher_ABC.h"
#include "Services.h"
#include "tools/MessageDispatcher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: DispatcherPlugin constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
DispatcherPlugin::DispatcherPlugin( Model& model, SimulationPublisher_ABC& simulation, tools::MessageDispatcher_ABC& clientCommands, LinkResolver_ABC& links )
    : model_     ( model )
    , simulation_( simulation )
    , links_     ( links )
{
    clientCommands.RegisterMessage( *this, &DispatcherPlugin::OnReceive );
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
void DispatcherPlugin::Receive( const ASN1T_MsgsSimToClient& message )
{
    for( std::set< ClientPublisher_ABC* >::const_iterator it = clients_.begin(); it != clients_.end(); ++it )
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
void DispatcherPlugin::OnReceive( const std::string& link, const ASN1T_MsgsClientToSim& asnMsg )
{
    if( links_.GetProfile( link ).CheckRights( asnMsg ) )
        simulation_.Send( asnMsg );
}
