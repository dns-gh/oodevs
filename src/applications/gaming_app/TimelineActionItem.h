// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelineActionItem_h_
#define __TimelineActionItem_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "TimelineItem_ABC.h"

namespace kernel
{
    class Controllers;
}

class Action_ABC;
class ActionTiming;

// =============================================================================
/** @class  TimelineActionItem
    @brief  TimelineActionItem
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelineActionItem : public TimelineItem_ABC
                         , public kernel::Observer_ABC
                         , public kernel::ElementObserver_ABC< ActionTiming >
{

public:
    //! @name Constructors/Destructor
    //@{
             TimelineActionItem( const TimelineItem_ABC& parent, kernel::Controllers& controllers, const Action_ABC& action );
    virtual ~TimelineActionItem();
    //@}

    //! @name Operations
    //@{
    virtual void Update();
    virtual void setVisible( bool visible );
    void Shift( long shift );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineActionItem( const TimelineActionItem& );            //!< Copy constructor
    TimelineActionItem& operator=( const TimelineActionItem& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void draw( QPainter& painter );
    virtual void NotifyUpdated( const ActionTiming& timing );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const TimelineItem_ABC& parentItem_;
    const Action_ABC& action_;

    QPalette palette_;
    //@}
};

#endif // __TimelineActionItem_h_
