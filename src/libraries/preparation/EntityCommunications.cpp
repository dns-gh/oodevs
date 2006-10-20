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
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/Controller.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: EntityCommunications constructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
EntityCommunications::EntityCommunications( Controller& controller, Entity_ABC& holder, Entity_ABC* superior )
    : DelayedEntityHierarchies< CommunicationHierarchies >( controller, holder, superior )
    , controller_( controller )
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
    controller_.Delete( *(CommunicationHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityCommunications::DoUpdate
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void EntityCommunications::DoUpdate( const kernel::InstanciationComplete& ic )
{
    DelayedEntityHierarchies< CommunicationHierarchies >::DoUpdate( ic );
    controller_.Create( *(CommunicationHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityCommunications::ChangeSuperior
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void EntityCommunications::ChangeSuperior( kernel::Entity_ABC& superior )
{
    SetSuperior( &superior );
    controller_.Update( *(CommunicationHierarchies*)this );
}
