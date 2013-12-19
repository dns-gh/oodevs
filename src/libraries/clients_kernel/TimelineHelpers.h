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

#define EVENT_DATE_FORMAT "yyyy-MM-ddTHH:mm:ssZ"

#define EVENT_ORDER_PROTOCOL "sword"
#define EVENT_REPORT_PROTOCOL "report"
#define EVENT_SIMULATION_SERVICE "simulation"
#define EVENT_MULTIMEDIA_PROTOCOL "sword"
#define EVENT_MULTIMEDIA_SERVICE "multimedia"

#define CREATE_EVENT_TARGET( PROTOCOL, SERVICE ) PROTOCOL "://" SERVICE

#endif // __TimelineHelpers_h__
