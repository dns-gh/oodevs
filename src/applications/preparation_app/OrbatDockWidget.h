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
                              const std::shared_ptr< gui::AutomatsLayer >& automats,
                              const std::shared_ptr< gui::FormationLayer >& formation,
                              const std::shared_ptr< gui::ParametersLayer >& paramLayer,
                              gui::EntitySymbols& icons,
                              ModelBuilder& modelBuilder,
                              Model& model,
                              StaticModel& staticModel,
                              gui::SymbolIcons& symbols );
    virtual ~OrbatDockWidget();
    //@}

    //! @name Operations
    //@{
    void Purge();
    void Load();
    //@}

private slots:
    //! @name Helpers
    //@{
    void OnCollapseClicked();
    void RestoreWidth();
    void OnTabChanged( int index );
    //@}

private:
    //! @name Helpers
    //@{
    void Expand( bool value );
    //@}

    //! @name Observers implementations
    //@{
    virtual void NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode );
    //@}

private:
    //! @name Member data
    //@{
    std::vector< gui::RichView_ABC* > views_;
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
