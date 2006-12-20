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
#include "WeatherWidget.h"
#include "LocalWeathersList.h"
#include "preparation/WeatherModel.h"
#include "preparation/LocalWeather.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"

#include <qhgroupbox.h>
#include <qgrid.h>

// -----------------------------------------------------------------------------
// Name: WeatherPanel constructor
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
WeatherPanel::WeatherPanel( QWidget* parent, gui::PanelStack_ABC& panel, kernel::Controllers& controllers )
    : gui::InfoPanel_ABC( parent, panel, tr( "Weather" ) )
    , controllers_( controllers )
    , currentModel_( 0 )
    , selectedLocal_( 0 )
{
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
    localWeathers_ = new LocalWeathersList( this );
    localWeather_ = new WeatherWidget( this, tr( "Local weather" ) );
    localWeather_->hide();

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
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherPanel::NotifyUpdated
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherPanel::NotifyUpdated( const WeatherModel& model )
{
    currentModel_ = const_cast< WeatherModel* >( &model );
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
    currentModel_->sunrise_  = sunrise_->time();
    currentModel_->sunset_   = sunset_ ->time();
    currentModel_->lighting_ = lighting_->GetValue();
    globalWeather_->CommitTo( *currentModel_->globalWeather_ );
    localWeathers_->CommitTo( *currentModel_ );
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
        if( selectedLocal_ )
            localWeather_->CommitTo( *selectedLocal_ );
        localWeather_->Update( *selected );
        localWeather_->show();
        selectedLocal_ = selected;
    }
    else
        localWeather_->hide();
}
