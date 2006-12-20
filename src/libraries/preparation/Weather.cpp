// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Weather.h"
#include "clients_kernel/Tools.h"
#include "xeumeuleu/xml.h"

using namespace xml;
using namespace kernel;

namespace
{
    // $$$$ SBO 2006-12-19: !!
    kernel::E_WeatherType ConvertToWeatherType( const QString& type )
    {
        for( unsigned int i = 0; i < (int)eNbrWeatherType; ++i )
            if( QString( tools::GetXmlSection( (E_WeatherType)i ) ) == type )
                return (E_WeatherType)i;
        return (E_WeatherType)-1;
    }
}

// -----------------------------------------------------------------------------
// Name: Weather constructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
Weather::Weather()
    : temperature_( 20 )
    , temperatureGradient_( 1 )
    , windSpeed_( 0 )
    , windDirection_( 0 )
    , cloudFloor_( 1000 )
    , cloudCeiling_( 10000 )
    , cloudDensity_( 0 )
    , type_( eWeatherTypeNone )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Weather constructor
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
Weather::Weather( xml::xistream& xis )
{
    std::string precipitation;
    xis >> content( "Temperature", temperature_ )
        >> content( "GradianTemperature", temperatureGradient_ )
        >> content( "VitesseVent", windSpeed_ )
        >> content( "DirectionVent", windDirection_ )
        >> content( "PlancherCouvertureNuageuse", cloudFloor_ )
        >> content( "PlafondCouvertureNuageuse", cloudCeiling_ )
        >> content( "DensiteMoyenneCouvertureNuageuse", cloudDensity_ )
        >> content( "Precipitation", precipitation );
    type_ = ConvertToWeatherType( precipitation.c_str() );
}

// -----------------------------------------------------------------------------
// Name: Weather destructor
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
Weather::~Weather()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Weather::Serialize
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void Weather::Serialize( xml::xostream& xos ) const
{
    xos << content( "Temperature", temperature_ )
        << content( "GradianTemperature", temperatureGradient_ )
        << content( "VitesseVent", windSpeed_ )
        << content( "DirectionVent", windDirection_ )
        << content( "PlancherCouvertureNuageuse", cloudFloor_ )
        << content( "PlafondCouvertureNuageuse", cloudCeiling_ )
        << content( "DensiteMoyenneCouvertureNuageuse", cloudDensity_ )
        << content( "Precipitation", tools::GetXmlSection( type_ ) ); // $$$$ SBO 2006-12-20: 
}
