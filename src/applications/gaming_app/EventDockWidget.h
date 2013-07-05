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

#include "clients_gui/RichDockWidget.h"
#include "ENT/ENT_Enums_Gen.h"
#include "clients_kernel/ActivationObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "tools/ElementObserver_ABC.h"

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
    struct Event;
}

namespace tools
{
    class ExerciseConfig;
}

class Event;
class EventAction;
class EventBottomWidget;
class EventFactory;
class EventTopWidget;
class EventWidget_ABC;
class Model;

// =============================================================================
/** @class  EventDockWidget
    @brief  EventDockWidget
*/
// Created: ABR 2013-07-02
// =============================================================================
class EventDockWidget : public gui::RichDockWidget
                      , public kernel::ContextMenuObserver_ABC< Event >
                      , public tools::ElementObserver_ABC< Event >
                      , public kernel::ActivationObserver_ABC< Event >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventDockWidget( QWidget* parent, kernel::Controllers& controllers, Model& model, const tools::ExerciseConfig& config,
                              const kernel::Time_ABC& simulation, actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                              const kernel::Profile_ABC& profile, gui::GlTools_ABC& tools );
    virtual ~EventDockWidget();
    //@}
    //! @name Operations
    //@{
    void Draw( gui::Viewport_ABC& viewport );
    //@}

private:
    //! @name Helpers
    //@{
    void SetEventType( E_EventTypes type );
    void Commit( timeline::Event& event );
    void Purge();
    void Fill();
    virtual void closeEvent( QCloseEvent * event );
    void SetContentVisible( bool visible );
    void SetEditing( bool editing, bool fromTimeline );
    //@}

    //! @name Observers implementation
    //@{
    virtual void NotifyActivated( const Event& event );
    virtual void NotifyContextMenu( const Event& event, kernel::ContextMenu& menu );
    virtual void NotifyDeleted( const Event& event );
    virtual void NotifyUpdated( const Event& event );
    //@}

signals:
    //! @name Signals
    //@{
    void CreateEvent( const timeline::Event& );
    void DeleteEvent( const std::string& uuid );
    void EditEvent( const timeline::Event& );
    void BeginDateChanged( const QDateTime& );
    void EditingChanged( bool, bool );
    //@}

private slots:
    //! @name Slots
    //@{
    void StartCreation( E_EventTypes type, const QDateTime& dateTime, bool fromTimeline );
    void StartEdition( const Event& event );

    void OnEditClicked();
    void OnDeleteClicked();

    void OnTrigger();
    void OnShowDetail();
    void OnDiscard();
    void OnSave();
    //@}

private:
    //! @name Member data
    //@{
    const EventFactory& factory_;
    const kernel::Time_ABC& simulation_;
    QWidget* mainWidget_;
    QStackedWidget* stack_;
    EventWidget_ABC* topWidget_;
    EventWidget_ABC* currentWidget_;
    EventWidget_ABC* detailWidget_;
    EventWidget_ABC* bottomWidget_;
    int lastCurrentIndex_;
    std::auto_ptr< Event > event_;
    kernel::SafePointer< Event > selected_;
    bool editing_;
    //@}
};

#endif // __EventDockWidget_h_
