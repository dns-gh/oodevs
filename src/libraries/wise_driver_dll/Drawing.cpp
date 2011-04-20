// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "Drawing.h"
#include "protocol/Messenger.h"
#include <iomanip>
#include <sstream>
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

namespace
{
    std::list< CWISEVec3 > ReadPoints( const sword::CoordLatLongList& message )
    {
        std::list< CWISEVec3 > points;
        for( int i = 0; i < message.elem_size(); ++i )
        {
            const sword::CoordLatLong& point = message.elem( i );
            points.push_back( CWISEVec3( point.latitude(), point.longitude(), 0 ) );
        }
        return points;
    }
    std::wstring ColorToHex( const sword::RgbColor& color )
    {
        std::wstringstream ss;
        ss << "#" << std::hex << std::setw( 2 ) << std::setfill( L'0' ) << color.red() << color.green() << color.blue();
        return ss.str();
    }
}

// -----------------------------------------------------------------------------
// Name: Drawing constructor
// Created: SEB 2011-01-07
// -----------------------------------------------------------------------------
Drawing::Drawing( const Model& /*model*/, const sword::ShapeCreation& message )
    : WiseEntity( message.id().id(), L"drawing" )
    , category_( message.shape().category().begin(), message.shape().category().end() )
    , color_( ColorToHex( message.shape().color() ) )
    , pattern_( message.shape().pattern().begin(), message.shape().pattern().end() )
    , points_( ReadPoints( message.shape().points() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Drawing destructor
// Created: SEB 2011-01-07
// -----------------------------------------------------------------------------
Drawing::~Drawing()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Drawing::Create
// Created: SEB 2011-01-07
// -----------------------------------------------------------------------------
void Drawing::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    try
    {
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, GetIdentifier(), L"Environment.Drawing", handle_, attributes_ ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Identifier" ], long( GetId() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Category" ], category_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Color" ], color_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Pattern" ], pattern_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Points" ], points_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->AddObjectToDatabase( database, handle_ ) );
        driver.NotifyDebugMessage( FormatMessage( L"Created." ), 0 );
    }
    catch( WISE_RESULT& error )
    {
        handle_ = WISE_INVALID_HANDLE;
        driver.NotifyErrorMessage( FormatMessage( L"Creation failed." ), error );
    }
}
