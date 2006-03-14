// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "Model.h"
#include "AgentsModel.h"
#include "ObjectsModel.h"
#include "TeamsModel.h"
#include "LogisticsModel.h"
#include "LimitsModel.h"
#include "AgentFactory.h"
#include "ObjectFactory.h"
#include "AgentTypes.h"
#include "ObjectKnowledgeFactory.h"
#include "TeamFactory.h"
#include "AgentKnowledgeFactory.h"
#include "KnowledgeGroupsModel.h"
#include "ObjectTypes.h"
#include "LogisticConsignFactory.h"
#include "FireResultFactory.h"
#include "FiresModel.h"

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::Model( Controller& controller, const Simulation& simulation, const std::string& scipioXml )
    : types_( * new AgentTypes( scipioXml ) )
    , objectTypes_( *new ObjectTypes( scipioXml ) )
    , objectKnowledgeFactory_( *new ObjectKnowledgeFactory( controller, *this ) )
    , agentsKnowledgeFactory_( *new AgentKnowledgeFactory( controller, *this ) )
    , teamFactory_( *new TeamFactory( controller, *this ) )
    , agentFactory_( *new AgentFactory( controller, types_, *this, simulation ) )
    , logisticFactory_( *new LogisticConsignFactory( controller, *this ) )
    , objectFactory_( *new ObjectFactory( controller, *this ) )
    , agents_( *new AgentsModel( agentFactory_ ) )
    , objects_( *new ObjectsModel( objectFactory_ ) )
    , teams_( *new TeamsModel( teamFactory_ ) )
    , knowledgeGroups_( *new KnowledgeGroupsModel( teams_ ) )
    , logistics_( *new LogisticsModel( logisticFactory_ ) )
    , limits_( *new LimitsModel() )
    , fires_( *new FiresModel( agents_, agents_ ) )
    , fireResultsFactory_( *new FireResultFactory( *this ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::~Model()
{
    delete &fires_;
    delete &fireResultsFactory_;
    delete &limits_;
    delete &logistics_;
    delete &knowledgeGroups_;
    delete &teams_;
    delete &objects_;
    delete &agents_;
    delete &logisticFactory_;
    delete &objectFactory_;
    delete &agentFactory_;
    delete &teamFactory_;
    delete &agentsKnowledgeFactory_;
    delete &objectKnowledgeFactory_;
    delete &objectTypes_;
    delete &types_;
}
    

    