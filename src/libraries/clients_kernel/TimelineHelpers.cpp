// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "TimelineHelpers.h"
#include <boost/lexical_cast.hpp>
#pragma warning( push )
#pragma warning( disable : 4724 )
#include <boost/uuid/random_generator.hpp>
#pragma warning( pop )
#include <boost/uuid/uuid_io.hpp>

#include <timeline/api.h>

#pragma warning( push, 0 )
#include <QtCore/QUrl>
#pragma warning( pop )

namespace timeline_helpers
{

    // -----------------------------------------------------------------------------
    // Name: CreateEventTarget
    // Created: JSR 2014-10-31
    // -----------------------------------------------------------------------------
    std::string CreateEventTarget( const std::string& protocol, const std::string& service )
    {
        return protocol + "://" + service;
    }

    // -----------------------------------------------------------------------------
    // Name: GenerateUuid
    // Created: SLI 2014-11-13
    // -----------------------------------------------------------------------------
    std::string GenerateUuid()
    {
        return boost::lexical_cast< std::string >( boost::uuids::random_generator()() );
    }

    E_EventTypes GetEventType( const timeline::Event& event )
    {
        const QUrl url( QString::fromStdString( event.action.target ) );
        const auto scheme = url.scheme();
        if( scheme == EVENT_ORDER_PROTOCOL )
            return eEventTypes_Order;
        if( scheme == EVENT_REPORT_PROTOCOL )
            return eEventTypes_Report;
        if( scheme == EVENT_MULTIMEDIA_PROTOCOL )
            return eEventTypes_Multimedia;
        if( scheme == EVENT_MARKER_PROTOCOL )
            return eEventTypes_Marker;
        if( scheme == EVENT_REPLAY_PROTOCOL )
            return eEventTypes_Replay;
        return eEventTypes_Task;
    }
}
