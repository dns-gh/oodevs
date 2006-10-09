// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AutomatHierarchies.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
AutomatHierarchies::AutomatHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, const kernel::Resolver_ABC< kernel::KnowledgeGroup_ABC >& groupResolver )
    : EntityHierarchies< CommunicationHierarchies >( controller, holder )
    , controller_   ( controller )
    , groupResolver_( groupResolver ) 
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies destructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
AutomatHierarchies::~AutomatHierarchies()
{
    if( GetSuperior() )
        if( CommunicationHierarchies* hierarchies = GetSuperior()->Retrieve< CommunicationHierarchies >() )
            hierarchies->UnregisterSubordinate( GetEntity() );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::DoUpdate
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AutomatHierarchies::DoUpdate( const ASN1T_MsgAutomateCreation& message )
{
    UpdateSuperior( groupResolver_.Get( message.oid_groupe_connaissance) );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::DoUpdate
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AutomatHierarchies::DoUpdate( const ASN1T_MsgChangeGroupeConnaissanceAck& message )
{
    if( ! message.error_code )
        UpdateSuperior( groupResolver_.Get( message.oid_groupe_connaissance) );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::UpdateSuperior
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AutomatHierarchies::UpdateSuperior( kernel::Entity_ABC& superior )
{
    if( GetSuperior() )
        if( CommunicationHierarchies* hierarchies = GetSuperior()->Retrieve< CommunicationHierarchies >() )
            hierarchies->RemoveSubordinate( GetEntity() );
    SetSuperior( &superior );
    if( CommunicationHierarchies* hierarchies = GetSuperior()->Retrieve< CommunicationHierarchies >() )
        hierarchies->AddSubordinate( GetEntity() );
    controller_.Update( *(CommunicationHierarchies*)this );
}
