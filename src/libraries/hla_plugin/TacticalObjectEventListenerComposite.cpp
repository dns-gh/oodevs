// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "TacticalObjectEventListenerComposite.h"

#include <boost/foreach.hpp>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: TacticalObjectEventListenerComposite constructor
// Created: AHC 2012-08-10
// -----------------------------------------------------------------------------
TacticalObjectEventListenerComposite::TacticalObjectEventListenerComposite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectEventListenerComposite destructor
// Created: AHC 2012-08-10
// -----------------------------------------------------------------------------
TacticalObjectEventListenerComposite::~TacticalObjectEventListenerComposite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectEventListenerComposite::Register
// Created: AHC 2012-08-10
// -----------------------------------------------------------------------------
void TacticalObjectEventListenerComposite::Register( TacticalObjectEventListener_ABC& listener )
{
    listeners_.insert( &listener );
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectEventListenerComposite::Unregister
// Created: AHC 2012-08-10
// -----------------------------------------------------------------------------
void TacticalObjectEventListenerComposite::Unregister( TacticalObjectEventListener_ABC& listener )
{
    listeners_.erase( &listener );
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectEventListenerComposite::SpatialChanged
// Created: AHC 2012-08-10
// -----------------------------------------------------------------------------
void TacticalObjectEventListenerComposite::SpatialChanged( const TacticalObjectEventListener_ABC::T_PositionVector& pos )
{
    BOOST_FOREACH( TacticalObjectEventListener_ABC* listener, listeners_ )
        listener->SpatialChanged( pos );
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectEventListenerComposite::ResourcesChanged
// Created: AHC 2012-09-06
// -----------------------------------------------------------------------------
void TacticalObjectEventListenerComposite::ResourcesChanged( const TacticalObjectEventListener_ABC::T_ResourceVector& res )
{
    BOOST_FOREACH( TacticalObjectEventListener_ABC* listener, listeners_ )
        listener->ResourcesChanged( res );
}

