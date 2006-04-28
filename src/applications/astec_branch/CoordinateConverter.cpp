// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "CoordinateConverter.h"
#include "geocoord/Geoid.h"

// -----------------------------------------------------------------------------
// Name: CoordinateConverter constructor
// Created: AGE 2005-03-14
// -----------------------------------------------------------------------------
CoordinateConverter::CoordinateConverter()
    : planar_( parameters_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::Load
// Created: AGE 2006-04-28
// -----------------------------------------------------------------------------
void CoordinateConverter::Load( const std::string& scipioXml )
{
    WorldParameters::Load( scipioXml );
    geocoord::Geoid::Instance().Initialize( geoid_ );

    extent_ = geometry::Rectangle2f( 0, 0, width_, height_ );
    translation_ = geometry::Vector2f( width_ * 0.5, height_ * 0.5 );
    const double rPiOver180 = std::acos( -1. ) / 180.;
    parameters_.SetOrigin( latitude_ * rPiOver180, longitude_ * rPiOver180 );
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
// Created: SBO 2006-03-14
// -----------------------------------------------------------------------------
geometry::Point2f CoordinateConverter::ConvertToXY( const ASN1T_CoordUTM& mgrs ) const
{
    const std::string coord( (const char*)( mgrs.data ), 15 );
    mgrs_.SetString( coord );
    planar_.SetCoordinates( mgrs_ );
    geometry::Point2f pos( planar_.GetX(), planar_.GetY() );
    return pos + translation_;
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::ConvertToGeo
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
geometry::Point2f CoordinateConverter::ConvertToGeo( const geometry::Point2f& pos ) const
{
    static const float r180OverPi = 180. / std::acos( -1. );
    const geometry::Point2f translated = pos - translation_;
    planar_.Set( translated.X(), translated.Y() );
    geodetic_.SetCoordinates( planar_ );
    geometry::Point2f result;
    result.Set( geodetic_.GetLongitude() * r180OverPi, geodetic_.GetLatitude()  * r180OverPi );
    return result;
}

// -----------------------------------------------------------------------------
// Name: CoordinateConverter::IsInBoundaries
// Created: SBO 2006-04-14
// -----------------------------------------------------------------------------
bool CoordinateConverter::IsInBoundaries( const geometry::Point2f& point ) const
{
    return extent_.IsInside( point );
}
