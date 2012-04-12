// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "StockAttribute_ABC.h"
#include "Displayer_ABC.h"
#include "Tools.h"
#include "DotationType.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: StockAttribute_ABC::SetDotation
// Created: BCI 2011-02-07
// -----------------------------------------------------------------------------
void StockAttribute_ABC::SetDotation( const kernel::DotationType& dotationType, kernel::OptionalValue< unsigned int > stock, kernel::OptionalValue< unsigned int > maxStock )
{
    StockDotation& stockDotation = stockDotations_[ &dotationType ];
    stockDotation.stock_ = stock;
    stockDotation.maxStock_ = maxStock;
}

// -----------------------------------------------------------------------------
// Name: StockAttribute_ABC::Display
// Created: AGE 2006-02-23
// -----------------------------------------------------------------------------
void StockAttribute_ABC::Display( Displayer_ABC& displayer ) const
{
    Displayer_ABC& local =
        displayer.Group( tools::translate( "Stock", "Stock resource attribute" ) );
    for( StockDotations::const_iterator it = stockDotations_.begin(); it != stockDotations_.end(); ++it )
        local.Display( it->first->GetName().c_str(), QString( "%L1 / %L2" ).arg( it->second.stock_ ).arg( it->second.maxStock_ ) );
}

// -----------------------------------------------------------------------------
// Name: StockAttribute_ABC::DisplayInSummary
// Created: BCI 2011-02-07
// -----------------------------------------------------------------------------
void StockAttribute_ABC::DisplayInSummary( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

