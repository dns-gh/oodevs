// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "PopulationUrbanOccupation.h"

using namespace extractors;

// -----------------------------------------------------------------------------
// Name: PopulationUrbanOccupation constructor
// Created: FPO 2011-05-12
// -----------------------------------------------------------------------------
PopulationUrbanOccupation::PopulationUrbanOccupation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationUrbanOccupation destructor
// Created: FPO 2011-05-12
// -----------------------------------------------------------------------------
PopulationUrbanOccupation::~PopulationUrbanOccupation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationUrbanOccupation constructor
// Created: FPO 2011-05-12
// -----------------------------------------------------------------------------
PopulationUrbanOccupation::PopulationUrbanOccupation( xml::xistream& xis )
    : filter_  ( xis, "blocks" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationUrbanOccupation::Extract
// Created: FPO 2011-05-12
// -----------------------------------------------------------------------------
int PopulationUrbanOccupation::Extract( const sword::SimToClient& wrapper )
{
    int result = 0;
    const sword::PopulationUpdate& update = wrapper.message().population_update();

    for( int i = 0; i < update.occupations_size(); ++i )
    {
        const sword::PopulationUpdate_BlockOccupation& blockOccupation = update.occupations( i );
        if( filter_.IsAllowed( blockOccupation.object().id() ) )
            for( int j = 0; j < blockOccupation.persons_size(); ++j )
                result += blockOccupation.persons( j ).number();
    }
    return result;
}
