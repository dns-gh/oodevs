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
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Automat_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Agent constructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::Agent( const ASN1T_MsgPionCreation& message, Controller& controller, 
              const Resolver_ABC< AgentType >& resolver,
              const Resolver_ABC< Automat_ABC >& automatResolver, 
              const Resolver_ABC< KnowledgeGroup_ABC >& gtiaResolver )
    : controller_( controller )
    , automatResolver_( automatResolver )
    , gtiaResolver_( gtiaResolver )
    , id_( message.oid_pion )
    , type_( resolver.Get( message.type_pion ) )
    , automat_( & automatResolver_.Get( message.oid_automate ) )
    , isPc_( id_ == automat_->GetId() )
{
    RegisterSelf( *this );
    name_ = QString( "%1 [%2]" ).arg( message.nom ).arg( id_ );

    CreateDictionary();
}

// -----------------------------------------------------------------------------
// Name: Agent destructor
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
Agent::~Agent()
{
    ChangeAutomat( 0 );
    controller_.Delete( *(Agent_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const InstanciationComplete& )
{
    controller_.Create( *(Agent_ABC*)this );
    if( automat_ )
        automat_->AddAgent( id_, *this );
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
        ChangeAutomat( message.oid_automate );
}

// -----------------------------------------------------------------------------
// Name: Agent::DoUpdate
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::DoUpdate( const ASN1T_MsgChangeAutomate& message )
{
    ChangeAutomat( message.oid_automate );
}

// -----------------------------------------------------------------------------
// Name: Agent::ChangeAutomat
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::ChangeAutomat( unsigned long id )
{
    if( automat_ )
        automat_->RemoveAgent( id_ );
    automat_ = automatResolver_.Find( id );
    if( automat_ )
        automat_->AddAgent( id_, *this );
    controller_.Update( *(Agent_ABC*)this );
}

// -----------------------------------------------------------------------------
// Name: Agent::AddChild
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::AddChild( Agent_ABC& child )
{
    if( !child.Get< CommunicationHierarchies >().IsSubordinateOf( *this ) )
    {
        Get< CommunicationHierarchies >().AddSubordinate( child );
        controller_.Update( *(Agent_ABC*)this );
    } 
}

// -----------------------------------------------------------------------------
// Name: Agent::RemoveChild
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void Agent::RemoveChild( Agent_ABC& child )
{
    Get< CommunicationHierarchies >().RemoveSubordinate( child );
    controller_.Update( *(Agent_ABC*)this );
}


// -----------------------------------------------------------------------------
// Name: Agent::GetKnowledgeGroup
// Created: AGE 2006-02-21
// -----------------------------------------------------------------------------
KnowledgeGroup_ABC& Agent::GetKnowledgeGroup() const
{
    return automat_->GetKnowledgeGroup();
}

// -----------------------------------------------------------------------------
// Name: Agent::GetAutomat
// Created: AGE 2006-03-14
// -----------------------------------------------------------------------------
const Automat_ABC& Agent::GetAutomat() const
{
    return *automat_;
}

// -----------------------------------------------------------------------------
// Name: Agent::Draw
// Created: SBO 2006-03-20
// -----------------------------------------------------------------------------
void Agent::Draw( const geometry::Point2f& where, const geometry::Rectangle2f& viewport, const GlTools_ABC& tools ) const
{
    if( viewport.IsInside( where ) )
        type_.Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Agent::GetType
// Created: SBO 2006-08-03
// -----------------------------------------------------------------------------
const AgentType& Agent::GetType() const
{
    return type_;
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
    dictionary.Register( tools::translate( "Agent", "Hiérarchie/Supérieur" ), automat_ );
}
