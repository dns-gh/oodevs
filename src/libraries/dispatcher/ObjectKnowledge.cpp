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
#include "Automat.h"
#include "KnowledgeGroup.h"
#include "Model.h"
#include "Object.h"
#include "Side.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge constructor
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
ObjectKnowledge::ObjectKnowledge( const Model_ABC& model, const sword::ObjectKnowledgeCreation& message )
    : ObjectKnowledge_ABC( message.knowledge().id() )
    , model_                         ( model )
    , owner_                         ( model.Sides().Get( message.party().id() ) )
    , pObject_                       ( model.Objects().Find( message.object().id() ) )
    , entityId_                      ( 0 )
    , nType_                         ( message.type().id() )
    , knowledgeGroup_                ( message.has_knowledge_group() ? &model.KnowledgeGroups().Get( message.knowledge_group().id() ) : 0 )
    , localisation_                  ()
    , bPerceived_                    ( false )
    , automatPerceptions_            ()
    , typename_                      ( "objectKnowledge" )
    , attributes_                    ( model )
{
    if( !pObject_ )
        pObject_ = model.UrbanBlocks().Find( message.object().id() );
    if( pObject_ )
        entityId_ = pObject_->GetId();

    optionals_.realObjectPresent = pObject_ != 0;
    optionals_.relevancePresent = 0;
    optionals_.locationPresent = 0;
    optionals_.perceivedPresent = 0;
    optionals_.specific_attributesPresent = 0;
    optionals_.automat_perceptionPresent = 0;
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

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::DoUpdate
// Created: AGE 2007-04-13
// -----------------------------------------------------------------------------
void ObjectKnowledge::DoUpdate( const sword::ObjectKnowledgeCreation& message )
{
    if( ( message.object().id() && ! pObject_ ) || ( pObject_ && pObject_->GetId() != ( unsigned int )message.object().id() ) )
    {
        pObject_ = model_.Objects().Find( message.object().id() );
        if( !pObject_ )
            pObject_ = model_.UrbanBlocks().Find( message.object().id() );
    }
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::DoUpdate
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObjectKnowledge::DoUpdate( const sword::ObjectKnowledgeUpdate& message )
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
        if( !pObject_ )
            pObject_ = model_.UrbanBlocks().Find( message.object().id() );
        if( pObject_ )
            entityId_ = pObject_->GetId();
        optionals_.realObjectPresent = 1;
    }

    attributes_.Update( message.attributes() );
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::SendCreation
// Created: NLD 2006-09-28
// -----------------------------------------------------------------------------
void ObjectKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::ObjectKnowledgeCreation asn;
    asn().mutable_knowledge()->set_id( GetId() );
    asn().mutable_party()->set_id( owner_.GetId() );
    if( knowledgeGroup_ )
        asn().mutable_knowledge_group()->set_id( knowledgeGroup_->GetId() );
    asn().mutable_object()->set_id( entityId_ );
    asn().mutable_type()->set_id( nType_ );  // $$$$ _RC_ PHC 2010-07-07: ???
    asn().mutable_attributes(); //$$$$ NLD 2010-10-26 - A VIRER quand viré dans le protocole ... le message de creation
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
    message().mutable_party()->set_id( owner_.GetId() );
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
            sword::AutomatId& data = *message().mutable_perceiving_automats()->add_elem();
            data.set_id( (*it)->GetId() );
        }
    attributes_.Send( *message().mutable_attributes() );
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
    asn().mutable_party()->set_id( owner_.GetId());
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
// Name: ObjectKnowledge::GetTeam
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::Team_ABC* ObjectKnowledge::GetTeam() const
{
    //$$$$ Useless method in dispatcher ...
    return 0;
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
// Name: ObjectKnowledge::GetEntityId
// Created: JSR 2011-09-28
// -----------------------------------------------------------------------------
unsigned long ObjectKnowledge::GetEntityId() const
{
    return pObject_ ? pObject_->GetId() : 0;
}

// -----------------------------------------------------------------------------
// Name: ObjectKnowledge::GetOwner
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
const kernel::Team_ABC& ObjectKnowledge::GetOwner() const
{
    return owner_;
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
