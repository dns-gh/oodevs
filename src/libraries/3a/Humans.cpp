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

#define GETTER( NAME, CONDITION ) \
google::protobuf::int32 Get##NAME##( const HumanDotations_HumanDotation& humans ) \
{ \
    return ( CONDITION ) ? humans.quantity() : 0 ; \
}

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
    GETTER( Total, true );
    GETTER( Operational, humans.state() == sword::healthy && humans.location() != sword::medical && !humans.mentally_wounded() && !humans.contaminated() );
    GETTER( Dead, humans.state() == sword::deadly );
    GETTER( Wounded, humans.state() == sword::injured );
    GETTER( MentallyWounded, humans.mentally_wounded() );
    GETTER( Contaminated, humans.contaminated() );
    GETTER( Healing, humans.location() == sword::medical );
    GETTER( Maintenance, humans.location() == sword::maintenance && humans.state() != sword::dead );
    GETTER( UnevacuatedWounded, humans.location() == sword::battlefield && humans.state() == sword::injured );
    GETTER( WoundedU1, humans.state() == sword::injured && humans.injuries_size() > 0 && humans.injuries( 0 ).seriousness() == wounded_u1 );
    GETTER( WoundedU2, humans.state() == sword::injured && humans.injuries_size() > 0 && humans.injuries( 0 ).seriousness() == wounded_u2 );
    GETTER( WoundedU3, humans.state() == sword::injured && humans.injuries_size() > 0 && humans.injuries( 0 ).seriousness() == wounded_u3 );
    GETTER( WoundedUE, humans.state() == sword::injured && humans.injuries_size() > 0 && humans.injuries( 0 ).seriousness() == wounded_ue  );

    const unsigned nHumanStates = 13;
    typedef google::protobuf::int32 ( HumanHelperFn )( const HumanDotations_HumanDotation& humans );
    HumanHelperFn* humanData[ nHumanStates ] =
    {
        &GetTotal,
        &GetOperational,
        &GetDead,
        &GetWounded,
        &GetMentallyWounded,
        &GetContaminated,
        &GetHealing,
        &GetMaintenance,
        &GetUnevacuatedWounded,
        &GetWoundedU1,
        &GetWoundedU2,
        &GetWoundedU3,
        &GetWoundedUE
    };
    const char* humanStates[ nHumanStates ] =
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
        "wounded-u1",
        "wounded-u2",
        "wounded-u3",
        "wounded-ue"
    };
    int ReadMask( xml::xistream& xis, const char* attribute, const char** names, unsigned int count )
    {
        if( !xis.has_attribute( attribute ) )
            return 0xFFFF;
        FilterHelper< std::string > states( xis, attribute );
        int result = 0;
        for( unsigned int i = 0; i < count; ++i )
            if( states.IsAllowed( names[ i ] ) )
                result |= ( 1 << i );
        return result;
    }
    int ReadStates( xml::xistream& xis )
    {
        return ReadMask( xis, "states", humanStates, nHumanStates );
    }

    const char* ranks[ 3 ] =
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

bool Humans::HasFlag( const sword::UnitAttributes& attributes ) const
{
    return attributes.has_human_dotations();
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
