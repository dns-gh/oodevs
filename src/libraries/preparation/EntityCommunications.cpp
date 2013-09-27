// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "EntityCommunications.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EntityCommunications constructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
EntityCommunications::EntityCommunications( Controller& controller, Entity_ABC& holder, Entity_ABC* superior )
    : EntityHierarchies< CommunicationHierarchies >( controller, holder, superior )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies destructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
EntityCommunications::~EntityCommunications()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: EntityCommunications::ChangeSuperior
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void EntityCommunications::ChangeSuperior( kernel::Entity_ABC& superior )
{
    EntityHierarchies< CommunicationHierarchies >::ChangeSuperior( &superior );
}

// -----------------------------------------------------------------------------
// Name: EntityCommunications::CanCommunicate
// Created: LDC 2010-04-07
// -----------------------------------------------------------------------------
bool EntityCommunications::CanCommunicate() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: EntityCommunications::CanReceive
// Created: LDC 2013-09-27
// -----------------------------------------------------------------------------
bool EntityCommunications::CanReceive() const
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: EntityCommunications::IsJammed
// Created: HBD 2010-07-30
// -----------------------------------------------------------------------------
bool EntityCommunications::IsJammed() const
{
    return false;
}
