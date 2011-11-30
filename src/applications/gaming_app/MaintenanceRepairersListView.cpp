// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "MaintenanceRepairersListView.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: MaintenanceRepairersListView constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceRepairersListView::MaintenanceRepairersListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : MaintenanceAvailabilitiesListView_ABC( parent, controllers, factory )
{
    AddColumn( tools::translate( "MaintenanceRepairersListView", "Repairers" ) )
    .AddColumn( tools::translate( "MaintenanceRepairersListView", "Total" ) )
    .AddColumn( tools::translate( "MaintenanceRepairersListView", "Available" ) )
    .AddColumn( tools::translate( "MaintenanceRepairersListView", "Working" ) )
    .AddColumn( tools::translate( "MaintenanceRepairersListView", "Resting" ) );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceRepairersListView destructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceRepairersListView::~MaintenanceRepairersListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaintenanceRepairersListView::NotifyUpdated
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
void MaintenanceRepairersListView::NotifyUpdated( const MaintenanceStates& a )
{
    if( ShouldUpdate( a ) )
        DeleteTail( DisplayList( a.dispoRepairers_.begin(), a.dispoRepairers_.end() ) );
}
