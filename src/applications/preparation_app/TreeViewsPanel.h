// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __TreeViewsPanel_h__
#define __TreeViewsPanel_h__

#include "clients_gui/RichTabWidget.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class HierarchyTreeView_ABC;
    class EntitySymbols;
    class SymbolIcons;
    class SearchTreeView_ABC;
    class AggregateToolbar;
}

class ModelBuilder;
class Model;
class StaticModel;

// =============================================================================
/** @class  TreeViewsPanel
    @brief  Tree views panel
*/
// Created: LGY 2012-06-26
// =============================================================================
class TreeViewsPanel : public gui::RichTabWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TreeViewsPanel( kernel::Controllers& controllers, gui::EntitySymbols& icons, ModelBuilder& modelBuilder,
                             Model& model, std::vector< gui::SearchTreeView_ABC* >& treeViews, gui::SymbolIcons& symbols,
                             StaticModel& staticModel, const gui::AggregateToolbar& aggregateToolbar );
    virtual ~TreeViewsPanel();
    //@}

public:
    //! @name Operations
    //@{
    void SetVisible( bool value );
    //@}

public slots:
    void FocusIn( gui::HierarchyTreeView_ABC* );

private:
    //! @name Helpers
    //@{
    void CreateUnitTabWidget( gui::RichTabWidget* parent, gui::RichTabWidget* tabWidget, kernel::Controllers& controllers,
                              gui::EntitySymbols& icons, ModelBuilder& modelBuilder, Model& model, StaticModel& staticModel,
                              std::vector< gui::SearchTreeView_ABC* >& treeViews, const gui::AggregateToolbar& aggregateToolbar, bool first );
    void Configure( gui::SearchTreeView_ABC* searchTreeView, std::vector< gui::SearchTreeView_ABC* >& treeViews,
                    const gui::AggregateToolbar& aggregateToolbar, int readOnlyMode = -1 );
    //@}

private:
    //! @name Member Data
    //@{
    gui::RichTabWidget* pSecondAgentsTabWidget_;
    std::vector< gui::HierarchyTreeView_ABC* > firstUnitViews_;
    std::vector< gui::HierarchyTreeView_ABC* > secondUnitViews_;
    //@}
};

#endif // __TreeViewsPanel_h__