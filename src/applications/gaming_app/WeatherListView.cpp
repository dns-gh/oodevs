// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "WeatherListView.h"
#include "moc_WeatherListView.cpp"
#include "clients_kernel/Time_ABC.h"
#include "gaming/MeteoModel.h"
#include "meteo/MeteoLocal.h"
#include <boost/shared_ptr.hpp>

// -----------------------------------------------------------------------------
// Name: WeatherListView constructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
WeatherListView::WeatherListView( QWidget* parent, const kernel::CoordinateConverter_ABC& converter, const kernel::Time_ABC& simulation )
    : gui::WeatherListView( parent, converter )
    , simulation_( simulation )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeatherListView destructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
WeatherListView::~WeatherListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeatherListView::Update
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherListView::Update( const MeteoModel& model )
{
    Clear();
    const weather::MeteoManager_ABC::T_MeteoSet& meteos = model.GetLocalMeteos();
    unsigned int maxId = 0;
    for( weather::MeteoManager_ABC::CIT_MeteoSet it = meteos.begin(); it != meteos.end(); ++it )
    {
        boost::shared_ptr< weather::MeteoLocal > weather = boost::shared_ptr< weather::MeteoLocal >( new weather::MeteoLocal( *static_cast< weather::MeteoLocal* >( ( *it ).get() ) ) );
        weathers_.push_back( weather );
        maxId = ( maxId > weather->GetId() ) ? maxId : weather->GetId();
        Q3ListViewItem* item = new Q3ListViewItem( this );
        item->setText( 0, weather->GetName() );
    }
    weather::MeteoLocal::localCounter_ = maxId + 1;
}

// -----------------------------------------------------------------------------
// Name: WeatherListView::CreateItem
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
void WeatherListView::CreateItem()
{
    boost::shared_ptr< weather::MeteoLocal > weather = boost::shared_ptr< weather::MeteoLocal >( new weather::MeteoLocal( converter_, tr( "Local weather " ).ascii() ) );
    weather->SetCreated( true );
    weather->SetPeriod( simulation_.GetDateTime(), simulation_.GetDateTime() );
    Q3ListViewItem* item = new Q3ListViewItem( this );
    item->setText( 0, weather->GetName() );
    weathers_.push_back( weather );
}

// -----------------------------------------------------------------------------
// Name: WeatherListView::ContextMenuRequested
// Created: ABR 2011-06-08
// -----------------------------------------------------------------------------
void WeatherListView::ContextMenuRequested( Q3ListViewItem* /*item*/, const QPoint& point, int /*column*/ )
{
    Q3PopupMenu* menu = new Q3PopupMenu( this );
    menu->insertItem( tr( "Add" ), this, SLOT( CreateItem() ) );
    menu->exec( point );
}
