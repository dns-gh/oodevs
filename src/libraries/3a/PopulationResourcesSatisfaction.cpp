// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "PopulationResourcesSatisfaction.h"
#include "protocol/Simulation.h"

using namespace extractors;

// -----------------------------------------------------------------------------
// Name: PopulationResourcesSatisfaction constructor
// Created: JSR 2014-10-08
// -----------------------------------------------------------------------------
PopulationResourcesSatisfaction::PopulationResourcesSatisfaction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationResourcesSatisfaction destructor
// Created: JSR 2014-10-08
// -----------------------------------------------------------------------------
PopulationResourcesSatisfaction::~PopulationResourcesSatisfaction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationResourcesSatisfaction constructor
// Created: JSR 2014-10-08
// -----------------------------------------------------------------------------
PopulationResourcesSatisfaction::PopulationResourcesSatisfaction( xml::xistream& xis )
    : filter_( xis, "dotations", "resources" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationResourcesSatisfaction::HasValue
// Created: JSR 2014-10-08
// -----------------------------------------------------------------------------
bool PopulationResourcesSatisfaction::HasValue( const sword::SimToClient& wrapper ) const
{
    return ( wrapper.message().has_population_update()
            && wrapper.message().population_update().has_satisfaction()
            && wrapper.message().population_update().satisfaction().resources_size() > 0 );
}

// -----------------------------------------------------------------------------
// Name: PopulationResourcesSatisfaction::Extract
// Created: JSR 2014-10-08
// -----------------------------------------------------------------------------
float PopulationResourcesSatisfaction::Extract( const sword::SimToClient& wrapper )
{
    float result = 0;
    int count = 0;
    const auto& msg = wrapper.message().population_update().satisfaction();
    for( int i = 0; i < msg.resources_size(); ++i )
        if( filter_.IsAllowed( msg.resources( i ).resource().id() ) )
        {
            result += msg.resources( i ).value();
            ++count;
        }

    if( count != 0 )
        result /= count;

    return result;
}
