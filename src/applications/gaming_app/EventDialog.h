// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventDialog_h_
#define __EventDialog_h_

#include "clients_gui/ModalDialog.h"
#include "ENT/ENT_Enums_Gen.h"

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
/** @class  EventDialog
    @brief  EventDialog
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventDialog : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventDialog( QWidget* parent, kernel::Controllers& controllers, Model& model, const tools::ExerciseConfig& config,
                          const kernel::Time_ABC& simulation, actions::gui::InterfaceBuilder_ABC& interfaceBuilder,
                          const kernel::Profile_ABC& profile, gui::GlTools_ABC& tools );
    virtual ~EventDialog();
    //@}

    //! @name Operations
    //@{
    void Create( E_EventTypes type );
    void Edit( Event& event );
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
    //@}

signals:
    //! @name Signals
    //@{
    void CreateEvent( const timeline::Event& );
    void EditEvent( const timeline::Event& );
    void CreateInstantOrder( const EventAction& );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnTrigger();
    void OnCreateReminder();
    void OnShowDetail();
    void OnDiscard();
    void OnSave();
    //@}

private:
    //! @name Member data
    //@{
    const EventFactory& factory_;
    const kernel::Time_ABC& simulation_;
    QStackedWidget* stack_;
    EventWidget_ABC* topWidget_;
    EventWidget_ABC* currentWidget_;
    EventWidget_ABC* detailWidget_;
    EventWidget_ABC* bottomWidget_;
    int lastCurrentIndex_;
    std::auto_ptr< Event > event_;
    bool editing_;
    //@}
};

#endif // __EventDialog_h_
