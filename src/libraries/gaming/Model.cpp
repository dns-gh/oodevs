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
#include "AfterActionModel.h"
#include "AgentFactory.h"
#include "AgentKnowledgeFactory.h"
#include "AgentsModel.h"
#include "DrawingFactory.h"
#include "DrawingsModel.h"
#include "FireFactory.h"
#include "FireResultFactory.h"
#include "FiresModel.h"
#include "FloodProxy.h"
#include "FolkModel.h"
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
#include "PathfindModel.h"
#include "ResourceNetworkModel.h"
#include "ScoreDefinitions.h"
#include "ScoreModel.h"
#include "Simulation.h"
#include "Surface.h"
#include "StaticModel.h"
#include "HistoryLogisticsModel.h"
#include "TacticalLineFactory.h"
#include "TeamFactory.h"
#include "TeamsModel.h"
#include "UrbanBlockDetectionMap.h"
#include "UrbanKnowledgeFactory.h"
#include "UrbanModel.h"
#include "UserProfileFactory.h"
#include "UserProfilesModel.h"
#include "WeatherModel.h"
#include "VisionMap.h"
#include "VisionMeteoModel.h"
#include "actions/ActionFactory.h"
#include "actions/ActionParameterFactory.h"
#include "actions/ActionsModel.h"
#include "clients_gui/EventFactory.h"
#include "clients_gui/EventsModel.h"
#include "clients_gui/TimelinePublisher.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/SymbolFactory.h"
#include "indicators/GaugeTypes.h"
#include <boost/make_shared.hpp>

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: Model constructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::Model( kernel::Controllers& controllers, const StaticModel& staticModel, const Simulation& simulation,
        kernel::Workers& workers, Publisher_ABC& publisher, const tools::ExerciseConfig& config,
        kernel::Profile_ABC& profile, const kernel::Profile_ABC& filter )
    : EntityResolverFacade( static_cast< kernel::Model_ABC& >( *this ) )
    , controllers_             ( controllers )
    , static_                  ( staticModel )
    , config_                  ( config )
    , objectKnowledgeConverter_( *new ObjectKnowledgeConverter( controllers ) )
    , actionParameterFactory_  ( *new actions::ActionParameterFactory( staticModel.coordinateConverter_, *this, staticModel, objectKnowledgeConverter_, controllers_.controller_ ) )
    , actionFactory_           ( *new actions::ActionFactory( controllers.controller_, actionParameterFactory_, *this, staticModel, simulation ) )
    , actions_                 ( *new actions::ActionsModel( actionFactory_, publisher, controllers, simulation ) )
    , agentsKnowledgeFactory_  ( *new AgentKnowledgeFactory( controllers, *this, staticModel.coordinateConverter_, profile ) )
    , objectKnowledgeFactory_  ( *new ObjectKnowledgeFactory( controllers, *this, staticModel, simulation ) )
    , urbanKnowledgeFactory_   ( *new UrbanKnowledgeFactory( controllers.controller_, *this ) )
    , knowledgeGroupFactory_   ( *new KnowledgeGroupFactory( controllers, *this, profile, actions_ ) )
    , teamFactory_             ( *new TeamFactory( controllers, *this, staticModel, profile, actions_ ) )
    , agentFactory_            ( *new AgentFactory( controllers, *this, staticModel, publisher, workers, profile, actions_ ) )
    , objectFactory_           ( *new ObjectFactory( controllers, *this, staticModel, simulation, profile, actions_ ) )
    , logisticFactory_         ( *new LogisticConsignFactory( controllers, *this, staticModel, simulation ) )
    , fireFactory_             ( *new FireFactory( *this, controllers_.controller_, filter ) )
    , tacticalLineFactory_     ( *new TacticalLineFactory( controllers, staticModel.coordinateConverter_, *this, publisher, profile ) )
    , fireResultsFactory_      ( *new FireResultFactory( *this, simulation, profile ) )
    , userProfileFactory_      ( *new UserProfileFactory( controllers_.controller_, publisher ) )
    , drawingFactory_          ( *new DrawingFactory( controllers, staticModel.drawings_, publisher, staticModel.coordinateConverter_, *this ) )
    , agents_                  ( *new AgentsModel( agentFactory_ ) )
    , objects_                 ( *new ObjectsModel( objectFactory_ ) )
    , teams_                   ( *new TeamsModel( teamFactory_ ) )
    , knowledgeGroups_         ( *new KnowledgeGroupsModel( knowledgeGroupFactory_ ) )
    , logistics_               ( *new LogisticsModel( logisticFactory_, *this, staticModel.objectTypes_, controllers.controller_ ) )
    , historyLogistics_        ( *new HistoryLogisticsModel( logisticFactory_, *this, staticModel.objectTypes_, controllers.controller_ ) )
    , limits_                  ( *new LimitsModel( tacticalLineFactory_ ) )
    , fires_                   ( *new FiresModel( agents_, agents_, agents_, profile ) )
    , weather_                 ( *new WeatherModel( controllers_.controller_, *this, profile ) )
    , profiles_                ( *new UserProfilesModel( userProfileFactory_ ) )
    , folk_                    ( *new FolkModel( controllers.controller_ ) )
    , aar_                     ( *new AfterActionModel( controllers.controller_, publisher ) )
    , drawings_                ( *new DrawingsModel( controllers, drawingFactory_, *this ) )
    , scoreDefinitions_        ( *new ScoreDefinitions( staticModel.indicators_, staticModel.gaugeTypes_ ) )
    , scores_                  ( *new ScoreModel( controllers, publisher, scoreDefinitions_ ) )
    , urbanBlockDetectionMap_  ( *new UrbanBlockDetectionMap( controllers, static_.detection_ ) )
    , resourceNetwork_         ( *new ResourceNetworkModel( controllers, *this, static_ ) )
    , urbanObjects_            ( *new UrbanModel( controllers, resourceNetwork_, static_ ) )
    , symbolsFactory_          ( *new kernel::SymbolFactory() )
    , notes_                   ( *new NotesModel( controllers.controller_ ))
    , meteo_                   ( *new MeteoModel( static_.coordinateConverter_, simulation, controllers.controller_ ) )
    , visionMeteoModel_        ( boost::make_shared< VisionMeteoModel >( meteo_ ) )
    , floodProxy_              ( *new FloodProxy( static_.detection_ ) )
    , publisher_               ( publisher )
    , eventFactory_            ( *new gui::EventFactory( actions_, controllers ) )
    , events_                  ( *new gui::EventsModel( eventFactory_, controllers.controller_ ) )
    , timelinePublisher_       ( *new gui::TimelinePublisher() )
    , pathfinds_               ( *new PathfindModel( controllers.controller_, actions_, agents_, agents_, staticModel.coordinateConverter_, profile ) )
{
    symbolsFactory_.Load( config );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: Model::GetTeamResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< kernel::Team_ABC >& Model::GetTeamResolver() const
{
    return teams_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetFormationResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< kernel::Formation_ABC >& Model::GetFormationResolver() const
{
    return teams_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetAutomatResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< kernel::Automat_ABC >& Model::GetAutomatResolver() const
{
    return agents_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetAgentResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< kernel::Agent_ABC >& Model::GetAgentResolver() const
{
    return agents_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetGhostResolver
// Created: JSR 2014-06-04
// -----------------------------------------------------------------------------
tools::Resolver_ABC< kernel::Ghost_ABC >& Model::GetGhostResolver() const
{
    throw MASA_EXCEPTION_NOT_IMPLEMENTED;
}

// -----------------------------------------------------------------------------
// Name: Model::GetKnowledgeGroupResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& Model::GetKnowledgeGroupResolver() const
{
    return knowledgeGroups_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetObjectResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< kernel::Object_ABC >& Model::GetObjectResolver() const
{
    return objects_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetPopulationResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< kernel::Population_ABC >& Model::GetPopulationResolver() const
{
    return agents_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetInhabitantResolver
// Created: JSR 2011-06-28
// -----------------------------------------------------------------------------
tools::Resolver_ABC< kernel::Inhabitant_ABC >& Model::GetInhabitantResolver() const
{
    return agents_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetUrbanObjectResolver
// Created: JSR 2012-05-16
// -----------------------------------------------------------------------------
tools::Resolver_ABC< kernel::UrbanObject_ABC >& Model::GetUrbanObjectResolver() const
{
    return urbanObjects_;
}

// -----------------------------------------------------------------------------
// Name: Model destructor
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
Model::~Model()
{
    controllers_.Unregister( *this );
    delete &events_;
    delete &eventFactory_;
    delete &floodProxy_;
    delete &meteo_;
    delete &notes_;
    delete &symbolsFactory_;
    delete &resourceNetwork_;
    delete &urbanBlockDetectionMap_;
    delete &urbanObjects_;
    delete &scores_;
    delete &scoreDefinitions_;
    delete &drawings_;
    delete &aar_;
    delete &folk_;
    delete &actions_;
    delete &profiles_;
    delete &weather_;
    delete &fires_;
    delete &limits_;
    delete &logistics_;
    delete &historyLogistics_;
    delete &knowledgeGroups_;
    delete &teams_;
    delete &objects_;
    delete &agents_;
    delete &drawingFactory_;
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
    delete &objectKnowledgeFactory_;
    delete &agentsKnowledgeFactory_;
    delete &pathfinds_;
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
    aar_.Purge();
    actions_.Purge();
    profiles_.Purge();
    weather_.Purge();
    limits_.Purge();
    logistics_.Purge();
    historyLogistics_.Purge();
    fires_.Purge();
    knowledgeGroups_.Purge();
    agents_.Purge();
    objects_.Purge();
    teams_.Purge();
    meteo_.Purge();
    events_.Purge();
    pathfinds_.Purge();
}

// -----------------------------------------------------------------------------
// Name: Model::NotifyUpdated
// Created: LGY 2014-04-10
// -----------------------------------------------------------------------------
void Model::NotifyUpdated( const MeteoModel& model )
{
    // Vision meteo model is used by the vision cones. It's a copy of the meteo model
    // to avoid threads problem when the model changes/destroys.
    visionMeteoModel_ = boost::make_shared< VisionMeteoModel >( model );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateSurface
// Created: LGY 2014-04-10
// -----------------------------------------------------------------------------
Surface* Model::CreateSurface( const kernel::Agent_ABC& agent, const sword::VisionCone& input )
{
    return new Surface( agent, input, static_.coordinateConverter_, static_.detection_,
        static_.types_, urbanBlockDetectionMap_, visionMeteoModel_ );
}

// -----------------------------------------------------------------------------
// Name: Model::CreateVisionMap
// Created: LGY 2014-04-10
// -----------------------------------------------------------------------------
VisionMap* Model::CreateVisionMap()
{
    return new VisionMap( static_.detection_ );
}
