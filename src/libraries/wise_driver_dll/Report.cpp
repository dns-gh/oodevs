// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "Report.h"
#include "Agent.h"
#include "Automat.h"
#include "Model.h"
#include "protocol/SimulationSenders.h"
#include <limits>
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

#undef max

namespace
{
    std::wstring DateToWString( const sword::DateTime& dateTime )
    {
        return std::wstring( dateTime.data().begin(), dateTime.data().end() );
    }
}

// -----------------------------------------------------------------------------
// Name: Report::Trigger
// Created: SBO 2010-12-15
// -----------------------------------------------------------------------------
void Report::Trigger( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& /*currentTime*/, const Model& model, const sword::Report& message )
{
    try
    {
        const Entity_ABC* source = 0;
        if( message.source().has_unit() )
            source = model.ResolveAgent( message.source().unit().id() );
        else if( message.source().has_automat() )
            source = model.ResolveAutomat( message.source().automat().id() );
        else
            return; // $$$$ SBO 2010-12-15: crowd, unsupported

        WISE_HANDLE handle = WISE_INVALID_HANDLE;
        std::map< std::wstring, WISE_HANDLE > attributes;
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateEventFromTemplate( database, L"MessageReport", handle, attributes ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Identifier" ], long( std::numeric_limits< unsigned long >::max() - message.report().id() ) ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Source" ], source ? source->GetHandle() : WISE_INVALID_HANDLE ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"DateTime" ], DateToWString( message.time() ) ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Type" ], long( message.type().id() ) ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Level" ], unsigned char( message.category() ) ) );
        // $$$$ SBO 2010-12-15: report parameters not supported
        CHECK_WISE_RESULT_EX( driver.GetSink()->SendEvent( database, handle ) );
        driver.NotifyDebugMessage( L"[message-report] Created.", 1 );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"[message-report] Creation failed.", error );
    }
}
