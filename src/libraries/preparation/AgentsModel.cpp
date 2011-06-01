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
#include "Agent.h"
#include "Automat.h"
#include "LogisticBaseStates.h"
#include "LimitsModel.h"
#include "AgentsModelChecker.h"
#include "DiamondFormation.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/AutomatComposition.h"
#include <xeumeuleu/xml.hpp>
#include <qmessagebox.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentsModel constructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
AgentsModel::AgentsModel( Controllers& controllers, AgentFactory_ABC& agentFactory )
    : agentFactory_( agentFactory )
    , controllers_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel destructor
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
AgentsModel::~AgentsModel()
{
    Purge();
    controllers_.Unregister( *this );
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
// Created: AGE 2007-05-29
// -----------------------------------------------------------------------------
kernel::Automat_ABC& AgentsModel::CreateAutomat( kernel::Entity_ABC& parent, const kernel::AutomatType& type, const QString& name )
{
    Automat_ABC* agent = agentFactory_.Create( parent, type, name );
    tools::Resolver< Automat_ABC >::Register( agent->GetId(), *agent );
    return *agent;
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAutomat
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsModel::CreateAutomat( kernel::Entity_ABC& parent, const kernel::AutomatType& type, const geometry::Point2f& position )
{
    kernel::Automat_ABC& automat = CreateAutomat( parent, type );
    DiamondFormation formation( position );
    bool pcSet = false;
    tools::Iterator< const AutomatComposition& > it = type.CreateIterator();
    while( it.HasMoreElements() )
    {
        const AutomatComposition& composition = it.NextElement();
        for( unsigned toAdd = composition.GetSensibleNumber(); toAdd > 0; --toAdd )
        {
            const bool isPc = !pcSet && &composition.GetType() == type.GetTypePC();
            CreateAgent( automat, composition.GetType(), formation.NextPosition(), isPc );
            if( isPc )
                pcSet = true;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAutomat
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsModel::CreateAutomat( xml::xistream& xis, kernel::Entity_ABC& parent, LimitsModel& limits, std::string& loadingErrors )
{
    try
    {
        Automat_ABC* agent = agentFactory_.Create( xis, parent );
        tools::Resolver< Automat_ABC >::Register( agent->GetId(), *agent );
        xis >> xml::list( "automat", *this , &AgentsModel::CreateAutomat, *agent, limits, loadingErrors )
            >> xml::list( "unit"   , *this , &AgentsModel::CreateAgent, *agent, loadingErrors )
            >> xml::list( "lima"   , limits, &LimitsModel::CreateLima , *(Entity_ABC*)agent )
            >> xml::list( "limit"  , limits, &LimitsModel::CreateLimit, *(Entity_ABC*)agent );
    }
    catch( std::exception& e )
    {
        loadingErrors += std::string( e.what() ) + "\n";
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetAutomat
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
kernel::Automat_ABC& AgentsModel::GetAutomat( unsigned long id )
{
    return tools::Resolver< Automat_ABC >::Get( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::FindAutomat
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
kernel::Automat_ABC* AgentsModel::FindAutomat( unsigned long id )
{
    return tools::Resolver< Automat_ABC >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAgent
// Created: AGE 2006-02-10
// -----------------------------------------------------------------------------
kernel::Agent_ABC& AgentsModel::CreateAgent( Automat_ABC& parent, const AgentType& type, const geometry::Point2f& position, bool commandPost, const QString& name )
{
    Agent_ABC* agent = agentFactory_.Create( parent, type, parameters_.Clip( position ), commandPost, name );
    tools::Resolver< Agent_ABC >::Register( agent->GetId(), *agent );
    return *agent;
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateAgent
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
void AgentsModel::CreateAgent( xml::xistream& xis, Automat_ABC& parent, std::string& loadingErrors )
{
    try
    {
        Agent_ABC* agent = agentFactory_.Create( xis, parent );
        tools::Resolver< Agent_ABC >::Register( agent->GetId(), *agent );
    }
    catch( std::exception& e )
    {
        loadingErrors += std::string( e.what() ) + "\n";
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
    return tools::Resolver< Population_ABC >::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreatePopulation
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void AgentsModel::CreatePopulation( Entity_ABC& parent, const PopulationType& type, int number, const geometry::Point2f& position )
{
    Population_ABC* popu = agentFactory_.Create( parent, type, number, parameters_.Clip( position ) );
    tools::Resolver< Population_ABC >::Register( popu->GetId(), *popu );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreatePopulation
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void AgentsModel::CreatePopulation( xml::xistream& xis, kernel::Team_ABC& parent, std::string& loadingErrors )
{
    try
    {
        Population_ABC* popu = agentFactory_.CreatePop( xis, parent );
        tools::Resolver< Population_ABC >::Register( popu->GetId(), *popu );
    }
    catch( std::exception& e )
    {
        loadingErrors += std::string( e.what() ) + "\n";
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
//
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
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void AgentsModel::CreateInhabitant( Entity_ABC& parent, const InhabitantType& type, int number, const QString& name, const kernel::Location_ABC& location )
{
    Inhabitant_ABC* inhab = agentFactory_.Create( parent, type, number, name, location );
    if( inhab )
        tools::Resolver< Inhabitant_ABC >::Register( inhab->GetId(), *inhab );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreateInhabitant
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void AgentsModel::CreateInhabitant( xml::xistream& xis, kernel::Team_ABC& parent, std::string& loadingErrors )
{
    try
    {
        Inhabitant_ABC* inhab = agentFactory_.CreateInhab( xis, parent );
        tools::Resolver< Inhabitant_ABC >::Register( inhab->GetId(), *inhab );
    }
    catch( std::exception& e )
    {
        loadingErrors += std::string( e.what() ) + "\n";
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetInhabitant
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Inhabitant_ABC& AgentsModel::GetInhabitant( unsigned long id )
{
    return tools::Resolver< Inhabitant_ABC >::Get( id );
}
//
// -----------------------------------------------------------------------------
// Name: AgentsModel::FindInhabitant
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
Inhabitant_ABC* AgentsModel::FindInhabitant( unsigned long id )
{
    return tools::Resolver< Inhabitant_ABC>::Find( id );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::NotifyDeleted
// Created: SBO 2006-09-04
// -----------------------------------------------------------------------------
void AgentsModel::NotifyDeleted( const Agent_ABC& agent )
{
    tools::Resolver< Agent_ABC >::Remove( agent.GetId() );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::NotifyDeleted
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsModel::NotifyDeleted( const kernel::Automat_ABC& agent )
{
    tools::Resolver< Automat_ABC >::Remove( agent.GetId() );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::NotifyDeleted
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
void AgentsModel::NotifyDeleted( const kernel::Population_ABC& agent )
{
    tools::Resolver< Population_ABC >::Remove( agent.GetId() );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::NotifyDeleted
// Created: SLG 2010-11-23
// -----------------------------------------------------------------------------
void AgentsModel::NotifyDeleted( const kernel::Inhabitant_ABC& agent )
{
    tools::Resolver< Inhabitant_ABC >::Remove( agent.GetId() );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CheckValidity
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
bool AgentsModel::CheckValidity( ModelChecker_ABC& checker ) const
{
    AgentsModelChecker agentChecker;
    return agentChecker.Check( *this, checker );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::NotifyUpdated
// Created: AGE 2007-05-30
// -----------------------------------------------------------------------------
void AgentsModel::NotifyUpdated( const kernel::ModelLoaded& model )
{
    parameters_.Load( model.config_ );
}
