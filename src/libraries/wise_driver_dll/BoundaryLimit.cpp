// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "stdafx.h"
#include "BoundaryLimit.h"
#include "Automat.h"
#include "Formation.h"
#include "Model.h"
#include "protocol/Messenger.h"
#include <iwisedriversink.h>
#include <sstream>
#include <wisedriver.h>

namespace
{
    const Entity_ABC* ResolveSuperior( const Model& model, const Common::MsgTacticalLine::Diffusion& message )
    {
        if( message.has_automat() )
            return model.ResolveAutomat( message.automat().id() );
        return model.ResolveFormation( message.formation().id() );
    }

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
// Name: BoundaryLimit constructor
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
BoundaryLimit::BoundaryLimit( const Model& model, const MsgsMessengerToClient::MsgLimitCreation& message )
    : id_( message.id().id() )
    , name_( message.tactical_line().name().begin(), message.tactical_line().name().end() )
    , superior_( ResolveSuperior( model, message.tactical_line().diffusion() ) )
    , points_( ReadPoints( message.tactical_line().geometry() ) )
    , handle_( WISE_INVALID_HANDLE )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BoundaryLimit destructor
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
BoundaryLimit::~BoundaryLimit()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BoundaryLimit::GetId
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
unsigned long BoundaryLimit::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: BoundaryLimit::GetHandle
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
WISE_HANDLE BoundaryLimit::GetHandle() const
{
    return handle_;
}

// -----------------------------------------------------------------------------
// Name: BoundaryLimit::Create
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
void BoundaryLimit::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    const std::wstring identifier( MakeIdentifier() );
    try
    {
        handle_ = WISE_INVALID_HANDLE;
        std::map< std::wstring, WISE_HANDLE > attributes;
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, identifier, L"BoundaryLimit", handle_, attributes ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Identifier" ], long( id_ ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Name" ], name_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Superior" ], superior_ ? superior_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Points" ], points_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->AddObjectToDatabase( database, handle_ ) );
        driver.NotifyInfoMessage( L"Boundary limit '" + identifier + L"' created." );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"Failed to create boundary limit '" + identifier + L"'.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: BoundaryLimit::Destroy
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
void BoundaryLimit::Destroy( CWISEDriver& driver, const WISE_HANDLE& database ) const
{
    const std::wstring identifier( MakeIdentifier() );
    try
    {
        CHECK_WISE_RESULT_EX( driver.GetSink()->RemoveObjectFromDatabase( database, handle_ ) );
        handle_ = WISE_INVALID_HANDLE;
        driver.NotifyInfoMessage( L"Boundary limit '" + identifier + L"' destroyed." );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"Failed to destroy boundary limit '" + identifier + L"'.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: BoundaryLimit::MakeIdentifier
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
std::wstring BoundaryLimit::MakeIdentifier() const
{
    std::wstringstream ss;
    ss << GetId() << "-" << name_;
    return ss.str();
}
