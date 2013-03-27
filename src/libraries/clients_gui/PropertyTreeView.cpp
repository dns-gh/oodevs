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
#include "PropertyModel.h"
#include <boost/foreach.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PropertyTreeView constructor
// Created: LGY 2012-08-09
// -----------------------------------------------------------------------------
PropertyTreeView::PropertyTreeView( const QString& objectName )
    : RichTreeView( objectName )
{
    header()->hide();
    setAnimated( true );
    setEditTriggers( SelectedClicked | DoubleClicked );
    setAlternatingRowColors( true );

    QPalette p = palette();
    p.setColor( QPalette::Inactive, QPalette::Highlight, Qt::lightGray );
    setPalette( p );
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
// Name: PropertyTreeView::SaveState
// Created: LGY 2012-09-03
// -----------------------------------------------------------------------------
void PropertyTreeView::SaveState()
{
    SaveState( rootIndex(), *static_cast< QStandardItemModel* >( model() ) );
}

// -----------------------------------------------------------------------------
// Name: PropertyTreeView::SaveState
// Created: LGY 2012-09-03
// -----------------------------------------------------------------------------
void PropertyTreeView::SaveState( QModelIndex root, const QStandardItemModel& model )
{
    for( int i = 0; i < model.rowCount( root ); ++i )
    {
        QModelIndex child = model.index( i, 0, root );
        if( !isExpanded( child ) )
            itemsCollapsed_.insert( child.data( Qt::UserRole ).toString().toStdString() );
        else
            SaveState( child, model );
    }
}

// -----------------------------------------------------------------------------
// Name: PropertyTreeView::RestoreState
// Created: LGY 2012-09-03
// -----------------------------------------------------------------------------
void PropertyTreeView::RestoreState( const PropertyModel& model )
{
    BOOST_FOREACH( const std::string& category, itemsCollapsed_ )
        if( QStandardItem* item = model.FindItem( category.c_str() ) )
            setExpanded( item->index(), false );
}

// -----------------------------------------------------------------------------
// Name: PropertyTreeView::Display
// Created: LGY 2012-08-20
// -----------------------------------------------------------------------------
void PropertyTreeView::Display()
{
    PropertyModel* propertyModel = static_cast< PropertyModel* >( model() );
    DisplayHeader( rootIndex(), *propertyModel );
    QHeaderView* headerView = header();
    if( headerView->count() > 1 )
    {
        headerView->setResizeMode( 0, QHeaderView::Stretch );
        headerView->setResizeMode( 1, QHeaderView::ResizeToContents );
    }
    sortByColumn( 0, Qt::AscendingOrder );
    expandAll();
    RestoreState( *propertyModel );
    itemsCollapsed_.clear();
}

// -----------------------------------------------------------------------------
// Name: PropertyTreeView::DisplayHeader
// Created: LGY 2012-08-20
// -----------------------------------------------------------------------------
void PropertyTreeView::DisplayHeader( QModelIndex root, const QStandardItemModel& model )
{
    for( int i = 0; i < model.rowCount( root ); ++i )
    {
        QModelIndex child = model.index( i, 0, root );
        DisplayHeader( child, model );
        if( QStandardItem* item = model.itemFromIndex( child ) )
            if( item->hasChildren() )
                setFirstColumnSpanned( i, root, true );
    }
}

// -----------------------------------------------------------------------------
// Name: PropertyTreeView::Exist
// Created: LGY 2012-09-05
// -----------------------------------------------------------------------------
bool PropertyTreeView::Exist( const QString& name ) const
{
    return static_cast< PropertyModel* >( model() )->FindItem( name ) != 0;
}
