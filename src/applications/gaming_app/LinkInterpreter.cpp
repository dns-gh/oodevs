// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LinkInterpreter.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Entity_ABC.h"
#include "gaming/AgentKnowledge_ABC.h"
#include "gaming/ObjectKnowledge_ABC.h"
#include "gaming/PopulationKnowledge_ABC.h"

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: LinkInterpreter constructor
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
LinkInterpreter::LinkInterpreter( QObject* parent, Controllers& controllers )
     : LinkInterpreter_ABC( parent, controllers.actions_ )
     , controllers_       ( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter destructor
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
LinkInterpreter::~LinkInterpreter()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyCreated
// Created: SBO 2006-10-13
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyCreated( const kernel::Entity_ABC& entity )
{
    AddEntity( entity.GetTypeName(), entity );
}
    
// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyDeleted
// Created: SBO 2006-10-13
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyDeleted( const kernel::Entity_ABC& entity )
{
    RemoveEntity( entity.GetTypeName(), entity );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyCreated
// Created: SBO 2006-10-12
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyCreated( const kernel::Agent_ABC& entity )
{
    NotifyCreated( (const Entity_ABC&)entity );
}
    
// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyDeleted
// Created: SBO 2006-10-12
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyDeleted( const kernel::Agent_ABC& entity )
{
    NotifyDeleted( (const Entity_ABC&)entity );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyCreated
// Created: SBO 2006-10-13
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyCreated( const kernel::Population_ABC& entity )
{
    NotifyCreated( (const Entity_ABC&)entity );
}
    
// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyDeleted
// Created: SBO 2006-10-13
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyDeleted( const kernel::Population_ABC& entity )
{
    NotifyDeleted( (const Entity_ABC&)entity );
}
    
// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyCreated
// Created: SBO 2006-10-13
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyCreated( const kernel::Object_ABC& entity )
{
    NotifyCreated( (const Entity_ABC&)entity );
}
    
// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyDeleted
// Created: SBO 2006-10-13
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyDeleted( const kernel::Object_ABC& entity )
{
    NotifyDeleted( (const Entity_ABC&)entity );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyCreated
// Created: SBO 2006-10-13
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyCreated( const AgentKnowledge_ABC& entity )
{
    NotifyCreated( (const Entity_ABC&)entity );
}
    
// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyDeleted
// Created: SBO 2006-10-13
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyDeleted( const AgentKnowledge_ABC& entity )
{
    NotifyDeleted( (const Entity_ABC&)entity );
}
    
// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyCreated
// Created: SBO 2006-10-13
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyCreated( const ObjectKnowledge_ABC& entity )
{
    NotifyCreated( (const Entity_ABC&)entity );
}
    
// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyDeleted
// Created: SBO 2006-10-13
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyDeleted( const ObjectKnowledge_ABC& entity )
{
    NotifyDeleted( (const Entity_ABC&)entity );
}
    
// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyCreated
// Created: SBO 2006-10-13
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyCreated( const PopulationKnowledge_ABC& entity )
{
    NotifyCreated( (const Entity_ABC&)entity );
}
    
// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyDeleted
// Created: SBO 2006-10-13
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyDeleted( const PopulationKnowledge_ABC& entity )
{
    NotifyDeleted( (const Entity_ABC&)entity );
}
