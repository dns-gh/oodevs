// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "stdafx.h"
#include "Party.h"
#include "protocol/Simulation.h"
#include <iwisedriversink.h>
#include <wisedriver.h>

namespace
{
    unsigned char ConvertAlignment( const Common::EnumDiplomacy& diplomacy )
    {
        if( diplomacy == Common::unknown_diplo )
            return Common::friend_diplo;
        return diplomacy - 1;
    }
}

// -----------------------------------------------------------------------------
// Name: Party constructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Party::Party( const Model& /*model*/, const MsgsSimToClient::MsgPartyCreation& message )
    : id_( message.party().id() )
    , name_( message.name().begin(), message.name().end() )
    , alignment_( message.type() )
    , handle_( WISE_INVALID_HANDLE )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Party destructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Party::~Party()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Party::GetId
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
unsigned long Party::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Party::GetHandle
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
WISE_HANDLE Party::GetHandle() const
{
    return handle_;
}

// -----------------------------------------------------------------------------
// Name: Party::Create
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Party::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    try
    {
        handle_ = WISE_INVALID_HANDLE;
        std::map< std::wstring, WISE_HANDLE > attributes;
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, name_, L"Party", handle_, attributes ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Identifier" ], long( id_ ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Name" ], name_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Alignment" ], alignment_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->AddObjectToDatabase( database, handle_ ) );
        driver.NotifyInfoMessage( L"Party '" + name_ + L"' created." );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"Failed to create party '" + name_ + L"'.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: Party::Destroy
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Party::Destroy( CWISEDriver& driver, const WISE_HANDLE& database ) const
{
    try
    {
        if( handle_ != WISE_INVALID_HANDLE )
        {
            CHECK_WISE_RESULT_EX( driver.GetSink()->RemoveObjectFromDatabase( database, handle_ ) );
            handle_ = WISE_INVALID_HANDLE;
        }
        driver.NotifyInfoMessage( L"Party '" + name_ + L"' destroyed." );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"Failed to destroy party '" + name_ + L"'.", error );
    }
}
