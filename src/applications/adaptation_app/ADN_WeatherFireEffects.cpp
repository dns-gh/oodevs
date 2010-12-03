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
#include <xeumeuleu/xml.hpp>

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
// Name: ADN_WeatherFireEffects::GetNodeName
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
std::string ADN_WeatherFireEffects::GetNodeName()
{
    std::string strResult( "� la condition m�t�o " );
    return strResult + ADN_Tr::ConvertFromSensorWeatherModifiers( weatherType_, ENT_Tr_ABC::eToTr );
}

// -----------------------------------------------------------------------------
// Name: ADN_WeatherFireEffects::GetItemName
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
std::string ADN_WeatherFireEffects::GetItemName()
{
    return ADN_Tr::ConvertFromSensorWeatherModifiers( weatherType_, ENT_Tr_ABC::eToTr );
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
void ADN_WeatherFireEffects::WriteArchive( xml::xostream& xos, const std::string& tag )
{
    xos << xml::start( tag )
            << xml::attribute( "weather", ADN_Tools::Scriptify( ADN_Tr::ConvertFromSensorWeatherModifiers( weatherType_ ) ) )
            << xml::attribute( "heat-decrease-rate", heatDecreaseRate_ )
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: ADN_WeatherFireEffects::operator<<
// Created: JSR 2010-12-01
// -----------------------------------------------------------------------------
xml::xostream& ADN_WeatherFireEffects::operator<<( xml::xostream& xos )
{
    WriteArchive( xos );
    return xos;
}
