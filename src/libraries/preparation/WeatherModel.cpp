// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "WeatherModel.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Tools.h"
#include "meteo/Meteo.h"
#include "meteo/MeteoLocal.h"
#include "meteo/PHY_Precipitation.h"
#include "meteo/PHY_Lighting.h"
#include "tools/Loader_ABC.h"
#include "tools/SchemaWriter_ABC.h"
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#include <qregexp.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: WeatherModel constructor
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
WeatherModel::WeatherModel( Controller& controller, const CoordinateConverter_ABC& converter )
    : controller_   ( controller )
    , converter_    ( converter )
    , time_         ( QDateTime::currentDateTime() )
    , sunrise_      (  7, 30 )
    , sunset_       ( 21, 30 )
    , dayLighting_  ( eDayLightingJourSansNuage )
    , nightLighting_( eNightLightingNuitPleineLune )
    , globalWeather_( new weather::Meteo( 0, 0 ) )
{
    weather::PHY_Precipitation::Initialize();
    weather::PHY_Lighting::Initialize();
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherModel destructor
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
WeatherModel::~WeatherModel()
{
    weather::PHY_Lighting::Terminate();
    weather::PHY_Precipitation::Terminate();
    Purge();
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::Purge
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
void WeatherModel::Purge()
{
    DeleteAll();
    globalWeather_.reset( new weather::Meteo( 0, 0 ) );
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::Load
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
void WeatherModel::Load( const tools::Loader_ABC& fileLoader, const std::string& filename )
{
    fileLoader.LoadFile( filename, boost::bind( &WeatherModel::Read, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::Read
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
void WeatherModel::Read( xml::xistream& xis )
{
    Purge();
    xis >> xml::start( "weather" )
        >> xml::start( "exercise-date" );
    ReadExerciseDate( xis );
    xis >> xml::end
        >> xml::start( "ephemerides" );
    ReadEphemerides( xis );
    xis >> xml::end
        >> xml::start( "theater" );
    ReadGlobalWeather( xis );
    xis >> xml::end
        >> xml::start( "local-weather" )
            >> xml::list( "local", *this, &WeatherModel::ReadLocalWeather )
        >> xml::end;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::Serialize
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherModel::Serialize( const std::string& filename, const tools::SchemaWriter_ABC& schemaWriter ) const
{
    xml::xofstream xos( filename );
    xos << xml::start( "weather" );
    schemaWriter.WriteExerciseSchema( xos, "weather" );
    xos     << xml::start( "exercise-date" )
                << xml::attribute( "value", time_.toString( "yyyyMMddThhmmss" ).ascii() )
            << xml::end
            << xml::start( "ephemerides" )
                << xml::attribute( "sunrise", QString( "%1h%2m%3s" ).arg( sunrise_.hour() ).arg( sunrise_.minute() ).arg( sunrise_.second() ).ascii() )
                << xml::attribute( "sunset", QString( "%1h%2m%3s" ).arg( sunset_.hour() ).arg( sunset_.minute() ).arg( sunset_.second() ).ascii() )
                << xml::attribute( "day-lighting", tools::GetXmlSection( dayLighting_ ) )
                << xml::attribute( "night-lighting", tools::GetXmlSection( nightLighting_ ) )
            << xml::end
            << xml::start( "theater" );
    globalWeather_->Serialize( xos );
    xos     << xml::end
            << xml::start( "local-weather" );
    tools::Iterator< const weather::MeteoLocal& > it( CreateIterator() );
    while( it.HasMoreElements() )
    {
        xos << xml::start( "local" );
        it.NextElement().Serialize( xos );
        xos << xml::end;
    }
    xos     << xml::end
        << xml::end;
}

namespace
{
    QTime MakeTime( const QString& str )
    {
        QStringList list = QStringList::split( QRegExp( "[hms]" ), str );
        return QTime( list.count() > 0 ? list[0].toInt() : 0
                    , list.count() > 1 ? list[1].toInt() : 0
                    , list.count() > 2 ? list[2].toInt() : 0 );
    }

    E_DayLightingType ConvertToDayLighting( const QString& lighting )
    {
        for( unsigned int i = 0; i < static_cast< int >( eNbrDayLightingType ); ++i )
            if( QString( tools::GetXmlSection( static_cast< E_DayLightingType >( i ) ) ) == lighting )
                return static_cast< E_DayLightingType >( i );
        return static_cast< E_DayLightingType >( -1 );
    }

    E_NightLightingType ConvertToNightLighting( const QString& lighting )
    {
        for( unsigned int i = 0; i < static_cast< int >( eNbrNightLightingType ); ++i )
            if( QString( tools::GetXmlSection( static_cast< E_NightLightingType >( i ) ) ) == lighting )
                return static_cast< E_NightLightingType >( i );
        return static_cast< E_NightLightingType >( -1 );
    }
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::ReadExerciseDate
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
void WeatherModel::ReadExerciseDate( xml::xistream& xis )
{
    // $$$$ AGE 2007-10-12:
    const std::string isoDate = xis.attribute< std::string >( "value" );
    QString extended( isoDate.c_str() );
    extended.insert( 13, ':' ); extended.insert( 11, ':' );
    extended.insert(  6, '-' ); extended.insert(  4, '-' );
    time_ = QDateTime::fromString( extended, Qt::ISODate );
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::ReadEphemerides
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
void WeatherModel::ReadEphemerides( xml::xistream& xis )
{
    sunrise_ = MakeTime( xis.attribute< std::string >( "sunrise" ).c_str() );
    sunset_ = MakeTime( xis.attribute< std::string >( "sunset" ).c_str() );
    if( xis.has_attribute( "moon" ) )
    {
        // $$$$ JSR 2010-07-09: Keep compatibility with old model (one lighting )
        std::string lighting = xis.attribute< std::string >( "moon" );
        E_DayLightingType dayType = ConvertToDayLighting( lighting.c_str() );
        E_NightLightingType nightType = ConvertToNightLighting( lighting.c_str() );
        if( dayType != static_cast< E_DayLightingType >( -1 ) )
            dayLighting_ = dayType;
        if( nightType != static_cast< E_NightLightingType >( -1 ) )
            nightLighting_ = nightType;
    }
    else
    {
        // $$$$ JSR 2010-07-09: New model (day and night lightings)
        dayLighting_ = ConvertToDayLighting( xis.attribute< std::string >( "day-lighting" ).c_str() );
        nightLighting_ = ConvertToNightLighting( xis.attribute< std::string >( "night-lighting" ).c_str() );
    }
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::ReadGlobalWeather
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
void WeatherModel::ReadGlobalWeather( xml::xistream& xis )
{
    globalWeather_.reset( new weather::Meteo( 0, xis, 0, 0 ) );
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::ReadLocalWeather
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
void WeatherModel::ReadLocalWeather( xml::xistream& xis )
{
    weather::MeteoLocal* local = new weather::MeteoLocal( xis, converter_, tools::translate( "WeatherModel", "Local weather " ).ascii() );
    Register( local->GetId(), *local );
}
