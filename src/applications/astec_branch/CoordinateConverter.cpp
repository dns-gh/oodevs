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
CoordinateConverter::CoordinateConverter( const std::string& scipioXml )
    : WorldParameters( scipioXml )
    , planar_( parameters_ )
{
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
