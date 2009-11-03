// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "UrbanObject.h"
#include "Model.h"
#include "ClientPublisher_ABC.h"
#include "ModelVisitor_ABC.h"

#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: UrbanObject constructor
// Created: SLG 2009-09-26
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( Model& model, const ASN1T_MsgUrbanCreation& msg )
: SimpleEntity<>( msg.oid, msg.name )
, model_                       ( model )
, strName_                     ( msg.name  )
, localisation_                ( msg.location )
{
  //  model_.Register( msg.oid, *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject destructor
// Created: SLG 2009-09-26
// -----------------------------------------------------------------------------
UrbanObject::~UrbanObject()
{
   // model_.urbanObjects_.Remove( GetId() );
}

/*#define MSG_ASN_CREATION( ASN, CLASS ) \
    if ( attributes.m.##ASN##Present ) \
    AddAttribute( new CLASS( model, attributes ) )
*/
// -----------------------------------------------------------------------------
// Name: UrbanObject::SendCreation
// Created: SLG 2009-09-27
// -----------------------------------------------------------------------------
void UrbanObject::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::UrbanCreation asn;

    asn().oid  = GetId(); 
    asn().name = strName_.c_str();
    localisation_.Send( asn().location );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::SendCreation
// Created: SLG 2009-09-27
// -----------------------------------------------------------------------------
void UrbanObject::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::SendCreation
// Created: SLG 2009-09-27
// -----------------------------------------------------------------------------
void UrbanObject::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void UrbanObject::Update( const ASN1T_MsgUrbanCreation& msg )
{
    //ApplyUpdate( msg );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::Accept
// Created: SLG 2009-06-20
// -----------------------------------------------------------------------------
void UrbanObject::Accept( ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::Display
// Created: SLG 2009-06-20
// -----------------------------------------------------------------------------
void UrbanObject::Display( kernel::Displayer_ABC& ) const
{
    // NOTHING
}
