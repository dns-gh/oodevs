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
class EventSupervisorActionWidget : public EventDefaultWidget_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventSupervisorActionWidget( gui::EventPresenter& presenter );
    virtual ~EventSupervisorActionWidget();
    //@}
};

#endif // __EventSupervisorActionWidget_h_
