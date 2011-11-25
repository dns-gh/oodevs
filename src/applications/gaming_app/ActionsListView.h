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
#include "tools/ElementObserver_ABC.h"
#include "tools/SelectionObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace actions
{
    class Action_ABC;
    class Parameter_ABC;
}

class ActionsListView;
typedef gui::ListDisplayer< ActionsListView > ActionsListViewBase;

// =============================================================================
/** @class  ActionsListView
    @brief  ActionsListView
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionsListView : public ActionsListViewBase
                      , public tools::Observer_ABC
                      , public tools::ElementObserver_ABC< actions::Action_ABC >
                      , public tools::SelectionObserver< actions::Action_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             ActionsListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~ActionsListView();
    //@}

    //! @name Operations
    //@{
    virtual void Display( const actions::Parameter_ABC& param, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const actions::Action_ABC& action );
    virtual void NotifySelected( const actions::Action_ABC* action );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QPixmap parameter_;
    //@}
};

#endif // __ActionsListView_h_
