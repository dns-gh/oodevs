// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "WeatherWidget.h"
#include "moc_WeatherWidget.cpp"
#include "preparation/Weather.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Units.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: WeatherWidget constructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
WeatherWidget::WeatherWidget( QWidget* parent, const QString& title )
    : QGroupBox( 2, Qt::Horizontal, title, parent )
{
    new QLabel( tr( "Wind direction/speed:" ), this );
    QHBox* box = new QHBox( this );
    windDirection_ = new QSpinBox( 0, 359, 1, box );
    windDirection_->setSuffix( Units::degrees );
    windSpeed_     = new QSpinBox( 0, 300, 5, box );
    windSpeed_->setSuffix( Units::kilometersPerHour );
    new QLabel( tr( "Clouds floor/ceiling/density:" ), this );
    box = new QHBox( this );
    cloudFloor_   = new QSpinBox( 0, 100000, 100, box );
    cloudFloor_->setSuffix( Units::meters );
    cloudCeiling_ = new QSpinBox( 0, 100000, 100, box );
    cloudCeiling_->setSuffix( Units::meters );
    cloudDensity_ = new QSpinBox( 0, 10, 1, box );
    new QLabel( tr( "Weather type:" ), this );
    type_ = new gui::ValuedComboBox< kernel::E_WeatherType >( this );
    for( int i = 0; i < (int)kernel::eNbrWeatherType; ++i )
        type_->AddItem( tools::ToString( (kernel::E_WeatherType)i ), (kernel::E_WeatherType)i );
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget destructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
WeatherWidget::~WeatherWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget::Update
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherWidget::Update( const Weather& weather )
{
    windDirection_->setValue( weather.windDirection_ );
    windSpeed_->setValue( weather.windSpeed_ );
    cloudFloor_->setValue( weather.cloudFloor_ );
    cloudCeiling_->setValue( weather.cloudCeiling_ );
    cloudDensity_->setValue( weather.cloudDensity_ );
    type_->SetCurrentItem( weather.type_ ); 
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget::CommitTo
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherWidget::CommitTo( Weather& weather ) const
{
    weather.windDirection_ = windDirection_->value();
    weather.windSpeed_ = windSpeed_->value();
    weather.cloudFloor_ = cloudFloor_->value();
    weather.cloudCeiling_ = cloudCeiling_->value();
    weather.cloudDensity_ = cloudDensity_->value();
    weather.type_ = type_->GetValue();
}
