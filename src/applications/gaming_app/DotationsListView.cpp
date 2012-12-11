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
#include "clients_kernel/DotationType.h"

// -----------------------------------------------------------------------------
// Name: DotationsListView constructor
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
DotationsListView::DotationsListView( QWidget* parent, kernel::Controllers& controllers )
    : ResourcesListView_ABC< kernel::Dotations_ABC >( parent, controllers )
{
    QStringList list;
    list.append( tr( "Resource" ) );
    list.append( tr( "Quantity" ) );
    model_.setHorizontalHeaderLabels( list );
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
// Name: DotationsListView::NotifyUpdated
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void DotationsListView::NotifyUpdated( const kernel::Dotations_ABC& a )
{
    if( ShouldUpdate( a ) )
    {
        const Dotations* dotations = dynamic_cast< const Dotations* >( &a );
        if( !dotations )
            throw MASA_EXCEPTION( "Unhandled Dotations_ABC" );

        ResizeModelOnNewContent( dotations->Count() );
        int i = 0;
        tools::Iterator< const Dotation& > iterator = dotations->CreateIterator();
        while( iterator.HasMoreElements() )
        {
            const Dotation& dotation = iterator.NextElement();
            model_.item( i, 0 )->setText( QString( dotation.type_->GetName().c_str() ) );
            model_.item( i, 1 )->setText( QString::number( dotation.quantity_ ) );
            ++i;
        }
    }
}
