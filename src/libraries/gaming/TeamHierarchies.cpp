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
#include "clients_kernel/Controller.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: TeamHierarchies constructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
TeamHierarchies::TeamHierarchies( Controller& controller, const kernel::Team_ABC& holder )
    : EntityHierarchies< TacticalHierarchies >( controller )
    , controller_( controller )
    , holder_( holder )
{
    controller_.Create( *(TacticalHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies destructor
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
TeamHierarchies::~TeamHierarchies()
{
    controller_.Delete( *(TacticalHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies::GetSuperior
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
const Entity_ABC* TeamHierarchies::GetSuperior() const
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchies::GetEntity
// Created: AGE 2006-09-20
// -----------------------------------------------------------------------------
const kernel::Entity_ABC& TeamHierarchies::GetEntity() const
{
    return holder_;
}
