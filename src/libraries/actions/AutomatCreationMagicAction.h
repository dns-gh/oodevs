// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __AutomatCreationMagicAction_h_
#define __AutomatCreationMagicAction_h_

#include "UnitMagicAction.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class AutomatType;
    class AgentTypes;
    class CoordinateConverter_ABC;
    class Controller;
    class MagicActionType;
    class StaticModel;
    class Time_ABC;
}

class AgentServerMsgMgr;

namespace actions
{
class CreationListener_ABC;
// =============================================================================
/** @class  AutomatCreationMagicAction
    @brief  AutomatCreationMagicAction
*/
// Created: HBD 2010-12-06
// =============================================================================

class AutomatCreationMagicAction : public UnitMagicAction
{

public:
    //! @name Constructors/Destructor
    //@{
                AutomatCreationMagicAction( const kernel::Entity_ABC& entity, const kernel::MagicActionType& magic, kernel::Controller& controller, const QString& name, const kernel::StaticModel& staticModel, const kernel::AutomatType& type, const geometry::Point2f& point, tools::Resolver_ABC< kernel::Automat_ABC >& agentsModel, CreationListener_ABC& agentMessenger, ActionsModel& actionsModel,  const kernel::Time_ABC& simulation, bool registered = true );
       virtual ~AutomatCreationMagicAction();
    //@}

    //! @name Operations
    //@{
       virtual void Publish( Publisher_ABC& publisher, int context = 0 ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatCreationMagicAction( const AutomatCreationMagicAction& );            //!< Copy constructor
    AutomatCreationMagicAction& operator=( const AutomatCreationMagicAction& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const kernel::AutomatType& automatType_;
    kernel::CoordinateConverter_ABC& coordinateConverter_;
    tools::Resolver_ABC< kernel::Automat_ABC >& agentsModel_;
    CreationListener_ABC& messageManager_;
    ActionsModel& actionsModel_;
    kernel::AgentTypes& agentTypes_;
    const kernel::Time_ABC& simulation_;
    const geometry::Point2f point_;
    //@}
};
}
#endif // __AutomatCreationMagicAction_h_
