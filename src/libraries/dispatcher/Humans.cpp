// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Humans.h"
#include "protocol/Protocol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Humans constructor
// Created: ABR 2011-07-21
// -----------------------------------------------------------------------------
Humans::Humans()
    : number_      ( 0 )
    , rank_        ( sword::officer )
    , state_       ( sword::healthy )
    , location_    ( sword::battlefield )
    , injuries_    ()
    , contaminated_( false )
    , psyop_       ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Humans destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Humans::~Humans()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Humans::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Humans::Update( const sword::HumanDotations_HumanDotation& message )
{
    number_       = message.quantity();
    rank_         = message.rank();
    state_        = message.state();
    location_     = message.location();
    assert( state_ == sword::injured && message.injuries_size() > 0 || state_ != sword::injured );
    injuries_.resize( message.injuries_size() );
    for( unsigned int i = 0; i < message.injuries_size(); ++i )
    {
        injuries_[ i ].first = message.injuries( i ).id();
        injuries_[ i ].second = message.injuries( i ).seriousness();
    }
    contaminated_ = message.has_contaminated() ? message.contaminated() : false;
    psyop_        = message.has_mentally_wounded() ? message.mentally_wounded() : false;
}

// -----------------------------------------------------------------------------
// Name: Humans::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Humans::Send( sword::HumanDotations_HumanDotation& message ) const
{
    message.set_quantity( number_ );
    message.set_rank( rank_ );
    message.set_state( state_ );
    message.set_location( location_ );
    for( unsigned int i = 0; i < injuries_.size(); ++i )
    {
        sword::Injury* injury = message.mutable_injuries()->Add();
        injury->set_id( injuries_[ i ].first );
        injury->set_seriousness( injuries_[ i ].second );
    }
    message.set_contaminated( contaminated_ );
    message.set_mentally_wounded( psyop_ );
}
