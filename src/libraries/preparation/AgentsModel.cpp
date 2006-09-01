// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentsModel.h"
#include "AgentFactory_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
AgentsModel::AgentsModel( AgentFactory_ABC& agentFactory )
    : agentFactory_( agentFactory )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentsModel destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
AgentsModel::~AgentsModel()
{
    Purge();
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void AgentsModel::Purge()
{
    Resolver< Agent_ABC >::DeleteAll();
    Resolver< Population_ABC >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void AgentsModel::CreateAgent( const Agent_ABC& parent, const AgentType& type, const geometry::Point2f& position )
{
    Agent_ABC* agent = agentFactory_.Create( parent, type, position );
    Resolver< Agent_ABC >::Register( agent->GetId(), *agent );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAgent
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
void AgentsModel::CreateAgent( const KnowledgeGroup_ABC& parent, const AutomatType& type, const geometry::Point2f& position )
{
    Agent_ABC* agent = agentFactory_.Create( parent, type, position );
    Resolver< Agent_ABC >::Register( agent->GetId(), *agent );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Agent_ABC& AgentsModel::GetAgent( unsigned long id ) const
{
    return Resolver< Agent_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAgent
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent_ABC* AgentsModel::FindAgent( unsigned long id ) const
{
    return Resolver< Agent_ABC >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAllAgent
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Entity_ABC* AgentsModel::FindAllAgent( unsigned long id ) const
{
    Entity_ABC* agent = Resolver< Agent_ABC >::Find( id );
    if( ! agent )
        agent = Resolver< Population_ABC >::Find( id );
    return agent;
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreatePopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void AgentsModel::CreatePopulation( const Team_ABC& parent, const PopulationType& type, const geometry::Point2f& position )
{
    Population_ABC* popu = agentFactory_.Create( parent, type, position );
    Resolver< Population_ABC >::Register( popu->GetId(), *popu );
}
//
// -----------------------------------------------------------------------------
// Name: AgentsModel::GetPopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population_ABC& AgentsModel::GetPopulation( unsigned long id )
{
    return Resolver< Population_ABC >::Get( id );
}
//
// -----------------------------------------------------------------------------
// Name: AgentsModel::FindPopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population_ABC* AgentsModel::FindPopulation( unsigned long id )
{
    return Resolver< Population_ABC>::Find( id );
}

