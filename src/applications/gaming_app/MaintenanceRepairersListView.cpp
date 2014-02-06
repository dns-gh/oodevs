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

#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/MaintenanceStates_ABC.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: MaintenanceRepairersListView constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceRepairersListView::MaintenanceRepairersListView( QWidget* parent,
                                                            kernel::Controllers& controllers,
                                                            bool listenSelectionChanged /* = true */ )
    : LogisticResourcesListView_ABC< kernel::MaintenanceStates_ABC >( "maintenance_repairers_listview", parent, controllers, listenSelectionChanged )
{
    QStringList list;
    list.append( tools::translate( "MaintenanceRepairersListView", "Repairers" ) );
    list.append( tools::translate( "MaintenanceRepairersListView", "Total" ) );
    list.append( tools::translate( "MaintenanceRepairersListView", "Available" ) );
    list.append( tools::translate( "MaintenanceRepairersListView", "Working" ) );
    list.append( tools::translate( "MaintenanceRepairersListView", "Resting" ) );
    list.append( tools::translate( "MaintenanceRepairersListView", "Lent" ) );
    model_.setHorizontalHeaderLabels( list );
    header()->setResizeMode( 0, QHeaderView::Stretch );
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
// Name: MaintenanceRepairersListView::GetAvailabilities
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
const std::vector< kernel::Availability >* MaintenanceRepairersListView::GetAvailabilities( const kernel::MaintenanceStates_ABC& states ) const
{
    return &states.GetDispoRepairers();
}
