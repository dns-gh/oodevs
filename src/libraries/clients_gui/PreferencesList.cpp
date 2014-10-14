// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "PreferencesList.h"
#include "moc_PreferencesList.cpp"
#include "KeyModel.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: PreferencesList constructor
// Created: SBO 2007-01-03
// -----------------------------------------------------------------------------
PreferencesList::PreferencesList( const QString& objectName, QStackedWidget& pages, QWidget* parent /* = 0 */ )
    : RichTreeView( objectName, parent )
    , pages_( pages )
    , model_( new KeyModel() )
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
        QStandardItem* item = model_->FindItem( parent, category );
        parent = item ? item : model_->CreateItem( *parent, category, name );
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
    for( auto it = indexes.constBegin(); it != indexes.constEnd(); ++it )
    {
        auto widget = widgets_.find( (*it).data( Qt::UserRole ).toString() );
        if( widget != widgets_.end() )
            pages_.setCurrentWidget( widget->second );
    }
}
