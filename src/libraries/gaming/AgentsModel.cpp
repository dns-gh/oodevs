// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentsModel.h"
#include "DIN/DIN_Input.h"
#include "AgentFactory_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Automat_ABC.h"

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
    Resolver< Automat_ABC >::DeleteAll();
    Resolver< Population_ABC >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAutomat
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AgentsModel::CreateAutomat( const ASN1T_MsgAutomatCreation& asnMsg )
{
    if( !Resolver< Automat_ABC >::Find( asnMsg.oid_automate ) )
    {
        Automat_ABC* pAgent = agentFactory_.Create( asnMsg );
        Resolver< Automat_ABC >::Register( asnMsg.oid_automate, *pAgent );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetAutomat
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
kernel::Automat_ABC& AgentsModel::GetAutomat( unsigned long id )
{
    return Resolver< Automat_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAutomat
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentsModel::FindAutomat( unsigned long id )
{
    return Resolver< Automat_ABC >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void AgentsModel::CreateAgent( const ASN1T_MsgUnitCreation& asnMsg )
{
    if( !Resolver< Agent_ABC >::Find( asnMsg.oid_pion ) )
    {
        Agent_ABC* pAgent = agentFactory_.Create( asnMsg );
        Resolver< Agent_ABC >::Register( asnMsg.oid_pion, *pAgent );
    }
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
// Name: AgentsModel::DestroyAgent
// Created: AGE 2007-04-24
// -----------------------------------------------------------------------------
void AgentsModel::DestroyAgent( const ASN1T_MsgUnitDestruction& id )
{
    delete Resolver< Agent_ABC >::Find( id );
    Resolver< Agent_ABC >::Remove( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAllAgent
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Entity_ABC* AgentsModel::FindAllAgent( unsigned long id ) const
{
    Entity_ABC* agent = Resolver< Automat_ABC >::Find( id );
    if( agent )
        return agent;
    if( agent = Resolver< Agent_ABC >::Find( id ) )
        return agent;
    return Resolver< Population_ABC >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreatePopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void AgentsModel::CreatePopulation( const ASN1T_MsgPopulationCreation& asnMsg )
{
    if( !Resolver< Population_ABC >::Find( asnMsg.oid_population ) )
    {
        Population_ABC* popu = agentFactory_.Create( asnMsg );
        Resolver< Population_ABC >::Register( asnMsg.oid_population, *popu );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetPopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population_ABC& AgentsModel::GetPopulation( unsigned long id )
{
    return Resolver< Population_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindPopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population_ABC* AgentsModel::FindPopulation( unsigned long id )
{
    return Resolver< Population_ABC>::Find( id );
}

