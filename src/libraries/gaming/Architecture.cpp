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
Architecture::Architecture( const sword::UrbanAttributes& message, kernel::PropertiesDictionary& dictionary )
    : gui::Architecture( dictionary )
{
    const sword::UrbanAttributes::Architecture& architecture = message.architecture();
    Initialize( static_cast< unsigned int >( architecture.height() ), architecture.floor_number(), architecture.parking_floors(), architecture.roof_shape(),
                architecture.material(), architecture.occupation(), architecture.trafficability() );
}

// -----------------------------------------------------------------------------
// Name: Architecture destructor
// Created: LGY 2011-04-15
// -----------------------------------------------------------------------------
Architecture::~Architecture()
{
    // NOTHING
}
