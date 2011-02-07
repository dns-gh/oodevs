// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "AgentProxy.h"
#include "EventListener_ABC.h"
#include "dispatcher/Agent_ABC.h"
#include <algorithm>
#include <boost/foreach.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: AgentProxy constructor
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
AgentProxy::AgentProxy( dispatcher::Agent_ABC& agent )
    : dispatcher::Observer< sword::UnitAttributes >( agent )
    , dispatcher::Observer< sword::UnitEnvironmentType >( agent )
    , agent_( agent )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentProxy destructor
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
AgentProxy::~AgentProxy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::Register
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AgentProxy::Register( EventListener_ABC& listener )
{
    listeners_.push_back( &listener );
    listener.SpatialChanged( agent_.GetPosition().X(), agent_.GetPosition().Y(),
                             agent_.GetAltitude(), agent_.GetSpeed(), agent_.GetDirection() );
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::Unregister
// Created: SLI 2011-02-07
// -----------------------------------------------------------------------------
void AgentProxy::Unregister( EventListener_ABC& listener )
{
    listeners_.erase( std::remove( listeners_.begin(), listeners_.end(), &listener ), listeners_.end() );
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::GetEquipments
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
const tools::Resolver< dispatcher::Equipment >& AgentProxy::GetEquipments() const
{
    return agent_.Equipments();
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::Notify
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
void AgentProxy::Notify( const sword::UnitAttributes& attributes )
{
    if( attributes.has_position() || attributes.has_height() || attributes.has_speed() || attributes.has_direction() )
        BOOST_FOREACH( EventListener_ABC* listener, listeners_ )
            listener->SpatialChanged( agent_.GetPosition().X(), agent_.GetPosition().Y(),
                                      agent_.GetAltitude(), agent_.GetSpeed(), agent_.GetDirection() );
    dispatcher::Observable< sword::UnitAttributes >::Notify( attributes );
}

// -----------------------------------------------------------------------------
// Name: AgentProxy::Notify
// Created: SLI 2011-02-04
// -----------------------------------------------------------------------------
void AgentProxy::Notify( const sword::UnitEnvironmentType& attributes )
{
    dispatcher::Observable< sword::UnitEnvironmentType >::Notify( attributes );
}
