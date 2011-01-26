// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "Formation.h"
#include "Model.h"
#include "Party.h"
#include "protocol/Simulation.h"
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Formation::Formation( const Model& model, const sword::FormationCreation& message )
    : WiseEntity( message.formation().id(), L"formation" )
    , name_( message.name().begin(), message.name().end() )
    , echelon_( unsigned char( message.level() ) )
    , party_( model.ResolveParty( message.party().id() ) )
    , superior_( model.ResolveFormation( message.parent().id() ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Formation::Create
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Formation::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    try
    {
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, GetIdentifier(), L"Orbat.Formation", handle_, attributes_ ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Identifier" ], long( GetId() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Name" ], name_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Echelon" ], echelon_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Superior" ], superior_ ? superior_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes_[ L"Party" ], party_ ? party_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->AddObjectToDatabase( database, handle_ ) );
        driver.NotifyDebugMessage( FormatMessage( L"Created." ), 0 );
    }
    catch( WISE_RESULT& error )
    {
        handle_ = WISE_INVALID_HANDLE;
        driver.NotifyErrorMessage( FormatMessage( L"Creation failed." ), error );
    }
}
