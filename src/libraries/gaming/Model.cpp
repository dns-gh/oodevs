// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Model.h"
#include "AgentsModel.h"
#include "ObjectsModel.h"
#include "TeamsModel.h"
#include "LogisticsModel.h"
#include "LimitsModel.h"
#include "AgentFactory.h"
#include "ObjectFactory.h"
#include "AgentKnowledgeFactory.h"
#include "ObjectKnowledgeFactory.h"
#include "AgentKnowledgeConverter.h"
#include "ObjectKnowledgeConverter.h"
#include "TeamFactory.h"
#include "KnowledgeGroupsModel.h"
#include "LogisticConsignFactory.h"
#include "FireResultFactory.h"
#include "FiresModel.h"
#include "FireFactory.h"
#include "WeatherModel.h"
#include "StaticModel.h"
#include "TacticalLineFactory.h"
#include "UserProfilesModel.h"
#include "UserProfileFactory.h"
#include "ActionParameterFactory.h"
#include "ActionFactory.h"
#include "actions/ActionsModel.h"
#include "FolkModel.h"
#include "AfterActionModel.h"
#include "IntelligenceFactory.h"
#include "IntelligencesModel.h"
#include "DrawingFactory.h"
#include "DrawingsModel.h"
#include "ScoreModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/FormationLevels.h"
#include "urban/Model.h"
#include "urban/StaticModel.h"
#include "urban/Controllers.h"
#include "urban/Controller.h"
#include "urban/ActionController.h"

#pragma warning( disable : 4355 )

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::Model( Controllers& controllers, const StaticModel& staticModel, const Simulation& simulation, Workers& workers, Publisher_ABC& publisher, const RcEntityResolver_ABC& rcResolver )
    : controllers_( controllers )
    , static_( staticModel )
    , agentsKnowledgeFactory_( *new AgentKnowledgeFactory( controllers, *this, staticModel.coordinateConverter_ ) )
    , objectKnowledgeFactory_( *new ObjectKnowledgeFactory( controllers, *this, staticModel ) )
    , agentKnowledgeConverter_( *new AgentKnowledgeConverter( controllers ) )
    , objectKnowledgeConverter_( *new ObjectKnowledgeConverter( controllers ) )
    , teamFactory_( *new TeamFactory( controllers, *this ) )
    , agentFactory_( *new AgentFactory( controllers, *this, staticModel, publisher, workers, rcResolver ) )
    , objectFactory_( *new ObjectFactory( controllers, *this, staticModel ) )
    , logisticFactory_( *new LogisticConsignFactory( controllers, *this, staticModel ) )
    , fireFactory_( *new FireFactory( *this ) )
    , tacticalLineFactory_( *new TacticalLineFactory( controllers, staticModel.coordinateConverter_, *this, publisher ) )
    , fireResultsFactory_( *new FireResultFactory( *this, simulation ) )
    , userProfileFactory_( *new UserProfileFactory( *this, controllers, publisher ) )
    , actionParameterFactory_( *new ActionParameterFactory( staticModel.coordinateConverter_, *this, staticModel, agentKnowledgeConverter_, objectKnowledgeConverter_, controllers_.controller_ ) )
    , actionFactory_( *new ActionFactory( controllers, actionParameterFactory_, *this, staticModel.types_, staticModel.types_, simulation ) )
    , intelligenceFactory_( *new IntelligenceFactory( controllers, staticModel.coordinateConverter_, *this, staticModel.levels_, publisher ) )
    , drawingFactory_( *new DrawingFactory( controllers.controller_, staticModel.drawings_, publisher, staticModel.coordinateConverter_ ) )
    , agents_( *new AgentsModel( agentFactory_ ) )
    , objects_( *new ObjectsModel( objectFactory_ ) )
    , teams_( *new TeamsModel( teamFactory_ ) )
    , knowledgeGroups_( *new KnowledgeGroupsModel() )
    , logistics_( *new LogisticsModel( logisticFactory_ ) )
    , limits_( *new LimitsModel( tacticalLineFactory_  ) )
    , fires_( *new FiresModel( agents_, agents_ ) )
    , weather_( *new WeatherModel( controllers, *this ) )
    , profiles_( *new UserProfilesModel( userProfileFactory_ ) )
    , actions_( *new actions::ActionsModel( actionFactory_ ) )
    , folk_( *new FolkModel( controllers.controller_ ) )
    , aar_( *new AfterActionModel( controllers.controller_, publisher ) )
    , intelligences_( *new IntelligencesModel( intelligenceFactory_ ) )
    , drawings_( *new DrawingsModel( controllers, drawingFactory_ ) )
    , scores_( *new ScoreModel( controllers, publisher, staticModel.scores_ ) )
 //   , urbanControllers_( *new tools::Controllers() )
 //   , urbanStaticModel_( *new urban::StaticModel() )
 //   , urbanModel_( *new urban::Model( urbanControllers_, urbanStaticModel_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::~Model()
{
    delete &scores_;
    delete &drawings_;
    delete &intelligences_;
    delete &aar_;
    delete &folk_;
    delete &actions_;
    delete &profiles_;
    delete &weather_;
    delete &fires_;
    delete &limits_;
    delete &logistics_;
    delete &knowledgeGroups_;
    delete &teams_;
    delete &objects_;
    delete &agents_;
    delete &drawingFactory_;
    delete &intelligenceFactory_;
    delete &actionFactory_;
    delete &actionParameterFactory_;
    delete &userProfileFactory_;
    delete &fireResultsFactory_;
    delete &tacticalLineFactory_;
    delete &fireFactory_;
    delete &logisticFactory_;
    delete &objectFactory_;
    delete &agentFactory_;
    delete &teamFactory_;
    delete &objectKnowledgeConverter_;
    delete &agentKnowledgeConverter_;
    delete &objectKnowledgeFactory_;
    delete &agentsKnowledgeFactory_;
}

// -----------------------------------------------------------------------------
// Name: Model::Purge
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
void Model::Purge()
{
    scores_.Purge();
    drawings_.Purge();
    intelligences_.Purge();
    aar_.Purge();
    actions_.Purge();
    profiles_.Purge();
    weather_.Purge();
    limits_.Purge();
    logistics_.Purge();
    fires_.Purge();
    agents_.Purge();
    objects_.Purge();
    knowledgeGroups_.Purge();
    teams_.Purge();
}
