// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "EventListenerComposite.h"

#include <boost/foreach.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: EventListenerComposite constructor
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
EventListenerComposite::EventListenerComposite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite destructor
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
EventListenerComposite::~EventListenerComposite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::Register
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void EventListenerComposite::Register( EventListener_ABC& listener )
{
    listeners_.insert( &listener );
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::Unregister
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void EventListenerComposite::Unregister( EventListener_ABC& listener )
{
    listeners_.erase( &listener );
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::SpatialChanged
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void EventListenerComposite::SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction )
{
    BOOST_FOREACH( EventListener_ABC* listener, listeners_ )
        listener->SpatialChanged( latitude, longitude, altitude, speed, direction );
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::FormationChanged
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void EventListenerComposite::FormationChanged( bool isOnRoad )
{
    BOOST_FOREACH( EventListener_ABC* listener, listeners_ )
        listener->FormationChanged( isOnRoad );
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::EquipmentChanged
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void EventListenerComposite::EquipmentChanged( unsigned int type, const rpr::EntityType& entityType, unsigned int available )
{
    BOOST_FOREACH( EventListener_ABC* listener, listeners_ )
        listener->EquipmentChanged( type, entityType, available );
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::EmbarkmentChanged
// Created: AHC 2012-07-26
// -----------------------------------------------------------------------------
void EventListenerComposite::EmbarkmentChanged( bool mounted )
{
    BOOST_FOREACH( EventListener_ABC* listener, listeners_ )
        listener->EmbarkmentChanged( mounted );
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::EmbarkmentChanged
// Created: AHC 2012-07-30
// -----------------------------------------------------------------------------
void EventListenerComposite::PlatformAdded( const std::string& name, unsigned int id )
{
    BOOST_FOREACH( EventListener_ABC* listener, listeners_ )
        listener->PlatformAdded( name, id );
}
