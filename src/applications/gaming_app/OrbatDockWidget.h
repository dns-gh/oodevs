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

#include "clients_gui/RichDockWidget.h"
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
    class ParametersLayer;
    class RichView_ABC;
}

namespace kernel
{
    class Controllers;
    class Time_ABC;
}

class DrawingsBuilder;
class LogisticTreeView;
class ProfileFilter;
class StaticModel;

// =============================================================================
/** @class  OrbatDockWidget
    @brief  OrbatDockWidget
*/
// Created: JSR 2012-09-26
// =============================================================================
class OrbatDockWidget : public gui::RichDockWidget
{

public:
    //! @name Constructors/Destructor
    //@{
    OrbatDockWidget( kernel::Controllers& controllers,
                     QWidget* parent,
                     const QString& objectName,
                     ProfileFilter& filter,
                     gui::AutomatsLayer& automats,
                     gui::FormationLayer& formations,
                     actions::ActionsModel& actionsModel,
                     const StaticModel& staticModel,
                     const kernel::Time_ABC& simulation,
                     const gui::EntitySymbols& icons,
                     DrawingsBuilder& drawingsBuilder,
                     gui::ParametersLayer& paramLayer );
    virtual ~OrbatDockWidget();
    //@}

public:
    //! @name operations
    //@{
    void Load();
    void Purge();
    //@}

private:
    //! @name Member data
    //@{
    LogisticTreeView* logisticListView_;
    gui::DummyModelObserver observer_;
    std::vector< gui::RichView_ABC* > views_;
    //@}
};

#endif // __OrbatDockWidget_h_
