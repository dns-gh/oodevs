// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalListView_h_
#define __TacticalListView_h_

#include "clients_gui/HierarchyListView.h"
#include "clients_kernel/TacticalHierarchies.h"

namespace kernel
{
    class Formation_ABC;
    class Automat_ABC;
    class Agent_ABC;
    class FormationLevels;
    class HierarchyLevel_ABC;
}

class AutomatDecisions;
class ModelBuilder;

// =============================================================================
/** @class  TacticalListView
    @brief  TacticalListView
    // $$$$ SBO 2006-11-30: Factorize additional icon column
*/
// Created: SBO 2006-08-29
// =============================================================================
class TacticalListView : public gui::HierarchyListView< kernel::TacticalHierarchies >
                       , public tools::ElementObserver_ABC< kernel::Entity_ABC >
                       , public tools::ElementObserver_ABC< AutomatDecisions >
                       , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TacticalListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons, ModelBuilder& modelBuilder, const kernel::FormationLevels& levels );
    virtual ~TacticalListView();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Entity_ABC& agent, gui::ValuedListItem* item );
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void OnContextMenuRequested( QListViewItem*, const QPoint&, int );
    void OnRename();
    void Engage();
    void Disengage();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void viewportResizeEvent( QResizeEvent* e );
    virtual void setColumnWidth( int column, int w );

    virtual void NotifyUpdated( const kernel::Entity_ABC& );
    virtual void NotifyUpdated( const AutomatDecisions& );
    virtual void NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Team_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    virtual void keyPressEvent( QKeyEvent* event );

    virtual bool Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target );
    virtual bool Drop( const kernel::Agent_ABC& item, const kernel::Entity_ABC& target );
    virtual bool Drop( const kernel::Automat_ABC& item, const kernel::Entity_ABC& target );
    virtual bool Drop( const kernel::Formation_ABC& item, const kernel::Entity_ABC& target );
    void AddFormationMenu( kernel::ContextMenu& menu, const kernel::HierarchyLevel_ABC& root );
    //@}

private:
    //! @name Member data
    //@{
    gui::ItemFactory_ABC& factory_;
    ModelBuilder& modelBuilder_;
    const kernel::FormationLevels& levels_;
    QPixmap lock_, commandPost_;
    //@}
};

#endif // __AgentListView_h_
