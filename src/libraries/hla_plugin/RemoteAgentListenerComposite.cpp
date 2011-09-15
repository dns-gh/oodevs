// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "RemoteAgentListenerComposite.h"
#include <boost/foreach.hpp>
#include <algorithm>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: RemoteAgentListenerComposite constructor
// Created: SLI 2011-09-02
// -----------------------------------------------------------------------------
RemoteAgentListenerComposite::RemoteAgentListenerComposite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentListenerComposite destructor
// Created: SLI 2011-09-02
// -----------------------------------------------------------------------------
RemoteAgentListenerComposite::~RemoteAgentListenerComposite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentListenerComposite::Register
// Created: SLI 2011-09-02
// -----------------------------------------------------------------------------
void RemoteAgentListenerComposite::Register( RemoteAgentListener_ABC& listener )
{
    listeners_.push_back( &listener );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentListenerComposite::Unregister
// Created: SLI 2011-09-02
// -----------------------------------------------------------------------------
void RemoteAgentListenerComposite::Unregister( RemoteAgentListener_ABC& listener )
{
    listeners_.erase( std::remove( listeners_.begin(), listeners_.end(), &listener ), listeners_.end() );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentListenerComposite::Created
// Created: SLI 2011-09-02
// -----------------------------------------------------------------------------
void RemoteAgentListenerComposite::Created( const std::string& identifier )
{
    BOOST_FOREACH( RemoteAgentListener_ABC* listener, listeners_ )
        listener->Created( identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentListenerComposite::Destroyed
// Created: SLI 2011-09-02
// -----------------------------------------------------------------------------
void RemoteAgentListenerComposite::Destroyed( const std::string& identifier )
{
    BOOST_FOREACH( RemoteAgentListener_ABC* listener, listeners_ )
        listener->Destroyed( identifier );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentListenerComposite::Moved
// Created: SLI 2011-09-02
// -----------------------------------------------------------------------------
void RemoteAgentListenerComposite::Moved( const std::string& identifier, double latitude, double longitude )
{
    BOOST_FOREACH( RemoteAgentListener_ABC* listener, listeners_ )
        listener->Moved( identifier, latitude, longitude );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentListenerComposite::SideChanged
// Created: VPR 2011-09-07
// -----------------------------------------------------------------------------
void RemoteAgentListenerComposite::SideChanged( const std::string& identifier, rpr::ForceIdentifier side )
{
    BOOST_FOREACH( RemoteAgentListener_ABC* listener, listeners_ )
        listener->SideChanged( identifier, side );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentListenerComposite::NameChanged
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
void RemoteAgentListenerComposite::NameChanged( const std::string& identifier, const std::string& name )
{
    BOOST_FOREACH( RemoteAgentListener_ABC* listener, listeners_ )
        listener->NameChanged( identifier, name );
}

// -----------------------------------------------------------------------------
// Name: RemoteAgentListenerComposite::TypeChanged
// Created: SLI 2011-09-14
// -----------------------------------------------------------------------------
void RemoteAgentListenerComposite::TypeChanged( const std::string& identifier, const rpr::EntityType& type )
{
    BOOST_FOREACH( RemoteAgentListener_ABC* listener, listeners_ )
        listener->TypeChanged( identifier, type );
}
