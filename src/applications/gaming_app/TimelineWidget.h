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

namespace actions
{
    class ActionsModel;
}

namespace gui
{
    class ItemFactory_ABC;
}

class ActionsScheduler;

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
             TimelineWidget( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& model, ActionsScheduler& scheduler, gui::ItemFactory_ABC& factory );
    virtual ~TimelineWidget();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelineWidget( const TimelineWidget& );            //!< Copy constructor
    TimelineWidget& operator=( const TimelineWidget& ); //!< Assignment operator
    //@}
};

#endif // __TimelineWidget_h_
