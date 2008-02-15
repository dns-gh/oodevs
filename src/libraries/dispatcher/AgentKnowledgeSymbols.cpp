// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AgentKnowledgeSymbols.h"
#include "Agent.h"
#include "AgentKnowledge.h"
#include "tools/App6Symbol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeSymbols constructor
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
AgentKnowledgeSymbols::AgentKnowledgeSymbols( const AgentKnowledge& holder )
    : holder_( holder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeSymbols destructor
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
AgentKnowledgeSymbols::~AgentKnowledgeSymbols()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeSymbols::BuildSymbol
// Created: SBO 2008-02-14
// -----------------------------------------------------------------------------
std::string AgentKnowledgeSymbols::BuildSymbol( bool /*up = true*/ ) const
{
    std::string symbol = holder_.agent_.Get< EntitySymbols_ABC >().BuildSymbol( true );
    tools::app6::FilterPerceptionLevel( symbol, (unsigned int)holder_.nMaxPerceptionLevel_ );
    return symbol;
}
