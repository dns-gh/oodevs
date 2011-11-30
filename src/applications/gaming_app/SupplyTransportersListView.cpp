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
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: SupplyTransportersListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyTransportersListView::SupplyTransportersListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : SupplyAvailabilitiesListView_ABC( parent, controllers, factory )
{
    AddColumn( tools::translate( "SupplyTransportersListView", "Transporters" ) )
    .AddColumn( tools::translate( "SupplyTransportersListView", "Total" ) )
    .AddColumn( tools::translate( "SupplyTransportersListView", "Available" ) )
    .AddColumn( tools::translate( "SupplyTransportersListView", "Working" ) )
    .AddColumn( tools::translate( "SupplyTransportersListView", "Resting" ) );
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
