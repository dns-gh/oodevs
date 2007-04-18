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
    class Agent_ABC;
    class Automat_ABC;
}

class AutomatDecisions;
class Publisher_ABC;

// =============================================================================
/** @class  TacticalListView
    @brief  TacticalListView
    // $$$$ SBO 2006-11-30: Factorize additional icon column
*/
// Created: AGE 2006-11-23
// =============================================================================
class TacticalListView : public gui::HierarchyListView< kernel::TacticalHierarchies >
                       , public kernel::ElementObserver_ABC< AutomatDecisions >
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalListView( QWidget* pParent, kernel::Controllers& controllers, Publisher_ABC& publisher, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons );
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
    bool Drop( const kernel::Agent_ABC& item,  const kernel::Automat_ABC& target );
    //@}

private:
    //! @name Member data
    //@{
    Publisher_ABC& publisher_;
    QPixmap lock_, commandPost_;
    //@}
};

#endif // __TacticalListView_h_
