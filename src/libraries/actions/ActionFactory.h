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

namespace Common
{
    class MsgMissionParameters;
}

namespace kernel
{
    class Controller;
    class EntityResolver_ABC;
    class OrderType;
    class OrderParameter;
    class MagicActionType;
    class StaticModel;
    class Time_ABC;
}

namespace actions
{
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
    virtual actions::Action_ABC* CreateAction( xml::xistream& xis ) const;

    virtual actions::Action_ABC* CreateAction( const Common::MsgUnitOrder& message ) const;
    virtual actions::Action_ABC* CreateAction( const Common::MsgAutomatOrder& message ) const;
    virtual actions::Action_ABC* CreateAction( const Common::MsgPopulationOrder& message ) const;
    virtual actions::Action_ABC* CreateAction( const MsgsClientToSim::MsgFragOrder& message ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionFactory( const ActionFactory& );            //!< Copy constructor
    ActionFactory& operator=( const ActionFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    actions::Action_ABC* CreateMission( xml::xistream& xis ) const;
    actions::Action_ABC* CreateFragOrder( xml::xistream& xis ) const;
    actions::Action_ABC* CreateMagicAction( xml::xistream& xis ) const;
    actions::Action_ABC* CreateUnitMagicAction( xml::xistream& xis ) const;
    actions::Action_ABC* CreateObjectMagicAction( xml::xistream& xis ) const;
    actions::Action_ABC* CreateKnowledgeGroupMagicAction( xml::xistream& xis ) const;

    void AddParameters( actions::Action_ABC& action, const kernel::OrderType& order, const Common::MsgMissionParameters& message ) const;
    void ReadParameter( xml::xistream& xis, actions::Action_ABC& action, tools::Iterator< const kernel::OrderParameter& >& it, const kernel::Entity_ABC& entity ) const;
    void ReadParameter( xml::xistream& xis, actions::Action_ABC& action, tools::Iterator< const kernel::OrderParameter& >& it ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const actions::ParameterFactory_ABC& factory_;
    const kernel::EntityResolver_ABC& entities_;
    const tools::Resolver_ABC< kernel::MissionType >& missions_;
    const tools::Resolver_ABC< kernel::FragOrderType >& fragOrders_;
    const tools::Resolver_ABC< kernel::MagicActionType, std::string >& magicActions_;
    const kernel::Time_ABC& simulation_;
    //@}
};

}

#endif // __actions_ActionFactory_h_
