// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentHierarchies.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Controller.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentHierarchies constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
AgentHierarchies::AgentHierarchies( Controller& controller, Entity_ABC& holder, 
                                    const Resolver_ABC< Agent_ABC >& agentResolver, 
                                    const Resolver_ABC< KnowledgeGroup_ABC >& gtiaResolver )
    : EntityHierarchies( controller )
    , controller_   ( controller )
    , holder_       ( holder )
    , agentResolver_( agentResolver ) 
    , gtiaResolver_ ( gtiaResolver )
    , superior_     ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies destructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
AgentHierarchies::~AgentHierarchies()
{
    if( superior_ )
        if( Hierarchies* hierarchies = superior_->Retrieve< Hierarchies >() )
            hierarchies->RemoveSubordinate( holder_ );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::GetSuperior
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
const Entity_ABC* AgentHierarchies::GetSuperior() const
{
    return superior_;
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::GetEntity
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& AgentHierarchies::GetEntity() const
{
    return holder_;
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::ChangeSuperior
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
template< typename Resolver >
void AgentHierarchies::ChangeSuperior( unsigned int id, const Resolver& resolver )
{
    UpdateSuperior( resolver.Get( id ) );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::UpdateSuperior
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentHierarchies::UpdateSuperior( Entity_ABC& superior )
{
    if( superior_ )
        if( Hierarchies* hierarchies = superior_->Retrieve< Hierarchies >() )
            hierarchies->RemoveSubordinate( holder_ );
    superior_ = &superior;
    if( Hierarchies* hierarchies = superior_->Retrieve< Hierarchies >() )
            hierarchies->AddSubordinate( holder_ );
    controller_.Update( *(Hierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentHierarchies::DoUpdate( const ASN1T_MsgAutomateCreation& message )
{
    ChangeSuperior( message.oid_groupe_connaissance, gtiaResolver_ );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentHierarchies::DoUpdate( const ASN1T_MsgPionCreation& message )
{
    ChangeSuperior( message.oid_automate, agentResolver_ );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentHierarchies::DoUpdate( const ASN1T_MsgChangeAutomateAck& message )
{
    if( message.error_code == EnumObjectErrorCode::no_error )
        ChangeSuperior( message.oid_automate, agentResolver_ );
}   

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentHierarchies::DoUpdate( const ASN1T_MsgChangeAutomate& message )
{
    ChangeSuperior( message.oid_automate, agentResolver_ );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentHierarchies::DoUpdate( const ASN1T_MsgChangeGroupeConnaissanceAck& message )
{
    if( message.error_code == EnumObjectErrorCode::no_error ) 
        ChangeSuperior( message.oid_groupe_connaissance, gtiaResolver_ );
}
