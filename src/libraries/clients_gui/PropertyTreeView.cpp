// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PropertyTreeView.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertyTreeView constructor
// Created: LGY 2012-08-09
// -----------------------------------------------------------------------------
PropertyTreeView::PropertyTreeView()
{
    header()->hide();
}

// -----------------------------------------------------------------------------
// Name: PropertyTreeView destructor
// Created: LGY 2012-08-09
// -----------------------------------------------------------------------------
PropertyTreeView::~PropertyTreeView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PropertyTreeView::Display
// Created: LGY 2012-08-20
// -----------------------------------------------------------------------------
void PropertyTreeView::Display()
{
    DisplayHeader( rootIndex() );
    QHeaderView* headerView = header();
    if( headerView->count() > 1 )
    {
        headerView->setResizeMode( 0, QHeaderView::Stretch );
        headerView->setResizeMode( 1, QHeaderView::ResizeToContents );
    }
    sortByColumn( 0, Qt::AscendingOrder );
    expandAll();
}

// -----------------------------------------------------------------------------
// Name: PropertyTreeView::DisplayHeader
// Created: LGY 2012-08-20
// -----------------------------------------------------------------------------
void PropertyTreeView::DisplayHeader( QModelIndex root )
{
    for( int i = 0; i < model()->rowCount( root ); ++i )
    {
        QModelIndex child = model()->index( i, 0, root );
        DisplayHeader( child );
        if( QStandardItem* item = static_cast< QStandardItemModel* >( model() )->itemFromIndex( child ) )
            if( item->hasChildren() )
                setFirstColumnSpanned( i, root, true );
    }
}
