// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionsListView_h_
#define __ActionsListView_h_

#include "clients_gui/ListDisplayer.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

class Action_ABC;
class ActionParameter_ABC;

// =============================================================================
/** @class  ActionsListView
    @brief  ActionsListView
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionsListView : public gui::ListDisplayer< ActionsListView >
                      , public kernel::Observer_ABC
                      , public kernel::ElementObserver_ABC< Action_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~ActionsListView();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const ActionParameter_ABC& param, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionsListView( const ActionsListView& );            //!< Copy constructor
    ActionsListView& operator=( const ActionsListView& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Action_ABC& action );
    virtual void NotifyUpdated( const Action_ABC& action );
    virtual void NotifyDeleted( const Action_ABC& action );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ItemFactory_ABC& factory_;
    //@}
};

#endif // __ActionsListView_h_
