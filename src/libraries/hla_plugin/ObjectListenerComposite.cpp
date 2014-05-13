// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "ObjectListenerComposite.h"
#include "rpr/Coordinates.h"
#include <boost/bind.hpp>
#include <vector>
#include <algorithm>

using namespace plugins::hla;

namespace
{
    template <typename T>
    void copyAndApply(const std::set<ObjectListener_ABC*>& listeners, const T& ftor )
    {
        std::vector<ObjectListener_ABC*> tmp( listeners.begin(), listeners.end() );
        std::for_each( tmp.begin(), tmp.end(), [&](ObjectListener_ABC* listener)
            {
                ftor( listener );
            });
    }
}

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
    copyAndApply( listeners_, boost::bind( &ObjectListener_ABC::Moved, _1, identifier, latitude, longitude ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::SideChanged
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ObjectListenerComposite::SideChanged( const std::string& identifier, rpr::ForceIdentifier side )
{
    copyAndApply( listeners_, boost::bind( &ObjectListener_ABC::SideChanged, _1, identifier, side ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::NameChanged
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ObjectListenerComposite::NameChanged( const std::string& identifier, const std::string& name )
{
    copyAndApply( listeners_, boost::bind( &ObjectListener_ABC::NameChanged, _1, identifier, name ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::TypeChanged
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ObjectListenerComposite::TypeChanged( const std::string& identifier, const rpr::EntityType& type )
{
    copyAndApply( listeners_, boost::bind( &ObjectListener_ABC::TypeChanged, _1, identifier, boost::cref(type) ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::EquipmentUpdated
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ObjectListenerComposite::EquipmentUpdated( const std::string& identifier, const rpr::EntityType& equipmentType, unsigned int available,
        unsigned int dead, unsigned int lightDamages, unsigned int heavyDamages )
{
    copyAndApply( listeners_, boost::bind( &ObjectListener_ABC::EquipmentUpdated, _1, identifier, boost::cref(equipmentType), available, dead, lightDamages, heavyDamages ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::UniqueIdChanged
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ObjectListenerComposite::UniqueIdChanged( const std::string& identifier, const std::vector< char >& uniqueId )
{
    copyAndApply( listeners_, boost::bind( &ObjectListener_ABC::UniqueIdChanged, _1, identifier, uniqueId ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::CallsignChanged
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ObjectListenerComposite::CallsignChanged( const std::string& identifier, const std::string& callsign )
{
    copyAndApply( listeners_, boost::bind( &ObjectListener_ABC::CallsignChanged, _1, identifier, callsign ) );
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
// Created: AHC 2010-05-29
// -----------------------------------------------------------------------------
void ObjectListenerComposite::EmbeddedUnitListChanged( const std::string& identifier, const std::vector< T_UniqueId >& embeddedUnits )
{
    copyAndApply( listeners_, boost::bind( &ObjectListener_ABC::EmbeddedUnitListChanged, _1, identifier, embeddedUnits ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::PerimeterChanged
// Created: AHC 2010-09-07
// -----------------------------------------------------------------------------
void ObjectListenerComposite::GeometryChanged( const std::string& identifier, const std::vector< rpr::WorldLocation >& perimeter, ObjectListener_ABC::GeometryType type )
{
    copyAndApply( listeners_, boost::bind( &ObjectListener_ABC::GeometryChanged, _1, identifier, perimeter, type ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::ParentChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void ObjectListenerComposite::ParentChanged( const std::string& rtiIdentifier, const std::string& parentRtiId )
{
    copyAndApply( listeners_, boost::bind( &ObjectListener_ABC::ParentChanged, _1, rtiIdentifier, parentRtiId ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::SubAgregatesChanged
// Created: AHC 2012-10-03
// -----------------------------------------------------------------------------
void ObjectListenerComposite::SubAgregatesChanged( const std::string& rtiIdentifier, const  ObjectListener_ABC::T_EntityIDs& children )
{
    copyAndApply( listeners_, boost::bind( &ObjectListener_ABC::SubAgregatesChanged, _1, rtiIdentifier, children ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::SubEntitiesChanged
// Created: AHC 2012-10-04
// -----------------------------------------------------------------------------
void ObjectListenerComposite::SubEntitiesChanged(const std::string& rtiIdentifier, const  ObjectListener_ABC::T_EntityIDs& children )
{
    copyAndApply( listeners_, boost::bind( &ObjectListener_ABC::SubEntitiesChanged, _1, rtiIdentifier, children ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectListenerComposite::PropagationChanged
// Created: AHC 2013-07-10
// -----------------------------------------------------------------------------
void ObjectListenerComposite::PropagationChanged( const std::string& rtiIdentifier, const std::vector< ObjectListener_ABC::PropagationData >& data,
        int col, int lig, double xll, double yll, double dx, double dy )
{
    copyAndApply( listeners_, boost::bind( &ObjectListener_ABC::PropagationChanged, _1, rtiIdentifier, data, col, lig, xll, yll, dx, dy ) );
}
