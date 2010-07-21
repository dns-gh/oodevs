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
#include "DelayAttribute.h"
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
ObjectKnowledge::ObjectKnowledge( const Model_ABC& model, const MsgsSimToClient::MsgObjectKnowledgeCreation& asnMsg )
    : dispatcher::ObjectKnowledge_ABC( asnMsg.oid() )
    , model_                         ( model )
    , team_                          ( model.Sides().Get( asnMsg.team() ) )
    , pObject_                       ( model.Objects().Find( asnMsg.real_object() ) )
    , nType_                         ( asnMsg.type() )
    , knowledgeGroup_                ( asnMsg.has_group() ? &model.KnowledgeGroups().Get( asnMsg.group() ) : 0 )
    , localisation_                  ( )
    , bPerceived_                    ( false )
    , automatPerceptions_            ()
    , typename_                      ( "objectKnowledge" )
{
    Initialize( model, asnMsg.attributes() );
    RegisterSelf( *this );
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
    if( attributes.has_##ASN##()  )                \
        AddAttribute( new CLASS( attributes ) )

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::CreateOrUpdate
// Created: SBO 2010-06-09
// -----------------------------------------------------------------------------
template< typename T >
void ObjectKnowledge::CreateOrUpdate( const Common::MsgObjectAttributes& message )
{
    T_ObjectAttributes::iterator it;
    for( it = attributes_.begin(); it != attributes_.end(); ++it )
        if( dynamic_cast< T* >( &*it ) )
        {
            it->Update( message );
            break;
        }
    if( it == attributes_.end() )
        AddAttribute( new T( message ) );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::CreateOrUpdate
// Created: SBO 2010-06-09
// -----------------------------------------------------------------------------
template< typename T >
void ObjectKnowledge::CreateOrUpdate( const Common::MsgObjectAttributes& message, const Model_ABC& model )
{
    T_ObjectAttributes::iterator it;
    for( it = attributes_.begin(); it != attributes_.end(); ++it )
        if( dynamic_cast< T* >( &*it ) )
        {
            it->Update( message );
            break;
        }
    if( it == attributes_.end() )
        AddAttribute( new T( model, message ) );
}

// -----------------------------------------------------------------------------
// Name: Object::Initialize
// Created: JCR 2008-06-08
// -----------------------------------------------------------------------------
void ObjectKnowledge::Initialize( const Model_ABC& model, const Common::MsgObjectAttributes& attributes )
{
    CHECK_ASN_ATTRIBUTE_CREATION( construction      , ConstructionAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( obstacle          , ObstacleAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( mine              , MineAttribute );
    if( attributes.has_logistic() )
        AddAttribute( new LogisticAttribute( model, attributes ) );
    CHECK_ASN_ATTRIBUTE_CREATION( bypass            , BypassAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( crossing_site     , CrossingSiteAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( supply_route      , SupplyRouteAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( nbc               , NBCAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( fire              , FireAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( medical_treatment , MedicalTreatmentAttribute );
    CHECK_ASN_ATTRIBUTE_CREATION( effect_delay      , DelayAttribute );
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
// Name: ObjectKnowledge::DoUpdate
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void ObjectKnowledge::DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeCreation& message )
{
    if( ( message.real_object() && ! pObject_ ) || ( pObject_ && pObject_->GetId() != ( unsigned int )message.real_object() ) )
        pObject_ = model_.Objects().Find( message.real_object() );
    ApplyUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::DoUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObjectKnowledge::DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& asnMsg )
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
            automatPerceptions_.push_back( &model_.Automats().Get( asnMsg.automat_perception().elem( i )) );
    }
    if( asnMsg.has_real_object()  )
        pObject_ = model_.Objects().Find( asnMsg.real_object() );
    if( asnMsg.attributes().has_construction() )
        CreateOrUpdate< ConstructionAttribute >( asnMsg.attributes() );
    if( asnMsg.attributes().has_obstacle() )
        CreateOrUpdate< ObstacleAttribute >( asnMsg.attributes() );
    if( asnMsg.attributes().has_mine() )
        CreateOrUpdate< MineAttribute >( asnMsg.attributes() );
    if( asnMsg.attributes().has_logistic() )
        CreateOrUpdate< LogisticAttribute >( asnMsg.attributes(), model_ );
    if( asnMsg.attributes().has_mine() )
        CreateOrUpdate< MineAttribute >( asnMsg.attributes() );
    if( asnMsg.attributes().has_bypass() )
        CreateOrUpdate< BypassAttribute >( asnMsg.attributes() );
    if( asnMsg.attributes().has_crossing_site() )
        CreateOrUpdate< CrossingSiteAttribute >( asnMsg.attributes() );
    if( asnMsg.attributes().has_supply_route() )
        CreateOrUpdate< SupplyRouteAttribute >( asnMsg.attributes() );
    if( asnMsg.attributes().has_nbc() )
        CreateOrUpdate< NBCAttribute >( asnMsg.attributes() );
    if( asnMsg.attributes().has_fire() )
        CreateOrUpdate< FireAttribute >( asnMsg.attributes() );
    if( asnMsg.attributes().has_medical_treatment() )
        CreateOrUpdate< MedicalTreatmentAttribute >( asnMsg.attributes() );
    if( asnMsg.attributes().has_effect_delay() )
        CreateOrUpdate< DelayAttribute >( asnMsg.attributes() );
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
    if( pObject_ )
        asn().set_real_object( pObject_->GetId() );
    asn().set_type( nType_ );
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
    if( pObject_ )
        asn().set_real_object( pObject_->GetId() );
    if( optionals_.locationPresent )
        localisation_.Send( *asn().mutable_location() );
    if( optionals_.automat_perceptionPresent )
        for( std::vector< const kernel::Automat_ABC* >::const_iterator it = automatPerceptions_.begin(); it != automatPerceptions_.end(); ++it )
            asn().mutable_automat_perception()->add_elem( (*it)->GetId() );
    asn().mutable_attributes();
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
    asn().set_oid( GetId() );
    asn().set_team( team_.GetId() );
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
// Name: ObjectKnowledge::GetTypeName
// Created: PHC 2010-07-20
// -----------------------------------------------------------------------------
QString ObjectKnowledge::GetTypeName() const
{
    return typename_;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::Display
// Created: PHC 2010-07-21
// -----------------------------------------------------------------------------
void ObjectKnowledge::Display( kernel::Displayer_ABC& displayer ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::DisplayInList
// Created: PHC 2010-07-21
// -----------------------------------------------------------------------------
void ObjectKnowledge::DisplayInList( kernel::Displayer_ABC& displayer ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}
