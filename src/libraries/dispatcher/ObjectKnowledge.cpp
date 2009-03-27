// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "ObjectKnowledge.h"
#include "ClientPublisher_ABC.h"
#include "Model.h"
#include "ConstructionAttribute.h"
#include "ObstacleAttribute.h"
#include "BypassAttribute.h"
#include "CrossingSiteAttribute.h"
#include "MineAttribute.h"
#include "NBCAttribute.h"
#include "SupplyRouteAttribute.h"
#include "LogisticAttribute.h"
#include "FireAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "ModelVisitor_ABC.h"
#include "Side.h"
#include "Automat.h"
#include "Object.h"

#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const Model& model, const ASN1T_MsgObjectKnowledgeCreation& asnMsg )
    : SimpleEntity< kernel::ObjectKnowledge_ABC >( asnMsg.oid )
    , model_                        ( model )
    , team_                         ( model.sides_.Get( asnMsg.team ) )
    , pObject_                      ( model.objects_.Find( asnMsg.real_object ) )
    , nType_                        ( asnMsg.type )
    , localisation_                 ( )
    , bPerceived_                   ( false )
    , automatPerceptions_           ()    
{
    Initialize( model, asnMsg.attributes );
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
    if ( attributes.m.##ASN##Present ) \
        AddAttribute( new CLASS( model, attributes ) )

// -----------------------------------------------------------------------------
// Name: Object::Initialize
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ObjectKnowledge::Initialize( const Model& model, const ASN1T_ObjectAttributes& attributes )
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
void ObjectKnowledge::Update( const ASN1T_MsgObjectKnowledgeCreation& message )
{
    bool realObjectChanged = ( message.real_object && ! pObject_ )
                          || ( pObject_ && pObject_->GetId() != message.real_object );
    if( realObjectChanged )
        pObject_ = model_.objects_.Find( message.real_object );

    ApplyUpdate( message );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Update
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObjectKnowledge::Update( const ASN1T_MsgObjectKnowledgeUpdate& asnMsg )
{  
    if( asnMsg.m.locationPresent )
    {
        localisation_.Update( asnMsg.location );
        optionals_.locationPresent = 1;
    }

    if( asnMsg.m.automat_perceptionPresent )
    {
        optionals_.automat_perceptionPresent = 1;
        automatPerceptions_.clear();
        for( unsigned int i = 0; i < asnMsg.automat_perception.n; ++i )
            automatPerceptions_.push_back( &model_.automats_.Get( asnMsg.automat_perception.elem[ i ]) );
    }

    if( asnMsg.m.real_objectPresent )
        pObject_ = model_.objects_.Find( asnMsg.real_object );

    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Update, _1, boost::cref( asnMsg.attributes ) ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SendCreation
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObjectKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::ObjectKnowledgeCreation asn;

    asn().oid         = GetId();
    asn().team        = team_.GetId();
    asn().real_object = pObject_ ? pObject_->GetId() : 0;
    asn().type        = nType_.c_str();

    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Send, _1, boost::ref( asn().attributes ) ) );

//    if( nTypeDotationForConstruction_ != std::numeric_limits< unsigned int >::max() )
//    {
//        asn().m.construction_dotation_typePresent = 1;
//        asn().construction_dotation_type = nTypeDotationForConstruction_;
//    }
//
//    if( nTypeDotationForMining_ != std::numeric_limits< unsigned int >::max() )
//    {
//        asn().m.mining_dotation_typePresent = 1;
//        asn().mining_dotation_type = nTypeDotationForMining_;
//    }
//
//    SEND_ASN_ATTRIBUTE( obstacle_type              , nObstacleType_              );
//    SEND_ASN_ATTRIBUTE( reserved_obstacle_activated, bReservedObstacleActivated_ );

    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SendFullUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObjectKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::ObjectKnowledgeUpdate asn;

    asn().oid  = GetId();
    asn().team = team_.GetId();

    asn().m.real_objectPresent = 1;
    asn().real_object          = pObject_ ? pObject_->GetId() : 0;

    if( optionals_.locationPresent )
    {
        asn().m.locationPresent = 1;
        localisation_.Send( asn().location );
    }

    if( optionals_.automat_perceptionPresent )
    {
        asn().m.automat_perceptionPresent = 1;
        asn().automat_perception.n = automatPerceptions_.size();
        asn().automat_perception.elem = asn().automat_perception.n > 0 ? new ASN1T_OID[ asn().automat_perception.n ] : 0;
        unsigned int i = 0;
        for( std::vector< const kernel::Automat_ABC* >::const_iterator it = automatPerceptions_.begin(); it != automatPerceptions_.end(); ++it, ++i )
            asn().automat_perception.elem[i] = (*it)->GetId();
    }

    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Send, _1, boost::ref( asn().attributes ) ) );

    asn.Send( publisher );

//    if( asn().m.specific_attributesPresent && pAttributes_ )
//        pAttributes_->AsnDelete( asn().specific_attributes );

    if( asn().m.automat_perceptionPresent && asn().automat_perception.n > 0 )
        delete [] asn().automat_perception.elem;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void ObjectKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::ObjectKnowledgeDestruction asn;
    asn().oid  = GetId();
    asn().team = team_.GetId();
    asn.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void ObjectKnowledge::Accept( ModelVisitor_ABC& visitor ) const
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
