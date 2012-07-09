// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TacticalListView_h_
#define __TacticalListView_h_

#include "clients_gui/ChangeSuperior_ABC.h"
#include "clients_gui/HierarchyListView.h"
#include "clients_kernel/TacticalHierarchies.h"

namespace gui
{
    class ChangeSuperiorDialog;
}

namespace kernel
{
    class Agent_ABC;
    class AgentTypes;
    class Automat_ABC;
    class Formation_ABC;
    class FormationLevels;
    class HierarchyLevel_ABC;
    class Ghost_ABC;
    class KnowledgeGroup_ABC;
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
                       , public tools::ElementObserver_ABC< kernel::Formation_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Ghost_ABC >
                       , public gui::ChangeSuperior_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TacticalListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, gui::EntitySymbols& icons,
                               ModelBuilder& modelBuilder, const kernel::FormationLevels& levels, const kernel::AgentTypes& agentTypes );
    virtual ~TacticalListView();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Entity_ABC& agent, gui::ValuedListItem* item );
    virtual bool CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const;
    virtual void DoChangeSuperior( kernel::Entity_ABC& entity, kernel::Entity_ABC& superior );
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::TacticalHierarchies& hierarchy );
    virtual void NotifyUpdated( const kernel::TacticalHierarchies& hierarchy );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnRename();
    void OnChangeSuperior();
    void Engage();
    void Disengage();
    void ChangeAutomatType();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void OnContextMenuRequested( Q3ListViewItem*, const QPoint&, int );
    virtual void hideEvent( QHideEvent* event );
    virtual void viewportResizeEvent( QResizeEvent* e );
    virtual void setColumnWidth( int column, int w );

    virtual void NotifyUpdated( const kernel::Entity_ABC& entity );
    virtual void NotifyUpdated( const AutomatDecisions& decisions );
    virtual void NotifyCreated( const kernel::Formation_ABC& entity );
    virtual void NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Team_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Ghost_ABC& agent, kernel::ContextMenu& menu );
    virtual void keyPressEvent( QKeyEvent* event );

    bool Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target );
    bool Drop( const kernel::Agent_ABC& item, const kernel::Entity_ABC& target );
    bool Drop( const kernel::Automat_ABC& item, const kernel::Entity_ABC& target );
    bool Drop( const kernel::Formation_ABC& item, const kernel::Entity_ABC& target );
    bool Drop( const kernel::Ghost_ABC& item, const kernel::Entity_ABC& target );
    bool Drop( const kernel::KnowledgeGroup_ABC& item, const kernel::Entity_ABC& target );
    void AddFormationMenu( kernel::ContextMenu& menu, const kernel::HierarchyLevel_ABC& root );
    void UpdatePixmap( const kernel::Entity_ABC& entity, gui::ValuedListItem* item );
    //@}

private:
    //! @name Member data
    //@{
    gui::ItemFactory_ABC& itemFactory_;
    const kernel::AgentTypes& agentTypes_;
    ModelBuilder& modelBuilder_;
    const kernel::FormationLevels& levels_;
    QPixmap lock_;
    QPixmap commandPost_;
    gui::ChangeSuperiorDialog* changeSuperiorDialog_;
    kernel::SafePointer< kernel::Entity_ABC > contextMenuEntity_;
    //@}
};

#endif // __TacticalListView_h_
