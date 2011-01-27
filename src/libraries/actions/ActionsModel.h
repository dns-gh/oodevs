// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionsModel_h_
#define __ActionsModel_h_

#include "tools/Resolver.h"

class Publisher_ABC;

namespace kernel
{
    class AgentType;
    class AgentTypes;
    class AutomatType;
    class Automat_ABC;
    class Controller;
    class CoordinateConverter_ABC;
    class Entity_ABC;
    class FragOrderType;
    class MissionType;
    class PopulationType;
    class StaticModel;
    class Resolver_ABC;
    class Time_ABC;
}

namespace xml
{
    class xostream;
    class xistream;
}

class AgentServerMsgMgr;

namespace actions
{

class Action_ABC;
class ActionFactory_ABC;
class ActionsFilter_ABC;
class CreationListener_ABC;

// =============================================================================
/** @class  ActionsModel
    @brief  ActionsModel
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionsModel : public tools::Resolver< Action_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionsModel( ActionFactory_ABC& factory, Publisher_ABC& publisher, Publisher_ABC& defaultPublisher );
    virtual ~ActionsModel();
    //@}

    //! @name Operations
    //@{
    Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::MissionType& mission );
    Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrderType& fragOrder );

    Action_ABC* CreateAutomatCreationAction( const geometry::Point2f& point, const kernel::AutomatType& type, const kernel::Entity_ABC& selected, kernel::Controller& controller, const kernel::StaticModel& staticModel, tools::Resolver_ABC< kernel::Automat_ABC >& agentsModel, CreationListener_ABC& agentMessenger, const kernel::Time_ABC& simulation );
    Action_ABC* CreateAgentCreationAction( const kernel::AgentType& type, const geometry::Point2f& point, const kernel::Entity_ABC& selected_, kernel::Controller& controller, kernel::AgentTypes& agentTypes, kernel::CoordinateConverter_ABC& coordinateConverter );
    Action_ABC* CreateFormationCreationAction( int level, const kernel::Entity_ABC& selected, kernel::Controller& controller, kernel::AgentTypes& agentTypes );
    Action_ABC* CreateCrowdCreationAction( const kernel::PopulationType& type, int number, const geometry::Point2f& point, const kernel::Entity_ABC& selected, kernel::Controller& controller, kernel::AgentTypes& agentTypes, kernel::CoordinateConverter_ABC& coordinateConverter );
    Action_ABC* CreateInhabitantChangeHealthStateAction( int healthy, int wounded, int dead, const kernel::Entity_ABC& selected, kernel::Controller& controller, kernel::AgentTypes& agentTypes );
    Action_ABC* CreateObjectMagicAction( const std::string& action, unsigned long targetId );
    void Destroy( const Action_ABC& action );
    void Purge( const ActionsFilter_ABC* filter = 0 );
    void Load( const std::string& filename, bool readonly = false );
    void Save( const std::string& filename, const ActionsFilter_ABC* filter = 0 ) const;
    void Publish( const Action_ABC& action, int context = 0 );
    void PublishForce( const Action_ABC& action );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionsModel( const ActionsModel& );            //!< Copy constructor
    ActionsModel& operator=( const ActionsModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadAction( xml::xistream& xis, bool readonly, std::string& errors );
    //@}

private:
    //! @name Member data
    //@{
    ActionFactory_ABC& factory_;
    Publisher_ABC& publisher_;
    Publisher_ABC& defaultPublisher_;
    //@}
};

}

#endif // __ActionsModel_h_
