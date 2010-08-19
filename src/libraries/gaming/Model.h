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

#include "EntityResolverFacade.h"

namespace kernel
{
    class Controllers;
    class Workers;
    class AgentKnowledgeConverter_ABC;
    class ObjectKnowledgeConverter_ABC;
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
class FolkModel;
class IntelligenceFactory;
class IntelligencesModel;
class KnowledgeGroupFactory_ABC;
class KnowledgeGroupsModel;
class LimitsModel;
class LogisticConsignFactory_ABC;
class LogisticsModel;
class ObjectFactory_ABC;
class ObjectKnowledgeFactory;
class ObjectsModel;
class Publisher_ABC;
class RcEntityResolver_ABC;
class ResourceNetworkFactory;
class ResourceNetworkModel;
class ScoreDefinitions;
class ScoreModel;
class Simulation;
class StaticModel;
class TacticalLineFactory;
class TeamFactory_ABC;
class TeamsModel;
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
class Model : public EntityResolverFacade
{
public:
    //! @name Constructors/Destructor
    //@{
             Model( kernel::Controllers& controllers, const StaticModel& staticModel, const Simulation& simulation, kernel::Workers& workers, Publisher_ABC& publisher, const RcEntityResolver_ABC& rcResolver );
    virtual ~Model();
    //@}

    //! @name Operations
    //@{
    void Purge();
    //@}

public:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const StaticModel& static_;

    AgentKnowledgeFactory& agentsKnowledgeFactory_;
    ObjectKnowledgeFactory& objectKnowledgeFactory_; // $$$$ AGE 2006-02-15: ABC
    UrbanKnowledgeFactory& urbanKnowledgeFactory_;
    kernel::AgentKnowledgeConverter_ABC& agentKnowledgeConverter_;
    kernel::ObjectKnowledgeConverter_ABC& objectKnowledgeConverter_;

    TeamFactory_ABC&                teamFactory_;
    KnowledgeGroupFactory_ABC&      knowledgeGroupFactory_;
    AgentFactory_ABC&               agentFactory_;
    ObjectFactory_ABC&              objectFactory_;
    LogisticConsignFactory_ABC&     logisticFactory_;
    FireFactory&                    fireFactory_;
    TacticalLineFactory&            tacticalLineFactory_;
    FireResultFactory&              fireResultsFactory_;
    UserProfileFactory_ABC&         userProfileFactory_;
    actions::ParameterFactory_ABC&  actionParameterFactory_;
    actions::ActionFactory_ABC&     actionFactory_;
    IntelligenceFactory&            intelligenceFactory_;
    DrawingFactory&                 drawingFactory_;
    ResourceNetworkFactory&         resourceNetworkFactory_;

    AgentsModel&          agents_;
    ObjectsModel&         objects_;
    TeamsModel&           teams_;
    KnowledgeGroupsModel& knowledgeGroups_;
    LogisticsModel&       logistics_;
    LimitsModel&          limits_;
    FiresModel&           fires_;
    WeatherModel&         weather_;
    UserProfilesModel&    profiles_;
    actions::ActionsModel& actions_;
    FolkModel&            folk_;
    AfterActionModel&     aar_;
    IntelligencesModel&   intelligences_;
    DrawingsModel&        drawings_;
    ScoreDefinitions&     scoreDefinitions_;
    ScoreModel&           scores_;
    UrbanModel&           urbanObjects_;
    ResourceNetworkModel& resourceNetwork_;
    SurfaceFactory&       surfaceFactory_;
    NotesModel&           notes_;
    MeteoModel&           meteo_;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignment operator
    //@}
};

#endif // __Model_h_
