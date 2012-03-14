// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
#include "Handle.h"
#include "Api_ABC.h"

using namespace process;

// -----------------------------------------------------------------------------
// Name: Handle::Handle
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
Handle::Handle( const Api_ABC& api, HANDLE value )
    : api_  ( api )
    , value_( value )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Handle::~Handle
// Created: BAX 2012-03-08
// -----------------------------------------------------------------------------
Handle::~Handle()
{
    if( value_ )
        api_.CloseHandle( value_ );
}
