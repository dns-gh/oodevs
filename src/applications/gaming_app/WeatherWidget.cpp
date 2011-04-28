// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "WeatherWidget.h"
#include "moc_WeatherWidget.cpp"
#include "actions/MagicAction.h"
#include "actions/Numeric.h"
#include "actions/Direction.h"
#include "actions/Enumeration.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Units.h"
#include "meteo/PHY_Meteo.h"
#include "meteo/PHY_Precipitation.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;
using namespace actions;
using namespace parameters;

// -----------------------------------------------------------------------------
// Name: WeatherWidget constructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
WeatherWidget::WeatherWidget( QWidget* parent, const QString& title )
    : QGroupBox( 2, Qt::Horizontal, title, parent, "WeatherWidget" )
{
    new QLabel( tools::translate( "WeatherWidget", "Wind speed/direction:" ), this );
    QHBox* box = new QHBox( this );
    windSpeed_     = new QSpinBox( 0, 300, 5, box );
    windSpeed_->setSuffix( Units::kilometersPerHour.AsString() );
    windDirection_ = new QSpinBox( 0, 359, 1, box );
    windDirection_->setSuffix( Units::degrees.AsString() );
    new QLabel( tools::translate( "WeatherWidget", "Clouds floor/ceiling/density:" ), this );
    box = new QHBox( this );
    cloudFloor_   = new QSpinBox( 0, 100000, 100, box );
    cloudFloor_->setSuffix( Units::meters.AsString() );
    cloudCeiling_ = new QSpinBox( 0, 100000, 100, box );
    cloudCeiling_->setSuffix( Units::meters.AsString() );
    cloudDensity_ = new QSpinBox( 0, 10, 1, box );
    new QLabel(  tools::translate( "WeatherWidget", "Weather type:" ), this );
    type_ = new gui::ValuedComboBox< E_WeatherType >( this );
    for( int i = 0; i < eNbrWeatherType; ++i )
        type_->AddItem( tools::ToDisplayedString( static_cast< E_WeatherType >( i ) ), static_cast< E_WeatherType >( i ) );
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
// Created: ABR 2011-04-28
// -----------------------------------------------------------------------------
void WeatherWidget::Update( const weather::PHY_Meteo& globalMeteo )
{
    windSpeed_->setValue( static_cast< int >( globalMeteo.GetWind().rWindSpeed_ / globalMeteo.GetConversionFactor() ) );
    windDirection_->setValue( static_cast< int >( globalMeteo.GetWind().eWindAngle_ ) );
    cloudDensity_->setValue( static_cast< int >( globalMeteo.GetCloudDensity() ) );
    cloudFloor_->setValue( globalMeteo.GetCloudFloor() );
    cloudCeiling_->setValue( globalMeteo.GetCloudCeiling() );
    E_WeatherType type = globalMeteo.GetPrecipitation().GetID();
    assert( type >= eWeatherType_None && type < eNbrWeatherType );
    type_->setCurrentItem( static_cast< int >( type ) );
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget::Commit
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void WeatherWidget::Commit( sword::WeatherAttributes& att ) const
{
    att.mutable_wind_direction()->set_heading( windDirection_->value() );
    att.set_wind_speed( windSpeed_->value() );
    att.set_cloud_floor( cloudFloor_->value() );
    att.set_cloud_ceiling( cloudCeiling_->value() );
    att.set_cloud_density( cloudDensity_->value() );
    const weather::PHY_Precipitation* precipitation = weather::PHY_Precipitation::FindPrecipitation( tools::ToString( type_->GetValue() ).ascii() );
    att.set_precipitation( precipitation->GetAsnID() );
    att.set_temperature( 0 );
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget::Commit
// Created: JSR 2010-04-12
// -----------------------------------------------------------------------------
void WeatherWidget::Commit( sword::MissionParameters& att ) const
{
    att.add_elem()->mutable_value()->Add()->set_areal( 0 ); // Temperature
    att.add_elem()->mutable_value()->Add()->set_areal( ( float) windSpeed_->value() );
    att.add_elem()->mutable_value()->Add()->mutable_heading()->set_heading( windDirection_->value() );
    att.add_elem()->mutable_value()->Add()->set_areal( ( float) cloudFloor_->value() );
    att.add_elem()->mutable_value()->Add()->set_areal( ( float) cloudCeiling_->value() );
    att.add_elem()->mutable_value()->Add()->set_areal( ( float) cloudDensity_->value() );
    const weather::PHY_Precipitation* precipitation = weather::PHY_Precipitation::FindPrecipitation( tools::ToString( type_->GetValue() ).ascii() );
    att.add_elem()->mutable_value()->Add()->set_enumeration( precipitation->GetAsnID() );
}

// -----------------------------------------------------------------------------
// Name: WeatherWidget::CreateParameters
// Created: JSR 2010-04-23
// -----------------------------------------------------------------------------
void WeatherWidget::CreateParameters( MagicAction& action, tools::Iterator< const OrderParameter& >& it )
{
    action.AddParameter( *new Numeric( it.NextElement(), 0 ) ); // TODO temperature
    action.AddParameter( *new Numeric( it.NextElement(), ( float) windSpeed_->value() ) );
    action.AddParameter( *new Direction( it.NextElement(), windDirection_->value() ) );
    action.AddParameter( *new Numeric( it.NextElement(), ( float) cloudFloor_->value() ) );
    action.AddParameter( *new Numeric( it.NextElement(), ( float) cloudCeiling_->value() ) );
    action.AddParameter( *new Numeric( it.NextElement(), ( float) cloudDensity_->value() ) );
    action.AddParameter( *new Enumeration( it.NextElement(), type_->GetValue() ) );
}
