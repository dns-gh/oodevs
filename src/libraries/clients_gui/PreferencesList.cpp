// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

/* TRANSLATOR gui::PreferencesList */

#include "clients_gui_pch.h"
#include "PreferencesList.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PreferencesList constructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
PreferencesList::PreferencesList( QWidget* parent, QStackedWidget& pages )
    : Q3ListView( parent )
    , pages_( pages )
{
    addColumn( tools::translate( "gui::PreferencesList", "Category" ) );
    setColumnWidthMode( 0, Q3ListView::Maximum );
    header()->hide();
    setRootIsDecorated( true );
    setSorting( -1 );
    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Preferred );
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
    Q3ListViewItem* parent = 0;
    for( int i = 0; i < list.count() - 1; ++i )
        parent = findItem( list[i], 0 );
    widgets_[name] = widget;
    pages_.addWidget( widget );
    Q3ListViewItem* item;
    if( parent )
        item = new Q3ListViewItem( parent );
    else
        item = new Q3ListViewItem( this );
    item->setText( 0, list.last() );
}

// -----------------------------------------------------------------------------
// Name: PreferencesList::setCurrentItem
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void PreferencesList::setCurrentItem( Q3ListViewItem* item )
{
    Q3ListView::setCurrentItem( item );
    if( item )
    {
        QString name = item->text( 0 );
        while( item->parent() )
        {
            name = item->parent()->text( 0 ) + "/" + name;
            item = item->parent();
        }
        QWidget* widget = widgets_[name];
        pages_.setCurrentWidget( widget );
    }
}
