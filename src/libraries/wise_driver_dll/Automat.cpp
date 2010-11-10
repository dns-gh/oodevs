// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "stdafx.h"
#include "Automat.h"
#include "Formation.h"
#include "Model.h"
#include "Party.h"
#include "protocol/Simulation.h"
#include <iwisedriversink.h>
#include <sstream>
#include <wisedriver.h>

namespace
{
    const Entity_ABC* ResolveSuperior( const Model& model, const Common::ParentEntity& parent )
    {
        if( parent.has_formation() )
            return model.ResolveFormation( parent.formation().id() );
        if( parent.has_automat() )
            return model.ResolveAutomat( parent.automat().id() );
        return 0;
    }
}

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Automat::Automat( const Model& model, const MsgsSimToClient::MsgAutomatCreation& message )
    : id_( message.automat().id() )
    , name_( message.nom().begin(), message.nom().end() )
    , type_( message.type().id() )
    , party_( model.ResolveParty( message.party().id() ) )
    , superior_( ResolveSuperior( model, message.parent() ) )
    , handle_( WISE_INVALID_HANDLE )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Automat::GetId
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
unsigned long Automat::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Automat::GetHandle
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
WISE_HANDLE Automat::GetHandle() const
{
    return handle_;
}

// -----------------------------------------------------------------------------
// Name: Automat::MakeIdentifier
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
std::wstring Automat::MakeIdentifier() const
{
    std::wstringstream ss;
    ss << GetId() << "-" << name_;
    return ss.str();
}

// -----------------------------------------------------------------------------
// Name: Automat::Create
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Automat::Create( CWISEDriver& driver, const WISE_HANDLE& database, const timeb& currentTime ) const
{
    const std::wstring identifier( MakeIdentifier() );
    try
    {
        handle_ = WISE_INVALID_HANDLE;
        std::map< std::wstring, WISE_HANDLE > attributes;
        CHECK_WISE_RESULT_EX( driver.GetSink()->CreateObjectFromTemplate( database, identifier, L"Automat", handle_, attributes ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Identifier" ], long( id_ ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Name" ], name_, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Type" ], long( type_ ), currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Superior" ], superior_ ? superior_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->SetAttributeValue( WISE_TRANSITION_CACHE_DATABASE, handle_, attributes[ L"Party" ], party_ ? party_->GetHandle() : WISE_INVALID_HANDLE, currentTime ) );
        CHECK_WISE_RESULT_EX( driver.GetSink()->AddObjectToDatabase( database, handle_ ) );
        driver.NotifyInfoMessage( L"Automat '" + identifier + L"' created." );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"Failed to create automat '" + identifier + L"'.", error );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::Destroy
// Created: SEB 2010-10-13
// -----------------------------------------------------------------------------
void Automat::Destroy( CWISEDriver& driver, const WISE_HANDLE& database ) const
{
    const std::wstring identifier( MakeIdentifier() );
    try
    {
        if( handle_ != WISE_INVALID_HANDLE )
        {
            CHECK_WISE_RESULT_EX( driver.GetSink()->RemoveObjectFromDatabase( database, handle_ ) );
            handle_ = WISE_INVALID_HANDLE;
        }
        driver.NotifyInfoMessage( L"Automat '" + identifier + L"' destroyed." );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( L"Failed to destroy automat '" + identifier + L"'.", error );
    }
}
