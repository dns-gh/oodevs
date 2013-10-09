// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterAction_h_
#define __AfterAction_h_

#include "clients_gui/RichDockWidget.h"
#include "tools/ElementObserver_ABC.h"

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;
    }
}

namespace kernel
{
    class Controllers;
}

class AfterActionModel;
class AfterActionRequest;
class IndicatorPlotFactory;

// =============================================================================
/** @class  AfterAction
    @brief  AfterAction
*/
// Created: AGE 2007-09-17
// =============================================================================
class AfterAction : public gui::RichDockWidget
                  , public tools::ElementObserver_ABC< AfterActionRequest >
{
public:
    //! @name Constructors/Destructor
    //@{
             AfterAction( QWidget* parent, kernel::Controllers& controllers, AfterActionModel& model,
                          IndicatorPlotFactory& plotFactory, actions::gui::InterfaceBuilder_ABC& interfaceBuilder );
    virtual ~AfterAction();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const AfterActionRequest& );
    //@}

private:
    //! @name Member data
    //@{
    QTabWidget* functionsTab_;
    //@}
};

#endif // __AfterAction_h_
