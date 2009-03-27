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
#include "Weather.h"
#include "LocalWeather.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.h>
#include <qregexp.h>

using namespace xml;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: WeatherModel constructor
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
WeatherModel::WeatherModel( kernel::Controller& controller, const kernel::CoordinateConverter_ABC& converter )
    : controller_   ( controller )
    , converter_    ( converter )
    , time_         ( QDateTime::currentDateTime() )
    , sunrise_      (  7, 30 )
    , sunset_       ( 21, 30 )
    , lighting_     ( eLightingJourSansNuage )
    , globalWeather_( new Weather() )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherModel destructor
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
WeatherModel::~WeatherModel()
{
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
    globalWeather_.reset( new Weather() );
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::Load
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
void WeatherModel::Load( const std::string& filename )
{
    Purge();
    xml::xifstream xis( filename );
    xis >> start( "weather" )
        >> start( "exercise-date" );
    ReadExerciseDate( xis );
    xis >> end()
        >> start( "ephemerides" );
    ReadEphemerides( xis );
    xis >> end()
        >> start( "theater" );
    ReadGlobalWeather( xis );
    xis >> end()
        >> start( "local-weather" )
            >> list( "local", *this, &WeatherModel::ReadLocalWeather )
        >> end();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::Serialize
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherModel::Serialize( const std::string& filename ) const
{
    xml::xofstream xos( filename, xml::encoding( "ISO-8859-1" ) );
    xos << start( "weather" )
            << start( "exercise-date" )
                << attribute( "value", time_.toString( "yyyyMMddThhmmss" ).ascii() )
            << end()
            << start( "ephemerides" )
                << attribute( "sunrise", QString( "%1h%2m%3s" ).arg( sunrise_.hour() ).arg( sunrise_.minute() ).arg( sunrise_.second() ).ascii() )
                << attribute( "sunset", QString( "%1h%2m%3s" ).arg( sunset_.hour() ).arg( sunset_.minute() ).arg( sunset_.second() ).ascii() )
                << attribute( "moon", tools::GetXmlSection( lighting_ ) ) // $$$$ SBO 2006-12-20: !!
            << end()
            << start( "theater" );
    globalWeather_->Serialize( xos );
    xos     << end()
            << start( "local-weather" );
    Iterator< const LocalWeather& > it( CreateIterator() );
    while( it.HasMoreElements() )
    {
        xos << start( "local" );
        it.NextElement().Serialize( xos );
        xos << end();
    }
    xos     << end()
        << end();
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

    // $$$$ SBO 2006-12-19: !!
    kernel::E_LightingType ConvertToLighting( const QString& lighting )
    {
        for( unsigned int i = 0; i < (int)eNbrLightingType; ++i )
            if( QString( tools::GetXmlSection( (E_LightingType)i ) ) == lighting )
                return (E_LightingType)i;
        return (E_LightingType)-1;
    }
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::ReadExerciseDate
// Created: AGE 2007-10-12
// -----------------------------------------------------------------------------
void WeatherModel::ReadExerciseDate( xml::xistream& xis )
{
    // $$$$ AGE 2007-10-12: 
    const std::string isoDate = attribute< std::string >( xis, "value" );
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
    std::string sunrise, sunset, lighting;
    xis >> attribute( "sunrise", sunrise )
        >> attribute( "sunset", sunset )
        >> attribute( "moon", lighting );
    sunrise_ = MakeTime( sunrise.c_str() );
    sunset_ = MakeTime( sunset.c_str() );
    lighting_ = ConvertToLighting( lighting.c_str() );
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::ReadGlobalWeather
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
void WeatherModel::ReadGlobalWeather( xml::xistream& xis )
{
    globalWeather_.reset( new Weather( xis ) );
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::ReadLocalWeather
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
void WeatherModel::ReadLocalWeather( xml::xistream& xis )
{
    LocalWeather* local = new LocalWeather( xis, converter_ );
    Register( local->GetId(), *local );
}
