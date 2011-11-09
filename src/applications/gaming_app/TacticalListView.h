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

#include "clients_gui/ChangeSuperior_ABC.h"
#include "clients_gui/HierarchyListView.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "protocol/ServerPublisher_ABC.h"

namespace gui
{
    class ChangeSuperiorDialog;
}

namespace kernel
{
    class Agent_ABC;
    class Automat_ABC;
    class Formation_ABC;
    class Time_ABC;
}

namespace actions
{
    class ActionsModel;
}

class AutomatDecisions;
class StaticModel;

// =============================================================================
/** @class  TacticalListView
    @brief  TacticalListView
    // $$$$ SBO 2006-11-30: Factorize additional icon column
*/
// Created: AGE 2006-11-23
// =============================================================================
class TacticalListView : public gui::HierarchyListView< kernel::TacticalHierarchies >
                       , public tools::ElementObserver_ABC< AutomatDecisions >
                       , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
                       , public gui::ChangeSuperior_ABC
{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             TacticalListView( QWidget* pParent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel, const StaticModel& staticModel, const kernel::Time_ABC& simulation, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons );
    virtual ~TacticalListView();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const kernel::Entity_ABC& agent, gui::ValuedListItem* item );
    virtual void NotifyUpdated( const AutomatDecisions& decisions );
    virtual void NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    virtual bool CanChangeSuperior( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior ) const;
    virtual void DoChangeSuperior( kernel::Entity_ABC& entity, kernel::Entity_ABC& superior );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChangeSuperior();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void hideEvent( QHideEvent* event );
    virtual void viewportResizeEvent( QResizeEvent* e );
    virtual void setColumnWidth( int column, int w );

    virtual bool Drop( const kernel::Entity_ABC& item, const kernel::Entity_ABC& target );
    bool Drop( const kernel::Agent_ABC&   item,  const kernel::Automat_ABC& target );
    bool Drop( const kernel::Automat_ABC& item,  const kernel::Automat_ABC& target );
    bool Drop( const kernel::Automat_ABC& item,  const kernel::Formation_ABC& target );
    //@}

private:
    //! @name Member data
    //@{
    actions::ActionsModel& actionsModel_;
    const StaticModel& static_;
    const kernel::Time_ABC& simulation_;
    QPixmap lock_, commandPost_;
    gui::ChangeSuperiorDialog* changeSuperiorDialog_;
    //@}
};

#endif // __TacticalListView_h_
