// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __AutomatCreationListener_h_
#define __AutomatCreationListener_h_

#include "geometry/types.h"
#include "protocol/MsgsSimToClientListener.h"
#include "tools/Resolver_ABC.h"

namespace actions
{
    class ActionsModel;
}

namespace kernel
{
    class AgentTypes;
    class Automat_ABC;
    class AutomatType;
    class Controller;
    class CoordinateConverter_ABC;
    class Time_ABC;
}

// =============================================================================
/** @class  AutomatCreationListener
    @brief  AutomatCreationListener
*/
// Created: LDC 2010-10-11
// =============================================================================
class AutomatCreationListener : public MsgsSimToClient::Listener
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatCreationListener( const geometry::Point2f& point, const kernel::AutomatType& type, int context,
                 tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, kernel::Controller& controller, kernel::AgentTypes& agentTypes,
                 kernel::CoordinateConverter_ABC& coordinateConverter, actions::ActionsModel& actionsModel, const kernel::Time_ABC& time );
    virtual ~AutomatCreationListener();
    //@}

    //! @name Operations
    //@{
    virtual bool OnMessageReceived( const MsgsSimToClient::MsgSimToClient& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatCreationListener( const AutomatCreationListener& );            //!< Copy constructor
    AutomatCreationListener& operator=( const AutomatCreationListener& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    const geometry::Point2f point_;
    const kernel::AutomatType& type_;
    int context_;
    tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver_;
    kernel::Controller& controller_;
    kernel::AgentTypes& agentTypes_;
    kernel::CoordinateConverter_ABC& coordinateConverter_;
    actions::ActionsModel& actionsModel_;
    const kernel::Time_ABC& time_;
    //@}
};

#endif // __AutomatCreationListener_h_
