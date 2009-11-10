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
#include "ModelVisitor_ABC.h"
#include "clients_kernel/ObjectType.h"

#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Object constructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Object::Object( Model& model, const ASN1T_MsgObjectCreation& msg )
    : SimpleEntity< kernel::Object_ABC >( msg.oid, msg.name )
    , type_                        ( model.GetObjectTypes().Get( msg.type ) )
    , strName_                     ( msg.name  )
    , localisation_                ( msg.location )
    , side_                        ( model.sides_.Get( msg.team ) )
{
    Initialize( model, msg.attributes );
	side_.objects_.Register( msg.oid, *this );
}

// -----------------------------------------------------------------------------
// Name: Object destructor
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
Object::~Object()
{
	side_.objects_.Remove( GetId() );
}

#define MSG_ASN_CREATION( ASN, CLASS ) \
    if ( attributes.m.##ASN##Present ) \
        AddAttribute( new CLASS( model, attributes ) )

// -----------------------------------------------------------------------------
// Name: Object::Initialize
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void Object::Initialize( Model& model, const ASN1T_ObjectAttributes& attributes )
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
void Object::Update( const ASN1T_MsgObjectUpdate& msg )
{
    if( msg.m.locationPresent )
        localisation_.Update( msg.location );
    
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Update, _1, boost::cref( msg.attributes ) ) );
}

// -----------------------------------------------------------------------------
// Name: Object::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void Object::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::ObjectCreation asn;

    asn().oid  = GetId();
    asn().type = type_.GetType().c_str();
    asn().name = strName_.c_str();
    asn().team = side_.GetId();

    localisation_.Send( asn().location );

    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Send, _1, boost::ref( asn().attributes ) ) );

    asn.Send( publisher );

//    if( pAttributes_ )
//        pAttributes_->AsnDelete( asn().specific_attributes );
}

// -----------------------------------------------------------------------------
// Name: Object::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void Object::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::ObjectUpdate asn;

    asn().oid = GetId();

    asn().m.locationPresent = 1;
    localisation_.Send( asn().location );

    std::for_each( attributes_.begin(), attributes_.end(),
        boost::bind( &ObjectAttribute_ABC::Send, _1, boost::ref( asn().attributes ) ) );

    asn.Send( publisher );

//    if( pAttributes_ )
//        pAttributes_->AsnDelete( asn().specific_attributes );
}

// -----------------------------------------------------------------------------
// Name: Object::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void Object::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::ObjectDestruction destruction;
    destruction() = GetId();
    destruction.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: Object::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void Object::Accept( ModelVisitor_ABC& visitor ) const
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
