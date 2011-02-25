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
#include <xeumeuleu/xml.hpp>

using namespace kernel;

namespace
{
    // $$$$ SBO 2006-12-19: !!
    E_WeatherType ConvertToWeatherType( const QString& type )
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
    : windSpeed_    ( 0 )
    , windDirection_( 0 )
    , cloudFloor_   ( 1000 )
    , cloudCeiling_ ( 10000 )
    , cloudDensity_ ( 0 )
    , type_         ( eWeatherType_None )
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
    xis >> xml::start( "wind" )
            >> xml::attribute( "speed", windSpeed_ )
            >> xml::attribute( "heading", windDirection_ )
        >> xml::end
        >> xml::start( "cloud-cover" )
            >> xml::attribute( "floor", cloudFloor_ )
            >> xml::attribute( "ceiling", cloudCeiling_ )
            >> xml::attribute( "density", cloudDensity_ )
        >> xml::end
        >> xml::start( "precipitation" )
            >> xml::attribute( "value", precipitation )
        >> xml::end;
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
    xos << xml::start( "wind" )
            << xml::attribute( "speed", windSpeed_ )
            << xml::attribute( "heading", windDirection_ )
        << xml::end
        << xml::start( "cloud-cover" )
            << xml::attribute( "floor", cloudFloor_ )
            << xml::attribute( "ceiling", cloudCeiling_ )
            << xml::attribute( "density", cloudDensity_ )
        << xml::end
        << xml::start( "precipitation" )
            << xml::attribute( "value", tools::GetXmlSection( type_ ) ) // $$$$ SBO 2006-12-20:
        << xml::end;
}
