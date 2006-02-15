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
    return new AgentKnowledge( message, controller_, model_.agents_ );
}
