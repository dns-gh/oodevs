// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.

// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "TacticalObjectProxy.h"
#include "TacticalObjectEventListener_ABC.h"
#include "dispatcher/Object_ABC.h"
#include "dispatcher/Localisation.h"

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
void TacticalObjectProxy::Register( TacticalObjectEventListener_ABC& listener )
{
    listeners_.Register( listener );
    const dispatcher::Localisation::T_PositionVector& points( object_.GetLocalisation().GetPoints() );
    listener.SpatialChanged( points );
}

// -----------------------------------------------------------------------------
// Name: TacticalObjectProxy::Unregister
// Created: AHC 2012-08-08
// -----------------------------------------------------------------------------
void TacticalObjectProxy::Unregister( TacticalObjectEventListener_ABC& listener )
{
    listeners_.Unregister( listener );
}
