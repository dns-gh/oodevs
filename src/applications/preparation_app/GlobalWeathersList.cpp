// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "GlobalWeathersList.h"
#include "moc_GlobalWeathersList.cpp"
#include "preparation/WeatherModel.h"
#include "preparation/Weather.h"

// -----------------------------------------------------------------------------
// Name: GlobalWeathersList constructor
// Created: SLG 2010-03-17
// -----------------------------------------------------------------------------
GlobalWeathersList::GlobalWeathersList( QWidget* parent, const kernel::CoordinateConverter_ABC& converter )
: QListView( parent, "LocalWeathersList" )
, converter_( converter )
{
    addColumn( tr( "Local patch" ) );
    setResizeMode( QListView::LastColumn );
    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( ContextMenuRequested( QListViewItem*, const QPoint&, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: GlobalWeathersList destructor
// Created: SLG 2010-03-17
// -----------------------------------------------------------------------------
GlobalWeathersList::~GlobalWeathersList()
{
    Clear();
}

// -----------------------------------------------------------------------------
// Name: GlobalWeathersList::ContextMenuRequested
// Created: SLG 2010-03-17
// -----------------------------------------------------------------------------
void GlobalWeathersList::ContextMenuRequested( QListViewItem* item, const QPoint& point, int )
{
    QPopupMenu* menu = new QPopupMenu( this );
    menu->insertItem( tr( "Add" ), this, SLOT( CreateItem() ) );
    if( item )
        menu->insertItem( tr( "Delete" ), this, SLOT( DeleteItem() ) );
    menu->exec( point );
}

// -----------------------------------------------------------------------------
// Name: GlobalWeathersList::CreateItem
// Created: SLG 2010-03-17
// -----------------------------------------------------------------------------
void GlobalWeathersList::CreateItem()
{
    Weather* weather = new Weather( /*converter_*/ );
    QListViewItem* item = new QListViewItem( this );
    item->setText( 0, weather->GetName() );
    weathers_.push_back( weather );
}

// -----------------------------------------------------------------------------
// Name: GlobalWeathersList::DeleteItem
// Created: SLG 2010-03-17
// -----------------------------------------------------------------------------
void GlobalWeathersList::DeleteItem()
{
    if( selectedItem() )
    {
        const QString text = selectedItem()->text( 0 );
        for( IT_Weathers it = weathers_.begin(); it != weathers_.end(); ++it )
            if( (*it)->GetName() == text )
            {
                weathers_.erase( it );
                break;
            }
            removeItem( selectedItem() );
    }
}

// -----------------------------------------------------------------------------
// Name: GlobalWeathersList::Clear
// Created: SLG 2010-03-17
// -----------------------------------------------------------------------------
void GlobalWeathersList::Clear()
{
    for( IT_Weathers it = weathers_.begin(); it != weathers_.end(); ++it )
        delete *it;
    weathers_.clear();
    clear();
}

// -----------------------------------------------------------------------------
// Name: GlobalWeathersList::CommitTo
// Created: SLG 2010-03-17
// -----------------------------------------------------------------------------
void GlobalWeathersList::CommitTo( WeatherModel& model )
{
    model.tools::Resolver< Weather >::DeleteAll();
    for( IT_Weathers it = weathers_.begin(); it != weathers_.end(); ++it )
    {
        Weather* weather = new Weather( **it );
        model.Resolver< Weather >::Register( weather->GetId(), *weather );
    }
}

// -----------------------------------------------------------------------------
// Name: GlobalWeathersList::Update
// Created: SLG 2010-03-17
// -----------------------------------------------------------------------------
void GlobalWeathersList::Update( const WeatherModel& model )
{
    Clear();
    tools::Iterator< const Weather& > it( model.Resolver< Weather >::CreateIterator() );
    while( it.HasMoreElements() )
    {
        Weather* weather = new Weather( it.NextElement() );
        weathers_.push_back( weather );
        QListViewItem* item = new QListViewItem( this );
        item->setText( 0, weather->GetName() );
    }
}

// -----------------------------------------------------------------------------
// Name: GlobalWeathersList::SelectedItem
// Created: SLG 2010-03-17
// -----------------------------------------------------------------------------
Weather* GlobalWeathersList::SelectedItem()
{
    if( selectedItem() )
    {
        const QString text = selectedItem()->text( 0 );
        for( IT_Weathers it = weathers_.begin(); it != weathers_.end(); ++it )
            if( (*it)->GetName() == text )
                return *it;
    }
    return 0;
}
