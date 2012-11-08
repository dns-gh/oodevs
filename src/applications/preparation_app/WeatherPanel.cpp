// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "WeatherPanel.h"
#include "moc_WeatherPanel.cpp"
#include "clients_gui/WeatherWidget.h"
#include "clients_gui/WeatherLayer.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Tools.h"
#include "meteo/MeteoLocal.h"
#include "preparation/WeatherModel.h"
#include "WeatherListView.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: WeatherPanel constructor
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
WeatherPanel::WeatherPanel( QWidget* parent, gui::PanelStack_ABC& panel, Controllers& controllers, const CoordinateConverter_ABC& converter, gui::WeatherLayer& layer )
    : gui::WeatherPanel( parent, panel, layer )
    , controllers_ ( controllers )
    , currentModel_( 0 )
{
    // Current date & Time
    Q3HBox* timeBox = new Q3HBox( headerLayout_ );
    timeBox->layout()->setMargin( 5 );
    new QLabel( tr( "Exercise date:" ), timeBox );
    time_ = new QDateTimeEdit( timeBox );
    time_->setDateTime( QDateTime::currentDateTime() );

    // Ephemerides
    {
        Q3GroupBox* group = new Q3GroupBox( 2, Qt::Horizontal, tr( "Ephemerides" ), headerLayout_ );
        new QLabel( tr( "Sunrise:" ), group );
        sunrise_ = new QTimeEdit( group );
        new QLabel( tr( "Sunset:" ), group );
        sunset_ = new QTimeEdit( group );
        new QLabel( tr( "Moon:" ), group );
        nightLighting_ = new gui::ValuedComboBox< E_NightLightingType >( group );
        for( int i = 0; i < static_cast< int >( eNbrNightLightingType ); ++i )
            nightLighting_->AddItem( tools::ToDisplayedString( static_cast< E_NightLightingType>( i ) ), static_cast< E_NightLightingType>( i ) );
    }

    // Global Weather
    globalWidget_ = new gui::WeatherWidget( globalLayout_, tr( "Weather parameters" ) );
    new QLabel( tr( "Day lighting:" ), globalWidget_ );
    dayLighting_ = new gui::ValuedComboBox< E_DayLightingType >( globalWidget_ );
    for( int i = 0; i < static_cast< int >( eNbrDayLightingType ); ++i )
        dayLighting_->AddItem( tools::ToDisplayedString( static_cast< E_DayLightingType>( i ) ), static_cast< E_DayLightingType>( i ) );

    // Local Weather
    localWidget_ = new gui::WeatherWidget( localLayout_, tr( "Weather parameters" ) );
    CreateLocalParameters();
    localWeathers_ = new WeatherListView( localLayout_, converter );
    connect( localWeathers_, SIGNAL( selectionChanged() ), this, SLOT( LocalSelectionChanged() ) );
    connect( sunrise_, SIGNAL( timeChanged( const QTime& ) ), SLOT( OnSunRiseChanged( const QTime& ) ) );
    connect( sunset_, SIGNAL( timeChanged( const QTime& ) ), SLOT( OnSunSetChanged( const QTime& ) ) );

    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel destructor
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
WeatherPanel::~WeatherPanel()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::NotifyUpdated
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherPanel::NotifyUpdated( const WeatherModel& model )
{
    currentModel_ = const_cast< WeatherModel* >( &model );
    time_->setDateTime( currentModel_->time_ );
    sunrise_->setTime( currentModel_->sunrise_ );
    sunset_ ->setTime( currentModel_->sunset_  );
    dayLighting_->SetCurrentItem( currentModel_->dayLighting_ );
    nightLighting_->SetCurrentItem( currentModel_->nightLighting_ );
    globalWidget_->Update( *currentModel_->globalWeather_ );
    static_cast< WeatherListView* >( localWeathers_ )->Update( *currentModel_ );
    Show();
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::NotifyDeleted
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherPanel::NotifyDeleted( const WeatherModel& model )
{
    if( &model == currentModel_ )
        currentModel_ = 0;
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::Commit
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherPanel::Commit()
{
    if( !currentModel_ )
        return;
    CommitLocalWeather();
    currentModel_->time_     = time_->dateTime();
    currentModel_->sunrise_  = sunrise_->time();
    currentModel_->sunset_   = sunset_ ->time();
    currentModel_->dayLighting_ = dayLighting_->GetValue();
    currentModel_->nightLighting_ = nightLighting_->GetValue();
    globalWidget_->CommitTo( *currentModel_->globalWeather_ );
    static_cast< WeatherListView* >( localWeathers_ )->CommitTo( *currentModel_ );
    controllers_.controller_.Create( this );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::Reset
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherPanel::Reset()
{
    layer_.Clear();
    selectedLocal_ = 0;
    if( currentModel_ )
        NotifyUpdated( *currentModel_ );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::OnSunRiseChanged
// Created: MMC 2012-04-17
// -----------------------------------------------------------------------------
void WeatherPanel::OnSunRiseChanged( const QTime &date )
{
    sunset_->setMinimumTime( date );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::OnSunSetChanged
// Created: MMC 2012-04-17
// -----------------------------------------------------------------------------
void WeatherPanel::OnSunSetChanged( const QTime &date )
{
    sunrise_->setMaximumTime( date );
}