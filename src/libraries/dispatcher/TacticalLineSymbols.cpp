// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "TacticalLineSymbols.h"
#include "Limit.h"
#include "Lima.h"
#include "Automat.h"
#include "Formation.h"
#include "tools/App6Symbol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: TacticalLineSymbols constructor
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
TacticalLineSymbols::TacticalLineSymbols( const Limit& holder )
    : holder_( holder )
    , function_( "**B***" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLineSymbols constructor
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
TacticalLineSymbols::TacticalLineSymbols( const Lima& holder )
    : holder_( holder )
    , function_( "**P***" )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLineSymbols destructor
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
TacticalLineSymbols::~TacticalLineSymbols()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TacticalLineSymbols::BuildSymbol
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
std::string TacticalLineSymbols::BuildSymbol( bool up /*= true*/ ) const
{
    std::string symbol( "G*GPGL-----*--X" ); // $$$$ SBO 2007-08-23: could be built using tool::app6 operations or symbol.xml
    tools::app6::SetFunction( symbol, function_ );
    if( up )
    {
        const EntitySymbols_ABC* symbols;
        if( holder_.automat_ )
            symbols = &holder_.automat_->Get< EntitySymbols_ABC >();
        else
            symbols = &holder_.formation_->Get< EntitySymbols_ABC >();
        return tools::app6::MergeSymbol( symbol, symbols->BuildSymbol() );
    }
    return symbol;
}
