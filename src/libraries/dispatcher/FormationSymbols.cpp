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
#include "Formation.h"
#include "tools/App6Symbol.h"
#include "dispatcher/Automat.h"
#include "dispatcher/Side.h"
#include <boost/bind.hpp>

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
        void AddAutomat( const kernel::Automat_ABC& automat )
        {
            symbol_ = tools::app6::FilterSymbol( automat.Get< EntitySymbols_ABC >().BuildSymbol( false ), symbol_ );
        }
        void AddFormation( const kernel::Formation_ABC& formation )
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
    holder_.automats_.Apply( boost::bind( &SymbolAggregator::AddAutomat, boost::ref( aggregator ), _1 ) );
    holder_.formations_.Apply( boost::bind( &SymbolAggregator::AddFormation, boost::ref( aggregator ), _1 ) );//@TODO URGENT MGD RESTORE
    if( up )
    {
        const EntitySymbols_ABC* symbols;
        if( holder_.parent_ )
            symbols = &holder_.parent_->Get< EntitySymbols_ABC >();
        else
            symbols = &holder_.team_.Get< EntitySymbols_ABC >();
        aggregator.symbol_ = tools::app6::MergeSymbol( symbols->BuildSymbol(), aggregator.symbol_ );
    }
    tools::app6::SetLevel( aggregator.symbol_, (unsigned int)holder_.level_ );
    return aggregator.symbol_;
}
