// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "bml_plugin_pch.h"
#include "Point.h"
#include "SerializationTools.h"
#include <xeumeuleu/xml.h>
#include <cmath>

using namespace plugins::bml;

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2008-05-23
// -----------------------------------------------------------------------------
Point::Point()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Point::Point( xml::xistream& xis, const std::string& fieldname )
{
    xis >> xml::start( fieldname )
            >> xml::content( NS( "LatitudeCoordinate" , "jc3iedm" ), latitude_ )
            >> xml::content( NS( "LongitudeCoordinate", "jc3iedm" ), longitude_ )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Point::Point( double latitude, double longitude )
    : latitude_( latitude )
    , longitude_( longitude )
    , height_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point destructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Point::~Point()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point::Serialize
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Point::Serialize( xml::xostream& xos, const std::string& fieldname ) const
{
    xos << xml::start( fieldname )
            << xml::attribute( "xsi:type", "jc3iedm:GeographicPoint" )
            << xml::content( "jc3iedm:LatitudeCoordinate" , latitude_ )
            << xml::content( "jc3iedm:LongitudeCoordinate", longitude_ )
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Point::Serialize
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
void Point::Serialize( ASN1T_CoordLatLong& asn ) const
{
    asn.latitude  = latitude_;
    asn.longitude = longitude_;
}

// -----------------------------------------------------------------------------
// Name: Point::ComputeBearing
// Created: SBO 2008-05-23
// -----------------------------------------------------------------------------
unsigned short Point::ComputeBearing( const Point& rhs ) const
{
    const double rad = std::atan2( rhs.latitude_ - latitude_, rhs.longitude_ - longitude_ );
    const short  deg = short( rad * 180. / std::acos( -1. ) );
    return deg < 0 ? 360 + deg : deg;
}
