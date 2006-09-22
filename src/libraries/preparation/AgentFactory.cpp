// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "AgentFactory.h"

#include "Model.h"
#include "clients_kernel/AgentTypes.h"
#include "Agent.h"

#include "AgentsModel.h"
#include "KnowledgeGroupsModel.h"
#include "AgentFactory.h"
#include "AutomatDecisions.h"
#include "AgentPositions.h"
#include "AgentHierarchies.h"
#include "TeamHierarchy.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/InstanciationComplete.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/DataDictionary.h"
#include "StaticModel.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentFactory constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
AgentFactory::AgentFactory( Controllers& controllers, Model& model, const StaticModel& staticModel )
    : controllers_( controllers )
    , model_( model )
    , static_( staticModel )
{
    // NOTHING
}
    
// -----------------------------------------------------------------------------
// Name: AgentFactory destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
AgentFactory::~AgentFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Agent_ABC* AgentFactory::Create( Agent_ABC& parent, const AgentType& type, const geometry::Point2f& position )
{
    Agent* result = new Agent( parent, type, controllers_.controller_ );
    DataDictionary& dico = result->Get< DataDictionary >();
    result->Attach< Positions >( *new AgentPositions( *result, static_.coordinateConverter_, position ) );
    result->Attach( *new TeamHierarchy( parent.GetTeam() ) );
    result->Attach< Hierarchies >( *new AgentHierarchies( controllers_.controller_, *result, &parent ) );
    AttachExtensions( *result );
    result->Update( InstanciationComplete() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: SBO 2006-09-01
// -----------------------------------------------------------------------------
kernel::Agent_ABC* AgentFactory::Create( Formation_ABC& parent, const AutomatType& type, const geometry::Point2f& position )
{
    Agent* result = new Agent( parent, type, controllers_.controller_ );
    DataDictionary& dico = result->Get< DataDictionary >();
    result->Attach< Positions >( *new AgentPositions( *result, static_.coordinateConverter_, position ) );
    result->Attach( *new AutomatDecisions( controllers_.controller_, *result ) );

    if( const TeamHierarchy* team = parent.Retrieve< TeamHierarchy >() )
        result->Attach( *new TeamHierarchy( team->GetTeam() ) );
    result->Attach< Hierarchies >( *new AgentHierarchies( controllers_.controller_, *result, &parent ) );
    AttachExtensions( *result );
    result->Update( InstanciationComplete() );
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::Create
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
Population_ABC* AgentFactory::Create( Team_ABC& parent, const PopulationType& type, const geometry::Point2f& position )
{
    return 0;
}

// -----------------------------------------------------------------------------
// Name: AgentFactory::AttachExtensions
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void AgentFactory::AttachExtensions( Entity_ABC& agent )
{

}

