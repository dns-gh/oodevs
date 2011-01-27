// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __actions_ActionFactory_ABC_h_
#define __actions_ActionFactory_ABC_h_

#include "tools/Resolver_ABC.h"
#include <geometry/types.h>

namespace sword
{
    class UnitOrder;
    class AutomatOrder;
    class CrowdOrder;
    class FragOrder;
}

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
    class Time_ABC;
}


namespace xml
{
    class xistream;
}

namespace actions
{

class Action_ABC;
class CreationListener_ABC;
class ActionsModel;

// =============================================================================
/** @class  ActionFactory_ABC
    @brief  Action factory def
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionFactory_ABC() {}
    virtual ~ActionFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual actions::Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::MissionType& mission ) const = 0;
    virtual actions::Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrderType& fragOrder ) const = 0;
    virtual actions::Action_ABC* CreateAction( xml::xistream& xis, bool readonly = false ) const = 0;
    virtual actions::Action_ABC* CreateStubAction( xml::xistream& xis ) const = 0;

    virtual actions::Action_ABC* CreateAction( const sword::UnitOrder& message ) const = 0;
    virtual actions::Action_ABC* CreateAction( const sword::AutomatOrder& message ) const = 0;
    virtual actions::Action_ABC* CreateAction( const sword::CrowdOrder& message ) const = 0;
    virtual actions::Action_ABC* CreateAction( const sword::FragOrder& message ) const = 0;
    
    virtual actions::Action_ABC* CreateAutomatCreationAction( const kernel::AutomatType& type, const kernel::Entity_ABC& selected, kernel::Controller& controller, const kernel::StaticModel& staticModel, const geometry::Point2f& point, tools::Resolver_ABC< kernel::Automat_ABC >& agentsModel,  CreationListener_ABC& agentMessenger, ActionsModel& actionsModel, const kernel::Time_ABC& simulation ) const = 0;
    virtual actions::Action_ABC* CreateAgentCreationAction( const kernel::AgentType& type, const geometry::Point2f& point, const kernel::Entity_ABC& selected_, kernel::Controller& controller, kernel::AgentTypes& agentTypes, kernel::CoordinateConverter_ABC& coordinateConverter ) const = 0;
    virtual actions::Action_ABC* CreateFormationCreationAction( int level, const kernel::Entity_ABC& selected, kernel::Controller& controller, kernel::AgentTypes& agentTypes ) const = 0;
    virtual actions::Action_ABC* CreateCrowdCreationAction( const kernel::PopulationType& type, int number, const geometry::Point2f& point, const kernel::Entity_ABC& selected, kernel::Controller& controller, kernel::AgentTypes& agentTypes, kernel::CoordinateConverter_ABC& coordinateConverter ) const = 0;

    virtual actions::Action_ABC* CreateInhabitantChangeHealthStateAction( int healthy, int wounded, int dead, const kernel::Entity_ABC& selected, kernel::Controller& controller, kernel::AgentTypes& agentTypes ) const = 0;

    virtual actions::Action_ABC* CreateObjectMagicAction( const std::string& magicAction, unsigned long targetId = 0 ) const = 0;
    //@}
};

}

#endif // __actions_ActionFactory_ABC_h_
