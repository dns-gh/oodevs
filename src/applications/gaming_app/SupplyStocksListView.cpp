// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "SupplyStocksListView.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/EntityHelpers.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include "gaming/Dotation.h"
#include "gaming/LogisticHelpers.h"
#include "gaming/Tools.h"
#include <boost/bind.hpp>
#include <vector>

using namespace kernel;
using namespace EntityHelpers;
using namespace logistic_helpers;

// -----------------------------------------------------------------------------
// Name: SupplyStocksListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyStocksListView::SupplyStocksListView( QWidget* parent, Controllers& controllers, gui::ItemFactory_ABC& factory )
    : SupplyAvailabilitiesListView_ABC( parent, controllers, factory )
{
    AddColumn( tools::translate( "SupplyStocksListView", "Stock" ) )
    .AddColumn( tools::translate( "SupplyStocksListView", "Quantity" ) );
    setColumnWidth( 0, 140 );
}

// -----------------------------------------------------------------------------
// Name: SupplyStocksListView destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyStocksListView::~SupplyStocksListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyStocksListView::NotifyUpdated
// Created: MMC 2012-10-02
// -----------------------------------------------------------------------------
void SupplyStocksListView::NotifyUpdated( const tools::Resolver< Dotation >& dotations )
{
    DeleteTail( DisplayList( dotations.CreateIterator() ) );
}

// -----------------------------------------------------------------------------
// Name: SupplyStocksListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void SupplyStocksListView::NotifyUpdated( const SupplyStates& supplyStates )
{
    if( ShouldUpdate( supplyStates ) )
        DeleteTail( DisplayList( supplyStates.CreateIterator() ) );
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog::NotifySelected
// Created: MMC 2012-10-01
// -----------------------------------------------------------------------------
void SupplyStocksListView::NotifySelected( const Entity_ABC* entity )
{
    if( entity )
    {
        if( entity->Retrieve< SupplyStates >() )
            NotifyUpdated( entity->Get< SupplyStates >() );
        else if( IsLogisticBase( *entity ) )
        {
            dotations_.clear();
            tools::Resolver< Dotation > dotations;
            VisitBaseStocksDotations( *entity, boost::bind( &SupplyStocksListView::TotalizeStocks, this, _1 ) );
            for( std::map< unsigned long, Dotation >::iterator it = dotations_.begin(); it != dotations_.end(); ++it )
                dotations.Register( it->second.type_->GetId(), it->second );
            NotifyUpdated( dotations );
        }
        show();
    }
    else
        hide();
    SupplyAvailabilitiesListView_ABC::UpdateSelected( entity );
}

// -----------------------------------------------------------------------------
// Name: InfoSupplyDialog::TotalizeStocks
// Created: MMC 2012-10-10
// -----------------------------------------------------------------------------
void SupplyStocksListView::TotalizeStocks( const Dotation& dotation )
{
    if( dotation.type_ )
    {
        Dotation& foundDotation = dotations_[ dotation.type_->GetId() ];
        foundDotation.type_ = dotation.type_;
        foundDotation.quantity_ += dotation.quantity_;
    }
}
