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
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controller.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentHierarchies constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
AgentHierarchies::AgentHierarchies( Controller& controller, Entity_ABC& holder, 
                                    const Resolver_ABC< Automat_ABC >& automatResolver )
    : EntityHierarchies< CommunicationHierarchies >( controller )
    , controller_   ( controller )
    , holder_       ( holder )
    , automatResolver_( automatResolver ) 
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
        if( CommunicationHierarchies* hierarchies = superior_->Retrieve< CommunicationHierarchies >() )
            hierarchies->UnregisterSubordinate( holder_ );
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
// Name: AgentHierarchies::UpdateSuperior
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentHierarchies::UpdateSuperior( Entity_ABC& superior )
{
    if( superior_ )
        if( CommunicationHierarchies* hierarchies = superior_->Retrieve< CommunicationHierarchies >() )
            hierarchies->RemoveSubordinate( holder_ );
    superior_ = &superior;
    if( CommunicationHierarchies* hierarchies = superior_->Retrieve< CommunicationHierarchies >() )
        hierarchies->AddSubordinate( holder_ );
    controller_.Update( *(CommunicationHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentHierarchies::DoUpdate( const ASN1T_MsgPionCreation& message )
{
    UpdateSuperior( automatResolver_.Get( message.oid_automate ) );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentHierarchies::DoUpdate( const ASN1T_MsgChangeAutomateAck& message )
{
    if( message.error_code == EnumObjectErrorCode::no_error )
        UpdateSuperior( automatResolver_.Get( message.oid_automate ) );
}   

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentHierarchies::DoUpdate( const ASN1T_MsgChangeAutomate& message )
{
    UpdateSuperior( automatResolver_.Get( message.oid_automate ) );
}
