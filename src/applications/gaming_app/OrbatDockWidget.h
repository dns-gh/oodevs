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

namespace gui
{
    class ChangeSuperiorDialog;
    class EntitySymbols;
    class GLMainProxy;
    class ModelObserver_ABC;
    class ParametersLayer;
    class RenameInterface;
    class RichView_ABC;
}

namespace kernel
{
    class Controllers;
    class Time_ABC;
}

class DrawingsBuilder;
class LogisticTreeView;
class Model;
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
                     gui::GLMainProxy& glMainProxy,
                     const std::shared_ptr< gui::ParametersLayer >& paramLayer,
                     Model& model,
                     const StaticModel& staticModel,
                     const kernel::Time_ABC& simulation,
                     const gui::EntitySymbols& icons,
                     DrawingsBuilder& drawingsBuilder );
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
    std::shared_ptr< gui::ChangeSuperiorDialog > changeSuperiorDialog_;
    std::unique_ptr< gui::ModelObserver_ABC > observer_;
    std::unique_ptr< gui::RenameInterface > renameInterface_;
    std::vector< gui::RichView_ABC* > views_;
    LogisticTreeView* logisticListView_;
    //@}
};

#endif // __OrbatDockWidget_h_
