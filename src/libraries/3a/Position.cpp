// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "Position.h"
#include <geocoord/PlanarCartesian.h>
#include <geocoord/MGRS.h>
#include <geocoord/Geodetic.h>
#include <boost/bind.hpp>
#include "protocol/protocol.h"

using namespace Common;

namespace
{
    bool Initialize( geocoord::PlanarCartesian::Parameters& parameters, const MsgCoordLatLong& coord  )
    {
        const double rPiOver180 = std::acos( -1. ) / 180.;
        parameters.SetOrigin( coord.latitude() * rPiOver180, coord.longitude() * rPiOver180);
        return true;
    }
    geometry::Point2f ToPoint( const MsgCoordLatLong& coord )
    {
        static geocoord::Geodetic geo_;
        static geocoord::PlanarCartesian::Parameters parameters_;
        static bool bInit = Initialize( parameters_, coord );
        static geocoord::PlanarCartesian planar_( parameters_ ) ;
        static const double rPiOver180 = std::acos( -1. ) / 180.;

        geo_.Set( coord.latitude() * rPiOver180, coord.longitude() * rPiOver180 );
        planar_.SetCoordinates( geo_ );
        return geometry::Point2f( float( planar_.GetX() ), float( planar_.GetY() ) );
    }
    std::string ToMgrs( const MsgCoordLatLong& coord )
    {
        static geocoord::Geodetic geo_;
        static geocoord::MGRS mgrs_;
        static const double rPiOver180 = std::acos( -1. ) / 180.;

        geo_.Set( coord.latitude() * rPiOver180, coord.longitude() * rPiOver180 );
        mgrs_.SetCoordinates( geo_ );
        return mgrs_.GetString();
    }

    MsgCoordLatLong ToCoord( const std::string& mgrs )
    {
        static geocoord::Geodetic geo_;
        static geocoord::MGRS mgrs_;
        static const double rPiOver180 = std::acos( -1. ) / 180.;

        mgrs_.SetString( mgrs );
        geo_.SetCoordinates( mgrs_ );
        MsgCoordLatLong result;
        result.set_latitude( geo_.GetLatitude() / rPiOver180 );
        result.set_longitude( geo_.GetLongitude() / rPiOver180 );
        return result;
    }
}

// -----------------------------------------------------------------------------
// Name: Position constructor
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
Position::Position()
    : init_( true )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Position constructor
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
Position::Position( const MsgCoordLatLong& coord )
    : coord_( coord )
    , init_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Position constructor
// Created: AGE 2008-04-03
// -----------------------------------------------------------------------------
Position::Position( const std::string& coord )
    : coord_( ToCoord( coord ) )
    , init_( false )
{
    // NOTHING
}


// -----------------------------------------------------------------------------
// Name: Position destructor
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
Position::~Position()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Position::Distance
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
float Position::Distance( const Position& rhs ) const
{
    return Point().Distance( rhs.Point() );
}

// -----------------------------------------------------------------------------
// Name: Position::operator<
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
bool Position::operator<( const Position& rhs ) const
{
    return coord_.latitude() < rhs.coord_.latitude()
        || ( coord_.latitude() == rhs.coord_.latitude() && coord_.longitude() < rhs.coord_.longitude() );
}

// -----------------------------------------------------------------------------
// Name: Position::operator>
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
bool Position::operator>( const Position& rhs ) const
{
    return coord_.latitude() > rhs.coord_.latitude()
        || ( coord_.latitude() == rhs.coord_.latitude() && coord_.longitude() > rhs.coord_.longitude() );
}

// -----------------------------------------------------------------------------
// Name: Position::operator==
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
bool Position::operator==( const Position& rhs ) const
{
    return coord_.latitude() == rhs.coord_.latitude()
        && coord_.longitude() == rhs.coord_.longitude();
}

// -----------------------------------------------------------------------------
// Name: Position::operator!=
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
bool Position::operator!=( const Position& rhs ) const
{
    return coord_.latitude() != rhs.coord_.latitude()
        || coord_.longitude() != rhs.coord_.longitude();
}

// -----------------------------------------------------------------------------
// Name: Position::operator double
// Created: AGE 2007-10-08
// -----------------------------------------------------------------------------
Position::operator double() const
{
    return Point().Distance( geometry::Point2f() );
}

// -----------------------------------------------------------------------------
// Name: Position::Point
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
const geometry::Point2f& Position::Point() const
{
    if( ! init_ )
    {
        point_ = ToPoint( coord_ );
        init_ = true;
    }
    return point_;
}

// -----------------------------------------------------------------------------
// Name: Position::ToPolygon
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
geometry::Polygon2f Position::ToPolygon( const std::vector< Position >& positions )
{
    std::vector< geometry::Point2f > points;
    std::transform( positions.begin(), positions.end(), std::back_inserter( points ),
                        boost::bind( &Position::Point, _1 ) );
    return geometry::Polygon2f( points );
}

// -----------------------------------------------------------------------------
// Name: Position::IsInside
// Created: AGE 2007-10-09
// -----------------------------------------------------------------------------
bool Position::IsInside( const geometry::Polygon2f& polygon ) const
{
    return polygon.IsInside( Point() );
}

std::ostream& operator<<( std::ostream& os, const Position& position )
{
    return os << ToMgrs( position.coord_ );
}

std::istream& operator>>( std::istream& is, Position& position )
{
    position.init_ = false;
    std::string mgrs;
    is >> mgrs;
    position.coord_ = ToCoord( mgrs );
    return is;
}
