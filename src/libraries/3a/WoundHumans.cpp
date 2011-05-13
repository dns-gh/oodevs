// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "WoundHumans.h"

using namespace sword;
using namespace extractors;

// -----------------------------------------------------------------------------
// Name: WoundHumans constructor
// Created: FPO 2011-05-02
// -----------------------------------------------------------------------------
WoundHumans::WoundHumans()
{
    // NOTHING
}

namespace
{
    const unsigned nHumanDamageStates = 6;
    typedef google::protobuf::int32( UnitHumanFireDamage::*UnitHumanFireDamageMemberFn )()const;
    UnitHumanFireDamageMemberFn humanDamageData[6] =
    {
        &UnitHumanFireDamage::alive,
        &UnitHumanFireDamage::dead,
        &UnitHumanFireDamage::wounded_u1,
        &UnitHumanFireDamage::wounded_u2,
        &UnitHumanFireDamage::wounded_u3,
        &UnitHumanFireDamage::wounded_ue,
    };
    const char* humanDamageStates[6] =
    {
        "alive",
        "dead",
        "wounded_u1",
        "wounded_u2",
        "wounded_u3",
        "wounded_ue",
    };
    int ReadMask( xml::xistream& xis, const char* attribute, const char** names, unsigned int count )
    {
        if( xis.attribute< std::string >( attribute ).empty() )
            return 1;
        FilterHelper< std::string > states( xis, attribute );
        int result = 0;
        for( unsigned int i = 0; i < count; ++i )
            if( states.IsAllowed( names[i] ) )
                result |= ( 1 << i );
        return result;
    }
    int ReadStates( xml::xistream& xis )
    {
        return ReadMask( xis, "states", humanDamageStates, nHumanDamageStates );
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
// Name: WoundHumans destructor
// Created: FPO 2011-05-02
// -----------------------------------------------------------------------------
WoundHumans::~WoundHumans()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WoundHumans constructor
// Created: FPO 2011-05-02
// -----------------------------------------------------------------------------
WoundHumans::WoundHumans( xml::xistream& xis )
    : selectFilter_ ( xis, "select" )
    , rankMask_ ( ReadRanks( xis ) )
    , stateMask_( ReadStates( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WoundHumans::Extract
// Created: FPO 2011-05-02
// -----------------------------------------------------------------------------
float WoundHumans::Extract( const sword::SimToClient& wrapper ) const
{
    const StopUnitFire& stop = wrapper.message().stop_unit_fire();
    float result = 0;
    for( int u = 0; u < stop.units_damages().elem_size(); ++u )
    {
        const UnitFireDamages& damages = stop.units_damages().elem( u );
        for( int e = 0; e < damages.humans().elem_size(); ++e )
        {
            const UnitHumanFireDamage& damage = damages.humans().elem( e );
            if( ( rankMask_ & ( 1 << damage.rank() ) ) != 0 && selectFilter_.IsAllowed( damages.target().id() ) )
                for( unsigned int i = 0; i < nHumanDamageStates; ++i )
                    if( ( stateMask_ & ( 1 << i ) ) != 0 )
                        result += ( damage.*humanDamageData[i] )();
        }
    }
    return result;
}
