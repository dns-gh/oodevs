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

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
KnowledgeGroupHierarchies::KnowledgeGroupHierarchies( Controller& controller, Team_ABC& team, KnowledgeGroup_ABC& holder )
    : EntityHierarchies< CommunicationHierarchies >( controller, holder, &team )
{
    // NOTHING   
}

// -----------------------------------------------------------------------------
// Name: KnowledgeGroupHierarchies destructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
KnowledgeGroupHierarchies::~KnowledgeGroupHierarchies()
{
    // NOTHING
}
