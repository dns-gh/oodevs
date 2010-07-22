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
#include "CapacityAttribute.h"
#include "ColorAttribute.h"
#include "UrbanObject.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: UrbanObject constructor
// Created: SLG 2009-09-26
// -----------------------------------------------------------------------------
UrbanObject::UrbanObject( Model_ABC& /*model*/, const MsgsSimToClient::MsgUrbanCreation& msg )
    : dispatcher::UrbanObject_ABC( msg.oid(), msg.name().c_str() )
    , strName_                   ( msg.name()  )
    , localisation_              ( msg.location() )
{
    Initialize( msg.attributes() );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject destructor
// Created: SLG 2009-09-26
// -----------------------------------------------------------------------------
UrbanObject::~UrbanObject()
{
    // NOTHING
}


#define MSG_ASN_CREATION( ASN, CLASS ) \
    if( attributes.has_##ASN##() ) \
    AddAttribute( new CLASS( attributes ) )

// -----------------------------------------------------------------------------
// Name: UrbanObject::Initialize
// Created: SLG 2009-09-26
// -----------------------------------------------------------------------------
void UrbanObject::Initialize( const MsgsSimToClient::MsgUrbanAttributes& attributes )
{
    MSG_ASN_CREATION( color       , ColorAttribute );
    MSG_ASN_CREATION( architecture, ArchitectureAttribute );
    MSG_ASN_CREATION( capacity    , CapacityAttribute );
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
void UrbanObject::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::UrbanUpdate asn;
    asn().set_oid( GetId() );
    std::for_each( attributes_.begin(), attributes_.end(),
        boost::bind( &UrbanObjectAttribute_ABC::Send, _1, boost::ref( *asn().mutable_attributes() ) ) );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::SendCreation
// Created: SLG 2009-09-27
// -----------------------------------------------------------------------------
void UrbanObject::SendDestruction( ClientPublisher_ABC& /*publisher*/ ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::Update
// Created: AGE 2007-04-12
// -----------------------------------------------------------------------------
void UrbanObject::DoUpdate( const MsgsSimToClient::MsgUrbanUpdate& msg )
{
    if( msg.has_attributes() )
        std::for_each( attributes_.begin(), attributes_.end(),
            boost::bind( &UrbanObjectAttribute_ABC::Update, _1, boost::cref( msg.attributes() ) ) );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::Accept
// Created: SLG 2009-06-20
// -----------------------------------------------------------------------------
void UrbanObject::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanObject::Display
// Created: PHC 2010-07-22
// -----------------------------------------------------------------------------
void UrbanObject::Display( kernel::Displayer_ABC& /*displayer*/ ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}
