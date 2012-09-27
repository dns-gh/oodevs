// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __OrbatDockWidget_h_
#define __OrbatDockWidget_h_

#include "clients_gui/DummyModelObserver.h"

namespace actions
{
    class ActionsModel;
}

namespace gui
{
    class AutomatsLayer;
    class EntitySymbols;
    class FormationLayer;
    class ItemFactory_ABC;
}

namespace kernel
{
    class Controllers;
    class Time_ABC;
}

class LogisticListView;
class ProfileFilter;
class StaticModel;

// =============================================================================
/** @class  OrbatDockWidget
    @brief  OrbatDockWidget
*/
// Created: JSR 2012-09-26
// =============================================================================
class OrbatDockWidget : public QDockWidget
{

public:
    //! @name Constructors/Destructor
    //@{
    OrbatDockWidget( kernel::Controllers& controllers, QWidget* parent, const QString& objectName, const QString& windowTitle,
                     ProfileFilter& filter, gui::AutomatsLayer& automats, gui::FormationLayer& formations,
                     actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation,
                     gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons );
    virtual ~OrbatDockWidget();
    //@}

public:
    //! @name operations
    //@{
    void Purge();
    //@}

private:
    //! @name Member data
    //@{
    LogisticListView* logisticListView_;
    gui::DummyModelObserver observer_;
    //@}
};

#endif // __OrbatDockWidget_h_
