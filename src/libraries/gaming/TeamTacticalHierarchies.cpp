// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "TeamTacticalHierarchies.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamTacticalHierarchies constructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
TeamTacticalHierarchies::TeamTacticalHierarchies( Controller& controller, Entity_ABC& entity )
    : EntityHierarchies< TacticalHierarchies >( controller, entity )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamTacticalHierarchies destructor
// Created: AGE 2006-10-20
// -----------------------------------------------------------------------------
TeamTacticalHierarchies::~TeamTacticalHierarchies()
{
    // NOTHING
}
