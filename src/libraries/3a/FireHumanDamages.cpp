// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "FireHumanDamages.h"

using namespace sword;
using namespace extractors;

// -----------------------------------------------------------------------------
// Name: FireHumanDamages constructor
// Created: FPO 2011-04-29
// -----------------------------------------------------------------------------
FireHumanDamages::FireHumanDamages()
{
    // NOTHING
}

namespace
{
    const unsigned nHumanDamageStates = 6;
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
        if( xis.attribute< std::string >( attribute ).empty() )
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
// Name: FireHumanDamages destructor
// Created: FPO 2011-04-29
// -----------------------------------------------------------------------------
FireHumanDamages::~FireHumanDamages()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireHumanDamages::FireHumanDamages
// Created: FPO 2011-05-02
// -----------------------------------------------------------------------------
FireHumanDamages::FireHumanDamages( xml::xistream& xis )
    : rankMask_ ( ReadRanks( xis ) )
    , stateMask_( ReadStates( xis ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FireHumanDamages::Extract
// Created: FPO 2011-04-29
// -----------------------------------------------------------------------------
float FireHumanDamages::Extract( const sword::SimToClient& wrapper ) const
{
    const UnitDamagedByUnitFire& damages = wrapper.message().unit_damaged_by_unit_fire();
    float result = 0;
    for( int e = 0; e < damages.humans().elem_size(); ++e )
    {
        const UnitHumanFireDamage& damage = damages.humans().elem( e );
        if( ( rankMask_ & ( 1 << damage.rank() ) ) != 0 )
            for( unsigned int i = 0; i < nHumanDamageStates; ++i )
                if( ( stateMask_ & ( 1 << i ) ) != 0 )
                    result += ( damage.*humanDamageData[i] )();
    }
    return result;
}
 