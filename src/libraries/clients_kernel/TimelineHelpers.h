// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __TimelineHelpers_h_
#define __TimelineHelpers_h_

#include "ENT/ENT_Enums.h"

#define EVENT_DATE_FORMAT "yyyy-MM-ddTHH:mm:ssZ"

#define EVENT_ORDER_PROTOCOL "sword"
#define EVENT_REPORT_PROTOCOL "report"
#define EVENT_SIMULATION_SERVICE "simulation"
#define EVENT_MULTIMEDIA_PROTOCOL "sword"
#define EVENT_MULTIMEDIA_SERVICE "multimedia"
#define EVENT_MARKER_PROTOCOL "marker"

namespace timeline
{
    struct Event;
}

namespace timeline_helpers
{
    std::string CreateEventTarget( const std::string& protocol, const std::string& service );
    E_EventTypes GetEventType( const timeline::Event& event );
}

#endif // __TimelineHelpers_h__
