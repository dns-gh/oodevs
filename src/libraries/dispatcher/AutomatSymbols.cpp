// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "AutomatSymbols.h"
#include "Automat.h"
#include "Formation.h"
#include "tools/App6Symbol.h"
#include "dispatcher/Agent.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: AutomatSymbols constructor
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
AutomatSymbols::AutomatSymbols( const Automat& holder )
    : holder_( holder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatSymbols destructor
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
AutomatSymbols::~AutomatSymbols()
{
    // NOTHING
}

namespace
{
    struct SymbolAggregator
    {
        SymbolAggregator() : level_( 0 ) {}
        void AddAgent( const kernel::Agent_ABC& agent )
        {
            const std::string agentSymbol = agent.Get< EntitySymbols_ABC >().BuildSymbol( false );
            level_ = std::max( level_, tools::app6::GetLevel( agentSymbol ) );
            symbol_ = tools::app6::FilterSymbol( agentSymbol, symbol_ );
        }
        std::string symbol_;
        unsigned int level_;
    };
}

// -----------------------------------------------------------------------------
// Name: AutomatSymbols::BuildSymbol
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
std::string AutomatSymbols::BuildSymbol( bool up /*= true*/ ) const
{
    SymbolAggregator aggregator;
    holder_.agents_.Apply( boost::bind( &SymbolAggregator::AddAgent, boost::ref( aggregator ), _1 ) );
    if( up )
    {
        const EntitySymbols_ABC* symbols;
        if( holder_.parentFormation_ )
            symbols = &holder_.parentFormation_->Get< EntitySymbols_ABC >();
        else
            symbols = &holder_.parentAutomat_->Get< EntitySymbols_ABC >();
        aggregator.symbol_ = tools::app6::MergeSymbol( symbols->BuildSymbol(), aggregator.symbol_ );
    }
    tools::app6::SetLevel( aggregator.symbol_, aggregator.level_ + 1 );
    return aggregator.symbol_;
}
