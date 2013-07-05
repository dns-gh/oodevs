// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __TimelineDockWidget_h_
#define __TimelineDockWidget_h_

#include "clients_gui/RichDockWidget.h"

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;
    }
}

namespace gui
{
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace kernel
{
    class Controllers;
    class Profile_ABC;
    class Time_ABC;
}

namespace timeline
{
    struct Configuration;
    struct Event;
}

namespace tools
{
    class ExerciseConfig;
}

class Config;
class EventAction;
class EventDialog;
class Model;
class TimelineToolBar;
class TimelineWebView;

// =============================================================================
/** @class  TimelineDockWidget
    @brief  TimelineDockWidget
*/
// Created: ABR 2013-05-14
// =============================================================================
class TimelineDockWidget : public gui::RichDockWidget
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TimelineDockWidget( QWidget* parent, kernel::Controllers& controllers, const Config& config, const kernel::Time_ABC& simulation,
                                 Model& model, actions::gui::InterfaceBuilder_ABC& interfaceBuilder, const kernel::Profile_ABC& profile,
                                 gui::GlTools_ABC& tools );
    virtual ~TimelineDockWidget();
    //@}

    //! @name Operations
    //@{
    void Connect();
    void Disconnect();
    void Draw( gui::Viewport_ABC& viewport );
    //@}

signals:
    //! @name Signals
    //@{
    void CreateEvent( const timeline::Event& );
    //@}

public slots:
    //! @name Slots
    //@{
    void AddView();
    void RemoveCurrentView();
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    std::auto_ptr< timeline::Configuration > cfg_;

    EventDialog* eventDialog_;

    QTabWidget* tabWidget_;
    std::vector< std::pair< int, QWidget* > > toolbars_;
    TimelineWebView* webView_;
    //@}

private:
    //! @name Static member data
    //@{
    static int maxTabNumber_;
    //@}
};

#endif // __TimelineDockWidget_h_
