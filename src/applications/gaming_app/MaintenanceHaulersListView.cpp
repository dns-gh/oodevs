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
#include "clients_kernel/Tools.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Availability.h"

// -----------------------------------------------------------------------------
// Name: MaintenanceHaulersListView constructor
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
MaintenanceHaulersListView::MaintenanceHaulersListView( QWidget* parent, kernel::Controllers& controllers )
    : ResourcesListView_ABC< kernel::MaintenanceStates_ABC >( parent, controllers )
{
    QStringList list;
    list.append( tools::translate( "MaintenanceHaulersListView", "Tow trucks" ) );
    list.append( tools::translate( "MaintenanceHaulersListView", "Total" ) );
    list.append( tools::translate( "MaintenanceHaulersListView", "Available" ) );
    list.append( tools::translate( "MaintenanceHaulersListView", "Working" ) );
    list.append( tools::translate( "MaintenanceHaulersListView", "Resting" ) );
    model_.setHorizontalHeaderLabels( list );
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
void MaintenanceHaulersListView::NotifyUpdated( const kernel::MaintenanceStates_ABC& a )
{
    if( ShouldUpdate( a ) )
    {
        ResizeModelOnNewContent( static_cast< int >( a.GetDispoHaulers().size() ) );
        for( int i = 0; i < a.GetDispoHaulers().size(); ++i )
        {
            model_.item( i, 0 )->setText( QString( a.GetDispoHaulers()[ i ].type_->GetName().c_str() ) );
            model_.item( i, 1 )->setText( QString::number( a.GetDispoHaulers()[ i ].total_ ) );
            model_.item( i, 2 )->setText( QString::number( a.GetDispoHaulers()[ i ].available_ ) );
            model_.item( i, 3 )->setText( QString::number( a.GetDispoHaulers()[ i ].atWork_ ) );
            model_.item( i, 4 )->setText( QString::number( a.GetDispoHaulers()[ i ].atRest_ ) );
        }
    }
}

