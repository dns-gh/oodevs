// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "Position.h"
#include <geocoord/PlanarCartesian.h>
#include <geocoord/MGRS.h>
#include <geocoord/Geodetic.h>
#include <boost/bind.hpp>

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
Position::Position( const ASN1T_CoordUTM& coord )
    : mgrs_( (const char*)coord.data, 15 )
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

namespace 
{
    bool Initialize( geocoord::PlanarCartesian::Parameters& parameters, const std::string& base )
    {
        geocoord::MGRS mgrs( base );
        geocoord::Geodetic geodetic( mgrs );
        parameters.SetOrigin( geodetic.GetLatitude(), geodetic.GetLongitude() );
        return true;
    }
    geometry::Point2f ToPoint( const std::string& mgrs )
    {
        static geocoord::MGRS mgrs_;
        static geocoord::PlanarCartesian::Parameters parameters_;
        static bool bInit = Initialize( parameters_, mgrs );
        static geocoord::PlanarCartesian planar_( parameters_ );

        mgrs_.SetString( mgrs );
        planar_.SetCoordinates( mgrs_ );
        return geometry::Point2f( float( planar_.GetX() ), float( planar_.GetY() ) );
    }
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
    return mgrs_ < rhs.mgrs_;
}

// -----------------------------------------------------------------------------
// Name: Position::operator>
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
bool Position::operator>( const Position& rhs ) const
{
    return mgrs_ > rhs.mgrs_;
}

// -----------------------------------------------------------------------------
// Name: Position::operator==
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
bool Position::operator==( const Position& rhs ) const
{
    return mgrs_ == rhs.mgrs_;
}

// -----------------------------------------------------------------------------
// Name: Position::operator!=
// Created: AGE 2007-09-25
// -----------------------------------------------------------------------------
bool Position::operator!=( const Position& rhs ) const
{
    return mgrs_ != rhs.mgrs_;
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
        point_ = ToPoint( mgrs_ );
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
