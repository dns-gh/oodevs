// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"

#include "Publisher_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Publisher_ABC constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Publisher_ABC::Publisher_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Publisher_ABC destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Publisher_ABC::~Publisher_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Publisher_ABC::Send
// Created: NLD 2007-01-30
// -----------------------------------------------------------------------------
void Publisher_ABC::Send( const ASN1T_MsgsInSim&    msg )
{
    throw std::runtime_error( "Invalid publisher" );
}

// -----------------------------------------------------------------------------
// Name: Publisher_ABC::Send
// Created: NLD 2007-01-30
// -----------------------------------------------------------------------------
void Publisher_ABC::Send( const ASN1T_MsgsInClient& msg )
{
    throw std::runtime_error( "Invalid publisher" );
}
