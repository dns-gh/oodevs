// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "Simulation.h"
#include "client_proxy/SwordMessagePublisher_ABC.h"
#include "protocol/SimulationSenders.h"
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/wisedriver.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: Simulation constructor
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
Simulation::Simulation( SwordMessagePublisher_ABC& publisher )
    : publisher_( publisher )
    , running_( false )
    , handle_( WISE_INVALID_HANDLE )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Simulation destructor
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
Simulation::~Simulation()
{
    // NOTHING
}

namespace
{
    std::wstring DateToWString( const sword::DateTime& dateTime )
    {
        return std::wstring( dateTime.data().begin(), dateTime.data().end() );
    }
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void Simulation::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::ControlInformation& message )
{
    try
    {
        if( handle_ != WISE_INVALID_HANDLE )
        {
            driver.NotifyWarningMessage( L"Simulation object is already initialised.", MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_W_ALREADY_INITIATED ) );
            return;
        }
        running_ = message.status() == sword::running;
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, L"SwordSimulation", L"Simulation", handle_, attributes_ ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Running" ], unsigned char( running_ ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Tick" ], long( message.current_tick() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"DateTime" ], DateToWString( message.date_time() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"InitialDateTime" ], DateToWString( message.initial_date_time() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"AccelerationFactor" ], long( message.time_factor() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->AddObjectToDatabase( database, handle_ ) );
        driver.NotifyInfoMessage( L"[simulation] Created." );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"[simulation] Creation failed.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void Simulation::Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::ControlBeginTick& message )
{
    try
    {
        if( handle_ == WISE_INVALID_HANDLE )
        {
            driver.NotifyWarningMessage( L"[simulation] Object is not initialised.", MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_W_NOT_INITIATED ) );
            return;
        }
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Tick" ], long( message.current_tick() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"DateTime" ], DateToWString( message.date_time() ), currentTime ) );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"[simulation] State update failed.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: Simulation::Update
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void Simulation::Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::ControlChangeTimeFactorAck& message )
{
    try
    {
        if( handle_ == WISE_INVALID_HANDLE )
        {
            driver.NotifyWarningMessage( L"[simulation] Object is not initialised.", MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_W_NOT_INITIATED ) );
            return;
        }
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"AccelerationFactor" ], long( message.time_factor() ), currentTime ) );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"[simulation] Acceleration factor update failed.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: Simulation::Destroy
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void Simulation::Destroy( CWISEDriver& driver, const WISE_HANDLE& database )
{
    try
    {
        if( handle_ != WISE_INVALID_HANDLE )
        {
            CHECK_WISE_RESULT_EX( driver.GetSink()->RemoveObjectFromDatabase( database, handle_ ) );
            handle_ = WISE_INVALID_HANDLE;
        }
        driver.NotifyInfoMessage( L"[simulation] Destroyed." );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"[simulation] Destruction failed.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: Simulation::OnUpdateState
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void Simulation::OnUpdateState( CWISEDriver& driver, const WISE_HANDLE& database, const WISE_HANDLE& handle )
{
    try
    {
        unsigned char running = 0;
        WISE_HANDLE attribute = WISE_INVALID_HANDLE;
        CHECK_WISE_RESULT_EX( driver.GetSink()->GetEventAttributeHandle( database, handle, L"Running", attribute ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->GetEventAttributeValue( database, handle, attribute, running ) );
        if( running )
            simulation::ControlResume().Send( publisher_ );
        else
            simulation::ControlPause().Send( publisher_ );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"[simulation] Failed to update state.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: Simulation::OnUpdateAccelerationFactor
// Created: SEB 2010-10-14
// -----------------------------------------------------------------------------
void Simulation::OnUpdateAccelerationFactor( CWISEDriver& driver, const WISE_HANDLE& database, const WISE_HANDLE& handle )
{
    try
    {
        long factor = 0;
        WISE_HANDLE attribute = WISE_INVALID_HANDLE;
        CHECK_WISE_RESULT_EX( driver.GetSink()->GetEventAttributeHandle( database, handle, L"AccelerationFactor", attribute ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->GetEventAttributeValue( database, handle, attribute, factor ) );
        if( factor > 0 )
        {
            simulation::ControlChangeTimeFactor message;
            message().set_time_factor( factor );
            message.Send( publisher_ );
        }
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"[simulation] Failed to update acceleration factor.", error );
    }
}
