// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "Event.h"
#include <module_api/Event.h>

using namespace sword::wrapper;

// -----------------------------------------------------------------------------
// Name: Event constructor
// Created: MCO 2012-01-02
// -----------------------------------------------------------------------------
Event::Event( const std::string& name )
    : event_( ::SWORD_CreateEvent() )
    , node_ ( event_ )
    , name_ ( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Event destructor
// Created: MCO 2012-01-02
// -----------------------------------------------------------------------------
Event::~Event()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Event::Post
// Created: SLI 2011-12-21
// -----------------------------------------------------------------------------
void Event::Post()
{
    ::SWORD_PostEvent( name_.c_str(), event_ );
}

// -----------------------------------------------------------------------------
// Name: Event::AddElement
// Created: VPR 2012-01-31
// -----------------------------------------------------------------------------
Node Event::AddElement()
{
    return node_.AddElement();
}
