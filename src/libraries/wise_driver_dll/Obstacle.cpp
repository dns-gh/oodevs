// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "Obstacle.h"
#include "Model.h"
#include "Party.h"
#include "protocol/Simulation.h"
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

namespace
{
    std::list< CWISEVec3 > ReadPoints( const sword::Location& message )
    {
        std::list< CWISEVec3 > points;
        for( int i = 0; i < message.coordinates().elem_size(); ++i )
        {
            const sword::CoordLatLong& point = message.coordinates().elem( i );
            points.push_back( CWISEVec3( point.latitude(), point.longitude(), 0 ) );
        }
        return points;
    }
}

// -----------------------------------------------------------------------------
// Name: Obstacle constructor
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
Obstacle::Obstacle( const Model& model, const sword::ObjectCreation& message )
    : WiseEntity( message.object().id(), L"obstacle" )
    , name_( message.name().begin(), message.name().end() )
    , type_( message.type().id().begin(), message.type().id().end() )
    , party_( model.ResolveParty( message.party().id() ) )
    , geometry_( unsigned char( message.location().type() ) )
    , points_( ReadPoints( message.location() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Obstacle destructor
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
Obstacle::~Obstacle()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Obstacle::Create
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
void Obstacle::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    try
    {
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, GetIdentifier(), L"Environment.Obstacle", handle_, attributes_ ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Identifier" ], long( GetId() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Name" ], name_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Type" ], type_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Party" ], party_ ? party_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Geometry" ], geometry_, currentTime ) );
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
