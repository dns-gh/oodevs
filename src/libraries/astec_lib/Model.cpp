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
#include "ObjectKnowledgeFactory.h"
#include "TeamFactory.h"
#include "AgentKnowledgeFactory.h"
#include "KnowledgeGroupsModel.h"
#include "LogisticConsignFactory.h"
#include "FireResultFactory.h"
#include "FiresModel.h"
#include "FireFactory.h"
#include "WeatherModel.h"
#include "ModelLoaded.h"
#include "Controllers.h"
#include "Controller.h"
#include "StaticModel.h"
#include "CoordinateConverter.h"

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::Model( Controllers& controllers, const StaticModel& staticModel, const Simulation& simulation, Workers& workers, Publisher_ABC& publisher )
    : controllers_( controllers )
    , static_( staticModel )
    , objectKnowledgeFactory_( *new ObjectKnowledgeFactory( controllers, *this, staticModel ) )
    , agentsKnowledgeFactory_( *new AgentKnowledgeFactory( controllers, *this, staticModel.coordinateConverter_ ) )
    , teamFactory_( *new TeamFactory( controllers, *this ) )
    , agentFactory_( *new AgentFactory( controllers, *this, staticModel, publisher, simulation, workers ) )
    , logisticFactory_( *new LogisticConsignFactory( controllers, *this, staticModel ) )
    , fireFactory_( *new FireFactory( *this ) )
    , objectFactory_( *new ObjectFactory( controllers, *this, staticModel ) )
    , agents_( *new AgentsModel( agentFactory_ ) )
    , objects_( *new ObjectsModel( objectFactory_ ) )
    , teams_( *new TeamsModel( teamFactory_ ) )
    , knowledgeGroups_( *new KnowledgeGroupsModel( teams_ ) )
    , logistics_( *new LogisticsModel( logisticFactory_ ) )
    , limits_( *new LimitsModel( controllers, staticModel.coordinateConverter_, publisher ) )
    , fires_( *new FiresModel( agents_, agents_ ) )
    , weather_( *new WeatherModel( controllers, *this ) )
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
    delete &fireResultsFactory_;
    delete &weather_;
    delete &fires_;
    delete &limits_;
    delete &logistics_;
    delete &knowledgeGroups_;
    delete &teams_;
    delete &objects_;
    delete &agents_;
    delete &fireFactory_;
    delete &logisticFactory_;
    delete &objectFactory_;
    delete &agentFactory_;
    delete &teamFactory_;
    delete &agentsKnowledgeFactory_;
    delete &objectKnowledgeFactory_;
}

// -----------------------------------------------------------------------------
// Name: Model::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void Model::Purge()
{
    agents_.Purge();
    objects_.Purge();
    teams_.Purge();
    knowledgeGroups_.Purge();
    logistics_.Purge();
//    limits_.Purge();
    fires_.Purge();
    weather_.Purge();
}
    

    