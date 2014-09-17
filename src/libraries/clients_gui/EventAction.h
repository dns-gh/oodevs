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
#include "clients_kernel/SafePointer.h"

namespace actions
{
    class Action_ABC;
    class ActionsModel;
}

namespace kernel
{
    class Controllers;
}

namespace gui
{

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
             EventAction( E_EventTypes type,
                          const timeline::Event& event,
                          actions::ActionsModel& model,
                          kernel::Controllers& controllers );
    virtual ~EventAction();
    //@}

    //! @name Operations
    //@{
    virtual Event* Clone() const;
    virtual void Update( const timeline::Event& event );
    virtual const actions::Action_ABC* GetAction() const;
    void SetAction( const actions::Action_ABC* action );
    //@}

    //! @name GraphicalEntity_ABC implementation
    //@{
    virtual void Select( kernel::ActionController& controller ) const;
    virtual QString GetName() const;
    virtual QString GetTooltip() const;
    //@}

private:
    //! @name Helpers
    //@{
    void Purge();
    void UpdateTiming();
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel& model_;
    kernel::Controllers& controllers_;
    kernel::SafePointer< actions::Action_ABC > action_;
    //@}
};

} //! namespace gui

#endif // __EventAction_h_
