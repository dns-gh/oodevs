// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ArchitectureAttribute.h"
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "ColorAttribute.h"
#include "Model.h"
#include "UrbanObject.h"
#include "protocol/ClientSenders.h"

#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: UrbanObject constructor
// Created: SLG 2009-09-26
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( Model& model, const MsgsSimToClient::MsgUrbanCreation& msg )
    : SimpleEntity<>( msg.oid(), msg.name().c_str() )
    , model_        ( model )
    , strName_      ( msg.name()  )
    , localisation_ ( msg.location() )
{
    Initialize( model, msg.attributes() );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject destructor
// Created: SLG 2009-09-26
// -----------------------------------------------------------------------------
UrbanObject::~UrbanObject()
{
}


#define MSG_ASN_CREATION( ASN, CLASS ) \
    if ( attributes.has_##ASN##() ) \
    AddAttribute( new CLASS( model, attributes ) )

// -----------------------------------------------------------------------------
// Name: UrbanObject::Initialize
// Created: SLG 2009-09-26
// -----------------------------------------------------------------------------
void UrbanObject::Initialize( Model& model, const MsgsSimToClient::MsgUrbanAttributes& attributes )
{
    MSG_ASN_CREATION( color         , ColorAttribute );
    MSG_ASN_CREATION( architecture  , ArchitectureAttribute );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::AddAttribute
// Created: SLG 2009-09-26
// -----------------------------------------------------------------------------
void UrbanObject::AddAttribute( UrbanObjectAttribute_ABC* attribute )
{
    attributes_.push_back( attribute );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::SendCreation
// Created: SLG 2009-09-27
// -----------------------------------------------------------------------------
void UrbanObject::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::UrbanCreation asn;

    asn().set_oid( GetId() );
    asn().set_name( strName_ );
    localisation_.Send( *asn().mutable_location() );

    std::for_each( attributes_.begin(), attributes_.end(),
        boost::bind( &UrbanObjectAttribute_ABC::Send, _1, boost::ref( *asn().mutable_attributes() ) ) );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::SendCreation
// Created: SLG 2009-09-27
// -----------------------------------------------------------------------------
void UrbanObject::SendFullUpdate( ClientPublisher_ABC& /*publisher*/ ) const
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::SendCreation
// Created: SLG 2009-09-27
// -----------------------------------------------------------------------------
void UrbanObject::SendDestruction( ClientPublisher_ABC& /*publisher*/ ) const
{
    //TODO
}

// -----------------------------------------------------------------------------
// Name: Agent::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void UrbanObject::Update( const MsgsSimToClient::MsgUrbanCreation& msg )
{
    //ApplyUpdate( msg );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::Accept
// Created: SLG 2009-06-20
// -----------------------------------------------------------------------------
void UrbanObject::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}
