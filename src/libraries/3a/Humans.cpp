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

using namespace MsgsSimToClient;
using namespace extractors;

// -----------------------------------------------------------------------------
// Name: Humans constructor
// Created: AGE 2007-10-29
// -----------------------------------------------------------------------------
Humans::Humans()
{
    humans_.resize( 3 );
}

namespace
{
    const unsigned nHumanStates = 8;
    typedef int (HumanDotations_HumanDotation::*HumanDotationsMemberFn)()const;
    HumanDotationsMemberFn humanData[8] =
    {
        &HumanDotations_HumanDotation::nb_total,
        &HumanDotations_HumanDotation::nb_operationnels,
        &HumanDotations_HumanDotation::nb_morts,
        &HumanDotations_HumanDotation::nb_blesses,
        &HumanDotations_HumanDotation::nb_blesses_mentaux,
        &HumanDotations_HumanDotation::nb_contamines_nbc,
        &HumanDotations_HumanDotation::nb_dans_chaine_sante,
        &HumanDotations_HumanDotation::nb_utilises_pour_maintenance,
    };
    const char* humanStates[8] =
    {
        "total",
        "operational",
        "dead",
        "wounded",
        "mental",
        "nbc",
        "in-treatment",
        "in-maintenance",
    };
    int ReadMask( xml::xistream& xis, const char* attribute, const char** names, unsigned count )
    {
        if( xml::attribute< std::string >( xis, attribute ).empty() )
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
    : rankMask_( ReadRanks( xis ) )
    , stateMask_( ReadStates( xis ) )
{
    humans_.resize( 3 );
}

// -----------------------------------------------------------------------------
// Name: Humans::Extract
// Created: AGE 2007-10-29
// -----------------------------------------------------------------------------
int Humans::Extract( const MsgUnitAttributes& attributes )
{
    unsigned size = attributes.dotation_eff_personnel().elem_size();
    while( size > 0 )
    {
        --size;
        const HumanDotations_HumanDotation& humans = attributes.dotation_eff_personnel().elem( size );
        if( ( rankMask_ & ( 1 << humans.rang() ) ) != 0 )
        {
            int quantity = 0;
            HumanDotationsMemberFn *data = humanData;
            for( unsigned i = 0; i < nHumanStates; ++i, ++data )
                if( ( stateMask_ & ( 1 << i ) ) != 0 )
                    quantity += (humans.*data[i])();
            humans_[ humans.rang() ] += quantity;
        }
    }
    int result = 0;
    for( std::vector< int >::const_iterator it = humans_.begin(); it != humans_.end(); ++it )
        result += *it;
    return result;
}
