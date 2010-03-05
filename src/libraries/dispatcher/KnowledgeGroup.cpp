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
#include "ClientPublisher_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/bind.hpp>

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup constructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
KnowledgeGroup::KnowledgeGroup( Model_ABC& model, const MsgsSimToClient::MsgKnowledgeGroupCreation& msg )
    : SimpleEntity< kernel::KnowledgeGroup_ABC >( msg.oid() )
    , model_( model )
    , team_( model_.Sides().Get( msg.oid_camp() ) )
    , parent_( msg.has_oid_parent() ? &model_.KnowledgeGroups().Get( msg.oid_parent() ) : 0 )
    , type_( msg.type() ) // LTO
    , enabled_( true ) // LTO
{
    // LTO begin
    ChangeSuperior( parent_ );
    // LTO end
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup destructor
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
KnowledgeGroup::~KnowledgeGroup()
{
    // LTO begin
    tools::Iterator< const kernel::KnowledgeGroup_ABC& > it( knowledgeGroups_.CreateIterator() );
    while( it.HasMoreElements() )
    {
        kernel::KnowledgeGroup_ABC& entity = const_cast< kernel::KnowledgeGroup_ABC& >( it.NextElement() );
        static_cast< KnowledgeGroup& >( entity ).ChangeSuperior( parent_ );
    }
    ChangeSuperior( 0 );
    // LTO end
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Update
// Created: SBO 2010-03-04
// -----------------------------------------------------------------------------
void KnowledgeGroup::Update( const MsgsSimToClient::MsgKnowledgeGroupCreation& /*message*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Update
// Created: SBO 2010-03-04
// -----------------------------------------------------------------------------
void KnowledgeGroup::Update( const MsgsSimToClient::MsgKnowledgeGroupUpdate& message )
{
    // LTO begin
    if( message.has_type() )
        type_ = message.type();
    if( message.has_enabled() )
        enabled_ = message.enabled();
    if( message.has_oid_parent() )
        ChangeSuperior( message.oid_parent() ? &model_.KnowledgeGroups().Get( message.oid_parent() ) : 0 );
    // LTO end
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::ChangeSuperior
// Created: SBO 2010-03-04
// -----------------------------------------------------------------------------
void KnowledgeGroup::ChangeSuperior( kernel::KnowledgeGroup_ABC* superior )
{
    // LTO begin
    if( superior == this )
        throw std::runtime_error( __FUNCTION__ ": recursive hierarchy." );
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
    
    message().set_oid( GetId() );
    message().set_oid_camp( team_.GetId() );
    // LTO begin
    message().set_type( type_ );
    if( parent_ )
        message().set_oid_parent( parent_->GetId() );
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
    message().set_oid( GetId() );
    if( parent_ )
        message().set_oid_parent( parent_->GetId() );
    else
        message().set_oid_camp( team_.GetId() );
    message().set_type( type_ );
    message().set_enabled( enabled_ );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::SendDestruction
// Created: AGE 2008-06-20
// -----------------------------------------------------------------------------
void KnowledgeGroup::SendDestruction( ClientPublisher_ABC& ) const
{
    throw std::runtime_error( __FUNCTION__ );
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
void KnowledgeGroup::Register( kernel::KnowledgeGroup_ABC& knowledgeGroup )
{
    knowledgeGroups_.Register( knowledgeGroup.GetId(), knowledgeGroup );
}
// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Register
// Created: MGD 2009-12-21
// LTO
// -----------------------------------------------------------------------------
void KnowledgeGroup::Remove( kernel::KnowledgeGroup_ABC& knowledgeGroup )
{
    knowledgeGroups_.Remove( knowledgeGroup.GetId() );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Register
// Created: MGD 2009-12-21
// LTO
// -----------------------------------------------------------------------------
void KnowledgeGroup::Register( kernel::Automat_ABC& automat )
{
    automats_.Register( automat.GetId(), automat );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::Register
// Created: MGD 2009-12-21
// LTO
// -----------------------------------------------------------------------------
void KnowledgeGroup::Remove( kernel::Automat_ABC& automat )
{
    automats_.Remove( automat.GetId() );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroup::GetTeam
// Created: SBO 2010-03-04
// -----------------------------------------------------------------------------
const kernel::Team_ABC& KnowledgeGroup::GetTeam() const
{
    return team_;
}
