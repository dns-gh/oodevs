// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "FormationSymbols.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "Formation.h"
#include "Automat.h"
#include "Side.h"
#include "tools/App6Symbol.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

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
    holder_.GetAutomates().Apply( boost::bind( &SymbolAggregator::AddAutomat, boost::ref( aggregator ), _1 ) );
    holder_.GetFormations().Apply( boost::bind( &SymbolAggregator::AddFormation, boost::ref( aggregator ), _1 ) );
    if( up )
    {
        const EntitySymbols_ABC* symbols;
        kernel::Formation_ABC* parent = holder_.GetParent();
        if( parent )
            symbols = &parent->Get< EntitySymbols_ABC >();
        else
            symbols = &holder_.GetTeam().Get< EntitySymbols_ABC >();
        aggregator.symbol_ = tools::app6::MergeSymbol( symbols->BuildSymbol(), aggregator.symbol_ );
    }
    tools::app6::SetLevel( aggregator.symbol_, static_cast< unsigned int >( holder_.GetLevel() ) );
    return aggregator.symbol_;
}
