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
#include <boost/bind.hpp>

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

// -----------------------------------------------------------------------------
// Name: MaintenanceHaulersListView::NotifyUpdated
// Created: SBO 2007-02-19
// -----------------------------------------------------------------------------
void MaintenanceHaulersListView::NotifyUpdated( const kernel::MaintenanceStates_ABC& a )
{
    if( !isVisible() || !selected_ )
        return;
    if( !HasRetrieveForLogistic( *selected_, a ) )
        return;
    DisplaySelectionAvailabilities();
}

// -----------------------------------------------------------------------------
// Name: MaintenanceHaulersListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void MaintenanceHaulersListView::NotifySelected( const kernel::Entity_ABC* entity )
{
    UpdateSelected( entity );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceHaulersListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void MaintenanceHaulersListView::UpdateSelected( const kernel::Entity_ABC* entity )
{
    selected_ = entity;
    if( !entity )
        return;
    if( !HasRetrieveForLogistic( *selected_ ) )
    {
        hide();
        return;
    }
    DisplaySelectionAvailabilities();
    show();
}
