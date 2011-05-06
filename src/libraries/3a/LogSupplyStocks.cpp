// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "LogSupplyStocks.h"

using namespace extractors;

// -----------------------------------------------------------------------------
// Name: LogSupplyStocks constructor
// Created: FPO 2011-05-04
// -----------------------------------------------------------------------------
LogSupplyStocks::LogSupplyStocks()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogSupplyStocks constructor
// Created: FPO 2011-05-04
// -----------------------------------------------------------------------------
LogSupplyStocks::LogSupplyStocks( xml::xistream& xis )
    : filter_   ( xis, "types" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogSupplyStocks destructor
// Created: FPO 2011-05-04
// -----------------------------------------------------------------------------
LogSupplyStocks::~LogSupplyStocks()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogSupplyStocks::Extract
// Created: FPO 2011-05-04
// -----------------------------------------------------------------------------
float LogSupplyStocks::Extract( const sword::SimToClient& wrapper )
{
    const sword::LogSupplyState& logstate = wrapper.message().log_supply_state();
    float result = 0;

    if(logstate.has_stocks())
    {
        const sword::SeqOfDotationStock& stocks = logstate.stocks();
        for( int i = 0; i < stocks.elem_size(); ++i )
        {
            const sword::DotationStock& stock = stocks.elem( i );
            const int resourceId = stock.resource().id();
            const int quantity = stock.quantity();
            if( filter_.IsAllowed( resourceId ) )
                logSupplyStocks_[ resourceId ] = quantity;
        }
    }

    for( std::map< int, int >::const_iterator it = logSupplyStocks_.begin(); it != logSupplyStocks_.end(); ++it )
        result += it->second;
    return result;
}
