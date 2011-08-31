// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_pch.h"
#include "HumanState.h"
#include "protocol/Protocol.h"

// -----------------------------------------------------------------------------
// Name: HumanState constructor
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
HumanState::HumanState()
    : rank_        ( eHumanRank_Officier )
    , state_       ( eHumanState_Healthy )
    , location_    ( eHumanLocation_Battlefield )
    , psyop_       ( false )
    , contaminated_( false )
    , quantity_    ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HumanState constructor
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
HumanState::HumanState( const HumanState& other )
    : rank_        ( other.rank_ )
    , state_       ( other.state_ )
    , location_    ( other.location_ )
    , psyop_       ( other.psyop_ )
    , contaminated_( other.contaminated_ )
    , quantity_    ( other.quantity_ )
{
    for( unsigned int i = 0; i < other.injuries_.size(); ++i )
        injuries_.push_back( other.injuries_[ i ] );
}

// -----------------------------------------------------------------------------
// Name: HumanState constructor
// Created: ABR 2011-07-28
// -----------------------------------------------------------------------------
HumanState::HumanState( const sword::HumanDotations_HumanDotation& msg )
{
    Update( msg );
}

// -----------------------------------------------------------------------------
// Name: HumanState destructor
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
HumanState::~HumanState()
{
    injuries_.clear();
}

// -----------------------------------------------------------------------------
// Name: HumanState::Update
// Created: ABR 2011-07-25
// -----------------------------------------------------------------------------
void HumanState::Update( const sword::HumanDotations_HumanDotation& message )
{
    rank_         = static_cast< E_HumanRank >( message.rank() );
    state_        = static_cast< E_HumanState >( message.state() );
    location_     = static_cast< E_HumanLocation >( message.location() );
    psyop_        = message.has_mentally_wounded() ? message.mentally_wounded() : false;
    contaminated_ = message.has_contaminated() ? message.contaminated() : false;
    quantity_     = message.quantity();

    assert( state_ == eHumanState_Injured && message.injuries_size() > 0 || state_ != eHumanState_Injured );
    injuries_.resize( message.injuries_size() );
    for( int i = 0; i < message.injuries_size(); ++i )
    {
        injuries_[ i ].first = message.injuries( i ).id();
        injuries_[ i ].second = static_cast< E_InjuriesSeriousness >( message.injuries( i ).seriousness() );
    }
}

// -----------------------------------------------------------------------------
// Name: HumanState::operator==
// Created: ABR 2011-08-29
// -----------------------------------------------------------------------------
bool HumanState::operator==( const HumanState& other ) const
{
    return( rank_ == other.rank_ && state_ == other.state_ && psyop_ == other.psyop_ && contaminated_ == other.contaminated_ && location_ == other.location_ && injuries_.size() == other.injuries_.size() );
}
