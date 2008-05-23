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
#include <xeumeuleu/xml.h>
#include <cmath>

using namespace bml;

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Point::Point( xml::xistream& xis )
{
    std::string type;
    xis >> xml::start( "AbstractAbsolutePoint" )
            >> xml::attribute( "xsi:type", type_ );
    if( type_ == "jc3iedm:GeographicPoint" )
        xis >> xml::content( "jc3iedm:LatitudeCoordinate" , latitude_ )
            >> xml::content( "jc3iedm:LongitudeCoordinate", longitude_ );
    else if( type_ == "jc3iedm:CartesianPoint" )
        xis >> xml::content( "jc3iedm:XCoordinateDimension", latitude_ )
            >> xml::content( "jc3iedm:YCoordinateDimension", longitude_ )
            >> xml::content( "jc3iedm:ZCoordinateDimension", height_ );
    xis >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: SBO 2008-05-22
// -----------------------------------------------------------------------------
Point::Point( double latitude, double longitude )
    : latitude_( latitude )
    , longitude_( longitude )
    , height_( 0 )
    , type_( "jc3iedm:GeographicPoint" )
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
void Point::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "AbstractAbsolutePoint" )
            << xml::attribute( "xsi:type", type_ );
    if( type_ == "jc3iedm:GeographicPoint" )
        xos << xml::content( "jc3iedm:LatitudeCoordinate" , latitude_ )
            << xml::content( "jc3iedm:LongitudeCoordinate", longitude_ );
    else if( type_ == "jc3iedm:CartesianPoint" )
        xos << xml::content( "jc3iedm:XCoordinateDimension", latitude_ )
            << xml::content( "jc3iedm:YCoordinateDimension", longitude_ )
            << xml::content( "jc3iedm:ZCoordinateDimension", height_ );
    xos << xml::end();
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
