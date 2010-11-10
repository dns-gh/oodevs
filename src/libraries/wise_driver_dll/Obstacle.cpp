// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "stdafx.h"
#include "Obstacle.h"
#include "Model.h"
#include "Party.h"
#include "protocol/Simulation.h"
#include <iwisedriversink.h>
#include <sstream>
#include <wisedriver.h>

namespace
{
    std::list< CWISEVec3 > ReadPoints( const Common::MsgLocation& message )
    {
        std::list< CWISEVec3 > points;
        for( int i = 0; i < message.coordinates().elem_size(); ++i )
        {
            const Common::MsgCoordLatLong& point = message.coordinates().elem( i );
            points.push_back( CWISEVec3( point.latitude(), point.longitude(), 0 ) );
        }
        return points;
    }
}

// -----------------------------------------------------------------------------
// Name: Obstacle constructor
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
Obstacle::Obstacle( const Model& model, const MsgsSimToClient::MsgObjectCreation& message )
    : id_( message.object().id() )
    , name_( message.name().begin(), message.name().end() )
    , type_( message.type().id().begin(), message.type().id().end() )
    , party_( model.ResolveParty( message.party().id() ) )
    , geometry_( message.location().type() )
    , points_( ReadPoints( message.location() ) )
    , handle_( WISE_INVALID_HANDLE )
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
// Name: Obstacle::GetId
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
unsigned long Obstacle::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Obstacle::GetHandle
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
WISE_HANDLE Obstacle::GetHandle() const
{
    return handle_;
}

// -----------------------------------------------------------------------------
// Name: Obstacle::Create
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
void Obstacle::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    const std::wstring identifier( MakeIdentifier() );
    try
    {
        handle_ = WISE_INVALID_HANDLE;
        std::map< std::wstring, WISE_HANDLE > attributes;
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, identifier, L"Obstacle", handle_, attributes ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Identifier" ], long( id_ ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Name" ], name_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Type" ], type_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Party" ], party_ ? party_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Geometry" ], geometry_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Points" ], points_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->AddObjectToDatabase( database, handle_ ) );
        driver.NotifyInfoMessage( L"Obstacle '" + identifier + L"' created." );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"Failed to create obstacle '" + identifier + L"'.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: Obstacle::Destroy
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
void Obstacle::Destroy( CWISEDriver& driver, const WISE_HANDLE& database ) const
{
    const std::wstring identifier( MakeIdentifier() );
    try
    {
        if( handle_ != WISE_INVALID_HANDLE )
        {
            CHECK_WISE_RESULT_EX( driver.GetSink()->RemoveObjectFromDatabase( database, handle_ ) );
            handle_ = WISE_INVALID_HANDLE;
        }
        driver.NotifyInfoMessage( L"Obstacle '" + identifier + L"' destroyed." );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"Failed to destroy obstacle '" + identifier + L"'.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: Obstacle::MakeIdentifier
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
std::wstring Obstacle::MakeIdentifier() const
{
    std::wstringstream ss;
    ss << GetId() << "-" << name_;
    return ss.str();
}
