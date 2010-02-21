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
#include "ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "protocol/clientsenders.h"

#include <boost/bind.hpp>

using namespace dispatcher;
//using namespace Common;
//using namespace MsgsSimToClient;

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Object::Object( Model& model, const MsgsSimToClient::MsgObjectCreation& msg )
    : SimpleEntity< kernel::Object_ABC >( msg.oid(), QString( msg.name().c_str() ) )
    , type_                        ( model.GetObjectTypes().Get( msg.type() ) )
    , strName_                     ( msg.name()  )
    , localisation_                ( msg.location() )
    , side_                        ( model.sides_.Get( msg.team() ) )
{
    Initialize( model, msg.attributes() );
	side_.Register( *this );
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
    if ( attributes.has_##ASN##()  ) \
        AddAttribute( new CLASS( model, attributes ) )

// -----------------------------------------------------------------------------
// Name: Object::Initialize
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void Object::Initialize( Model& model, const Common::MsgObjectAttributes& attributes )
{
    MSG_ASN_CREATION( construction      , ConstructionAttribute );
    MSG_ASN_CREATION( obstacle          , ObstacleAttribute );
    MSG_ASN_CREATION( mine              , MineAttribute );
    MSG_ASN_CREATION( logistic          , LogisticAttribute );
    MSG_ASN_CREATION( bypass            , BypassAttribute );
    MSG_ASN_CREATION( crossing_site     , CrossingSiteAttribute );
    MSG_ASN_CREATION( supply_route      , SupplyRouteAttribute );
    MSG_ASN_CREATION( nbc               , NBCAttribute );
    MSG_ASN_CREATION( toxic_cloud       , ToxicCloudAttribute );
    MSG_ASN_CREATION( fire              , FireAttribute );
    MSG_ASN_CREATION( medical_treatment , MedicalTreatmentAttribute );
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
// Name: Object::Update
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
void Object::Update( const MsgsSimToClient::MsgObjectUpdate& msg )
{
    if( msg.has_location()  )
        localisation_.Update( msg.location() );
    
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

    asn().set_oid( GetId() );
    asn().set_type( type_.GetType().c_str() );
    asn().set_name( strName_.c_str() );
    asn().set_team( side_.GetId() );

    localisation_.Send( *asn().mutable_location() );

    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Send, _1, boost::ref( *asn().mutable_attributes() ) ) );
    asn.Send( publisher );
    
//    if( pAttributes_ )
//        pAttributes_->Delete( asn().specific_attributes );
}

// -----------------------------------------------------------------------------
// Name: Object::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Object::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::ObjectUpdate asn;

    asn().set_oid( GetId() );
    localisation_.Send( *asn().mutable_location() );

    std::for_each( attributes_.begin(), attributes_.end(),
        boost::bind( &ObjectAttribute_ABC::Send, _1, boost::ref( *asn().mutable_attributes() ) ) );

    asn.Send( publisher );

//    if( pAttributes_ )
//        pAttributes_->Delete( asn.specific_attributes );
}

// -----------------------------------------------------------------------------
// Name: Object::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Object::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::ObjectDestruction destruction;
    destruction().set_oid( GetId() );
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
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Object::GetType
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::ObjectType& Object::GetType() const
{
	return type_;
}
