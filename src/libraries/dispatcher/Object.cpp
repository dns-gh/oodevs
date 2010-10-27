// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
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
{
    Initialize( model, msg.attributes() );
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

#define MSG_ASN_CREATION( ASN, CLASS ) \
    if( attributes.has_##ASN##()  ) \
        AddAttribute( new CLASS( attributes ) )

// -----------------------------------------------------------------------------
// Name: Object::Initialize
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void Object::Initialize( Model_ABC& model, const Common::ObjectAttributes& attributes )
{
    MSG_ASN_CREATION( construction      , ConstructionAttribute );
    MSG_ASN_CREATION( obstacle          , ObstacleAttribute );
    MSG_ASN_CREATION( mine              , MineAttribute );
    if( attributes.has_logistic() )
        AddAttribute( new LogisticAttribute( model, attributes ) );
    MSG_ASN_CREATION( bypass            , BypassAttribute );
    MSG_ASN_CREATION( crossing_site     , CrossingSiteAttribute );
    MSG_ASN_CREATION( supply_route      , SupplyRouteAttribute );
    MSG_ASN_CREATION( nbc               , NBCAttribute );
    MSG_ASN_CREATION( toxic_cloud       , ToxicCloudAttribute );
    MSG_ASN_CREATION( fire              , FireAttribute );
    MSG_ASN_CREATION( medical_treatment , MedicalTreatmentAttribute );
    MSG_ASN_CREATION( effect_delay      , DelayAttribute );
    MSG_ASN_CREATION( resource_networks , ResourceNetworkAttribute );
}

// -----------------------------------------------------------------------------
// Name: Object::AddAttribute
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void Object::AddAttribute( ObjectAttribute_ABC* attribute )
{
    attributes_.push_back( attribute );
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
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Update, _1, boost::cref( msg.attributes() ) ) );
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
    Send( *asn().mutable_attributes() );
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
    Send( *asn().mutable_attributes() );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Object::Send
// Created: JCR 2010-10-08
// -----------------------------------------------------------------------------
void Object::Send( Common::ObjectAttributes& attributes ) const
{
    std::for_each( attributes_.begin(), attributes_.end(),
        boost::bind( &ObjectAttribute_ABC::Send, _1, boost::ref( attributes ) ) );
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
