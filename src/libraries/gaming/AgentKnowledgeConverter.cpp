// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentKnowledgeConverter.h"
#include "AgentKnowledge.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "PopulationKnowledge.h"
#include "clients_kernel/Controllers.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter constructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
AgentKnowledgeConverter::AgentKnowledgeConverter( Controllers& controllers )
    : controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter destructor
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
AgentKnowledgeConverter::~AgentKnowledgeConverter()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const AgentKnowledge* AgentKnowledgeConverter::Find( const AgentKnowledge& base, const KnowledgeGroup_ABC& owner )
{
    return Find( base.GetRealAgent(), owner );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const AgentKnowledge* AgentKnowledgeConverter::Find( const Agent_ABC& base, const KnowledgeGroup_ABC& owner )
{
    return agents_[ &owner ][ &base ];
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const PopulationKnowledge* AgentKnowledgeConverter::Find( const PopulationKnowledge& base, const KnowledgeGroup_ABC& owner )
{
    return Find( base.GetRealPopulation(), owner );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const PopulationKnowledge* AgentKnowledgeConverter::Find( const Population_ABC& base, const KnowledgeGroup_ABC& owner )
{
    return populations_[ &owner ][ &base ];
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::NotifyCreated
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void AgentKnowledgeConverter::NotifyCreated( const AgentKnowledge& knowledge )
{
    agents_[ & knowledge.GetKnowledgeGroup() ][ & knowledge.GetRealAgent() ] = &knowledge;
}
    
// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::NotifyDeleted
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void AgentKnowledgeConverter::NotifyDeleted( const AgentKnowledge& knowledge )
{
    agents_[ & knowledge.GetKnowledgeGroup() ].erase( & knowledge.GetRealAgent() );
}
    
// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::NotifyCreated
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void AgentKnowledgeConverter::NotifyCreated( const PopulationKnowledge& knowledge )
{
    populations_[ & knowledge.GetKnowledgeGroup() ][ & knowledge.GetRealPopulation() ] = &knowledge;
}
    
// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::NotifyDeleted
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
void AgentKnowledgeConverter::NotifyDeleted( const PopulationKnowledge& knowledge )
{
    populations_[ & knowledge.GetKnowledgeGroup() ].erase( & knowledge.GetRealPopulation() );
}
