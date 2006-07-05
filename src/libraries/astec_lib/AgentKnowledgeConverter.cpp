// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentKnowledgeConverter.h"
#include "AgentKnowledge.h"
#include "Agent.h"
#include "Population.h"
#include "PopulationKnowledge.h"
#include "Controllers.h"

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
const AgentKnowledge* AgentKnowledgeConverter::Find( const AgentKnowledge& base, const KnowledgeGroup& owner )
{
    return Find( base.GetRealAgent(), owner );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const AgentKnowledge* AgentKnowledgeConverter::Find( const Agent& base, const KnowledgeGroup& owner )
{
    return agents_[ &owner ][ &base ];
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const PopulationKnowledge* AgentKnowledgeConverter::Find( const PopulationKnowledge& base, const KnowledgeGroup& owner )
{
    return Find( base.GetRealPopulation(), owner );
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeConverter::Find
// Created: AGE 2006-05-18
// -----------------------------------------------------------------------------
const PopulationKnowledge* AgentKnowledgeConverter::Find( const Population& base, const KnowledgeGroup& owner )
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
