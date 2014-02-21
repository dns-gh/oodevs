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

#include "clients_kernel/Controllers.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/MaintenanceStates_ABC.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: MaintenanceHaulersListView constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceHaulersListView::MaintenanceHaulersListView( QWidget* parent,
                                                        kernel::Controllers& controllers,
                                                        bool listenSelectionChanged /* = true */ )
    : LogisticResourcesListView_ABC< kernel::MaintenanceStates_ABC >( "maintenance_haulers_listview", parent, controllers, listenSelectionChanged )
{
    QStringList list;
    list.append( tools::translate( "MaintenanceHaulersListView", "Tow trucks" ) );
    list.append( tools::translate( "MaintenanceHaulersListView", "Unit" ) );
    list.append( tools::translate( "MaintenanceHaulersListView", "Total" ) );
    list.append( tools::translate( "MaintenanceHaulersListView", "Available" ) );
    list.append( tools::translate( "MaintenanceHaulersListView", "Working" ) );
    list.append( tools::translate( "MaintenanceHaulersListView", "Resting" ) );
    list.append( tools::translate( "MaintenanceHaulersListView", "Lent" ) );
    model_.setHorizontalHeaderLabels( list );
    header()->setResizeMode( 0, QHeaderView::Stretch );
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
// Name: MaintenanceHaulersListView::GetAvailabilities
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
const std::vector< kernel::Availability >* MaintenanceHaulersListView::GetAvailabilities( const kernel::MaintenanceStates_ABC& states ) const
{
    return &states.GetDispoHaulers();
}
