// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_WeatherFireEffects.h"
#include "ADN_Tr.h"

// -----------------------------------------------------------------------------
// Name: ADN_WeatherFireEffects constructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_WeatherFireEffects::ADN_WeatherFireEffects( E_SensorWeatherModifiers weatherType )
    : weatherType_( weatherType )
    , heatDecreaseRate_( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_WeatherFireEffects destructor
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
ADN_WeatherFireEffects::~ADN_WeatherFireEffects()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ADN_WeatherFireEffects::CopyFrom
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_WeatherFireEffects::CopyFrom( ADN_WeatherFireEffects& other )
{
    weatherType_ = other.weatherType_;
    heatDecreaseRate_ = other.heatDecreaseRate_.GetData();
}

// -----------------------------------------------------------------------------
// Name: ADN_WeatherFireEffects::ReadArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_WeatherFireEffects::ReadArchive( xml::xistream& xis )
{
    xis >> xml::attribute( "heat-decrease-rate", heatDecreaseRate_ );
}

// -----------------------------------------------------------------------------
// Name: ADN_WeatherFireEffects::WriteArchive
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
void ADN_WeatherFireEffects::WriteArchive( xml::xostream& xos, const std::string& tag ) const
{
    xos << xml::start( tag )
            << xml::attribute( "weather", ADN_Tools::Scriptify( ADN_Tr::ConvertFromSensorWeatherModifiers( weatherType_ ) ) )
            << xml::attribute( "heat-decrease-rate", heatDecreaseRate_ )
        << xml::end;
}
