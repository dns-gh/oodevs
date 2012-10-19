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

#include <boost/ptr_container/ptr_vector.hpp>
#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class DisplayExtractor_ABC;
    class Profile_ABC;
}

namespace actions
{
    class ActionsFilter_ABC;
    class ActionsModel;
}

class ActionsScheduler;
class ActionsToolbar;
class Config;
class TimelineWidget;

// =============================================================================
/** @class  TimelinePanel
    @brief  TimelinePanel
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelinePanel : public QDockWidget
                    , private boost::noncopyable
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             TimelinePanel( QMainWindow* parent, kernel::Controllers& controllers, actions::ActionsModel& model, ActionsScheduler& scheduler, const Config& config, const kernel::Profile_ABC& profile, kernel::DisplayExtractor_ABC& extractor );
    virtual ~TimelinePanel();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnViewChanged();
    //@}

private:
    //! @name Types
    //@{
    typedef boost::ptr_vector< actions::ActionsFilter_ABC > T_Filters;
    //@}

private:
    //! @name Member data
    //@{
    ActionsToolbar* toolbar_;
    TimelineWidget* timeline_;
    QRadioButton *  globalView_;
    QRadioButton *  currentView_;
    T_Filters       filters_;
    //@}
};

#endif // __TimelinePanel_h_
