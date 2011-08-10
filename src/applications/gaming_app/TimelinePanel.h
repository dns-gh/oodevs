// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __TimelinePanel_h_
#define __TimelinePanel_h_

#include <boost/ptr_container/ptr_vector.hpp>

namespace kernel
{
    class Controllers;
    class Profile_ABC;
}

namespace actions
{
    class ActionsFilter_ABC;
    class ActionsModel;
}

namespace gui
{
    class ItemFactory_ABC;
}

namespace tools
{
    class SessionConfig;
}

class ActionsScheduler;
class ActionsToolbar;
class TimelineWidget;

// =============================================================================
/** @class  TimelinePanel
    @brief  TimelinePanel
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelinePanel : public QDockWidget
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TimelinePanel( QMainWindow* parent, kernel::Controllers& controllers, actions::ActionsModel& model, ActionsScheduler& scheduler, const tools::SessionConfig& config, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile );
    virtual ~TimelinePanel();
    //@}

signals:
    //! @name Signal
    //@{
    void PlanificationModeChange();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    TimelinePanel( const TimelinePanel& );            //!< Copy constructor
    TimelinePanel& operator=( const TimelinePanel& ); //!< Assignment operator
    //@}

    //! @name Types
    //@{
public:
    typedef boost::ptr_vector< actions::ActionsFilter_ABC > T_Filters;
    //@}

private:
    //! @name Member data
    //@{
    ActionsToolbar* toolbar_;
    TimelineWidget* timeline_;
    QTabWidget* tabs_;
    T_Filters filters_;
    //@}
};

#endif // __TimelinePanel_h_
