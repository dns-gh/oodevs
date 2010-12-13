// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
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
    typedef google::protobuf::int32 (HumanDotations_HumanDotation::*HumanDotationsMemberFn)()const;
    HumanDotationsMemberFn humanData[9] =
    {
        &HumanDotations_HumanDotation::nb_total,
        &HumanDotations_HumanDotation::nb_operationnels,
        &HumanDotations_HumanDotation::nb_morts,
        &HumanDotations_HumanDotation::nb_blesses,
        &HumanDotations_HumanDotation::nb_blesses_mentaux,
        &HumanDotations_HumanDotation::nb_contamines_nbc,
        &HumanDotations_HumanDotation::nb_dans_chaine_sante,
        &HumanDotations_HumanDotation::nb_utilises_pour_maintenance,
        &HumanDotations_HumanDotation::nb_blesses_non_evacues,
    };
    const char* humanStates[9] =
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
    int ReadMask( xml::xistream& xis, const char* attribute, const char** names, unsigned count )
    {
        if( xis.attribute< std::string >( attribute ).empty() )
            return 1;
        FilterHelper< std::string > states( xis, attribute );
        int result = 0;
        for( unsigned i = 0; i < count; ++i )
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
    unsigned size = attributes.dotation_eff_personnel().elem_size();
    while( size > 0 )
    {
        --size;
        const HumanDotations_HumanDotation& humans = attributes.dotation_eff_personnel().elem( size );
        if( ( rankMask_ & ( 1 << humans.rang() ) ) != 0 )
            for( unsigned i = 0; i < nHumanStates; ++i )
                if( ( stateMask_ & ( 1 << i ) ) != 0 )
                    result += (humans.*humanData[i])();
    }
    return result;
}
