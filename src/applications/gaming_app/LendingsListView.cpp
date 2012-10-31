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
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Agent_ABC.h"

// -----------------------------------------------------------------------------
// Name: LendingsListView constructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
LendingsListView::LendingsListView( QWidget* parent, kernel::Controllers& controllers )
    : ResourcesListView_ABC< Lendings >( parent, controllers )
{
    QStringList list;
    list.append( tr( "Borrower" ) );
    list.append( tr( "Equipment" ) );
    list.append( tr( "Quantity" ) );
    model_.setHorizontalHeaderLabels( list );
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
// Name: LendingsListView::NotifyUpdated
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void LendingsListView::NotifyUpdated( const Lendings& a )
{
    if( ShouldUpdate( a ) )
    {
        ResizeModelOnNewContent( static_cast< int >( a.lendings_.size() ) );
        for( int i = 0; i < a.lendings_.size(); ++i )
        {
            model_.item( i, 0 )->setText( QString( a.lendings_[ i ].type_->GetName().c_str() ) );
            model_.item( i, 1 )->setText( QString( a.lendings_[ i ].agent_->GetName() ) );
            model_.item( i, 2 )->setText( QString::number( a.lendings_[ i ].quantity_ ) );
        }
    }
}
