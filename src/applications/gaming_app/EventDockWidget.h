// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventDockWidget_h_
#define __EventDockWidget_h_

#include "ENT/ENT_Enums.h"
#include "clients_gui/EventView_ABC.h"
#include "clients_gui/EventViewState.h"
#include "clients_gui/RichDockWidget.h"
#include "clients_kernel/ActivationObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include <tools/ElementObserver_ABC.h>
#include <tools/SelectionObserver_ABC.h>
#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>

namespace actions
{
    class Action_ABC;
    namespace gui
    {
        class InterfaceBuilder_ABC;
    }
}

namespace gui
{
    class EntitySymbols;
    class Event;
    class EventAction;
    class EventFactory;
    class EventPresenter;
    struct EventViewState;
    class GlTools_ABC;
    class Viewport_ABC;
}

namespace kernel
{
    class Controllers;
    class Profile_ABC;
    class Time_ABC;
    class TimelineHandler_ABC;
}

namespace timeline
{
    struct Event;
}

namespace tools
{
    class ExerciseConfig;
}

template< typename T > class EventWidget_ABC;
class Model;

// =============================================================================
/** @class  EventDockWidget
    @brief  EventDockWidget
*/
// Created: ABR 2013-07-02
// =============================================================================
class EventDockWidget : public gui::RichDockWidget
                      , public gui::EventDefaultView_ABC
                      , public kernel::ContextMenuObserver_ABC< gui::Event >
                      , public tools::ElementObserver_ABC< gui::Event >
                      , public tools::SelectionObserver< gui::Event >
                      , public kernel::ActivationObserver_ABC< gui::Event >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventDockWidget( QWidget* parent,
                              kernel::Controllers& controllers,
                              Model& model,
                              const tools::ExerciseConfig& config,
                              const kernel::Time_ABC& simulation,
                              actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                              const kernel::Profile_ABC& profile,
                              gui::GlTools_ABC& tools,
                              const gui::EntitySymbols& entitySymbols );
    virtual ~EventDockWidget();
    //@}
    //! @name Operations
    //@{
    virtual void Draw( gui::Viewport_ABC& viewport );
    gui::EventPresenter& GetPresenter() const;
    void SetTimelineHandler( const boost::shared_ptr< kernel::TimelineHandler_ABC >& timelineHandler );
    //@}

private:
    //! @name types
    //@{
    typedef std::function< void( gui::EventDefaultView_ABC* ) > T_ViewFunctor;
    //@}

private:
    //! @name gui::EventDefaultView_ABC implementation
    //@{
    virtual void Purge();
    virtual void Build( const gui::EventViewState& state );
    virtual void BlockSignals( bool blocked );
    //@}

    //! @name Observers implementation
    //@{
    virtual void NotifyActivated( const gui::Event& event );
    virtual void NotifySelected( const gui::Event* event );
    virtual void NotifyContextMenu( const gui::Event& event, kernel::ContextMenu& menu );
    virtual void NotifyDeleted( const gui::Event& event );
    virtual void NotifyUpdated( const gui::Event& event );
    //@}

    //! @name RichDockWidget implementation
    //@{
    virtual void NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode );
    //@}

    //! @name Helpers
    //@{
    virtual void closeEvent( QCloseEvent * event );
    void SetContentVisible( bool visible );
    void ApplyToViews( const T_ViewFunctor& functor );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnEditClicked();
    void OnDeleteClicked();
    //@}

private:
    //! @name Member data
    //@{
    boost::scoped_ptr< gui::EventPresenter > presenter_;
    kernel::SafePointer< gui::Event > selected_;
    QStackedWidget* stack_;
    std::vector< gui::EventDefaultView_ABC* > views_;
    boost::shared_ptr< kernel::TimelineHandler_ABC > timelineHandler_;
    //@}
};

#endif // __EventDockWidget_h_
