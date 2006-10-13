// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TeamHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Controller.h"
#include "KnowledgeGroupFactory_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamHierarchies constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
TeamHierarchies::TeamHierarchies( Controller& controller, kernel::Team_ABC& holder, KnowledgeGroupFactory_ABC& factory )
    : EntityHierarchies< CommunicationHierarchies >( controller, holder )
    , controller_( controller )
    , holder_( holder )
    , factory_( factory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies destructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
TeamHierarchies::~TeamHierarchies()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies::DoUpdate
// Created: AGE 2006-10-09
// -----------------------------------------------------------------------------
void TeamHierarchies::DoUpdate( const KnowledgeGroupCreationMessage& message )
{
    unsigned long id;
    message >> id;

    if( ! Find( id ) )
        factory_.CreateKnowledgeGroup( id, holder_ );
}
