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

namespace kernel
{
    class Controllers;
    class Workers;
    class AgentKnowledgeConverter_ABC;
    class ObjectKnowledgeConverter_ABC;
}

namespace tools
{
    class Controllers;
}

namespace actions
{
    class ActionsModel;
}

class AgentsModel;
class ObjectsModel;
class TeamsModel;
class KnowledgeGroupsModel;
class LogisticsModel;
class LimitsModel;
class AgentFactory_ABC;
class ObjectFactory_ABC;
class AgentKnowledgeFactory;
class ObjectKnowledgeFactory;
class TeamFactory_ABC;
class LogisticConsignFactory_ABC;
class Simulation;
class FireResultFactory;
class FiresModel;
class FireFactory;
class WeatherModel;
class Publisher_ABC;
class StaticModel;
class RcEntityResolver_ABC;
class TacticalLineFactory;
class UserProfilesModel;
class UserProfileFactory_ABC;
class FolkModel;
class AfterActionModel;
class IntelligenceFactory;
class IntelligencesModel;
class DrawingFactory;
class DrawingsModel;
class ScoreModel;
class ActionFactory_ABC;
class ParameterFactory_ABC;
class UrbanModel;

// =============================================================================
/** @class  Model
    @brief  Model
*/
// Created: AGE 2006-02-15
// =============================================================================
class Model
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
    kernel::AgentKnowledgeConverter_ABC& agentKnowledgeConverter_;
    kernel::ObjectKnowledgeConverter_ABC& objectKnowledgeConverter_;

    TeamFactory_ABC&            teamFactory_;
    AgentFactory_ABC&           agentFactory_;
    ObjectFactory_ABC&          objectFactory_;
    LogisticConsignFactory_ABC& logisticFactory_;
    FireFactory&                fireFactory_;
    TacticalLineFactory&        tacticalLineFactory_;
    FireResultFactory&          fireResultsFactory_;
    UserProfileFactory_ABC&     userProfileFactory_;
    ::ParameterFactory_ABC&     actionParameterFactory_;
    ActionFactory_ABC&          actionFactory_;
    IntelligenceFactory&        intelligenceFactory_;
    DrawingFactory&             drawingFactory_;

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
    ScoreModel&           scores_;
    UrbanModel&           urbanObjects_;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignement operator
    //@}
};

#endif // __Model_h_
