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
#include <sstream>
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Formation::Formation( const Model& model, const MsgsSimToClient::MsgFormationCreation& message )
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
// Name: Formation::MakeIdentifier
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
std::wstring Formation::MakeIdentifier() const
{
    std::wstringstream ss;
    ss << GetId() << "-" << name_;
    return ss.str();
}

// -----------------------------------------------------------------------------
// Name: Formation::Create
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Formation::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    try
    {
        const std::wstring identifier( MakeIdentifier() );
        handle_ = WISE_INVALID_HANDLE;
        std::map< std::wstring, WISE_HANDLE > attributes;
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, identifier, L"Formation", handle_, attributes ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Identifier" ], long( GetId() ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Name" ], name_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Echelon" ], echelon_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Superior" ], superior_ ? superior_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Party" ], party_ ? party_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->AddObjectToDatabase( database, handle_ ) );
        driver.NotifyInfoMessage( FormatMessage( L"Created." ) );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( FormatMessage( L"Creation failed." ), error );
    }
}
