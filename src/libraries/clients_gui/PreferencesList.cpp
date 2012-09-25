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
#include "moc_PreferencesList.cpp"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PreferencesList constructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
PreferencesList::PreferencesList( QWidget* parent, QStackedWidget& pages )
    : QTreeView( parent )
    , pages_( pages )
    , model_( new QStandardItemModel() )
{
    header()->hide();
    setModel( model_ );
    connect( selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnSelect( const QItemSelection&, const QItemSelection& ) ) );
}

// -----------------------------------------------------------------------------
// Name: PreferencesList destructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
PreferencesList::~PreferencesList()
{
    // NOTHING
}

namespace
{
    QStandardItem* FindItem( QStandardItem* item, const QString& text ) // $$$$ LGY 2012-09-25: ^c^v property model
    {
        for( int row = 0; row < item->rowCount(); ++row )
        {
            QStandardItem* childItem = item->child( row, 0 );
            if( childItem && childItem->text() == text )
                return childItem;
        }
        return 0;
    }
    QStandardItem* FindItem( const QString& category, QStandardItem* root )
    {
        QStandardItem* parent = root;
        QStringList path = QStringList::split( '/', category );
        for( int i = 0; i < path.size(); ++i )
        {
            const QString name = path.at( i );
            QStandardItem* item = FindItem( parent, name );
            if( !item )
                return 0;
            if( i == path.size() - 1 )
                return item;
            parent = item;
        }
        return 0 ;
    }
    QStandardItem* CreateItem( QStandardItem& parent, const QString& name, const QString& path )
    {
        QStandardItem* category = new QStandardItem( name );
        category->setEditable( false );
        QVariant* variant = new QVariant();
        variant->setValue( path );
        category->setData( *variant, Qt::UserRole );
        parent.appendRow( category );
        return category;
    }
}

// -----------------------------------------------------------------------------
// Name: PreferencesList::AddPage
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
void PreferencesList::AddPage( const QString& name, QWidget* widget )
{
    QStandardItem* parent = model_->invisibleRootItem();
    QStringList path = QStringList::split( '/', name );
    for( int i = 0; i < path.size(); ++i )
    {
        const QString category = path.at( i );
        QStandardItem* item = FindItem( parent, category );
        parent = item ? item : CreateItem( *parent, category, name );
    }
    widgets_[ name ] = widget;
    pages_.addWidget( widget );
    if( QStandardItem* child = model_->invisibleRootItem()->child( 0 ) )
    {
        QItemSelectionModel* selectionModel = QTreeView::selectionModel();
        selectionModel->clear();
        selectionModel->select( child->index(), QItemSelectionModel::Select );
    }
    resizeColumnToContents( 0 );
    sortByColumn( 0, Qt::AscendingOrder );
}

// -----------------------------------------------------------------------------
// Name: PreferencesList::OnSelect
// Created: LGY 2012-09-25
// -----------------------------------------------------------------------------
void PreferencesList::OnSelect( const QItemSelection& /*selected*/, const QItemSelection& /*deselected*/ )
{
    QModelIndexList indexes = selectedIndexes();
    for( QModelIndexList::const_iterator it = indexes.constBegin(); it != indexes.constEnd(); ++it )
    {
        CIT_Widgets widget = widgets_.find( (*it).data( Qt::UserRole ).toString() );
        if( widget != widgets_.end() )
            pages_.setCurrentWidget( widget->second );
    }
}

