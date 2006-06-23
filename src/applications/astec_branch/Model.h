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
class Controllers;
class AgentTypes;
class ObjectKnowledgeFactory;
class AgentKnowledgeFactory;
class TeamFactory_ABC;
class ObjectTypes;
class LogisticConsignFactory_ABC;
class Simulation;
class FireResultFactory;
class FiresModel;
class CoordinateConverter;
class FireFactory;
class WeatherModel;
class DetectionMap;
class SurfaceFactory;
class Workers;

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
             Model( Controllers& controllers, const Simulation& simulation, Workers& workers );
    virtual ~Model();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& scipioXml );
    void Purge();
    void PurgeDynamic();
    //@}

public:
    //! @name Member data
    //@{
    Controllers& controllers_;
    CoordinateConverter& coordinateConverter_;
    DetectionMap& detection_;

    AgentTypes& types_;
    ObjectTypes& objectTypes_;

    ObjectKnowledgeFactory& objectKnowledgeFactory_; // $$$$ AGE 2006-02-15: ABC
    AgentKnowledgeFactory& agentsKnowledgeFactory_;

    TeamFactory_ABC&            teamFactory_;
    AgentFactory_ABC&           agentFactory_;
    ObjectFactory_ABC&          objectFactory_;
    LogisticConsignFactory_ABC& logisticFactory_;
    FireFactory&                fireFactory_;
    SurfaceFactory&             surfaceFactory_;

    AgentsModel&    agents_;
    ObjectsModel&   objects_;
    TeamsModel&     teams_;
    KnowledgeGroupsModel& knowledgeGroups_;
    LogisticsModel& logistics_;
    LimitsModel&    limits_;
    FiresModel&     fires_;
    WeatherModel&   weather_;

    FireResultFactory& fireResultsFactory_;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Model( const Model& );            //!< Copy constructor
    Model& operator=( const Model& ); //!< Assignement operator
    //@}
};

#endif // __Model_h_
