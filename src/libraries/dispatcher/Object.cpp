// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Object.h"
#include "Model.h"
#include "Side.h"
#include "ConstructionAttribute.h"
#include "DelayAttribute.h"
#include "ObstacleAttribute.h"
#include "BypassAttribute.h"
#include "CrossingSiteAttribute.h"
#include "MineAttribute.h"
#include "NBCAttribute.h"
#include "FireAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "SupplyRouteAttribute.h"
#include "LogisticAttribute.h"
#include "ToxicCloudAttribute.h"
#include "ResourceNetworkAttribute.h"
#include "protocol/ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "protocol/ClientSenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Object::Object( Model_ABC& model, const MsgsSimToClient::MsgObjectCreation& msg, const tools::Resolver_ABC< kernel::ObjectType, std::string >& types )
    : dispatcher::Object_ABC( msg.object().id(), QString( msg.name().c_str() ) )
    , type_                 ( types.Get( msg.type().id() ) )
    , localisation_         ( msg.location() )
    , side_                 ( model.Sides().Get( msg.party().id() ) )
    , attributes_           ( model )
{
    side_.Register( *this );
    RegisterSelf( *this );
    optionals_.localisationPresent = 0;
}

// -----------------------------------------------------------------------------
// Name: Object destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Object::~Object()
{
    side_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: Object::DoUpdate
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Object::DoUpdate( const MsgsSimToClient::MsgObjectUpdate& msg )
{
    if( msg.has_location() )
    {
        localisation_.Update( msg.location() );
        optionals_.localisationPresent = 1;
    }
    attributes_.Update( msg.attributes() );
}

// -----------------------------------------------------------------------------
// Name: Object::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Object::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::ObjectCreation asn;
    asn().mutable_object()->set_id( GetId() );
    asn().mutable_type()->set_id( type_.GetType().c_str() );
    asn().set_name( GetName() );
    asn().mutable_party()->set_id( side_.GetId() );
    localisation_.Send( *asn().mutable_location() );
    asn().mutable_attributes(); //$$$$ NLD 2010-10-26 - A VIRER quand vir� dans le protocole ... le message de creation ne doit PAS envoyer les attributs
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Object::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Object::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::ObjectUpdate asn;
    asn().mutable_object()->set_id( GetId() );
    if( optionals_.localisationPresent )
        localisation_.Send( *asn().mutable_location() );
    attributes_.Send( *asn().mutable_attributes() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Object::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Object::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::ObjectDestruction destruction;
    destruction().mutable_object()->set_id( GetId() );
    destruction.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Object::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Object::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: Object::Display
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Object::Display( kernel::Displayer_ABC& ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: Object::GetType
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::ObjectType& Object::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: Object::GetTeam
// Created: SBO 2010-06-07
// -----------------------------------------------------------------------------
const dispatcher::Team_ABC& Object::GetTeam() const
{
    return side_;
}
