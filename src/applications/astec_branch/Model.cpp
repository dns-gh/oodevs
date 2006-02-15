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

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::Model( Controller& controller, const std::string& scipioXml )
    : controller_( controller )
    , types_( * new AgentTypes( scipioXml ) )
    , objectKnowledgeFactory_( *new ObjectKnowledgeFactory( controller_, *this ) )
    , teamFactory_( *new TeamFactory( controller_, *this ) )
    , agentFactory_( *new AgentFactory( controller_,types_, *this ) )
    , objectFactory_( *new ObjectFactory( controller_ ) )
    , agents_( *new AgentsModel( agentFactory_ ) )
    , objects_( *new ObjectsModel( objectFactory_ ) )
    , teams_( *new TeamsModel( teamFactory_ ) )
    , logistics_( *new LogisticsModel() )
    , limits_( *new LimitsModel() )
{

}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::~Model()
{
    delete &types_;
    delete &agentFactory_;
    delete &objectFactory_;
    delete &agents_;
    delete &objects_;
    delete &teams_;
    delete &logistics_;
    delete &limits_;
}
    

    