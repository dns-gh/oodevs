// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "ObjectListenerComposite.h"
#include <boost/foreach.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite constructor
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
ObjectListenerComposite::~ObjectListenerComposite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite destructor
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
ObjectListenerComposite::ObjectListenerComposite()
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::Moved
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ObjectListenerComposite::Moved( const std::string& identifier, double latitude, double longitude )
{
    BOOST_FOREACH( ObjectListener_ABC* listener, listeners_ )
        listener->Moved( identifier, latitude, longitude );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::SideChanged
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ObjectListenerComposite::SideChanged( const std::string& identifier, rpr::ForceIdentifier side )
{
    BOOST_FOREACH( ObjectListener_ABC* listener, listeners_ )
        listener->SideChanged( identifier, side );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::NameChanged
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ObjectListenerComposite::NameChanged( const std::string& identifier, const std::string& name )
{
    BOOST_FOREACH( ObjectListener_ABC* listener, listeners_ )
        listener->NameChanged( identifier, name );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::TypeChanged
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ObjectListenerComposite::TypeChanged( const std::string& identifier, const rpr::EntityType& type )
{
    BOOST_FOREACH( ObjectListener_ABC* listener, listeners_ )
        listener->TypeChanged( identifier, type );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::EquipmentUpdated
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ObjectListenerComposite::EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int number )
{
    BOOST_FOREACH( ObjectListener_ABC* listener, listeners_ )
        listener->EquipmentUpdated( identifier, equipmentType, number );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::UniqueIdChanged
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ObjectListenerComposite::UniqueIdChanged( const std::string& identifier, const std::string& uniqueId )
{
    BOOST_FOREACH( ObjectListener_ABC* listener, listeners_ )
        listener->UniqueIdChanged( identifier, uniqueId );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::CallsignChanged
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ObjectListenerComposite::CallsignChanged( const std::string& identifier, const std::string& callsign )
{
    BOOST_FOREACH( ObjectListener_ABC* listener, listeners_ )
        listener->CallsignChanged( identifier, callsign );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::Register
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ObjectListenerComposite::Register( ObjectListener_ABC& listener )
{
    listeners_.insert(&listener);
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::Unregister
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ObjectListenerComposite::Unregister( ObjectListener_ABC& listener )
{
    listeners_.erase(&listener);
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::EmbeddedUnitListChanged
// Created: AHC 2012-07-04
// -----------------------------------------------------------------------------
void ObjectListenerComposite::EmbeddedUnitListChanged( const std::string& identifier, const std::vector< std::string >& embeddedUnits )
{
    BOOST_FOREACH( ObjectListener_ABC* listener, listeners_ )
        listener->EmbeddedUnitListChanged( identifier, embeddedUnits );
}
