// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "LocalWeathersList.h"
#include "moc_LocalWeathersList.cpp"
#include "preparation/WeatherModel.h"
#include "preparation/LocalWeather.h"

// -----------------------------------------------------------------------------
// Name: LocalWeathersList constructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
LocalWeathersList::LocalWeathersList( QWidget* parent, const kernel::CoordinateConverter_ABC& converter )
    : QListView( parent )
    , converter_( converter )
{
    addColumn( tr( "Local patch" ) );
    setResizeMode( QListView::LastColumn );
    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( ContextMenuRequested( QListViewItem*, const QPoint&, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: LocalWeathersList destructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
LocalWeathersList::~LocalWeathersList()
{
    Clear();
}

// -----------------------------------------------------------------------------
// Name: LocalWeathersList::ContextMenuRequested
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void LocalWeathersList::ContextMenuRequested( QListViewItem* item, const QPoint& point, int )
{
    QPopupMenu* menu = new QPopupMenu( this );
    menu->insertItem( tr( "Add" ), this, SLOT( CreateItem() ) );
    if( item )
        menu->insertItem( tr( "Delete" ), this, SLOT( DeleteItem() ) );
    menu->exec( point );
}

// -----------------------------------------------------------------------------
// Name: LocalWeathersList::CreateItem
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void LocalWeathersList::CreateItem()
{
    LocalWeather* weather = new LocalWeather( converter_ );
    QListViewItem* item = new QListViewItem( this );
    item->setText( 0, weather->GetName() );
    weathers_.push_back( weather );
}

// -----------------------------------------------------------------------------
// Name: LocalWeathersList::DeleteItem
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void LocalWeathersList::DeleteItem()
{
    if( selectedItem() )
    {
        const QString text = selectedItem()->text( 0 );
        for( IT_LocalWeathers it = weathers_.begin(); it != weathers_.end(); ++it )
            if( (*it)->GetName() == text )
            {
                weathers_.erase( it );
                break;
            }
        removeItem( selectedItem() );
    }
}

// -----------------------------------------------------------------------------
// Name: LocalWeathersList::Clear
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void LocalWeathersList::Clear()
{
    for( IT_LocalWeathers it = weathers_.begin(); it != weathers_.end(); ++it )
        delete *it;
    weathers_.clear();
    clear();
}

// -----------------------------------------------------------------------------
// Name: LocalWeathersList::CommitTo
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void LocalWeathersList::CommitTo( WeatherModel& model )
{
    model.DeleteAll();
    for( IT_LocalWeathers it = weathers_.begin(); it != weathers_.end(); ++it )
    {
        LocalWeather* weather = new LocalWeather( **it );
        model.Register( weather->GetId(), *weather );
    }
}

// -----------------------------------------------------------------------------
// Name: LocalWeathersList::Update
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void LocalWeathersList::Update( const WeatherModel& model )
{
    Clear();
    kernel::Iterator< const LocalWeather& > it( model.CreateIterator() );
    while( it.HasMoreElements() )
    {
        LocalWeather* weather = new LocalWeather( it.NextElement() );
        weathers_.push_back( weather );
        QListViewItem* item = new QListViewItem( this );
        item->setText( 0, weather->GetName() );
    }
}

// -----------------------------------------------------------------------------
// Name: LocalWeathersList::SelectedItem
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
LocalWeather* LocalWeathersList::SelectedItem()
{
    if( selectedItem() )
    {
        const QString text = selectedItem()->text( 0 );
        for( IT_LocalWeathers it = weathers_.begin(); it != weathers_.end(); ++it )
            if( (*it)->GetName() == text )
                return *it;
    }
    return 0;
}
