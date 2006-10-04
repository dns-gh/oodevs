// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Agent.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/DataDictionary.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Hierarchies.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::Agent( const ASN1T_MsgAutomateCreation& message, Controller& controller, 
              const Resolver_ABC< AutomatType >& resolver,
              const Resolver_ABC< Agent_ABC >& agentResolver, 
              const Resolver_ABC< KnowledgeGroup_ABC >& gtiaResolver )
    : controller_( controller )
    , agentResolver_( agentResolver )
    , gtiaResolver_( gtiaResolver )
    , id_( message.oid_automate )
    , automatType_( & resolver.Get( message.type_automate ) )
    , type_( automatType_->GetTypePC() )
    , superior_( 0 )
    , gtia_( 0 )
    , aggregated_( false )
{
    RegisterSelf( *this );
    name_ = QString( "%1 [%2]" ).arg( message.nom ).arg( id_ );

    CreateDictionary();
    gtia_ = & gtiaResolver_.Get( message.oid_groupe_connaissance );
}

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::Agent( const ASN1T_MsgPionCreation& message, Controller& controller, 
              const Resolver_ABC< AgentType >& resolver,
              const Resolver_ABC< Agent_ABC >& agentResolver, 
              const Resolver_ABC< KnowledgeGroup_ABC >& gtiaResolver )
    : controller_( controller )
    , agentResolver_( agentResolver )
    , gtiaResolver_( gtiaResolver )
    , id_( message.oid_pion )
    , automatType_( 0 )
    , type_( & resolver.Get( message.type_pion ) )
    , superior_( 0 )
    , gtia_( 0 )
    , aggregated_( false )
{
    RegisterSelf( *this );
    name_ = QString( "%1 [%2]" ).arg( message.nom ).arg( id_ );

    CreateDictionary();
    superior_ = static_cast< Agent* >( &agentResolver_.Get( message.oid_automate ) );
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    ChangeKnowledgeGroup( ( kernel::KnowledgeGroup_ABC*)0 );
    ChangeSuperior( 0 );
    Iterator< const Entity_ABC& > it = Get< Hierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const Agent& agent = static_cast< const Agent& >( it.NextElement() );
        const_cast< Agent& >( agent ).superior_ = 0;
    }
    controller_.Delete( *(Agent_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const InstanciationComplete& )
{
    controller_.Create( *(Agent_ABC*)this );
    if( gtia_ )
        gtia_->AddAutomat( id_, *this );
    Iterator< const Entity_ABC& > it = Get< Hierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const Agent& agent = static_cast< const Agent& >( it.NextElement() );
        const_cast< Agent& >( agent ).gtia_ = gtia_; // $$$$ SBO 2006-10-03: use KnowledgeGroupExtension
    }
    if( superior_ )
        superior_->AddChild( *this );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetName
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
QString Agent::GetName() const
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
    {
        ChangeKnowledgeGroup( message.oid_groupe_connaissance );
        controller_.Update( *(Agent_ABC*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::ChangeKnowledgeGroup
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::ChangeKnowledgeGroup( unsigned long id )
{
    ChangeKnowledgeGroup( gtiaResolver_.Find( id ) );
}

// -----------------------------------------------------------------------------
// Name: Agent::ChangeKnowledgeGroup
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::ChangeKnowledgeGroup( KnowledgeGroup_ABC* gtia )
{
    if( gtia_ )
        gtia_->RemoveAutomat( id_ );
    gtia_ = gtia;
    if( gtia_ )
        gtia_->AddAutomat( id_, *this );
    Iterator< const Entity_ABC& > it = Get< Hierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const Agent& agent = static_cast< const Agent& >( it.NextElement() );
        const_cast< Agent& >( agent ).gtia_ = gtia_; // $$$$ SBO 2006-10-03: use KnowledgeGroupExtension
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::ChangeSuperior
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::ChangeSuperior( unsigned long id )
{
    if( superior_ )
        superior_->RemoveChild( *this );
    superior_ = static_cast< Agent* >( agentResolver_.Find( id ) );
    if( superior_ )
        superior_->AddChild( *this );
    controller_.Update( *(Agent_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent::AddChild
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::AddChild( Agent_ABC& child )
{
    if( !child.Get< Hierarchies >().IsSubordinateOf( *this ) )
    {
        Get< Hierarchies >().AddSubordinate( child );
        controller_.Update( *(Agent_ABC*)this );
    } 
}

// -----------------------------------------------------------------------------
// Name: Agent::RemoveChild
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::RemoveChild( Agent_ABC& child )
{
    Get< Hierarchies >().RemoveSubordinate( child );
    controller_.Update( *(Agent_ABC*)this );
}


// -----------------------------------------------------------------------------
// Name: Agent::GetKnowledgeGroup
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC& Agent::GetKnowledgeGroup() const
{
    if( gtia_ )
        return *gtia_;
    if( superior_ )
        return superior_->GetKnowledgeGroup();
    throw std::runtime_error( "I have no knowledge group" );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetSuperior
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const Agent_ABC* Agent::GetSuperior() const
{
    return superior_;
}

// -----------------------------------------------------------------------------
// Name: Agent::Draw
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void Agent::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsInside( where ) )
    {
        if( automatType_ && ! aggregated_ )
            automatType_->Draw( where, viewport, tools );
        if( type_ && ! aggregated_ )
            type_->Draw( where, viewport, tools );
        else if( type_ && aggregated_ )
            type_->DrawAggregated( where, viewport, tools );
    }
}

// -----------------------------------------------------------------------------
// Name: Agent::Aggregate
// Created: AGE 2006-04-11
// -----------------------------------------------------------------------------
void Agent::Aggregate( const bool& bDenis )
{
    aggregated_ = bDenis;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetAutomatType
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
const AutomatType* Agent::GetAutomatType() const
{
    return automatType_;
}

// -----------------------------------------------------------------------------
// Name: Agent::GetType
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
const AgentType& Agent::GetType() const
{
    return *type_;
}

// -----------------------------------------------------------------------------
// Name: Agent::CreateDictionary
// Created: AGE 2006-06-27
// -----------------------------------------------------------------------------
void Agent::CreateDictionary()
{
    DataDictionary& dictionary = *new DataDictionary();
    Attach( dictionary );
    dictionary.Register( tools::translate( "Agent", "Info/Identifiant" ), id_ );
    dictionary.Register( tools::translate( "Agent", "Info/Nom" ), name_ );
    dictionary.Register( tools::translate( "Agent", "Hiérarchie/Supérieur" ), superior_ );
}
