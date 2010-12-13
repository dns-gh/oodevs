// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AgentKnowledgeFactory.h"
#include "AgentKnowledge.h"
#include "Model.h"
#include "AgentsModel.h"
#include "PerceptionMap.h"
#include "TeamsModel.h"
#include "PopulationKnowledge.h"
#include "clients_kernel/Controllers.h"
#include "AgentKnowledgePositions.h"
#include "PopulationKnowledgePositions.h"
#include "Lives.h"
#include "Speeds.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeFactory constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
AgentKnowledgeFactory::AgentKnowledgeFactory( Controllers& controllers, Model& model, const CoordinateConverter_ABC& converter )
    : controllers_( controllers )
    , model_( model )
    , converter_( converter )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeFactory destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
AgentKnowledgeFactory::~AgentKnowledgeFactory()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeFactory::CreateAgentKnowledge
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
AgentKnowledge_ABC* AgentKnowledgeFactory::CreateAgentKnowledge( const KnowledgeGroup_ABC& group, const sword::UnitKnowledgeCreation& message )
{
    AgentKnowledge* result = new AgentKnowledge( group, message, controllers_.controller_, converter_, model_.agents_, model_.teams_ );
    result->Attach( *new PerceptionMap( controllers_.controller_, model_.agents_ ) );
    result->Attach< Positions >( *new AgentKnowledgePositions( converter_ ) );
    result->Attach< Lives >( *new Lives( controllers_.controller_ ) );
    result->Attach< Speeds >( *new Speeds( converter_ ) );
    result->Polish();
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeFactory::CreatePopulationKnowledge
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
PopulationKnowledge_ABC* AgentKnowledgeFactory::CreatePopulationKnowledge( const KnowledgeGroup_ABC& group, const sword::CrowdKnowledgeCreation& message )
{
    PopulationKnowledge* result = new PopulationKnowledge( group, controllers_.controller_, converter_, model_.agents_, message );
    result->Attach< Positions >( *new PopulationKnowledgePositions( *result ) );
    result->Polish();
    return result;
}
