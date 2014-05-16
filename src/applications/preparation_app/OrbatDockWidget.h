// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef OrbatDockWidget_h
#define OrbatDockWidget_h

#include "clients_gui/RichDockWidget.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class AutomatsLayer;
    class EntitySymbols;
    class FormationLayer;
    class ParametersLayer;
    class RichView_ABC;
    class SymbolIcons;
    template< typename T > class RichWidget;
}

class Model;
class StaticModel;
class ModelBuilder;
class TreeViewsPanel;

// =============================================================================
/** @class  OrbatDockWidget
    @brief  Orbat dock widget
*/
// Created: LGY 2012-06-27
// =============================================================================
class OrbatDockWidget : public gui::RichDockWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             OrbatDockWidget( kernel::Controllers& controllers,
                              QWidget* parent,
                              const QString& objectName,
                              const QString& windowTitle,
                              gui::AutomatsLayer& automats,
                              gui::FormationLayer& formation,
                              gui::EntitySymbols& icons,
                              ModelBuilder& modelBuilder,
                              Model& model,
                              StaticModel& staticModel,
                              std::vector< gui::RichView_ABC* >& treeViews,
                              gui::SymbolIcons& symbols,
                              gui::ParametersLayer& paramLayer );
    virtual ~OrbatDockWidget();
    //@}

private slots:
    //! @name Helpers
    //@{
    void OnCollapseClicked();
    void RestoreWidth();
    void OnTabChanged( int index );
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode );
    void Expand( bool value );
    //@}

private:
    //! @name Member data
    //@{
    gui::RichWidget< QToolButton >* expandButton_;
    TreeViewsPanel* pTreeViewPanel_;
    QIcon expandIcon_;
    QIcon collapseIcon_;
    bool expanded_;
    int oldMinWith_;
    int oldMaxWith_;
    //@}
};

#endif // OrbatDockWidget_h
