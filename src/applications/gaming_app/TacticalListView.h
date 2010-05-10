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
#include "protocol/ServerPublisher_ABC.h"

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
{

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
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TacticalListView( const TacticalListView& );            //!< Copy constructor
    TacticalListView& operator=( const TacticalListView& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
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
    //@}
};

#endif // __TacticalListView_h_
