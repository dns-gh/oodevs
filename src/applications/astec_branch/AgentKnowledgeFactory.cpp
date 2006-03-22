// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "AgentKnowledgeFactory.h"
#include "AgentKnowledge.h"
#include "Model.h"
#include "AgentsModel.h"
#include "PerceptionMap.h"
#include "TeamsModel.h"
#include "PopulationKnowledge.h"
#include "Controllers.h"

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeFactory constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
AgentKnowledgeFactory::AgentKnowledgeFactory( Controllers& controllers, Model& model )
    : controllers_( controllers )
    , model_( model )
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
AgentKnowledge* AgentKnowledgeFactory::CreateAgentKnowledge( const ASN1T_MsgUnitKnowledgeCreation& message )
{
    AgentKnowledge* result = new AgentKnowledge( message, controllers_.controller_, model_.coordinateConverter_, model_.agents_, model_.teams_ );
    result->Attach( *new PerceptionMap( controllers_.controller_, model_.agents_ ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeFactory::CreatePopulationKnowledge
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
PopulationKnowledge* AgentKnowledgeFactory::CreatePopulationKnowledge( const ASN1T_MsgPopulationKnowledgeCreation& message )
{
    return new PopulationKnowledge( controllers_.controller_, model_.coordinateConverter_, model_.agents_, message );
}
