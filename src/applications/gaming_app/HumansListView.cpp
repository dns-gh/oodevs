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
HumansListView::HumansListView( QWidget* parent, kernel::Controllers& controllers )
    : ResourcesListView_ABC< TroopsCompatibilityVersion >( parent, controllers )
{
    QStringList list;
    list.append( tr( "Rank" ) );
    list.append( tr( "Officer" ) );
    list.append( tr( "Warrant-officer" ) );
    list.append( tr( "Private" ) );
    model_.setHorizontalHeaderLabels( list );
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
// Name: HumansListView::NotifyUpdated
// Created: SBO 2007-02-16
// -----------------------------------------------------------------------------
void HumansListView::NotifyUpdated( const TroopsCompatibilityVersion& a )
{
    if( ShouldUpdate( a ) )
    {
        int count = static_cast< int >( kernel::eTroopHealthStateNbrStates );
        ResizeModelOnNewContent( count );
        for( int i = 0; i < count; ++i )
        {
            model_.item( i, 0 )->setText( QString::number( a.humans_[ i ].state_ ) );
            model_.item( i, 1 )->setText( QString::number( a.humans_[ i ].officers_ ) );
            model_.item( i, 2 )->setText( QString::number( a.humans_[ i ].subOfficers_ ) );
            model_.item( i, 3 )->setText( QString::number( a.humans_[ i ].troopers_ ) );
        }
    }
}