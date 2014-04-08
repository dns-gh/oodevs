// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "NET_Publisher_ABC.h"

namespace
{
    NET_Publisher_ABC* pPublisher = 0;
}

// -----------------------------------------------------------------------------
// Name: NET_Publisher_ABC constructor
// Created: LDC 2010-01-04
// -----------------------------------------------------------------------------
NET_Publisher_ABC::NET_Publisher_ABC()
{
    if( pPublisher )
        throw MASA_EXCEPTION( "Publisher already registered" );
    pPublisher = this;
}

// -----------------------------------------------------------------------------
// Name: NET_Publisher_ABC destructor
// Created: LDC 2010-01-04
// -----------------------------------------------------------------------------
NET_Publisher_ABC::~NET_Publisher_ABC()
{
    if( pPublisher != this )
        throw MASA_EXCEPTION( "Unregistering incorrect publisher" );
    pPublisher = 0;
}

// -----------------------------------------------------------------------------
// Name: NET_Publisher_ABC::Publisher
// Created: LDC 2010-02-04
// -----------------------------------------------------------------------------
NET_Publisher_ABC& NET_Publisher_ABC::Publisher()
{
    if( !pPublisher )
        throw MASA_EXCEPTION( "No publisher registered" );
    return *pPublisher;
}
