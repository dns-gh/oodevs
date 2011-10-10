// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "DPRESatisfaction.h"

using namespace extractors;

// -----------------------------------------------------------------------------
// Name: DPRESatisfaction constructor
// Created: JSR 2011-08-12
// -----------------------------------------------------------------------------
DPRESatisfaction::DPRESatisfaction()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DPRESatisfaction destructor
// Created: JSR 2011-08-12
// -----------------------------------------------------------------------------
DPRESatisfaction::~DPRESatisfaction()
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
// Name: DPRESatisfaction constructor
// Created: JSR 2011-08-12
// -----------------------------------------------------------------------------
DPRESatisfaction::DPRESatisfaction( xml::xistream& xis )
    : lodging_( ReadMask ( xis, "lodging" ) )
    , health_ ( ReadMask ( xis, "health" ) )
    , safety_ ( ReadMask ( xis, "safety" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DPRESatisfaction::Extract
// Created: JSR 2011-08-12
// -----------------------------------------------------------------------------
float DPRESatisfaction::Extract( const sword::SimToClient& wrapper )
{
    float result = 0;
    int count = 0;
    const sword::UnitAttributes::Satisfaction& satisfactions = wrapper.message().unit_attributes().satisfaction();

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
        if( satisfactions.has_access_to_health_care() )
            health_.value_ = satisfactions.access_to_health_care();
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
