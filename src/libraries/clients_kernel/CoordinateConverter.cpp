    // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "CoordinateConverter.h"
#include "geocoord/Geoid.h"
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include "CoordinateSystems.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: CoordinateConverter constructor
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
CoordinateConverter::CoordinateConverter()
    : planar_( parameters_ )
    , coordinateSystems_( *new CoordinateSystems() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter constructor
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
CoordinateConverter::CoordinateConverter( const CoordinateSystems& coordSystems )
    : planar_( parameters_ )
    , coordinateSystems_( coordSystems )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter constructor
// Created: AGE 2008-04-01
// -----------------------------------------------------------------------------
CoordinateConverter::CoordinateConverter( const tools::ExerciseConfig& config )
    : planar_( parameters_ )
    , coordinateSystems_( *new CoordinateSystems() )
{
    Load( config );
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::Load
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void CoordinateConverter::Load( const tools::ExerciseConfig& config )
{
    world_.Load( config );
    extent_ = geometry::Rectangle2f( 0, 0, world_.width_, world_.height_ );
    translation_ = geometry::Vector2f( world_.width_ * 0.5f, world_.height_ * 0.5f );
    const double rPiOver180 = std::acos( -1. ) / 180.;
    parameters_.SetOrigin( world_.latitude_ * rPiOver180, world_.longitude_ * rPiOver180 );
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
    return std::string ( geodetic_.GetLatitude( "DD� MM' SS.SS H" ) + ":" + geodetic_.GetLongitude( "DD� MM' SS.SS H" ) );
}
// -----------------------------------------------------------------------------
// Name: CoordinateConverter::ConvertToUtm        
// Created: AME 2010-02-23
// -----------------------------------------------------------------------------
std::string CoordinateConverter::ConvertToUtm( const geometry::Point2f& pos ) const
{
    ConvertToMgrs( pos );
    mgrs_.GetCoordinates( utm_ );
    return boost::str( boost::format( "%d%s %dE %dN" ) % utm_.GetZone() 
                                                       % ( utm_.GetHemisphere() == geocoord::eNorth ? "N" : "S" )
                                                       % boost::lexical_cast< int >( utm_.GetNorthing() )
                                                       % boost::lexical_cast< int >( utm_.GetEasting() ) );
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
    geodetic_.Set( latitude , longitude,  "DD� MM' SS.SS H",  "DD� MM' SS.SS H" );
    planar_.SetCoordinates( geodetic_ );
    return geometry::Point2f( float( planar_.GetX() ), float( planar_.GetY() ) ) + translation_;
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::GetCoordSystem
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
const CoordinateSystems& CoordinateConverter::GetCoordSystem() const
{
    return coordinateSystems_;
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::GetStringPosition
// Created: AME 2010-03-15
// -----------------------------------------------------------------------------
std::string CoordinateConverter::GetStringPosition( const geometry::Point2f& position ) const
{
    std::string positionStr;    
    switch( coordinateSystems_.defaultCoordinateSystem_ )
    {
        case CoordinateSystems::E_Mgrs: positionStr = ConvertToMgrs( position ); break; 
        case CoordinateSystems::E_Wgs84Dd: 
            {
                geometry::Point2d pos( ConvertToGeo( position ) );
                positionStr = boost::str( boost::format( "%f:%f" )  % pos.Y()
                                                                    % pos.X() );
                break;
            }
        case CoordinateSystems::E_Wgs84Dms: positionStr = ConvertToGeoDms( position ); break;
        case CoordinateSystems::E_Local:
            positionStr = boost::str( boost::format( "%f:%f" ) % position.Y()
                                                               % position.X() );
             break;
        default: 
            positionStr = "----"; break;
    }

    return positionStr;
}
