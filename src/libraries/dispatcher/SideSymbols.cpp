// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "SideSymbols.h"
#include "Side.h"
#include "tools/App6Symbol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: SideSymbols constructor
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
SideSymbols::SideSymbols( const Side& holder )
    : holder_( holder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SideSymbols destructor
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
SideSymbols::~SideSymbols()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SideSymbols::BuildSymbol
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
std::string SideSymbols::BuildSymbol( bool /*up = true*/ ) const
{
    std::string symbol;
    tools::app6::SetAffiliation( symbol, (unsigned int)holder_.GetKarma().GetUId() ); // $$$$ SBO 2007-08-23: enumerate value... find something better
    return symbol;
}
