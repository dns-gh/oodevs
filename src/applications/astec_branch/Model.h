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

class AgentsModel;
class ObjectsModel;
class TeamsModel;
class KnowledgeGroupsModel;
class LogisticsModel;
class LimitsModel;
class AgentFactory_ABC;
class ObjectFactory_ABC;
class Controller;
class AgentTypes;
class ObjectKnowledgeFactory;
class AgentKnowledgeFactory_ABC;
class TeamFactory_ABC;

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
             Model( Controller& controller, const std::string& scipioXml );
    virtual ~Model();
    //@}

    //! @name Operations
    //@{
    //@}

public:
    //! @name Member data
    //@{
    Controller& controller_;

    AgentTypes& types_;

    ObjectKnowledgeFactory& objectKnowledgeFactory_; // $$$$ AGE 2006-02-15: ABC
    AgentKnowledgeFactory_ABC& agentsKnowledgeFactory_;

    TeamFactory_ABC&         teamFactory_;
    AgentFactory_ABC&        agentFactory_;
    ObjectFactory_ABC&       objectFactory_;

    AgentsModel&    agents_;
    ObjectsModel&   objects_;
    TeamsModel&     teams_;
    KnowledgeGroupsModel& knowledgeGroups_;
    LogisticsModel& logistics_;
    LimitsModel&    limits_;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignement operator
    //@}
};

#endif // __Model_h_
