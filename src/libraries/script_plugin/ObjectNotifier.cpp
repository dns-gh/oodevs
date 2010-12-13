// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "ObjectNotifier.h"
#include "AgentEvents.h"
#include "clients_kernel/Controller.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: ObjectNotifier constructor
// Created: SBO 2010-07-13
// -----------------------------------------------------------------------------
ObjectNotifier::ObjectNotifier( kernel::Controller& controller, const dispatcher::Object_ABC& entity )
    : controller_( controller )
    , entity_    ( entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectNotifier destructor
// Created: SBO 2010-07-13
// -----------------------------------------------------------------------------
ObjectNotifier::~ObjectNotifier()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectNotifier::DoUpdate
// Created: SBO 2010-07-13
// -----------------------------------------------------------------------------
void ObjectNotifier::DoUpdate( const sword::ObjectDestruction& /*message*/ )
{
    controller_.Update( events::ObjectDestroyed( entity_ ) );
}
