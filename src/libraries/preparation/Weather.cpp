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
#include <xeumeuleu/xml.h>

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

    QTime MakeTime( const QString& str )
    {
        QStringList list = QStringList::split( QRegExp( "[hms]" ), str );
        return QTime( list.count() > 0 ? list[0].toInt() : 0
            , list.count() > 1 ? list[1].toInt() : 0
            , list.count() > 2 ? list[2].toInt() : 0 );
    }

}

// -----------------------------------------------------------------------------
// Name: Weather constructor
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
Weather::Weather()
    : windSpeed_( 0 )
    , windDirection_( 0 )
    , cloudFloor_( 1000 )
    , cloudCeiling_( 10000 )
    , cloudDensity_( 0 )
    , type_( eWeatherTypeNone )
    , startTime_()
    , endTime_()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Weather constructor
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
Weather::Weather( xml::xistream& xis )
{   
    std::string precipitation, startTime, endTime;
    xis >> attribute( "start-time", startTime )
        >> attribute( "end-time", endTime );
    xis >> start( "wind" )
            >> attribute( "speed", windSpeed_ )
            >> attribute( "direction", windDirection_ )
        >> end()
        >> start( "cloud-cover" )
            >> attribute( "floor", cloudFloor_ )
            >> attribute( "ceiling", cloudCeiling_ )
            >> attribute( "density", cloudDensity_ )
        >> end()
        >> start( "precipitation" )
            >> attribute( "value", precipitation )
        >> end();
    type_ = ConvertToWeatherType( precipitation.c_str() );
    startTime_ = MakeTime( startTime.c_str() );
    endTime_ = MakeTime( endTime.c_str() );
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
    xos << attribute( "start-time", QString( "%1h%2m%3s" ).arg( startTime_.hour() ).arg( startTime_.minute() ).arg( startTime_.second() ).ascii() )
        << attribute( "end-time", QString( "%1h%2m%3s" ).arg( endTime_.hour() ).arg( endTime_.minute() ).arg( endTime_.second() ).ascii() );
    xos << start( "wind" )
            << attribute( "speed", windSpeed_ )
            << attribute( "direction", windDirection_ )
        << end()
        << start( "cloud-cover" )
            << attribute( "floor", cloudFloor_ )
            << attribute( "ceiling", cloudCeiling_ )
            << attribute( "density", cloudDensity_ )
        << end()
        << start( "precipitation" )
            << attribute( "value", tools::GetXmlSection( type_ ) ) // $$$$ SBO 2006-12-20: 
        << end();
}
