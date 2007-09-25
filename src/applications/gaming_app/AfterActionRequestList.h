// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionRequestList_h_
#define __AfterActionRequestList_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_gui/ListDisplayer.h"

namespace kernel
{
    class Controllers;
}

class AfterActionModel;

// =============================================================================
/** @class  AfterActionRequestList
    @brief  AfterActionRequestList
*/
// Created: AGE 2007-09-25
// =============================================================================
class AfterActionRequestList : public QVBox
                             , public kernel::Observer_ABC
                             , public kernel::ElementObserver_ABC< AfterActionModel > 
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionRequestList( QWidget* parent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory );
    virtual ~AfterActionRequestList();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionRequestList( const AfterActionRequestList& );            //!< Copy constructor
    AfterActionRequestList& operator=( const AfterActionRequestList& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

#endif // __AfterActionRequestList_h_
