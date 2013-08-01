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
#include "Services.h"
#include "Logger.h"
#include "ClientsNetworker.h"
#include "protocol/SimulationSenders.h"
#include "protocol/RightsHelper.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: DispatcherPlugin constructor
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
DispatcherPlugin::DispatcherPlugin( SimulationPublisher_ABC& simulation, ClientsNetworker& clients,
                                    LinkResolver_ABC& links, OrderResolver_ABC& order, tools::RotatingLog& log )
    : simulation_( simulation )
    , links_     ( links )
    , order_     ( order )
    , clients_   ( clients )
{
    clients.RegisterMessage( MakeLogger( log, "Dispatcher received : ", *this, &DispatcherPlugin::OnReceive ) );
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
void DispatcherPlugin::Receive( const sword::SimToClient& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DispatcherPlugin::OnReceive
// Created: AGE 2007-08-24
// -----------------------------------------------------------------------------
void DispatcherPlugin::OnReceive( const std::string& link, sword::ClientToSim& message )
{
    if( links_.GetProfile( link ).CheckRights( message ) )
    {
        if( unsigned int clientID = links_.GetClientID( link ) )
            message.set_client_id( clientID );
        order_.Resolve( message );
        simulation_.Send( message );
    }
    else
    {
        sword::SimToClient error;
        error.set_context( message.context() );
        if( !protocol::GetForbiddenError( message, error ))
        {
            // Irrelevant but better than no error
            auto m = error.mutable_message()->mutable_magic_action_ack();
            m->set_error_code( sword::MagicActionAck::error_invalid_parameter );
        }
        if( unsigned int clientID = links_.GetClientID( link ) )
            error.set_client_id( clientID );
        clients_.Send( error );
    }
}
