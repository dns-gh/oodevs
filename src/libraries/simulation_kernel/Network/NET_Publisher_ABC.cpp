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
    NET_Publisher_ABC* pPublisher_ = 0;
    // -----------------------------------------------------------------------------
    // Name: Register
    // Created: LDC 2010-01-04
    // -----------------------------------------------------------------------------
    void Register( NET_Publisher_ABC& publisher )
    {
        if( pPublisher_ == 0 )
            pPublisher_ = &publisher;
        else
            throw MASA_EXCEPTION( "Publisher already registered" );
    }

    // -----------------------------------------------------------------------------
    // Name: Unregister
    // Created: LDC 2010-01-04
    // -----------------------------------------------------------------------------
    void Unregister( NET_Publisher_ABC& publisher )
    {
        if( pPublisher_ == &publisher )
            pPublisher_ = 0;
        else
            throw MASA_EXCEPTION( "Unregistering incorrect publisher" );
    }
}

// -----------------------------------------------------------------------------
// Name: NET_Publisher_ABC constructor
// Created: LDC 2010-01-04
// -----------------------------------------------------------------------------
NET_Publisher_ABC::NET_Publisher_ABC()
{
    Register( *this );
}

// -----------------------------------------------------------------------------
// Name: NET_Publisher_ABC destructor
// Created: LDC 2010-01-04
// -----------------------------------------------------------------------------
NET_Publisher_ABC::~NET_Publisher_ABC()
{
    Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: NET_Publisher_ABC::Publisher
// Created: LDC 2010-02-04
// -----------------------------------------------------------------------------
NET_Publisher_ABC& NET_Publisher_ABC::Publisher()
{
    if( !pPublisher_ )
        throw MASA_EXCEPTION( "No publisher registered" );
    return *pPublisher_;
}
