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
#include "Side.h"
#include "UrbanObject.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ModelVisitor_ABC.h"
#include "protocol/ClientPublisher_ABC.h"
#include "protocol/ClientSenders.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge constructor
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
UrbanKnowledge::UrbanKnowledge( const Model_ABC& model, const MsgsSimToClient::MsgUrbanKnowledgeCreation& message )
    : dispatcher::UrbanKnowledge_ABC( message.oid() )
    , model_                        ( model )
    , team_                         ( model.Sides().Get( message.team() ) )
    , pUrban_                       ( model.UrbanBlocks().Find( message.real_urban() ) )
    , bPerceived_                   ( false )
    , rProgress_                    ( 0. )
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
void UrbanKnowledge::DoUpdate( const MsgsSimToClient::MsgUrbanKnowledgeCreation& message )
{
    if( ( message.real_urban() && ! pUrban_ ) || ( pUrban_ && pUrban_->GetId() != unsigned int( message.real_urban() ) ) )
        pUrban_ = model_.UrbanBlocks().Find( message.real_urban() );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::Update
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::DoUpdate( const MsgsSimToClient::MsgUrbanKnowledgeUpdate& message )
{
    if( message.has_real_urban() )
        pUrban_ = model_.UrbanBlocks().Find( message.real_urban() );
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
    if( message.has_maxprogress() )
    {
        optionals_.maxProgressPresent = 1;
        rMaxProgress_ = message.maxprogress();
    }
    if( message.has_automat_perception() )
    {
        optionals_.automat_perceptionPresent = 1;
        automatPerceptions_.clear();
        for( int i = 0; i < message.automat_perception().elem_size(); ++i )
            automatPerceptions_.push_back( &model_.Automats().Get( message.automat_perception().elem( i ) ) );
    }
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::SendCreation
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::SendCreation( ClientPublisher_ABC& publisher ) const
{
    client::UrbanKnowledgeCreation message;
    message().set_oid( GetId() );
    message().set_team( team_.GetId() );
    message().set_real_urban( pUrban_ ? pUrban_->GetId() : 0 );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::SendFullUpdate
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::SendFullUpdate( ClientPublisher_ABC& publisher ) const
{
    client::UrbanKnowledgeUpdate message;
    message().set_oid( GetId() );
    message().set_team( team_.GetId() );
    message().set_real_urban( pUrban_ ? pUrban_->GetId() : 0 );
    if( optionals_.perceivedPresent )
        message().set_perceived( bPerceived_ );
    if( optionals_.progressPresent )
        message().set_progress( rProgress_ );
    if( optionals_.maxProgressPresent )
        message().set_maxprogress( rMaxProgress_ );
    if( optionals_.automat_perceptionPresent )
        for( std::vector< const kernel::Automat_ABC* >::const_iterator it = automatPerceptions_.begin(); it != automatPerceptions_.end(); ++it )
            message().mutable_automat_perception()->add_elem( (*it)->GetId() );
    message.Send( publisher );
}

// -----------------------------------------------------------------------------
// Name: UrbanKnowledge::SendDestruction
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
void UrbanKnowledge::SendDestruction( ClientPublisher_ABC& publisher ) const
{
    client::UrbanKnowledgeDestruction message;
    message().set_oid( GetId() );
    message().set_team( team_.GetId() );
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
// Name: UrbanKnowledge::GetRecognizedEntity
// Created: MGD 2009-12-11
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* UrbanKnowledge::GetRecognizedEntity() const
{
    return pUrban_;
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
