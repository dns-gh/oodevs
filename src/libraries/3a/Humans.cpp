// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Humans.h"

#include "protocol/Simulation.h"

using namespace sword;
using namespace extractors;

// -----------------------------------------------------------------------------
// Name: Humans constructor
// Created: AGE 2007-10-29
// -----------------------------------------------------------------------------
Humans::Humans()
{
    // NOTHING
}

namespace
{
    const unsigned nHumanStates = 9;
    typedef google::protobuf::int32( HumanDotations_HumanDotation::*HumanDotationsMemberFn )()const;
    HumanDotationsMemberFn humanData[nHumanStates] =
    {
        &HumanDotations_HumanDotation::total,
        &HumanDotations_HumanDotation::operational,
        &HumanDotations_HumanDotation::dead,
        &HumanDotations_HumanDotation::wounded,
        &HumanDotations_HumanDotation::mentally_wounded,
        &HumanDotations_HumanDotation::contaminated,
        &HumanDotations_HumanDotation::healing,
        &HumanDotations_HumanDotation::maintenance,
        &HumanDotations_HumanDotation::unevacuated_wounded
    };
    const char* humanStates[nHumanStates] =
    {
        "total",
        "operational",
        "dead",
        "wounded",
        "mental",
        "nbc",
        "in-treatment",
        "in-maintenance",
        "wounded-not-evacuated",
    };
    int ReadMask( xml::xistream& xis, const char* attribute, const char** names, unsigned int count )
    {
        if( !xis.has_attribute( attribute ) )
            return 0xFFFF;
        FilterHelper< std::string > states( xis, attribute );
        int result = 0;
        for( unsigned int i = 0; i < count; ++i )
            if( states.IsAllowed( names[i] ) )
                result |= ( 1 << i );
        return result;
    }
    int ReadStates( xml::xistream& xis )
    {
        return ReadMask( xis, "states", humanStates, nHumanStates );
    }

    const char* ranks[3] =
    {
        "officer",
        "sub-officer",
        "troopers"
    };
    int ReadRanks( xml::xistream& xis )
    {
        return ReadMask( xis, "ranks", ranks, 3 );
    }
}

// -----------------------------------------------------------------------------
// Name: Humans constructor
// Created: AGE 2007-10-29
// -----------------------------------------------------------------------------
Humans::Humans( xml::xistream& xis )
    : rankMask_ ( ReadRanks( xis ) )
    , stateMask_( ReadStates( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Humans::Extract
// Created: AGE 2007-10-29
// -----------------------------------------------------------------------------
int Humans::Extract( const UnitAttributes& attributes )
{
    int result = 0;
    unsigned int size = attributes.human_dotations().elem_size();
    while( size > 0 )
    {
        --size;
        const HumanDotations_HumanDotation& humans = attributes.human_dotations().elem( size );
        if( ( rankMask_ & ( 1 << humans.rank() ) ) != 0 )
            for( unsigned int i = 0; i < nHumanStates; ++i )
                if( ( stateMask_ & ( 1 << i ) ) != 0 )
                    result += ( humans.*humanData[i] )();
    }
    return result;
}
