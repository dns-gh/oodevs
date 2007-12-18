// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "BorrowingsListView.h"
#include "moc_BorrowingsListView.cpp"

// -----------------------------------------------------------------------------
// Name: BorrowingsListView constructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
BorrowingsListView::BorrowingsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : ResourcesListView_ABC< BorrowingsListView, Borrowings >( parent, *this, controllers, factory )
{
    AddColumn( tr( "Lender" ) )
    .AddColumn( tr( "Equipment" ) )
    .AddColumn( tr( "Quantity" ) );
}

// -----------------------------------------------------------------------------
// Name: BorrowingsListView destructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
BorrowingsListView::~BorrowingsListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: BorrowingsListView::Display
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void BorrowingsListView::Display( const Loan& loan, kernel::Displayer_ABC& displayer, gui::ValuedListItem* )
{
    displayer.Display( QString(), loan.agent_ )
             .Display( tr( "Equipment" ), loan.type_ )
             .Display( tr( "Quantity" ), loan.quantity_ );
}

// -----------------------------------------------------------------------------
// Name: BorrowingsListView::NotifyUpdated
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void BorrowingsListView::NotifyUpdated( const Borrowings& a )
{
    if( ShouldUpdate( a ) )
        DeleteTail( DisplayList( a.borrowings_.begin(), a.borrowings_.end() ) );
}
