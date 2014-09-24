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

#include "clients_gui/RichWidget.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class AggregateToolbar;
    class ChangeSuperiorDialog;
    class EntitySymbols;
    class EntityTreeView_ABC;
    class HierarchyTreeView_ABC;
    class ParametersLayer;
    class RenameInterface;
    class RichView_ABC;
    class SymbolIcons;
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
class TreeViewsPanel : public gui::RichWidget< QTabWidget >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TreeViewsPanel( kernel::Controllers& controllers,
                             gui::EntitySymbols& icons,
                             ModelBuilder& modelBuilder,
                             Model& model,
                             std::vector< gui::RichView_ABC* >& treeViews,
                             gui::SymbolIcons& symbols,
                             StaticModel& staticModel,
                             const gui::AggregateToolbar& aggregateToolbar,
                             const std::shared_ptr< gui::ParametersLayer >& paramLayer );
    virtual ~TreeViewsPanel();
    //@}

public:
    //! @name Operations
    //@{
    void SetVisible( bool value );
    //@}

public slots:
    //! @name Slots
    //@{
    void FocusIn( gui::HierarchyTreeView_ABC* );
    //@}

private:
    //! @name Member Data
    //@{
    gui::RichWidget< QTabWidget >* pSecondAgentsTabWidget_;
    std::vector< gui::HierarchyTreeView_ABC* > firstUnitViews_;
    std::vector< gui::HierarchyTreeView_ABC* > secondUnitViews_;
    std::unique_ptr< gui::ChangeSuperiorDialog > changeSuperiorDialog_;
    std::unique_ptr< gui::RenameInterface > renameInterface_;
    //@}
};

#endif // __TreeViewsPanel_h__
