// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "TacticalObjectProxy.h"
#include "EventListener_ABC.h"
#include "dispatcher/Object_ABC.h"

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: TacticalObjectProxy constructor
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
TacticalObjectProxy::TacticalObjectProxy( dispatcher::Object_ABC& object )
    : object_( object )
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectProxy destructor
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
TacticalObjectProxy::~TacticalObjectProxy()
{
    // TODO
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectProxy::Register
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
void TacticalObjectProxy::Register( EventListener_ABC& listener )
{
    listeners_.Register( listener );
    //listener.SpatialChanged( object_.GetPosition().X(), object_.GetPosition().Y(),
    //        object_.GetAltitude(), object_.GetSpeed(), object_.GetDirection() );
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectProxy::Unregister
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
void TacticalObjectProxy::Unregister( EventListener_ABC& listener )
{
    listeners_.Unregister( listener );
}
