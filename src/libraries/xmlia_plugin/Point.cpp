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
   //xis >> xml::start( "mpia:PointGeographique" )
       xis >> xml::attribute( "id", sQname_ )
         >> xml::content( "mpia:Latitude", latitude_ )
         >> xml::content( "mpia:Latitude", longitude_ );
     //  >> xml::end();
   std::string sId = sQname_.substr( 5 );
   std::istringstream is( sId );
   is >> id_;
}

// -----------------------------------------------------------------------------
// Name: Point constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
Point::Point( const double& latitude, const double& longitude, unsigned idParent )
: Entity_ABC()
, latitude_( latitude )
, longitude_( longitude )
{
  std::ostringstream os;
  os << idParent;
  std::string sId = os.str();
  sQname_ = "loca-" + sId;
}

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
void Point::Serialize( xml::xostream& xos, const std::string& sQnameRapport ) const
{
    xos << xml::start( "mpia:PointGeographique" )
          << xml::attribute( "id", QName() )
          << xml::content( "mpia:Latitude" , latitude_ )
          << xml::content( "mpia:Longitude", longitude_ )
        << xml::end();
}

// -----------------------------------------------------------------------------
// Name: Point::QName
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
std::string Point::QName() const
{
  return sQname_;
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

}

// -----------------------------------------------------------------------------
// Name: Point::Update
// Created: RPD 2009-06-12
// -----------------------------------------------------------------------------
void Point::FillLatLong( ASN1T_CoordLatLong& utm )
{
    utm.latitude = ( ASN1REAL ) latitude_;
    utm.longitude = ( ASN1REAL ) longitude_;
// -----------------------------------------------------------------------------
// Name: Point GetId
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
unsigned int Point::GetId() const
{
  return id_;
}}
