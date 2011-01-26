// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "Weather.h"
#include "protocol/SimulationSenders.h"
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/wisedriver.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: Weather constructor
// Created: SEB 2011-01-07
// -----------------------------------------------------------------------------
Weather::Weather()
    : handle_( WISE_INVALID_HANDLE )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Weather destructor
// Created: SEB 2011-01-07
// -----------------------------------------------------------------------------
Weather::~Weather()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Weather::Create
// Created: SEB 2011-01-07
// -----------------------------------------------------------------------------
void Weather::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::ControlGlobalWeather& message )
{
    try
    {
        if( handle_ != WISE_INVALID_HANDLE )
        {
            driver.NotifyWarningMessage( L"[weather] Object is already initialised.", MAKE_WISE_RESULT( WISE_FACILITY_COM_ADAPTER, WISE_W_ALREADY_INITIATED ) );
            return;
        }
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, L"Weather", L"Environment.Weather", handle_, attributes_ ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Temperature" ], long( message.attributes().temperature() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Wind.Speed" ], long( message.attributes().wind_speed() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Wind.Direction" ], long( message.attributes().wind_direction().heading() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Cloud.Floor" ], long( message.attributes().cloud_floor() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Cloud.Ceiling" ], long( message.attributes().cloud_ceiling() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Cloud.Density" ], long( message.attributes().cloud_density() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Precipitations" ], unsigned char( message.attributes().precipitation() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Lighting" ], unsigned char( message.attributes().lighting() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->AddObjectToDatabase( database, handle_ ) );
        driver.NotifyDebugMessage( L"[weather] Created.", 0 );
    }
    catch( WISE_RESULT& error )
    {
        handle_ = WISE_INVALID_HANDLE;
        driver.NotifyErrorMessage( L"[weather] Creation failed.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: Weather::Update
// Created: SEB 2011-01-07
// -----------------------------------------------------------------------------
void Weather::Update( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime, const sword::ControlGlobalWeather& message )
{
    if( handle_ == WISE_INVALID_HANDLE )
        Create( driver, database, currentTime, message );
    else
    {
        try
        {
            if( message.attributes().has_temperature() )
                CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Temperature" ], long( message.attributes().temperature() ), currentTime ) );
            if( message.attributes().has_wind_speed() )
                CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Wind.Speed" ], long( message.attributes().wind_speed() ), currentTime ) );
            if( message.attributes().has_wind_direction() )
                CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Wind.Direction" ], long( message.attributes().wind_direction().heading() ), currentTime ) );
            if( message.attributes().has_cloud_floor() )
                CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Cloud.Floor" ], long( message.attributes().cloud_floor() ), currentTime ) );
            if( message.attributes().has_cloud_ceiling() )
                CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Cloud.Ceiling" ], long( message.attributes().cloud_ceiling() ), currentTime ) );
            if( message.attributes().has_cloud_density() )
                CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Cloud.Density" ], long( message.attributes().cloud_density() ), currentTime ) );
            if( message.attributes().has_precipitation() )
                CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Precipitations" ], unsigned char( message.attributes().precipitation() ), currentTime ) );
            if( message.attributes().has_lighting() )
                CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( database, handle_, attributes_[ L"Lighting" ], unsigned char( message.attributes().lighting() ), currentTime ) );
            driver.NotifyDebugMessage( L"[weather] Updated.", 2 );
        }
        catch( WISE_RESULT& error )
        {
            driver.NotifyErrorMessage( L"[weather] Update failed.", error );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: Weather::Destroy
// Created: SEB 2011-01-07
// -----------------------------------------------------------------------------
void Weather::Destroy( CWISEDriver& driver, const WISE_HANDLE& database )
{
    try
    {
        if( handle_ != WISE_INVALID_HANDLE )
        {
            CHECK_WISE_RESULT_EX( driver.GetSink()->RemoveObjectFromDatabase( database, handle_ ) );
            handle_ = WISE_INVALID_HANDLE;
        }
        driver.NotifyDebugMessage( L"[weather] Destroyed.", 0 );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"[weather] Destruction failed.", error );
    }
}
