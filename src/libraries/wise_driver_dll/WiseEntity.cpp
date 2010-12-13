// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "wise_driver_dll_pch.h"
#include "WiseEntity.h"
#pragma warning( push )
#pragma warning( disable: 4100 4201 )
#include <wise/iwisedriversink.h>
#include <wise/wisedriver.h>
#pragma warning( pop )

// -----------------------------------------------------------------------------
// Name: WiseEntity constructor
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
WiseEntity::WiseEntity( unsigned long id, const std::wstring& label )
    : id_( id )
    , label_( label )
    , handle_( WISE_INVALID_HANDLE )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WiseEntity destructor
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
WiseEntity::~WiseEntity()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WiseEntity::GetId
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
unsigned long WiseEntity::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: WiseEntity::GetHandle
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
WISE_HANDLE WiseEntity::GetHandle() const
{
    return handle_;
}

// -----------------------------------------------------------------------------
// Name: WiseEntity::Destroy
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
void WiseEntity::Destroy( CWISEDriver& driver, const WISE_HANDLE& database ) const
{
    const std::wstring identifier( MakeIdentifier() );
    try
    {
        if( handle_ != WISE_INVALID_HANDLE )
        {
            CHECK_WISE_RESULT_EX( driver.GetSink()->RemoveObjectFromDatabase( database, handle_ ) );
            handle_ = WISE_INVALID_HANDLE;
        }
        driver.NotifyInfoMessage( FormatMessage( L"Destroyed." ) );
    }
    catch( WISE_RESULT& error )
    {
        driver.NotifyErrorMessage( FormatMessage( L"Destruction failed." ), error );
    }
}

// -----------------------------------------------------------------------------
// Name: WiseEntity::FormatMessage
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
std::wstring WiseEntity::FormatMessage( const std::wstring& message ) const
{
    return L"[" + label_ + L":" + MakeIdentifier() + L"] " + message;
}

// -----------------------------------------------------------------------------
// Name: WiseEntity::GetAttribute
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
std::wstring WiseEntity::GetAttribute( const WISE_HANDLE& handle ) const
{
    for( std::map< std::wstring, WISE_HANDLE >::const_iterator it = attributes_.begin(); it != attributes_.end(); ++it )
        if( it->second == handle )
            return it->first;
    return L"";
}

// -----------------------------------------------------------------------------
// Name: WiseEntity::Update
// Created: SEB 2010-12-13
// -----------------------------------------------------------------------------
void WiseEntity::Update( SwordMessagePublisher_ABC& publisher, const WISE_HANDLE& attribute, const CWISEValueUnion& value )
{
    // $$$$ SEB 2010-12-13: remove default implementation
}
