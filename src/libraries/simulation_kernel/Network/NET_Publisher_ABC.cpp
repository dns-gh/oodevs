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

// -----------------------------------------------------------------------------
// Name: NET_Publisher_ABC constructor
// Created: LDC 2010-01-04
// -----------------------------------------------------------------------------
NET_Publisher_ABC::NET_Publisher_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NET_Publisher_ABC destructor
// Created: LDC 2010-01-04
// -----------------------------------------------------------------------------
NET_Publisher_ABC::~NET_Publisher_ABC()
{
    // NOTHING
}

namespace
{
    NET_Publisher_ABC* pPublisher_ = 0;
}

// -----------------------------------------------------------------------------
// Name: NET_Publisher_ABC::Register
// Created: LDC 2010-01-04
// -----------------------------------------------------------------------------
void NET_Publisher_ABC::Register( NET_Publisher_ABC& publisher )
{
    if( pPublisher_ == 0 )
        pPublisher_ = &publisher;
    else
        throw std::runtime_error( "Publisher already registered" );
}

// -----------------------------------------------------------------------------
// Name: NET_Publisher_ABC::Unregister
// Created: LDC 2010-01-04
// -----------------------------------------------------------------------------
void NET_Publisher_ABC::Unregister( NET_Publisher_ABC& publisher )
{
    if( pPublisher_ == &publisher )
        pPublisher_ = 0;
    else
        throw std::runtime_error( "Unregistering incorrect publisher" );
}

// -----------------------------------------------------------------------------
// Name: NET_Publisher_ABC::SendAsnMessage
// Created: LDC 2010-01-04
// -----------------------------------------------------------------------------
void NET_Publisher_ABC::SendAsnMessage( ASN1T_MsgsSimToClient& msg )
{
    if( pPublisher_ )
        pPublisher_->Send( msg );
    else
        throw std::runtime_error( "No publisher registered" );
}

