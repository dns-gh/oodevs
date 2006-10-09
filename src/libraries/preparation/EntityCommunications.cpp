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
    : kernel::EntityHierarchies< CommunicationHierarchies >( controller, holder )
    , controller_( controller )
{
    if( superior )
        SetSuperior( superior );
}

// -----------------------------------------------------------------------------
// Name: CommunicationHierarchies destructor
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
EntityCommunications::~EntityCommunications()
{
    DeleteAll(); // $$$$ SBO 2006-10-09: 
    if( GetSuperior() )
        if( CommunicationHierarchies* hierarchies = GetSuperior()->Retrieve< CommunicationHierarchies >() )
            hierarchies->UnregisterSubordinate( GetEntity() );
    controller_.Delete( *(CommunicationHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityCommunications::DoUpdate
// Created: SBO 2006-09-25
// -----------------------------------------------------------------------------
void EntityCommunications::DoUpdate( const kernel::InstanciationComplete& )
{
    RegisterToSuperior();
    controller_.Create( *(CommunicationHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityCommunications::ChangeSuperior
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void EntityCommunications::ChangeSuperior( kernel::Entity_ABC& superior )
{
    RemoveFromSuperior();
    SetSuperior( &superior );
    RegisterToSuperior();
    controller_.Update( *(CommunicationHierarchies*)this );
}

// -----------------------------------------------------------------------------
// Name: EntityCommunications::RegisterToSuperior
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void EntityCommunications::RegisterToSuperior()
{
    if( GetSuperior() )
        if( CommunicationHierarchies* hierarchies = GetSuperior()->Retrieve< CommunicationHierarchies >() )
            hierarchies->AddSubordinate( GetEntity() );
}
    
// -----------------------------------------------------------------------------
// Name: EntityCommunications::RemoveFromSuperior
// Created: SBO 2006-09-26
// -----------------------------------------------------------------------------
void EntityCommunications::RemoveFromSuperior()
{
    if( GetSuperior() )
        if( CommunicationHierarchies* hierarchies = GetSuperior()->Retrieve< CommunicationHierarchies >() )
            hierarchies->RemoveSubordinate( GetEntity() );
}