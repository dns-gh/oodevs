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
    : EntityHierarchies< CommunicationHierarchies >( controller )
    , controller_   ( controller )
    , holder_       ( holder )
    , groupResolver_( groupResolver ) 
    , superior_     ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies destructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
AutomatHierarchies::~AutomatHierarchies()
{
    if( superior_ )
        if( CommunicationHierarchies* hierarchies = superior_->Retrieve< CommunicationHierarchies >() )
            hierarchies->UnregisterSubordinate( holder_ );
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::GetSuperior
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
const kernel::Entity_ABC* AutomatHierarchies::GetSuperior() const
{
    return superior_;
}

// -----------------------------------------------------------------------------
// Name: AutomatHierarchies::GetEntity
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& AutomatHierarchies::GetEntity() const
{
    return holder_;
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
    if( superior_ )
        if( CommunicationHierarchies* hierarchies = superior_->Retrieve< CommunicationHierarchies >() )
            hierarchies->RemoveSubordinate( holder_ );
    superior_ = &superior;
    if( CommunicationHierarchies* hierarchies = superior_->Retrieve< CommunicationHierarchies >() )
        hierarchies->AddSubordinate( holder_ );
    controller_.Update( *(CommunicationHierarchies*)this );
}
