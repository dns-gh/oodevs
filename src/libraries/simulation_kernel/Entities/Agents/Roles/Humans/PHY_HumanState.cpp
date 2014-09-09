// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_HumanState.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "simulation_kernel/CheckPoints/MIL_CheckPointInArchive.h"
#include "simulation_kernel/CheckPoints/MIL_CheckPointOutArchive.h"

using namespace human;

// -----------------------------------------------------------------------------
// Name: PHY_HumanState Constructor
// Created: ABR 2011-07-18
// -----------------------------------------------------------------------------
PHY_HumanState::PHY_HumanState()
    : number_      ( 0 )
    , rank_        ( &PHY_HumanRank::officier_ )
    , state_       ( &PHY_HumanWound::notWounded_ )
    , location_    ( eHumanLocation_Battlefield )
    , contaminated_( false )
    , psyop_       ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanState constructor
// Created: ABR 2011-07-18
// -----------------------------------------------------------------------------
PHY_HumanState::PHY_HumanState( unsigned int number, const PHY_HumanRank& rank, const PHY_HumanWound& state, E_HumanLocation location /*= eHumanLocation_Battlefield*/, bool contaminated /*= false*/, bool psyop /*= false*/ )
    : number_      ( number )
    , rank_        ( &rank )
    , state_       ( &state )
    , location_    ( location )
    , contaminated_( contaminated )
    , psyop_       ( psyop )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanState destructor
// Created: ABR 2011-07-18
// -----------------------------------------------------------------------------
PHY_HumanState::~PHY_HumanState()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanState::load
// Created: ABR 2011-07-18
// -----------------------------------------------------------------------------
void PHY_HumanState::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    unsigned int rankId;
    file >> number_;
    file >> rankId;
    file >> state_;
    file >> location_;
    file >> contaminated_;
    file >> psyop_;
    rank_ = PHY_HumanRank::Find( rankId );
    assert( rank_ && state_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanState::save
// Created: ABR 2011-07-18
// -----------------------------------------------------------------------------
void PHY_HumanState::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    const unsigned int rankId = rank_->GetID();
    file << number_;
    file << rankId;
    file << state_;
    file << location_;
    file << contaminated_;
    file << psyop_;
}

// -----------------------------------------------------------------------------
// Name: PHY_HumanState::Write
// Created: LDC 2012-08-26
// -----------------------------------------------------------------------------
void PHY_HumanState::Write( xml::xostream& xos ) const
{
    xos << xml::start( "human" )
            << xml::attribute( "number", number_ )
            << xml::attribute( "rank", rank_->GetName() )
            << xml::attribute( "state", state_->GetName() );
    if( contaminated_ )
        xos << xml::attribute( "contaminated", contaminated_ );
    if( psyop_ )
        xos << xml::attribute( "psyop", psyop_ );
    xos << xml::end;
}
