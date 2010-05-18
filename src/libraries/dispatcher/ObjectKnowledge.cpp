// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Automat.h"
#include "BypassAttribute.h"
#include "ConstructionAttribute.h"
#include "CrossingSiteAttribute.h"
#include "FireAttribute.h"
#include "LogisticAttribute.h"
#include "KnowledgeGroup.h"
#include "MedicalTreatmentAttribute.h"
#include "MineAttribute.h"
#include "Model.h"
#include "NBCAttribute.h"
#include "ObjectKnowledge.h"
#include "ObstacleAttribute.h"
#include "Object.h"
#include "Side.h"
#include "SupplyRouteAttribute.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/clientsenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const Model& model, const MsgsSimToClient::MsgObjectKnowledgeCreation& asnMsg )
    : SimpleEntity< kernel::ObjectKnowledge_ABC >( asnMsg.oid() )
    , model_                        ( model )
    , team_                         ( model.Sides().Get( asnMsg.team() ) )
    , pObject_                      ( model.Objects().Find( asnMsg.real_object() ) )
    , nType_                        ( asnMsg.type() )
    , knowledgeGroup_               ( 0 )
    , localisation_                 ( )
    , bPerceived_                   ( false )
    , automatPerceptions_           ()    
{
    if( asnMsg.has_group() )
        knowledgeGroup_ = &model.KnowledgeGroups().Get( asnMsg.group() );
    Initialize( model, asnMsg.attributes() );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge destructor
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
ObjectKnowledge::~ObjectKnowledge()
{
    // NOTHING
}


#define CHECK_ASN_ATTRIBUTE_CREATION( ASN, CLASS ) \
    if ( attributes.has_##ASN##()  ) \
        AddAttribute( new CLASS( model, attributes ) )

// -----------------------------------------------------------------------------
// Name: Object::Initialize
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ObjectKnowledge::Initialize( const Model& model, const Common::MsgObjectAttributes& attributes )
{
    CHECK_ASN_ATTRIBUTE_CREATION( construction      , ConstructionAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( obstacle          , ObstacleAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( mine              , MineAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( logistic          , LogisticAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( bypass            , BypassAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( crossing_site     , CrossingSiteAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( supply_route      , SupplyRouteAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( nbc               , NBCAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( fire              , FireAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( medical_treatment , MedicalTreatmentAttribute );
}

// -----------------------------------------------------------------------------
// Name: Object::AddAttribute
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ObjectKnowledge::AddAttribute( ObjectAttribute_ABC* attribute )
{
    attributes_.push_back( attribute );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Update
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void ObjectKnowledge::Update( const MsgsSimToClient::MsgObjectKnowledgeCreation& message )
{
    bool realObjectChanged = ( message.real_object() && ! pObject_ )
                          || ( pObject_ && pObject_->GetId() != ( unsigned int )message.real_object() );
    if( realObjectChanged )
        pObject_ = model_.objects_.Find( message.real_object() );

    ApplyUpdate( message );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Update
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObjectKnowledge::Update( const MsgsSimToClient::MsgObjectKnowledgeUpdate& asnMsg )
{  
    if( asnMsg.has_location()  )
    {
        localisation_.Update( asnMsg.location() );
        optionals_.locationPresent = 1;
    }

    if( asnMsg.has_automat_perception()  )
    {
        optionals_.automat_perceptionPresent = 1;
        automatPerceptions_.clear();
        for( int i = 0; i < asnMsg.automat_perception().elem_size(); ++i )
            automatPerceptions_.push_back( &model_.automats_.Get( asnMsg.automat_perception().elem( i )) );
    }

    if( asnMsg.has_real_object()  )
        pObject_ = model_.objects_.Find( asnMsg.real_object() );

    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Update, _1, boost::cref( asnMsg.attributes() ) ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SendCreation
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObjectKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::ObjectKnowledgeCreation asn;

    asn().set_oid( GetId() );
    asn().set_team( team_.GetId() );
    if( knowledgeGroup_ )
        asn().set_group( knowledgeGroup_->GetId() );
    asn().set_real_object( pObject_ ? pObject_->GetId() : 0 );
    asn().set_type( nType_.c_str() );

    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Send, _1, boost::ref( *asn().mutable_attributes() ) ) );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObjectKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::ObjectKnowledgeUpdate asn;
    asn().set_oid( GetId() );
    asn().set_team( team_.GetId() );
    if( knowledgeGroup_ )
        asn().set_group( knowledgeGroup_->GetId() );
    asn().set_real_object( pObject_ ? pObject_->GetId() : 0 );

    if( asn().has_location() )
        localisation_.Send( *asn().mutable_location() );

    if( asn().has_automat_perception() )
    {
        unsigned int i = 0;
        for( std::vector< const kernel::Automat_ABC* >::const_iterator it = automatPerceptions_.begin(); it != automatPerceptions_.end(); ++it, ++i )
            asn().mutable_automat_perception()->set_elem( i, (*it)->GetId() );
    }
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Send, _1, boost::ref( *asn().mutable_attributes() ) ) );
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void ObjectKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::ObjectKnowledgeDestruction asn;
    asn().set_oid  ( GetId() );
    asn().set_team ( team_.GetId());
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void ObjectKnowledge::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetRecognizedEntity
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* ObjectKnowledge::GetRecognizedEntity() const
{
    return pObject_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetEntity
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::Object_ABC* ObjectKnowledge::GetEntity() const
{
    return pObject_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetOwner
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::Team_ABC& ObjectKnowledge::GetOwner() const
{
    return team_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Display
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void ObjectKnowledge::Display( kernel::Displayer_ABC& ) const
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::DisplayInList
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void ObjectKnowledge::DisplayInList( kernel::Displayer_ABC& ) const
{
    // NOTHING
}
