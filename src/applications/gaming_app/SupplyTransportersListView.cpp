// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "SupplyTransportersListView.h"

// -----------------------------------------------------------------------------
// Name: SupplyTransportersListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyTransportersListView::SupplyTransportersListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : SupplyAvailabilitiesListView_ABC( parent, controllers, factory )
{
    AddColumn( tr( "Transporters" ) )
    .AddColumn( tr( "Total" ) )
    .AddColumn( tr( "Available" ) )
    .AddColumn( tr( "Working" ) )
    .AddColumn( tr( "Resting" ) );
}

// -----------------------------------------------------------------------------
// Name: SupplyTransportersListView destructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyTransportersListView::~SupplyTransportersListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SupplyTransportersListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void SupplyTransportersListView::NotifyUpdated( const SupplyStates& a )
{
    if( ShouldUpdate( a ) )
        DeleteTail( DisplayList( a.dispoTransporters_.begin(), a.dispoTransporters_.end() ) );
}
