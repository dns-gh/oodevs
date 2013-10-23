// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "WeatherListView.h"
#include "moc_WeatherListView.cpp"
#include "meteo/Meteo.h"
#include <boost/shared_ptr.hpp>
#include "clients_kernel/ContextMenu.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: WeatherListView constructor
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
WeatherListView::WeatherListView( QWidget* parent, const kernel::CoordinateConverter_ABC& converter )
    : QTreeView( parent )
    , converter_( converter )
    , model_    ( new QStandardItemModel() )
{
    setObjectName( "weather-list-view" );
    setModel( model_ );
    parent->layout()->addWidget( this );
}

// -----------------------------------------------------------------------------
// Name: WeatherListView destructor
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
WeatherListView::~WeatherListView()
{
    Clear();
}

// -----------------------------------------------------------------------------
// Name: WeatherListView::Clear
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
void WeatherListView::Clear()
{
    weathers_.clear();
    model_->clear();
    model_->setHorizontalHeaderLabels( QStringList( tr( "Local weathers" ) ) );
}

// -----------------------------------------------------------------------------
// Name: WeatherListView::contextMenuEvent
// Created: LGY 2012-09-26
// -----------------------------------------------------------------------------
void WeatherListView::contextMenuEvent( QContextMenuEvent* event )
{
    if( event )
    {
        QMenu* menu = new QMenu( this );
        connect( menu, SIGNAL( aboutToHide() ), menu, SLOT( deleteLater() ) );
        menu->addAction( tr( "Add" ), this, SLOT( CreateItem() ) );
        if( indexAt( event->pos() ) == selectionModel()->currentIndex() )
            menu->addAction( tr( "Delete" ), this, SLOT( DeleteItem() ) );
        menu->popup( event->globalPos() );
    }
}

// -----------------------------------------------------------------------------
// Name: WeatherListView::SelectedItem
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
weather::Meteo* WeatherListView::SelectedItem()
{
    if( QStandardItem* item = model_->itemFromIndex( selectionModel()->currentIndex() ) )
    {
        const QString text = item->text();
        for( IT_Weathers it = weathers_.begin(); it != weathers_.end(); ++it )
            if( (*it)->GetName() == text.toStdString() )
                return it->get();
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: WeatherListView::DeleteItem
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
void WeatherListView::DeleteItem()
{
    QModelIndex index = selectionModel()->currentIndex();
    if( QStandardItem* item = model_->itemFromIndex( index ) )
    {
        const QString text = item->text();
        model_->removeRow( index.row() );

        for( IT_Weathers it = weathers_.begin(); it != weathers_.end(); ++it )
            if( (*it)->GetName() == text.toStdString() )
            {
                weathers_.erase( it );
                break;
            }
    }
}

// -----------------------------------------------------------------------------
// Name: WeatherListView::GetLocalWeathers
// Created: ABR 2011-06-07
// -----------------------------------------------------------------------------
const WeatherListView::T_Weathers& WeatherListView::GetLocalWeathers() const
{
    return weathers_;
}
