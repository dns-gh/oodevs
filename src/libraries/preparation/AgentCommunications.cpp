// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentCommunications.h"

// -----------------------------------------------------------------------------
// Name: AgentCommunications constructor
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
AgentCommunications::AgentCommunications( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior )
    : EntityCommunications( controller, holder, superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentCommunications destructor
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
AgentCommunications::~AgentCommunications()
{
    // NOTHING
}
