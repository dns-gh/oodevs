// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PreferencesList.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PreferencesList constructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
PreferencesList::PreferencesList( QWidget* parent, QWidgetStack& pages )
    : QListView( parent )
    , pages_( pages )
{
    addColumn( tools::translate( "PreferencesList", "Category" ) );
    setColumnWidthMode( 0, QListView::Maximum );
    header()->hide();
    setRootIsDecorated( true );
    setSorting( -1 );
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
    setFixedSize( 150, 340 );
}

// -----------------------------------------------------------------------------
// Name: PreferencesList destructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
PreferencesList::~PreferencesList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PreferencesList::AddPage
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void PreferencesList::AddPage( const QString& name, QWidget* widget )
{
    QStringList list = QStringList::split( '/', name );
    QListViewItem* parent = 0;
    for( unsigned int i = 0; i < list.count() - 1; ++i )
        parent = findItem( list[i], 0 );
    widgets_[name] = widget;
    pages_.addWidget( widget );
    QListViewItem* item;
    if( parent )
        item = new QListViewItem( parent );
    else
        item = new QListViewItem( this );
    item->setText( 0, list.last() );
}

// -----------------------------------------------------------------------------
// Name: PreferencesList::setCurrentItem
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void PreferencesList::setCurrentItem( QListViewItem* item )
{
    QListView::setCurrentItem( item );
    if( item )
    {
        QString name = item->text( 0 );
        while( item->parent() )
        {
            name = item->parent()->text( 0 ) + "/" + name;
            item = item->parent();
        }
        QWidget* widget = widgets_[name];
        pages_.raiseWidget( widget );
    }
}
