// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FireResultListView_h_
#define __FireResultListView_h_

#include "clients_gui/ListDisplayer.h"
#include "tools/Observer_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class SubItemDisplayer;
}

class Explosions;
class PopulationFireResult;
class AgentFireResult;
class Equipment;
class Casualties;

// =============================================================================
/** @class  FireResultListView
    @brief  FireResultListView
*/
// Created: AGE 2006-03-10
// =============================================================================
class FireResultListView : public gui::ListDisplayer< FireResultListView >
                         , public tools::Observer_ABC
                         , public tools::SelectionObserver< kernel::Entity_ABC >
                         , public tools::ElementObserver_ABC< Explosions >
{
public:
    //! @name Constructors/Destructor
    //@{
             FireResultListView( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~FireResultListView();
    //@}

    //! @name Operations
    //@{
    void Display( const PopulationFireResult* result, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    void Display( const AgentFireResult* result, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    void Display( const Equipment& equipment, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    void Display( const Casualties& casualties, kernel::Displayer_ABC& displayer, gui::ValuedListItem* item );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    virtual void NotifyUpdated( const Explosions& results );
    void DisplayFirer( kernel::Displayer_ABC& displayer, const kernel::Entity_ABC* firer );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ItemFactory_ABC& factory_;
    gui::SubItemDisplayer* subDisplayer_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

#endif // __FireResultListView_h_
