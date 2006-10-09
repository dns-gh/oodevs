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
    : EntityHierarchies< CommunicationHierarchies >( controller, holder )
    , controller_   ( controller )
    , automatResolver_( automatResolver ) 
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies destructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
AgentHierarchies::~AgentHierarchies()
{
    if( GetSuperior() )
        if( CommunicationHierarchies* hierarchies = GetSuperior()->Retrieve< CommunicationHierarchies >() )
            hierarchies->UnregisterSubordinate( GetEntity() );
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies::UpdateSuperior
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void AgentHierarchies::UpdateSuperior( Entity_ABC& superior )
{
    if( GetSuperior() )
        if( CommunicationHierarchies* hierarchies = GetSuperior()->Retrieve< CommunicationHierarchies >() )
            hierarchies->RemoveSubordinate( GetEntity() );
    SetSuperior( &superior );
    if( CommunicationHierarchies* hierarchies = superior.Retrieve< CommunicationHierarchies >() )
        hierarchies->AddSubordinate( GetEntity() );
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
