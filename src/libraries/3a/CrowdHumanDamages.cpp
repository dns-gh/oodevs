// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "CrowdHumanDamages.h"
#include "protocol/Simulation.h"

using namespace sword;
using namespace extractors;

// -----------------------------------------------------------------------------
// Name: CrowdHumanDamages constructor
// Created: FPO 2011-05-26
// -----------------------------------------------------------------------------
CrowdHumanDamages::CrowdHumanDamages()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CrowdHumanDamages destructor
// Created: FPO 2011-05-26
// -----------------------------------------------------------------------------
CrowdHumanDamages::~CrowdHumanDamages()
{
    // NOTHING
}

namespace
{
    const unsigned int nHumanDamageStates = 6;

    typedef google::protobuf::int32( UnitHumanFireDamage::*UnitHumanFireDamageMemberFn )()const;

    UnitHumanFireDamageMemberFn humanDamageData[ nHumanDamageStates ] =
    {
        &UnitHumanFireDamage::alive,
        &UnitHumanFireDamage::dead,
        &UnitHumanFireDamage::wounded_u1,
        &UnitHumanFireDamage::wounded_u2,
        &UnitHumanFireDamage::wounded_u3,
        &UnitHumanFireDamage::wounded_ue,
    };

    const char* humanDamageStates[ nHumanDamageStates ] =
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
// Name: CrowdHumanDamages constructor
// Created: FPO 2011-05-26
// -----------------------------------------------------------------------------
CrowdHumanDamages::CrowdHumanDamages( xml::xistream& xis )
    : rankMask_   ( ReadRanks( xis ) )
    , stateMask_  ( ReadStates( xis ) )
    , partyFilter_( xis, "party" )
    , crowdFilter_( xis, "crowd" )
{
    // NOTHING
}

bool CrowdHumanDamages::HasValue( const sword::SimToClient& wrapper ) const
{
    return wrapper.message().has_unit_damaged_by_crowd_fire();
}

// -----------------------------------------------------------------------------
// Name: CrowdHumanDamages::Extract
// Created: FPO 2011-05-26
// -----------------------------------------------------------------------------
int CrowdHumanDamages::Extract( const SimToClient& wrapper ) const
{
    const UnitDamagedByCrowdFire& damages = wrapper.message().unit_damaged_by_crowd_fire();
    if( !partyFilter_.IsAllowed( damages.party().id() ) )
        return 0;
    if( !crowdFilter_.IsAllowed( damages.firer().id() ) )
        return 0;
    int result = 0;
    for( int e = 0; e < damages.humans().elem_size(); ++e )
    {
        const UnitHumanFireDamage& damage = damages.humans().elem( e );
        if( ( rankMask_ & ( 1 << damage.rank() ) ) != 0 )
            for( unsigned int i = 0; i < nHumanDamageStates; ++i )
                if( ( stateMask_ & ( 1 << i ) ) != 0 )
                    result += ( damage.*humanDamageData[ i ] )();
    }
    return result;
}
