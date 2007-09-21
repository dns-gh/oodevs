// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterAction_h_
#define __AfterAction_h_

namespace kernel
{
    class Controllers;
}
namespace gui
{
    class ItemFactory_ABC;
}

// =============================================================================
/** @class  AfterAction
    @brief  AfterAction
*/
// Created: AGE 2007-09-17
// =============================================================================
class AfterAction : public QObject
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterAction( QMainWindow* window, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~AfterAction();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterAction( const AfterAction& );            //!< Copy constructor
    AfterAction& operator=( const AfterAction& ); //!< Assignment operator
    //@}
};

#endif // __AfterAction_h_
