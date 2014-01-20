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
#include "AgentFactory_ABC.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/PopulationConcentration_ABC.h"
#include "clients_kernel/PopulationFlow_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "protocol/Protocol.h"

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
    tools::Resolver< Agent_ABC >::DeleteAll();
    tools::Resolver< Automat_ABC >::DeleteAll();
    tools::Resolver< Population_ABC >::DeleteAll();
    tools::Resolver< Inhabitant_ABC >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAutomat
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void AgentsModel::CreateAutomat( const sword::AutomatCreation& message )
{
    if( !Resolver< Automat_ABC >::Find( message.automat().id() ) )
    {
        Automat_ABC* pAgent = agentFactory_.Create( message );
        tools::Resolver< Automat_ABC >::Register( message.automat().id(), *pAgent );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetAutomat
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
kernel::Automat_ABC& AgentsModel::GetAutomat( unsigned long id )
{
    return tools::Resolver< Automat_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAutomat
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentsModel::FindAutomat( unsigned long id )
{
    return tools::Resolver< Automat_ABC >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
void AgentsModel::CreateAgent( const sword::UnitCreation& message )
{
    if( !Resolver< Agent_ABC >::Find( message.unit().id() ) )
    {
        Agent_ABC* pAgent = agentFactory_.Create( message );
        tools::Resolver< Agent_ABC >::Register( message.unit().id(), *pAgent );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
Agent_ABC& AgentsModel::GetAgent( unsigned long id ) const
{
    return tools::Resolver< Agent_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAgent
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent_ABC* AgentsModel::FindAgent( unsigned long id ) const
{
    return tools::Resolver< Agent_ABC >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::DestroyAgent
// Created: AGE 2007-04-24
// -----------------------------------------------------------------------------
void AgentsModel::DestroyAgent( const sword::UnitDestruction& message )
{
    for( auto it = tools::Resolver< Agent_ABC >::CreateIterator(); it.HasMoreElements(); )
        const_cast< kernel::Agent_ABC& >( it.NextElement() ).Update( message );
    tools::Resolver< Agent_ABC >::Delete( message.unit().id() );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::DestroyAutomat
// Created: LDC 2010-11-02
// -----------------------------------------------------------------------------
void AgentsModel::DestroyAutomat( const sword::AutomatDestruction& message )
{
    tools::Resolver< Automat_ABC >::Delete( message.automat().id() );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::DestroyCrowd
// Created: LDC 2010-11-02
// -----------------------------------------------------------------------------
void AgentsModel::DestroyCrowd( const sword::CrowdDestruction& message )
{
    tools::Resolver< Population_ABC >::Delete( message.crowd().id() );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAllAgent
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Entity_ABC* AgentsModel::FindAllAgent( unsigned long id ) const
{
    Entity_ABC* agent = tools::Resolver< Automat_ABC >::Find( id );
    if( agent )
        return agent;
    agent = tools::Resolver< Agent_ABC >::Find( id );
    if( agent )
        return agent;
    agent = tools::Resolver< Population_ABC >::Find( id );
    if( agent )
        return agent;
    return tools::Resolver< Inhabitant_ABC >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreatePopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void AgentsModel::CreatePopulation( const sword::CrowdCreation& message )
{
    if( !Resolver< Population_ABC >::Find( message.crowd().id() ) )
    {
        Population_ABC* popu = agentFactory_.Create( message );
        tools::Resolver< Population_ABC >::Register( message.crowd().id(), *popu );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetPopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population_ABC& AgentsModel::GetPopulation( unsigned long id )
{
    return tools::Resolver< Population_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindPopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population_ABC* AgentsModel::FindPopulation( unsigned long id )
{
    return tools::Resolver< Population_ABC>::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateInhabitant
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
void AgentsModel::CreateInhabitant( const sword::PopulationCreation& message )
{
    if( !Resolver< Inhabitant_ABC >::Find( message.id().id() ) )
    {
        Inhabitant_ABC* inhab = agentFactory_.Create( message );
        tools::Resolver< Inhabitant_ABC >::Register( message.id().id(), *inhab );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetInhabitant
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
Inhabitant_ABC& AgentsModel::GetInhabitant( unsigned long id )
{
    return tools::Resolver< Inhabitant_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindInhabitant
// Created: SLG 2010-11-29
// -----------------------------------------------------------------------------
Inhabitant_ABC* AgentsModel::FindInhabitant( unsigned long id )
{
    return tools::Resolver< Inhabitant_ABC>::Find( id );
}

// -----------------------------------------------------------------------------
// Name: PopulationPartResolver::Find
// Created: LDC 2012-06-19
// -----------------------------------------------------------------------------
kernel::PopulationPart_ABC* PopulationPartResolver::Find( const unsigned long& id ) const
{
    return FindPopulationPart( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindPopulationPart
// Created: LDC 2012-06-19
// -----------------------------------------------------------------------------
kernel::PopulationPart_ABC* AgentsModel::FindPopulationPart( const unsigned long& id ) const
{
    tools::Iterator< const kernel::Population_ABC& > populations = this->tools::Resolver< kernel::Population_ABC >::CreateIterator();
    while( populations.HasMoreElements() )
    {
        const kernel::Population_ABC& next = populations.NextElement();
        const kernel::PopulationFlow_ABC* flow = next.FindFlow( id );
        if( flow )
            return const_cast< kernel::PopulationFlow_ABC* >( flow );
        const kernel::PopulationConcentration_ABC* concentration = next.FindConcentration( id );
        if( concentration )
            return const_cast< kernel::PopulationConcentration_ABC* >( concentration );
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationPartResolver::Get
// Created: LDC 2012-06-19
// -----------------------------------------------------------------------------
kernel::PopulationPart_ABC& PopulationPartResolver::Get( const unsigned long& id ) const
{
    kernel::PopulationPart_ABC* element = Find( id );
    if( !element )
        throw MASA_EXCEPTION( "Element does not exist" );
    return *element;
}

// -----------------------------------------------------------------------------
// Name: Iterator< const kernel::PopulationPart_ABC& > PopulationPartResolver::CreateIterator
// Created: LDC 2012-06-19
// -----------------------------------------------------------------------------
tools::Iterator< const kernel::PopulationPart_ABC& > PopulationPartResolver::CreateIterator() const
{
    throw MASA_EXCEPTION( "Not implemented" );
}