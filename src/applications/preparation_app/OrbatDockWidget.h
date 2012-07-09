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
    class FormationLayer;
    class EntitySymbols;
    class ItemFactory_ABC;
    class GlProxy;
    class SearchListView_ABC;
    class SymbolIcons;
}

class Model;
class StaticModel;
class ModelBuilder;
class ListViewsPanel;

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
             OrbatDockWidget( kernel::Controllers& controllers, QWidget* parent, const QString& objectName,
                              const QString& windowTitle, gui::AutomatsLayer& automats, gui::FormationLayer& formation,
                              gui::EntitySymbols& icons, ModelBuilder& modelBuilder, gui::ItemFactory_ABC& factory,
                              Model& model, StaticModel& staticModel, gui::GlProxy& glProxy,
                              std::vector< gui::SearchListView_ABC* >& listViews, gui::SymbolIcons& symbols );
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
    virtual void NotifyModeChanged( int newMode, bool useDefault, bool firstChangeToSavedMode );
    void Expand( bool value );
    //@}

private:
    //! @name Member data
    //@{
    QToolButton* expandButton_;
    ListViewsPanel* pListViewPanel_;
    QIcon expandIcon_;
    QIcon collapseIcon_;
    bool expanded_;
    int oldMinWith_;
    int oldMaxWith_;
    //@}
};

#endif // OrbatDockWidget_h
