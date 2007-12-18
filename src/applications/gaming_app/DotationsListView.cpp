// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "DotationsListView.h"
#include "moc_DotationsListView.cpp"

// -----------------------------------------------------------------------------
// Name: DotationsListView constructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
DotationsListView::DotationsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : ResourcesListView_ABC< DotationsListView, Dotations >( parent, *this, controllers, factory )
{
    AddColumn( tr( "Resource" ) )
    .AddColumn( tr( "Quantity" ) );
}

// -----------------------------------------------------------------------------
// Name: DotationsListView destructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
DotationsListView::~DotationsListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DotationsListView::Display
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void DotationsListView::Display( const Dotation& dotation, kernel::Displayer_ABC& displayer, gui::ValuedListItem* )
{
    displayer.Display( tr( "Resource" ), dotation.type_ )
             .Display( tr( "Quantity" ), dotation.quantity_ );
}

// -----------------------------------------------------------------------------
// Name: DotationsListView::NotifyUpdated
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void DotationsListView::NotifyUpdated( const Dotations& a )
{
    if( ShouldUpdate( a ) )
        DeleteTail( DisplayList( a.CreateIterator() ) );
}
