// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
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
   : nDotationType_             ( asnMsg.type().id() )
   , nNbr_                      ( asnMsg.quantity() )
   , rSupplyThresholdPercentage_( asnMsg.threshold() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Dotation constructor
// Created: NLD 2006-10-02
// -----------------------------------------------------------------------------
Dotation::Dotation( const sword::DotationStock & asnMsg )
   : nDotationType_             ( asnMsg.resource().id() )
   , nNbr_                      ( asnMsg.quantity() )
   , rSupplyThresholdPercentage_( 0.f )
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
    rSupplyThresholdPercentage_ = asnMsg.threshold();
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
    asnMsg.set_threshold( rSupplyThresholdPercentage_ );
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
