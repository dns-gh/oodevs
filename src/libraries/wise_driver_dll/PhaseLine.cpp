// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "PhaseLine.h"
#include "Automat.h"
#include "Formation.h"
#include "Model.h"
#include "protocol/Messenger.h"
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

namespace
{
    const Entity_ABC* ResolveSuperior( const Model& model, const sword::Diffusion& message )
    {
        if( message.has_automat() )
            return model.ResolveAutomat( message.automat().id() );
        return model.ResolveFormation( message.formation().id() );
    }

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
// Name: PhaseLine constructor
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
PhaseLine::PhaseLine( const Model& model, const sword::PhaseLineCreation& message )
    : WiseEntity( message.id().id(), L"phase-line" )
    , name_( message.tactical_line().name().begin(), message.tactical_line().name().end() )
    , superior_( ResolveSuperior( model, message.tactical_line().diffusion() ) )
    , points_( ReadPoints( message.tactical_line().geometry() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PhaseLine destructor
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
PhaseLine::~PhaseLine()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PhaseLine::Create
// Created: SEB 2010-10-27
// -----------------------------------------------------------------------------
void PhaseLine::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    try
    {
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, GetIdentifier(), L"Environment.PhaseLine", handle_, attributes_ ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Identifier" ], long( GetId() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Name" ], name_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Superior" ], superior_ ? superior_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
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
