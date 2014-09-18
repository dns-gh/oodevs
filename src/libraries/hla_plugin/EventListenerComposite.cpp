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
    listeners_.insert( &listener );
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::Unregister
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void EventListenerComposite::Unregister( EventListener_ABC& listener )
{
    listeners_.erase( &listener );
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::Unregister
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void EventListenerComposite::SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction )
{
    std::for_each( listeners_.begin(), listeners_.end(), [&](EventListener_ABC* listener)
        {
            listener->SpatialChanged( latitude, longitude, altitude, speed, direction );
        });
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::Unregister
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void EventListenerComposite::FormationChanged( bool isOnRoad )
{
    std::for_each( listeners_.begin(), listeners_.end(), [&](EventListener_ABC* listener)
        {
            listener->FormationChanged( isOnRoad );
        });
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::Unregister
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void EventListenerComposite::EquipmentChanged( unsigned int type, const rpr::EntityType& entityType, unsigned int available,
        unsigned int dead, unsigned int lightDamages, unsigned int heavyDamages )
{
    std::for_each( listeners_.begin(), listeners_.end(), [&](EventListener_ABC* listener)
        {
            listener->EquipmentChanged( type, entityType, available, dead, lightDamages, heavyDamages );
        });
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::Unregister
// Created: AHC 2012-02-21
// -----------------------------------------------------------------------------
void EventListenerComposite::EmbarkmentChanged( bool mounted )
{
    std::for_each( listeners_.begin(), listeners_.end(), [&](EventListener_ABC* listener)
        {
            listener->EmbarkmentChanged( mounted );
        });
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::PlatformAdded
// Created: AHC 2012-07-30
// -----------------------------------------------------------------------------
void EventListenerComposite::PlatformAdded( const std::string& name, unsigned int id )
{
    std::for_each( listeners_.begin(), listeners_.end(), [&](EventListener_ABC* listener)
        {
            listener->PlatformAdded( name, id );
        });
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::PlatformAdded
// Created: AHC 2012-07-30
// -----------------------------------------------------------------------------
void EventListenerComposite::ChildrenChanged( const T_ChildrenIds& children )
{
    std::for_each( listeners_.begin(), listeners_.end(), [&](EventListener_ABC* listener)
        {
            listener->ChildrenChanged( children );
        });
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::PlatformAdded
// Created: AHC 2012-07-30
// -----------------------------------------------------------------------------
void EventListenerComposite::ParentChanged( const std::string& parentId )
{
    std::for_each( listeners_.begin(), listeners_.end(), [&](EventListener_ABC* listener)
        {
            listener->ParentChanged( parentId );
        });
}

// -----------------------------------------------------------------------------
// Name: EventListenerComposite::StateChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void EventListenerComposite::StateChanged( rpr::DamageState32 state )
{
    std::for_each( listeners_.begin(), listeners_.end(), [&](EventListener_ABC* listener)
        {
            listener->StateChanged( state );
        });
}
