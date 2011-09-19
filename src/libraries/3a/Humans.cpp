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
    inline google::protobuf::int32 GetTotal( const HumanDotations_HumanDotation& humans )
    {
        return humans.quantity();
    }
    inline google::protobuf::int32 GetOperational( const HumanDotations_HumanDotation& humans )
    {
        return ( humans.state() == sword::healthy && humans.location() != sword::medical && !humans.mentally_wounded() && !humans.contaminated() ) ? humans.quantity() : 0 ;
    }
    inline google::protobuf::int32 GetDead( const HumanDotations_HumanDotation& humans )
    {
        return ( humans.state() == sword::deadly ) ? humans.quantity() : 0;
    }
    inline google::protobuf::int32 GetWounded( const HumanDotations_HumanDotation& humans )
    {
        return ( humans.state() != sword::healthy && humans.state() != sword::deadly ) ? humans.quantity() : 0;
    }
    inline google::protobuf::int32 GetMentallyWounded( const HumanDotations_HumanDotation& humans )
    {
        return ( humans.mentally_wounded() ) ? humans.quantity() : 0;
    }
    inline google::protobuf::int32 GetContaminated( const HumanDotations_HumanDotation& humans )
    {
        return ( humans.contaminated() ) ? humans.quantity() : 0;
    }
    inline google::protobuf::int32 GetHealing( const HumanDotations_HumanDotation& humans )
    {
        return ( humans.location() == sword::medical ) ? humans.quantity() : 0;
    }
    inline google::protobuf::int32 GetMaintenance( const HumanDotations_HumanDotation& humans )
    {
        return ( humans.location() == sword::maintenance && humans.state() != sword::dead ) ? humans.quantity() : 0;
    }
    inline google::protobuf::int32 GetUnevacuatedWounded( const HumanDotations_HumanDotation& humans )
    {
        return ( humans.location() == sword::battlefield && humans.state() == sword::injured ) ? humans.quantity() : 0;
    }

    const unsigned nHumanStates = 9;
    typedef inline google::protobuf::int32 ( HumanHelperFn )( const HumanDotations_HumanDotation& humans );
    HumanHelperFn* humanData[nHumanStates] =
    {
        &GetTotal,
        &GetOperational,
        &GetDead,
        &GetWounded,
        &GetMentallyWounded,
        &GetContaminated,
        &GetHealing,
        &GetMaintenance,
        &GetUnevacuatedWounded //$$$ RPD TO IMPLEMENT
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
                    result += humanData[ i ]( humans );
    }
    return result;
}
