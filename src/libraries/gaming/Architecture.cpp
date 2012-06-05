// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Architecture.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: Architecture constructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
Architecture::Architecture( const sword::UrbanAttributes& message, kernel::PropertiesDictionary& dictionary, const kernel::ObjectTypes& objectTypes )
    : kernel::Architecture( dictionary )
{
    if( message.has_architecture() )
    {
        const sword::UrbanAttributes::Architecture& architecture = message.architecture();
        Initialize( objectTypes, architecture.height(), architecture.floor_number(), architecture.parking_floors(), architecture.occupation(),
            architecture.trafficability(), architecture.material(), architecture.roof_shape() );
    }
    else
        Initialize( objectTypes, 20.f, 6, 0, 0.5f, 0.5f );
    CreateDictionnary( true );
}

// -----------------------------------------------------------------------------
// Name: Architecture destructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
Architecture::~Architecture()
{
    // NOTHING
}
