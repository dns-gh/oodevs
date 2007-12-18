// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LendingsListView.h"
#include "moc_LendingsListView.cpp"

// -----------------------------------------------------------------------------
// Name: LendingsListView constructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
LendingsListView::LendingsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : ResourcesListView_ABC< LendingsListView, Lendings >( parent, *this, controllers, factory )
{
    AddColumn( tr( "Borrower" ) )
    .AddColumn( tr( "Equipment" ) )
    .AddColumn( tr( "Quantity" ) );
}

// -----------------------------------------------------------------------------
// Name: LendingsListView destructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
LendingsListView::~LendingsListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LendingsListView::Display
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void LendingsListView::Display( const Loan& loan, kernel::Displayer_ABC& displayer, gui::ValuedListItem* )
{
    displayer.Display( QString(), loan.agent_ )
             .Display( tr( "Equipment" ), loan.type_ )
             .Display( tr( "Quantity" ), loan.quantity_ );
}

// -----------------------------------------------------------------------------
// Name: LendingsListView::NotifyUpdated
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void LendingsListView::NotifyUpdated( const Lendings& a )
{
    if( ShouldUpdate( a ) )
        DeleteTail( DisplayList( a.lendings_.begin(), a.lendings_.end() ) );
}
