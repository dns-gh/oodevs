// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "EventFactory.h"
#include "EventAction.h"
#include "ENT/ENT_Tr.h"
#include "timeline/api.h"

// -----------------------------------------------------------------------------
// Name: EventFactory constructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
EventFactory::EventFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventFactory destructor
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
EventFactory::~EventFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EventFactory::Create
// Created: ABR 2013-05-28
// -----------------------------------------------------------------------------
Event* EventFactory::Create( const timeline::Event& event ) const
{
    //Retrieve type from event.action.target instead of event.info
    E_EventTypes type = ENT_Tr::ConvertToEventType( event.info, ENT_Tr_ABC::eToSim );
    return Create( type, &event );
}

// -----------------------------------------------------------------------------
// Name: EventFactory::Create
// Created: ABR 2013-05-30
// -----------------------------------------------------------------------------
Event* EventFactory::Create( E_EventTypes type, const timeline::Event* event ) const
{
    Event* result = 0;
    static const timeline::Event dummyEvent;
    switch( type )
    {
    case eEventTypes_Order:
    case eEventTypes_SupervisorAction:
        result = new EventAction( type, ( event ) ? *event : dummyEvent );
        break;
    case eEventTypes_Report:
    case eEventTypes_Task:
    case eEventTypes_Multimedia:
        result = new Event(  type, ( event ) ? *event : dummyEvent );
        break;
    default:
        assert( false );
    }
    return result;
}
