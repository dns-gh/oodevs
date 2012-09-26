// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "WeatherListView.h"
#include "moc_WeatherListView.cpp"
#include "clients_kernel/Tools.h"
#include "meteo/MeteoLocal.h"
#include "preparation/WeatherModel.h"

// -----------------------------------------------------------------------------
// Name: WeatherListView constructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
WeatherListView::WeatherListView( QWidget* parent, const kernel::CoordinateConverter_ABC& converter )
    : gui::WeatherListView( parent, converter )
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
// Name: WeatherListView::CommitTo
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherListView::CommitTo( WeatherModel& model )
{
    model.Resolver< weather::MeteoLocal >::DeleteAll();
    for( IT_Weathers it = weathers_.begin(); it != weathers_.end(); ++it )
    {
        weather::MeteoLocal* weather = new weather::MeteoLocal( *static_cast< weather::MeteoLocal* >( ( *it ).get() ) );
        weather->SetCreated( true );
        if( weather->IsValid() )
            model.Resolver< weather::MeteoLocal >::Register( weather->GetId(), *weather );
        else
            QMessageBox::warning( this, weather->GetName().c_str(), tr( "Time parameters or location are incorrect" ) );
    }
    QMessageBox::warning( this, tr( "Weather" ), tr( "Weather modifications applied" ) );
    selectionModel()->clear();
}

namespace
{
    void Create( const std::string& name, QStandardItemModel& model )
    {
        QStandardItem* item = new QStandardItem( name.c_str() );
        item->setEditable( false );
        model.appendRow( item );
    }
}

// -----------------------------------------------------------------------------
// Name: WeatherListView::Update
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherListView::Update( const WeatherModel& model )
{
    Clear();
    exerciceTime_ = model.time_;
    tools::Iterator< const weather::MeteoLocal& > it( model.Resolver< weather::MeteoLocal >::CreateIterator() );
    while( it.HasMoreElements() )
    {
        boost::shared_ptr< weather::MeteoLocal > weather = boost::shared_ptr< weather::MeteoLocal >( new weather::MeteoLocal( it.NextElement() ) );
        weather->SetCreated( true );
        weathers_.push_back( weather );
        Create( weather->GetName(), *model_ );
    }
}

// -----------------------------------------------------------------------------
// Name: WeatherListView::CreateItem
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
void WeatherListView::CreateItem()
{
    boost::shared_ptr< weather::MeteoLocal > weather = boost::shared_ptr< weather::MeteoLocal >( new weather::MeteoLocal( converter_, tr( "Local weather " ).toAscii().constData() ) );
    weather->SetCreated( true );
    weather->SetPeriod( tools::QTimeToBoostTime( exerciceTime_ ), tools::QTimeToBoostTime( exerciceTime_.addDays( 1 ) ) );
    Create( weather->GetName(), *model_ );
    weathers_.push_back( weather );
}
