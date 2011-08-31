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

// -----------------------------------------------------------------------------
// Name: EquipmentsListView constructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
EquipmentsListView::EquipmentsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : ResourcesListView_ABC< EquipmentsListView, Equipments >( parent, *this, controllers, factory )
{
    AddColumn( tr( "Equipment" ) )
    .AddColumn( tr( "Available" ) )
    .AddColumn( tr( "Unavailable" ) )
    .AddColumn( tr( "Repairable" ) )
    .AddColumn( tr( "On site fixable" ) )
    .AddColumn( tr( "In maintenance" ) )
    .AddColumn( tr( "Prisoner" ) );
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
// Name: EquipmentsListView::Display
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void EquipmentsListView::Display( const Equipment& equipment, kernel::Displayer_ABC& displayer, gui::ValuedListItem* )
{
    displayer.Display( tr( "Equipment" ), equipment.type_  )
             .Display( tr( "Available" ), equipment.available_ )
             .Display( tr( "Unavailable" ), equipment.unavailable_ )
             .Display( tr( "Repairable" ), equipment.repairable_ )
             .Display( tr( "On site fixable" ), equipment.onSiteFixable_ )
             .Display( tr( "In maintenance" ), equipment.inMaintenance_ )
             .Display( tr( "Prisoner" ), equipment.prisonners_ );
}

// -----------------------------------------------------------------------------
// Name: EquipmentsListView::NotifyUpdated
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void EquipmentsListView::NotifyUpdated( const Equipments& a )
{
    if( ShouldUpdate( a ) )
        DeleteTail( DisplayList( a.CreateIterator() ) );
}