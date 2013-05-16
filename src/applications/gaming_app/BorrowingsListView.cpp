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
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Agent_ABC.h"

// -----------------------------------------------------------------------------
// Name: BorrowingsListView constructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
BorrowingsListView::BorrowingsListView( QWidget* parent, kernel::Controllers& controllers )
    : ResourcesListView_ABC< Borrowings >( parent, controllers )
{
    QStringList list;
    list.append( tr( "Lender" ) );
    list.append( tr( "Equipment" ) );
    list.append( tr( "Quantity" ) );
    model_.setHorizontalHeaderLabels( list );
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
// Name: BorrowingsListView::NotifyUpdated
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void BorrowingsListView::NotifyUpdated( const Borrowings& a )
{
    if( ShouldUpdate( a ) )
    {
        ResizeModelOnNewContent( static_cast< int >( a.borrowings_.size() ) );
        for( unsigned int i = 0; i < a.borrowings_.size(); ++i )
        {
            model_.item( i, 0 )->setText( QString( a.borrowings_[ i ].agent_->GetName() ) );
            model_.item( i, 1 )->setText( QString( a.borrowings_[ i ].type_->GetName().c_str() ) );
            model_.item( i, 2 )->setText( QString::number( a.borrowings_[ i ].quantity_ ) );
        }
    }
}
