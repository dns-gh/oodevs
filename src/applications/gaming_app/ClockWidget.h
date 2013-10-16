// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ClockWidget_h_
#define __ClockWidget_h_

#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Time_ABC;
}

class ActionsScheduler;
class Publisher_ABC;
class Profile;
class Simulation;

// =============================================================================
/** @class  ClockWidget
    @brief  ClockWidget
*/
// Created: SBO 2007-04-17
// =============================================================================
class ClockWidget : public Q3HBox
                  , public tools::Observer_ABC
                  , public tools::ElementObserver_ABC< Simulation >
{
public:
    //! @name Constructors/Destructor
    //@{
             ClockWidget( QWidget* parent, kernel::Controllers& controllers, const kernel::Time_ABC& simulation, ActionsScheduler& scheduler );
    virtual ~ClockWidget();
    //@}

    //! @name Operations
    //@{
    void SetAlarmVisible( bool visible );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const Simulation& simulation );
    virtual void paintEvent( QPaintEvent* e );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QLabel* time_;
    QLabel* day_;
    QPushButton* alarmButton_;
    //@}
};

#endif // __ClockWidget_h_
