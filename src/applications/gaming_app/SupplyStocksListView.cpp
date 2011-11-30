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
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: SupplyStocksListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyStocksListView::SupplyStocksListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : SupplyAvailabilitiesListView_ABC( parent, controllers, factory )
{
    AddColumn( tools::translate( "SupplyStocksListView", "Stock" ) )
    .AddColumn( tools::translate( "SupplyStocksListView", "Quantity" ) );
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
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void SupplyStocksListView::NotifyUpdated( const SupplyStates& a )
{
    if( ShouldUpdate( a ) )
        DeleteTail( DisplayList( a.CreateIterator() ) );
}
