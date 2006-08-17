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
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "gaming/InternalLinks.h"
#include "gaming/ObjectKnowledge.h"
#include "gaming/AgentKnowledge.h"
#include "gaming/PopulationKnowledge.h"

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
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyCreated( const Agent_ABC& agent )
{
    AddEntity( InternalLinks::agent_, agent );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyDeleted
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyDeleted( const Agent_ABC& agent )
{
    RemoveEntity( InternalLinks::agent_, agent );
}
    
// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyCreated
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyCreated( const Population_ABC& popu )
{
    AddEntity( InternalLinks::population_, popu );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyDeleted
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyDeleted( const Population_ABC& popu )
{
    RemoveEntity( InternalLinks::population_, popu );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyCreated
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyCreated( const Object_ABC& object )
{
    AddEntity( InternalLinks::object_, object );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyDeleted
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyDeleted( const Object_ABC& object )
{
    RemoveEntity( InternalLinks::object_, object );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyCreated
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyCreated( const AgentKnowledge& k )
{
    AddEntity( InternalLinks::agentKnowledge_, k );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyDeleted
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyDeleted( const AgentKnowledge& k )
{
    RemoveEntity( InternalLinks::agentKnowledge_, k );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyCreated
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyCreated( const PopulationKnowledge& k )
{
    AddEntity( InternalLinks::populationKnowledge_, k );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyDeleted
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyDeleted( const PopulationKnowledge& k )
{
    RemoveEntity( InternalLinks::populationKnowledge_, k );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyCreated
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyCreated( const ObjectKnowledge& k )
{
    AddEntity( InternalLinks::objectKnowledge_, k );
}

// -----------------------------------------------------------------------------
// Name: LinkInterpreter::NotifyDeleted
// Created: AGE 2006-08-11
// -----------------------------------------------------------------------------
void LinkInterpreter::NotifyDeleted( const ObjectKnowledge& k )
{
    RemoveEntity( InternalLinks::objectKnowledge_, k );
}

