// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventTopWidget_h_
#define __EventTopWidget_h_

#include "EventWidget_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace gui
{
    class RichDateTimeEdit;
}

namespace kernel
{
    class ActionController;
    class ContextMenu;
    class Time_ABC;
}

// =============================================================================
/** @class  EventTopWidget
    @brief  EventTopWidget
*/
// Created: ABR 2013-05-28
// =============================================================================
class EventTopWidget : public EventWidget_ABC
                     , public tools::Observer_ABC
                     , public kernel::ContextMenuObserver_ABC< QDateTime >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             EventTopWidget( const kernel::Time_ABC& simulation, kernel::ActionController& actionController );
    virtual ~EventTopWidget();
    //@}

public slots:
    //! @name Slots
    //@{
    void SetBeginDateTime( const QDateTime& dateTime );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnBeginDateTimeChanged( const QDateTime& dateTime );
    void OnHasEndTimeChanged( int state );
    void OnBeginDateTimeSelected();
    void OnEndDateTimeSelected();
    //@}

private:
    //! @name EventWidget_ABC implementation
    //@{
    virtual void Fill( const Event& event );
    virtual void Commit( timeline::Event& event ) const;
    //@}

    //! @name kernel::ContextMenuObserver_ABC< QDateTime >
    //@{
    virtual void NotifyContextMenu( const QDateTime& dateTime, kernel::ContextMenu& menu );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Time_ABC& simulation_;
    kernel::ActionController& actionController_;

    QLabel* title_;
    QLabel* source_;
    QDateTime selectedDateTime_;

    gui::RichDateTimeEdit* beginDateTimeEdit_;
    QCheckBox* hasEndDateTimeCheckbox_;
    gui::RichDateTimeEdit* endDateTimeEdit_;
    //@}
};

#endif // __EventTopWidget_h_
