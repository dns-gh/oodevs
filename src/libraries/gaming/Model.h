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

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class AgentKnowledgeConverter_ABC;
    class Controllers;
    class FormationLevels;
    class ObjectKnowledgeConverter_ABC;
    class SymbolFactory;
    class Workers;
}

namespace actions
{
    class ActionFactory_ABC;
    class ActionsModel;
    class ParameterFactory_ABC;
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
class KnowledgeGroupFactory_ABC;
class KnowledgeGroupsModel;
class LimitsModel;
class LogisticConsignFactory_ABC;
class LogisticsModel;
class ObjectFactory_ABC;
class ObjectKnowledgeFactory;
class ObjectsModel;
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
class UserProfileFactory_ABC;
class UserProfilesModel;
class WeatherModel;
class UrbanKnowledgeFactory;;
class UrbanModel;
class SurfaceFactory;
class NotesModel;
class MeteoModel;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: AGE 2006-02-15
// =============================================================================
class Model : public kernel::Model_ABC
            , public kernel::EntityResolverFacade
{
public:
    //! @name Constructors/Destructor
    //@{
             Model( kernel::Controllers& controllers, const StaticModel& staticModel, const Simulation& simulation, kernel::Workers& workers, Publisher_ABC& publisher, const tools::ExerciseConfig& config );
    virtual ~Model();
    //@}

    //! @name From Model_ABC
    //@
    virtual tools::Resolver_ABC< kernel::Team_ABC >& GetTeamResolver() const;
    virtual tools::Resolver_ABC< kernel::Formation_ABC >& GetFormationResolver() const;
    virtual tools::Resolver_ABC< kernel::Automat_ABC >& GetAutomatResolver() const;
    virtual tools::Resolver_ABC< kernel::Agent_ABC >& GetAgentResolver() const;
    virtual tools::Resolver_ABC< kernel::KnowledgeGroup_ABC >& GetKnowledgeGroupResolver() const;
    virtual tools::Resolver_ABC< kernel::Object_ABC >& GetObjectResolver() const;
    virtual tools::Resolver_ABC< kernel::Population_ABC >& GetPopulationResolver() const;
    virtual tools::Resolver_ABC< kernel::Inhabitant_ABC >& GetInhabitantResolver() const;
    // $$$$ _RC_ JSR 2011-06-28: Passer TerrainObjectProxy dans clients_kernel pour remplacer ces méthodes
    virtual kernel::Object_ABC* FindUrbanObject( unsigned int id ) const;
    virtual kernel::Object_ABC& GetUrbanObject( unsigned int id ) const;
    //@}

    //! @name Operations
    //@{
    void Purge();
    //@}

public:
    //! @name Member data
    //@{
    // $$$$ _RC_ JSR 2011-04-05: Passer en private ce qui peut l'être, voire utiliser des accesseurs.
    kernel::Controllers& controllers_;
    const StaticModel& static_;
    AgentKnowledgeFactory& agentsKnowledgeFactory_;
    ObjectKnowledgeFactory& objectKnowledgeFactory_; // $$$$ AGE 2006-02-15: ABC
    UrbanKnowledgeFactory& urbanKnowledgeFactory_;
    kernel::AgentKnowledgeConverter_ABC& agentKnowledgeConverter_;
    kernel::ObjectKnowledgeConverter_ABC& objectKnowledgeConverter_;
    TeamFactory_ABC& teamFactory_;
    KnowledgeGroupFactory_ABC& knowledgeGroupFactory_;
    AgentFactory_ABC& agentFactory_;
    ObjectFactory_ABC& objectFactory_;
    LogisticConsignFactory_ABC& logisticFactory_;
    FireFactory& fireFactory_;
    TacticalLineFactory& tacticalLineFactory_;
    FireResultFactory& fireResultsFactory_;
    UserProfileFactory_ABC& userProfileFactory_;
    actions::ParameterFactory_ABC& actionParameterFactory_;
    actions::ActionFactory_ABC& actionFactory_;
    DrawingFactory& drawingFactory_;
    AgentsModel& agents_;
    ObjectsModel& objects_;
    TeamsModel& teams_;
    KnowledgeGroupsModel& knowledgeGroups_;
    LogisticsModel& logistics_;
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
    SurfaceFactory& surfaceFactory_;
    kernel::SymbolFactory& symbolsFactory_;
    NotesModel& notes_;
    MeteoModel& meteo_;
    FloodProxy& floodProxy_;
    Publisher_ABC& publisher_;
    //@}
};

#endif // __Model_h_
