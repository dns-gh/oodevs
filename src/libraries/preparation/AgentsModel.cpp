// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentsModel.h"
#include "AgentFactory_ABC.h"
#include "Agent.h"
#include "Automat.h"
#include "MaintenanceStates.h"
#include "MedicalStates.h"
#include "SupplyStates.h"
#include "Tc2States.h"
#include "LimitsModel.h"
#include "AgentsModelChecker.h"
#include "DiamondFormation.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/AutomatComposition.h"
#include <xeumeuleu/xml.h>
#include <qmessagebox.h>

using namespace kernel;
using namespace xml;

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
    errors_ = "";
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
void AgentsModel::CreateAutomat( xml::xistream& xis, kernel::Entity_ABC& parent, LimitsModel& limits )
{
    try
    {
        Automat_ABC* agent = agentFactory_.Create( xis, parent );
        tools::Resolver< Automat_ABC >::Register( agent->GetId(), *agent );
        xis >> list( "automat", *this , &AgentsModel::CreateAutomat, *agent, limits )
            >> list( "unit"   , *this , &AgentsModel::CreateAgent, *agent )
            >> list( "lima"   , limits, &LimitsModel::CreateLima , *(Entity_ABC*)agent )
            >> list( "limit"  , limits, &LimitsModel::CreateLimit, *(Entity_ABC*)agent );
    }
    catch( std::exception& e )
    {
        errors_ += std::string( e.what() ) + "\n";
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
void AgentsModel::CreateAgent( xml::xistream& xis, Automat_ABC& parent )
{
    try
    {
        Agent_ABC* agent = agentFactory_.Create( xis, parent );
        tools::Resolver< Agent_ABC >::Register( agent->GetId(), *agent );
    }
    catch( std::exception& e )
    {
        errors_ += std::string( e.what() ) + "\n";
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
void AgentsModel::CreatePopulation( Entity_ABC& parent, const PopulationType& type, const geometry::Point2f& position )
{
    Population_ABC* popu = agentFactory_.Create( parent, type, parameters_.Clip( position ) );
    tools::Resolver< Population_ABC >::Register( popu->GetId(), *popu );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::CreatePopulation
// Created: SBO 2006-11-09
// -----------------------------------------------------------------------------
void AgentsModel::CreatePopulation( xml::xistream& xis, kernel::Team_ABC& parent )
{
    Population_ABC* popu = agentFactory_.Create( xis, parent );
    tools::Resolver< Population_ABC >::Register( popu->GetId(), *popu );
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
// Name: AgentsModel::ReadLogistic
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void AgentsModel::ReadLogistic( xml::xistream& xis )
{
    int id;
    xis >> attribute( "id", id );
    if( Automat_ABC* entity = tools::Resolver< Automat_ABC >::Find( id ) )
        xis >> list( "subordinate", *this, &AgentsModel::ReadLogisticLink, *entity );
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::ReadLogisticLink
// Created: SBO 2006-10-26
// -----------------------------------------------------------------------------
void AgentsModel::ReadLogisticLink( xml::xistream& xis, kernel::Automat_ABC& automat )
{
    int id;
    std::string linkType;
    xis >> attribute( "id", id )
        >> attribute( "link", linkType );
    if( Entity_ABC* entity = tools::Resolver< Automat_ABC >::Find( id ) )
    {
        ReadLogisticLink< TC2Hierarchies >        ( xis, linkType, automat, *entity );
        ReadLogisticLink< MedicalHierarchies >    ( xis, linkType, automat, *entity );
        ReadLogisticLink< MaintenanceHierarchies >( xis, linkType, automat, *entity );
        ReadLogisticLink< SupplyHierarchies >     ( xis, linkType, automat, *entity );
    }
}

// -----------------------------------------------------------------------------
// Name: AgentsModel::ReadLogisticLink
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
template< typename H >
void AgentsModel::ReadLogisticLink( xml::xistream& xis, const std::string& type, kernel::Automat_ABC& superior, kernel::Entity_ABC& entity )
{
    H* hierarchies = entity.Retrieve< H >();
    if( hierarchies && hierarchies->GetLinkType().ascii() == type )
        hierarchies->Load( xis, &superior );
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

// -----------------------------------------------------------------------------
// Name: AgentsModel::GetLoadingError
// Created: SBO 2010-07-08
// -----------------------------------------------------------------------------
std::string AgentsModel::GetLoadingErrors() const
{
    return errors_;
}
