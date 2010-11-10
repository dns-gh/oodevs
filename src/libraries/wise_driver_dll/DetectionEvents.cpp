// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "stdafx.h"
#include "DetectionEvents.h"
#include "Agent.h"
#include "Model.h"
#include "Obstacle.h"
#include "protocol/SimulationSenders.h"
#include <iwisedriversink.h>
#include <wisedriver.h>

// -----------------------------------------------------------------------------
// Name: DetectionEvents::Trigger
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
void DetectionEvents::Trigger( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const Model& model, const MsgsSimToClient::MsgUnitDetection& message )
{
    try
    {
        const Agent* observer = model.ResolveAgent( message.observer().id() );
        const Agent* target = model.ResolveAgent( message.detected_unit().id() );

        WISE_HANDLE handle = WISE_INVALID_HANDLE;
        std::map< std::wstring, WISE_HANDLE > attributes;
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateEventFromTemplate( database, L"AgentDetection", handle, attributes ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Observer" ], observer ? observer->GetHandle() : WISE_INVALID_HANDLE ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Target" ], target ? target->GetHandle() : WISE_INVALID_HANDLE ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"CurrentPerceptionLevel" ], unsigned char( message.current_visibility() ) ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"BestPerceptionLevel" ], unsigned char( message.max_visibility() ) ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SendEvent( database, handle ) );
        driver.NotifyInfoMessage( L"Agent detection event created." );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"Failed to create agent detection event.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: DetectionEvents::Trigger
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
void DetectionEvents::Trigger( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const Model& model, const MsgsSimToClient::MsgObjectDetection& message )
{
    try
    {
        const Agent* observer = model.ResolveAgent( message.observer().id() );
        const Obstacle* target = model.ResolveObstacle( message.detected_object().id() );

        WISE_HANDLE handle = WISE_INVALID_HANDLE;
        std::map< std::wstring, WISE_HANDLE > attributes;
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateEventFromTemplate( database, L"ObstacleDetection", handle, attributes ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Observer" ], observer ? observer->GetHandle() : WISE_INVALID_HANDLE ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Target" ], target ? target->GetHandle() : WISE_INVALID_HANDLE ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"PerceptionLevel" ], unsigned char( message.visibility() ) ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SendEvent( database, handle ) );
        driver.NotifyInfoMessage( L"Obstacle detection event created." );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"Failed to create obstacle detection event.", error );
    }
}
