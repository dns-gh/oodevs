// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Agent.h"
#include "AgentType.h"
#include "AutomatType.h"
#include "Controller.h"
#include "KnowledgeGroup.h"
#include "Attributes.h"

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::Agent( const ASN1T_MsgAutomateCreation& message, Controller& controller, 
              const Resolver_ABC< AutomatType >& resolver,
              const Resolver_ABC< Agent >& agentResolver, 
              const Resolver_ABC< KnowledgeGroup >& gtiaResolver )
    : controller_( controller )
    , agentResolver_( agentResolver )
    , gtiaResolver_( gtiaResolver )
    , name_( message.nom )
    , id_( message.oid_automate )
    , automatType_( & resolver.Get( message.type_automate ) )
    , type_( automatType_->GetTypePC() )
    , superior_( 0 )
    , gtia_( 0 )
    , aggregated_( false )
{
    // $$$$ AGE 2006-02-16: message.oid_camp
    ChangeKnowledgeGroup( message.oid_groupe_connaissance );
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::Agent( const ASN1T_MsgPionCreation& message, Controller& controller, 
              const Resolver_ABC< AgentType >& resolver,
              const Resolver_ABC< Agent >& agentResolver, 
              const Resolver_ABC< KnowledgeGroup >& gtiaResolver )
    : controller_( controller )
    , agentResolver_( agentResolver )
    , gtiaResolver_( gtiaResolver )
    , name_( message.nom )
    , id_( message.oid_pion )
    , automatType_( 0 )
    , type_( & resolver.Get( message.type_pion ) )
    , superior_( 0 )
    , gtia_( 0 )
    , aggregated_( false )
{
    ChangeSuperior( message.oid_automate );
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    ChangeKnowledgeGroup( 0 );
    ChangeSuperior( 0 );
    for( IT_Elements it = elements_.begin(); it != elements_.end(); ++it )
        it->second->superior_ = 0;
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
std::string Agent::GetName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetId
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
unsigned long Agent::GetId() const
{
    return id_;
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const ASN1T_MsgChangeAutomateAck& message )
{
    if( message.error_code == EnumObjectErrorCode::no_error ) 
        ChangeSuperior( message.oid_automate );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const ASN1T_MsgChangeAutomate& message )
{
    ChangeSuperior( message.oid_automate );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const ASN1T_MsgChangeGroupeConnaissanceAck& message )
{
    if( message.error_code == EnumObjectErrorCode::no_error ) 
        ChangeKnowledgeGroup( message.oid_groupe_connaissance );
}

// -----------------------------------------------------------------------------
// Name: Agent::ChangeKnowledgeGroup
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::ChangeKnowledgeGroup( unsigned long id )
{
    KnowledgeGroup* gtia = gtiaResolver_.Find( id );
    if( gtia )
        ChangeKnowledgeGroup( *gtia );
}

// -----------------------------------------------------------------------------
// Name: Agent::ChangeKnowledgeGroup
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::ChangeKnowledgeGroup( KnowledgeGroup& gtia )
{
    if( gtia_ )
        gtia_->RemoveAutomat( id_ );
    gtia_ = &gtia;
    gtia_->AddAutomat( id_, *this );
    for( Resolver< Agent >::IT_Elements it = Resolver< Agent >::elements_.begin(); it != Resolver< Agent >::elements_.end(); ++it )
        it->second->gtia_ = gtia_;
//    controller_.Update( *this ); // $$$$ AGE 2006-02-16: 
}

// -----------------------------------------------------------------------------
// Name: Agent::ChangeSuperior
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::ChangeSuperior( unsigned long id )
{
    if( superior_ )
        superior_->RemoveChild( *this );
    superior_ = agentResolver_.Find( id );
    if( superior_ )
        superior_->AddChild( *this );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent::AddChild
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::AddChild( Agent& child )
{
    Resolver< Agent >::Register( child.id_, child );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent::RemoveChild
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::RemoveChild( Agent& child )
{
    Resolver< Agent >::Remove( child.id_ );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetKnowledgeGroup
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
KnowledgeGroup& Agent::GetKnowledgeGroup() const
{
    if( gtia_ )
        return *gtia_;
    if( superior_ )
        return superior_->GetKnowledgeGroup();
    throw std::runtime_error( "I have no knowledge group" );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetTeam
// Created: AGE 2006-03-28
// -----------------------------------------------------------------------------
const Team& Agent::GetTeam() const
{
    return GetKnowledgeGroup().GetTeam();
}

// -----------------------------------------------------------------------------
// Name: Agent::GetDecisionalModel
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const DecisionalModel& Agent::GetDecisionalModel() const
{
    return type_->GetDecisionalModel();
}

// -----------------------------------------------------------------------------
// Name: Agent::GetAutomatDecisionalModel
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const DecisionalModel* Agent::GetAutomatDecisionalModel() const
{
    return automatType_ ? & automatType_->GetDecisionalModel() : 0;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetSuperior
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const Agent* Agent::GetSuperior() const
{
    return superior_;
}

// -----------------------------------------------------------------------------
// Name: Agent::Draw
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void Agent::Draw( const geometry::Point2f& where, const GlTools_ABC& tools ) const
{
    if( automatType_ && ! aggregated_ )
        automatType_->Draw( where, tools );
    if( type_ && ! aggregated_ )
        type_->Draw( where, tools );
    if( type_ && aggregated_ )
        type_->DrawAggregated( where, tools );
}

// -----------------------------------------------------------------------------
// Name: Agent::Aggregate
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void Agent::Aggregate( const bool& bDenis )
{
    aggregated_ = bDenis;
}
