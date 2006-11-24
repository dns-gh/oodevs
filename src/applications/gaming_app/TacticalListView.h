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
class AutomatDecisions;

// =============================================================================
/** @class  TacticalListView
    @brief  TacticalListView
*/
// Created: AGE 2006-11-23
// =============================================================================
class TacticalListView : public gui::HierarchyListView< kernel::TacticalHierarchies >
                       , public kernel::ElementObserver_ABC< AutomatDecisions >
{

public:
    //! @name Constructors/Destructor
    //@{
             TacticalListView( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile, gui::SymbolIcons& icons );
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
};

#endif // __TacticalListView_h_
