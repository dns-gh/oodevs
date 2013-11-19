// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventFactory_h_
#define __EventFactory_h_

#include <boost/noncopyable.hpp>

namespace actions
{
    class ActionsModel;
}

namespace timeline
{
    struct Event;
}

namespace kernel
{
    class Controller;
    class Controllers;
    class Event;

// =============================================================================
/** @class  EventFactory
    @brief  EventFactory
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventFactory : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             EventFactory( actions::ActionsModel& actionsModel, Controllers& controllers );
    virtual ~EventFactory();
    //@}

    //! @name Operations
    //@{
    Event* Create( const timeline::Event& event ) const;
    Event* Create( E_EventTypes type, const timeline::Event* event = 0 ) const;
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel& actionsModel_;
    Controllers& controllers_;
    //@}
};

} //! namespace kernel

#endif // __EventFactory_h_
