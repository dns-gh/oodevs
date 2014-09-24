// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "WeatherPanel.h"
#include "moc_WeatherPanel.cpp"
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/RichWidget.h"
#include "clients_gui/RichWidget.h"
#include "clients_gui/WeatherWidget.h"
#include "clients_gui/WeatherLayer.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Tools.h"
#include "meteo/MeteoLocal.h"
#include "preparation/WeatherModel.h"
#include "WeatherListView.h"
#include "ENT/ENT_Tr.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: WeatherPanel constructor
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
WeatherPanel::WeatherPanel( QWidget* parent,
                            gui::PanelStack_ABC& panel,
                            Controllers& controllers,
                            const CoordinateConverter_ABC& converter,
                            const std::shared_ptr< gui::WeatherLayer >& layer )
    : gui::WeatherPanel( parent, panel, layer )
    , controllers_ ( controllers )
    , currentModel_( 0 )
{
    gui::SubObjectName subObject( "WeatherPanel" );
    // Current date & Time
    Q3HBox* timeBox = new Q3HBox();
    headerLayout_->addWidget( timeBox );
    timeBox->layout()->setMargin( 5 );
    new QLabel( tr( "Exercise date:" ), timeBox );
    time_ = new gui::RichWidget< QDateTimeEdit >( "time", timeBox );
    time_->setDateTime( QDateTime::currentDateTime() );

    // Ephemerides
    {
        gui::SubObjectName subObject( "Ephemerides" );
        QLabel* sunriseLabel = new QLabel( tr( "Sunrise:" ) );
        sunrise_ = new gui::RichWidget< QTimeEdit >( "sunrise");

        QLabel* sunsetLabel = new QLabel( tr( "Sunset:" ) );
        sunset_ = new gui::RichWidget< QTimeEdit >( "sunset" );

        QLabel* dayLabel = new QLabel( tr( "Day lighting:" ) );
        dayLighting_ = new gui::ValuedComboBox< E_LightingType >( "dayLighting" );
        for( int i = 0; i < static_cast< int >( eLightingType_NuitPleineLune ); ++i )
            dayLighting_->AddItem( ENT_Tr::ConvertFromLightingType( static_cast< E_LightingType >( i ), ENT_Tr::eToTr ), static_cast< E_LightingType >( i ) );

        QLabel* nightLabel = new QLabel( tr( "Night lighting:" ) );
        nightLighting_ = new gui::ValuedComboBox< E_LightingType >( "nightLighting" );
        for( int i = static_cast< int >( eLightingType_NuitPleineLune ); i < static_cast< int >( eLightingType_Eclairant ); ++i )
            nightLighting_->AddItem( ENT_Tr::ConvertFromLightingType( static_cast< E_LightingType >( i ), ENT_Tr::eToTr ), static_cast< E_LightingType >( i ) );

        gui::RichGroupBox* ephemeridesGroup = new gui::RichGroupBox( "ephemeridesGroup", tr( "Ephemerides" ) );
        QGridLayout* ephemeridesGroupLayout = new QGridLayout( ephemeridesGroup );
        ephemeridesGroupLayout->addWidget( sunriseLabel, 0, 0 );
        ephemeridesGroupLayout->addWidget( sunrise_, 0, 1 );
        ephemeridesGroupLayout->addWidget( sunsetLabel, 1, 0 );
        ephemeridesGroupLayout->addWidget( sunset_, 1, 1 );
        ephemeridesGroupLayout->addWidget( dayLabel, 2, 0 );
        ephemeridesGroupLayout->addWidget( dayLighting_, 2, 1 );
        ephemeridesGroupLayout->addWidget( nightLabel, 3, 0 );
        ephemeridesGroupLayout->addWidget( nightLighting_, 3, 1 );
        headerLayout_->addWidget( ephemeridesGroup );
    }

    // Global Weather
    globalWeatherWidget_ = new gui::WeatherWidget( "globalWidget", globalWidget_, tr( "Weather parameters" ) );
    // Local Weather
    localWeatherWidget_ = new gui::WeatherWidget( "localWidget", localWidget_, tr( "Weather parameters" ) );
    CreateLocalParameters();
    localWeathers_ = new WeatherListView( localWidget_, converter );
    connect( localWeathers_->selectionModel(), SIGNAL( currentChanged( const QModelIndex&, const QModelIndex& ) ), this, SLOT( LocalSelectionChanged() ) );
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
    globalWeatherWidget_->Update( *currentModel_->globalWeather_ );
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
    globalWeatherWidget_->CommitTo( *currentModel_->globalWeather_ );
    static_cast< WeatherListView* >( localWeathers_ )->CommitTo( *currentModel_ );
    controllers_.controller_.Create( this );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::Reset
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherPanel::Reset()
{
    layer_->Clear();
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

// -----------------------------------------------------------------------------
// Name: WeatherPanel::NotifyUpdated
// Created: SLI 2013-12-02
// -----------------------------------------------------------------------------
void WeatherPanel::NotifyUpdated( const kernel::ModelLoaded& )
{
    Show();
}
