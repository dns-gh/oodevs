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
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <tools/Observer_ABC.h>

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class Ghost_ABC;
    class Team_ABC;
}

namespace gui
{
    class AggregateToolbar;
    class ChangeSuperiorDialog;
    class EntitySymbols;
    class HierarchyTreeView_ABC;
    class ParametersLayer;
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
                     , public tools::Observer_ABC
                     , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                     , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                     , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                     , public kernel::ContextMenuObserver_ABC< kernel::Ghost_ABC >
                     , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
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
                             gui::ParametersLayer& paramLayer );
    virtual ~TreeViewsPanel();
    //@}

public:
    //! @name Operations
    //@{
    void SetVisible( bool value );
    //@}

public slots:
    void FocusIn( gui::HierarchyTreeView_ABC* );
    void OnRename();

private:
    //! @name Helpers
    //@{
    void AddContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    //@}

    //! @name ContextMenuObserver implementations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& automat, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Ghost_ABC& automat, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Team_ABC& automat, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member Data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Entity_ABC > contextMenuEntity_;
    ModelBuilder& modelBuilder_;
    gui::RichWidget< QTabWidget >* pSecondAgentsTabWidget_;
    std::vector< gui::HierarchyTreeView_ABC* > firstUnitViews_;
    std::vector< gui::HierarchyTreeView_ABC* > secondUnitViews_;
    std::unique_ptr< gui::ChangeSuperiorDialog > changeSuperiorDialog_;
    //@}
};

#endif // __TreeViewsPanel_h__
