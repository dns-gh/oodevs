// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "HumansListView.h"
#include "moc_HumansListView.cpp"
#include "clients_kernel/Types.h"

// -----------------------------------------------------------------------------
// Name: HumansListView constructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
HumansListView::HumansListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : ResourcesListView_ABC< HumansListView, TroopsCompatibilityVersion >( parent, *this, controllers, factory )
{
    AddColumn( tr( "Rank" ) )
    .AddColumn( tr( "Officer" ) )
    .AddColumn( tr( "Warrant-officer" ) )
    .AddColumn( tr( "Private" ) );
}

// -----------------------------------------------------------------------------
// Name: HumansListView destructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
HumansListView::~HumansListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: HumansListView::Display
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void HumansListView::Display( const Humans& humans, kernel::Displayer_ABC& displayer, gui::ValuedListItem* )
{
    displayer.Display( tr( "Rank" ), humans.state_ )
             .Display( tr( "Officer" ), humans.officers_ )
             .Display( tr( "Warrant-officer" ), humans.subOfficers_ )
             .Display( tr( "Private" ), humans.troopers_ );
}

// -----------------------------------------------------------------------------
// Name: HumansListView::NotifyUpdated
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void HumansListView::NotifyUpdated( const TroopsCompatibilityVersion& a )
{
    if( ShouldUpdate( a ) )
        DeleteTail( DisplayList( &*a.humans_, a.humans_ + kernel::eTroopHealthStateNbrStates ) );
}
