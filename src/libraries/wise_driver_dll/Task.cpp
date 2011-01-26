// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "Task.h"
#include "Agent.h"
#include "Automat.h"
#include "Model.h"
#include "TaskControlMeasures.h"
#include "WiseHelpers.h"
#include "client_proxy/SwordMessagePublisher_ABC.h"
#include "protocol/SimulationSenders.h"
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

namespace
{
    template< typename T >
    void Notify( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const Model& model, const T& message, const Entity_ABC* tasker )
    {
        try
        {
            TaskControlMeasures controlMeasures( driver, database, currentTime, model, message.parameters() );
            WISE_HANDLE handle = WISE_INVALID_HANDLE;
            std::map< std::wstring, WISE_HANDLE > attributes;
            CHECK_WISE_RESULT_EX( driver.GetSink()->CreateEventFromTemplate( database, L"TaskReport", handle, attributes ) );
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Tasker" ], tasker ? tasker->GetHandle() : WISE_INVALID_HANDLE ) );
            CHECK_WISE_RESULT_EX( driver.GetSink()->SetEventAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle, attributes[ L"Type" ], long( message.type().id() ) ) );
            controlMeasures.Send( handle, attributes );
            CHECK_WISE_RESULT_EX( driver.GetSink()->SendEvent( database, handle ) );
            driver.NotifyDebugMessage( L"[task-report] Created.", 1 );
        }
        catch( WISE_RESULT& error )
        {
            driver.NotifyErrorMessage( L"[task-report] Creation failed.", error );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Task::Notify
// Created: SEB 2010-12-27
// -----------------------------------------------------------------------------
void Task::Notify( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const Model& model, const sword::UnitOrder& message )
{
    const Entity_ABC* tasker = model.ResolveAgent( message.tasker().id() );
    ::Notify( driver, database, currentTime, model, message, tasker );
}

// -----------------------------------------------------------------------------
// Name: Task::Notify
// Created: SEB 2011-01-02
// -----------------------------------------------------------------------------
void Task::Notify( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const Model& model, const sword::AutomatOrder& message )
{
    const Entity_ABC* tasker = model.ResolveAutomat( message.tasker().id() );
    ::Notify( driver, database, currentTime, model, message, tasker );
}

namespace
{
    template< typename Message >
    void IssueOrder( CWISEDriver& driver, const WISE_HANDLE& database, const Model& model, SwordMessagePublisher_ABC& publisher, const WISE_HANDLE& handle, const WiseEntity& tasker )
    {
        long type = 0;
        wise::FetchEventAttribute( driver, database, handle, L"Type", type );
        Message message;
        message().mutable_type()->set_id( type );
        message().mutable_tasker()->set_id( tasker.GetId() );
        TaskControlMeasures::Send( driver, database, model, handle, *message().mutable_parameters() );
        message.Send( publisher );
    }
}

// -----------------------------------------------------------------------------
// Name: Task::Issue
// Created: SEB 2010-12-29
// -----------------------------------------------------------------------------
void Task::Issue( CWISEDriver& driver, const WISE_HANDLE& database, const Model& model, SwordMessagePublisher_ABC& publisher, const WISE_HANDLE& handle )
{
    try
    {
        WISE_HANDLE tasker = WISE_INVALID_HANDLE;
        wise::FetchEventAttribute( driver, database, handle, L"Tasker", tasker );
        if( const WiseEntity* entity = model.ResolveEntity( tasker ) )
        {
            if( entity->IsA< Agent >() )
                IssueOrder< simulation::UnitOrder >( driver, database, model, publisher, handle, *entity );
            else if( entity->IsA< Automat >() )
                IssueOrder< simulation::AutomatOrder >( driver, database, model, publisher, handle, *entity );
        }
        else
            driver.NotifyErrorMessage( L"[task-creation-request] Missing tasker.", MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_DOES_NOT_EXIST ) );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"[task-creation-request] Failed to send task to simulation.", error );
    }
}
