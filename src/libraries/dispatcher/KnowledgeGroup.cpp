// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "KnowledgeGroup.h"
#include "Side.h"
#include "Model.h"
#include "Automat_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( Model_ABC& model, const sword::KnowledgeGroupCreation& msg )
    : KnowledgeGroup_ABC( msg.knowledge_group().id(), QString::fromStdString( msg.name() ) )
    , model_( model )
    , team_( model_.Sides().Get( msg.party().id() ) )
    , parent_( msg.has_parent() ? &model_.KnowledgeGroups().Get( msg.parent().id() ) : 0 )
    , type_( msg.type() ) // LTO
    , enabled_( true ) // LTO
    , jammed_( msg.has_jam() && msg.jam() )
    , crowd_( msg.has_crowd() && msg.crowd() )
{
    // LTO begin
    if( parent_ )
    {
        if( parent_ == this )
           throw MASA_EXCEPTION( "recursive hierarchy." );
        parent_->Register( *this );
    }
    else
        team_.Register( *this );
    // LTO end
    AddExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    // LTO begin
    tools::Iterator< const dispatcher::KnowledgeGroup_ABC& > it( knowledgeGroups_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        dispatcher::KnowledgeGroup_ABC& entity = const_cast< dispatcher::KnowledgeGroup_ABC& >( it.NextElement() );
        entity.ChangeSuperior( parent_ );
    }
    if( parent_ )
        parent_->Remove( *this );
    else
        team_.Remove( *this );
    // LTO end
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::DoUpdate
// Created: SBO 2010-03-04
// -----------------------------------------------------------------------------
void KnowledgeGroup::DoUpdate( const sword::KnowledgeGroupUpdate& message )
{
    // LTO begin
    if( message.has_type() )
        type_ = message.type();
    if( message.has_enabled() )
        enabled_ = message.enabled();
    if( message.has_parent() )
        ChangeSuperior( message.parent().id() ? &model_.KnowledgeGroups().Get( message.parent().id() ) : 0 );
    // LTO end
    if( message.has_name() )
        SetName( QString::fromStdString( message.name() ) );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::IsActivated
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
bool KnowledgeGroup::IsActivated() const
{
    return enabled_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::IsCrowd
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
bool KnowledgeGroup::IsCrowd() const
{
    return crowd_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::ChangeSuperior
// Created: SBO 2010-03-04
// -----------------------------------------------------------------------------
void KnowledgeGroup::ChangeSuperior( dispatcher::KnowledgeGroup_ABC* superior )
{
    // LTO begin
    if( superior == this )
        throw MASA_EXCEPTION( "recursive hierarchy." );
    if( parent_ )
        parent_->Remove( *this );
    else
        team_.Remove( *this );
    parent_ = superior;
    if( parent_ )
        parent_->Register( *this );
    else
        team_.Register( *this );
    // LTO end
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SendCreation
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void KnowledgeGroup::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::KnowledgeGroupCreation message;
    message().mutable_knowledge_group()->set_id( GetId() );
    message().mutable_party()->set_id( team_.GetId() );
    // LTO begin
    message().set_type( type_ );
    message().set_crowd( crowd_ );
    if( parent_ )
        message().mutable_parent()->set_id( parent_->GetId() );
    if( jammed_ )
        message().set_jam( true );
    message().set_name( GetName().toStdString() );
    // LTO end
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SendFullUpdate
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
void KnowledgeGroup::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::KnowledgeGroupUpdate message;
    message().mutable_knowledge_group()->set_id( GetId() );
    if( parent_ )
        message().mutable_parent()->set_id( parent_->GetId() );
    else
        message().mutable_party()->set_id( team_.GetId() );
    message().set_type( type_ );
    message().set_enabled( enabled_ );
    message().set_name( GetName().toStdString() );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SendDestruction
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void KnowledgeGroup::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::KnowledgeGroupDestruction message;
    message().mutable_knowledge_group()->set_id( GetId() );
    message().mutable_party()->set_id( team_.GetId() );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Accept
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void KnowledgeGroup::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
    knowledgeGroups_.Apply( boost::bind( &KnowledgeGroup_ABC::Accept, _1, boost::ref( visitor ) ) ); // LTO
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Register
// Created: MGD 2009-12-21
// LTO
// -----------------------------------------------------------------------------
void KnowledgeGroup::Register( dispatcher::KnowledgeGroup_ABC& knowledgeGroup )
{
    knowledgeGroups_.Register( knowledgeGroup.GetId(), knowledgeGroup );
}
// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Register
// Created: MGD 2009-12-21
// LTO
// -----------------------------------------------------------------------------
void KnowledgeGroup::Remove( dispatcher::KnowledgeGroup_ABC& knowledgeGroup )
{
    knowledgeGroups_.Remove( knowledgeGroup.GetId() );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Register
// Created: MGD 2009-12-21
// LTO
// -----------------------------------------------------------------------------
void KnowledgeGroup::Register( dispatcher::Automat_ABC& automat )
{
    automats_.Register( automat.GetId(), automat );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Register
// Created: MGD 2009-12-21
// LTO
// -----------------------------------------------------------------------------
void KnowledgeGroup::Remove( dispatcher::Automat_ABC& automat )
{
    automats_.Remove( automat.GetId() );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetTeam
// Created: SBO 2010-03-04
// -----------------------------------------------------------------------------
const dispatcher::Team_ABC& KnowledgeGroup::GetTeam() const
{
    return team_;
}
