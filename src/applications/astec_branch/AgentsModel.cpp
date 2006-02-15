// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentsModel.h"
#include "DIN/DIN_Input.h"
#include "AgentFactory_ABC.h"
#include "Agent.h"
#include "Population.h"

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
    Resolver< Agent >::DeleteAll();
    Resolver< Population >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void AgentsModel::CreateAgent( const ASN1T_MsgAutomateCreation& asnMsg )
{
    if( !Resolver< Agent >::Find( asnMsg.oid_automate ) )
    {
        Agent* pAgent = agentFactory_.Create( asnMsg );
        Resolver< Agent >::Register( asnMsg.oid_automate, *pAgent );
//        application_.NotifyAgentCreated( *pAgent );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void AgentsModel::CreateAgent( const ASN1T_MsgPionCreation& asnMsg )
{
    if( !Resolver< Agent >::Find( asnMsg.oid_pion ) )
    {
        Agent* pAgent = agentFactory_.Create( asnMsg );
        Resolver< Agent >::Register( asnMsg.oid_pion, *pAgent );
//        application_.NotifyAgentCreated( *pAgent );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Agent& AgentsModel::GetAgent( unsigned long id ) const
{
    return Resolver< Agent >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAgent
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent* AgentsModel::FindAgent( unsigned long id ) const
{
    return Resolver< Agent >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAllAgent
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent_ABC* AgentsModel::FindAllAgent( unsigned long id ) const
{
    Agent_ABC* agent = Resolver< Agent >::Find( id );
    if( ! agent )
        agent = Resolver< Population >::Find( id );
    return agent;
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreatePopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void AgentsModel::CreatePopulation( const ASN1T_MsgPopulationCreation& asnMsg )
{
    if( !Resolver< Population >::Find( asnMsg.oid_population ) )
    {
        Population* popu = agentFactory_.Create( asnMsg );
        Resolver< Population >::Register( asnMsg.oid_population, *popu );
//        application_.NotifyPopulationCreated( *popu );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetPopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population& AgentsModel::GetPopulation( unsigned long id )
{
    return Resolver< Population >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindPopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population* AgentsModel::FindPopulation( unsigned long id )
{
    return Resolver< Population >::Find( id );
}

