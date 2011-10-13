// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

/* TRANSLATOR gui::WeatherWidget */

#include "clients_gui_pch.h"
#include "WeatherWidget.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Units.h"
#include "meteo/Meteo.h"
#include "meteo/PHY_Precipitation.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: WeatherWidget constructor
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
WeatherWidget::WeatherWidget( QWidget* parent, const QString& title )
    : Q3GroupBox( 2, Qt::Horizontal, title, parent, "WeatherWidget" )
{
    new QLabel( tools::translate( "gui::WeatherWidget", "Wind speed/direction:" ), this );
    Q3HBox* box = new Q3HBox( this );
    box->layout()->setSpacing( 5 );
    windSpeed_ = new QSpinBox( 0, 300, 5, box );
    windSpeed_->setSuffix( Units::kilometersPerHour.AsString() );
    windDirection_ = new QSpinBox( 0, 359, 1, box );
    windDirection_->setSuffix( Units::degrees.AsString() );

    new QLabel( tools::translate( "gui::WeatherWidget", "Temperature:" ), this );
    temperature_ = new QSpinBox( -20, 40, 1, this );
    temperature_->setSuffix( "°C" );

    new QLabel( tools::translate( "gui::WeatherWidget", "Clouds floor/ceiling/density:" ), this );
    box = new Q3HBox( this );
    box->layout()->setSpacing( 5 );
    cloudFloor_   = new QSpinBox( 0, 100000, 100, box );
    cloudFloor_->setSuffix( Units::meters.AsString() );
    cloudCeiling_ = new QSpinBox( 0, 100000, 100, box );
    cloudCeiling_->setSuffix( Units::meters.AsString() );
    cloudDensity_ = new QSpinBox( 0, 10, 1, box );

    new QLabel( tools::translate( "gui::WeatherWidget", "Weather type:" ), this );
    type_ = new gui::ValuedComboBox< E_WeatherType >( this );
    assert( eNbrWeatherType > 0 );
    for( int i = 0; i < eNbrWeatherType; ++i )
        type_->AddItem( tools::ToDisplayedString( static_cast< E_WeatherType >( i ) ), static_cast< E_WeatherType >( i ) );
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget destructor
// Created: ABR 2011-05-30
// -----------------------------------------------------------------------------
WeatherWidget::~WeatherWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget::Clear
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
void WeatherWidget::Clear()
{
    windSpeed_->setValue( 0 );
    windDirection_->setValue( 0 );
    temperature_->setValue( 0 );
    cloudFloor_->setValue( 0 );
    cloudCeiling_->setValue( 0 );
    cloudDensity_->setValue( 0 );
    type_->setCurrentItem( 0 );
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget::Update
// Created: ABR 2011-06-06
// -----------------------------------------------------------------------------
void WeatherWidget::Update( const weather::Meteo& meteo )
{
    windSpeed_->setValue( static_cast< int >( meteo.GetWind().rSpeed_ / meteo.GetConversionFactor() ) );
    windDirection_->setValue( static_cast< int >( meteo.GetWind().eAngle_ ) );
    temperature_->setValue( meteo.GetTemperature() );
    cloudDensity_->setValue( meteo.GetCloud().nDensityPercentage_ );
    cloudFloor_->setValue( meteo.GetCloud().nFloor_ );
    cloudCeiling_->setValue( meteo.GetCloud().nCeiling_ );
    E_WeatherType type = meteo.GetPrecipitation().GetID();
    assert( type >= eWeatherType_None && type < eNbrWeatherType );
    type_->setCurrentItem( static_cast< int >( type ) );
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget::CommitTo
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherWidget::CommitTo( weather::Meteo& meteo ) const
{
    weather::Meteo::sWindData wind;
    wind.rSpeed_ = windSpeed_->value() * meteo.GetConversionFactor();
    wind.eAngle_ = windDirection_->value();
    weather::Meteo::sCloudData cloud;
    cloud.nFloor_ = cloudFloor_->value();
    cloud.nCeiling_ = cloudCeiling_->value();
    cloud.nDensityPercentage_ = cloudDensity_->value();
    cloud.rDensity_ = cloudDensity_->value() / 100.;

    meteo.SetWind( wind );
    meteo.SetCloud( cloud );
    meteo.SetTemperature( temperature_->value() );
    const weather::PHY_Precipitation* precipitation = weather::PHY_Precipitation::FindPrecipitation( tools::ToString( type_->GetValue() ).ascii() );
    if( !precipitation )
        throw std::runtime_error( std::string( __FUNCTION__ ) + " unknown precipitation: " + tools::ToString( type_->GetValue() ).ascii() );
    meteo.SetPrecipitation( *precipitation );
}
