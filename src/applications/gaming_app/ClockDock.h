// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClockDock_h_
#define __ClockDock_h_

namespace kernel
{
    class Controllers;
}

// =============================================================================
/** @class  ClockDock
    @brief  ClockDock
*/
// Created: SBO 2007-04-17
// =============================================================================
class ClockDock : public QDockWindow
{

public:
    //! @name Constructors/Destructor
    //@{
             ClockDock( QWidget* parent, kernel::Controllers& controllers );
    virtual ~ClockDock();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ClockDock( const ClockDock& );            //!< Copy constructor
    ClockDock& operator=( const ClockDock& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __ClockDock_h_
