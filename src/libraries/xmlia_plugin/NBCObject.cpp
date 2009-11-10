// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "xmlia_plugin_pch.h"
#include "NBCObject.h"

#include "clients_kernel/ObjectType.h"
#include "dispatcher/Agent.h"
#include "dispatcher/Object.h"

#include "xmlia_plugin/Point.h"

#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: NBCObject constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
NBCObject::NBCObject( xml::xistream& xis )
{
  xis >> xml::start( "AutreEvenementNRBC" )
        >> xml::attribute( "id", id_)
        >> xml::content( "Nom", type_ )
        >> xml::start( "Localisation" )
          >> xml::list( "PointGeographique", *this, &NBCObject::ReadPoint, localisation_ )
        >> xml::end()
      >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: NBCObject ReadPoint
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void NBCObject::ReadPoint( xml::xistream& xis, std::vector< Point >& localisation )
{
  localisation.push_back( Point( xis ) );
}

// -----------------------------------------------------------------------------
// Name: NBCObject constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
NBCObject::NBCObject( dispatcher::Object& obj )
: Entity_ABC()
, type_( obj.strName_ )
, id_( obj.GetId() )
{
  ASN1T_Location asn;
  obj.localisation_.Send( asn );
  
  localisation_.reserve( asn.coordinates.n );
  for( unsigned int i = 0; i < asn.coordinates.n; ++i )
  {
    localisation_.push_back( Point( asn.coordinates.elem[i].latitude, asn.coordinates.elem[i].longitude ) );
  }
}

// -----------------------------------------------------------------------------
// Name: NBCObject destructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
NBCObject::~NBCObject()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NBCObject::Serialize
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void NBCObject::Serialize( xml::xostream& xos ) const
{
  xos << xml::start( "AutreEvenementNRBC" )
    << xml::attribute( "id", id_)
    << xml::content( "Nom", type_ )
    << xml::start( "Localisation" );
    for( std::vector< Point >::const_iterator it = localisation_.begin(); it != localisation_.end(); it++ )
    {
      it->Serialize( xos );
    }
  xos << xml::end()
    << xml::end();
}

// -----------------------------------------------------------------------------
// Name: NBCObject::Update
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void NBCObject::Update( dispatcher::Object& obj )
{
  type_ = obj.strName_;
  id_ =  obj.GetId();
}

