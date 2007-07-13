// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionsPanel_h_
#define __ActionsPanel_h_

namespace kernel
{
    class Controllers;
}

namespace gui
{
    class ItemFactory_ABC;
}

class ActionsModel;
class Publisher_ABC;
class Simulation;

// =============================================================================
/** @class  ActionsPanel
    @brief  ActionsPanel
*/
// Created: SBO 2007-03-12
// =============================================================================
class ActionsPanel : public QDockWindow
{

public:
    //! @name Constructors/Destructor
    //@{
             ActionsPanel( QMainWindow* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, ActionsModel& actions, Publisher_ABC& publisher, const Simulation& simulation );
    virtual ~ActionsPanel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionsPanel( const ActionsPanel& );            //!< Copy constructor
    ActionsPanel& operator=( const ActionsPanel& ); //!< Assignment operator
    //@}
};

#endif // __ActionsPanel_h_
