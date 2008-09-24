// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "script_plugin_pch.h"
#include "EventCondition.h"
#include "clients_kernel/Controller.h"

using namespace plugins::script;

// -----------------------------------------------------------------------------
// Name: EventCondition constructor
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
EventCondition::EventCondition( kernel::Controller& controller )
    : controller_( controller )
    , active_    ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventCondition destructor
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
EventCondition::~EventCondition()
{
    Activate( false );
}

// -----------------------------------------------------------------------------
// Name: EventCondition::Activate
// Created: AGE 2008-06-17
// -----------------------------------------------------------------------------
void EventCondition::Activate( bool active )
{
    if( active_ && !active )
        controller_.Unregister( *this );
    else if( !active_ && active )
        controller_.Register( *this );
    active_ = active;
    BaseCondition::Activate( active );
}
