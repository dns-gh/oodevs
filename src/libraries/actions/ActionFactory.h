// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __actions_ActionFactory_h_
#define __actions_ActionFactory_h_

#include "ActionFactory_ABC.h"
#include "tools/Resolver_ABC.h"

namespace sword
{
    class MissionParameters;
}

namespace kernel
{
    class Automat_ABC;
    class Controller;
    class CoordinateConverter_ABC;
    class EntityResolver_ABC;
    class OrderType;
    class OrderParameter;
    class MagicActionType;
    class StaticModel;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
    class CreationListener_ABC;
    class ParameterFactory_ABC;

// =============================================================================
/** @class  ActionFactory
    @brief  Action factory base implementation
*/
// Created: SBO 2010-05-07
// =============================================================================
class ActionFactory : public actions::ActionFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionFactory( kernel::Controller& controller, const actions::ParameterFactory_ABC& factory, const kernel::EntityResolver_ABC& entities
                          , const kernel::StaticModel& staticModel, const kernel::Time_ABC& simulation );
    virtual ~ActionFactory();
    //@}

    //! @name Operations
    //@{
    virtual actions::Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::MissionType& mission ) const;
    virtual actions::Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrderType& fragOrder ) const;
    virtual actions::Action_ABC* CreateAction( xml::xistream& xis, bool readonly = false ) const;
    virtual actions::Action_ABC* CreateStubAction( xml::xistream& xis ) const;

    virtual actions::Action_ABC* CreateAction( const sword::UnitOrder& message ) const;
    virtual actions::Action_ABC* CreateAction( const sword::AutomatOrder& message ) const;
    virtual actions::Action_ABC* CreateAction( const sword::CrowdOrder& message ) const;
    virtual actions::Action_ABC* CreateAction( const sword::FragOrder& message ) const;

    virtual actions::Action_ABC* CreateAutomatCreationAction( const kernel::AutomatType& type, const kernel::Entity_ABC& selected, const geometry::Point2f& point, tools::Resolver_ABC< kernel::Automat_ABC >& agentsModel, CreationListener_ABC& agentMessenger, ActionsModel& actionsModel, const kernel::Time_ABC& simulation ) const;
    virtual actions::Action_ABC* CreateAgentCreationAction( const kernel::AgentType& type, const geometry::Point2f& point, const kernel::Entity_ABC& selected_ ) const;
    virtual actions::Action_ABC* CreateFormationCreationAction( int level, const kernel::Entity_ABC& selected ) const;
    virtual actions::Action_ABC* CreateCrowdCreationAction( const kernel::PopulationType& type, int number, const geometry::Point2f& point, const kernel::Entity_ABC& selected ) const;

    virtual actions::Action_ABC* CreateObjectMagicAction( const std::string& magicAction, unsigned long targetId = 0 ) const;

    virtual actions::Action_ABC* CreateInhabitantChangeHealthStateAction( int healthy, int wounded, int dead, const kernel::Entity_ABC& selected ) const;
    virtual actions::Action_ABC* CreateInhabitantChangeAlertedStateAction( bool alerted, const kernel::Entity_ABC& selected ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionFactory( const ActionFactory& );            //!< Copy constructor
    ActionFactory& operator=( const ActionFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    actions::Action_ABC* CreateMission( xml::xistream& xis, bool readonly ) const;
    actions::Action_ABC* CreateFragOrder( xml::xistream& xis, bool readonly ) const;
    actions::Action_ABC* CreateMagicAction( xml::xistream& xis, bool readonly ) const;
    actions::Action_ABC* CreateUnitMagicAction( xml::xistream& xis, bool readonly ) const;
    actions::Action_ABC* CreateObjectMagicAction( xml::xistream& xis, bool readonly ) const;
    actions::Action_ABC* CreateKnowledgeGroupMagicAction( xml::xistream& xis, bool readonly ) const;
    actions::Action_ABC* AutomateChangeModeMagicAction( xml::xistream& xis, bool readonly ) const;

    actions::Action_ABC* CreateStubMission( xml::xistream& xis ) const;
    actions::Action_ABC* CreateStubFragOrder( xml::xistream& xis ) const;

    actions::Action_ABC* CreateMission( xml::xistream& xis, bool readonly, bool stub ) const;
    actions::Action_ABC* CreateFragOrder( xml::xistream& xis, bool readonly, bool stub ) const;

    void AddParameters( actions::Action_ABC& action, const kernel::OrderType& order, const sword::MissionParameters& message ) const;
    void ReadParameter( xml::xistream& xis, actions::Action_ABC& action, tools::Iterator< const kernel::OrderParameter& >& it, const kernel::Entity_ABC& entity ) const;
    void ReadParameter( xml::xistream& xis, actions::Action_ABC& action, tools::Iterator< const kernel::OrderParameter& >& it ) const;
    void ReadStubParameter( xml::xistream& xis, actions::Action_ABC& action, tools::Iterator< const kernel::OrderParameter& >& it, const kernel::Entity_ABC& entity ) const;
    template< typename Message >
    void AddTiming( actions::Action_ABC& action, const Message& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const actions::ParameterFactory_ABC& factory_;
    const kernel::EntityResolver_ABC& entities_;
    const kernel::StaticModel& staticModel_;
    const tools::Resolver_ABC< kernel::MissionType >& missions_;
    const tools::Resolver_ABC< kernel::FragOrderType >& fragOrders_;
    const tools::Resolver_ABC< kernel::MagicActionType, std::string >& magicActions_;
    const kernel::CoordinateConverter_ABC& coordinateConverter_;
    const kernel::Time_ABC& simulation_;
    //@}
};

}

#endif // __actions_ActionFactory_h_
