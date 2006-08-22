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

namespace gui
{
    class SubItemDisplayer;
}

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
{

public:
    //! @name Constructors/Destructor
    //@{
             FireResultListView( QWidget* parent, gui::ItemFactory_ABC& factory );
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
    //! @name Copy/Assignement
    //@{
    FireResultListView( const FireResultListView& );            //!< Copy constructor
    FireResultListView& operator=( const FireResultListView& ); //!< Assignement operator
    //@}

private:
    //! @name Member data
    //@{
    gui::ItemFactory_ABC& factory_;
    gui::SubItemDisplayer* agentDisplay_;
    //@}
};

#endif // __FireResultListView_h_
