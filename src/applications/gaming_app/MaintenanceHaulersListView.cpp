// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MaintenanceHaulersListView.h"

// -----------------------------------------------------------------------------
// Name: MaintenanceHaulersListView constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceHaulersListView::MaintenanceHaulersListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : MaintenanceAvailabilitiesListView_ABC( parent, controllers, factory )
{
    AddColumn( tr( "Tow trucks" ) )
    .AddColumn( tr( "Total" ) )
    .AddColumn( tr( "Available" ) )
    .AddColumn( tr( "Working" ) )
    .AddColumn( tr( "Resting" ) );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceHaulersListView destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceHaulersListView::~MaintenanceHaulersListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaintenanceHaulersListView::NotifyUpdated
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
void MaintenanceHaulersListView::NotifyUpdated( const MaintenanceStates& a )
{
    if( ShouldUpdate( a ) )
        DeleteTail( DisplayList( a.dispoHaulers_.begin(), a.dispoHaulers_.end() ) );
}
