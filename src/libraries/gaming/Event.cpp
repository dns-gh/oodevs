// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Event.h"
#include "timeline/api.h"

// -----------------------------------------------------------------------------
// Name: Event constructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
Event::Event( E_EventTypes type, const timeline::Event& event )
    : type_( type )
    , event_( new timeline::Event( event ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Event destructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
Event::~Event()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Event::GetType
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
E_EventTypes Event::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Event::GetEvent
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
timeline::Event& Event::GetEvent() const
{
    return *event_;
}

// -----------------------------------------------------------------------------
// Name: Event::Clone
// Created: ABR 2013-06-03
// -----------------------------------------------------------------------------
Event* Event::Clone() const
{
    return new Event( type_, *event_ );
}

// -----------------------------------------------------------------------------
// Name: Event::Update
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void Event::Update( const timeline::Event& event )
{
    event_.reset( new timeline::Event( event ) );
}
