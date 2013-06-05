// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "EventsModel.h"
#include "EventFactory.h"
#include "timeline/api.h"

// -----------------------------------------------------------------------------
// Name: EventsModel constructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
EventsModel::EventsModel( const EventFactory& factory )
    : StringResolver()
    , factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventsModel destructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
EventsModel::~EventsModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: EventsModel::Create
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
Event* EventsModel::Create( const timeline::Event& event )
{
    Event* newEvent = factory_.Create( event );
    Register( event.uuid, *newEvent );
    return newEvent;
}

// -----------------------------------------------------------------------------
// Name: EventsModel::Update
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void EventsModel::Update( const timeline::Event& event )
{
    Event& gamingEvent = Get( event.uuid );
    gamingEvent.Update( event );
}

// -----------------------------------------------------------------------------
// Name: EventsModel::Destroy
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void EventsModel::Destroy( const std::string& uuid )
{
    Remove( uuid );
}

// -----------------------------------------------------------------------------
// Name: EventsModel::Purge
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void EventsModel::Purge()
{
    DeleteAll();
}
