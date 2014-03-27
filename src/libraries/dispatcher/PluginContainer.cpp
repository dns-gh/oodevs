// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "PluginContainer.h"

using namespace dispatcher;

void PluginContainer::Add( const boost::shared_ptr< Plugin_ABC >& plugin )
{
    plugins_.push_back( plugin );
    handlers_.push_back( plugin );
}

void PluginContainer::AddHandler( const boost::shared_ptr< MessageHandler_ABC >& handler )
{
    handlers_.push_back( handler );
}

void PluginContainer::Receive( const sword::SimToClient& message )
{
    if( ForwardSimToClient( message ) )
        for( auto it = handlers_.begin(); it != handlers_.end(); ++it )
            (*it)->Receive( message );
}

void PluginContainer::Receive( const sword::AarToClient& message )
{
    for( auto it = handlers_.begin(); it != handlers_.end(); ++it )
        (*it)->Receive( message );
}

void PluginContainer::Receive( const sword::MessengerToClient& message )
{
    for( auto it = handlers_.begin(); it != handlers_.end(); ++it )
        (*it)->Receive( message );
}

void PluginContainer::Register( dispatcher::Services& services )
{
    for( auto it = plugins_.begin(); it != plugins_.end(); ++it )
        (*it)->Register( services );
}

void PluginContainer::NotifyClientAuthenticated( ClientPublisher_ABC& client,
        const std::string& link, Profile_ABC& profile, bool uncounted )
{
    for( auto it = plugins_.begin(); it != plugins_.end(); ++it )
        (*it)->NotifyClientAuthenticated( client, link, profile, uncounted );
}

void PluginContainer::NotifyClientLeft( ClientPublisher_ABC& client,
        const std::string& link, bool uncounted )
{
    for( auto it = plugins_.begin(); it != plugins_.end(); ++it )
        (*it)->NotifyClientLeft( client, link, uncounted );
}

void PluginContainer::NotifySimulationLeft()
{
    for( auto it = plugins_.begin(); it != plugins_.end(); ++it )
        (*it)->NotifySimulationLeft();
}

void PluginContainer::Update()
{
    for( auto it = plugins_.begin(); it != plugins_.end(); ++it )
        (*it)->Update();
}

void PluginContainer::Close()
{
    for( auto it = plugins_.begin(); it != plugins_.end(); ++it )
        (*it)->Close();
}

bool PluginContainer::ForwardSimToClient( const sword::SimToClient& )
{
    return true;
}

bool PluginContainer::HandleClientToSim( const sword::ClientToSim& msg,
        RewritingPublisher_ABC& unicaster, ClientPublisher_ABC& broadcaster )
{
    for( auto it = plugins_.begin(); it != plugins_.end(); ++it )
        if( (*it)->HandleClientToSim( msg, unicaster, broadcaster ))
            return true;
    return false;
}

bool PluginContainer::HandleClientToReplay( const sword::ClientToReplay& msg,
        RewritingPublisher_ABC& unicaster, ClientPublisher_ABC& broadcaster )
{
    for( auto it = plugins_.begin(); it != plugins_.end(); ++it )
        if( (*it)->HandleClientToReplay( msg, unicaster, broadcaster ))
            return true;
    return false;
}
