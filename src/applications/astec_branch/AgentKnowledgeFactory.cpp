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

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeFactory constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
AgentKnowledgeFactory::AgentKnowledgeFactory( Controller& controller, Model& model )
    : controller_( controller )
    , model_( model )
{

}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeFactory destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
AgentKnowledgeFactory::~AgentKnowledgeFactory()
{

}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeFactory::CreateAgentKnowledge
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
AgentKnowledge* AgentKnowledgeFactory::CreateAgentKnowledge( const ASN1T_MsgUnitKnowledgeCreation& message )
{
    AgentKnowledge* result = new AgentKnowledge( message, controller_, model_.agents_, model_.teams_ );
    result->Attach( *new PerceptionMap( controller_, model_.agents_ ) );
    return result;
}

// -----------------------------------------------------------------------------
// Name: AgentKnowledgeFactory::CreatePopulationKnowledge
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
PopulationKnowledge* AgentKnowledgeFactory::CreatePopulationKnowledge( const ASN1T_MsgPopulationKnowledgeCreation& message )
{
    return new PopulationKnowledge( controller_, model_.agents_, message );
}
