// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "PlatformProxy.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: PlatformProxy constructor
// Created: AHC 2012-07-27
// -----------------------------------------------------------------------------
PlatformProxy::PlatformProxy()
    : state_( rpr::damageState_NoDamage )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PlatformProxy::Register
// Created: AHC 2012-07-27
// -----------------------------------------------------------------------------
void PlatformProxy::Register( EventListener_ABC& listener )
{
    listeners_.Register( listener );
}

// -----------------------------------------------------------------------------
// Name: PlatformProxy::Unregister
// Created: AHC 2012-07-27
// -----------------------------------------------------------------------------
void PlatformProxy::Unregister( EventListener_ABC& listener )
{
    listeners_.Unregister( listener );
}

// -----------------------------------------------------------------------------
// Name: PlatformProxy::SpatialChanged
// Created: AHC 2012-07-27
// -----------------------------------------------------------------------------
void PlatformProxy::SpatialChanged( double latitude, double longitude, float altitude, float speed, float direction )
{
    listeners_.SpatialChanged( latitude, longitude, altitude, speed, direction );
}

// -----------------------------------------------------------------------------
// Name: PlatformProxy::AddSubordinate
// Created: AHC 2012-10-08
// -----------------------------------------------------------------------------
void PlatformProxy::AddSubordinate( unsigned int /*id*/, Agent_ABC& /*agent*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PlatformProxy::RemoveSubordinate
// Created: AHC 2012-10-08
// -----------------------------------------------------------------------------
void PlatformProxy::RemoveSubordinate( unsigned int /*id*/ )
{
}

// -----------------------------------------------------------------------------
// Name: PlatformProxy::StateChanged
// Created: AHC 2012-11-12
// -----------------------------------------------------------------------------
void PlatformProxy::StateChanged( rpr::DamageState32 state )
{
    if( state != state_ )
    {
        state_ = state;
        listeners_.StateChanged( state_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PlatformProxy::MountedChanged
// Created: AHC 2012-11-13
// -----------------------------------------------------------------------------
void PlatformProxy::MountedChanged( bool isMounted )
{
    if( isMounted_ != isMounted )
    {
        isMounted_ = isMounted;
        listeners_.EmbarkmentChanged( isMounted_ );
    }
}
