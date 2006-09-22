// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "TeamHierarchy.h"

// -----------------------------------------------------------------------------
// Name: TeamHierarchy constructor
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
TeamHierarchy::TeamHierarchy( const kernel::Team_ABC& team )
    : team_( team )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: TeamHierarchy destructor
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
TeamHierarchy::~TeamHierarchy()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TeamHierarchy::GetTeam
// Created: SBO 2006-09-22
// -----------------------------------------------------------------------------
const kernel::Team_ABC& TeamHierarchy::GetTeam() const
{
    return team_;
}
