// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "PopulationSatisfaction.h"

using namespace extractors;

// -----------------------------------------------------------------------------
// Name: PopulationSatisfaction constructor
// Created: FPO 2011-05-10
// -----------------------------------------------------------------------------
PopulationSatisfaction::PopulationSatisfaction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationSatisfaction destructor
// Created: FPO 2011-05-10
// -----------------------------------------------------------------------------
PopulationSatisfaction::~PopulationSatisfaction()
{
    // NOTHING
}

namespace
{
    bool ReadMask( xml::xistream& xis, std::string type )
    {
        if( !xis.has_attribute( "type" ) )
            return true;
        FilterHelper< std::string > filter( xis, "type" );
        bool result = false;
        if( filter.IsAllowed( type ) )
            result = true;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationSatisfaction constructor
// Created: FPO 2011-05-10
// -----------------------------------------------------------------------------
PopulationSatisfaction::PopulationSatisfaction( xml::xistream& xis )
    : lodging_( ReadMask ( xis, "lodging" ) )
    , health_ ( ReadMask ( xis, "health" ) )
    , safety_ ( ReadMask ( xis, "safety" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationSatisfaction::Extract
// Created: FPO 2011-05-10
// -----------------------------------------------------------------------------
float PopulationSatisfaction::Extract( const sword::SimToClient& wrapper )
{
    float result = 0;
    int count = 0;
    const sword::PopulationUpdate::Satisfaction& satisfactions = wrapper.message().population_update().satisfaction();

    if( lodging_.isAsked_ )
    {
        ++count;
        if( satisfactions.has_lodging() )
            lodging_.value_ = satisfactions.lodging();
        result += lodging_.value_;
    }

    if( health_.isAsked_ )
    {
        ++count;
        if( satisfactions.has_health() )
            health_.value_ = satisfactions.health();
        result += health_.value_;
    }

    if( safety_.isAsked_ )
    {
        ++count;
        if( satisfactions.has_safety() )
            safety_.value_ = satisfactions.safety();
        result += safety_.value_;
    }

    if( count != 0 )
        result /= count;

    return result;
}
