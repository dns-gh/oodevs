// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "FormationSymbols.h"
#include "Automat.h"
#include "Formation.h"
#include "Side.h"
#include "tools/App6Symbol.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: FormationSymbols constructor
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
FormationSymbols::FormationSymbols( const Formation& holder )
    : holder_( holder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FormationSymbols destructor
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
FormationSymbols::~FormationSymbols()
{
    // NOTHING
}

namespace
{
    struct SymbolAggregator
    {
        void operator()( Automat& automat )
        {
            symbol_ = tools::app6::FilterSymbol( automat.Get< EntitySymbols_ABC >().BuildSymbol( false ), symbol_ );
        }
        void operator()( Formation& formation )
        {
            symbol_ = tools::app6::FilterSymbol( formation.Get< EntitySymbols_ABC >().BuildSymbol( false ), symbol_ );
        }
        std::string symbol_;
    };
}

// -----------------------------------------------------------------------------
// Name: FormationSymbols::BuildSymbol
// Created: SBO 2008-02-13
// -----------------------------------------------------------------------------
std::string FormationSymbols::BuildSymbol( bool up /*= true*/ ) const
{
    SymbolAggregator aggregator;
    holder_.automats_.Apply< SymbolAggregator& >( aggregator );
    holder_.subordinates_.Apply< SymbolAggregator& >( aggregator );
    if( up )
    {
        const EntitySymbols_ABC* symbols;
        if( holder_.pParent_ )
            symbols = &holder_.pParent_->Get< EntitySymbols_ABC >();
        else
            symbols = &holder_.side_.Get< EntitySymbols_ABC >();
        aggregator.symbol_ = tools::app6::MergeSymbol( symbols->BuildSymbol(), aggregator.symbol_ );
    }
    tools::app6::SetLevel( aggregator.symbol_, (unsigned int)holder_.nLevel_ );
    return aggregator.symbol_;
}
