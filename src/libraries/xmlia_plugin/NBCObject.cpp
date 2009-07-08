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

#include <xeumeuleu/xml.h>

using namespace plugins::xmlia;

// -----------------------------------------------------------------------------
// Name: NBCObject constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
NBCObject::NBCObject( xml::xistream& xis )
{
  xis >> xml::start( "mpia:AutreEvenementNRBC" )
        >> xml::attribute( "id", id_)
        >> xml::content( "mpia:Nom", type_ )
      >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: NBCObject constructor
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
NBCObject::NBCObject( dispatcher::Object& obj )
: Entity_ABC()
, type_( obj.GetType().GetType() )
, id_( obj.GetId() )
{
  
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
  xos << xml::start( "mpia:AutreEvenementNRBC" )
    << xml::attribute( "id", id_)
    << xml::content( "mpia:Nom", type_ )
    << xml::end();
}

// -----------------------------------------------------------------------------
// Name: NBCObject::Update
// Created: MGD 2009-06-12
// -----------------------------------------------------------------------------
void NBCObject::Update( dispatcher::Object& obj )
{
  type_ = obj.GetType().GetType();
  id_ =  obj.GetId();
}

