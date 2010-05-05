// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __TimelinePanel_h_
#define __TimelinePanel_h_

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

namespace tools
{
    class ExerciseConfig;
}

class ActionsScheduler;

// =============================================================================
/** @class  TimelinePanel
    @brief  TimelinePanel
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelinePanel : public QDockWindow
{

public:
    //! @name Constructors/Destructor
    //@{
             TimelinePanel( QMainWindow* parent, kernel::Controllers& controllers, actions::ActionsModel& model, ActionsScheduler& scheduler, const tools::ExerciseConfig& config, gui::ItemFactory_ABC& factory );
    virtual ~TimelinePanel();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelinePanel( const TimelinePanel& );            //!< Copy constructor
    TimelinePanel& operator=( const TimelinePanel& ); //!< Assignment operator
    //@}
};

#endif // __TimelinePanel_h_
