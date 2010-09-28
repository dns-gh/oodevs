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
#include "protocol/ClientSenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const Model_ABC& model, const MsgsSimToClient::MsgObjectKnowledgeCreation& message )
    : dispatcher::ObjectKnowledge_ABC( message.knowledge().id() )
    , model_                         ( model )
    , team_                          ( model.Sides().Get( message.party().id() ) )
    , pObject_                       ( model.Objects().Find( message.object().id() ) )
    , nType_                         ( message.type().id() )
    , knowledgeGroup_                ( message.has_knowledge_group() ? &model.KnowledgeGroups().Get( message.knowledge_group().id() ) : 0 )
    , localisation_                  ()
    , bPerceived_                    ( false )
    , automatPerceptions_            ()
    , typename_                      ( "objectKnowledge" )
{
    optionals_.realObjectPresent = pObject_ != 0;
    optionals_.relevancePresent = 0;
    optionals_.locationPresent = 0;
    optionals_.perceivedPresent = 0;
    optionals_.specific_attributesPresent = 0;
    optionals_.automat_perceptionPresent = 0;
    Initialize( model, message.attributes() );
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

#define CHECK_MSG_ATTRIBUTE_CREATION( MSG, CLASS ) \
    if( attributes.has_##MSG##()  )                \
        AddAttribute( new CLASS( attributes ) )

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::CreateOrUpdate
// Created: SBO 2010-06-09
// -----------------------------------------------------------------------------
template< typename T >
void ObjectKnowledge::CreateOrUpdate( const Common::ObjectAttributes& message )
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
void ObjectKnowledge::CreateOrUpdate( const Common::ObjectAttributes& message, const Model_ABC& model )
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
void ObjectKnowledge::Initialize( const Model_ABC& model, const Common::ObjectAttributes& attributes )
{
    CHECK_MSG_ATTRIBUTE_CREATION( construction      , ConstructionAttribute );
    CHECK_MSG_ATTRIBUTE_CREATION( obstacle          , ObstacleAttribute );
    CHECK_MSG_ATTRIBUTE_CREATION( mine              , MineAttribute );
    if( attributes.has_logistic() )
        AddAttribute( new LogisticAttribute( model, attributes ) );
    CHECK_MSG_ATTRIBUTE_CREATION( bypass            , BypassAttribute );
    CHECK_MSG_ATTRIBUTE_CREATION( crossing_site     , CrossingSiteAttribute );
    CHECK_MSG_ATTRIBUTE_CREATION( supply_route      , SupplyRouteAttribute );
    CHECK_MSG_ATTRIBUTE_CREATION( nbc               , NBCAttribute );
    CHECK_MSG_ATTRIBUTE_CREATION( fire              , FireAttribute );
    CHECK_MSG_ATTRIBUTE_CREATION( medical_treatment , MedicalTreatmentAttribute );
    CHECK_MSG_ATTRIBUTE_CREATION( effect_delay      , DelayAttribute );
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
    if( ( message.object().id() && ! pObject_ ) || ( pObject_ && pObject_->GetId() != ( unsigned int )message.object().id() ) )
        pObject_ = model_.Objects().Find( message.object().id() );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::DoUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObjectKnowledge::DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate& message )
{
    if( message.has_location()  )
    {
        localisation_.Update( message.location() );
        optionals_.locationPresent = 1;
    }
    if( message.has_perceived() )
    {
        bPerceived_ = message.perceived();
        optionals_.perceivedPresent = 1;
    }
    if( message.has_relevance() )
    {
        nRelevance_ = message.relevance();
        optionals_.relevancePresent = 1;
    }
    if( message.has_perceiving_automats() )
    {
        optionals_.automat_perceptionPresent = 1;
        automatPerceptions_.clear();
        for( int i = 0; i < message.perceiving_automats().elem_size(); ++i )
            automatPerceptions_.push_back( &model_.Automats().Get( message.perceiving_automats().elem( i ).id() ) );
    }
    if( message.has_object() )
    {
        pObject_ = model_.Objects().Find( message.object().id() );
        optionals_.realObjectPresent = 1;
    }
    if( message.attributes().has_construction() )
        CreateOrUpdate< ConstructionAttribute >( message.attributes() );
    if( message.attributes().has_obstacle() )
        CreateOrUpdate< ObstacleAttribute >( message.attributes() );
    if( message.attributes().has_mine() )
        CreateOrUpdate< MineAttribute >( message.attributes() );
    if( message.attributes().has_logistic() )
        CreateOrUpdate< LogisticAttribute >( message.attributes(), model_ );
    if( message.attributes().has_mine() )
        CreateOrUpdate< MineAttribute >( message.attributes() );
    if( message.attributes().has_bypass() )
        CreateOrUpdate< BypassAttribute >( message.attributes() );
    if( message.attributes().has_crossing_site() )
        CreateOrUpdate< CrossingSiteAttribute >( message.attributes() );
    if( message.attributes().has_supply_route() )
        CreateOrUpdate< SupplyRouteAttribute >( message.attributes() );
    if( message.attributes().has_nbc() )
        CreateOrUpdate< NBCAttribute >( message.attributes() );
    if( message.attributes().has_fire() )
        CreateOrUpdate< FireAttribute >( message.attributes() );
    if( message.attributes().has_medical_treatment() )
        CreateOrUpdate< MedicalTreatmentAttribute >( message.attributes() );
    if( message.attributes().has_effect_delay() )
        CreateOrUpdate< DelayAttribute >( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SendCreation
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObjectKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::ObjectKnowledgeCreation asn;
    asn().mutable_knowledge()->set_id( GetId() );
    asn().mutable_party()->set_id( team_.GetId() );
    if( knowledgeGroup_ )
        asn().mutable_knowledge_group()->set_id( knowledgeGroup_->GetId() );
    if( optionals_.realObjectPresent )
        asn().mutable_object()->set_id( pObject_ ? pObject_->GetId() : 0 );
    asn().mutable_type()->set_id( nType_ );  // $$$$ _RC_ PHC 2010-07-07: ???
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
    client::ObjectKnowledgeUpdate message;
    message().mutable_knowledge()->set_id( GetId() );
    message().mutable_party()->set_id( team_.GetId() );
    if( knowledgeGroup_ )
        message().mutable_knowledge_group()->set_id( knowledgeGroup_->GetId() );
    if( optionals_.realObjectPresent )
        message().mutable_object()->set_id( pObject_ ? pObject_->GetId() : 0 );
    if( optionals_.locationPresent )
        localisation_.Send( *message().mutable_location() );
    if( optionals_.perceivedPresent )
        message().set_perceived( bPerceived_ );
    if( optionals_.relevancePresent )
        message().set_relevance( nRelevance_ );
    if( optionals_.automat_perceptionPresent )
        for( std::vector< const kernel::Automat_ABC* >::const_iterator it = automatPerceptions_.begin(); it != automatPerceptions_.end(); ++it )
        {
            Common::AutomatId& data = *message().mutable_perceiving_automats()->add_elem();
            data.set_id( (*it)->GetId() );
        }
    std::for_each( attributes_.begin(), attributes_.end(),
                   boost::bind( &ObjectAttribute_ABC::Send, _1, boost::ref( *message().mutable_attributes() ) ) );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SendDestruction
// Created: AGE 2007-04-25
// -----------------------------------------------------------------------------
void ObjectKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::ObjectKnowledgeDestruction asn;
    asn().mutable_knowledge()->set_id( GetId() );
    asn().mutable_party()->set_id( team_.GetId());
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
void ObjectKnowledge::Display( kernel::Displayer_ABC& /*displayer*/ ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::DisplayInList
// Created: PHC 2010-07-21
// -----------------------------------------------------------------------------
void ObjectKnowledge::DisplayInList( kernel::Displayer_ABC& /*displayer*/ ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}
