// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentHierarchies.h"

// -----------------------------------------------------------------------------
// Name: AgentHierarchies constructor
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
AgentHierarchies::AgentHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : TacticalHierarchies( controller, holder, superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentHierarchies destructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
AgentHierarchies::~AgentHierarchies()
{
    // NOTHING
}
