// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::WeatherListView */

#include "clients_gui_pch.h"
#include "WeatherListView.h"
#include "moc_WeatherListView.cpp"
#include "Meteo/Meteo.h"
#include <boost/shared_ptr.hpp>

using namespace gui;

// -----------------------------------------------------------------------------
// Name: WeatherListView constructor
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
WeatherListView::WeatherListView( QWidget* parent, const kernel::CoordinateConverter_ABC& converter )
    : Q3ListView( parent, "WeatherListView" )
    , converter_( converter )
{
    addColumn( tr( "Local weathers" ) );
    setResizeMode( Q3ListView::AllColumns );
    connect( this, SIGNAL( contextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ), this, SLOT( ContextMenuRequested( Q3ListViewItem*, const QPoint&, int ) ) );
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
    clear();
}

// -----------------------------------------------------------------------------
// Name: WeatherListView::ContextMenuRequested
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
void WeatherListView::ContextMenuRequested( Q3ListViewItem* item, const QPoint& point, int /*column*/ )
{
    Q3PopupMenu* menu = new Q3PopupMenu( this );
    menu->insertItem( tr( "Add" ), this, SLOT( CreateItem() ) );
    if( item )
        menu->insertItem(tr( "Delete" ), this, SLOT( DeleteItem() ) );
    menu->exec( point );
}

// -----------------------------------------------------------------------------
// Name: WeatherListView::SelectedItem
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
weather::Meteo* WeatherListView::SelectedItem()
{
    if( selectedItem() )
    {
        const QString text = selectedItem()->text( 0 );
        for( IT_Weathers it = weathers_.begin(); it != weathers_.end(); ++it )
            if( (*it)->GetName() == text.ascii() )
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
    if( selectedItem() )
    {
        const QString text = selectedItem()->text( 0 );
        for( IT_Weathers it = weathers_.begin(); it != weathers_.end(); ++it )
            if( (*it)->GetName() == text.ascii() )
            {
                weathers_.erase( it );
                break;
            }
            removeItem( selectedItem() );
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
