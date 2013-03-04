// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClockDock_h_
#define __ClockDock_h_

#include "clients_gui/RichDockWidget.h"
#include "ClockWidget.h"

namespace kernel
{
    class Controllers;
    class Time_ABC;
}

class ActionsScheduler;

// =============================================================================
/** @class  ClockDock
    @brief  ClockDock
*/
// Created: SBO 2007-04-17
// =============================================================================
class ClockDock : public gui::RichDockWidget
{
public:
    //! @name Constructors/Destructor
    //@{
             ClockDock( QWidget* parent, kernel::Controllers& controllers, const kernel::Time_ABC& simulation, ActionsScheduler& scheduler );
    virtual ~ClockDock();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode );
    //@}

    //! @name Members
    //@{
    ClockWidget* clockWidget_;
    //@}
};

#endif // __ClockDock_h_
