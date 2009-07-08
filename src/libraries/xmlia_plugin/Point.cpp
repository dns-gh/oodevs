// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "Point.h"

#include "dispatcher/Agent.h"

#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Point::Point( xml::xistream& xis )
{
  xis >> xml::start( "mpia:PointGeographique" )
        >> xml::content( "mpia:Latitude", latitude_ )
        >> xml::content( "mpia:Latitude", longitude_ )
      >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Point::Point( const double& latitude, const double& longitude )
: Entity_ABC()
, latitude_( latitude )
, longitude_( longitude )
{}

// -----------------------------------------------------------------------------
// Name: Point destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Point::~Point()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Point::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Point::Serialize( xml::xostream& xos ) const
{
    xos << xml::start( "mpia:PointGeographique" )
          << xml::content( "mpia:Latitude" , latitude_ )
          << xml::content( "mpia:Longitude", longitude_ )
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Point::Update
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void Point::Update( dispatcher::Agent& agent )
{
  latitude_  = agent.position_.latitude;
  longitude_ = agent.position_.longitude;
}

// -----------------------------------------------------------------------------
// Name: Point::Update
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void Point::FillLatLong( ASN1T_CoordLatLong& utm )
{
    utm.latitude = ( ASN1REAL ) latitude_;
    utm.longitude = ( ASN1REAL ) longitude_;
}

