// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "StockAttribute.h"
#include "protocol/SimulationSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: StockAttribute constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
StockAttribute::StockAttribute( const sword::ObjectAttributes& asnMsg )
{
    DoUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: StockAttribute destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
StockAttribute::~StockAttribute()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void StockAttribute::Update( const sword::ObjectAttributes& asnMsg )
{
    DoUpdate( asnMsg );
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::DoUpdate
// Created: BCI 2011-02-07
// -----------------------------------------------------------------------------
void StockAttribute::DoUpdate( const sword::ObjectAttributes& asnMsg )
{
    if( asnMsg.has_stock()  )
    {
        stocks_.clear();
        for( int i = 0; i < asnMsg.stock().resources_size(); ++i )
            stocks_.push_back( StockResource( asnMsg.stock().resources( i ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::Send
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void StockAttribute::Send( sword::ObjectAttributes& asnMsg ) const
{
    for( std::vector< StockResource >::const_iterator it = stocks_.begin(); it != stocks_.end(); ++it )
    {
        sword::StockResource* tmp = asnMsg.mutable_stock()->add_resources();
        tmp->mutable_resource()->set_id( it->resource );
        tmp->set_current( it->current );
        tmp->set_maximum( it->maximum );
    }
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::StockResource::StockResource
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
StockAttribute::StockResource::StockResource()
    : resource( 0 )
    , current ( 0 )
    , maximum ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: StockAttribute::StockResource::StockResource
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
StockAttribute::StockResource::StockResource( const sword::StockResource& msg )
    : resource( msg.resource().id() )
    , current ( msg.current() )
    , maximum ( msg.has_maximum() ? msg.maximum() : 0 ) //$$$$ NLD 2010-10-27 - La SIM envoit tjrs le maximum
{
}

