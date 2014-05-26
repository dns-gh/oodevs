// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Dotation.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Dotation constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Dotation::Dotation( const sword::ResourceDotations_ResourceDotation& asnMsg )
    : nDotationType_           ( asnMsg.type().id() )
    , nNbr_                    ( asnMsg.quantity() )
    , rLowThresholdPercentage_ ( asnMsg.low_threshold() )
    , rHighThresholdPercentage_( asnMsg.high_threshold() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotation constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
Dotation::Dotation( const sword::DotationStock & asnMsg )
    : nDotationType_           ( asnMsg.resource().id() )
    , nNbr_                    ( asnMsg.quantity() )
    , rLowThresholdPercentage_ ( 0.f )
    , rHighThresholdPercentage_( 0.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotation destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Dotation::~Dotation()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotation::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Dotation::Update( const sword::ResourceDotations_ResourceDotation& asnMsg )
{
    nNbr_ = asnMsg.quantity();
    rLowThresholdPercentage_ = asnMsg.low_threshold();
    rHighThresholdPercentage_ = asnMsg.high_threshold();
}

// -----------------------------------------------------------------------------
// Name: Dotation::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Dotation::Update( const sword::DotationStock& asnMsg )
{
    nNbr_ = asnMsg.quantity();
}

// -----------------------------------------------------------------------------
// Name: Dotation::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Dotation::Send( sword::ResourceDotations_ResourceDotation& asnMsg ) const
{
    asnMsg.mutable_type()->set_id( nDotationType_ );
    asnMsg.set_quantity( nNbr_ );
    asnMsg.set_low_threshold( rLowThresholdPercentage_ );
    asnMsg.set_high_threshold( rHighThresholdPercentage_ );
}

// -----------------------------------------------------------------------------
// Name: Dotation::Send
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Dotation::Send( sword::DotationStock& asnMsg ) const
{
    asnMsg.mutable_resource()->set_id( nDotationType_ );
    asnMsg.set_quantity( nNbr_ );
}
