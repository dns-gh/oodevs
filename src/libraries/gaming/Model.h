// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Model_h_
#define __Model_h_

#include "clients_kernel/EntityResolverFacade.h"
#include "clients_kernel/Model_ABC.h"
#include "SurfaceFactory_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class Controllers;
    class ObjectKnowledgeConverter_ABC;
    class Profile_ABC;
    class ProfileFactory_ABC;
    class SymbolFactory;
    class Workers;
}

namespace actions
{
    class ActionFactory_ABC;
    class ActionsModel;
    class ParameterFactory_ABC;
}

namespace gui
{
    class EventFactory;
    class EventsModel;
    class TimelinePublisher;
}

class AfterActionModel;
class AgentFactory_ABC;
class AgentKnowledgeFactory;
class AgentsModel;
class DrawingFactory;
class DrawingsModel;
class FireFactory;
class FireResultFactory;
class FiresModel;
class FloodProxy;
class FolkModel;
class HistoryLogisticsModel;
class KnowledgeGroupFactory_ABC;
class KnowledgeGroupsModel;
class LimitsModel;
class LogisticConsignFactory_ABC;
class LogisticsModel;
class MeteoModel;
class NotesModel;
class ObjectFactory_ABC;
class ObjectKnowledgeFactory;
class ObjectsModel;
class PathfindModel;
class Publisher_ABC;
class ResourceNetworkModel;
class ScoreDefinitions;
class ScoreModel;
class Simulation;
class StaticModel;
class TacticalLineFactory;
class TeamFactory_ABC;
class TeamsModel;
class UrbanBlockDetectionMap;
class UrbanKnowledgeFactory;
class ProfileFactory_ABC;
class UserProfilesModel;
class UrbanModel;
class VisionMeteoModel;
class WeatherModel;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: AGE 2006-02-15
// =============================================================================
class Model : public kernel::Model_ABC
            , public kernel::EntityResolverFacade
            , public tools::Observer_ABC
            , public tools::ElementObserver_ABC< MeteoModel >
            , public SurfaceFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             Model( kernel::Controllers& controllers, const StaticModel& staticModel, const Simulation& simulation,
                    kernel::Workers& workers, Publisher_ABC& publisher, const tools::ExerciseConfig& config,
                    kernel::Profile_ABC& profile, const kernel::Profile_ABC& filter );
    virtual ~Model();
    //@}

    //! @name From Model_ABC
    //@
    virtual tools::Resolver_ABC< kernel::Team_ABC           >& GetTeamResolver() const;
    virtual tools::Resolver_ABC< kernel::Formation_ABC      >& GetFormationResolver() const;
    virtual tools::Resolver_ABC< kernel::Automat_ABC        >& GetAutomatResolver() const;
    virtual tools::Resolver_ABC< kernel::Agent_ABC          >& GetAgentResolver() const;
    virtual tools::Resolver_ABC< kernel::Ghost_ABC          >& GetGhostResolver() const;
    virtual tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& GetKnowledgeGroupResolver() const;
    virtual tools::Resolver_ABC< kernel::Object_ABC         >& GetObjectResolver() const;
    virtual tools::Resolver_ABC< kernel::Population_ABC     >& GetPopulationResolver() const;
    virtual tools::Resolver_ABC< kernel::Inhabitant_ABC     >& GetInhabitantResolver() const;
    virtual tools::Resolver_ABC< kernel::UrbanObject_ABC    >& GetUrbanObjectResolver() const;
    //@}

    //! @name Operations
    //@{
    virtual Surface* CreateSurface( const kernel::Agent_ABC& agent, const sword::VisionCone& input );
    virtual VisionMap* CreateVisionMap();
    virtual void NotifyUpdated( const MeteoModel& model );
    void Purge();
    //@}

public:
    //! @name Member data
    //@{
    // $$$$ _RC_ JSR 2011-04-05: Passer en private ce qui peut l'être, voire utiliser des accesseurs.
    kernel::Controllers& controllers_;
    const StaticModel& static_;
    const tools::ExerciseConfig& config_;
    AgentKnowledgeFactory& agentsKnowledgeFactory_;
    ObjectKnowledgeFactory& objectKnowledgeFactory_;
    UrbanKnowledgeFactory& urbanKnowledgeFactory_;
    kernel::ObjectKnowledgeConverter_ABC& objectKnowledgeConverter_;
    TeamFactory_ABC& teamFactory_;
    KnowledgeGroupFactory_ABC& knowledgeGroupFactory_;
    AgentFactory_ABC& agentFactory_;
    ObjectFactory_ABC& objectFactory_;
    LogisticConsignFactory_ABC& logisticFactory_;
    FireFactory& fireFactory_;
    TacticalLineFactory& tacticalLineFactory_;
    FireResultFactory& fireResultsFactory_;
    kernel::ProfileFactory_ABC& userProfileFactory_;
    actions::ParameterFactory_ABC& actionParameterFactory_;
    actions::ActionFactory_ABC& actionFactory_;
    DrawingFactory& drawingFactory_;
    AgentsModel& agents_;
    ObjectsModel& objects_;
    TeamsModel& teams_;
    KnowledgeGroupsModel& knowledgeGroups_;
    LogisticsModel& logistics_;
    HistoryLogisticsModel& historyLogistics_;
    LimitsModel& limits_;
    FiresModel& fires_;
    WeatherModel& weather_;
    UserProfilesModel& profiles_;
    actions::ActionsModel& actions_;
    FolkModel& folk_;
    AfterActionModel& aar_;
    DrawingsModel& drawings_;
    ScoreDefinitions& scoreDefinitions_;
    ScoreModel& scores_;
    UrbanBlockDetectionMap& urbanBlockDetectionMap_;
    ResourceNetworkModel& resourceNetwork_;
    UrbanModel& urbanObjects_;
    kernel::SymbolFactory& symbolsFactory_;
    NotesModel& notes_;
    MeteoModel& meteo_;
    boost::shared_ptr< VisionMeteoModel > visionMeteoModel_;
    FloodProxy& floodProxy_;
    Publisher_ABC& publisher_;
    const gui::EventFactory& eventFactory_;
    gui::EventsModel& events_;
    gui::TimelinePublisher& timelinePublisher_;
    PathfindModel& pathfinds_;
    //@}
};

#endif // __Model_h_
