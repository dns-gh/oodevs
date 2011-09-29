// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "UrbanKnowledge.h"
#include "Automat.h"
#include "Model_ABC.h"
#include "Object_ABC.h"
#include "Side.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge constructor
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
UrbanKnowledge::UrbanKnowledge( const Model_ABC& model, const sword::UrbanKnowledgeCreation& message )
    : UrbanKnowledge_ABC( message.knowledge().id() )
    , model_            ( model )
    , team_             ( model.Sides().Get( message.party().id() ) )
    , pUrban_           ( model.UrbanBlocks().Find( message.object().id() ) )
    , bPerceived_       ( false )
    , rProgress_        ( 0 )
{
    optionals_.perceivedPresent = 0;
    optionals_.automat_perceptionPresent = 0;
    optionals_.progressPresent = 0;
    optionals_.maxProgressPresent = 0;
    RegisterSelf( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge destructor
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
UrbanKnowledge::~UrbanKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::Update
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::DoUpdate( const sword::UrbanKnowledgeCreation& message )
{
    pUrban_ = model_.UrbanBlocks().Find( message.object().id() );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::Update
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::DoUpdate( const sword::UrbanKnowledgeUpdate& message )
{
    pUrban_ = model_.UrbanBlocks().Find( message.object().id() );
    if( message.has_perceived() )
    {
        optionals_.perceivedPresent = 1;
        bPerceived_ = message.perceived();
    }
    if( message.has_progress() )
    {
        optionals_.progressPresent = 1;
        rProgress_ = message.progress();
    }
    if( message.has_max_progress() )
    {
        optionals_.maxProgressPresent = 1;
        rMaxProgress_ = message.max_progress();
    }
    if( message.has_automat_perceptions() )
    {
        optionals_.automat_perceptionPresent = 1;
        automatPerceptions_.clear();
        for( int i = 0; i < message.automat_perceptions().elem_size(); ++i )
            automatPerceptions_.push_back( &model_.Automats().Get( message.automat_perceptions().elem( i ).id() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::SendCreation
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::UrbanKnowledgeCreation message;
    message().mutable_knowledge()->set_id( GetId() );
    message().mutable_party()->set_id( team_.GetId() );
    message().mutable_object()->set_id( pUrban_ ? pUrban_->GetId() : 0 );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::SendFullUpdate
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::UrbanKnowledgeUpdate message;
    message().mutable_knowledge()->set_id( GetId() );
    message().mutable_party()->set_id( team_.GetId() );
    message().mutable_object()->set_id( pUrban_ ? pUrban_->GetId() : 0 );
    if( optionals_.perceivedPresent )
        message().set_perceived( bPerceived_ );
    if( optionals_.progressPresent )
        message().set_progress( rProgress_ );
    if( optionals_.maxProgressPresent )
        message().set_max_progress( rMaxProgress_ );
    if( optionals_.automat_perceptionPresent )
        for( std::vector< const kernel::Automat_ABC* >::const_iterator it = automatPerceptions_.begin(); it != automatPerceptions_.end(); ++it )
            message().mutable_automat_perceptions()->add_elem()->set_id( ( *it )->GetId() );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::SendDestruction
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::UrbanKnowledgeDestruction message;
    message().mutable_knowledge()->set_id( GetId() );
    message().mutable_party()->set_id( team_.GetId() );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::Accept
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::Accept( kernel::ModelVisitor_ABC& visitor ) const
{
    visitor.Visit( *this );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetTeam
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
const kernel::Team_ABC* UrbanKnowledge::GetTeam() const
{
    //$$$$ Useless method in dispatcher ...
    return 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetEntity
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* UrbanKnowledge::GetEntity() const
{
    return pUrban_;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetEntityId
// Created: JSR 2011-09-28
// -----------------------------------------------------------------------------
unsigned long UrbanKnowledge::GetEntityId() const
{
    return pUrban_ ? pUrban_->GetId() : 0;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::GetOwner
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
const kernel::Team_ABC& UrbanKnowledge::GetOwner() const
{
    return team_;
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::Display
// Created: PHC 2010-07-22
// -----------------------------------------------------------------------------
void UrbanKnowledge::Display( kernel::Displayer_ABC& /*displayer*/ ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::DisplayInList
// Created: PHC 2010-07-22
// -----------------------------------------------------------------------------
void UrbanKnowledge::DisplayInList( kernel::Displayer_ABC& /*displayer*/ ) const
{
    throw std::runtime_error( __FUNCTION__ " not implemented" );
}
