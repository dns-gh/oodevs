// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "KnowledgeGroupHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Controller.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
KnowledgeGroupHierarchies::KnowledgeGroupHierarchies( Controller& controller, Team_ABC& team, KnowledgeGroup_ABC& holder )
    : EntityHierarchies< TacticalHierarchies >( controller )
    , controller_( controller )
    , superior_  ( team )
    , holder_    ( holder )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies destructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
KnowledgeGroupHierarchies::~KnowledgeGroupHierarchies()
{
    if( TacticalHierarchies* hierarchies = superior_.Retrieve< TacticalHierarchies >() )
        hierarchies->RemoveSubordinate( holder_ );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void KnowledgeGroupHierarchies::DoUpdate( const kernel::InstanciationComplete& )
{
    if( TacticalHierarchies* hierarchies = superior_.Retrieve< TacticalHierarchies >() )
    {
        hierarchies->AddSubordinate( holder_ );
        controller_.Update( *hierarchies );
        controller_.Update( *(TacticalHierarchies*)this );
    }
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies::GetSuperior
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
const Entity_ABC* KnowledgeGroupHierarchies::GetSuperior() const
{
    return &superior_;
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies::GetEntity
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& KnowledgeGroupHierarchies::GetEntity() const
{
    return holder_;
}
