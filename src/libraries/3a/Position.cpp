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
#include <geometry/Types.h>

// -----------------------------------------------------------------------------
// Name: Position constructor
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
Position::Position()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Position constructor
// Created: AGE 2007-09-12
// -----------------------------------------------------------------------------
Position::Position( const ASN1T_CoordUTM& coord )
    : mgrs_( (const char*)coord.data, 15 )
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
    return ToPoint( mgrs_ ).Distance( ToPoint( rhs.mgrs_ ) );
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