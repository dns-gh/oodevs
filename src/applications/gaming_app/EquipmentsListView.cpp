// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "EquipmentsListView.h"
#include "moc_EquipmentsListView.cpp"
#include "gaming/Equipments.h"
#include "gaming/Equipment.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Equipments_ABC.h"

// -----------------------------------------------------------------------------
// Name: EquipmentsListView constructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
EquipmentsListView::EquipmentsListView( QWidget* parent, kernel::Controllers& controllers )
    : ResourcesListView_ABC< kernel::Equipments_ABC >( parent, controllers )
{
    QStringList list;
    list.append( tr( "Equipment" ) );
    list.append( tr( "Available" ) );
    list.append( tr( "Unavailable" ) );
    list.append( tr( "Repairable" ) );
    list.append( tr( "On site fixable" ) );
    list.append( tr( "In maintenance" ) );
    list.append( tr( "Prisoner" ) );
    model_.setHorizontalHeaderLabels( list );
}

// -----------------------------------------------------------------------------
// Name: EquipmentsListView destructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
EquipmentsListView::~EquipmentsListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EquipmentsListView::NotifyUpdated
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void EquipmentsListView::NotifyUpdated( const kernel::Equipments_ABC& equipments )
{
    if( ShouldUpdate( equipments ) )
    {
        const Equipments& eq = static_cast< const Equipments& >( equipments );
        ResizeModelOnNewContent( eq.Count() );
        int i = 0;
        tools::Iterator< const Equipment& > iterator = eq.CreateIterator();
        while( iterator.HasMoreElements() )
        {
            const Equipment& equipment = iterator.NextElement();
            model_.item( i, 0 )->setText( QString( equipment.type_.GetName().c_str() ) );
            model_.item( i, 1 )->setText( QString::number( equipment.available_ ) );
            model_.item( i, 2 )->setText( QString::number( equipment.unavailable_ ) );
            model_.item( i, 3 )->setText( QString::number( equipment.repairable_ ) );
            model_.item( i, 4 )->setText( QString::number( equipment.onSiteFixable_ ) );
            model_.item( i, 5 )->setText( QString::number( equipment.inMaintenance_ ) );
            model_.item( i, 6 )->setText( QString::number( equipment.prisonners_ ) );
            ++i;
        }
    }
}
