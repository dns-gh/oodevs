// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ConstructionAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ConstructionAttribute::ConstructionAttribute( const Common::MsgObjectAttributes& asnMsg )
    : dotation_ ( 0 )
    , nNbrDotationForConstruction_( 0 )
    , nPercentageConstruction_( 0 )
{
    Update( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
ConstructionAttribute::~ConstructionAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void ConstructionAttribute::Update( const Common::MsgObjectAttributes& asnMsg )
{
    if( asnMsg.has_construction()  )
    {
        if ( asnMsg.construction().has_resource() )
            dotation_ = asnMsg.construction().resource().id();
        if( asnMsg.construction().has_dotation_nbr() )
            nNbrDotationForConstruction_ = asnMsg.construction().dotation_nbr();
        if( asnMsg.construction().has_percentage() )
            nPercentageConstruction_ = asnMsg.construction().percentage();
    }
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ConstructionAttribute::Send( Common::MsgObjectAttributes& asnMsg ) const
{
    asnMsg.mutable_construction()->mutable_resource()->set_id( dotation_ );
    asnMsg.mutable_construction()->set_dotation_nbr( nNbrDotationForConstruction_ );
    asnMsg.mutable_construction()->set_percentage( nPercentageConstruction_ );
}
