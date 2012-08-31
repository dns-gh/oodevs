// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************


#include "hla_plugin_pch.h"
#include "EventListenerComposite.h"
#include <algorithm>
#include <boost/foreach.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: EventListenerComposite constructor
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
EventListenerComposite::EventListenerComposite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite destructor
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
EventListenerComposite::~EventListenerComposite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::Register
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void EventListenerComposite::Register( EventListener_ABC& listener )
{
    listeners_.push_back( &listener );
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::Unregister
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void EventListenerComposite::Unregister( EventListener_ABC& listener )
{
    listeners_.erase( std::remove( listeners_.begin(), listeners_.end(), &listener ), listeners_.end() );
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::Unregister
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void EventListenerComposite::SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction )
{
    BOOST_FOREACH( EventListener_ABC* listener, listeners_ )
        listener->SpatialChanged( latitude, longitude, altitude, speed, direction );
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::Unregister
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void EventListenerComposite::FormationChanged( bool isOnRoad )
{
    BOOST_FOREACH( EventListener_ABC* listener, listeners_ )
        listener->FormationChanged( isOnRoad );
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::Unregister
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void EventListenerComposite::EquipmentChanged( unsigned int type, const rpr::EntityType& entityType, unsigned int available )
{
    BOOST_FOREACH( EventListener_ABC* listener, listeners_ )
        listener->EquipmentChanged( type, entityType, available );
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::Unregister
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void EventListenerComposite::EmbarkmentChanged( bool mounted )
{
    BOOST_FOREACH( EventListener_ABC* listener, listeners_ )
        listener->EmbarkmentChanged( mounted );
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::PlatformAdded
// Created: AHC 2012-07-30
// -----------------------------------------------------------------------------
void EventListenerComposite::PlatformAdded( const std::string& name, unsigned int id )
{
    BOOST_FOREACH( EventListener_ABC* listener, listeners_ )
        listener->PlatformAdded( name, id );
}
