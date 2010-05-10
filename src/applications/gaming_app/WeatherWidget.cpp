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
#include "preparation/Weather.h"
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
    new QLabel( tr( "Wind direction/speed:" ), this );
    QHBox* box = new QHBox( this );
    windDirection_ = new QSpinBox( 0, 359, 1, box );
    windDirection_->setSuffix( Units::degrees.AsString() );
    windSpeed_     = new QSpinBox( 0, 300, 5, box );
    windSpeed_->setSuffix( Units::kilometersPerHour.AsString() );
    new QLabel( tr( "Clouds floor/ceiling/density:" ), this );
    box = new QHBox( this );
    cloudFloor_   = new QSpinBox( 0, 100000, 100, box );
    cloudFloor_->setSuffix( Units::meters.AsString() );
    cloudCeiling_ = new QSpinBox( 0, 100000, 100, box );
    cloudCeiling_->setSuffix( Units::meters.AsString() );
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
// Name: WeatherWidget::Commit
// Created: SLG 2010-03-25
// -----------------------------------------------------------------------------
void WeatherWidget::Commit( Common::MsgMeteoAttributes& att ) const
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
void WeatherWidget::Commit( Common::MsgMissionParameters& att ) const
{    
    att.add_elem()->mutable_value()->set_areal( 0 ); // Temperature
    att.add_elem()->mutable_value()->set_areal( ( float) windSpeed_->value() );
    att.add_elem()->mutable_value()->mutable_heading()->set_heading( windDirection_->value() );
    att.add_elem()->mutable_value()->set_areal( ( float) cloudFloor_->value() );
    att.add_elem()->mutable_value()->set_areal( ( float) cloudCeiling_->value() );
    att.add_elem()->mutable_value()->set_areal( ( float) cloudDensity_->value() );
    const weather::PHY_Precipitation* precipitation = weather::PHY_Precipitation::FindPrecipitation( tools::ToString( type_->GetValue() ).ascii() );
    att.add_elem()->mutable_value()->set_enumeration( precipitation->GetAsnID() );
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
