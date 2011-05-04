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
float LogSupplyStocks::Extract( const sword::SimToClient& wrapper ) const
{
    const sword::LogSupplyState& logstate = wrapper.message().log_supply_state();
    float result = 0;

    if(logstate.has_stocks())
    {
        const sword::SeqOfDotationStock& stocks = logstate.stocks();
        for( int i = 0; i < stocks.elem_size(); ++i )
        {
            const sword::DotationStock& stock = stocks.elem( i );
            if( filter_.IsAllowed( stock.resource().id() ) )
                result += stocks.elem( i ).quantity();
        }
    }
    return result;
}
