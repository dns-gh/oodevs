// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Usages.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: Usages constructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Usages::Usages( const sword::UrbanAttributes& message, gui::PropertiesDictionary& dictionary, const kernel::AccommodationTypes& accommodationTypes,
                float livingSpace, kernel::Entity_ABC& owner )
    : gui::Usages( dictionary, accommodationTypes, livingSpace, owner )
{
    for( int i = 0; i <  message.usages_size(); ++i )
    {
        const sword::UrbanUsage& usage =  message.usages( i );
        Add( usage.role(), usage.percentage() );
    }
}

// -----------------------------------------------------------------------------
// Name: Usages destructor
// Created: LGY 2011-04-14
// -----------------------------------------------------------------------------
Usages::~Usages()
{
    // NOTHING
}
