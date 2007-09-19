// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AfterActionItemsPanel_h_
#define __AfterActionItemsPanel_h_

#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
}
class AfterActionModel;
class QToolBox;

// =============================================================================
/** @class  AfterActionItemsPanel
    @brief  AfterActionItemsPanel
*/
// Created: AGE 2007-09-17
// =============================================================================
class AfterActionItemsPanel : public QVBox
                            , public kernel::Observer_ABC
                            , public kernel::ElementObserver_ABC< AfterActionModel >
{

public:
    //! @name Constructors/Destructor
    //@{
             AfterActionItemsPanel( QWidget* parent, kernel::Controllers& controllers );
    virtual ~AfterActionItemsPanel();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AfterActionItemsPanel( const AfterActionItemsPanel& );            //!< Copy constructor
    AfterActionItemsPanel& operator=( const AfterActionItemsPanel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const AfterActionModel& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QToolBox* toolBox_;
    //@}
};

#endif // __AfterActionItemsPanel_h_
