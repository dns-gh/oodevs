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
#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

namespace kernel
{
    class ModelLoaded;
    class Formation_ABC;
}

class AutomatDecisions;
class ModelBuilder;
class FormationLevels;

// =============================================================================
/** @class  TacticalListView
    @brief  TacticalListView
*/
// Created: SBO 2006-08-29
// =============================================================================
class TacticalListView : public gui::HierarchyListView< kernel::TacticalHierarchies >
                       , public kernel::ElementObserver_ABC< kernel::ModelLoaded >
                       , public kernel::ElementObserver_ABC< kernel::Entity_ABC >
                       , public kernel::ElementObserver_ABC< AutomatDecisions >
                       , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                       , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                       , public kernel::SelectionObserver_Base< kernel::Formation_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TacticalListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ModelBuilder& modelBuilder, const FormationLevels& levels );
    virtual ~TacticalListView();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Hierarchies& hierarchy, gui::ValuedListItem* item );
    virtual void Display( const kernel::Entity_ABC& agent, gui::ValuedListItem* item );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnContextMenuRequested( QListViewItem*, const QPoint&, int );
    void Engage();
    void Disengage();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    TacticalListView( const TacticalListView& );            //!< Copy constructor
    TacticalListView& operator=( const TacticalListView& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& );
    virtual void NotifyUpdated( const kernel::Entity_ABC& );
    virtual void NotifyUpdated( const AutomatDecisions& );
    virtual void NotifyContextMenu( const kernel::Team_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& agent, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu );
    virtual void keyPressEvent( QKeyEvent* event );

    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::Formation_ABC& element );
    virtual bool Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target );
    virtual bool Drop( const kernel::Agent_ABC& item, const kernel::Entity_ABC& target );
    virtual bool Drop( const kernel::Formation_ABC& item, const kernel::Entity_ABC& target );
    //@}

private:
    //! @name Member data
    //@{
    gui::ItemFactory_ABC& factory_;
    ModelBuilder& modelBuilder_;
    const FormationLevels& levels_;
    kernel::SafePointer< kernel::Formation_ABC > selectedFormation_;
    //@}
};

#endif // __AgentListView_h_
