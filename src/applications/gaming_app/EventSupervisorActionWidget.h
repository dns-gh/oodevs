// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventSupervisorActionWidget_h_
#define __EventSupervisorActionWidget_h_

#include "EventWidget_ABC.h"

// =============================================================================
/** @class  EventSupervisorActionWidget
    @brief  EventSupervisorActionWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventSupervisorActionWidget : public EventWidget_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             EventSupervisorActionWidget();
    virtual ~EventSupervisorActionWidget();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name EventWidget_ABC implementation
    //@{
    virtual void Fill( const Event& event );
    virtual void Commit( timeline::Event& event );
    //@}
};

#endif // __EventSupervisorActionWidget_h_
