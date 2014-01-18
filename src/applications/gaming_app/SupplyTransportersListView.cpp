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
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Availability.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: SupplyTransportersListView constructor
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
SupplyTransportersListView::SupplyTransportersListView( QWidget* parent, kernel::Controllers& controllers )
    : ResourcesListView_ABC< SupplyStates >( parent, controllers )
{
    QStringList list;
    list.append( tools::translate( "SupplyTransportersListView", "Transporters" ) );
    list.append( tools::translate( "SupplyTransportersListView", "Total" ) );
    list.append( tools::translate( "SupplyTransportersListView", "Available" ) );
    list.append( tools::translate( "SupplyTransportersListView", "Working" ) );
    list.append( tools::translate( "SupplyTransportersListView", "Resting" ) );
    list.append( tools::translate( "SupplyTransportersListView", "Lent" ) );
    model_.setHorizontalHeaderLabels( list );
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
// Name: SupplyTransportersListView::GetAvailabilities
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
const std::vector< kernel::Availability >* SupplyTransportersListView::GetAvailabilities( const SupplyStates& states ) const
{
    return &states.dispoTransporters_;
}

// -----------------------------------------------------------------------------
// Name: SupplyTransportersListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void SupplyTransportersListView::NotifyUpdated( const SupplyStates& a )
{
    if( !isVisible() || !selected_ )
        return;
    if( !HasRetrieveForLogistic( *selected_, a ) )
        return;
    DisplaySelectionAvailabilities();
}

// -----------------------------------------------------------------------------
// Name: SupplyTransportersListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void SupplyTransportersListView::NotifySelected( const kernel::Entity_ABC* entity )
{
    UpdateSelected( entity );
}

// -----------------------------------------------------------------------------
// Name: SupplyTransportersListView::NotifyUpdated
// Created: MMC 2013-01-28
// -----------------------------------------------------------------------------
void SupplyTransportersListView::UpdateSelected( const kernel::Entity_ABC* entity )
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
