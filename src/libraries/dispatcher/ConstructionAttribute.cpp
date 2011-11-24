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
ConstructionAttribute::ConstructionAttribute( const sword::ObjectAttributes& asnMsg )
    : hasDotation_                ( false )
    , dotation_                   ( 0 )
    , nNbrDotationForConstruction_( 0 )
    , nPercentageConstruction_    ( 0 )
    , density_                    ( 1.0f )
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
void ConstructionAttribute::Update( const sword::ObjectAttributes& asnMsg )
{
    if( asnMsg.has_construction()  )
    {
        if( asnMsg.construction().has_resource() )
        {
            hasDotation_ = true;
            dotation_ = asnMsg.construction().resource().id();
        }
        if( asnMsg.construction().has_dotation() )
            nNbrDotationForConstruction_ = asnMsg.construction().dotation();
        if( asnMsg.construction().has_percentage() )
            nPercentageConstruction_ = asnMsg.construction().percentage();
        if( asnMsg.construction().has_density() )
            density_ = asnMsg.construction().density();
    }
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void ConstructionAttribute::Send( sword::ObjectAttributes& asnMsg ) const
{
    if( hasDotation_ )
    {
        asnMsg.mutable_construction()->mutable_resource()->set_id( dotation_ );
        asnMsg.mutable_construction()->set_dotation( nNbrDotationForConstruction_ );
    }
    asnMsg.mutable_construction()->set_percentage( nPercentageConstruction_ );
    asnMsg.mutable_construction()->set_density( density_ );
}
