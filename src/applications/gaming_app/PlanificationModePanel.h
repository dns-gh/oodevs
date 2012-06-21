// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef PlanificationModePanel_h
#define PlanificationModePanel_h

#include "clients_gui/RichDockWidget.h"

namespace kernel
{
    class controllers;
}

// =============================================================================
/** @class  PlanificationModePanel
    @brief  Planification mode panel
*/
// Created: LGY 2012-06-21
// =============================================================================
class PlanificationModePanel: public gui::RichDockWidget
{
public:
    //! @name Constructors/Destructor
    //@{
             PlanificationModePanel( QMainWindow* parent, kernel::Controllers& controllers );
    virtual ~PlanificationModePanel();
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};


#endif // PlanificationModePanel_h
