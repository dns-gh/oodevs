// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "EntityCommunications.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controller.h"

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
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: EntityCommunications::ChangeSuperior
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void EntityCommunications::ChangeSuperior( kernel::Entity_ABC& superior )
{
    EntityHierarchies< CommunicationHierarchies >::ChangeSuperior( &superior );
}
