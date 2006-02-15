// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Hierarchies.h"
#include "Controller.h"
#include "Agent.h"

// -----------------------------------------------------------------------------
// Name: Hierarchies constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Hierarchies::Hierarchies( Controller& controller, const Resolver_ABC< Agent >& agentResolver, const Resolver_ABC< KnowledgeGroup >& gtiaResolver  )
    : controller_( controller )
    , agentResolver_( agentResolver )
    , gtiaResolver_( gtiaResolver )
    , superior_( 0 )
    , gtia_( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: Hierarchies destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Hierarchies::~Hierarchies()
{

}

// -----------------------------------------------------------------------------
// Name: Hierarchies::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Hierarchies::Update( const ASN1T_MsgAutomateCreation& message )
{
    ChangeSuperior( message.oid_automate );
    ChangeKnowledgeGroup( message.oid_groupe_connaissance );
}

// -----------------------------------------------------------------------------
// Name: Hierarchies::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Hierarchies::Update( const ASN1T_MsgPionCreation& message )
{
    ChangeSuperior( message.oid_automate );
}

// -----------------------------------------------------------------------------
// Name: Hierarchies::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Hierarchies::Update( const ASN1T_MsgChangeAutomateAck& message )
{
    if( message.error_code == EnumObjectErrorCode::no_error ) 
        ChangeSuperior( message.oid_automate );
}

// -----------------------------------------------------------------------------
// Name: Hierarchies::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Hierarchies::Update( const ASN1T_MsgChangeAutomate& message )
{
    ChangeSuperior( message.oid_automate );
}

// -----------------------------------------------------------------------------
// Name: Hierarchies::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Hierarchies::Update( const ASN1T_MsgChangeGroupeConnaissanceAck& message )
{
    if( message.error_code == EnumObjectErrorCode::no_error ) 
        ChangeKnowledgeGroup( message.oid_groupe_connaissance );
}

// -----------------------------------------------------------------------------
// Name: Hierarchies::ChangeKnowledgeGroup
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Hierarchies::ChangeKnowledgeGroup( unsigned long id )
{
    KnowledgeGroup* gtia = gtiaResolver_.Find( id );
    if( gtia )
        ChangeKnowledgeGroup( *gtia );
}

// -----------------------------------------------------------------------------
// Name: Hierarchies::ChangeSuperior
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Hierarchies::ChangeSuperior( unsigned long id )
{
    if( superior_ )
        superior_->RemoveChild( *this );
    Agent* agent = agentResolver_.Find( id );
    superior_ = agent ? & agent->Get< Hierarchies >() : 0;
    if( superior_ )
        superior_->AddChild( *this );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Hierarchies::ChangeKnowledgeGroup
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Hierarchies::ChangeKnowledgeGroup( const KnowledgeGroup& gtia )
{
    gtia_ = &gtia;
    for( IT_Hierarchies it = children_.begin(); it != children_.end(); ++it )
        (*it)->ChangeKnowledgeGroup( gtia );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Hierarchies::AddChild
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Hierarchies::AddChild( Hierarchies& child )
{
    // $$$$ AGE 2006-02-14: check
    children_.push_back( &child );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: Hierarchies::RemoveChild
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void Hierarchies::RemoveChild( Hierarchies& child )
{
    IT_Hierarchies it = std::find( children_.begin(), children_.end(), &child );
    if( it != children_.end() )
    {
        std::swap( *it, children_.back() );
        children_.pop_back();
        controller_.Update( *this );
    }
}
