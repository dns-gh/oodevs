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

#include <boost/noncopyable.hpp>

namespace kernel
{
    class Controllers;
    class DisplayExtractor_ABC;
}

namespace actions
{
    class ActionsFilter_ABC;
    class ActionsModel;
}

class ActionsScheduler;
class TimelineListView;
class TimelineView;

// =============================================================================
/** @class  TimelineWidget
    @brief  TimelineWidget
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelineWidget : public Q3HBox
                     , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             TimelineWidget( QWidget* parent, kernel::Controllers& controllers, actions::ActionsModel& model, ActionsScheduler& scheduler, kernel::DisplayExtractor_ABC& extractor );
    virtual ~TimelineWidget();
    //@}

    //! @name Operations
    //@{
    void SetFilter( const actions::ActionsFilter_ABC& filter );
    //@}

private:
    //! @name Member data
    //@{
    TimelineListView* list_;
    TimelineView* view_;
    //@}
};

#endif // __TimelineWidget_h_
