// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "EventsModel.h"
#include "EventFactory.h"
#include "clients_kernel/Controller.h"
#include <timeline/api.h>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: EventsModel constructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
EventsModel::EventsModel( const EventFactory& factory, kernel::Controller& controller )
    : StringResolver()
    , factory_( factory )
    , controller_( controller )
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
    controller_.Create( *newEvent );
    return newEvent;
}

// -----------------------------------------------------------------------------
// Name: EventsModel::Update
// Created: ABR 2013-05-31
// -----------------------------------------------------------------------------
void EventsModel::Update( const timeline::Event& event )
{
    if( Event* gamingEvent = Find( event.uuid ) )
    {
        gamingEvent->Update( event );
        controller_.Update( *gamingEvent );
    }
}

// -----------------------------------------------------------------------------
// Name: EventsModel::Destroy
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void EventsModel::Destroy( const std::string& uuid )
{
    Event* event = Remove( uuid );
    if( event )
    {
        controller_.Delete( *event );
        delete event;
    }
}

// -----------------------------------------------------------------------------
// Name: EventsModel::Purge
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
void EventsModel::Purge()
{
    DeleteAll();
}
