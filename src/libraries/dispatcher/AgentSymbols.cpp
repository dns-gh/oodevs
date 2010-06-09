// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AgentSymbols.h"
#include "Agent.h"
#include "Automat.h"
#include "clients_kernel/AgentType.h"
#include "tools/App6Symbol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentSymbols constructor
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
AgentSymbols::AgentSymbols( const Agent& holder )
    : holder_( holder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentSymbols destructor
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
AgentSymbols::~AgentSymbols()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentSymbols::BuildSymbol
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
std::string AgentSymbols::BuildSymbol( bool up /* = true */ ) const
{
    std::string symbol( holder_.GetType().GetSymbol() );
    symbol = symbol.substr( symbol.find_last_of( "/" ) + 1 );
    tools::app6::SetCommandPost( symbol, holder_.IsCommandPost() );
    tools::app6::SetLevel( symbol, holder_.GetType().GetLevelSymbol() );
    if( up )
    {
        const EntitySymbols_ABC& symbols = holder_.GetSuperior().Get< EntitySymbols_ABC >();
        return tools::app6::MergeSymbol( symbol, symbols.BuildSymbol() );
    }
    return symbol;
}
