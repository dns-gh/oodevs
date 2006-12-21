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
#include "clients_kernel/PathTools.h"
#include "clients_kernel/Tools.h"
#include "xeumeuleu/xml.h"
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
    , sunset_       ()
    , sunrise_      ()
    , lighting_     ( (kernel::E_LightingType)-1 )
    , globalWeather_( 0 )
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
    delete globalWeather_;
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::Load
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
void WeatherModel::Load( const std::string& filename )
{
    Purge();
    xml::xifstream xis( filename );
    xis >> start( "Meteo" )
        >> start( "Ephemeride" );
    ReadEphemerides( xis );
    xis >> end()
        >> start( "MeteoGlobale" );
    ReadGlobalWeather( xis );
    xis >> end()
        >> start( "PatchsLocaux" )
            >> list( "PatchLocal", *this, &WeatherModel::ReadLocalWeather )
        >> end();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: WeatherModel::Serialize
// Created: SBO 2006-12-20
// -----------------------------------------------------------------------------
void WeatherModel::Serialize( xml::xostream& xos ) const
{
    xos << start( "Meteo" )
            << start( "Ephemeride" )
                << content( "HeureLeverSoleil", QString( "%1h%2m%3s" ).arg( sunrise_.hour() ).arg( sunrise_.minute() ).arg( sunrise_.second() ).ascii() )
                << content( "HeureCoucherSoleil", QString( "%1h%2m%3s" ).arg( sunset_.hour() ).arg( sunset_.minute() ).arg( sunset_.second() ).ascii() )
                << content( "Lune", tools::GetXmlSection( lighting_ ) ) // $$$$ SBO 2006-12-20: !!
            << end()
            << start( "MeteoGlobale" );
    globalWeather_->Serialize( xos );
    xos     << end()
            << start( "PatchsLocaux" );
    Iterator< const LocalWeather& > it( CreateIterator() );
    while( it.HasMoreElements() )
    {
        xos << start( "PatchLocal" );
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
// Name: WeatherModel::ReadEphemerides
// Created: SBO 2006-12-19
// -----------------------------------------------------------------------------
void WeatherModel::ReadEphemerides( xml::xistream& xis )
{
    std::string sunrise, sunset, lighting;
    xis >> content( "HeureLeverSoleil", sunrise )
        >> content( "HeureCoucherSoleil", sunset )
        >> content( "Lune", lighting );
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
    globalWeather_ = new Weather( xis );
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
