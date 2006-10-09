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
    : EntityHierarchies< CommunicationHierarchies >( controller, holder )
    , controller_( controller )
{
    SetSuperior( &team );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies destructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
KnowledgeGroupHierarchies::~KnowledgeGroupHierarchies()
{
    if( CommunicationHierarchies* hierarchies = GetSuperior()->Retrieve< CommunicationHierarchies >() )
        hierarchies->RemoveSubordinate( GetEntity() );
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies::DoUpdate
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
void KnowledgeGroupHierarchies::DoUpdate( const kernel::InstanciationComplete& )
{
    if( CommunicationHierarchies* hierarchies = GetSuperior()->Retrieve< CommunicationHierarchies >() )
    {
        hierarchies->AddSubordinate( GetEntity() );
        controller_.Update( *(CommunicationHierarchies*)this );
    }
}
