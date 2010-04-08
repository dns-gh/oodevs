// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionFactory_h_
#define __ActionFactory_h_

#include "ActionFactory_ABC.h"
#include "tools/Resolver_ABC.h"

namespace Common
{
    class MsgMissionParameters;
    class MsgUnitOrder;
    class MsgAutomatOrder;
    class MsgPopulationOrder;
}

namespace MsgsClientToSim
{
    class MsgFragOrder;
}

namespace kernel
{
    class Controllers;
    class OrderType;
    class OrderParameter;
    class MagicActionType;
}

class Model;
class Simulation;
class ParameterFactory_ABC;

// =============================================================================
/** @class  ActionFactory
    @brief  ActionFactory
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionFactory : public ActionFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionFactory( kernel::Controllers& controllers, const ParameterFactory_ABC& factory, const Model& model
                          , const tools::Resolver_ABC< kernel::MissionType >& missions
                          , const tools::Resolver_ABC< kernel::FragOrderType >& fragOrders
                          , const tools::Resolver_ABC< kernel::MagicActionType, std::string >& magicActions
                          , const Simulation& simulation );
    virtual ~ActionFactory();
    //@}

    //! @name Operations
    //@{
    virtual actions::Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::MissionType& mission ) const;
    virtual actions::Action_ABC* CreateAction( const kernel::Entity_ABC& target, const kernel::FragOrderType& fragOrder ) const;

    virtual actions::Action_ABC* CreateAction( const Common::MsgUnitOrder& message ) const;
    virtual actions::Action_ABC* CreateAction( const Common::MsgAutomatOrder& message ) const;
    virtual actions::Action_ABC* CreateAction( const Common::MsgPopulationOrder& message ) const;
    virtual actions::Action_ABC* CreateAction( const MsgsClientToSim::MsgFragOrder& message ) const;

    virtual actions::Action_ABC* CreateAction( xml::xistream& xis ) const;
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

    void AddParameters( actions::Action_ABC& action, const kernel::OrderType& order, const Common::MsgMissionParameters& message ) const;
    void ReadParameter( xml::xistream& xis, actions::Action_ABC& action, tools::Iterator< const kernel::OrderParameter& >& it, const kernel::Entity_ABC& entity ) const;
    void ReadParameter( xml::xistream& xis, actions::Action_ABC& action, tools::Iterator< const kernel::OrderParameter& >& it ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const ParameterFactory_ABC& factory_;
    const Model& model_;
    const tools::Resolver_ABC< kernel::MissionType >& missions_;
    const tools::Resolver_ABC< kernel::FragOrderType >& fragOrders_;
    const tools::Resolver_ABC< kernel::MagicActionType, std::string >& magicActions_;
    const Simulation& simulation_;
    //@}
};

#endif // __ActionFactory_h_
