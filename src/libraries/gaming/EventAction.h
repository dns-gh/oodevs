// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventAction_h_
#define __EventAction_h_

#include "Event.h"

namespace actions
{
    class Action_ABC;
}

// =============================================================================
/** @class  EventAction
    @brief  EventAction
*/
// Created: ABR 2013-05-30
// =============================================================================
class EventAction : public Event
{

public:
    //! @name Constructors/Destructor
    //@{
             EventAction( E_EventTypes type, const timeline::Event& event );
    virtual ~EventAction();
    //@}

    //! @name Operations
    //@{
    virtual Event* Clone() const;
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< actions::Action_ABC > action_;
    //@}
};

#endif // __EventAction_h_
