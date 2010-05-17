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
#include "WeatherWidget.h"
#include "LocalWeathersList.h"
#include "WeatherLayer.h"
#include "preparation/WeatherModel.h"
#include "preparation/LocalWeather.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Tools.h"

#include <qhgroupbox.h>
#include <qgrid.h>

// -----------------------------------------------------------------------------
// Name: WeatherPanel constructor
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
WeatherPanel::WeatherPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers, const kernel::CoordinateConverter_ABC& converter, WeatherLayer& layer )
    : InfoPanel_ABC( parent, panel, tr( "Weather" ), "WeatherPanel" )
    , controllers_( controllers )
    , layer_( layer )
    , currentModel_( 0 )
    , selectedLocal_( 0 )
{
    QHBox* timeBox = new QHBox( this );
    new QLabel( tr( "Exercise date:" ), timeBox );
    time_ = new QDateTimeEdit( timeBox );
    time_->setDateTime( QDateTime::currentDateTime() );

    QGroupBox* group = new QGroupBox( 2, Qt::Horizontal, tr( "Ephemerides" ), this );
    new QLabel( tr( "Sunrise:" ), group );
    sunrise_ = new QTimeEdit( group );
    new QLabel( tr( "Sunset:" ), group );
    sunset_ = new QTimeEdit( group );
    new QLabel( tr( "Lighting:" ), group );
    lighting_ = new gui::ValuedComboBox< kernel::E_LightingType >( group );
    for( int i = 0; i < (int)kernel::eNbrLightingType; ++i )
        lighting_->AddItem( tools::ToString( (kernel::E_LightingType)i ), (kernel::E_LightingType)i );

    globalWeather_ = new WeatherWidget( this, tr( "Global weather" ) );
    QGroupBox* localGroup = new QGroupBox( 1, Qt::Horizontal, tr( "Local weather" ), this );
    localWeathers_ = new LocalWeathersList( localGroup, converter );
    localWeatherBox_ = new QVBox( localGroup );
    localWeather_ = new WeatherWidget( localWeatherBox_, tr( "Weather parameters" ) );

    QGroupBox* localParametersGroup = new QGroupBox( 2, Qt::Horizontal, tr( "Time and Position Parameters" ), localWeatherBox_ );
    startTimeLabel_ = new QLabel( tr( "Start time:" ), localParametersGroup );
    startTime_ = new QDateTimeEdit( localParametersGroup );
    endTimeLabel_ = new QLabel( tr( "End time:" ), localParametersGroup );
    endTime_ = new QDateTimeEdit( localParametersGroup );

    QButton* btn = new QPushButton( tr( "Set location" ), localParametersGroup );
    connect( btn, SIGNAL( clicked() ), this, SLOT( SetPatchPosition() ) );
    localWeatherBox_->hide();

    QHBox* box = new QHBox( this );
    box->setMaximumHeight( 30 );
    QButton* okBtn     = new QPushButton( tr( "Save" )  , box );
    QButton* cancelBtn = new QPushButton( tr( "Cancel" ), box );

    connect( okBtn,     SIGNAL( clicked() ), this, SLOT( Commit() ) );
    connect( cancelBtn, SIGNAL( clicked() ), this, SLOT( Reset() ) );
    connect( localWeathers_, SIGNAL( selectionChanged() ), this, SLOT( LocalSelectionChanged() ) );
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
    lighting_->SetCurrentItem( currentModel_->lighting_ );
    globalWeather_->Update( *currentModel_->globalWeather_ );
    localWeathers_->Update( *currentModel_ );
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
    currentModel_->lighting_ = lighting_->GetValue();
    globalWeather_->CommitTo( *currentModel_->globalWeather_ );
    localWeathers_->CommitTo( *currentModel_ );
    controllers_.controller_.Create( this );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::CommitLocalWeather
// Created: SBO 2010-05-17
// -----------------------------------------------------------------------------
void WeatherPanel::CommitLocalWeather()
{
    if( selectedLocal_ )
    {
        localWeather_->CommitTo( *selectedLocal_ );
        selectedLocal_->SetPeriod( startTime_->dateTime(), endTime_->dateTime() );
    }
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::Reset
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherPanel::Reset()
{
    if( currentModel_ )
        NotifyUpdated( *currentModel_ );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::LocalSelectionChanged
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherPanel::LocalSelectionChanged()
{
    LocalWeather* selected = localWeathers_->SelectedItem();
    if( selected )
    {
        CommitLocalWeather();
        localWeather_->Update( *selected );
        startTime_->setDateTime( selected->GetStartTime() );
        endTime_->setDateTime( selected->GetEndTime() );
        layer_.SetPosition( *selected );
        selectedLocal_ = selected;
    }
    localWeatherBox_->setShown( selected != 0 );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::SetPatchPosition
// Created: SBO 2006-12-21
// -----------------------------------------------------------------------------
void WeatherPanel::SetPatchPosition()
{
    layer_.StartEdition( *localWeathers_->SelectedItem() );
}
