// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "PopulationStates.h"
#include "protocol/Protocol.h"

using namespace sword;
using namespace extractors;

// -----------------------------------------------------------------------------
// Name: PopulationStates constructor
// Created: FPO 2011-05-05
// -----------------------------------------------------------------------------
PopulationStates::PopulationStates()
{
    // NOTHING
}

namespace
{
    bool ReadMask( xml::xistream& xis, std::string state )
    {
        if( !xis.has_attribute( "states" ) )
            return true;
        FilterHelper< std::string > equipments( xis, "states" );
        bool result = false;
        if( equipments.IsAllowed( state ) )
            result = true;
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationStates constructor
// Created: FPO 2011-05-05
// -----------------------------------------------------------------------------
PopulationStates::PopulationStates( xml::xistream& xis )
    : healthy_( ReadMask ( xis, "healthy" ) )
    , wounded_( ReadMask ( xis, "wounded" ) )
    , dead_   ( ReadMask ( xis, "dead" ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationStates destructor
// Created: FPO 2011-05-05
// -----------------------------------------------------------------------------
PopulationStates::~PopulationStates()
{
    // NOTHING
}

bool PopulationStates::HasValue( const sword::SimToClient& wrapper ) const
{
    return ( wrapper.message().has_population_update() );
}

// -----------------------------------------------------------------------------
// Name: PopulationStates::Extract
// Created: FPO 2011-05-05
// -----------------------------------------------------------------------------
int PopulationStates::Extract( const sword::SimToClient& wrapper )
{
    int result = 0;
    const PopulationUpdate& update = wrapper.message().population_update();

    if( healthy_.isAsked_ && update.has_healthy() )
        healthy_.quantity_ = update.healthy();
    if( wounded_.isAsked_ && update.has_wounded() )
        wounded_.quantity_ = update.wounded();
    if( dead_.isAsked_ && update.has_dead() )
        dead_.quantity_ = update.dead();

    result += healthy_.quantity_;
    result += wounded_.quantity_;
    result += dead_.quantity_;
    return result;
}
