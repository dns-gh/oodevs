// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EventMarker.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EventMarker constructor
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
EventMarker::EventMarker( E_EventTypes type, const timeline::Event& event )
    : Event( type, event )
{
    Event::Update();
}

// -----------------------------------------------------------------------------
// Name: EventMarker destructor
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
EventMarker::~EventMarker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventMarker::Clone
// Created: JSR 2014-10-24
// -----------------------------------------------------------------------------
Event* EventMarker::Clone() const
{
    return new EventMarker( type_, *event_ );
}
