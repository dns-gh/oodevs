// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ClockWidget_h_
#define __ClockWidget_h_

#include "tools/ElementObserver_ABC.h"
#include "gaming/Simulation.h"

namespace kernel
{
    class Controllers;
}

class Publisher_ABC;
class Profile;

// =============================================================================
/** @class  ClockWidget
    @brief  ClockWidget
*/
// Created: SBO 2007-04-17
// =============================================================================
class ClockWidget : public QHBox
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< Simulation >
{

public:
    //! @name Constructors/Destructor
    //@{
             ClockWidget( QWidget* parent, kernel::Controllers& controllers, const Simulation& simulation );
    virtual ~ClockWidget();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ClockWidget( const ClockWidget& );            //!< Copy constructor
    ClockWidget& operator=( const ClockWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QLabel* time_;
    QLabel* day_;
    //@}
};

#endif // __ClockWidget_h_
