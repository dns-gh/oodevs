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

namespace kernel
{
    class Time_ABC;
}

namespace timeline
{
    struct Event;
}

class Event;
class EventAction;
class EventBottomWidget;
class EventFactory;
class EventTopWidget;
class EventWidget_ABC;

// =============================================================================
/** @class  EventDialog
    @brief  EventDialog
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventDialog : public ModalDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventDialog( QWidget* parent, const EventFactory& factory, const kernel::Time_ABC& simulation );
    virtual ~EventDialog();
    //@}

    //! @name Operations
    //@{
    void Create( E_EventTypes type );
    void Edit( Event& event );
    //@}

private:
    //! @name Helpers
    //@{
    void SetEventType( E_EventTypes type );
    void Commit( timeline::Event& event );
    void Fill();
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
