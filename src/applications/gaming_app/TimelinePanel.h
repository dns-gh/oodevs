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

#include "clients_gui/RichDockWidget.h"
#include <boost/ptr_container/ptr_vector.hpp>

namespace kernel
{
    class Controllers;
    class DisplayExtractor_ABC;
    class Profile_ABC;
}

namespace actions
{
    class ActionsFilter_ABC;
}

class ActionsScheduler;
class ActionsToolbar;
class GamingConfig;
class Model;
class TimelineWidget;

// =============================================================================
/** @class  TimelinePanel
    @brief  TimelinePanel
*/
// Created: SBO 2007-07-04
// =============================================================================
class TimelinePanel : public gui::RichDockWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TimelinePanel( QMainWindow* parent, kernel::Controllers& controllers, Model& model, ActionsScheduler& scheduler, const GamingConfig& config, const kernel::Profile_ABC& profile, kernel::DisplayExtractor_ABC& extractor );
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
