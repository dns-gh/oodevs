// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "EventAction.h"
#include "actions/Action_ABC.h"

// -----------------------------------------------------------------------------
// Name: EventAction constructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventAction::EventAction( E_EventTypes type, const timeline::Event& event )
    : Event( type, event )
    , action_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventAction destructor
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
EventAction::~EventAction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventAction::Clone
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
Event* EventAction::Clone() const
{
    return new EventAction( type_, *event_ );
}
