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
// Name: SupplyTransportersListView::NotifyUpdated
// Created: SBO 2007-02-20
// -----------------------------------------------------------------------------
void SupplyTransportersListView::NotifyUpdated( const SupplyStates& a )
{
    if( ShouldUpdate( a ) )
    {
        ResizeModelOnNewContent( static_cast< int >( a.dispoTransporters_.size() ) );
        for( int i = 0; i < a.dispoTransporters_.size(); ++i )
        {
            model_.item( i, 0 )->setText( QString( a.dispoTransporters_[ i ].type_->GetName().c_str() ) );
            model_.item( i, 1 )->setText( QString::number( a.dispoTransporters_[ i ].total_ ) );
            model_.item( i, 2 )->setText( QString::number( a.dispoTransporters_[ i ].available_ ) );
            model_.item( i, 3 )->setText( QString::number( a.dispoTransporters_[ i ].atWork_ ) );
            model_.item( i, 4 )->setText( QString::number( a.dispoTransporters_[ i ].atRest_ ) );
        }
    }
}
