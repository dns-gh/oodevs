// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "ChildListener.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: ChildListener Constructor
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
ChildListener::ChildListener( T_Callback callback )
    : locationCallback_( callback ) 
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChildListener::SpatialChanged
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void ChildListener::SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction )
{
    location_ = LocationStruct( latitude, longitude, altitude, speed, direction );
    locationCallback_( *this );
}

// -----------------------------------------------------------------------------
// Name: ChildListener::FormationChanged
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void ChildListener::FormationChanged( bool /*isOnRoad*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChildListener::EquipmentChanged
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void ChildListener::EquipmentChanged( unsigned int /*type*/, const rpr::EntityType& /*entityType*/, unsigned int /*available*/,
        unsigned int /*dead*/, unsigned int /*lightDamages*/, unsigned int /*heavyDamages*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChildListener::EmbarkmentChanged
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void ChildListener::EmbarkmentChanged( bool /*mounted*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChildListener::ChildrenChanged
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void ChildListener::ChildrenChanged( const EventListener_ABC::T_ChildrenIds& /*children*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChildListener::ParentChanged
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void ChildListener::ParentChanged( const std::string& /*parentId*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChildListener::GetLocation
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
const  ChildListener::LocationStruct&  ChildListener::GetLocation() const
{
    return location_;
}

// -----------------------------------------------------------------------------
// Name: ChildListener::ParentChanged
// Created: AHC 2012-10-02
// -----------------------------------------------------------------------------
void ChildListener::PlatformAdded( const std::string& /*name*/, unsigned int id )
{
    childPlatforms_.insert( id );
}

// -----------------------------------------------------------------------------
// Name: ChildListener::StateChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void ChildListener::StateChanged( rpr::DamageState32 /*state*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ChildListener::GetPlatforms
// Created: AHC 2012-11-14
// -----------------------------------------------------------------------------
const std::set< unsigned int >& ChildListener::GetPlatforms() const
{
    return childPlatforms_;
}
