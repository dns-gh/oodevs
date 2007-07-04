// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelineWidget_h_
#define __TimelineWidget_h_

namespace kernel
{
    class Controllers;
}

class ActionsModel;

// =============================================================================
/** @class  TimelineWidget
    @brief  TimelineWidget
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelineWidget : public QHBox
{

public:
    //! @name Constructors/Destructor
    //@{
             TimelineWidget( QWidget* parent, kernel::Controllers& controllers, ActionsModel& actions );
    virtual ~TimelineWidget();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineWidget( const TimelineWidget& );            //!< Copy constructor
    TimelineWidget& operator=( const TimelineWidget& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __TimelineWidget_h_
