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
#include "clients_kernel/Availability.h"
#include "clients_kernel/EquipmentType.h"

// -----------------------------------------------------------------------------
// Name: MaintenanceRepairersListView constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceRepairersListView::MaintenanceRepairersListView( QWidget* parent, kernel::Controllers& controllers )
    : ResourcesListView_ABC< kernel::MaintenanceStates_ABC >( parent, controllers )
{
    QStringList list;
    list.append( tools::translate( "MaintenanceRepairersListView", "Repairers" ) );
    list.append( tools::translate( "MaintenanceRepairersListView", "Total" ) );
    list.append( tools::translate( "MaintenanceRepairersListView", "Available" ) );
    list.append( tools::translate( "MaintenanceRepairersListView", "Working" ) );
    list.append( tools::translate( "MaintenanceRepairersListView", "Resting" ) );
    model_.setHorizontalHeaderLabels( list );
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
void MaintenanceRepairersListView::NotifyUpdated( const kernel::MaintenanceStates_ABC& a )
{
    if( ShouldUpdate( a ) )
    {
        ResizeModelOnNewContent( static_cast< int >( a.GetDispoRepairers().size() ) );
        for( unsigned int i = 0; i < a.GetDispoHaulers().size(); ++i )
        {
            model_.item( i, 0 )->setText( QString( a.GetDispoRepairers()[ i ].type_->GetName().c_str() ) );
            model_.item( i, 1 )->setText( QString::number( a.GetDispoRepairers()[ i ].total_ ) );
            model_.item( i, 2 )->setText( QString::number( a.GetDispoRepairers()[ i ].available_ ) );
            model_.item( i, 3 )->setText( QString::number( a.GetDispoRepairers()[ i ].atWork_ ) );
            model_.item( i, 4 )->setText( QString::number( a.GetDispoRepairers()[ i ].atRest_ ) );
        }
    }
}
