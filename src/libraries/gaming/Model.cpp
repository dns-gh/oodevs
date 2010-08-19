// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "Model.h"
#include "ActionPublisher.h"
#include "AfterActionModel.h"
#include "AgentFactory.h"
#include "AgentKnowledgeConverter.h"
#include "AgentKnowledgeFactory.h"
#include "AgentsModel.h"
#include "DrawingFactory.h"
#include "DrawingsModel.h"
#include "FireFactory.h"
#include "FireResultFactory.h"
#include "FiresModel.h"
#include "FolkModel.h"
#include "IntelligenceFactory.h"
#include "IntelligencesModel.h"
#include "KnowledgeGroupFactory.h"
#include "KnowledgeGroupsModel.h"
#include "LimitsModel.h"
#include "LogisticConsignFactory.h"
#include "LogisticsModel.h"
#include "MeteoModel.h"
#include "NotesModel.h"
#include "ObjectFactory.h"
#include "ObjectKnowledgeConverter.h"
#include "ObjectKnowledgeFactory.h"
#include "ObjectsModel.h"
#include "ResourceNetworkFactory.h"
#include "ResourceNetworkModel.h"
#include "ScoreDefinitions.h"
#include "ScoreModel.h"
#include "Simulation.h"
#include "StaticModel.h"
#include "SurfaceFactory.h"
#include "TacticalLineFactory.h"
#include "TeamFactory.h"
#include "TeamsModel.h"
#include "UrbanKnowledgeFactory.h"
#include "UrbanModel.h"
#include "UserProfileFactory.h"
#include "UserProfilesModel.h"
#include "WeatherModel.h"
#include "actions/ActionFactory.h"
#include "actions/ActionParameterFactory.h"
#include "actions/ActionsModel.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/FormationLevels.h"
#include "indicators/GaugeTypes.h"

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::Model( kernel::Controllers& controllers, const StaticModel& staticModel, const Simulation& simulation, kernel::Workers& workers, Publisher_ABC& publisher, const RcEntityResolver_ABC& rcResolver )
    : EntityResolverFacade( *this )
    , controllers_( controllers )
    , static_( staticModel )
    , agentsKnowledgeFactory_( *new AgentKnowledgeFactory( controllers, *this, staticModel.coordinateConverter_ ) )
    , objectKnowledgeFactory_( *new ObjectKnowledgeFactory( controllers, *this, staticModel ) )
    , urbanKnowledgeFactory_( *new UrbanKnowledgeFactory( controllers, *this ) )
    , agentKnowledgeConverter_( *new AgentKnowledgeConverter( controllers ) )
    , objectKnowledgeConverter_( *new ObjectKnowledgeConverter( controllers ) )
    , teamFactory_( *new TeamFactory( controllers, *this ) )
    , knowledgeGroupFactory_( *new KnowledgeGroupFactory( controllers, *this ) )
    , agentFactory_( *new AgentFactory( controllers, *this, staticModel, publisher, workers, rcResolver ) )
    , objectFactory_( *new ObjectFactory( controllers, *this, staticModel ) )
    , logisticFactory_( *new LogisticConsignFactory( controllers, *this, staticModel ) )
    , fireFactory_( *new FireFactory( *this ) )
    , tacticalLineFactory_( *new TacticalLineFactory( controllers, staticModel.coordinateConverter_, *this, publisher ) )
    , fireResultsFactory_( *new FireResultFactory( *this, simulation ) )
    , userProfileFactory_( *new UserProfileFactory( *this, controllers, publisher ) )
    , actionParameterFactory_( *new actions::ActionParameterFactory( staticModel.coordinateConverter_, *this, staticModel, agentKnowledgeConverter_, objectKnowledgeConverter_, controllers_.controller_ ) )
    , actionFactory_( *new actions::ActionFactory( controllers.controller_, actionParameterFactory_, *this, staticModel, simulation ) )
    , intelligenceFactory_( *new IntelligenceFactory( controllers, staticModel.coordinateConverter_, *this, staticModel.levels_, publisher ) )
    , drawingFactory_( *new DrawingFactory( controllers.controller_, staticModel.drawings_, publisher, staticModel.coordinateConverter_ ) )
    , resourceNetworkFactory_( *new ResourceNetworkFactory( controllers.controller_, *this ) )
    , agents_( *new AgentsModel( agentFactory_ ) )
    , objects_( *new ObjectsModel( objectFactory_ ) )
    , teams_( *new TeamsModel( teamFactory_ ) )
    , knowledgeGroups_( *new KnowledgeGroupsModel( knowledgeGroupFactory_ ) )
    , logistics_( *new LogisticsModel( logisticFactory_ ) )
    , limits_( *new LimitsModel( tacticalLineFactory_  ) )
    , fires_( *new FiresModel( agents_, agents_ ) )
    , weather_( *new WeatherModel( controllers, *this ) )
    , profiles_( *new UserProfilesModel( userProfileFactory_ ) )
    , actions_( *new actions::ActionsModel( actionFactory_, *new ActionPublisher( publisher, controllers_ ) ) )
    , folk_( *new FolkModel( controllers.controller_ ) )
    , aar_( *new AfterActionModel( controllers.controller_, publisher ) )
    , intelligences_( *new IntelligencesModel( intelligenceFactory_ ) )
    , drawings_( *new DrawingsModel( controllers, drawingFactory_ ) )
    , scoreDefinitions_( *new ScoreDefinitions( staticModel.indicators_, staticModel.gaugeTypes_ ) )
    , scores_( *new ScoreModel( controllers, publisher, scoreDefinitions_ ) )
    , urbanObjects_( *new UrbanModel( controllers.controller_, *this, static_.detection_ ) )
    , resourceNetwork_( *new ResourceNetworkModel( resourceNetworkFactory_ ) )
    , surfaceFactory_( *new SurfaceFactory( static_.coordinateConverter_, static_.detection_, static_.types_, urbanObjects_.GetUrbanBlockMap() ) )
    , notes_( *new NotesModel( controllers.controller_ ))
    , meteo_( *new MeteoModel( static_.coordinateConverter_ ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::~Model()
{
    delete &meteo_;
    delete &notes_;
    delete &surfaceFactory_;
    delete &resourceNetwork_;
    delete &urbanObjects_;
    delete &scores_;
    delete &scoreDefinitions_;
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
    delete &resourceNetworkFactory_;
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
    urbanObjects_.Purge();
    scores_.Purge();
    scoreDefinitions_.Purge();
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
    meteo_.Purge();
    resourceNetwork_.Purge();
}
