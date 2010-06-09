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
    : DotationsListViewBase( parent, *this, controllers, factory )
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
void DotationsListView::NotifyUpdated( const kernel::Dotations_ABC& a )
{
    if( ShouldUpdate( a ) )
    {
        const Dotations* dotations = dynamic_cast< const Dotations* >( &a );
        if( !dotations )
            throw std::runtime_error( __FUNCTION__ ": Unhandled Dotations_ABC" );
        DeleteTail( DisplayList( dotations->CreateIterator() ) );
    }
}
