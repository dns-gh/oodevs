    // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "CoordinateConverter.h"
#include "Tools.h"
#include "tools/ExerciseConfig.h"
#include "ENT/ENT_Enums.h"
#include <geocoord/Geoid.h>
#include <geocoord/Datums.h>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: CoordinateConverter constructor
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
CoordinateConverter::CoordinateConverter()
    : planar_ ( parameters_ )
    , defaultCoordSystem_( eCoordinateSystem_Mgrs )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter constructor
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
CoordinateConverter::CoordinateConverter( E_CoordinateSystem projection )
    : planar_ ( parameters_ )
    , defaultCoordSystem_( projection )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter constructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
CoordinateConverter::CoordinateConverter( const tools::ExerciseConfig& config )
    : planar_ ( parameters_ )
    , defaultCoordSystem_( eCoordinateSystem_Mgrs )
{
    Load( config );
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::Load
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void CoordinateConverter::Load( const tools::ExerciseConfig& config )
{
    extent_ = geometry::Rectangle2f( 0, 0, config.GetTerrainWidth(), config.GetTerrainHeight() );
    translation_ = geometry::Vector2f( config.GetTerrainWidth() * 0.5f, config.GetTerrainHeight() * 0.5f );
    const double rPiOver180 = std::acos( -1. ) / 180.;
    parameters_.SetOrigin( config.GetTerrainLatitude() * rPiOver180, config.GetTerrainLongitude() * rPiOver180 );
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter destructor
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
CoordinateConverter::~CoordinateConverter()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::ConvertToMgrs
// Created: SBO 2006-03-14
// -----------------------------------------------------------------------------
std::string CoordinateConverter::ConvertToMgrs( const geometry::Point2f& pos ) const
{
    const geometry::Point2f translated = pos - translation_;
    planar_.Set( translated.X(), translated.Y() );
    mgrs_.SetCoordinates( planar_ );
    return mgrs_.GetString();
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::ConvertToXY
// Created: SBO 2006-08-23
// -----------------------------------------------------------------------------
geometry::Point2f CoordinateConverter::ConvertToXY( const std::string& mgrs ) const
{
    mgrs_.SetString( mgrs );
    planar_.SetCoordinates( mgrs_ );
    geometry::Point2f pos( float( planar_.GetX() ), float( planar_.GetY() ) );
    return pos + translation_;
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::ConvertToGeo
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
geometry::Point2d CoordinateConverter::ConvertToGeo( const geometry::Point2f& pos ) const
{
    static const double r180OverPi = 180. / std::acos( -1. );
    SetGeodeticCoordinates( pos );
    geometry::Point2d result;
    result.Set( geodetic_.GetLongitude() * r180OverPi, geodetic_.GetLatitude() * r180OverPi );
    return result;
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::ConvertToGeo
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
geometry::Point2d CoordinateConverter::ConvertToGeo( const geometry::Point2d& pos ) const
{
    static const double r180OverPi = 180. / std::acos( -1. );
    const geometry::Point2d translated = pos - geometry::Vector2d( translation_.X(), translation_.Y() );
    planar_.Set( translated.X(), translated.Y() );
    geodetic_.SetCoordinates( planar_ );
    return geometry::Point2d( geodetic_.GetLongitude() * r180OverPi, geodetic_.GetLatitude() * r180OverPi );
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::ConvertFromGeo
// Created: AGE 2007-09-04
// -----------------------------------------------------------------------------
geometry::Point2f CoordinateConverter::ConvertFromGeo( const geometry::Point2d& pos ) const
{
    static const float rPiOver180 = std::acos( -1.f ) / 180.f;
    geodetic_.Set( pos.Y() * rPiOver180, pos.X() * rPiOver180 );
    planar_.SetCoordinates( geodetic_ );
    return geometry::Point2f( float( planar_.GetX() ), float( planar_.GetY() ) ) + translation_;
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::IsInBoundaries
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
bool CoordinateConverter::IsInBoundaries( const geometry::Point2f& point ) const
{
    return extent_.IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::ConvertToGeoDms
// Created: AME 2010-02-22
// -----------------------------------------------------------------------------
std::string CoordinateConverter::ConvertToGeoDms( const geometry::Point2f& pos ) const
{
    SetGeodeticCoordinates( pos );
    const std::string strFormat = tools::translate( "CoordinateConverter", "DD° MM' SS.SS H" ).toStdString();
    return geodetic_.GetLatitude( strFormat ) + ":" + geodetic_.GetLongitude( strFormat );
}

namespace
{
    const geocoord::Datum& Find( const std::string& code )
    {
        const geocoord::Datum* datum = geocoord::Datums::Find( code );
        if( datum )
            return *datum;
        throw MASA_EXCEPTION( "unable to find coordinate datum " + code );
    }
}

std::string CoordinateConverter::ConvertToUtm( const geometry::Point2f& p, const std::string& code ) const
{
    const geometry::Point2f translated = p - translation_;
    planar_.Set( translated.X(), translated.Y() );
    // 31Q5000002000000
    // the band (Q here) is actually part of MGRS and not UTM
    // see http://en.wikipedia.org/wiki/Universal_Transverse_Mercator#Latitude_bands
    const auto& datum = Find( code );
    const geocoord::MGRS mgrs( planar_, geocoord::MGRS::Parameters( datum ) ) ;
    const geocoord::UTM utm( planar_, geocoord::UTM::Parameters( datum ) );
    return boost::str( boost::format( "%d%s%06.0f%07.0f" )
        % utm.GetZone()
        % mgrs.GetString().substr( 2, 1 )
        % std::floor( utm.GetEasting() )
        % std::floor( utm.GetNorthing() ) );
}

geometry::Point2f CoordinateConverter::ConvertFromUtm( const std::string& pos, const std::string& code ) const
{
    // 31Q5000002000000
    const boost::regex regex( "(\\d\\d)([C-X])(\\d{6})(\\d{7})" );
    boost::smatch what;
    if( !boost::regex_match( pos, what, regex ) )
        throw MASA_EXCEPTION( "invalid UTM coordinate format for " + pos );
    const int zone = boost::lexical_cast< int >( what[ 1 ] );
    const char band = boost::lexical_cast< char >( what[ 2 ] );
    // bands from C to M are South and N to X are North
    // see http://en.wikipedia.org/wiki/Universal_Transverse_Mercator#Latitude_bands_2
    const auto hemisphere = band < 'N' ? geocoord::eSouth : geocoord::eNorth;
    const double easting = boost::lexical_cast< double >( what[ 3 ] );
    const double northing = boost::lexical_cast< double >( what[ 4 ] );
    planar_.SetCoordinates( geocoord::UTM( hemisphere, zone, northing, easting, geocoord::UTM::Parameters( Find( code ) ) ) );
    return geometry::Point2f( float( planar_.GetX() ), float( planar_.GetY() ) ) + translation_;
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::SetGeodeticCoordinates
// Created: AME 2010-02-23
// -----------------------------------------------------------------------------
void CoordinateConverter::SetGeodeticCoordinates( const geometry::Point2f& pos ) const
{
    const geometry::Point2f translated = pos - translation_;
    planar_.Set( translated.X(), translated.Y() );
    geodetic_.SetCoordinates( planar_ );
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::ConvertFromGeoDms
// Created: AME 2010-03-05
// -----------------------------------------------------------------------------
geometry::Point2f CoordinateConverter::ConvertFromGeoDms ( const std::string& longitude, const std::string& latitude ) const
{
    const std::string strFormat = tools::translate( "CoordinateConverter", "DD° MM' SS.SS H" ).toStdString();
    geodetic_.Set( latitude , longitude, strFormat, strFormat );
    planar_.SetCoordinates( geodetic_ );
    return geometry::Point2f( float( planar_.GetX() ), float( planar_.GetY() ) ) + translation_;
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::GetDefaultProjection
// Created: ABR 2014-02-27
// -----------------------------------------------------------------------------
E_CoordinateSystem CoordinateConverter::GetDefaultCoordinateSystem() const
{
    return defaultCoordSystem_;
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::SetDefaultCoordinateSystem
// Created: ABR 2014-02-28
// -----------------------------------------------------------------------------
void CoordinateConverter::SetDefaultCoordinateSystem( E_CoordinateSystem coordSystem )
{
    defaultCoordSystem_ = coordSystem;
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::GetStringPosition
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
std::string CoordinateConverter::GetStringPosition( const geometry::Point2f& position ) const
{
    return GetStringPosition( position, defaultCoordSystem_ );
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::GetStringPosition
// Created: LGY 2014-01-22
// -----------------------------------------------------------------------------
std::string CoordinateConverter::GetStringPosition( const geometry::Point2f& position, E_CoordinateSystem projection ) const
{
    switch( projection )
    {
    case eCoordinateSystem_Mgrs:
        return ConvertToMgrs( position );
    case eCoordinateSystem_SanC:
        return ConvertToUtm( position, "SAN-C" );
    case eCoordinateSystem_Wgs84Dd:
        {
            const geometry::Point2d pos( ConvertToGeo( position ) );
            return boost::str( boost::format( "%f:%f" ) % pos.Y() % pos.X() );
        }
    case eCoordinateSystem_Wgs84Dms:
        return ConvertToGeoDms( position );
    case eCoordinateSystem_Local:
        return boost::str( boost::format( "%f:%f" ) % position.Y() % position.X() );
    default:
        return "----";
    }
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::GetExtent
// Created: AME 2011-02-03
// -----------------------------------------------------------------------------
const geometry::Rectangle2f& CoordinateConverter::GetExtent() const
{
    return extent_;
}
